/*jslint beta, name, node*/
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
    let addon = require(
        "./_binary_sqlmath_napi"
        + "_" + process.versions.napi
        + "_" + process.platform
        + "_" + process.arch
        + ".node"
    );
    let dbMap = new WeakMap();

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
// this function will return sqlite-database-connection <db>
        await cCall("_sqlite3_close_v2", [db.ptr]);
        dbMap.delete(db);
    }

    async function dbExec({
        db,
        sql
    }) {
// this function will exec <sql> in <db> and return result
        let result = await cCall("_jssqlExec", [db.ptr, sql]);
        return Buffer.from(result[1], 0, result[1].byteLength - 1);
    }

    async function dbImport({
        db,
        json
    }) {
// this function will import <json> into db
        let buf = Buffer.allocUnsafe(4096);
        let ii = 0;
        let jj;
        let kk;
        let offset = 0;
        let row;
        let val;
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
        if (!json || json.length === 0) {
            return;
        }
        json = jsonRowListNormalize(json);
        assertOrThrow((
            Array.isArray(json)
            && (json.length === 0 || Array.isArray(json[0]))
        ), "json is not array or array");
        // type - json
        // 1. array
        // 2. boolean
        // 3. null
        // 4. number
        // 5. object
        // 6. string
        // type - sqlite
        // 1. blob
        // 2. integer
        // 3. null
        // 4. real
        // 5. text
        ii = 0;
        kk = 0;
        while (ii < json.length) {
            row = json[ii];
            jj = 0;
            while (jj < row.length) {
                val = row[jj];
                if (!val) {
                    switch (typeof val) {
                    case "boolean":
                    case "number":
                        // 2. integer
                        kk = bufAppend(buf, kk, "i\u0000", undefined);
                        break;
                    case "string":
                        // 5. text
                        kk = bufAppend(buf, kk, "t\u0000", undefined);
                        break;
                    // case "array":
                    // case "null":
                    // case "object":
                    default:
                        // 3. null
                        kk = bufAppend(buf, kk, "\u0000", undefined);
                    }
                } else {
                    switch (typeof val) {
                    case "boolean":
                        // 2. integer
                        kk = bufAppend(buf, kk, "i\u0001", undefined);
                        break;
                    case "number":
                        // 4. real
                        kk = bufAppend(buf, kk, "r", val);
                        break;
                    case "string":
                        // 5. text
                        kk = bufAppend(buf, kk, "t", val);
                        break;
                    // case "array":
                    // case "null":
                    // case "object":
                    default:
                        // 5. text
                        kk = bufAppend(buf, kk, "t", JSON.stringify(val));
                    }
                }
                jj += 1;
            }
            ii += 1;
        }
        await cCall("_jssqlImport", [
            db.ptr, buf
        ]);
    }

    async function dbOpen({
        filename,
        flags = 6
    }) {
// this function will return sqlite-database-connection <db>
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
        let db = {};
        let result = await cCall("_sqlite3_open_v2", [
            filename, undefined, flags, undefined
        ]);
        db.ptr = result[0][0];
        dbMap.set(db, true);
        return db;
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
            return {
                colList,
                rowList
            };
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

    (async function testJsbaton() {
// this function will test passing argList between nodejs <-> c
        let db;
        let result;
        result = await cCall("jspromiseNoop", [1234]);
        //!! debugInline(result, "jspromiseNoop");
        db = await dbOpen({
            filename: ":memory:"
        });
        //!! debugInline(db, "db");
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
        ].forEach(function ([
            aa, bb
        ]) {
            let cc;
            result = cCall("napiNoop", [aa]);
            cc = String(result[0][0]);
            assertOrThrow(bb === cc, [aa, bb, cc]);
        });
        await Promise.all(Array.from(new Array(4)).map(async function () {
            let sql = (`
CREATE TABLE tt1 AS
SELECT 101 AS c101, 102 AS c102
UNION ALL
VALUES (201, 202),
       (301, NULL);
CREATE TABLE tt2 AS
SELECT 401 AS c401, 402 AS c402, 403 AS c403
UNION ALL
VALUES (501, 502.0123, 5030123456789),
       (601, '602', '603' || '\"\x01\x08\x09\x0a\x0b\x0c\x0d\x0e');
       --(701, b64decode('0123456789'), b64decode('8J+YgQ'));
SELECT * FROM tt1;
SELECT * FROM tt2;
select noop(1234);
            `);
            try {
                result = await dbExec({
                    db,
                    sql
                });
                result = JSON.parse(Buffer.from(result));
                //!! debugInline(result, "jssqlExec");
            } catch (err) {
                console.error(err);
            }
        }));
        await dbImport({
            db,
            json: []
        });
        await dbClose({
            db
        });
    }());

    module.exports = addon;
    // coverage-hack
    noop(assertJsonEqual);
}());
