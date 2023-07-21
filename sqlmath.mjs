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
let SQLITE_DATATYPE_BLOB = 0x04;
let SQLITE_DATATYPE_FLOAT = 0x02;
let SQLITE_DATATYPE_INTEGER = 0x01;
let SQLITE_DATATYPE_INTEGER_0 = 0x11;
let SQLITE_DATATYPE_INTEGER_1 = 0x21;
let SQLITE_DATATYPE_NULL = 0x05;
let SQLITE_DATATYPE_OFFSET = 768;
let SQLITE_DATATYPE_SHAREDARRAYBUFFER = 0x71;
let SQLITE_DATATYPE_TEXT = 0x03;
let SQLITE_DATATYPE_TEXT_0 = 0x13;
let SQLITE_MAX_LENGTH2 = 1_000_000_000;
let SQLITE_OPEN_AUTOPROXY = 0x00000020;     /* VFS only */
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
let SQLITE_WIN_SLR_ELEM_SIZE = 10;
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
let version = "v2023.7.21";

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

    let argi = 0;
    let errStack;
    assertOrThrow(
        argList.length < 16,
        "cCallAsync - argList.length must be less than than 16"
    );
    baton = baton || jsbatonCreate();
    // pad argList to length JSBATON_ARGC
    while (argList.length < 2 * JSBATON_ARGC) {
        argList.push(0n);
    }
    // serialize js-value to c-value
    argList = argList.map(function (value, ii) {
        argi = ii;
        switch (typeof value) {
        case "bigint":
        case "boolean":
            baton.setBigInt64(8 + argi * 8, BigInt(value), true);
            return value;
        case "number":
            // check for min/max safe-integer
            assertOrThrow(
                (
                    -9_007_199_254_740_991 <= value
                    && value <= 9_007_199_254_740_991
                ),
                (
                    "non-bigint integer must be within inclusive-range"
                    + " -9,007,199,254,740,991 to 9,007,199,254,740,991"
                )
            );
            baton.setBigInt64(8 + argi * 8, BigInt(value), true);
            return value;
        // case "object":
        //     break;
        case "string":
            baton = jsbatonValuePush({
                argi,
                baton,
                value: (
                    value.endsWith("\u0000")
                    ? value
                    // append null-terminator to string
                    : value + "\u0000"
                )
            });
            return;
        }
        if (ArrayBuffer.isView(value)) {
            return new DataView(
                value.buffer,
                value.byteOffset,
                value.byteLength
            );
        }
        if (isExternalBuffer(value)) {
            return value;
        }
    });
    // encode cFuncName into baton
    argi += 1;
    baton = jsbatonValuePush({
        argi,
        baton,
        value: cFuncName + "\u0000"
    });
    // prepend baton, cFuncName to argList
    argList = [
        baton, cFuncName, ...argList
    ];
    // preserve stack-trace
    errStack = new Error().stack.replace((
        /.*$/m
    ), "");
    try {
        return (
            IS_BROWSER
            ? await sqlMessagePost(...argList)
            : await cModule[cFuncName](argList)
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

    consoleError(`ciBuildExt1Nodejs - configure binding.gyp`);
    await fsWriteFileUnlessTest("binding.gyp", JSON.stringify({
        "target_defaults": {
            "cflags": ["-Wextra", "-std=c11"],
            "conditions": [
                [
                    "OS == \u0027win\u0027",
                    {"defines": ["WIN32"]},
                    {"defines": ["HAVE_UNISTD_H"]}
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
                "cflags": [
                    "-Wno-all",
                    "-Wno-implicit-fallthrough",
                    "-Wno-unused-parameter"
                ],
                "sources": [
                    "build/SRC_ZLIB_BASE.c",
                    "build/SRC_SQLITE_BASE.c",
                    "build/SQLMATH_BASE.c"
                ],
                "target_name": "SRC_SQLITE_BASE",
                "type": "static_library",
                "xcode_settings": {"OTHER_CFLAGS": [
                    "-Wno-all",
                    "-Wno-implicit-fallthrough",
                    "-Wno-unused-parameter"
                ]}
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
                "sources": ["sqlmath_base.c"],
                "target_name": "binding"
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

async function dbCloseAsync({
    db
}) {

// This function will close sqlite-database-connection <db>.

    let __db = dbDeref(db);
    // prevent segfault - do not close db if actions are pending
    assertOrThrow(
        __db.busy === 0,
        "db cannot close with " + __db.busy + " actions pending"
    );
    // cleanup connPool
    await Promise.all(__db.connPool.map(async function (ptr) {
        let val = ptr[0];
        ptr[0] = 0n;
        await cCallAsync(
            undefined,
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

async function dbExecAndReturnLastJsonAsync(option) {

// This function will exec <sql> in <db> and return last value retrieved
// from execution as raw text.

    return JSON.parse(
        await dbExecAndReturnLastTextAsync(option)
    );
}

async function dbExecAndReturnLastTextAsync(option) {

// This function will exec <sql> in <db> and return last value retrieved
// from execution as raw text.

    return new TextDecoder().decode(
        await dbExecAndReturnLastBlobAsync(option)
    );
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
    baton = jsbatonCreate();
    bindByKey = !Array.isArray(bindList);
    bindListLength = (
        Array.isArray(bindList)
        ? bindList.length
        : Object.keys(bindList).length
    );
    externalbufferList = [];
    Object.entries(bindList).forEach(function ([
        key, val
    ]) {
        if (bindByKey) {
            baton = jsbatonValuePush({
                baton,
                value: ":" + key + "\u0000"
            });
        }
        baton = jsbatonValuePush({
            baton,
            externalbufferList,
            value: val
        });
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
            ? 1
            : 0
        ),
        undefined, // 5
        undefined, // 6
        undefined, // 7 - response
        ...externalbufferList        // 8
    );
    result = result[2 + 7];
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
        undefined,
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

// This function will do nothing except return argList.

    return await cCallAsync(undefined, "_dbNoop", ...argList);
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
    let db = {
        filename
    };
    assertOrThrow(
        typeof filename === "string",
        `invalid filename ${filename}`
    );
    assertOrThrow(
        !dbData || isExternalBuffer(dbData),
        "dbData must be ArrayBuffer"
    );
    connPool = await Promise.all(Array.from(new Array(
        threadCount
    ), async function () {
        let ptr = await cCallAsync(
            undefined,
            "_dbOpen",
            // 0. const char *filename,   Database filename (UTF-8)
            String(filename),
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
        ptr = [
            ptr[0].getBigInt64(4 + 4, true)
        ];
        dbFinalizationRegistry.register(db, {
            afterFinalization,
            ptr
        });
        return ptr;
    }));
    dbDict.set(db, {
        busy: 0,
        connPool,
        filename,
        ii: 0
    });
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

    return buf && (
        buf.constructor === ArrayBuffer
        || (
            typeof SharedArrayBuffer === "function"
            && buf.constructor === SharedArrayBuffer
        )
    );
}

function jsbatonCreate() {

// This function will create buffer <baton>.

    let baton = new DataView(new ArrayBuffer(1024));
    // offset nalloc, nused
    baton.setInt32(4, SQLITE_DATATYPE_OFFSET, true);
    return baton;
}

function jsbatonValuePush({
    argi,
    baton,
    externalbufferList,
    value
}) {

// This function will push <value> to buffer <baton>.

    let nn;
    let nused;
    let tmp;
    let vsize;
    let vtype;
/*
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_OFFSET          768
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       0x71
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
    //  1. false.bigint
    //  2. false.boolean
    //  3. false.function
    //  4. false.number
    //  5. false.object
    //  6. false.string
    //  7. false.symbol
    //  8. false.undefined
    //  9. true.bigint
    // 10. true.boolean
    // 11. true.function
    // 12. true.number
    // 13. true.object
    // 14. true.string
    // 15. true.symbol
    // 16. true.undefined
    // 17. true.buffer
    // 18. true.externalbuffer
*/
    // 10. true.boolean
    if (value === 1 || value === 1n) {
        value = true;
    }
    switch (Boolean(value) + "." + typeof(value)) {
    //  1. false.bigint
    case "false.bigint":
    //  2. false.boolean
    case "false.boolean":
    //  4. false.number
    case "false.number":
        vtype = SQLITE_DATATYPE_INTEGER_0;
        vsize = 0;
        break;
    //  3. false.function
    // case "false.function":
    //  5. false.object
    case "false.object":
    //  7. false.symbol
    case "false.symbol":
    //  8. false.undefined
    case "false.undefined":
    // 11. true.function
    case "true.function":
    // 15. true.symbol
    case "true.symbol":
    // 16. true.undefined
    // case "true.undefined":
        vtype = SQLITE_DATATYPE_NULL;
        vsize = 0;
        break;
    //  6. false.string
    case "false.string":
        vtype = SQLITE_DATATYPE_TEXT_0;
        vsize = 0;
        break;
    //  9. true.bigint
    case "true.bigint":
        vtype = SQLITE_DATATYPE_INTEGER;
        vsize = 8;
        break;
    // 10. true.boolean
    case "true.boolean":
        vtype = SQLITE_DATATYPE_INTEGER_1;
        vsize = 0;
        break;
    // 12. true.number
    case "true.number":
        vtype = SQLITE_DATATYPE_FLOAT;
        vsize = 8;
        break;
    // 13. true.object
    // 14. true.string
    default:
        // 18. true.externalbuffer
        if (isExternalBuffer(value)) {
            assertOrThrow(
                !IS_BROWSER,
                "external ArrayBuffer cannot be passed directly to wasm"
            );
            assertOrThrow(
                externalbufferList.length <= 8,
                "externalbufferList.length must be less than 8"
            );
            externalbufferList.push(new DataView(value));
            vtype = SQLITE_DATATYPE_SHAREDARRAYBUFFER;
            vsize = 4;
            break;
        }
        // 17. true.buffer
        if (ArrayBuffer.isView(value)) {
            if (value.byteLength === 0) {
                vtype = SQLITE_DATATYPE_NULL;
                vsize = 0;
                break;
            }
            vtype = SQLITE_DATATYPE_BLOB;
            vsize = 4 + value.byteLength;
            break;
        }
        // 13. true.object
        value = String(
            typeof value === "string"
            ? value
            : typeof value.toJSON === "function"
            ? value.toJSON()
            : JSON.stringify(value)
        );
        // 14. true.string
        value = new TextEncoder().encode(value);
        vtype = SQLITE_DATATYPE_TEXT;
        vsize = 4 + value.byteLength;
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
        // update nalloc
        baton.setInt32(0, baton.byteLength, true);
        // copy tmp to baton
        new Uint8Array(
            baton.buffer,
            baton.byteOffset,
            nused
        ).set(new Uint8Array(tmp.buffer, tmp.byteOffset, nused), 0);
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
            baton.setInt32(8 + argi * 8, nused, true);
        }
        vsize -= 4;
        // push vsize
        assertOrThrow(
            0 <= vsize && vsize <= 1_000_000_000,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + " 0 to 1,000,000,000"
            )
        );
        baton.setInt32(nused + 1, vsize, true);
        new Uint8Array(
            baton.buffer,
            nused + 1 + 4,
            vsize
        ).set(new Uint8Array(value.buffer, value.byteOffset, vsize), 0);
        break;
    case SQLITE_DATATYPE_FLOAT:
        baton.setFloat64(nused + 1, value, true);
        break;
    case SQLITE_DATATYPE_INTEGER:
        assertOrThrow(
            (
                -9_223_372_036_854_775_808n <= value
                && value <= 9_223_372_036_854_775_807n
            ),
            (
                "sqlite-integer must be within inclusive-range "
                + "-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807"
            )
        );
        baton.setBigInt64(nused + 1, value, true);
        break;
    case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
        vsize = value.byteLength;
        // push vsize
        assertOrThrow(
            0 <= vsize && vsize <= 1_000_000_000,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + " 0 to 1,000,000,000"
            )
        );
        baton.setInt32(nused + 1, vsize, true);
        break;
    }
    return baton;
}

function jsbatonValueString({
    argi,
    baton
}) {

// This function will return string-value from <baton> at given <offset>.

    let offset = baton.getInt32(4 + 4 + argi * 8, true);
    return new TextDecoder().decode(new Uint8Array(
        baton.buffer,
        offset + 1 + 4,
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
        {
            argList,
            baton,
            cFuncName,
            id
        },
        // transfer arraybuffer without copying
        [
            baton.buffer,
            ...argList.filter(function (elem) {
                return elem && elem.constructor === ArrayBuffer;
            })
        ]
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
            "sqlMessagePost - " + JSON.stringify({
                cFuncName,
                timeElapsed
            }) + errStack
        );
    }
    assertOrThrow(!result.errmsg, result.errmsg);
    return [
        result.baton, result.cFuncName, ...result.argList
    ];
}

async function sqlmathInit() {

// This function will init sqlmath.

    let moduleModule;
    dbFinalizationRegistry = (
        dbFinalizationRegistry
    ) || new FinalizationRegistry(function ({
        afterFinalization,
        ptr
    }) {

// This function will auto-close any open sqlite3-db-pointer,
// after its js-wrapper has been garbage-collected.

        cCallAsync(undefined, "_dbClose", ptr[0]);
        if (afterFinalization) {
            afterFinalization();
        }
    });

// Feature-detect nodejs.

    if (
        !(
            typeof process === "object"
            && typeof process?.versions?.node === "string"
        )
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
        cCallAsync(undefined, "testTimeElapsed", true);
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
    SQLITE_MAX_LENGTH2,
    SQLITE_OPEN_AUTOPROXY,
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
    SQLITE_WIN_SLR_ELEM_SIZE,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    childProcessSpawn2,
    ciBuildExt,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAndReturnLastJsonAsync,
    dbExecAndReturnLastTextAsync,
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
