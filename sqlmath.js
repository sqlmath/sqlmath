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
    let JSBATON_INFO;
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

    function assertOrThrow(passed, err) {
// this function will throw <err> if passed is falsy
        if (!passed) {
            throw (
                typeof err === "string"
                ? new Error(err)
                : err
            );
        }
    }

    function dbCall(db, fnc, argList) {
// this function will serialize <argList> to <db.argList>,
// suitable for passing into napi
        assertOrThrow(dbMap.has(db), "invalid db handle");
        // zero arg
        [
            JSBATON_INFO.buf8, JSBATON_INFO.num8
        ].forEach(function ([
            aa, bb
        ]) {
            bb += aa;
            while (aa < bb) {
                db.baton[aa] = 0;
                aa += 1;
            }
        });
        db.argList = [
            db.baton
        ];
        argList.slice(0, 8).forEach(function (arg, ii) {
            // handle buffer
            if (Buffer.isBuffer(arg)) {
                db.argList.push(arg);
                return;
            }
            if (typeof arg === "string") {
                db.argList.push(Buffer.from(arg + "\u0000"));
                return;
            }
            arg = Number(arg ?? 0);
            db.baton.writeDoubleLE(arg, JSBATON_INFO.num8[0] + 8 * ii);
        });
        return addon[fnc](...db.argList);
    }

    function dbClose({
        db
    }) {
// this function will return sqlite-database-connection <db>
        dbCall(db, "_sqlite3_close_v2", []);
        dbMap.delete(db);
    }

    function dbExec({
        db,
        sql
    }) {
// this function will exec <sql> in <db> and return result
        return Buffer.from(dbCall(db, "_jssqlExec", [
            sql
        ]));
    }

    function dbOpen({
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
        let db = {
            argList: [],
            baton: Buffer.alloc(JSBATON_INFO.sizeof[0])
        };
        dbMap.set(db, true);
        dbCall(db, "_sqlite3_open_v2", [
            filename, undefined, flags, undefined
        ]);
        return db;
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

    JSBATON_INFO = JSON.parse(addon.jsbatonInfo());
    debugInline(JSBATON_INFO);
    assertJsonEqual(JSBATON_INFO.sizeof[0], JSBATON_INFO.sizeof[1]);

    (function testJsbaton() {
// this function will test passing argList between nodejs <-> c
        let db = dbOpen({
            filename: ":memory:"
        });
        let result;
        let sql;
        [
            [-0, "0"],
            [-Infinity, "-Infinity"],
            [0, "0"],
            [1 / 0, "Infinity"],
            [Infinity, "Infinity"],
            [false, "0"],
            [null, "0"],
            [true, "1"],
            [undefined, "0"],
            [{}, "NaN"]
        ].forEach(function ([
            aa, bb
        ]) {
            let cc;
            dbCall(db, "napiNoop", [aa]);
            cc = String(db.baton.readDoubleLE(JSBATON_INFO.num8[0]));
            assertOrThrow(bb === cc, [aa, bb, cc]);
        });
        sql = (`
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
        debugInline(JSON.parse(dbExec({
            db,
            sql
        })));
        try {
            debugInline(JSON.parse(dbExec({
                db,
                sql
            })));
        } catch (err) {
            console.error(err);
        }
        try {
            debugInline(JSON.parse(dbExec({
                db,
                sql
            })));
        } catch (err) {
            console.error(err);
        }
        dbClose({
            db
        });
    }());

    module.exports = addon;

    // coverage-hack
    noop();
}());
