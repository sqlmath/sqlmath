/*jslint beta, debug, name, node*/
"use strict";
// init debugInline
(function () {
    let consoleError = console.error;
    globalThis.debugInline = globalThis.debugInline || function (...argList) {
// this function will both print <argList> to stderr and return <argList>[0]
        consoleError("\n\ndebugInline");
        consoleError(...argList);
        consoleError("\n");
        return argList[0];
    };
}());

function noop(val) {
// this function will do nothing except return val
    return val;
}

(function () {
    let SQLITE_MAX_LENGTH = 1000000000;
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

    let addon = require(
        "./_binary_sqlmath_napi"
        + "_" + process.versions.napi
        + "_" + process.platform
        + "_" + process.arch
        + ".node"
    );
    // private map of sqlite-database-connections
    let dbOpenMap = new WeakMap();

    function __dbBusyDec(db) {
// this function will decrement db.busy
        db.busy -= 1;
        assertOrThrow(db.busy >= 0, "invalid db.busy " + db.busy);
    }

    function __dbBusyInc(db) {
// this function will increment db.busy
        db.busy += 1;
    }

    function __dbGet(db) {
// this function will get private-object mapped to <db>
        db = dbOpenMap.get(db);
        assertOrThrow(db, "invalid or closed db");
        assertOrThrow(db.busy >= 0, "invalid db.busy " + db.busy);
        return db;
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

    function cCall(fnc, argList) {
// this function will serialize <argList> to a c <baton>,
// suitable for passing into napi
        let baton = new BigInt64Array(512);
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
            case "string":
                // append null-terminator to string
                arg = Buffer.from(arg + "\u0000");
                break;
            }
            if (Buffer.isBuffer(arg)) {
                baton[ii] = BigInt(arg.byteLength);
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
        // call napi with fnc and argList
        return addon[fnc](argList);
    }

    async function dbClose({
        db
    }) {
// this function will close sqlite-database-connection <db>
        let __db = __dbGet(db);
        // prevent segfault - do not close db if actions are pending
        assertOrThrow(
            __db.busy === 0,
            "db cannot close with " + __db.busy + " actions pending"
        );
        dbOpenMap.delete(db);
        await cCall("__sqlite3_close_v2", [
            __db.ptr
        ]);
    }

    async function dbExec({
        db,
        sql
    }) {
// this function will exec <sql> in <db> and return result
        let result;
        db = __dbGet(db);
        __dbBusyInc(db);
        try {
            result = await cCall("_dbExec", [
                db.ptr, sql
            ]);
        } finally {
            __dbBusyDec(db);
        }
        return Buffer.from(result[1], 0, result[1].byteLength - 1);
    }

    async function dbOpen({
        filename,
        flags = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
    }) {
// this function will open and return sqlite-database-connection <db>
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
        let db = {};
        let ptr = noop(await cCall("__sqlite3_open_v2", [
            filename, undefined, flags, undefined
        ]))[0][0];
        dbOpenMap.set(db, {
            busy: 0,
            ptr
        });
        return db;
    }

    async function dbTableInsert({
        db,
        json,
        tableName
    }) {
// this function will insert <rowList> into <db>.<tableName>
        let buf = Buffer.allocUnsafe(4096);
        let offset = 0;
        function bufAppend(type, val) {
            let alloced = buf.byteLength;
            let nn = offset + type.length + (
                val === undefined
                ? 0
                : typeof val === "number"
                ? 8
                : Buffer.byteLength(val)
            );
            let tmp;
            // exponentially grow buf as needed
            while (alloced < nn) {
                alloced *= 2;
                assertOrThrow(alloced <= SQLITE_MAX_LENGTH, "");
            }
            if (alloced > buf.byteLength) {
                tmp = Buffer.allocUnsafe(alloced);
                buf.copy(tmp);
                buf = tmp;
            }
            offset += buf.write(type, offset);
            if (typeof val === "number") {
                offset = buf.writeDoubleLE(val, offset);
            }
            if (typeof val === "string") {
                offset += buf.write(val, offset);
            }
        }
        if (typeof json === "string") {
            json = JSON.parse(json);
        }
        if (json?.length === 0) {
            return;
        }
        json = jsonRowListNormalize({
            rowList: json
        });
        assertOrThrow(
            Array.isArray(json) && Array.isArray(json[0]),
            "json is not array-of-array"
        );
        // type - js
        // 1. bigint
        // 2. boolean
        // 3. null
        // 4. number
        // 5. object
        // 6. string
        // 7. symbol
        // 8. undefined
        // type - sqlite
        // 1. blob
        // 2. integer
        // 3. null
        // 4. real
        // 5. text
        json.forEach(function (row) {
            row.forEach(function (val) {
                if (!val) {
                    switch (typeof val) {
                    case "bigint":
                    case "boolean":
                    case "number":
                        // 2. integer
                        bufAppend("i\u0000", undefined);
                        break;
                    case "string":
                        // 5. text
                        bufAppend("t\u0000", undefined);
                        break;
                    // case "null":
                    // case "object":
                    // case "symbol":
                    // case "undefined":
                    default:
                        // 3. null
                        bufAppend("\u0000", undefined);
                    }
                    return;
                }
                switch (typeof val) {
                case "bigint":
                    // 5. text
                    bufAppend("t", val.toString());
                    break;
                case "boolean":
                    // 2. integer
                    bufAppend("i\u0001", undefined);
                    break;
                case "number":
                    // 4. real
                    bufAppend("r", val);
                    break;
                case "string":
                    // 5. text
                    bufAppend("t", val);
                    break;
                case "symbol":
                    // 3. null
                    bufAppend("\u0000", undefined);
                    break;
                // case "null":
                // case "object":
                // case "undefined":
                default:
                    // 5. text
                    bufAppend("t", JSON.stringify(val));
                }
            });
        });
        await cCall("_jstableInsert", [
            db.ptr, tableName, buf
        ]);
    }

    function jsonRowListNormalize({
        colList,
        colListPriority = [],
        rowList
    }) {
// this function will normalize <rowList> with given <colList>
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
        // sort colList by colListPriority
        if (!colListPriority) {
            rowList.unshift(colList);
            return rowList;
        }
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

    (function testAssertXxx() {
// this function will test assertXxx's handling-behavior
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
    }());

    (function testCcall() {
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
            cc = String(await cCall("noopAsync", [aa]))[0][0];
            assertOrThrow(bb === cc, [aa, bb, cc]);
            cc = String(cCall("noopSync", [aa]))[0][0];
            assertOrThrow(bb === cc, [aa, bb, cc]);
        });
    }());

    (async function testDbExec() {
// this function will test dbExec's handling-behavior
        let db;
        let promiseList = [];
        let tmp;
        db = await dbOpen({
            filename: ":memory:"
        });
        tmp = async function () {
            let result;
            try {
                result = JSON.stringify(JSON.parse(await dbExec({
                    db,
                    sql: (`
CREATE TABLE tt1 AS
SELECT 101 AS c101, 102 AS c102
UNION ALL
VALUES (201, 202),
       (301, NULL);
CREATE TABLE tt2 AS
SELECT 401 AS c401, 402 AS c402, 403 AS c403
UNION ALL
VALUES (501, 502.0123, 5030123456789),
       (601, '602', '603_\"\x01\x08\x09\x0a\x0b\x0c\x0d\x0e');
SELECT * FROM tt1;
SELECT * FROM tt2;
                    `)
                })));
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
                        [601, "602", "603_\"\u0001\b\t\n\u000b\f\r\u000e"]
                    ]
                ]));
            } catch (err) {
                assertOrThrow(
                    err.message.indexOf("table tt1 already exists") >= 0,
                    err
                );
                return;
            }
        };
        promiseList.push(tmp());
        promiseList.push(tmp());
        promiseList.push(tmp());
        promiseList.push(tmp());
        try {
            await dbClose({
                db
            });
        } catch (err) {
            assertOrThrow((
                /db cannot close with \d+? actions pending/
            ).test(err.message), err);
        }
        //!! await dbTableInsert({
            //!! db,
            //!! json: [
                //!! [
                    //!! "aa", "bb"
                //!! ]
            //!! ]
        //!! });
        await Promise.all(promiseList);
        dbClose({
            db
        });
    }());

    module.exports = Object.assign(addon, {
        SQLITE_MAX_LENGTH,
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
        dbClose,
        dbExec,
        dbOpen,
        dbTableInsert
    });

    // coverage-hack
    noop(assertJsonEqual, dbTableInsert);
}());
