// MIT License
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


/*jslint beta, bitwise, name, node*/
/*global FinalizationRegistry*/
"use strict";

let FILENAME_DBTMP = "/tmp/__dbtmp1";
let IS_BROWSER;

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

let SQLITE_OPEN_AUTOPROXY = 0x00000020;     /* VFS only */ //jslint-ignore-line
let SQLITE_OPEN_CREATE = 0x00000004;        /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_DELETEONCLOSE = 0x00000008; /* VFS only */
let SQLITE_OPEN_EXCLUSIVE = 0x00000010;     /* VFS only */
let SQLITE_OPEN_FULLMUTEX = 0x00010000;     /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_MAIN_DB = 0x00000100;       /* VFS only */
let SQLITE_OPEN_MAIN_JOURNAL = 0x00000800;  /* VFS only */
let SQLITE_OPEN_MEMORY = 0x00000080;        /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_NOFOLLOW = 0x01000000;      /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_NOMUTEX = 0x00008000;       /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_PRIVATECACHE = 0x00040000;  /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_READONLY = 0x00000001;      /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_READWRITE = 0x00000002;     /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_SHAREDCACHE = 0x00020000;   /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_SUBJOURNAL = 0x00002000;    /* VFS only */
let SQLITE_OPEN_SUPER_JOURNAL = 0x00004000; /* VFS only */
let SQLITE_OPEN_TEMP_DB = 0x00000200;       /* VFS only */
let SQLITE_OPEN_TEMP_JOURNAL = 0x00001000;  /* VFS only */
let SQLITE_OPEN_TRANSIENT_DB = 0x00000400;  /* VFS only */
let SQLITE_OPEN_URI = 0x00000040;           /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_WAL = 0x00080000;           /* VFS only */
let cModule;
let cModulePath;
let consoleError = console.error;
let dbDict = new WeakMap(); // private-dict of sqlite-database-connections
let dbFinalizationRegistry;
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
let moduleChildProcess;
let moduleChildProcessSpawn;
let moduleFs;
let moduleFsInitResolveList;
let modulePath;
let moduleUrl;
let {
    npm_config_mode_debug,
    npm_config_mode_setup,
    npm_config_mode_test
} = typeof process === "object" && process?.env;
let sqlMessageDict = {}; // dict of web-worker-callbacks
let sqlMessageId = 0;
let sqlWorker;
let version = "v2023.10.1-beta";

function assertInt64(val) {
    // This function will assert <val> is within range of c99-signed-long-long.
    val = BigInt(val);
    if (!(
        -9_223_372_036_854_775_808n <= val && val <= 9_223_372_036_854_775_807n
    )) {
        throw new Error(
            `integer ${val} outside signed-64-bit inclusive-range`
            + " -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807"
        );
    }
}

function assertJsonEqual(aa, bb, message) {

// This function will assert JSON.stringify(<aa>) === JSON.stringify(<bb>).

    aa = JSON.stringify(objectDeepCopyWithKeysSorted(aa), undefined, 1);
    bb = JSON.stringify(objectDeepCopyWithKeysSorted(bb), undefined, 1);
    if (aa !== bb) {
        throw new Error(
            "\n" + aa + "\n!==\n" + bb
            + (
                typeof message === "string"
                ? " - " + message
                : message
                ? " - " + JSON.stringify(message)
                : ""
            )
        );
    }
}

function assertNumericalEqual(aa, bb, message) {

// This function will assert aa - bb <= Number.EPSILON.

    assertOrThrow(aa, "value cannot be 0 or falsy");
    if (!(Math.abs((aa - bb) / Math.max(aa, bb)) <= 256 * Number.EPSILON)) {
        throw new Error(
            JSON.stringify(aa) + " != " + JSON.stringify(bb) + (
                message
                ? " - " + message
                : ""
            )
        );
    }
}

function assertOrThrow(condition, message) {

// This function will throw <message> if <condition> is falsy.

    if (!condition) {
        throw (
            (!message || typeof message === "string")
            ? new Error(String(message).slice(0, 2048))
            : message
        );
    }
}

async function cCallAsync(baton, cFuncName, ...argList) {

// This function will serialize <argList> to a c <baton>,
// suitable for passing into napi.

    let errStack;
    assertOrThrow(
        argList.length <= JSBATON_ARGC,
        `cCallAsync - argList.length must be less than than ${JSBATON_ARGC}`
    );
    // pad argList to length JSBATON_ARGC
    while (argList.length < JSBATON_ARGC) {
        argList.push(0n);
    }
    // serialize js-value to c-value
    argList = argList.map(function (val, argi) {
        switch (typeof val) {
        case "bigint":
        case "boolean":
        case "number":
            // check for min/max safe-integer
            assertOrThrow(
                (
                    (JS_MIN_SAFE_INTEGER <= val && val <= JS_MAX_SAFE_INTEGER)
                    || typeof val !== "number"
                ),
                (
                    "non-bigint integer must be within inclusive-range"
                    + ` ${JS_MIN_SAFE_INTEGER} to ${JS_MAX_SAFE_INTEGER}`
                )
            );
            val = BigInt(val);
            assertInt64(val);
            baton.setBigInt64(JSBATON_OFFSET_ARGV + argi * 8, val, true);
            return;
        // case "object":
        //     break;
        case "string":
            baton = jsbatonValuePush(baton, argi, (
                val.endsWith("\u0000")
                ? val
                // append null-terminator to string
                : val + "\u0000"
            ));
            return;
        }
        // normalize buffer to zero-byte-offset
        if (ArrayBuffer.isView(val)) {
            return new DataView(val.buffer, val.byteOffset, val.byteLength);
        }
        if (isExternalBuffer(val)) {
            return val;
        }
    });
    //!! // encode cFuncName into baton
    baton = jsbatonValuePush(baton, 2 * JSBATON_ARGC, `${cFuncName}\u0000`);
    // prepend baton, cFuncName to argList
    argList = [baton, cFuncName, ...argList];
    // preserve stack-trace
    errStack = new Error().stack.replace((/.*$/m), "");
    try {
        return (
            IS_BROWSER
            ? await sqlMessagePost(...argList)
            : await cModule._jspromiseCreate(argList)
        );
    } catch (err) {
        err.stack += errStack;
        assertOrThrow(undefined, err);
    }
}

async function childProcessSpawn2(command, args, option) {

// This function will run child_process.spawn as a promise.

    return await new Promise(function (resolve, reject) {
        let bufList = [[], [], []];
        let child;
        let {
            modeCapture,
            modeDebug,
            stdio
        } = option;
        if (modeDebug) {
            consoleError(
                `childProcessSpawn2 - ${command} ${JSON.stringify(args)}`
            );
        }
        child = moduleChildProcessSpawn(
            command,
            args,
            Object.assign({}, option, {
                stdio: [
                    "ignore",
                    (
                        modeCapture
                        ? "pipe"
                        : stdio[1]
                    ),
                    (
                        modeCapture
                        ? "pipe"
                        : stdio[2]
                    )
                ]
            })
        );
        if (modeCapture) {
            [
                child.stdin, child.stdout, child.stderr
            ].forEach(function (pipe, ii) {
                if (ii === 0) {
                    return;
                }
                pipe.on("data", function (chunk) {
                    bufList[ii].push(chunk);
                    if (stdio[ii] !== "ignore") {
                        switch (ii) {
                        case 1:
                            process.stdout.write(chunk);
                            break;
                        case 2:
                            process.stderr.write(chunk);
                            break;
                        }
                    }
                });
            });
        }
        child.on("exit", function (exitCode) {
            let resolve0 = resolve;
            let stderr;
            let stdout;
            // coverage-hack
            if (exitCode || npm_config_mode_test) {
                resolve = reject;
            }
            // coverage-hack
            if (npm_config_mode_test) {
                resolve = resolve0;
            }
            [stdout, stderr] = bufList.slice(1).map(function (buf) {
                return (
                    typeof modeCapture === "string"
                    ? Buffer.concat(buf).toString(modeCapture)
                    : Buffer.concat(buf)
                );
            });
            resolve({exitCode, stderr, stdout});
        });
    });

}

async function ciBuildExt({
    modeSkip,
    process
}) {

// This function will build sqlmath from c.

    let option;
    option = {
        binNodegyp: modulePath.resolve(
            modulePath.dirname(process.execPath || ""),
            "node_modules/npm/node_modules/node-gyp/bin/node-gyp.js"
        ).replace("/bin/node_modules/", "/lib/node_modules/"),
        isWin32: process.platform === "win32",
        modeDebug: npm_config_mode_debug,
        modeSkip,
        process
    };
    await ciBuildExt2NodejsBuild(option);
}

async function ciBuildExt1NodejsConfigure({
    binNodegyp,
    modeDebug
}) {

// This function will setup posix/win32 env for building c-extension.

    let cflagsNowarning = [
        "-Wno-all",
        "-Wno-extra",
        "-Wno-implicit-fallthrough",
        "-Wno-int-conversion",
        "-Wno-unused-parameter"
    ];
    consoleError(`ciBuildExt1Nodejs - configure binding.gyp`);
    await fsWriteFileUnlessTest("binding.gyp", JSON.stringify({
        "target_defaults": {
            "cflags": ["-Wextra", "-std=c11"],
            "conditions": [
                [
                    "OS == \u0027win\u0027",
                    {"defines": ["WIN32"]}
                ]
            ],
// https://github.com/nodejs/node-gyp/blob/v9.3.1/gyp/pylib/gyp/MSVSSettings.py
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "WarningLevel": 3
                }
            },
            "xcode_settings": {"OTHER_CFLAGS": ["-Wextra", "-std=c11"]}
        },
        "targets": [
            {
                "cflags": cflagsNowarning,
                "sources": [
                    "build/SRC_ZLIB_BASE.c",
                    "build/SRC_SQLITE_BASE.c",
                    "build/SQLMATH_BASE.c"
                ],
                "target_name": "SRC_SQLITE_BASE",
                "type": "static_library",
                "xcode_settings": {"OTHER_CFLAGS": cflagsNowarning}
            },
            {
                "sources": [
                    "build/SQLMATH_CUSTOM.c"
                ],
                "target_name": "SQLMATH_CUSTOM",
                "type": "static_library"
            },
            {
                "defines": ["SQLMATH_NODEJS_C2"],
                "dependencies": [
                    "SQLMATH_CUSTOM",
                    "SRC_SQLITE_BASE"
                ],
                "sources": [
                    "sqlmath_base.c"
                ],
                "target_name": "binding"
            },
            {
                "cflags": cflagsNowarning,
                "defines": [
                    "SQLITE3_SHELL_C2"
                ],
                "dependencies": [
                    "SQLMATH_CUSTOM",
                    "SRC_SQLITE_BASE"
                ],
                "sources": [
                    "sqlmath_base.c",
                    "build/SRC_SQLITE_SHELL.c"
                ],
                "target_name": "shell",
                "type": "executable",
                "xcode_settings": {"OTHER_CFLAGS": cflagsNowarning}
            }
        ]
    }, undefined, 4) + "\n");
    await childProcessSpawn2(
        "sh",
        [
            "-c",
            (`
(set -e
    # node "${binNodegyp}" clean
    node "${binNodegyp}" configure
)
            `)
        ],
        {modeDebug, stdio: ["ignore", 1, 2]}
    );
}

async function ciBuildExt2NodejsBuild({
    binNodegyp,
    modeDebug
}) {

// This function will archive <fileObj> into <fileLib>

    if (!noop(
        await fsExistsUnlessTest(cModulePath)
    )) {
        await ciBuildExt1NodejsConfigure({
            binNodegyp,
            modeDebug
        });
    }
    consoleError(
        `ciBuildExt2Nodejs - linking lib ${modulePath.resolve(cModulePath)}`
    );
    await childProcessSpawn2(
        "sh",
        [
            "-c",
            (`
(set -e
    # rebuild binding
    rm -rf build/Release/obj/SQLMATH_CUSTOM/
    node "${binNodegyp}" build --release
    mv build/Release/binding.node "${cModulePath}"
    if [ "${process.platform}" = "win32" ]
    then
        mv build/Release/shell \
            "_sqlmath.shell_${process.platform}_${process.arch}.exe"
    else
        mv build/Release/shell \
            "_sqlmath.shell_${process.platform}_${process.arch}"
    fi
)
            `)
        ],
        {modeDebug, stdio: ["ignore", 1, 2]}
    );
}

function dbCallAsync(baton, cFuncName, db, ...argList) {

// This function will call <cFuncName> using db <argList>[0].

    let __db = dbDeref(db);
    // increment __db.busy
    __db.busy += 1;
    return cCallAsync(
        baton,
        cFuncName,
        __db.ptr,
        ...argList
    ).finally(function () {
        // decrement __db.busy
        __db.busy -= 1;
        assertOrThrow(__db.busy >= 0, `invalid __db.busy ${__db.busy}`);
    });
}

async function dbCloseAsync(db) {

// This function will close sqlite-database-connection <db>.

    let __db = dbDeref(db);
    // prevent segfault - do not close db if actions are pending
    assertOrThrow(
        __db.busy === 0,
        `dbCloseAsync - cannot close with ${__db.busy} actions pending`
    );
    // cleanup connPool
    await Promise.all(__db.connPool.map(async function (ptr) {
        let val = ptr[0];
        ptr[0] = 0n;
        await cCallAsync(
            jsbatonCreate("_dbClose"),
            "_dbClose",
            val,
            __db.filename
        );
    }));
    dbDict.delete(db);
}

function dbDeref(db) {

// This function will get private-object mapped to <db>.

    let __db = dbDict.get(db);
    assertOrThrow(__db?.connPool[0] > 0, "invalid or closed db");
    assertOrThrow(__db.busy >= 0, "invalid db.busy " + __db.busy);
    __db.ii = (__db.ii + 1) % __db.connPool.length;
    __db.ptr = __db.connPool[__db.ii][0];
    assertOrThrow(__db.ptr > 0n, "invalid or closed db");
    return __db;
}

function dbExecAndReturnLastBlobAsync({
    bindList = [],
    db,
    sql
}) {

// This function will exec <sql> in <db> and return last value retrieved
// from execution as raw blob/buffer.

    return dbExecAsync({
        bindList,
        db,
        responseType: "lastBlob",
        sql
    });
}

async function dbExecAsync({
    bindList = [],
    db,
    modeRetry,
    responseType,
    sql
}) {

// This function will exec <sql> in <db> and return <result>.

    let baton;
    let bindByKey;
    let bindListLength;
    let externalbufferList;
    let result;
    while (modeRetry > 0) {
        try {
            return await dbExecAsync({
                bindList,
                db,
                responseType,
                sql
            });
        } catch (err) {
            assertOrThrow(modeRetry > 0, err);
            consoleError(err);
            consoleError(
                "dbExecAsync - retry failed sql-query with "
                + modeRetry
                + " remaining retries"
            );
            modeRetry -= 1;
            await new Promise(function (resolve) {
                setTimeout(resolve, 5_000 * !npm_config_mode_test);
            });
        }
    }
    baton = jsbatonCreate("_dbExec");
    bindByKey = !Array.isArray(bindList);
    bindListLength = (
        Array.isArray(bindList)
        ? bindList.length
        : Object.keys(bindList).length
    );
    externalbufferList = [];
    Object.entries(bindList).forEach(function ([key, val]) {
        if (bindByKey) {
            baton = jsbatonValuePush(baton, undefined, `:${key}\u0000`);
        }
        baton = jsbatonValuePush(baton, undefined, val, externalbufferList);
    });
    result = await dbCallAsync(
        baton,
        "_dbExec",
        db,                     // 0
        String(sql) + "\n;\nPRAGMA noop",   // 1
        bindListLength,         // 2
        bindByKey,              // 3
        (                       // 4
            responseType === "lastBlob"
            ? SQLITE_RESPONSETYPE_LASTBLOB
            : 0
        ),
        undefined, // 5
        undefined, // 6
        undefined, // 7 - response
        ...externalbufferList        // 8
    );
    result = result[JSBATON_OFFSET_ARG0 + 0];
    switch (responseType) {
    case "arraybuffer":
    case "lastBlob":
        return result;
    case "list":
        return JSON.parse(new TextDecoder().decode(result));
    default:
        result = JSON.parse(new TextDecoder().decode(result));
        return result.map(function (rowList) {
            let colList = rowList.shift();
            return rowList.map(function (row) {
                let dict = {};
                colList.forEach(function (key, ii) {
                    dict[key] = row[ii];
                });
                return dict;
            });
        });
    }
}

async function dbFileExportAsync({
    db,
    dbData,
    filename,
    modeExport = 1,
    modeNoop
}) {

// This function will export <db> to <filename>.

    if (modeNoop) {
        return;
    }
    if (IS_BROWSER) {
        filename = FILENAME_DBTMP;
    }
    assertOrThrow(
        typeof filename === "string" && filename,
        `invalid filename ${filename}`
    );
    return await dbCallAsync(
        jsbatonCreate("_dbFileImportOrExport"),
        "_dbFileImportOrExport",
        db,                     // 0. sqlite3 * pInMemory,
        String(filename),       // 1. char *zFilename,
        modeExport,             // 2. const int isSave
        undefined,              // 3. undefined
        dbData                  // 4. dbData
    );
}

async function dbFileImportAsync({
    db,
    dbData,
    filename
}) {

// This function will import <filename> to <db>.

    await dbFileExportAsync({
        db,
        dbData,
        filename,
        modeExport: 0
    });
}

async function dbNoopAsync(...argList) {

// This function will do nothing except return <argList>.

    return await cCallAsync(jsbatonCreate("_dbNoop"), "_dbNoop", ...argList);
}

async function dbOpenAsync({
    afterFinalization,
    dbData,
    filename,
    flags,
    threadCount = 1
}) {

// This function will open and return sqlite-database-connection <db>.

// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
    let connPool;
    let db = {filename};
    assertOrThrow(typeof filename === "string", `invalid filename ${filename}`);
    assertOrThrow(
        !dbData || isExternalBuffer(dbData),
        "dbData must be ArrayBuffer"
    );
    connPool = await Promise.all(Array.from(new Array(
        threadCount
    ), async function () {
        let ptr = await cCallAsync(
            jsbatonCreate("_dbOpen"),
            "_dbOpen",
            // 0. const char *filename,   Database filename (UTF-8)
            filename,
            // 1. sqlite3 **ppDb,         OUT: SQLite db handle
            undefined,
            // 2. int flags,              Flags
            flags ?? (
                SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
            ),
            // 3. const char *zVfs        Name of VFS module to use
            undefined,
            // 4. wasm-only - arraybuffer of raw sqlite-database to open in wasm
            dbData
        );
        ptr = [ptr[0].getBigInt64(JSBATON_OFFSET_ARGV + 0, true)];
        dbFinalizationRegistry.register(db, {afterFinalization, ptr});
        return ptr;
    }));
    dbDict.set(db, {busy: 0, connPool, filename, ii: 0});
    return db;
}

async function fsCopyFileUnlessTest(file1, file2, mode) {

// This function will copy <file1> to <file2> unless <npm_config_mode_test> = 1.

    if (npm_config_mode_test && mode !== "force") {
        return;
    }
    await moduleFs.promises.copyFile(file1, file2, mode | 0);
}

async function fsExistsUnlessTest(file, mode) {

// This function will test if <file> exists unless <npm_config_mode_test> = 1.

    if (npm_config_mode_test && mode !== "force") {
        return false;
    }
    try {
        await moduleFs.promises.access(file);
        return true;
    } catch (ignore) {
        return false;
    }
}

async function fsReadFileUnlessTest(file, mode, defaultData) {

// This function will read <data> from <file> unless <npm_config_mode_test> = 1.

    if (npm_config_mode_test && mode !== "force") {
        return defaultData;
    }
    return await moduleFs.promises.readFile(
        file,
        mode && mode.replace("force", "utf8")
    );
}

async function fsWriteFileUnlessTest(file, data, mode) {

// This function will write <data> to <file> unless <npm_config_mode_test> = 1.

    if (npm_config_mode_test && mode !== "force") {
        return;
    }
    await moduleFs.promises.writeFile(file, data);
}

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

function jsbatonCreate(cFuncName) {

// This function will create buffer <baton>.

    let baton = new DataView(new ArrayBuffer(1024));
    // init nallc, nused
    baton.setInt32(4, JSBATON_OFFSET_ALL, true);
    // copy cFuncName into baton
    new Uint8Array(
        baton.buffer,
        baton.byteOffset + JSBATON_OFFSET_CFUNCNAME,
        SIZEOF_CFUNCNAME - 1
    ).set(new TextEncoder().encode(cFuncName));
    return baton;
}

function jsbatonValuePush(baton, argi, val, externalbufferList) {

// This function will push <val> to buffer <baton>.

    let nn;
    let nused;
    let tmp;
    let vsize;
    let vtype;
/*
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x00
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       0x71
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
    //  1. 0.bigint
    //  2. 0.boolean
    //  3. 0.function
    //  4. 0.number
    //  5. 0.object
    //  6. 0.string
    //  7. 0.symbol
    //  8. 0.undefined
    //  9. 1.bigint
    // 10. 1.boolean
    // 11. 1.function
    // 12. 1.number
    // 13. 1.object
    // 14. 1.string
    // 15. 1.symbol
    // 16. 1.undefined
    // 17. 1.buffer
    // 18. 1.externalbuffer
*/
    // 10. 1.boolean
    if (val === 1 || val === 1n) {
        val = true;
    }
    switch (
        val
        ? "1." + typeof(val)
        : "0." + typeof(val)
    ) {
    //  1. 0.bigint
    case "0.bigint":
    //  2. 0.boolean
    case "0.boolean":
    //  4. 0.number
    case "0.number":
        vtype = SQLITE_DATATYPE_INTEGER_0;
        vsize = 0;
        break;
    //  3. 0.function
    // case "0.function":
    //  5. 0.object
    case "0.object":
    //  7. 0.symbol
    case "0.symbol":
    //  8. 0.undefined
    case "0.undefined":
    // 11. 1.function
    case "1.function":
    // 15. 1.symbol
    case "1.symbol":
    // 16. 1.undefined
    // case "1.undefined":
        vtype = SQLITE_DATATYPE_NULL;
        vsize = 0;
        break;
    //  6. 0.string
    case "0.string":
        vtype = SQLITE_DATATYPE_TEXT_0;
        vsize = 0;
        break;
    //  9. 1.bigint
    case "1.bigint":
        vtype = SQLITE_DATATYPE_INTEGER;
        vsize = 8;
        break;
    // 10. 1.boolean
    case "1.boolean":
        vtype = SQLITE_DATATYPE_INTEGER_1;
        vsize = 0;
        break;
    // 12. 1.number
    case "1.number":
        vtype = SQLITE_DATATYPE_FLOAT;
        vsize = 8;
        break;
    // 14. 1.string
    case "1.string":
        val = new TextEncoder().encode(val);
        vtype = SQLITE_DATATYPE_TEXT;
        vsize = 4 + val.byteLength;
        break;
    // 13. 1.object
    default:
        // 18. 1.externalbuffer
        if (isExternalBuffer(val)) {
            assertOrThrow(
                !IS_BROWSER,
                "external ArrayBuffer cannot be passed directly to wasm"
            );
            assertOrThrow(
                externalbufferList.length <= 8,
                "externalbufferList.length must be less than or equal to 8"
            );
            externalbufferList.push(new DataView(val));
            vtype = SQLITE_DATATYPE_SHAREDARRAYBUFFER;
            vsize = 4;
            break;
        }
        // 17. 1.buffer
        if (ArrayBuffer.isView(val)) {
            if (val.byteLength === 0) {
                vtype = SQLITE_DATATYPE_NULL;
                vsize = 0;
                break;
            }
            vtype = SQLITE_DATATYPE_BLOB;
            vsize = 4 + val.byteLength;
            break;
        }
        // 13. 1.object
        val = new TextEncoder().encode(
            typeof val.toJSON === "function"
            ? val.toJSON()
            : JSON.stringify(val)
        );
        vtype = SQLITE_DATATYPE_TEXT;
        vsize = 4 + val.byteLength;
    }
    nused = baton.getInt32(4, true);
    nn = nused + 1 + vsize;
    assertOrThrow(
        nn <= 0xffff_ffff,
        "jsbaton cannot exceed 0x7fff_ffff / 2,147,483,647 bytes"
    );
    // exponentially grow baton as needed
    if (baton.byteLength < nn) {
        tmp = baton;
        baton = new DataView(new ArrayBuffer(
            Math.min(2 ** Math.ceil(Math.log2(nn)), 0x7fff_ffff)
        ));
        // update nallc
        baton.setInt32(0, baton.byteLength, true);
        // copy old-baton into new-baton
        new Uint8Array(baton.buffer, baton.byteOffset, nused).set(
            new Uint8Array(tmp.buffer, tmp.byteOffset, nused)
        );
    }
    // push vtype
    baton.setUint8(nused, vtype);
    // update nused
    baton.setInt32(4, nused + 1 + vsize, true);
    // handle blob-value
    switch (vtype) {
    case SQLITE_DATATYPE_BLOB:
    case SQLITE_DATATYPE_TEXT:
        // set argv[ii] to blob/text location
        if (argi !== undefined) {
            baton.setInt32(JSBATON_OFFSET_ARGV + argi * 8, nused, true);
        }
        vsize -= 4;
        assertOrThrow(
            0 <= vsize && vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + ` 0 to ${SIZEOF_BLOB_MAX}`
            )
        );
        // push vsize
        baton.setInt32(nused + 1, vsize, true);
        // copy val into baton
        new Uint8Array(
            baton.buffer,
            baton.byteOffset + nused + 1 + 4,
            vsize
        ).set(new Uint8Array(val.buffer, val.byteOffset, vsize));
        break;
    case SQLITE_DATATYPE_FLOAT:
        baton.setFloat64(nused + 1, val, true);
        break;
    case SQLITE_DATATYPE_INTEGER:
        assertInt64(val);
        baton.setBigInt64(nused + 1, val, true);
        break;
    case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
        vsize = val.byteLength;
        assertOrThrow(
            0 <= vsize && vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + ` 0 to ${SIZEOF_BLOB_MAX}`
            )
        );
        // push vsize
        baton.setInt32(nused + 1, vsize, true);
        break;
    }
    return baton;
}

function jsbatonValueString(baton, argi) {

// This function will return string-value from <baton> at given <offset>.

    let offset = baton.getInt32(JSBATON_OFFSET_ARGV + argi * 8, true);
    return new TextDecoder().decode(new Uint8Array(
        baton.buffer,
        baton.byteOffset + offset + 1 + 4,
        // remove null-terminator from string
        baton.getInt32(offset + 1, true) - 1
    ));
}

async function moduleFsInit() {

// This function will import nodejs builtin-modules if they have not yet been
// imported.

// State 3 - Modules already imported.

    if (moduleFs !== undefined) {
        return;
    }

// State 2 - Wait while modules are importing.

    if (moduleFsInitResolveList !== undefined) {
        return new Promise(function (resolve) {
            moduleFsInitResolveList.push(resolve);
        });
    }

// State 1 - Start importing modules.

    moduleFsInitResolveList = [];
    [
        moduleChildProcess,
        moduleFs,
        modulePath,
        moduleUrl
    ] = await Promise.all([
        import("child_process"),
        import("fs"),
        import("path"),
        import("url")
    ]);
    while (moduleFsInitResolveList.length > 0) {
        moduleFsInitResolveList.shift()();
    }
}

function noop(val) {

// This function will do nothing except return <val>.

    return val;
}

function objectDeepCopyWithKeysSorted(obj) {

// This function will recursively deep-copy <obj> with keys sorted.

    let sorted;
    if (typeof obj !== "object" || !obj) {
        return obj;
    }

// Recursively deep-copy list with child-keys sorted.

    if (Array.isArray(obj)) {
        return obj.map(objectDeepCopyWithKeysSorted);
    }

// Recursively deep-copy obj with keys sorted.

    sorted = Object.create(null);
    Object.keys(obj).sort().forEach(function (key) {
        sorted[key] = objectDeepCopyWithKeysSorted(obj[key]);
    });
    return sorted;
}

async function sqlMessagePost(baton, cFuncName, ...argList) {

// This function will post msg to <sqlWorker> and return result.

    let errStack;
    let id;
    let result;
    let timeElapsed = Date.now();
    // increment sqlMessageId
    sqlMessageId += 1;
    id = sqlMessageId;
    // postMessage to web-worker
    sqlWorker.postMessage(
        {argList, baton, cFuncName, id},
        // transfer arraybuffer without copying
        [baton.buffer, ...argList.filter(noop)]
    );
    // preserve stack-trace
    errStack = new Error().stack.replace((
        /.*$/m
    ), "");
    // await result from web-worker
    result = await new Promise(function (resolve) {
        sqlMessageDict[id] = resolve;
    });
    // cleanup sqlMessageDict
    delete sqlMessageDict[id];
    // debug slow postMessage
    timeElapsed = Date.now() - timeElapsed;
    if (timeElapsed > 500 || cFuncName === "testTimeElapsed") {
        consoleError(
            "sqlMessagePost - "
            + JSON.stringify({cFuncName, timeElapsed})
            + errStack
        );
    }
    assertOrThrow(!result.errmsg, result.errmsg);
    return [result.baton, result.cFuncName, ...result.argList];
}

async function sqlmathInit() {

// This function will init sqlmath.

    let moduleModule;
    dbFinalizationRegistry = (
        dbFinalizationRegistry
    ) || new FinalizationRegistry(function ({afterFinalization, ptr}) {

// This function will auto-close any open sqlite3-db-pointer,
// after its js-wrapper has been garbage-collected.

        cCallAsync(jsbatonCreate("_dbClose"), "_dbClose", ptr[0]);
        if (afterFinalization) {
            afterFinalization();
        }
    });

// Feature-detect nodejs.

    if (
        typeof process !== "object"
        || typeof process?.versions?.node !== "string"
        || cModule
    ) {
        return;
    }

// Init moduleFs.

    await moduleFsInit();
    moduleFsInit(); // coverage-hack
    moduleChildProcessSpawn = moduleChildProcess.spawn;

// Init moduleFs.

    await moduleFsInit();
    moduleFsInit(); // coverage-hack
    moduleChildProcessSpawn = moduleChildProcess.spawn;
    cModulePath = moduleUrl.fileURLToPath(import.meta.url).replace(
        (/\bsqlmath\.mjs$/),
        `_sqlmath.napi6_${process.platform}_${process.arch}.node`
    );

// Import napi c-addon.

    if (!npm_config_mode_setup) {
        moduleModule = await import("module");
        if (!cModule) {
            cModule = moduleModule.createRequire(cModulePath);
            cModule = cModule(cModulePath);
        }
    }
    if (npm_config_mode_test) {

// Mock consoleError.

        consoleError = noop;

// Mock moduleChildProcessSpawn.

        moduleChildProcessSpawn = function () {
            let child = {
                end: noop,
                on: function (onType, resolve) {
                    switch (onType) {
                    case "data":
                        resolve(Buffer.alloc(0));
                        return;
                    default:
                        resolve(0);
                    }
                },
                setEncoding: noop,
                write: noop
            };
            child.stderr = child;
            child.stdin = child;
            child.stdout = child;
            return child;
        };
    }
}

function sqlmathWebworkerInit({
    db,
    modeTest
}) {

// This function will init sqlmath web-worker.

// Feature-detect browser.

    let Worker = globalThis.Worker;
    IS_BROWSER = true;
    if (modeTest) {
        Worker = function () {
            return;
        };
    }
    sqlWorker = new Worker("sqlmath_wasm.js");
    sqlWorker.onmessage = function ({
        data
    }) {
        sqlMessageDict[data.id](data);
    };
    if (modeTest) {
        sqlWorker.postMessage = function (data) {
            setTimeout(function () {
                sqlWorker.onmessage({
                    data
                });
            });
        };
        // test cCallAsync handling-behavior
        cCallAsync(jsbatonCreate("testTimeElapsed"), "testTimeElapsed", true);
        // test dbFileExportAsync handling-behavior
        dbFileExportAsync({
            db,
            filename: "aa",
            modeTest
        });
        // revert IS_BROWSER
        IS_BROWSER = undefined;
    }
}

sqlmathInit(); // coverage-hack
await sqlmathInit();
sqlmathInit(); // coverage-hack

export {
    JSBATON_ARGC,
    JSBATON_OFFSET_ALL,
    JSBATON_OFFSET_ARG0,
    JSBATON_OFFSET_ARGV,
    JSBATON_OFFSET_BUFV,
    JSBATON_OFFSET_CFUNCNAME,
    JS_MAX_SAFE_INTEGER,
    JS_MIN_SAFE_INTEGER,
    SIZEOF_BLOB_MAX,
    SIZEOF_CFUNCNAME,
    SIZEOF_MESSAGE,
    SQLITE_DATATYPE_BLOB,
    SQLITE_DATATYPE_FLOAT,
    SQLITE_DATATYPE_INTEGER,
    SQLITE_DATATYPE_INTEGER_0,
    SQLITE_DATATYPE_INTEGER_1,
    SQLITE_DATATYPE_NULL,
    SQLITE_DATATYPE_SHAREDARRAYBUFFER,
    SQLITE_DATATYPE_TEXT,
    SQLITE_DATATYPE_TEXT_0,
    SQLITE_RESPONSETYPE_LASTBLOB,
    //
    SQLITE_OPEN_AUTOPROXY, //jslint-ignore-line
    SQLITE_OPEN_CREATE,
    SQLITE_OPEN_DELETEONCLOSE,
    SQLITE_OPEN_EXCLUSIVE,
    SQLITE_OPEN_FULLMUTEX,
    SQLITE_OPEN_MAIN_DB,
    SQLITE_OPEN_MAIN_JOURNAL,
    SQLITE_OPEN_MEMORY,
    SQLITE_OPEN_NOFOLLOW,
    SQLITE_OPEN_NOMUTEX,
    SQLITE_OPEN_PRIVATECACHE,
    SQLITE_OPEN_READONLY,
    SQLITE_OPEN_READWRITE,
    SQLITE_OPEN_SHAREDCACHE,
    SQLITE_OPEN_SUBJOURNAL,
    SQLITE_OPEN_SUPER_JOURNAL,
    SQLITE_OPEN_TEMP_DB,
    SQLITE_OPEN_TEMP_JOURNAL,
    SQLITE_OPEN_TRANSIENT_DB,
    SQLITE_OPEN_URI,
    SQLITE_OPEN_WAL,
    assertInt64,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    childProcessSpawn2,
    ciBuildExt,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbNoopAsync,
    dbOpenAsync,
    debugInline,
    fsCopyFileUnlessTest,
    fsExistsUnlessTest,
    fsReadFileUnlessTest,
    fsWriteFileUnlessTest,
    jsbatonValueString,
    noop,
    objectDeepCopyWithKeysSorted,
    sqlmathWebworkerInit,
    version
};
