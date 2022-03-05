/*jslint beta, bitwise, name, node*/
"use strict";
import {createRequire} from "module";
import jslint from "./jslint.mjs";

let {
    assertErrorThrownAsync,
    assertJsonEqual,
    assertOrThrow,
    debugInline,
    noop,
    objectDeepCopyWithKeysSorted
} = jslint;
let local = Object.assign({}, jslint);

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


/*
file sqlmath.js
*/
(function () {
    let JSBATON_ARGC = 16;
    // let SIZEOF_MESSAGE_DEFAULT = 768;
    let SQLITE_MAX_LENGTH2 = 1000000000;
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
    let addon;
    let consoleError = console.error;
    let dbDict = new WeakMap(); // private map of sqlite-database-connections
    let requireCjs = createRequire(import.meta.url);
    let testList;

    function cCall(func, argList) {
// this function will serialize <argList> to a c <baton>,
// suitable for passing into napi
        let baton = new BigInt64Array(2048);
        let errStack;
        let result;
        // serialize js-args to c-args
        argList = argList.map(function (arg, ii) {
            switch (typeof arg) {
            case "bigint":
            case "boolean":
            case "number":
                try {
                    baton[ii] = BigInt(arg);
                } catch (ignore) {
                    return;
                }
                break;
            // case "object":
            //     break;
            case "string":
                // append null-terminator to string
                arg = new TextEncoder().encode(arg + "\u0000");
                break;
            }
            if (ArrayBuffer.isView(arg)) {
                baton[ii] = BigInt(arg.byteLength);
                return new DataView(
                    arg.buffer,
                    arg.byteOffset,
                    arg.byteLength
                );
            }
            return arg;
        });
        // pad argList to length = JSBATON_ARGC
        argList = argList.concat(
            Array.from(new Array(JSBATON_ARGC))
        ).slice(0, JSBATON_ARGC);
        // prepend baton to argList
        argList.unshift(baton);
        // call napi with func and argList
        result = addon[func](argList);
        if (typeof result?.catch === "function") {
            errStack = new Error().stack.replace((
                /.*$/m
            ), "");
            return result.catch(function (err) {
                err.stack += errStack;
                throw err;
            });
        }
        return result;
    }

    function dbCallAsync(func, db, argList) {
// this function will call <func> using <db>
        db = dbDeref(db);
        // increment db.busy
        db.busy += 1;
        return cCall(func, [
            db.ptr
        ].concat(argList)).finally(function () {
            // decrement db.busy
            db.busy -= 1;
            assertOrThrow(db.busy >= 0, "invalid db.busy " + db.busy);
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
            await cCall("__dbCloseAsync", [
                val
            ]);
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

    async function dbExecAsync({
        bindList = [],
        db,
        responseType,
        sql,
        tmpColList,
        tmpColListPriority,
        tmpCsv,
        tmpRowList,
        tmpTableName
    }) {
// this function will exec <sql> in <db> and return <result>
        let bindByKey = !Array.isArray(bindList);
        let bindListLength = (
            Array.isArray(bindList)
            ? bindList.length
            : Object.keys(bindList).length
        );
        let result;
        let serialize = jsToSqlSerializer();
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
                serialize(":" + key + "\u0000");
            }
            serialize(val);
        });
        result = await dbCallAsync("__dbExecAsync", db, [
            String(sql) + "\n;\nPRAGMA noop",
            bindListLength,
            serialize.bufResult,
            bindByKey,
            (
                responseType === "lastBlob"
                ? 1
                : responseType === "lastMatrixDouble"
                ? 2
                : 0
            )
        ].concat(serialize.bufSharedList));
        result = result[1];
        switch (responseType) {
        case "arraybuffer":
        case "lastBlob":
            return result;
        case "lastMatrixDouble":
            return new Float64Array(result);
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

    async function dbExecWithRetryAsync(option) {
// this function will exec <sql> in <db> and return <result> with <retryLimit>
        let retry = option.retryLimit || 1;
        while (true) {
            try {
                return await dbExecAsync(option);
            } catch (err) {
                assertOrThrow(retry > 0, err);
                consoleError(err);
                consoleError(
                    "dbExecWithRetryAsync - retry failed sql-query with "
                    + retry
                    + " remaining retry"
                );
                retry -= 1;
            }
        }
    }

    function dbGetLastBlobAsync({
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

    function dbGetLastMatrixDouble({
        bindList = [],
        db,
        sql
    }) {
// this function will exec <sql> in <db> and return last SELECT-statement
// from execution as row x col matrix of doubles
        return dbExecAsync({
            bindList,
            db,
            responseType: "lastMatrixDouble",
            sql
        });
    }

    async function dbMemoryLoadAsync({
        db,
        filename
    }) {
// This function will load <filename> to <db>
        assertOrThrow(filename, "invalid filename " + filename);
        await dbCallAsync("__dbMemoryLoadOrSave", db, [
            String(filename), 0
        ]);
    }

    async function dbMemorySaveAsync({
        db,
        filename
    }) {
// This function will save <db> to <filename>
        assertOrThrow(filename, "invalid filename " + filename);
        await dbCallAsync("__dbMemoryLoadOrSave", db, [
            String(filename), 1
        ]);
    }

    async function dbOpenAsync({
        filename,
        flags,
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
        let db = {};
        assertOrThrow(
            typeof filename === "string",
            "invalid filename " + filename
        );
        connPool = await Promise.all(Array.from(new Array(
            threadCount
        ), async function () {
            let finalizer;
            let ptr = await cCall("__dbOpenAsync", [
                String(filename),
                undefined,
                flags ?? (
                    SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
                ),
                undefined
            ]);
            ptr = ptr[0][0];
            finalizer = new BigInt64Array(addon.__dbFinalizerCreate());
            finalizer[0] = BigInt(ptr);
            return finalizer;
        }));
        dbDict.set(db, {
            busy: 0,
            connPool,
            ii: 0,
            ptr: 0n
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
        let serialize = jsToSqlSerializer();
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
            row.forEach(serialize);
        });
        await dbCallAsync("__dbTableInsertAsync", db, [
            String(sqlCreateTable),
            String(sqlInsertRow),
            serialize.bufResult,
            colList.length,
            rowList.length
        ]);
    }

    function jsToSqlSerializer() {
// this function will return another function that serializes javascript <val>
// to <bufResult> as sqlite-values
        let BIGINT64_MAX = 2n ** 63n - 1n;
        let BIGINT64_MIN = -(2n ** 63n - 1n);
        let SQLITE_DATATYPE_BLOB = 0x04;
        // let SQLITE_DATATYPE_BLOB_0 = 0x14;
        let SQLITE_DATATYPE_FLOAT = 0x02;
        // let SQLITE_DATATYPE_FLOAT_0 = 0x12;
        let SQLITE_DATATYPE_INTEGER = 0x01;
        let SQLITE_DATATYPE_INTEGER_0 = 0x11;
        let SQLITE_DATATYPE_INTEGER_1 = 0x21;
        let SQLITE_DATATYPE_NULL = 0x05;
        let SQLITE_DATATYPE_SHAREDARRAYBUFFER = -0x01;
        let SQLITE_DATATYPE_TEXT = 0x03;
        let SQLITE_DATATYPE_TEXT_0 = 0x13;
        let bufResult = new DataView(new ArrayBuffer(2048));
        let bufSharedList = [];
        let offset = 0;
        function bufferAppendDatatype(datatype, byteLength) {
// this function will grow <bufResult> by <bytelength> and append <datatype>
            let nn = offset + 1 + byteLength;
            let tmp;
            // exponentially grow bufResult as needed
            if (bufResult.byteLength < nn) {
                assertOrThrow(nn <= SQLITE_MAX_LENGTH2, (
                    "sqlite - string or blob exceeds size limit of "
                    + SQLITE_MAX_LENGTH2 + " bytes"
                ));
                tmp = bufResult;
                bufResult = new DataView(new ArrayBuffer(
                    Math.min(2 ** Math.ceil(Math.log2(nn)), SQLITE_MAX_LENGTH2)
                ));
                // copy tmp to bufResult with offset
                bufferSetBuffer(bufResult, tmp, 0);
                // save bufResult
                serialize.bufResult = bufResult;
            }
            bufResult.setUint8(offset, datatype);
            offset += 1;
        }
        function bufferSetBigint64(offset, val) {
// this function will set bigint <val> to buffer <bufResult> at <offset>
            assertOrThrow(
                BIGINT64_MIN <= val && val <= BIGINT64_MAX,
                (
                    "The value of \"value\" is out of range."
                    + " It must be >= -(2n ** 63n) and < 2n ** 63n."
                )
            );
            bufResult.setBigInt64(offset, val, true);
        }
        function bufferSetBuffer(aa, bb, offset) {
// this function will set buffer <bb> to buffer <aa> at <offset>
            aa = new Uint8Array(aa.buffer, aa.byteOffset, aa.byteLength);
            bb = new Uint8Array(bb.buffer, bb.byteOffset, bb.byteLength);
            aa.set(bb, offset);
            return bb.byteLength;
        }
        function serialize(val) {
// this function will write to <bufResult>, <val> at given <offset>
/*
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_BLOB_0          0x14
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_FLOAT_0         0x12
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
            // 1. false.bigint
            // 2. false.boolean
            // 3. false.function
            // 4. false.number
            // 5. false.object
            // 6. false.string
            // 7. false.symbol
            // 8. false.undefined
            // 11. true.bigint
            // 12. true.boolean
            // 13. true.function
            // 14. true.number
            // 15. true.object
            // 16. true.string
            // 17. true.symbol
            // 18. true.undefined
*/
            // -1. SharedArrayBuffer
            if (val && val.constructor === SharedArrayBuffer) {
                assertOrThrow(
                    bufSharedList.length <= 0.5 * JSBATON_ARGC,
                    (
                        "too many SharedArrayBuffer's " + bufSharedList.length
                        + " > " + (0.5 * JSBATON_ARGC)
                    )
                );
                bufferAppendDatatype(SQLITE_DATATYPE_SHAREDARRAYBUFFER, 0);
                bufSharedList.push(new DataView(val));
                return;
            }
            // 12. true.boolean
            if (val === 1 || val === 1n || val === true) {
                bufferAppendDatatype(SQLITE_DATATYPE_INTEGER_1, 0);
                return;
            }
            switch (Boolean(val) + "." + typeof(val)) {
            // 1. false.bigint
            case "false.bigint":
            // 2. false.boolean
            case "false.boolean":
            // 4. false.number
            case "false.number":
                bufferAppendDatatype(SQLITE_DATATYPE_INTEGER_0, 0);
                return;
            // 3. false.function
            // case "false.function":
            // 5. false.object
            case "false.object":
            // 7. false.symbol
            // case "false.symbol":
            // 8. false.undefined
            case "false.undefined":
            // 13. true.function
            case "true.function":
            // 17. true.symbol
            case "true.symbol":
            // 18. true.undefined
            // case "true.undefined":
                bufferAppendDatatype(SQLITE_DATATYPE_NULL, 0);
                return;
            // 6. false.string
            case "false.string":
                bufferAppendDatatype(SQLITE_DATATYPE_TEXT_0, 0);
                return;
            // 11. true.bigint
            case "true.bigint":
                bufferAppendDatatype(SQLITE_DATATYPE_INTEGER, 8);
                bufferSetBigint64(offset, val);
                offset += 8;
                return;
            // 14. true.number
            case "true.number":
                bufferAppendDatatype(SQLITE_DATATYPE_FLOAT, 8);
                bufResult.setFloat64(offset, val, true);
                offset += 8;
                return;
            // 16. true.string
            case "true.string":
                val = new TextEncoder().encode(val);
                bufferAppendDatatype(SQLITE_DATATYPE_TEXT, 8 + val.byteLength);
                bufferSetBigint64(offset, BigInt(val.byteLength));
                offset += 8;
                offset += bufferSetBuffer(bufResult, val, offset);
                return;
            // 15. true.object
            default:
                assertOrThrow(
                    val && typeof val === "object",
                    "invalid data " + (typeof val) + " " + val
                );
                // write buffer
                if (ArrayBuffer.isView(val)) {
                    if (val.byteLength === 0) {
                        bufferAppendDatatype(SQLITE_DATATYPE_NULL, 0);
                        return;
                    }
                    bufferAppendDatatype(
                        SQLITE_DATATYPE_BLOB,
                        8 + val.byteLength
                    );
                    bufferSetBigint64(offset, BigInt(val.byteLength));
                    offset += 8;
                    // copy val to bufResult with offset
                    bufferSetBuffer(bufResult, val, offset);
                    offset += val.byteLength;
                    return;
                }
                // write JSON.stringify(val)
                val = String(
                    typeof val.toJSON === "function"
                    ? val.toJSON()
                    : JSON.stringify(val)
                );
                val = new TextEncoder().encode(val);
                bufferAppendDatatype(SQLITE_DATATYPE_TEXT, 8 + val.byteLength);
                bufferSetBigint64(offset, BigInt(val.byteLength));
                offset += 8;
                offset += bufferSetBuffer(bufResult, val, offset);
            }
        }
        // save bufResult
        serialize.bufResult = bufResult;
        // save bufSharedList
        serialize.bufSharedList = bufSharedList;
        return serialize;
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

    function testAll() {
// this function will run all tests
        testList.forEach(function (testFunc) {
            testFunc();
        });
    }

    function testAssertXxx() {
// this function will test assertXxx's handling-behavior
        // test assertNumericalEqual's handling-behavior
        assertNumericalEqual(1, 1);
        assertErrorThrownAsync(function () {
            assertNumericalEqual(0, 0);
        }, "value cannot be 0 or falsy");
        assertErrorThrownAsync(function () {
            assertNumericalEqual(1, 2);
        }, "1 != 2");
        assertErrorThrownAsync(function () {
            assertNumericalEqual(1, 2, "aa");
        }, "aa");
    }

    function testCcall() {
// this function will test cCall's handling-behavior
        [
            [-0, "0"],
            [-Infinity, "0"],
            [0, "0"],
            [1 / 0, "0"],
            [Infinity, "0"],
            [false, "0"],
            [null, "0"],
            [true, "1"],
            [undefined, "0"],
            [{}, "0"]
        ].forEach(async function ([
            aa, bb
        ]) {
            let cc;
            cc = String(
                await cCall("noopAsync", [
                    aa
                ])
            )[0][0];
            assertOrThrow(bb === cc, [aa, bb, cc]);
            cc = String(cCall("noopSync", [
                aa
            ]))[0][0];
            assertOrThrow(bb === cc, [aa, bb, cc]);
        });
    }

    async function testDbBind() {
// this function will test db's bind handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        async function testDbGetLastBlobAsync(val) {
            return await dbGetLastBlobAsync({
                bindList: [
                    val
                ],
                db,
                sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
            });
        }
        // test bigint-error handling-behavior
        noop([
            -(2n ** 63n),
            2n ** 63n
        ]).forEach(function (val) {
            assertErrorThrownAsync(testDbGetLastBlobAsync.bind(undefined, val));
        });
        // test datatype handling-behavior
        [
            // -1. SharedArrayBuffer
            // new SharedArrayBuffer(0), null,
            // 1. bigint
            -0n, -0,
            -0x7fffffffffffffffn, "-9223372036854775807",
            -1n, -1,
            -2n, -2,
            0n, 0,
            0x7fffffffffffffffn, "9223372036854775807",
            1n, 1,
            2n, 2,
            // 2. boolean
            false, 0,
            true, 1,
            // 3. function
            noop, null,
            // 4. number
            -0, 0,
            -1 / 0, null,
            -1e-999, 0,
            -1e999, null,
            -2, -2,
            -Infinity, null,
            -NaN, 0,
            0, 0,
            1 / 0, null,
            1e-999, 0,
            1e999, null,
            2, 2,
            Infinity, null,
            NaN, 0,
            // 5. object
            new Uint8Array(0), null,
            new TextEncoder().encode(""), null,
            new TextEncoder().encode("\u0000"), null,
            new TextEncoder().encode("\u0000\u{1f600}\u0000"), null,
            [], "[]",
            new Date(0), "1970-01-01T00:00:00.000Z",
            new RegExp(), "{}",
            null, null,
            {}, "{}",
            // 6. string
            "", "",
            "0", "0",
            "1", "1",
            "2", "2",
            "\u0000", "\u0000",
            "\u0000\u{1f600}\u0000", "\u0000\u{1f600}\u0000",
            "a".repeat(9999), "a".repeat(9999),
            // 7. symbol
            Symbol(), null,
            // 8. undefined
            undefined, null
        ].forEach(function (aa, ii, list) {
            let bb = list[ii + 1];
            if (ii % 2 === 1) {
                return;
            }
            ii *= 0.5;
            // test dbGetLastBlobAsync's bind handling-behavior
            [
                aa
            ].forEach(async function (aa) {
                let cc = String(bb);
                let dd = new TextDecoder().decode(
                    await testDbGetLastBlobAsync(aa)
                );
                switch (typeof(aa)) {
                case "bigint":
                    aa = Number(aa);
                    break;
                case "function":
                case "symbol":
                case "undefined":
                    cc = "";
                    break;
                case "number":
                    switch (aa) {
                    case -2:
                        cc = "-2.0";
                        break;
                    case -Infinity:
                        cc = "-Inf";
                        break;
                    case 2:
                        cc = "2.0";
                        break;
                    case Infinity:
                        cc = "Inf";
                        break;
                    }
                    break;
                case "object":
                    if (ArrayBuffer.isView(aa)) {
                        cc = new TextDecoder().decode(aa);
                        break;
                    }
                    if (aa === null) {
                        cc = "";
                    }
                    break;
                }
                // debugInline(ii, aa, bb, cc, dd);
                assertJsonEqual(cc, dd, {
                    ii,
                    aa, //jslint-quiet
                    bb,
                    cc,
                    dd
                });
            });
            // test dbGetLastMatrixDouble's bind handling-behavior
            [
                aa
            ].forEach(async function (aa) {
                let cc = Number(
                    typeof aa === "symbol"
                    ? 0
                    : aa
                ) || 0;
                let dd = await dbGetLastMatrixDouble({
                    bindList: [
                        aa
                    ],
                    db,
                    sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
                });
                switch (typeof(aa)) {
                case "bigint":
                    aa = String(aa);
                    break;
                case "object":
                    if (typeof aa?.getUTCFullYear === "function") {
                        cc = aa.getUTCFullYear();
                    }
                    break;
                }
                cc = new Float64Array([
                    1, 3, 1, 2, cc
                ]);
                // debugInline(ii, aa, bb, cc, dd);
                cc.forEach(function (val, jj) {
                    assertJsonEqual(
                        val,
                        dd[jj],
                        {
                            ii,
                            aa, //jslint-quiet
                            bb,
                            cc,
                            dd
                        }
                    );
                });
            });
            // test dbExecAsync's responseType handling-behavior
            [
                "arraybuffer",
                "list",
                undefined
            ].forEach(async function (responseType) {
                let cc = noop(
                    await dbExecAsync({
                        bindList: [
                            aa
                        ],
                        db,
                        responseType,
                        sql: "SELECT ? AS val"
                    })
                );
                // debugInline(ii, responseType, aa, bb, cc);
                switch (responseType) {
                case "arraybuffer":
                    cc = JSON.parse(new TextDecoder().decode(cc))[0][1][0];
                    break;
                case "list":
                    cc = cc[0][1][0];
                    break;
                default:
                    cc = cc[0][0].val;
                }
                assertJsonEqual(bb, cc, {
                    aa,
                    bb,
                    cc
                });
            });
            // test dbExecAsync's bind handling-behavior
            [
                [
                    [
                        bb, bb, 0
                    ],
                    (
                        "SELECT 0;"
                        + " SELECT ? AS c1, ? AS c2, ? AS c3, ? AS c4"
                        + " UNION ALL SELECT ?1, ?2, ?3, ?4"
                        + " UNION ALL SELECT ?1, ?2, ?3, ?4"
                    )
                ], [
                    {
                        k1: bb,
                        k2: bb,
                        k3: 0
                    },
                    (
                        "SELECT 0;"
                        + " SELECT $k1 AS c1, $k2 AS c2, $k3 AS c3, $k4 AS c4"
                        + " UNION ALL SELECT :k1, :k2, :k3, :k4"
                        + " UNION ALL SELECT @k1, @k2, @k3, @k4"
                    )
                ]
            ].forEach(async function ([
                bindList, sql
            ]) {
                let cc = noop(
                    await dbExecAsync({
                        bindList,
                        db,
                        responseType: "list",
                        sql
                    })
                );
                // debugInline(ii, aa, bb, cc);
                assertJsonEqual(
                    [
                        [
                            [
                                "0"
                            ], [
                                0
                            ]
                        ], [
                            [
                                "c1", "c2", "c3", "c4"
                            ], [
                                bb, bb, 0, undefined
                            ], [
                                bb, bb, 0, undefined
                            ], [
                                bb, bb, 0, undefined
                            ]
                        ]
                    ],
                    cc
                );
            });
            // test dbTableInsertAsync's bind handling-behavior
            [
                {
                    // test list-of-list handling-behavior
                    rowList: [
                        [
                            "c1", "c2", "c3"
                        ],
                        [
                            aa, aa
                        ]
                    ]
                }, {
                    // test list-of-dict handling-behavior
                    rowList: [
                        {
                            "c1": aa,
                            "c2": aa,
                            "c3": undefined
                        }
                    ]
                }, {
                    // test colList and list-of-list handling-behavior
                    colList: [
                        "c1", "c2", "c3"
                    ],
                    rowList: [
                        [
                            aa, aa
                        ]
                    ]
                }, {
                    // test colList and list-of-dict handling-behavior
                    colList: [
                        "c1", "c2", "c3"
                    ],
                    rowList: [
                        {
                            "c1": aa,
                            "c2": aa,
                            "c3": undefined
                        }
                    ]
                }, {
                    // test colList and list-of-list handling-behavior
                    colList: [
                        "c1", "c3", "c2"
                    ],
                    colListPriority: [
                        "c1", "c2"
                    ],
                    rowList: [
                        [
                            aa, undefined, aa
                        ]
                    ]
                }
            ].forEach(async function ({
                colList,
                colListPriority,
                rowList
            }, jj) {
                let cc = noop(
                    await dbExecAsync({
                        db,
                        responseType: "list",
                        sql: `SELECT * FROM datatype_${ii}_${jj}`,
                        tmpColList: colList,
                        tmpColListPriority: colListPriority,
                        tmpRowList: rowList,
                        tmpTableName: "datatype_" + ii + "_" + jj
                    })
                )[0];
                // debugInline(ii, jj, aa, bb, cc);
                assertJsonEqual([
                    [
                        "c1", "c2", "c3"
                    ], [
                        bb, bb, undefined
                    ]
                ], cc);
            });
        });
    }

    async function testDbCloseAsync() {
// this function will test dbCloseAsync's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbCloseAsync({});
        }, "invalid or closed db");
        // test close handling-behavior
        await dbCloseAsync({
            db
        });
    }

    async function testDbExecAsync() {
// this function will test dbExecAsync's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecAsync({
                db,
                sql: undefined
            });
        }, "near \"undefined\": syntax error");
        // test race-condition handling-behavior
        Array.from(new Array(4)).forEach(async function () {
            let result;
            try {
                result = JSON.stringify(
                    await dbExecAsync({
                        bindList: [
                            new TextEncoder().encode("foob"),
                            new TextEncoder().encode("fooba"),
                            new TextEncoder().encode("foobar")
                        ],
                        db,
                        responseType: "list",
                        sql: (`
CREATE TABLE testDbExecAsync1 AS
SELECT 101 AS c101, 102 AS c102
--
UNION ALL
VALUES
    (201, 202),
    (301, NULL);
CREATE TABLE testDbExecAsync2 AS
SELECT 401 AS c401, 402 AS c402, 403 AS c403
--
UNION ALL
VALUES
    (501, 502.0123, 5030123456789),
    (601, '602', '603_\"\x01\x08\x09\x0a\x0b\x0c\x0d\x0e'),
    (?1, ?2, ?3),
    (tostring(?1), tostring(?2), tostring(?3)),
    (tobase64(?1), tobase64(?2), tobase64(?3)),
    (
        tobase64(uncompress(compress(?1))),
        tobase64(uncompress(compress(?2))),
        tobase64(uncompress(compress(?3)))
    );
SELECT * FROM testDbExecAsync1;
SELECT * FROM testDbExecAsync2;
                        `)
                    })
                );
                assertJsonEqual(result, JSON.stringify([
                    [
                        ["c101", "c102"],
                        [101, 102],
                        [201, 202],
                        [301, null]
                    ],
                    [
                        ["c401", "c402", "c403"],
                        [401, 402, 403],
                        [501, 502.0123, 5030123456789],
                        [601, "602", "603_\"\u0001\b\t\n\u000b\f\r\u000e"],
                        [
                            null, null, null
                        ],
                        [
                            "foob", "fooba", "foobar"
                        ],
                        [
                            "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
                        ],
                        [
                            "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
                        ]
                    ]
                ]));
            } catch (err) {
                assertOrThrow(
                    err.message.indexOf(
                        "table testDbExecAsync1 already exists"
                    ) >= 0,
                    err
                );
            }
        });
        // test close-while-busy handling-behavior
        assertErrorThrownAsync(function () {
            return dbCloseAsync({
                db
            });
        }, (
            /db cannot close with \d+? actions pending/
        ));
        // test sqlmath-defined-func handling-behavior
        [
            "COT(NULL)", null,
            "COT('-1')", -0.642092615934331,
            "COT('0')", null,
            "COT('1')", 0.642092615934331,
            "COT(-1)", -0.642092615934331,
            "COT(0)", null,
            "COT(1)", 0.642092615934331,
            "COTH(NULL)", null,
            "COTH('-1')", -1.31303528549933,
            "COTH('0')", null,
            "COTH('1')", 1.31303528549933,
            "COTH(-1)", -1.31303528549933,
            "COTH(0)", null,
            "COTH(1)", 1.31303528549933,
            "ROUNDORZERO(NULL)", 0,
            "ROUNDORZERO(NULL, NULL)", 0,
            "ROUNDORZERO(NULL, 0.5)", 0,
            "ROUNDORZERO(0.5, NULL)", 1,
            "ROUNDORZERO(0.5, 0.5)", 1,
            "ROUNDORZERO(0.5, 1)", 0.5,
            "SIGN(NULL)", null,
            "SIGN('-1')", -1,
            "SIGN('0')", 0,
            "SIGN('1')", 1,
            "SIGN(-0x7fffffffffffffff)", -1,
            "SIGN(-1)", -1,
            "SIGN(-1e999)", -1,
            "SIGN(0)", 0,
            "SIGN(0x7fffffffffffffff)", 1,
            "SIGN(0x8000000000000000)", -1,
            "SIGN(0xffffffffffffffff)", -1,
            "SIGN(1)", 1,
            "SIGN(1e999)", 1,
            // sentinel
            "NULL", null
        ].forEach(async function (sql, ii, list) {
            let bb = list[ii + 1];
            let cc;
            if (ii % 2 === 1) {
                return;
            }
            ii *= 0.5;
            cc = noop(
                await dbExecAsync({
                    db,
                    responseType: "dict",
                    sql: `SELECT ${sql} AS val`
                })
            )[0][0].val;
            assertJsonEqual(bb, cc, {
                bb,
                cc,
                ii,
                sql
            });
        });
    }

    function testDbExecWithRetryAsync() {
// this function will test dbExecWithRetryAsync's handling-behavior
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecWithRetryAsync({});
        }, "invalid or closed db");
    }

    async function testDbMemoryXxx() {
// this function will test dbMemoryXxx's handling-behavior
        let data;
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbMemoryLoadAsync({
                db
            });
        }, "invalid filename undefined");
        assertErrorThrownAsync(function () {
            return dbMemorySaveAsync({
                db
            });
        }, "invalid filename undefined");
        await dbExecAsync({
            db,
            sql: "CREATE TABLE t01 AS SELECT 1 AS c01"
        });
        await dbMemorySaveAsync({
            db,
            filename: ".testDbMemoryXxx.sqlite"
        });
        db = await dbOpenAsync({
            filename: ":memory:"
        });
        await dbMemoryLoadAsync({
            db,
            filename: ".testDbMemoryXxx.sqlite"
        });
        data = await dbExecAsync({
            db,
            sql: "SELECT * FROM t01"
        });
        assertJsonEqual(data, [
            [
                {
                    c01: 1
                }
            ]
        ]);
    }

    function testDbOpenAsync() {
// this function will test dbOpenAsync's handling-behavior
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbOpenAsync({});
        }, "invalid filename undefined");
    }

    async function testDbTableInsertAsync() {
// this function will test dbTableInsertAsync's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test error handling-behavior
        [
            [
                undefined,
                (
                    /invalid rowList undefined/
                )
            ], [
                [
                    []
                ],
                (
                    /invalid colList \[\]/
                )
            ], [
                [
                    {}
                ],
                (
                    /invalid colList \[\]/
                )
            ]
        ].forEach(function ([
            rowList, rgx
        ]) {
            assertErrorThrownAsync(
                dbTableInsertAsync.bind(
                    undefined,
                    {
                        rowList
                    }
                ),
                rgx
            );
        });
        // test csv handling-behavior
        [
            [
                "0", undefined
            ],
            [
                "0,0,0\n1,1,1",
                [
                    [
                        "c_0", "c_0_2", "c_0_3"
                    ], [
                        "1", "1", "1"
                    ]
                ]
            ],
            [
                (
                    "c1,c1,c2\n"
                    + "1, 2 \n"
                    + `"1","""2""","3\r\n"\n`
                    + "\n"
                    + "1,2,3\n"
                ),
                [
                    [
                        "c1", "c1_2", "c2"
                    ], [
                        "1", " 2 ", null
                    ], [
                        "1", "\"2\"", "3\n"
                    ], [
                        "", null, null
                    ], [
                        "1", "2", "3"
                    ]
                ]
            ]
        ].forEach(async function ([
            aa, bb
        ], ii) {
            let cc = noop(
                await dbExecAsync({
                    db,
                    responseType: "list",
                    sql: `SELECT * FROM temp.csv_${ii}`,
                    tmpCsv: aa,
                    tmpTableName: "csv_" + ii
                })
            )[0];
            assertOrThrow(
                JSON.stringify(bb) === JSON.stringify(cc),
                JSON.stringify([
                    ii, aa, bb, cc
                ], undefined, 4)
            );
        });
    }

    async function testSqlError() {
// this function will test sql-error's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
            1, "SQL logic error",
            2, "unknown error",
            3, "access permission denied",
            4, "query aborted",
            5, "database is locked",
            6, "database table is locked",
            7, "out of memory",
            8, "attempt to write a readonly database",
            9, "interrupted",
            10, "disk I/O error",
            11, "database disk image is malformed",
            12, "unknown operation",
            13, "database or disk is full",
            14, "unable to open database file",
            15, "locking protocol",
            16, "unknown error",
            17, "database schema has changed",
            18, "string or blob too big",
            19, "constraint failed",
            20, "datatype mismatch",
            21, "bad parameter or other API misuse",
            22, "unknown error",
            23, "authorization denied",
            24, "unknown error",
            25, "column index out of range",
            26, "file is not a database",
            27, "notification message",
            28, "warning message",
            100, "unknown error",
            101, "unknown error"
        ].forEach(function (sql, ii, list) {
            let bb = list[ii + 1];
            if (ii % 2 === 1) {
                return;
            }
            ii *= 0.5;
            sql = `SELECT throwerror(${sql})`;
            assertErrorThrownAsync(function () {
                return dbExecAsync({
                    db,
                    sql
                });
            }, bb);
        });
    }

    async function testSqlExt() {
// this function will test sql-ext's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
            {
                aa: [
                    [
                        {
                            "c01": ""
                        }
                    ]
                ],
                sql: "SELECT tobase64(NULL) AS c01"
            },
            {
                aa: [
                    [
                        {
                            "c01": ""
                        }
                    ]
                ],
                sql: "SELECT tobase64(?) AS c01"
            },
            {
                aa: [
                    [
                        {
                            "c01": "AAAAAAAAAAA="
                        }
                    ]
                ],
                bindList: [
                    new Uint8Array(8)
                ],
                sql: "SELECT tobase64(uncompress(compress(?))) AS c01"
            }
        ].forEach(async function ({
            aa,
            bindList,
            sql
        }) {
            let bb = await dbExecAsync({
                bindList,
                db,
                sql
            });
            assertJsonEqual(aa, bb);
        });
    }

    async function testSqlKthpercentile() {
// this function will test sql-kthpercentile's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
            [
                [], -99, 1
            ], [
                [], 0, 1
            ], [
                [], 0.125, 1
            ], [
                [], 0.25, 2
            ], [
                [], 0.375, 3
            ], [
                [], 0.5, 4
            ], [
                [], 0.625, 5
            ], [
                [], 0.75, 6
            ], [
                [], 0.875, 7
            ], [
                [], 1, 8
            ], [
                [], 99, 8
            ], [
                [
                    0.5
                ], 0, 0.5
            ], [
                [
                    0.5
                ], 0.125, 0.5
            ], [
                [
                    1.5
                ], 0.25, 1.5
            ], [
                [
                    2.5
                ], 0.375, 2.5
            ], [
                [
                    3.5
                ], 0.5, 3.5
            ], [
                [
                    4.5
                ], 0.625, 4.5
            ], [
                [
                    5.5
                ], 0.75, 5.5
            ], [
                [
                    6.5
                ], 0.875, 6.5
            ], [
                [
                    7.5
                ], 1, 8
            ]
        ].forEach(async function ([
            data, kk, expected
        ], ii) {
            let actual;
            data = data.concat([
                undefined, undefined, 8, 7, 6, 5, 4, 3, 2, 1, undefined
            ]);
            actual = noop(
                await dbExecAsync({
                    db,
                    sql: (`
SELECT kthpercentile(val, ${kk}) AS val FROM __tmp${ii};
                    `),
                    tmpRowList: data.map(function (val) {
                        return {
                            val
                        };
                    }),
                    tmpTableName: `__tmp${ii}`
                })
            )[0][0].val;
            assertJsonEqual(actual, expected, {
                data,
                kk
            });
        });
    }

    addon = requireCjs(
        "./_binary_sqlmath"
        + "_napi8"
        + "_" + process.platform
        + "_" + process.arch
        + ".node"
    );
    testList = [
        testAssertXxx,
        testCcall,
        testDbBind,
        testDbCloseAsync,
        testDbExecAsync,
        testDbExecWithRetryAsync,
        testDbMemoryXxx,
        testDbOpenAsync,
        testDbTableInsertAsync,
        testSqlError,
        testSqlExt,
        testSqlKthpercentile
    ];
    Object.assign(local, {
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
        assertErrorThrownAsync,
        assertNumericalEqual,
        dbCloseAsync,
        dbExecAsync,
        dbExecWithRetryAsync,
        dbGetLastBlobAsync,
        dbMemoryLoadAsync,
        dbMemorySaveAsync,
        dbOpenAsync,
        dbTableInsertAsync,
        debugInline,
        objectDeepCopyWithKeysSorted,
        testAll,
        testList
    });
    if (process.env.npm_config_mode_test) {
        // mock consoleError
        consoleError = noop;
    }
}());

export default Object.freeze(local);
