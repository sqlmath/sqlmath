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

let IS_BROWSER;
let JSBATON_ARGC = 16;
let SQLITE_DATATYPE_BLOB = 0x04;
// let SQLITE_DATATYPE_BLOB_0 = 0x14;
let SQLITE_DATATYPE_EXTERNALBUFFER = -0x01;
let SQLITE_DATATYPE_FLOAT = 0x02;
// let SQLITE_DATATYPE_FLOAT_0 = 0x12;
let SQLITE_DATATYPE_INTEGER = 0x01;
let SQLITE_DATATYPE_INTEGER_0 = 0x11;
let SQLITE_DATATYPE_INTEGER_1 = 0x21;
let SQLITE_DATATYPE_NULL = 0x05;
let SQLITE_DATATYPE_OFFSET = 768;
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
let cModule;
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
    __consoleError = console.error;
    return debug;
}());
let sqlMessageDict = {}; // dict of web-worker-callbacks
let sqlMessageId = 0;
let sqlWorker;

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

// This function will assert aa - bb <= Number.EPSILON

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
// this function will serialize <argList> to a c <baton>,
// suitable for passing into napi
    let argi = 0;
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
            return;
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
            return;
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
    return (
        IS_BROWSER
        ? await sqlMessagePost(...argList)
        : await cModule[cFuncName](argList)
    );
}

function dbCallAsync(baton, cFuncName, db, ...argList) {
// this function will call <cFuncName> using db <argList>[0]
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
// this function will close sqlite-database-connection <db>
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
            val
        );
    }));
    dbDict.delete(db);
}

function dbDeref(db) {
// this function will get private-object mapped to <db>
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
// this function will exec <sql> in <db> and return last value retrieved
// from execution as raw blob/buffer
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
    sql,
    tmpColList,
    tmpColListPriority,
    tmpCsv,
    tmpRowList,
    tmpTableName
}) {
// this function will exec <sql> in <db> and return <result>
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
                sql,
                tmpColList,
                tmpColListPriority,
                tmpCsv,
                tmpRowList,
                tmpTableName
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
                setTimeout(resolve, 50);
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
    if (tmpCsv || tmpRowList) {
        await dbTableInsertAsync({
            colList: tmpColList,
            colListPriority: tmpColListPriority,
            csv: tmpCsv,
            db,
            rowList: tmpRowList,
            tableName: tmpTableName
        });
    }
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
        ...externalbufferList        // 5
    );
    result = result[2];
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
    filename,
    modeExport = 1
}) {
// This function will export <db> to <filename>
    assertOrThrow(filename, "invalid filename " + filename);
    await dbCallAsync(
        undefined,
        "_dbFileImportOrExport",
        db,
        String(filename),
        modeExport
    );
}

async function dbFileImportAsync({
    db,
    filename
}) {
// This function will import <filename> to <db>
    await dbFileExportAsync({
        db,
        filename,
        modeExport: 0
    });
}

async function dbNoopAsync(...argList) {
// this function will do nothing except return argList
    return await cCallAsync(undefined, "_dbNoop", ...argList);
}

async function dbOpenAsync({
    afterFinalization,
    dbData,
    filename,
    flags,
    rawPtr,
    threadCount = 1
}) {
// this function will open and return sqlite-database-connection <db>
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
    if (rawPtr) {
        rawPtr = [
            BigInt(rawPtr)
        ];
        threadCount = 1;
    }
    connPool = await Promise.all(Array.from(new Array(
        threadCount
    ), async function () {
        let ptr = rawPtr || await cCallAsync(
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
        ptr = rawPtr || [
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
        ii: 0
    });
    return db;
}

async function dbTableInsertAsync({
    colList,
    colListPriority,
    csv,
    db,
    rowList,
    tableName
}) {
// this function will create-or-replace temp <tablename> with <rowList>
    let baton = jsbatonCreate();
    let externalbufferList = [];
    let sqlCreateTable;
    let sqlInsertRow;
    // normalize and validate tableName
    tableName = tableName || "__tmp1";
    tableName = "temp." + JSON.stringify(tableName.replace((
        /^temp\./
    ), ""));
    assertOrThrow((
        /^temp\."[A-Z_a-z][0-9A-Z_a-z]*?"$/
    ).test(tableName), "invalid tableName " + tableName);
    // parse csv
    if (!rowList && csv) {
        rowList = jsonRowListFromCsv({
            csv
        });
    }
    rowList = jsonRowListNormalize({
        colList,
        colListPriority,
        rowList
    });
    colList = rowList.shift();
    sqlCreateTable = (
        "DROP TABLE IF EXISTS " + tableName + ";"
        + "CREATE TEMP TABLE " + tableName + "(" + colList.join(",") + ");"
    );
    sqlInsertRow = (
        "INSERT INTO " + tableName + " VALUES("
        + ",?".repeat(colList.length).slice(1) + ");"
    );
    rowList.forEach(function (row) {
        row.forEach(function (val) {
            baton = jsbatonValuePush({
                baton,
                externalbufferList,
                value: val
            });
        });
    });
    await dbCallAsync(
        baton,
        "_dbTableInsert",
        db,
        colList.length,
        rowList.length,
        String(sqlCreateTable),
        String(sqlInsertRow),
        ...externalbufferList
    );
}

function isExternalBuffer(buf) {
// this function will check if <buf> is ArrayBuffer or SharedArrayBuffer
    return buf && (
        buf.constructor === ArrayBuffer
        || (
            typeof SharedArrayBuffer === "function"
            && buf.constructor === SharedArrayBuffer
        )
    );
}

function jsbatonCreate() {
// this function will create buffer <baton>
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
// this function will push <value> to buffer <baton>
    let nn;
    let nused;
    let tmp;
    let vsize;
    let vtype;
/*
#define SQLITE_DATATYPE_BLOB            0x04
// #define SQLITE_DATATYPE_BLOB_0          0x14
#define SQLITE_DATATYPE_FLOAT           0x02
// #define SQLITE_DATATYPE_FLOAT_0         0x12
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_EXTERNALBUFFER       -0x01
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
            vtype = SQLITE_DATATYPE_EXTERNALBUFFER;
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
    case SQLITE_DATATYPE_EXTERNALBUFFER:
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
    }
    return baton;
}

function jsbatonValueString({
    argi,
    baton
}) {
// this function will return string-value from <baton> at given <offset>
    let offset = baton.getInt32(4 + 4 + argi * 8, true);
    return new TextDecoder().decode(new Uint8Array(
        baton.buffer,
        offset + 1 + 4,
        // remove null-terminator from string
        baton.getInt32(offset + 1, true) - 1
    ));
}

function jsonRowListFromCsv({
    csv
}) {
// this function will convert <csv>-text to json list-of-list
/*
https://tools.ietf.org/html/rfc4180#section-2
Definition of the CSV Format
    While there are various specifications and implementations for the
    CSV format (for ex. [4], [5], [6] and [7]), there is no formal
    specification in existence, which allows for a wide variety of
    interpretations of CSV files.  This section documents the format that
    seems to be followed by most implementations:
1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
2.  The last record in the file may or may not have an ending line
    break.  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx
3.  There maybe an optional header line appearing as the first line
    of the file with the same format as normal record lines.  This
    header will contain names corresponding to the fields in the file
    and should contain the same number of fields as the records in
    the rest of the file (the presence or absence of the header line
    should be indicated via the optional "header" parameter of this
    MIME type).  For example:
    field_name,field_name,field_name CRLF
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:
    aaa,bbb,ccc
5.  Each field may or may not be enclosed in double quotes (however
    some programs, such as Microsoft Excel, do not use double quotes
    at all).  If fields are not enclosed with double quotes, then
    double quotes may not appear inside the fields.  For example:
    "aaa","bbb","ccc" CRLF
    zzz,yyy,xxx
6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:
    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx
7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:
    "aaa","b""bb","ccc"
*/
    let match;
    let quote = false;
    let rgx = (
        /(.*?)(""|"|,|\n)/g
    );
    let row = [];
    let rowList = [];
    let val = "";
    // normalize "\r\n" to "\n"
    csv = csv.replace((
        /\r\n?/g
    ), "\n");
/*
2.  The last record in the file may or may not have an ending line
    break.  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx
*/
    if (csv[csv.length - 1] !== "\n") {
        csv += "\n";
    }
    while (true) {
        match = rgx.exec(csv);
        if (!match) {
            return rowList;
        }
        // build val
        val += match[1];
        match = match[2];
        switch (quote + "." + match) {
        case "false.,":
/*
4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:
    aaa,bbb,ccc
*/
            // delimit val
            row.push(val);
            val = "";
            break;
        case "false.\"":
        case "true.\"":
/*
5.  Each field may or may not be enclosed in double quotes (however
    some programs, such as Microsoft Excel, do not use double quotes
    at all).  If fields are not enclosed with double quotes, then
    double quotes may not appear inside the fields.  For example:
    "aaa","bbb","ccc" CRLF
    zzz,yyy,xxx
*/
            assertOrThrow(quote || val === "", (
                "invalid csv - naked double-quote in unquoted-string "
                + JSON.stringify(val + "\"")
            ));
            quote = !quote;
            break;
        // backtrack for naked-double-double-quote
        case "false.\"\"":
            quote = true;
            rgx.lastIndex -= 1;
            break;
        case "false.\n":
        case "false.\r\n":
/*
1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
*/
            // delimit val
            row.push(val);
            val = "";
            // append row
            rowList.push(row);
            // reset row
            row = [];
            break;
        case "true.\"\"":
/*
7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:
    "aaa","b""bb","ccc"
*/
            val += "\"";
            break;
        default:
/*
6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:
    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx
*/
            assertOrThrow(quote, (
                "invalid csv - illegal character in unquoted-string "
                + JSON.stringify(match)
            ));
            val += match;
        }
    }
}

function jsonRowListNormalize({
    colList,
    colListPriority,
    rowList
}) {
// this function will normalize <rowList> with given <colList>
    let colDict = {};
    if (!(rowList?.length > 0)) {
        throw new Error("invalid rowList " + JSON.stringify(rowList));
    }
    // convert list-of-dict to list-of-list
    if (!Array.isArray(rowList[0])) {
        colList = new Map(Array.from(
            colList || []
        ).map(function (key, ii) {
            return [
                key, ii
            ];
        }));
        rowList = rowList.map(function (row) {
            Object.keys(row).forEach(function (key) {
                if (!colList.has(key)) {
                    colList.set(key, colList.size);
                }
            });
            return Array.from(colList.keys()).map(function (key) {
                return row[key];
            });
        });
        colList = Array.from(colList.keys());
    }
    if (!colList) {
        colList = rowList[0];
        rowList = rowList.slice(1);
    }
    if (!(colList?.length > 0)) {
        throw new Error("invalid colList " + JSON.stringify(colList));
    }
    colList = colList.map(function (key) {
        // sanitize column-name
        key = String(key).replace((
            /^[^A-Z_a-z]/
        ), "c_" + key);
        key = key.replace((
            /[^0-9A-Z_a-z]/g
        ), "_");
        // for duplicate column-name, add ordinal _2, _3, _4, ...
        colDict[key] = colDict[key] || 0;
        colDict[key] += 1;
        if (colDict[key] > 1) {
            key += "_" + colDict[key];
        }
        return key;
    });
    // normalize rowList
    rowList = rowList.map(function (row) {
        return (
            row.length === colList.length
            ? row
            : colList.map(function (ignore, ii) {
                return row[ii];
            })
        );
    });
    if (!colListPriority) {
        rowList.unshift(colList);
        return rowList;
    }
    // sort colList by colListPriority
    colListPriority = new Map([].concat(
        colListPriority,
        colList
    ).map(function (key) {
        return [
            key, colList.indexOf(key)
        ];
    }).filter(function ([
        ignore, ii
    ]) {
        return ii >= 0;
    }));
    colList = Array.from(colListPriority.keys());
    colListPriority = Array.from(colListPriority.values());
    rowList = rowList.map(function (row) {
        return colListPriority.map(function (ii) {
            return row[ii];
        });
    });
    rowList.unshift(colList);
    return rowList;
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

// This function will post msg to <sqlWorker> and return result

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
    // await result from web-worker
    result = await new Promise(function (resolve) {
        sqlMessageDict[id] = resolve;
    });
    // cleanup sqlMessageDict
    delete sqlMessageDict[id];
    console.error(
        "sqlMessagePost - " + JSON.stringify({
            cFuncName,
            timeElapsed: Date.now() - timeElapsed
        })
    );
    assertOrThrow(!result.errmsg, result.errmsg);
    return [
        result.baton, result.cFuncName, ...result.argList
    ];
}

async function sqlmathInit() {
    dbFinalizationRegistry = new FinalizationRegistry(function ({
        afterFinalization,
        ptr
    }) {
    // This function will auto-close any open sqlite3-db-pointer,
    // after its js-wrapper has been garbage-collected
        cCallAsync(undefined, "_dbClose", ptr[0]);
        if (afterFinalization) {
            afterFinalization();
        }
    });

// Feature-detect nodejs.

    if (
        typeof process === "object"
        && typeof process?.versions?.node === "string"
    ) {
        cModule = await import("module");
        cModule = cModule.createRequire(import.meta.url);
        cModule = cModule(
            "./_binary_sqlmath"
            + "_napi8"
            + "_" + process.platform
            + "_" + process.arch
            + ".node"
        );
        if (process.env.npm_config_mode_test) {
            // mock consoleError
            consoleError = noop;
        }
    }
}

function sqlmathWebworkerInit({
    Worker
}) {

// Feature-detect browser.

    IS_BROWSER = true;
    Worker = Worker || globalThis.Worker;
    sqlWorker = new Worker("sqlmath_wasm.js");
    sqlWorker.onmessage = function ({
        data
    }) {
        sqlMessageDict[data.id](data);
    };
    /*
    let db = await dbOpenAsync({ //jslint-quiet
        filename: ":memory:"
    });
    debugInline(
        await dbExecAsync({
            db,
            sql: "aSELECT 1234"
        })
    );
    */
}

await sqlmathInit({});

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
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbNoopAsync,
    dbOpenAsync,
    dbTableInsertAsync,
    debugInline,
    jsbatonValueString,
    noop,
    objectDeepCopyWithKeysSorted,
    sqlmathInit,
    sqlmathWebworkerInit
};
