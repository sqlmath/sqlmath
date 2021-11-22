/*jslint beta, bitwise, name, node*/
"use strict";
import {createRequire} from "module";
let local = {};


/*
file debugInline.js
*/
(function () {
    let consoleError = function () {
        return;
    };
    function debugInline(...argList) {
// this function will both print <argList> to stderr and return <argList>[0]
        consoleError("\n\ndebugInline");
        consoleError(...argList);
        consoleError("\n");
        return argList[0];
    }
    // coverage-hack
    debugInline();
    consoleError = console.error;
    globalThis.debugInline = globalThis.debugInline || debugInline;
}());


/*
file helper.js
*/
(function () {
    function assertErrorThrownAsync(func, onError) {
// this function will assert <func> throws error
        let isCaught;
        func().catch(function (err) {
            isCaught = true;
            onError(err);
        }).finally(function () {
            assertOrThrow(isCaught, "no error caught");
        });
    }

    function assertJsonEqual(aa, bb) {
// this function will assert JSON.stringify(<aa>) === JSON.stringify(<bb>)
        aa = JSON.stringify(objectDeepCopyWithKeysSorted(aa));
        bb = JSON.stringify(objectDeepCopyWithKeysSorted(bb));
        if (aa !== bb) {
            throw new Error(
                JSON.stringify(aa) + " !== " + JSON.stringify(bb)
            );
        }
    }

    function assertOrThrow(cond, msg) {
// this function will throw <msg> if <cond> is falsy
        if (!cond) {
            throw (
                typeof msg === "string"
                ? new Error(msg)
                : msg
            );
        }
    }

    function noop(val) {
// this function will do nothing except return val
        return val;
    }

    function objectDeepCopyWithKeysSorted(obj) {
// this function will recursively deep-copy <obj> with keys sorted
        let sorted;
        if (typeof obj !== "object" || !obj) {
            return obj;
        }
        // recursively deep-copy list with child-keys sorted
        if (Array.isArray(obj)) {
            return obj.map(objectDeepCopyWithKeysSorted);
        }
        // recursively deep-copy obj with keys sorted
        sorted = {};
        Object.keys(obj).sort().forEach(function (key) {
            sorted[key] = objectDeepCopyWithKeysSorted(obj[key]);
        });
        return sorted;
    }

    Object.assign(local, {
        assertErrorThrownAsync,
        assertJsonEqual,
        assertOrThrow,
        noop,
        objectDeepCopyWithKeysSorted
    });
}());


/*
file sqlmath.js
*/
(function () {
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
    let {
        assertErrorThrownAsync,
        assertJsonEqual,
        assertOrThrow,
        noop
    } = local;
    let dbDict = new WeakMap();
    let requireCjs = createRequire(import.meta.url);
    // private map of sqlite-database-connections

    function bufferFromView(view) {
// this function will recast <view> to nodejs buffer-object
        return (
            (ArrayBuffer.isView(view) && !Buffer.isBuffer(view))
            ? Buffer.from(view.buffer, view.byteOffset, view.byteLength)
            : view
        );
    }

    function cCall(func, argList) {
// this function will serialize <argList> to a c <baton>,
// suitable for passing into napi
        let baton = new BigInt64Array(1024);
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
            case "object":
                arg = bufferFromView(arg);
                if (Buffer.isBuffer(arg)) {
                    baton[ii] = BigInt(arg.byteLength);
                    return arg;
                }
                break;
            case "string":
                // append null-terminator to string
                return Buffer.from(arg + "\u0000");
            }
            return arg;
        });
        // pad argList to length = 8
        argList = argList.concat([
            undefined, undefined, undefined, undefined,
            undefined, undefined, undefined, undefined
        ]).slice(0, 8);
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
        sql
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
        Object.entries(bindList).forEach(function ([
            key, val
        ]) {
            if (bindByKey) {
                serialize(":" + key + "\u0000");
            }
            serialize(val);
        });
        result = noop(
            await dbCallAsync("__dbExecAsync", db, [
                String(sql),
                bindListLength,
                Buffer.from(serialize.buf),
                bindByKey,
                (
                    responseType === "lastBlob"
                    ? 1
                    : responseType === "lastMatrixDouble"
                    ? 2
                    : 0
                )
            ])
        )[1];
        switch (responseType) {
        case "arraybuffer":
        case "lastBlob":
            return result;
        case "lastMatrixDouble":
            return new Float64Array(result);
        case "list":
            return JSON.parse(Buffer.from(result));
        default:
            result = JSON.parse(Buffer.from(result));
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
                String(filename), undefined, flags ?? (
                    SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
                ), undefined
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
        tableName = "tmp1"
    }) {
// this function will create-or-replace temp <tablename> with <rowList>
        let serialize = jsToSqlSerializer();
        let sqlCreateTable;
        let sqlInsertRow;
        // normalize and validate tableName
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
            Buffer.from(serialize.buf),
            colList.length,
            rowList.length
        ]);
    }

    function jsToSqlSerializer() {
// this function will return another function that serializes javascript <val>
// to <buf> as sqlite-values
        let SQLITE_DATATYPE_BLOB = 0x04;
        // let SQLITE_DATATYPE_BLOB_0 = 0x14;
        let SQLITE_DATATYPE_FLOAT = 0x02;
        // let SQLITE_DATATYPE_FLOAT_0 = 0x12;
        let SQLITE_DATATYPE_INTEGER = 0x01;
        let SQLITE_DATATYPE_INTEGER_0 = 0x11;
        let SQLITE_DATATYPE_INTEGER_1 = 0x21;
        let SQLITE_DATATYPE_NULL = 0x05;
        let SQLITE_DATATYPE_TEXT = 0x03;
        let SQLITE_DATATYPE_TEXT_0 = 0x13;
        let buf = Buffer.allocUnsafe(1024);
        let offset = 0;
        function bufferAppendDatatype(datatype, byteLength) {
// this function will grow <buf> by <bytelength> and append <datatype>
            let nn = offset + 1 + byteLength;
            let tmp;
            // exponentially grow buf as needed
            if (buf.byteLength < nn) {
                assertOrThrow(nn <= SQLITE_MAX_LENGTH2, (
                    "sqlite - string or blob exceeds size limit of "
                    + SQLITE_MAX_LENGTH2 + " bytes"
                ));
                tmp = Buffer.allocUnsafe(
                    Math.min(2 ** Math.ceil(Math.log2(nn)), SQLITE_MAX_LENGTH2)
                );
                buf.copy(tmp);
                buf = tmp;
                // save buf
                serialize.buf = buf;
            }
            buf[offset] = datatype;
            offset += 1;
        }
        function serialize(val) {
// this function will write to <buf>, <val> at given <offset>
            let byteLength = 0;
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
                // try to write bigint as finite number
                try {
                    offset = buf.writeBigInt64LE(val, offset);
                // else write as +/-infinity
                } catch (ignore) {
                    buf[offset - 1] = SQLITE_DATATYPE_FLOAT;
                    offset = buf.writeDoubleLE((
                        val > 0
                        ? Infinity
                        : -Infinity
                    ), offset);
                }
                return;
            // 14. true.number
            case "true.number":
                bufferAppendDatatype(SQLITE_DATATYPE_FLOAT, 8);
                offset = buf.writeDoubleLE(val, offset);
                return;
            // 16. true.string
            case "true.string":
                byteLength = Buffer.byteLength(val);
                bufferAppendDatatype(SQLITE_DATATYPE_TEXT, 8 + byteLength);
                offset = buf.writeBigInt64LE(BigInt(byteLength), offset);
                offset += buf.write(val, offset);
                return;
            // 15. true.object
            default:
                assertOrThrow(
                    val && typeof val === "object",
                    "invalid data " + (typeof val) + " " + val
                );
                // write buffer
                val = bufferFromView(val);
                if (Buffer.isBuffer(val)) {
                    if (val.byteLength === 0) {
                        bufferAppendDatatype(SQLITE_DATATYPE_NULL, 0);
                        return;
                    }
                    byteLength = val.byteLength;
                    bufferAppendDatatype(SQLITE_DATATYPE_BLOB, 8 + byteLength);
                    offset = buf.writeBigInt64LE(BigInt(byteLength), offset);
                    offset += val.copy(buf, offset);
                    return;
                }
                // write JSON.stringify(val)
                val = String(
                    typeof val.toJSON === "function"
                    ? val.toJSON()
                    : JSON.stringify(val)
                );
                byteLength = Buffer.byteLength(val);
                bufferAppendDatatype(SQLITE_DATATYPE_TEXT, 8 + byteLength);
                offset = buf.writeBigInt64LE(BigInt(byteLength), offset);
                offset += buf.write(val, offset);
            }
        }
        // save buf
        serialize.buf = buf;
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
        testAssertXxx();
        testCcall();
        testDbBind();
        testDbCloseAsync();
        testDbExecAsync();
        testDbOpenAsync();
        testDbTableInsertAsync();
        testSqlcSlrOhlcv();
    }

    function testAssertXxx() {
// this function will test assertXxx's handling-behavior
        // test assertErrorThrownAsync's handling-behavior
        assertErrorThrownAsync(function () {
            return new Promise(function (ignore, reject) {
                reject(new Error());
            });
        }, function (err) {
            assertOrThrow(err, err);
        });
        // test assertJsonEqual handling-behavior
        assertJsonEqual([
            {
                aa: 1,
                bb: 2
            }
        ], [
            {
                bb: 2,
                aa: 1 //jslint-quiet
            }
        ]);
        try {
            assertJsonEqual("aa", "bb");
        } catch (err) {
            assertJsonEqual(err.message, "\"\\\"aa\\\"\" !== \"\\\"bb\\\"\"");
        }
        // test assertOrThrow handling-behavior
        [
            [
                "aa", "aa"
            ], [
                new Error("aa"), "aa"
            ]
        ].forEach(function ([
            err, msg
        ]) {
            try {
                assertOrThrow(undefined, err);
            } catch (err2) {
                assertJsonEqual(err2.message, msg);
            }
        });
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
        // test datatype handling-behavior
        [
            // 1. bigint
            -0n, -0,
            -0x7fffffffffffffffn, "-9223372036854775807",
            -1n, -1,
            -2n, -2,
            -BigInt(1e64), null,
            0n, 0,
            0x7fffffffffffffffn, "9223372036854775807",
            1n, 1,
            2n, 2,
            BigInt(1e64), null,
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
            Buffer.alloc(0), null,
            Buffer.from(""), null,
            Buffer.from("\u0000"), (
                "data:application/octet-stream;base64,"
                + Buffer.from("\u0000").toString("base64")
            ),
            Buffer.from("\u0000\u{1f600}\u0000"), (
                "data:application/octet-stream;base64,"
                + Buffer.from("\u0000\u{1f600}\u0000").toString("base64")
            ),
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
                let dd = String(Buffer.from(
                    await dbGetLastBlobAsync({
                        bindList: [
                            aa
                        ],
                        db,
                        sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
                    })
                ));
                switch (typeof(aa)) {
                case "bigint":
                    switch (aa) {
                    case -BigInt(1e64):
                        cc = "-Inf";
                        break;
                    case BigInt(1e64):
                        cc = "Inf";
                        break;
                    }
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
                    if (Buffer.isBuffer(aa)) {
                        cc = String(aa);
                        break;
                    }
                    if (aa === null) {
                        cc = "";
                    }
                    break;
                }
                // debugInline(ii, aa, bb, cc, dd);
                assertJsonEqual(cc, dd);
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
                    switch (aa) {
                    case -BigInt(1e64):
                        cc = -Infinity;
                        break;
                    case BigInt(1e64):
                        cc = Infinity;
                        break;
                    }
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
                    assertOrThrow(val === dd[jj], JSON.stringify([
                        ii, String(aa), bb, String(cc), String(dd)
                    ]));
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
                    cc = JSON.parse(Buffer.from(cc))[0][1][0];
                    break;
                case "list":
                    cc = cc[0][1][0];
                    break;
                default:
                    cc = cc[0][0].val;
                }
                assertOrThrow(bb === cc, [aa, bb, cc]);
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
                assertJsonEqual([
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
                ], cc);
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
                let cc;
                await dbTableInsertAsync({
                    colList,
                    colListPriority,
                    db,
                    rowList,
                    tableName: "datatype_" + ii + "_" + jj
                });
                cc = noop(
                    await dbExecAsync({
                        db,
                        responseType: "list",
                        sql: "SELECT * FROM datatype_" + ii + "_" + jj
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
        }, function (err) {
            assertOrThrow((
                /invalid\sor\sclosed\sdb/
            ).test(err.message), err);
        });
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
        }, function (err) {
            assertOrThrow((
                /sqlite\s-\snear\s"undefined":\ssyntax\serror/
            ).test(err.message), err);
        });
        // test race-condition handling-behavior
        Array.from(new Array(4)).forEach(async function () {
            let result;
            try {
                result = JSON.stringify(
                    await dbExecAsync({
                        bindList: [
                            Buffer.from("foob"),
                            Buffer.from("fooba"),
                            Buffer.from("foobar")
                        ],
                        db,
                        responseType: "list",
                        sql: (`
CREATE TABLE testDbExecAsync1 AS
SELECT 101 AS c101, 102 AS c102
UNION ALL
VALUES (201, 202),
       (301, NULL);
CREATE TABLE testDbExecAsync2 AS
SELECT 401 AS c401, 402 AS c402, 403 AS c403
UNION ALL
VALUES (501, 502.0123, 5030123456789),
       (601, '602', '603_\"\x01\x08\x09\x0a\x0b\x0c\x0d\x0e'),
       (?1, ?2, ?3),
       (tobase64(?1), tobase64(?2), tobase64(?3)),
       (tostring(?1), tostring(?2), tostring(?3));
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
                            "data:application/octet-stream;base64,Zm9vYg==",
                            "data:application/octet-stream;base64,Zm9vYmE=",
                            "data:application/octet-stream;base64,Zm9vYmFy"
                        ],
                        [
                            "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
                        ],
                        [
                            "foob", "fooba", "foobar"
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
        }, function (err) {
            assertOrThrow((
                /db\scannot\sclose\swith\s\d+?\sactions\spending/
            ).test(err.message), err);
        });
        // test sqlmath-defined-func handling-behavior
        [
            "COT('-1')", -0.642092615934331,
            "COT('0')", null,
            "COT('1')", 0.642092615934331,
            "COT(-1)", -0.642092615934331,
            "COT(0)", null,
            "COT(1)", 0.642092615934331,
            "COTH('-1')", -1.31303528549933,
            "COTH('0')", null,
            "COTH('1')", 1.31303528549933,
            "COTH(-1)", -1.31303528549933,
            "COTH(0)", null,
            "COTH(1)", 1.31303528549933,
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
            "SIGN(NULL)", null,
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
                    sql: "SELECT " + sql + " AS val"
                })
            )[0][0].val;
            assertOrThrow(bb === cc, JSON.stringify([
                ii, sql, bb, cc
            ]));
        });
    }

    function testDbOpenAsync() {
// this function will test dbOpenAsync's handling-behavior
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbOpenAsync({});
        }, function (err) {
            assertOrThrow((
                /invalid\sfilename\sundefined/
            ).test(err.message), err);
        });
    }

    async function testDbTableInsertAsync() {
// this function will test testDbTableInsertAsync's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test error handling-behavior
        [
            [
                undefined,
                (
                    /invalid\srowList\sundefined/
                )
            ], [
                [
                    []
                ],
                (
                    /invalid\scolList\s\[\]/
                )
            ], [
                [
                    {}
                ],
                (
                    /invalid\scolList\s\[\]/
                )
            ]
        ].forEach(function ([
            rowList, rgx
        ]) {
            assertErrorThrownAsync(function () {
                return dbTableInsertAsync({
                    rowList
                });
            }, function (err) {
                assertOrThrow(rgx.test(err.message), err);
            });
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
            let cc;
            await dbTableInsertAsync({
                csv: aa,
                db,
                tableName: "csv_" + ii
            });
            cc = noop(
                await dbExecAsync({
                    db,
                    responseType: "list",
                    sql: "SELECT * FROM temp.csv_" + ii
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

    async function testSqlcSlrOhlcv() {
// this function will test testSqlSlrOhlcv's handling-behavior
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        let structSlrxy = [
            "caa", "cbb", "crr",
            "dxx",
            "mxx", "myy",
            "nn",
            "offset",
            "saa", "sbb", "see", "sxx", "sxy", "syy",
            "xxhh", "xxll",
            "xxxx"
        ];
        function validateSimpleLinearRegression({
            aa,
            xxList,
            yyList
        }) {
// this function will run validate simple-linear-regression <bb>
// from <xxList> and <yyList> matches <aa>
            let bb;
            let caa = 0;
            let cbb = 0;
            let crr = 0;
            let dxx = 0;
            let mxx = 0;
            let myy = 0;
            let nn = 0;
            let saa = 0;
            let sbb = 0;
            let see = 0;
            let sxx = 0;
            let sxy = 0;
            let syy = 0;
            let xxxx = 0;
            // calculate sxx, syy, sxy
            xxList.forEach(function (xx, ii) {
                let yy = yyList[ii];
                nn += 1;
                xxxx += xx * xx;
                // wellford - calculate syy
                dxx = yy - myy;
                myy += dxx / nn;
                syy += dxx * (yy - myy);
                // wellford - calculate sxx
                dxx = xx - mxx;
                mxx += dxx / nn;
                sxx += dxx * (xx - mxx);
                // wellford - calculate sxy
                sxy += dxx * (yy - myy);
            });
            // calculate caa, cbb, crr
            cbb = sxy / sxx;
            caa = myy - cbb * mxx;
            crr = sxy / Math.sqrt(sxx * syy);
            // calculate see = rss / (nn - 2)
            xxList.forEach(function (xx, ii) {
                let yy = yyList[ii];
                see += Math.pow(caa + cbb * xx - yy, 2);
                ii += 2;
            });
            see = see / (nn - 2);
            // calculate sbb = see / sxx
            sbb = see / sxx;
            // calculate saa = see * xxxx / (nn * sxx) = sbb * xxxx / nn
            saa = sbb * (xxxx / nn);
            bb = {
                caa,
                cbb,
                crr,
                mxx,
                myy,
                saa,
                sbb,
                see
            };
            [
                "mxx", "myy", "cbb", "caa", "crr", "see", "sbb", "saa"
            ].forEach(function (key) {
                aa[key] = String(aa[key]).slice(0, 8);
                bb[key] = String(bb[key]).slice(0, 8);
                assertOrThrow(aa[key] === bb[key], JSON.stringify([
                    key, aa[key], bb[key]
                ]));
            });
        }
        validateSimpleLinearRegression({
            aa: {
                caa: -39.06191,
                cbb: 61.272181,
                crr: 0.9945831,
                mxx: 1.6506661,
                myy: 62.077991,
                saa: 8.6318501,
                sbb: 3.1539011,
                see: 0.5761961
            },
            xxList: [
                1.47, 1.50, 1.52, 1.55, 1.57, 1.60, 1.63, 1.65,
                1.68, 1.70, 1.73, 1.75, 1.78, 1.80, 1.83
            ],
            yyList: [
                52.21, 53.12, 54.48, 55.84, 57.20, 58.57, 59.93, 61.29,
                63.11, 64.47, 66.28, 68.10, 69.92, 72.19, 74.46
            ]
        });
        validateSimpleLinearRegression({
            aa: {
                caa: -39.74681,
                cbb: 61.674631,
                crr: 0.9954871,
                mxx: 1.6509991,
                myy: 62.077991,
                saa: 7.2784471,
                sbb: 2.6584591,
                see: 0.4802361
            },
            xxList: [
                1.47, 1.50, 1.52, 1.55, 1.57, 1.60, 1.63, 1.65,
                1.68, 1.70, 1.73, 1.75, 1.78, 1.80, 1.83
            ].map(function (elem) {
                return 0.0254 * Math.round(elem / 0.0254);
            }),
            yyList: [
                52.21, 53.12, 54.48, 55.84, 57.20, 58.57, 59.93, 61.29,
                63.11, 64.47, 66.28, 68.10, 69.92, 72.19, 74.46
            ]
        });
        [
            [
                {
                    "0": 1,
                    "1": 1,
                    "2": 2,
                    "3": 2,
                    "4": 3,
                    "5": 3
                },
                [
                    {
                        offset: 0,
                        xxList: [
                            1, 2, 3, 4, 5
                        ],
                        yyList: [
                            1, 2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 1,
                        xxList: [
                            1, 2, 3, 4, 5
                        ],
                        yyList: [
                            1, 2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 7,
                        xxList: [
                            1, 2, 3, 4, 5
                        ],
                        yyList: [
                            1, 2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 8,
                        xxList: [],
                        yyList: []
                    },
                    {
                        offset: 25 * 8 - 1,
                        xxList: [],
                        yyList: []
                    }
                ]
            ],
            [
                {
                    "30": 1,
                    "31": 1,
                    "32": 2,
                    "33": 2,
                    "34": 3,
                    "35": 3
                },
                [
                    {
                        offset: 0,
                        xxList: [
                            30, 31
                        ],
                        yyList: [
                            1, 1
                        ]
                    },
                    {
                        offset: 1,
                        xxList: [
                            30, 31, 32, 33, 34, 35
                        ],
                        yyList: [
                            1, 1, 2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 7,
                        xxList: [
                            30, 31, 32, 33, 34, 35
                        ],
                        yyList: [
                            1, 1, 2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 8,
                        xxList: [
                            32, 33, 34, 35
                        ],
                        yyList: [
                            2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 15,
                        xxList: [
                            32, 33, 34, 35
                        ],
                        yyList: [
                            2, 2, 3, 3
                        ]
                    },
                    {
                        offset: 16,
                        xxList: [],
                        yyList: []
                    },
                    {
                        offset: 25 * 8 - 1,
                        xxList: [],
                        yyList: []
                    }
                ]
            ]
        ].forEach(function ([
            ohlcvDict, slrList
        ]) {
            [
                0, 1, 2, 3
            ].forEach(async function (ll) {
                let buf = new Float64Array(
                    2 + 5 + 5 * Object.keys(ohlcvDict)[
                        Object.keys(ohlcvDict).length - 1
                    ]
                );
                buf[0] = (buf.length - 2) / 5;
                buf[1] = 5;
                Object.entries(ohlcvDict).forEach(function ([
                    key, val
                ]) {
                    buf[2 + key * 5 + ll] = val;
                });
                //!! debugInline(buf);
                buf = new Float64Array(await dbGetLastBlobAsync({
                    bindList: [
                        buf
                    ],
                    db,
                    sql: "SELECT(slr_ohlcv(?)) AS blob;"
                }));
                slrList.forEach(function ({
                    offset,
                    xxList,
                    yyList
                }) {
                    let aa = {};
                    structSlrxy.forEach(function (key, ii) {
                        aa[key] = buf[ii + offset * structSlrxy.length];
                    });
                    //!! debugInline(buf);
                    validateSimpleLinearRegression({
                        aa,
                        xxList: xxList.map(function (kk) {
                            return (
                                // price - open
                                ll === 0
                                ? kk + 0.270833333333333333
                                // price - high, low
                                : (ll === 1 || ll === 2)
                                ? kk + 0.135416666666666666
                                // price - close
                                : kk
                            );
                        }),
                        yyList
                    });
                });
            });
        });
    }

    addon = requireCjs(
        "./_binary_sqlmath_napi"
        + "_" + process.versions.napi
        + "_" + process.platform
        + "_" + process.arch
        + ".node"
    );
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
        dbCloseAsync,
        dbExecAsync,
        dbOpenAsync,
        dbTableInsertAsync,
        testAll
    });
}());

export default Object.freeze(local);
