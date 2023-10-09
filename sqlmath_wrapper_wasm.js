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


let JSBATON_ARGC = 16;
let JSBATON_OFFSET_ALL = 768;
let JSBATON_OFFSET_ARG0 = 2;
let JSBATON_OFFSET_ARGV = 8;
let JSBATON_OFFSET_BUFV = 136;
let JSBATON_OFFSET_CFUNCNAME = 552;
let JS_MAX_SAFE_INTEGER = 0x1fffffffffffff;
let JS_MIN_SAFE_INTEGER = -0x1fffffffffffff;
let SIZEOF_BLOB_MAX = 1000000000;
let SIZEOF_CFUNCNAME = 24;
let SIZEOF_MESSAGE = 256;
let SQLITE_DATATYPE_BLOB = 0x04;
let SQLITE_DATATYPE_FLOAT = 0x02;
let SQLITE_DATATYPE_INTEGER = 0x01;
let SQLITE_DATATYPE_INTEGER_0 = 0x00;
let SQLITE_DATATYPE_INTEGER_1 = 0x21;
let SQLITE_DATATYPE_NULL = 0x05;
let SQLITE_DATATYPE_SHAREDARRAYBUFFER = 0x71;
let SQLITE_DATATYPE_TEXT = 0x03;
let SQLITE_DATATYPE_TEXT_0 = 0x13;
let SQLITE_RESPONSETYPE_LASTBLOB = 1;


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
    let FILENAME_DBTMP = "/tmp/__dbtmp1";
    let __dbFileImportOrExport;
    let jsbatonGetErrmsg;
    let jsbatonGetString;
    let onModulePostRun;
    let sqlite3_errmsg;

    function messageDispatch(data) {
        let argList = data["argList"];
        let baton = new Uint8Array(data["baton"] && data["baton"].buffer);
        let batonPtr = 0;
        let cfuncname;
        let dbData = argList[4];
        let dbPtr = 0;
        let errcode = 0;
        let errmsg = "";
        let filename;
        let id = data.id;
        let modeExport = argList[2];
        cfuncname = new TextDecoder().decode(
            baton.subarray(
                JSBATON_OFFSET_CFUNCNAME,
                JSBATON_OFFSET_CFUNCNAME + SIZEOF_CFUNCNAME - 1
            )
        ).replace((/\u0000/g), "");
        switch (cfuncname) {
        case "_dbClose":
        case "_dbExec":
        case "_dbFileImportOrExport":
        case "_dbNoop":
        case "_dbOpen":
            // copy baton to wasm
            batonPtr = _sqlite3_malloc(baton.byteLength);
            data["batonPtr"] = batonPtr;
            HEAPU8.set(baton, batonPtr);
            switch (cfuncname) {
            case "_dbClose":
                filename = jsbatonGetString(batonPtr, 1);
                console.error(`_dbClose("${filename}")`);
                break;
            case "_dbFileImportOrExport":
                // import dbData
                if (!modeExport) {
                    FS.writeFile(FILENAME_DBTMP, new Uint8Array(dbData));
                }
                break;
            case "_dbOpen":
                filename = jsbatonGetString(batonPtr, 0);
                console.error(`_dbOpen("${filename}")`);
                break;
            }
            // call c-function
            Module["_dbCall"](batonPtr);
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
            switch (!errmsg && cfuncname) {
            case "_dbFileImportOrExport":
                // export dbData
                if (modeExport) {
                    dbData = FS.readFile(FILENAME_DBTMP);
                    argList[0] = dbData.buffer;
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
                    "cfuncname": cfuncname,
                    "errmsg": errmsg,
                    "id": id
                },
                // transfer arraybuffer without copying
                [baton.buffer, ...argList.filter(isExternalBuffer)]
            );
            return;
        }
        throw new Error(`invalid cfuncname "${cfuncname}"`);
    }

    // init event-handling
    globalThis["onmessage"] = async function ({
        data
    }) {
        let cfuncname = data["cfuncname"];
        function cleanup() {
            // cleanup batonPtr
            _sqlite3_free(data["batonPtr"]);
            // cleanup FILENAME_DBTMP
            switch (cfuncname) {
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
        ["number", "string", "number"]
    );
    jsbatonGetErrmsg = cwrap("jsbatonGetErrmsg", "string", ["number"]);
    jsbatonGetString = cwrap(
        "jsbatonGetString",
        "string",
        ["number", "number"]
    );
    sqlite3_errmsg = cwrap("sqlite3_errmsg", "string", ["number"]);

    // init sqlite
    _sqlite3_initialize();
}());
