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

let JSBATON_ARGC = 8;
let JSBATON_OFFSET_ALL = 256;
let JSBATON_OFFSET_ARGV = 128;
let JSBATON_OFFSET_BUFV = 192;
let JSBATON_OFFSET_FUNCNAME = 8;
let JS_MAX_SAFE_INTEGER = 0x1f_ffff_ffff_ffff;
let JS_MIN_SAFE_INTEGER = -0x1f_ffff_ffff_ffff;
let SIZEOF_BLOB_MAX = 1_000_000_000;
let SIZEOF_FUNCNAME = 16;
let SQLITE_DATATYPE_BLOB = 0x04;
let SQLITE_DATATYPE_EXTERNALBUFFER = 0x71;
let SQLITE_DATATYPE_FLOAT = 0x02;
let SQLITE_DATATYPE_INTEGER = 0x01;
let SQLITE_DATATYPE_INTEGER_0 = 0x00;
let SQLITE_DATATYPE_INTEGER_1 = 0x21;
let SQLITE_DATATYPE_NULL = 0x05;
let SQLITE_DATATYPE_TEXT = 0x03;
let SQLITE_DATATYPE_TEXT_0 = 0x13;
let SQLITE_RESPONSETYPE_LASTBLOB = 1;

let FILENAME_DBTMP = "/tmp/__dbtmp1"; //jslint-ignore-line
let IS_BROWSER;
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
let cModule;
let cModulePath;
let consoleError = console.error;
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
let version = "v2024.1.21";

async function assertErrorThrownAsync(asyncFunc, regexp) {

// This function will assert calling <asyncFunc> throws an error.

    let err;
    try {
        await asyncFunc();
    } catch (errCaught) {
        err = errCaught;
    }
    assertOrThrow(err, "No error thrown.");
    assertOrThrow(
        !regexp || new RegExp(regexp).test(err.message),
        err
    );
}

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
            "cflags": [
                "-Wextra",
                "-std=c11"
            ],
            "conditions": [
                [
                    "OS == 'win'",
                    {
                        "defines": [
                            "WIN32"
                        ]
                    }
                ]
            ],
// https://github.com/nodejs/node-gyp/blob/v9.3.1/gyp/pylib/gyp/MSVSSettings.py
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "WarningLevel": 3
                }
            },
            "xcode_settings": {
                "OTHER_CFLAGS": [
                    "-Wextra",
                    "-std=c11"
                ]
            }
        },
        "targets": [
            {
                "cflags": cflagsNowarning,
                "defines": [
                    "SRC_SQLITE_BASE_C2"
                ],
                "sources": [
                    "sqlmath_base.c",
                    "sqlmath_external_pcre2.c",
                    "sqlmath_external_sqlite.c",
                    "sqlmath_external_zlib.c"
                ],
                "target_name": "SRC_SQLITE_BASE",
                "type": "static_library",
                "xcode_settings": {
                    "OTHER_CFLAGS": cflagsNowarning
                }
            },
            {
                "defines": [
                    "SQLMATH_CUSTOM"
                ],
                "sources": [
                    "sqlmath_custom.c"
                ],
                "target_name": "SQLMATH_CUSTOM",
                "type": "static_library"
            },
            {
                "defines": [
                    "SQLMATH_NODEJS_C2"
                ],
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
                    "SRC_SQLITE_SHELL_C2"
                ],
                "dependencies": [
                    "SQLMATH_CUSTOM",
                    "SRC_SQLITE_BASE"
                ],
                "sources": [
                    "sqlmath_external_sqlite.c"
                ],
                "target_name": "shell",
                "type": "executable",
                "xcode_settings": {
                    "OTHER_CFLAGS": cflagsNowarning
                }
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

async function dbCallAsync(baton, argList, mode) {

// This function will call c-function dbXxx() with given <funcname>
// and return [<baton>, ...argList].

    let db;
    let errStack;
    let funcname;
    let id;
    let result;
    let timeElapsed;
    // If argList contains <db>, then mark it as busy.
    if (mode === "modeDb") {
        // init db
        db = argList[0];
        assertOrThrow(
            db.busy >= 0,
            `dbCallAsync - invalid db.busy = ${db.busy}`
        );
        db.ii = (db.ii + 1) % db.connPool.length;
        db.ptr = db.connPool[db.ii][0];
        // increment db.busy
        db.busy += 1;
        try {
            return await dbCallAsync(baton, [db.ptr, ...argList.slice(1)]);
        } finally {
            // decrement db.busy
            db.busy -= 1;
            assertOrThrow(
                db.busy >= 0,
                `dbCallAsync - invalid db.busy = ${db.busy}`
            );
        }
    }
    // copy argList to avoid side-effect
    argList = [...argList];
    assertOrThrow(
        argList.length <= JSBATON_ARGC,
        `dbCallAsync - argList.length must be less than than ${JSBATON_ARGC}`
    );
    // pad argList to length JSBATON_ARGC
    while (argList.length < JSBATON_ARGC) {
        argList.push(0n);
    }
    // serialize js-value to c-value
    argList = argList.map(function (val, argi) {
        if (val === null || val === undefined) {
            val = 0;
        }
        switch (typeof val) {
        case "bigint":
        case "boolean":
        case "number":
            // check for min/max safe-integer
            assertOrThrow(
                (
                    (JS_MIN_SAFE_INTEGER <= val && val <= JS_MAX_SAFE_INTEGER)
                    || typeof val === "bigint"
                ),
                (
                    "dbCallAsync - "
                    + "non-bigint-integer must be within inclusive-range"
                    + ` ${JS_MIN_SAFE_INTEGER} to ${JS_MAX_SAFE_INTEGER}`
                )
            );
            val = BigInt(val);
            assertInt64(val);
            baton.setBigInt64(JSBATON_OFFSET_ARGV + argi * 8, val, true);
            return val;
        // case "object":
        //     break;
        case "string":
            baton = jsbatonSetValue(baton, argi, (
                val.endsWith("\u0000")
                ? val
                // append null-terminator to string
                : val + "\u0000"
            ));
            return;
        }
        assertOrThrow(
            !ArrayBuffer.isView(val) || val.byteOffset === 0,
            (
                "dbCallAsync - argList cannot contain arraybuffer-views"
                + " with non-zero byteOffset"
            )
        );
        if (isExternalBuffer(val)) {
            return val;
        }
        throw new Error(`dbCallAsync - invalid arg-type "${typeof val}"`);
    });
    // assert byteOffset === 0
    [baton, ...argList].forEach(function (arg) {
        assertOrThrow(!ArrayBuffer.isView(arg) || arg.byteOffset === 0, arg);
    });
    // extract funcname
    funcname = new TextDecoder().decode(
        new DataView(baton.buffer, JSBATON_OFFSET_FUNCNAME, SIZEOF_FUNCNAME)
    ).replace((/\u0000/g), "");
    // preserve stack-trace
    errStack = new Error().stack.replace((/.*$/m), "");
    try {

// Dispatch to nodejs-napi.

        if (!IS_BROWSER) {
            await cModule._jspromiseCreate(baton.buffer, argList, funcname);
            // prepend baton to argList
            return [baton, ...argList];
        }

// Dispatch to web-worker.

        // increment sqlMessageId
        sqlMessageId += 1;
        id = sqlMessageId;
        // postMessage to web-worker
        sqlWorker.postMessage(
            {
                FILENAME_DBTMP,
                JSBATON_OFFSET_ALL,
                JSBATON_OFFSET_BUFV,
                argList,
                baton,
                funcname,
                id
            },
            // transfer arraybuffer without copying
            [baton.buffer, ...argList.filter(isExternalBuffer)]
        );
        // init timeElapsed
        timeElapsed = Date.now();
        // await result from web-worker
        result = await new Promise(function (resolve) {
            sqlMessageDict[id] = resolve;
        });
        // cleanup sqlMessageDict
        delete sqlMessageDict[id];
        // debug slow postMessage
        timeElapsed = Date.now() - timeElapsed;
        if (timeElapsed > 500 || funcname === "testTimeElapsed") {
            consoleError(
                "sqlMessagePost - "
                + JSON.stringify({funcname, timeElapsed})
                + errStack
            );
        }
        assertOrThrow(!result.errmsg, result.errmsg);
        // prepend baton to argList
        return [result.baton, ...result.argList];
    } catch (err) {
        err.stack += errStack;
        assertOrThrow(undefined, err);
    }
}

async function dbCloseAsync(db) {

// This function will close sqlite-database-connection <db>.

    // prevent segfault - do not close db if actions are pending
    assertOrThrow(
        db.busy === 0,
        `dbCloseAsync - cannot close db with ${db.busy} actions pending`
    );
    // cleanup connPool
    await Promise.all(db.connPool.map(async function (ptr) {
        let val = ptr[0];
        ptr[0] = 0n;
        await dbCallAsync(jsbatonCreate("_dbClose"), [val, db.filename]);
    }));
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
        responseType: "lastblob",
        sql
    });
}

async function dbExecAsync({
    bindList = [],
    db,
    responseType,
    sql
}) {

// This function will exec <sql> in <db> and return <result>.

    let baton = jsbatonCreate("_dbExec");
    let bindByKey = !Array.isArray(bindList);
    let bufi = [0];
    let referenceList = [];
    let result;
    if (bindByKey) {
        Object.entries(bindList).forEach(function ([key, val]) {
            baton = jsbatonSetValue(baton, undefined, `:${key}\u0000`);
            baton = jsbatonSetValue(
                baton,
                undefined,
                val,
                bufi,
                referenceList
            );
        });
    } else {
        bindList.forEach(function (val) {
            baton = jsbatonSetValue(
                baton,
                undefined,
                val,
                bufi,
                referenceList
            );
        });
    }
    [baton, ...result] = await dbCallAsync(
        baton,
        [
            // 0. db
            db,
            // 1. sql
            String(sql) + "\n;\nPRAGMA noop",
            // 2. bindList.length
            (
                bindByKey
                ? Object.keys(bindList).length
                : bindList.length
            ),
            // 3. bindByKey
            bindByKey,
            // 4. responseType
            (
                responseType === "lastblob"
                ? SQLITE_RESPONSETYPE_LASTBLOB
                : 0
            )
        ],
        "modeDb"
    );
    result = result[0];
    if (!IS_BROWSER) {
        result = cModule._jsbatonStealCbuffer(
            baton.buffer,
            0,
            Number(
                responseType !== "arraybuffer" && responseType !== "lastblob"
            )
        );
    }
    switch (responseType) {
    case "arraybuffer":
    case "lastblob":
        return result;
    case "list":
        return jsonParseArraybuffer(result);
    default:
        return jsonParseArraybuffer(result).map(function (table) {
            let colList = table.shift();
            return table.map(function (row) {
                let dict = {};
                colList.forEach(function (key, ii) {
                    dict[key] = row[ii];
                });
                return dict;
            });
        });
    }
}

async function dbFileLoadAsync({
    db,
    dbData,
    filename,
    modeNoop,
    modeSave = 0
}) {

// This function will load <filename> to <db>.

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
    dbData = await dbCallAsync(
        jsbatonCreate("_dbFileLoad"),
        [
            // 0. sqlite3 * pInMemory
            db,
            // 1. char *zFilename
            filename,
            // 2. const int isSave
            modeSave,
            // 3. undefined
            undefined,
            // 4. dbData - same position as dbOpenAsync
            dbData
        ],
        "modeDb"
    );
    return dbData[1 + 0];
}

async function dbFileSaveAsync({
    db,
    dbData,
    filename
}) {

// This function will save <db> to <filename>.

    return await dbFileLoadAsync({
        db,
        dbData,
        filename,
        modeSave: 1
    });
}

async function dbNoopAsync(...argList) {

// This function will do nothing except return <argList>.

    return await dbCallAsync(jsbatonCreate("_dbNoop"), argList);
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
    let db = {busy: 0, filename, ii: 0};
    assertOrThrow(typeof filename === "string", `invalid filename ${filename}`);
    assertOrThrow(
        !dbData || isExternalBuffer(dbData),
        "dbData must be ArrayBuffer"
    );
    connPool = await Promise.all(Array.from(new Array(
        threadCount
    ), async function () {
        let [ptr] = await dbCallAsync(
            jsbatonCreate("_dbOpen"),
            [
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
                // 4. wasm-only - arraybuffer of raw sqlite-database
                dbData
            ]
        );
        ptr = [ptr.getBigInt64(JSBATON_OFFSET_ARGV + 0, true)];
        dbFinalizationRegistry.register(db, {afterFinalization, ptr});
        return ptr;
    }));
    db.connPool = connPool;
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

// This function will check if <buf> is ArrayBuffer.

    return buf && buf.constructor === ArrayBuffer;
}

function jsbatonCreate(funcname) {

// This function will create buffer <baton>.

    let baton = new DataView(new ArrayBuffer(JSBATON_OFFSET_ALL));
    // init nallc, nused
    baton.setInt32(4, JSBATON_OFFSET_ALL, true);
    // copy funcname into baton
    new Uint8Array(
        baton.buffer,
        baton.byteOffset + JSBATON_OFFSET_FUNCNAME,
        SIZEOF_FUNCNAME - 1
    ).set(new TextEncoder().encode(funcname));
    return baton;
}

function jsbatonGetInt64(baton, argi) {

// This function will return int64-value from <baton> at <argi>.

    return baton.getBigInt64(JSBATON_OFFSET_ARGV + argi * 8, true);
}

function jsbatonGetString(baton, argi) {

// This function will return string-value from <baton> at <argi>.

    let offset = baton.getInt32(JSBATON_OFFSET_ARGV + argi * 8, true);
    return new TextDecoder().decode(new Uint8Array(
        baton.buffer,
        baton.byteOffset + offset + 1 + 4,
        // remove null-terminator from string
        baton.getInt32(offset + 1, true) - 1
    ));
}

function jsbatonSetValue(baton, argi, val, bufi, referenceList) {

// This function will set <val> to buffer <baton>.

    let nn;
    let nused;
    let tmp;
    let vsize;
    let vtype;
/*
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_EXTERNALBUFFER          0x71
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x00
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
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
        if (Number.isNaN(val)) {
            vtype = SQLITE_DATATYPE_NULL;
            vsize = 0;
            break;
        }
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
        if (val.constructor === ArrayBuffer) {
            assertOrThrow(
                !IS_BROWSER,
                "external ArrayBuffer cannot be passed directly to wasm"
            );
            vtype = SQLITE_DATATYPE_EXTERNALBUFFER;
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
    // push vtype - 1-byte
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
        // push vsize - 4-byte
        baton.setInt32(nused + 1, vsize, true);
        // push SQLITE-BLOB/TEXT - vsize-byte
        new Uint8Array(
            baton.buffer,
            baton.byteOffset + nused + 1 + 4,
            vsize
        ).set(new Uint8Array(val.buffer, val.byteOffset, vsize));
        break;
    case SQLITE_DATATYPE_EXTERNALBUFFER:
        vsize = val.byteLength;
        assertOrThrow(
            0 <= vsize && vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + ` 0 to ${SIZEOF_BLOB_MAX}`
            )
        );
        assertOrThrow(
            bufi[0] < JSBATON_ARGC,
            `cannot pass more than ${JSBATON_ARGC} arraybuffers`
        );
        // push externalbuffer - 4-byte
        baton.setInt32(nused + 1, bufi[0], true);
        // set buffer
        cModule._jsbatonSetArraybuffer(baton.buffer, bufi[0], val);
        // increment bufi
        bufi[0] += 1;
        // add buffer to reference_list to prevent gc during db_call.
        referenceList.push(val);
        break;
    case SQLITE_DATATYPE_FLOAT:
        // push SQLITE-REAL - 8-byte
        baton.setFloat64(nused + 1, val, true);
        break;
    case SQLITE_DATATYPE_INTEGER:
        assertInt64(val);
        // push SQLITE-INTEGER - 8-byte
        baton.setBigInt64(nused + 1, val, true);
        break;
    }
    return baton;
}

function jsonParseArraybuffer(buf) {

// This function will JSON.parse arraybuffer <buf>.

    return JSON.parse(
        IS_BROWSER
        ? new TextDecoder().decode(buf)
        : buf
    );
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

async function sqlmathInit() {

// This function will init sqlmath.

    let moduleModule;
    dbFinalizationRegistry = (
        dbFinalizationRegistry
    ) || new FinalizationRegistry(function ({afterFinalization, ptr}) {

// This function will auto-close any open sqlite3-db-pointer,
// after its js-wrapper has been garbage-collected.

        dbCallAsync(jsbatonCreate("_dbClose"), [ptr[0]]);
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
                sqlWorker.onmessage({data});
            });
        };
        // test dbCallAsync handling-behavior
        dbCallAsync(jsbatonCreate("testTimeElapsed"), [true]);
        // test dbFileLoadAsync handling-behavior
        dbFileLoadAsync({db, filename: "aa", modeTest});
        // test jsonParseArraybuffer handling-behavior
        jsonParseArraybuffer(new TextEncoder().encode("0"));
        // revert IS_BROWSER
        IS_BROWSER = undefined;
    }
}

sqlmathInit(); // coverage-hack
await sqlmathInit();
sqlmathInit(); // coverage-hack

export {
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
    assertErrorThrownAsync,
    assertInt64,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    childProcessSpawn2,
    ciBuildExt,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAsync,
    dbFileLoadAsync,
    dbFileSaveAsync,
    dbNoopAsync,
    dbOpenAsync,
    debugInline,
    fsCopyFileUnlessTest,
    fsExistsUnlessTest,
    fsReadFileUnlessTest,
    fsWriteFileUnlessTest,
    jsbatonGetInt64,
    jsbatonGetString,
    noop,
    objectDeepCopyWithKeysSorted,
    sqlmathWebworkerInit,
    version
};
