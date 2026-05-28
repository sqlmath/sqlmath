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
    _jsbatonGetInt64
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

// This function will check if <buf> is ArrayBuffer.

    return buf && buf.constructor === ArrayBuffer;
}

function noop(val) {

// This function will do nothing except return <val>.

    return val;
}

(async function () {
    // cwrap string-based c-functions
    let dbFileLoadOrSave = cwrap("dbFileLoadOrSave", "number", [
        "number", "string", "number"
    ]);
    let jsbatonGetErrmsg = cwrap("jsbatonGetErrmsg", "string", ["number"]);
    let jsbatonGetString = cwrap("jsbatonGetString", "string", [
        "number", "number"
    ]);
    let sqlite3_errmsg = cwrap("sqlite3_errmsg", "string", ["number"]);

    let onModulePostRun; //jslint-ignore-line

    function messageDispatch(data) {
        let FILENAME_DBTMP = data["FILENAME_DBTMP"];
        let JSBATON_OFFSET_ALL = data["JSBATON_OFFSET_ALL"];
        let JSBATON_OFFSET_BUFV = data["JSBATON_OFFSET_BUFV"];
        let argList = data["argList"];
        let baton = new Uint8Array(data["baton"].buffer);
        let batonPtr = 0;
        let dbData = argList[4];
        let errcode = 0;
        let errmsg = "";
        let funcname = data["funcname"];
        let modeSave = argList[2];
        switch (funcname) {
        case "_dbClose":
        case "_dbExec":
        case "_dbFileLoad":
        case "_dbNoop":
        case "_dbOpen":
            // copy baton to wasm-memory
            batonPtr = _sqlite3_malloc(baton.byteLength);
            data["batonPtr"] = batonPtr;
            HEAPU8.set(baton, batonPtr);
            switch (funcname) {
            case "_dbClose":
                console.error(`_dbClose("${jsbatonGetString(batonPtr, 1)}")`);
                break;
            case "_dbFileLoad":
                // load dbData
                if (!modeSave) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                }
                break;
            case "_dbOpen":
                console.error(`_dbOpen("${jsbatonGetString(batonPtr, 0)}")`);
                break;
            }
            // call c-function
            _dbCall(batonPtr);
            // init errmsg
            errmsg = jsbatonGetErrmsg(batonPtr);
            // update baton from wasm-memory
            baton.set(HEAPU8.subarray(batonPtr, batonPtr + JSBATON_OFFSET_ALL));
            switch (!errmsg && funcname) {
            case "_dbExec":
                dbData = new BigInt64Array(baton.buffer, JSBATON_OFFSET_BUFV);
                argList[0] = HEAPU8.slice(
                    Number(dbData[0]),
                    Number(dbData[0] + dbData[1])
                ).buffer;
                _sqlite3_free(Number(dbData[0]));
                break;
            case "_dbFileLoad":
                // save dbData
                if (modeSave) {
                    dbData = FS.readFile(FILENAME_DBTMP);
                    argList[0] = dbData.buffer;
                    FS.unlink(FILENAME_DBTMP);
                }
                break;
            case "_dbOpen":
                // open dbData
                if (dbData) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                    dbData = Number(_jsbatonGetInt64(batonPtr, 0));
                    errcode = dbFileLoadOrSave(dbData, FILENAME_DBTMP, 0);
                    if (errcode) {
                        errmsg = sqlite3_errmsg(dbData);
                    }
                    FS.unlink(FILENAME_DBTMP);
                }
                break;
            }
            postMessage(
                {
                    "argList": argList,
                    "baton": new DataView(baton.buffer),
                    "errmsg": errmsg,
                    "funcname": funcname,
                    "id": data["id"]
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
        await onModulePostRun;
        // reset batonPtr
        data["batonPtr"] = 0;
        try {
            await messageDispatch(data);
        } catch (err) {
            postMessage({
                "errmsg": err.stack,
                "id": data["id"]
            });
        } finally {
            // cleanup batonPtr
            _sqlite3_free(data["batonPtr"]);
        }
    };

    // init Module
    onModulePostRun = new Promise(function (resolve) {
        Module["postRun"] = resolve;
    });
    await onModulePostRun;

    // init sqlite
    _sqlite3_initialize();
}());
