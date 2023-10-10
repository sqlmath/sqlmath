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
    _dbCall
    _dbFileLoadOrSave
    _jsbatonGetString
    _sqlite3_errmsg
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

function isExternalBuffer(buf) {

// This function will check if <buf> is ArrayBuffer or SharedArrayBuffer.

    return (
        buf
        && (
            buf.constructor === ArrayBuffer
            || (
                typeof SharedArrayBuffer === "function"
                && buf.constructor === SharedArrayBuffer
            )
        )
    );
}

function noop(val) {

// This function will do nothing except return <val>.

    return val;
}

(async function () {
    let JSBATON_ARGC = 16;
    let JSBATON_OFFSET_ALL = 512;
    let JSBATON_OFFSET_ARG0 = 1;
    let JSBATON_OFFSET_ARGV = 128;
    //
    let FILENAME_DBTMP = "/tmp/__dbtmp1"; //jslint-ignore-line
    let jsbatonGetErrmsg;
    let onModulePostRun;

    function messageDispatch(data) {
        let argList = data["argList"].slice(JSBATON_OFFSET_ARG0);
        let baton = new Uint8Array(data["baton"] && data["baton"].buffer);
        let batonPtr = 0;
        let dbData = argList[4];
        let dbPtr = 0;
        let errcode = 0;
        let errmsg = "";
        let filename;
        let funcname = data["funcname"];
        let id = data.id;
        let modeSave = argList[2];
        switch (funcname) {
        case "_dbClose":
        case "_dbExec":
        case "_dbFileLoad":
        case "_dbNoop":
        case "_dbOpen":
            // copy baton to wasm
            batonPtr = _sqlite3_malloc(baton.byteLength);
            data["batonPtr"] = batonPtr;
            HEAPU8.set(baton, batonPtr);
            switch (funcname) {
            case "_dbClose":
                filename = _jsbatonGetString(batonPtr, 1);
                console.error(`_dbClose("${filename}")`);
                break;
            case "_dbFileLoad":
                // import dbData
                if (!modeSave) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                }
                break;
            case "_dbOpen":
                filename = _jsbatonGetString(batonPtr, 0);
                console.error(`_dbOpen("${filename}")`);
                break;
            }
            // call c-function
            _dbCall(batonPtr);
            // init errmsg
            errmsg = jsbatonGetErrmsg(batonPtr);
            // update baton
            baton.set(HEAPU8.subarray(batonPtr, batonPtr + JSBATON_OFFSET_ALL));
            baton = new BigInt64Array(baton.buffer, JSBATON_OFFSET_ARGV);
            baton.subarray(
                JSBATON_ARGC,
                2 * JSBATON_ARGC
            ).forEach(function (ptr, ii) {
                // ignore ArrayBuffer
                if (isExternalBuffer(argList[ii])) {
                    return;
                }
                ptr = Number(ptr);
                // init argList[ii] = argv[ii]
                if (ptr === 0) {
                    argList[ii] = baton[ii];
                // init argList[ii] = bufv[ii]
                } else {
                    argList[ii] = new ArrayBuffer(Number(baton[ii]));
                    new Uint8Array(argList[ii]).set(
                        HEAPU8.subarray(ptr, ptr + argList[ii].byteLength)
                    );
                    // cleanup ptr
                    _sqlite3_free(ptr);
                }
            });
            switch (!errmsg && funcname) {
            case "_dbFileLoad":
                // export dbData
                if (modeSave) {
                    dbData = FS.readFile(FILENAME_DBTMP);
                    argList[0] = dbData.buffer;
                }
                break;
            case "_dbOpen":
                // import dbData
                if (dbData) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                    dbPtr = Number(argList[0]);
                    errcode = _dbFileLoadOrSave(dbPtr, FILENAME_DBTMP, 0);
                    if (errcode) {
                        errmsg = _sqlite3_errmsg(dbPtr);
                    }
                }
                break;
            }
            baton = new DataView(baton.buffer);
            // prepend baton to argList
            argList.unshift(baton);
            postMessage(
                {
                    "argList": argList,
                    "errmsg": errmsg,
                    "funcname": funcname,
                    "id": id
                },
                // transfer arraybuffer without copying
                [baton.buffer, ...argList.filter(isExternalBuffer)]
            );
            return;
        }
        throw new Error(`invalid funcname "${funcname}"`);
    }

    // init event-handling
    globalThis["onmessage"] = async function ({
        data
    }) {
        let funcname = data["funcname"];
        function cleanup() {
            // cleanup batonPtr
            _sqlite3_free(data["batonPtr"]);
            // cleanup FILENAME_DBTMP
            switch (funcname) {
            case "_dbFileLoad":
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
    jsbatonGetErrmsg = cwrap("jsbatonGetErrmsg", "string", ["number"]);

    // init sqlite
    _sqlite3_initialize();
}());
