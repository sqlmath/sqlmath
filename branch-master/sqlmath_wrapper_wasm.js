// The MIT License (MIT)
//
// Copyright (c) 2021 Kai Zhu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


/*jslint beta, browser, devel, nomen, subscript*/
/*global
    AsciiToString
    FS
    HEAPU8
    Module
    _sqlite3_free
    _sqlite3_initialize
    _sqlite3_malloc
    cwrap
*/


// init debugInline
let debugInline = (function () {
    let __consoleError = function () {
        return;
    };
    function debug(...argv) {

// This function will print <argv> to stderr and then return <argv>[0].

        __consoleError("\n\ndebugInline");
        __consoleError(...argv);
        __consoleError("\n");
        return argv[0];
    }
    debug(); // Coverage-hack.
    __consoleError = console.error; //jslint-ignore-line
    return debug;
}());

function noop(val) {

// This function will do nothing except return <val>.

    return val;
}

(async function () {
    let FILENAME_DBTMP = "/tmp/__dbtmp1";
    let JSBATON_ARGC = 16;
    let __dbFileImportOrExport;
    let jsbatonValueErrmsg;
    let jsbatonValueStringArgi;
    let onModulePostRun;
    let sqlite3_errmsg;

    function messageDispatch(data) {
        let argList = data["argList"];
        let baton = new Uint8Array(data["baton"] && data["baton"].buffer);
        let batonPtr = 0;
        let cFuncName = data["cFuncName"];
        let dbData = argList[4];
        let dbPtr = 0;
        let errcode = 0;
        let errmsg = "";
        let filename;
        let id = data.id;
        let modeExport = argList[2];
        switch (cFuncName) {
        case "_dbClose":
        case "_dbExec":
        case "_dbFileImportOrExport":
        case "_dbNoop":
        case "_dbOpen":
            // copy baton to wasm
            batonPtr = _sqlite3_malloc(baton.byteLength);
            data["batonPtr"] = batonPtr;
            HEAPU8.set(baton, batonPtr);
            switch (cFuncName) {
            case "_dbClose":
                filename = jsbatonValueStringArgi(batonPtr, 1);
                console.error(`_dbClose("${filename}")`);
                break;
            case "_dbFileImportOrExport":
                // import dbData
                if (!modeExport) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                }
                break;
            case "_dbOpen":
                filename = jsbatonValueStringArgi(batonPtr, 0);
                console.error(`_dbOpen("${filename}")`);
                break;
            }
            // call c-function
            Module[cFuncName](batonPtr);
            // init errmsg
            errmsg = jsbatonValueErrmsg(batonPtr);
            // update baton
            baton.set(new Uint8Array(
                HEAPU8.buffer,
                HEAPU8.byteOffset + batonPtr,
                1024
            ));
            baton = new BigInt64Array(baton.buffer, 4 + 4);
            baton.subarray(
                JSBATON_ARGC,
                2 * JSBATON_ARGC
            ).forEach(function (ptr, ii) {
                // ignore ArrayBuffer
                if (argList[ii] && (
                    argList[ii].constructor === ArrayBuffer
                    || (
                        typeof SharedArrayBuffer === "function"
                        && argList[ii].constructor === SharedArrayBuffer
                    )
                )) {
                    return;
                }
                ptr = Number(ptr);
                // init argList[ii] = argv[ii]
                if (ptr === 0) {
                    argList[ii] = baton[ii];
                // init argList[ii] = bufv[ii]
                } else {
                    argList[ii] = new ArrayBuffer(
                        Number(baton[ii])
                    );
                    new Uint8Array(argList[ii]).set(
                        HEAPU8.subarray(ptr, ptr + argList[ii].byteLength)
                    );
                    // cleanup ptr
                    _sqlite3_free(ptr);
                }
            });
            switch (!errmsg && cFuncName) {
            case "_dbFileImportOrExport":
                // export dbData
                if (modeExport) {
                    dbData = FS.readFile(FILENAME_DBTMP);
                    argList[4] = dbData.buffer;
                }
                break;
            case "_dbOpen":
                // import dbData
                if (dbData) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                    dbPtr = Number(argList[0]);
                    errcode = __dbFileImportOrExport(dbPtr, FILENAME_DBTMP, 0);
                    if (errcode) {
                        errmsg = sqlite3_errmsg(dbPtr);
                    }
                }
                break;
            }
            postMessage(
                {
                    "argList": argList,
                    "baton": new DataView(baton.buffer),
                    "cFuncName": cFuncName,
                    "errmsg": errmsg,
                    "id": id
                },
                // transfer arraybuffer without copying
                [
                    baton.buffer,
                    ...argList.filter(function (elem) {
                        return elem && elem.constructor === ArrayBuffer;
                    })
                ]
            );
            return;
        }
        throw new Error(`invalid cFuncName "${cFuncName}"`);
    }

    // init event-handling
    globalThis["onmessage"] = async function ({
        data
    }) {
        let cFuncName = data["cFuncName"];
        function cleanup() {
            // cleanup batonPtr
            _sqlite3_free(data["batonPtr"]);
            // cleanup FILENAME_DBTMP
            switch (cFuncName) {
            case "_dbFileImportOrExport":
            case "_dbOpen":
                try {
                    FS.unlink(FILENAME_DBTMP);
                } catch (ignore) {}
                break;
            }
        }
        await onModulePostRun;
        // reset batonPtr
        data["batonPtr"] = 0;
        try {
            await messageDispatch(data);
        } catch (err) {
            postMessage({
                "errmsg": err.stack,
                "id": data.id
            });
        } finally {
            cleanup();
        }
    };

    // init Module
    onModulePostRun = new Promise(function (resolve) {
        Module["postRun"] = resolve;
    });
    await onModulePostRun;
    __dbFileImportOrExport = cwrap(
        "__dbFileImportOrExport",
        "number",
        [
            "number", "string", "number"
        ]
    );
    jsbatonValueErrmsg = cwrap("jsbatonValueErrmsg", "string", [
        "number"
    ]);
    jsbatonValueStringArgi = cwrap("jsbatonValueStringArgi", "string", [
        "number", "number"
    ]);
    sqlite3_errmsg = cwrap("sqlite3_errmsg", "string", [
        "number"
    ]);

    // init sqlite
    _sqlite3_initialize();
}());
