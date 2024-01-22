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

/*
 * example usage:
npm_config_mode_test_save2=1 npm test
 */

/*jslint beta, node*/
import jslint from "./jslint.mjs";
import {
    assertErrorThrownAsync,
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
    sqlmathWebworkerInit,
    version
} from "./sqlmath.mjs";
let {
    jstestDescribe,
    jstestIt
} = jslint;
let {
    npm_config_mode_test_save
} = typeof process === "object" && process?.env;
noop(debugInline);

jstestDescribe((
    "test_apidoc"
), function test_apidoc() {
    jstestIt((
        "test apidoc handling-behavior"
    ), function () {
        jslint.jslint_apidoc({
            example_list: [
                "README.md",
                "test.mjs",
                "sqlmath.mjs"
            ],
            github_repo: "https://github.com/sqlmath/sqlmath",
            module_list: [
                {
                    pathname: "./sqlmath.mjs"
                }
            ],
            package_name: "sqlmath",
            pathname: ".artifact/apidoc.html",
            version
        });
    });
});

jstestDescribe((
    "test_assertXxx"
), function test_assertXxx() {
    jstestIt((
        "test assertXxx handling-behavior"
    ), function () {
        assertErrorThrownAsync(function () {
            assertNumericalEqual(0, 0);
        }, "value cannot be 0 or falsy");
        assertErrorThrownAsync(function () {
            assertNumericalEqual(1, 2);
        }, "1 != 2");
        assertErrorThrownAsync(function () {
            assertNumericalEqual(1, 2, "aa");
        }, "aa");
        assertNumericalEqual(1, 1);
    });
});

jstestDescribe((
    "test_childProcessSpawn2"
), function test_childProcessSpawn2() {
    jstestIt((
        "test childProcessSpawn2 handling-behavior"
    ), async function () {
        await Promise.all([
            childProcessSpawn2(
                "aa",
                [],
                {modeCapture: "utf8", modeDebug: true, stdio: []}
            )
        ]);
    });
});

jstestDescribe((
    "test_ciBuildExtXxx"
), function test_ciBuildExtXxx() {
    jstestIt((
        "test ciBuildExt handling-behavior"
    ), async function () {
        await Promise.all([
            ciBuildExt({modeSkip: true, process: {}}),
            ciBuildExt({process: {arch: "arm", env: {}, platform: "win32"}}),
            ciBuildExt({process: {arch: "arm64", env: {}, platform: "win32"}}),
            ciBuildExt({process: {arch: "ia32", env: {}, platform: "win32"}}),
            ciBuildExt({process: {env: {}, platform: "darwin"}}),
            ciBuildExt({process: {env: {}, platform: "win32"}}),
            ciBuildExt({process: {}})
        ]);
    });
});

jstestDescribe((
    "test_dbBind"
), function test_dbBind() {
    jstestIt((
        "test db-bind handling-behavior"
    ), async function test_dbBind() {
        let db;
        async function test_dbBind_exec(ii, valIn, valExpect) {
            await Promise.all([
                [
                    [
                        valExpect, valExpect, 0
                    ],
                    (
                        "SELECT 0;"
                        + " SELECT ? AS c1, ? AS c2, ? AS c3, ? AS c4"
                        + " UNION ALL SELECT ?1, ?2, ?3, ?4"
                        + " UNION ALL SELECT ?1, ?2, ?3, ?4"
                    )
                ],
                [
                    {
                        k1: valExpect,
                        k2: valExpect,
                        k3: 0
                    },
                    (
                        "SELECT 0;"
                        + " SELECT $k1 AS c1, $k2 AS c2, $k3 AS c3, $k4 AS c4"
                        + " UNION ALL SELECT :k1, :k2, :k3, :k4"
                        + " UNION ALL SELECT @k1, @k2, @k3, @k4"
                    )
                ]
            ].map(async function ([
                bindList, sql
            ]) {
                let bufActual = await dbExecAsync({
                    bindList,
                    db,
                    responseType: "list",
                    sql
                });
                let bufExpect = [
                    [
                        ["0"],
                        [0]
                    ],
                    [
                        ["c1", "c2", "c3", "c4"],
                        [valExpect, valExpect, 0, undefined],
                        [valExpect, valExpect, 0, undefined],
                        [valExpect, valExpect, 0, undefined]
                    ]
                ];
                assertJsonEqual(bufActual, bufExpect, {
                    bufActual,
                    ii,
                    valExpect,
                    valIn
                });
            }));
        }
        async function test_dbBind_lastblob(ii, valIn, valExpect) {
            let bufActual;
            let bufExpect;
            if (valExpect === Error) {
                assertErrorThrownAsync(
                    dbExecAndReturnLastBlobAsync.bind(undefined, {
                        bindList: [valIn],
                        db,
                        sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
                    }),
                    "inclusive-range|not JSON serializable"
                );
                return;
            }
            bufActual = new TextDecoder().decode(
                await dbExecAndReturnLastBlobAsync({
                    bindList: [valIn],
                    db,
                    sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
                })
            );
            bufExpect = String(valExpect);
            switch (typeof(valIn)) {
            case "bigint":
                valIn = Number(valIn);
                break;
            case "function":
            case "symbol":
            case "undefined":
                bufExpect = "";
                break;
            case "number":
                switch (valIn) {
                case -2:
                    bufExpect = "-2.0";
                    break;
                case -Infinity:
                    bufExpect = "-Inf";
                    break;
                case 2:
                    bufExpect = "2.0";
                    break;
                case Infinity:
                    bufExpect = "Inf";
                    break;
                default:
                    if (Number.isNaN(valIn)) {
                        bufExpect = "";
                    }
                }
                break;
            case "object":
                if (valIn === null) {
                    bufExpect = "";
                    break;
                }
                if (
                    valIn?.constructor === ArrayBuffer
                    || ArrayBuffer.isView(valIn)
                ) {
                    bufExpect = new TextDecoder().decode(valIn);
                    break;
                }
                break;
            }
            assertJsonEqual(bufActual, bufExpect, {
                bufActual,
                bufExpect,
                ii,
                valExpect,
                valIn
            });
        }
        async function test_dbBind_responseType(ii, valIn, valExpect) {
            await Promise.all([
                "arraybuffer",
                "list",
                undefined
            ].map(async function (responseType) {
                let bufActual;
                if (valExpect === Error) {
                    assertErrorThrownAsync(
                        dbExecAsync.bind(undefined, {
                            bindList: [valIn],
                            db,
                            responseType,
                            sql: "SELECT ? AS val"
                        }),
                        "inclusive-range|not JSON serializable"
                    );
                    return;
                }
                bufActual = await dbExecAsync({
                    bindList: [valIn],
                    db,
                    responseType,
                    sql: "SELECT ? AS val"
                });
                switch (responseType) {
                case "arraybuffer":
                    bufActual = JSON.parse(
                        new TextDecoder().decode(bufActual)
                    )[0][1][0];
                    break;
                case "list":
                    bufActual = bufActual[0][1][0];
                    break;
                default:
                    bufActual = bufActual[0][0].val;
                }
                assertJsonEqual(bufActual, valExpect, {
                    bufActual,
                    ii,
                    responseType,
                    valExpect,
                    valIn
                });
            }));
        }
        db = await dbOpenAsync({filename: ":memory:"});
        await Promise.all([
            // 1. bigint
            [-0n, -0],
            [-0n, 0],
            [-0x8000000000000000n, "-9223372036854775808"],
            [-0x8000000000000001n, Error],
            [-1n, -1],
            [-2n, -2],
            [0n, -0],
            [0n, 0],
            [0x7fffffffffffffffn, "9223372036854775807"],
            [0x8000000000000000n, Error],
            [1n, 1],
            [2n, 2],
            // 2. boolean
            [false, 0],
            [true, 1],
            // 3. function
            [noop, null],
            // 4. number
            [-0, -0],
            [-0, 0],
            [-0.5, -0.5],
            [-1 / 0, null],
            [-1e-999, 0],
            [-1e999, null],
            [-2, -2],
            [-Infinity, null],
            [-NaN, null],
            [0, -0],
            [0, 0],
            [0.5, 0.5],
            [1 / 0, null],
            [1e-999, 0],
            [1e999, null],
            [2, 2],
            [Infinity, null],
            [NaN, null],
            // 5. object
            [[], "[]"],
            [new ArrayBuffer(0), null],
            [new ArrayBuffer(1), null],
            [new Date(0), "1970-01-01T00:00:00.000Z"],
            [new RegExp(), "{}"],
            [new TextEncoder().encode(""), null],
            [new TextEncoder().encode("\u0000"), null],
            [new TextEncoder().encode("\u0000\u{1f600}\u0000"), null],
            [new Uint8Array(0), null],
            [new Uint8Array(1), null],
            [null, null],
            [{}, "{}"],
            // 6. string
            ["", ""],
            ["0", "0"],
            ["1", "1"],
            ["2", "2"],
            ["\u0000", "\u0000"],
            ["\u0000\u{1f600}\u0000", "\u0000\u{1f600}\u0000"],
            ["a".repeat(9999), "a".repeat(9999)],
            // 7. symbol
            [Symbol(), null],
            // 8. undefined
            [undefined, null]
        ].map(async function ([valIn, valExpect], ii) {
            await Promise.all([
                test_dbBind_exec(ii, valIn, valExpect),
                test_dbBind_lastblob(ii, valIn, valExpect),
                test_dbBind_responseType(ii, valIn, valExpect)
            ]);
        }));
    });
});

jstestDescribe((
    "test_dbNoopAsync"
), function test_dbNoopAsync() {
    jstestIt((
        "test dbNoopAsync handling-behavior"
    ), async function () {
        // test datatype handling-behavior
        await Promise.all([
            // 1. bigint
            [-0n, -0],
            [-0n, 0],
            [-0x8000000000000000n, -0x8000000000000000n],
            [-0x8000000000000001n, Error],
            [-1n, -1],
            [-2n, -2],
            [0n, -0],
            [0n, 0],
            [0x7fffffffffffffffn, 0x7fffffffffffffffn],
            [0x8000000000000000n, Error],
            [1n, 1],
            [2n, 2],
            // 2. boolean
            [false, 0],
            [true, 1],
            // 3. function
            [noop, Error],
            // 4. number
            [-0, -0],
            [-0, 0],
            [-0.5, Error],
            [-1 / 0, Error],
            [-1e-999, 0],
            [-1e999, Error],
            [-2, -2],
            [-Infinity, Error],
            [-NaN, Error],
            [0, -0],
            [0, 0],
            [0.5, Error],
            [1 / 0, Error],
            [1e-999, 0],
            [1e999, Error],
            [2, 2],
            [Infinity, Error],
            [NaN, Error],
            // 5. object
            [[], Error],
            [new ArrayBuffer(0), 0],
            [new ArrayBuffer(1), 0],
            [new Date(0), Error],
            [new RegExp(), Error],
            [new TextEncoder().encode(""), Error],
            [new TextEncoder().encode("\u0000"), Error],
            [new TextEncoder().encode("\u0000\u{1f600}\u0000"), Error],
            [new Uint8Array(0), Error],
            [new Uint8Array(1), Error],
            [null, 0],
            [{}, Error],
            // 6. string
            ["", ""],
            ["0", "0"],
            ["1", "1"],
            ["2", "2"],
            ["\u0000", ""],
            ["\u0000\u{1f600}\u0000", "\u0000\u{1f600}"],
            ["a".repeat(9999), "a".repeat(9999)],
            // 7. symbol
            [Symbol(), Error],
            // 8. undefined
            [undefined, 0]
        ].map(async function ([valIn, valExpect], ii) {
            let baton;
            let valActual;
            if (valExpect === Error) {
                assertErrorThrownAsync(function () {
                    return dbNoopAsync(undefined, valIn, undefined);
                }, "invalid arg|integer");
                return;
            }
            baton = await dbNoopAsync(undefined, valIn, undefined);
            baton = baton[0];
            valActual = (
                typeof valIn === "string"
                ? jsbatonGetString(baton, 1)
                : String(jsbatonGetInt64(baton, 1))
            );
            valExpect = String(valExpect);
            if (typeof valIn === "bigint") {
                valIn = String(valIn);
            }
            assertJsonEqual(valActual, valExpect, {
                ii,
                valActual,
                valExpect,
                valIn
            });
        }));
    });
});

jstestDescribe((
    "test_dbXxxAsync"
), function test_dbXxxAsync() {
    jstestIt((
        "test dbCloseAsync handling-behavior"
    ), async function test_dbCloseAsync() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbCloseAsync({});
        }, "cannot close db");
        // test close handling-behavior
        dbCloseAsync(db);
    });
    jstestIt((
        "test dbExecAsync handling-behavior"
    ), async function test_dbExecAsync() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecAsync({
                db,
                sql: undefined
            });
        }, "syntax error");
        // test race-condition handling-behavior
        Array.from(new Array(4)).forEach(async function () {
            let result;
            try {
                result = await dbExecAsync({
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
    (CAST(?1 AS TEXT), CAST(?2 AS TEXT), CAST(?3 AS TEXT)),
    (
        CAST(uncompress(compress(?1)) AS TEXT),
        CAST(uncompress(compress(?2)) AS TEXT),
        CAST(uncompress(compress(?3)) AS TEXT)
    );
SELECT * FROM testDbExecAsync1;
SELECT * FROM testDbExecAsync2;
                    `)
                });
                assertJsonEqual(
                    result,
                    [
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
                            [null, null, null],
                            ["foob", "fooba", "foobar"],
                            ["foob", "fooba", "foobar"]
                        ]
                    ]
                );
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
            return dbCloseAsync(db);
        }, "cannot close db");
    });
    jstestIt((
        "test dbFileXxx handling-behavior"
    ), async function test_dbFileXxx() {
        let data;
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        dbFileLoadAsync({
            modeNoop: true
        });
        assertErrorThrownAsync(function () {
            return dbFileLoadAsync({
                db
            });
        }, "invalid filename undefined");
        assertErrorThrownAsync(function () {
            return dbFileSaveAsync({
                db
            });
        }, "invalid filename undefined");
        await dbExecAsync({
            db,
            sql: "CREATE TABLE t01 AS SELECT 1 AS c01"
        });
        await dbFileSaveAsync({
            db,
            filename: ".testDbFileXxx.sqlite"
        });
        db = await dbOpenAsync({
            filename: ":memory:"
        });
        await dbFileLoadAsync({
            db,
            filename: ".testDbFileXxx.sqlite"
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
    });
    jstestIt((
        "test dbOpenAsync handling-behavior"
    ), async function test_dbOpenAsync() {
        // test auto-finalization handling-behavior
        await new Promise(function (resolve) {
            dbOpenAsync({
                afterFinalization: resolve,
                filename: ":memory:"
            });
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbOpenAsync({});
        }, "invalid filename");
    });
});

jstestDescribe((
    "test_fsXxx"
), function test_fsXxx() {
    jstestIt((
        "test fsXxx handling-behavior"
    ), async function () {
        await Promise.all([
            fsCopyFileUnlessTest("", ""),
            fsExistsUnlessTest(""),
            fsReadFileUnlessTest("", ""),
            fsWriteFileUnlessTest("", ""),
            //
            fsCopyFileUnlessTest(
                "package.json",
                ".tmp/test_fsCopyFileUnlessTest_force",
                "force"
            ),
            fsExistsUnlessTest("", "force"),
            fsExistsUnlessTest("package.json", "force"),
            fsReadFileUnlessTest("package.json", "force"),
            fsWriteFileUnlessTest(
                ".tmp/test_fsWriteFileUnlessTest_force",
                "",
                "force"
            )
        ]);
    });
});

jstestDescribe((
    "test_misc"
), function test_misc() {
    jstestIt((
        "test misc handling-behavior"
    ), async function () {
        // test assertErrorThrownAsync error handling-behavior
        await assertErrorThrownAsync(function () {
            return assertErrorThrownAsync(noop);
        }, "No error thrown");
        // test assertJsonEqual error handling-behavior
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2);
        }, "!==");
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2, "undefined");
        }, "undefined");
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2, {});
        }, "");
        // test assertOrThrow error handling-behavior
        await assertErrorThrownAsync(function () {
            assertOrThrow(undefined, "undefined");
        }, "undefined");
        await assertErrorThrownAsync(function () {
            assertOrThrow(undefined, new Error());
        }, "");
    });
});

jstestDescribe((
    "test_sqlite"
), function test_sqlite() {
    jstestIt((
        "test sqlite-error handling-behavior"
    ), async function test_sqliteError() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        assertJsonEqual(
            "not an error",
            noop(
                await dbExecAsync({
                    db,
                    sql: `SELECT throwerror(0) AS val`
                })
            )[0][0].val
        );
        await Promise.all([
            [1, "SQL logic error"],
            [2, "unknown error"],
            [3, "access permission denied"],
            [4, "query aborted"],
            [5, "database is locked"],
            [6, "database table is locked"],
            [7, "out of memory"],
            [8, "attempt to write a readonly database"],
            [9, "interrupted"],
            [10, "disk I/O error"],
            [11, "database disk image is malformed"],
            [12, "unknown operation"],
            [13, "database or disk is full"],
            [14, "unable to open database file"],
            [15, "locking protocol"],
            [16, "unknown error"],
            [17, "database schema has changed"],
            [18, "string or blob too big"],
            [19, "constraint failed"],
            [20, "datatype mismatch"],
            [21, "bad parameter or other API misuse"],
            [22, "unknown error"],
            [23, "authorization denied"],
            [24, "unknown error"],
            [25, "column index out of range"],
            [26, "file is not a database"],
            [27, "notification message"],
            [28, "warning message"],
            [100, "unknown error"],
            [101, "unknown error"]
        ].map(async function ([
            errno, errmsg
        ]) {
            await assertErrorThrownAsync(function () {
                return dbExecAsync({
                    db,
                    sql: `SELECT throwerror(${errno})`
                });
            }, errmsg);
        }));
    });
    jstestIt((
        "test sqlite-extension-doublearray_xxx handling-behavior"
    ), async function test_sqlite_extension_doublearray_xxx() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await Promise.all([
            [" [ , 1 ] ", "error"],
            [" [ , ] ", "error"],
            [" [ 1 , ] ", "error"],
            [" [ ] ", "[]"],
            [" [ null ] ", "[0.0]"],
            ["", "error"],
            ["1,2]", "error"],
            ["[,1]", "error"],
            ["[,]", "error"],
            ["[0]", "[0.0]"],
            ["[1,2", "error"],
            ["[1,2,a]", "[1.0,2.0,0.0]"],
            ["[1,]", "error"],
            ["[1,a,3]", "[1.0,0.0,3.0]"],
            ["[1]", "[1.0]"],
            ["[]", "[]"],
            ["[a,2,3]", "[0.0,2.0,3.0]"],
            [0, "error"],
            [1, "error"],
            [`[${"1".repeat(100)}]`, `[1.11111111111111e+99]`],
            [null, "error"],
            [undefined, "error"],
            [{}, "error"]
        ].map(async function ([valIn, valExpect], ii) {
            let valActual;
            try {
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            valIn
                        },
                        db,
                        sql: (`
SELECT doublearray_jsonto(doublearray_jsonfrom($valIn)) AS result;
                        `)
                    })
                )[0][0].result;
            } catch (ignore) {
                assertOrThrow(valExpect === "error", JSON.stringify({
                    ii,
                    valActual,
                    valExpect,
                    valIn
                }, undefined, 4));
                return;
            }
            assertJsonEqual(valActual, valExpect, {
                ii,
                valActual,
                valExpect,
                valIn
            });
        }));
    });
    jstestIt((
        "test sqlite-extension-fill_forward handling-behavior"
    ), async function test_sqlite_extension_fill_forward() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let result = await dbExecAsync({
            db,
            sql: (`
SELECT
    fill_forward(val) OVER (ORDER BY id ASC) AS val
    FROM (
        SELECT 10 AS id, NULL AS val
        UNION ALL SELECT 9 AS id, 9 AS val
        UNION ALL SELECT 8 AS id, 8 AS val
        UNION ALL SELECT 7 AS id, NULL AS val
        UNION ALL SELECT 6 AS id, 6 AS val
        UNION ALL SELECT 5 AS id, 5 AS val
        UNION ALL SELECT 4 AS id, NULL AS val
        UNION ALL SELECT 3 AS id, 3 AS val
        UNION ALL SELECT 2 AS id, NULL AS val
        UNION ALL SELECT 1 AS id, NULL AS val
    );
            `)
        });
        result = result[0].map(function ({val}) {
            return val;
        });
        assertJsonEqual(result, [null, null, 3, 3, 5, 6, 6, 8, 9, 9]);
    });
    jstestIt((
        "test sqlite-extension-math handling-behavior"
    ), async function test_sqlite_extension_math() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test sqlmath-defined-func handling-behavior
        Object.entries({
            "''": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": ""
            },
            "'-0.5'": {
                "castrealornull": -0.5,
                "castrealorzero": -0.5,
                "casttextorempty": "-0.5",
                "copyblob": "-0.5"
            },
            "'-1'": {
                "castrealornull": -1,
                "castrealorzero": -1,
                "casttextorempty": "-1",
                "copyblob": "-1",
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1,
                "sqrtwithsign": -1
            },
            "'0'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": "0",
                "cot": null,
                "coth": null,
                "sign": 0,
                "sqrtwithsign": 0
            },
            "'0.5'": {
                "castrealornull": 0.5,
                "castrealorzero": 0.5,
                "casttextorempty": "0.5",
                "copyblob": "0.5"
            },
            "'1'": {
                "castrealornull": 1,
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": "1",
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1,
                "sqrtwithsign": 1
            },
            "'aa'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "aa",
                "copyblob": "aa"
            },
            "'hello'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "hello",
                "copyblob": "hello"
            },
            "-0.5": {
                "castrealornull": -0.5,
                "castrealorzero": -0.5,
                "casttextorempty": "-0.5",
                "copyblob": -0.5
            },
            "-0x7fffffffffffffff": {
                "cot": -0.0118008981305845,
                "coth": -1,
                "sign": -1,
                "sqrtwithsign": -3037000499.97605
            },
            "-1": {
                "castrealornull": -1,
                "castrealorzero": -1,
                "casttextorempty": "-1",
                "copyblob": -1,
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1,
                "sqrtwithsign": -1
            },
            "-1e999": {
                "cot": null,
                "coth": -1,
                "sign": -1,
                "sqrtwithsign": null
            },
            "0": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": 0,
                "cot": null,
                "coth": null,
                "sign": 0,
                "sqrtwithsign": 0
            },
            "0.5": {
                "castrealornull": 0.5,
                "castrealorzero": 0.5,
                "casttextorempty": "0.5",
                "copyblob": 0.5
            },
            "0.5, 0.5": {
                "roundorzero": 1
            },
            "0.5, 1": {
                "roundorzero": 0.5
            },
            "0.5, NULL": {
                "roundorzero": 1
            },
            "0x7fffffffffffffff": {
                "cot": 0.0118008981305845,
                "coth": 1,
                "sign": 1,
                "sqrtwithsign": 3037000499.97605
            },
            "0x8000000000000000": {
                "cot": -0.0118008981305845,
                "coth": -1,
                "sign": -1,
                "sqrtwithsign": -3037000499.97605
            },
            "0xffffffffffffffff": {
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1,
                "sqrtwithsign": -1
            },
            "1": {
                "castrealornull": 1,
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": 1,
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1,
                "sqrtwithsign": 1
            },
            "1e999": {
                "castrealornull": null,
                "castrealorzero": 0,
                "cot": null,
                "coth": 1,
                "sign": 1,
                "sqrtwithsign": null
            },
            "null": {
                "castrealornull": null,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null,
                "cot": null,
                "coth": null,
                "sign": null,
                "sqrtwithsign": null
            },
            "null, 0": {
                "roundorzero": 0
            },
            "null, 0.5": {
                "roundorzero": 0
            },
            "null, null": {
                "roundorzero": 0
            },
            "zeroblob(0)": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null
            },
            "zeroblob(1)": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null
            }
        }).forEach(function ([
            arg, funcDict
        ]) {
            Object.entries(funcDict).forEach(async function ([
                func, valExpect
            ]) {
                let sql = `SELECT ${func}(${arg}) AS val`;
                let valActual = noop(
                    await dbExecAsync({
                        db,
                        sql
                    })
                )[0][0].val;
                assertJsonEqual(valActual, valExpect, {
                    sql,
                    valActual,
                    valExpect
                });
            });
        });
    });
    jstestIt((
        "test sqlite-extension-quantile handling-behavior"
    ), async function test_sqlite_extension_quantile() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await (async function () {
            let valActual = noop(
                await dbExecAsync({
                    db,
                    sql: (`
-- test null-case handling-behavior
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT
        1 AS id,
        median(val) AS mdn,
        quantile(val, 0.5) AS qnt,
        stdev(val) AS std
    FROM __tmp1;
                    `)
                })
            )[0];
            assertJsonEqual(
                valActual,
                [{id: 1, mdn: null, qnt: null, std: null}]
            );
        }());
        await Promise.all([
            [
                [[], -99, undefined],
                [[], 0, undefined],
                [[], 0.5, undefined],
                [[], 1, undefined],
                [[], 99, undefined],
                [[undefined, undefined, 1, 1, 2, 3, 4], 0.5, 2],
                [[undefined, undefined, 1, 2, 3, 4], 0.5, 2.5],
                [[undefined], 0.5, undefined]
            ],
            [
                [[], -99, 1],
                [[], 0, 1],
                [[], 0.125, 1.875],
                [[], 0.250, 2.75],
                [[], 0.375, 3.625],
                [[], 0.500, 4.5],
                [[], 0.625, 5.375],
                [[], 0.750, 6.25],
                [[], 0.875, 7.125],
                [[], 1, 8],
                [[], 99, 8],
                [[0.1], 0, 0.1],
                [[1.1], 0.125, 1.1],
                [[2.1], 0.250, 2.1],
                [[3.1], 0.375, 3.1],
                [[4.1], 0.500, 4.1],
                [[4.1], 0.625, 5],
                [[5.1], 0.750, 6],
                [[6.1], 0.875, 7],
                [[7.1], 1, 8],
                [[0], 0, 0],
                [[1], 0.125, 1],
                [[2], 0.250, 2],
                [[3], 0.375, 3],
                [[4], 0.500, 4],
                [[5], 0.625, 5],
                [[6], 0.750, 6],
                [[7], 0.875, 7],
                [[8], 1, 8],
                [[], 0, 1]
            ].map(function ([
                data, kk, valExpect
            ]) {
                return [
                    data.concat([
                        undefined, -Infinity, Infinity, NaN,
                        "8", 7, 6, "5", "4", 3, 2, "1",
                        undefined
                    ]),
                    kk,
                    valExpect
                ];
            })
        ].flat().map(async function ([
            data, kk, valExpect
        ]) {
            let avg = 0;
            let data2;
            let valExpectMdn;
            let valExpectStd = 0;
            data2 = data.map(function (elem) {
                return Number(elem);
            }).filter(function (elem) {
                return Number.isFinite(elem);
            }).sort();
            valExpectMdn = (
                data2.length % 2 === 0
                ? 0.5 * (
                    data2[0.5 * data2.length - 1] + data2[0.5 * data2.length]
                )
                : data2[0.5 * (data2.length - 1)]
            );
            data2.forEach(function (elem) {
                avg += elem;
            });
            avg *= 1 / data2.length;
            data2.forEach(function (elem) {
                valExpectStd += (elem - avg) ** 2;
            });
            valExpectStd = (
                data2.length <= 0
                ? null
                // : data2.length === 1
                // ? 0
                : Number(Math.sqrt(
                    valExpectStd / (data2.length - 1)
                ).toFixed(8))
            );
            await Promise.all([
                data,
                Array.from(data).reverse()
            ].map(async function (data) {
                let valActual;
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            tmp1: JSON.stringify(data)
                        },
                        db,
                        sql: (`
SELECT
        median(value) AS mdn,
        quantile(value, ${kk}) AS qnt,
        ROUND(stdev(value), 8) AS std
    FROM JSON_EACH($tmp1);
-- test null-case handling-behavior
SELECT quantile(value, ${kk}) AS qnt FROM JSON_EACH($tmp1) WHERE 0;
                        `)
                    })
                )[0][0];
                assertJsonEqual(
                    valActual,
                    {
                        mdn: valExpectMdn,
                        qnt: valExpect ?? null,
                        std: valExpectStd
                    },
                    {
                        data,
                        kk,
                        valActual,
                        valExpect,
                        valExpectMdn,
                        valExpectStd
                    }
                );
            }));
        }));
    });
    jstestIt((
        "test sqlite-extension-win_emax handling-behavior"
    ), async function test_sqlite_extension_win_emax() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valIn;
        async function test_win_emax_aggregate({
            aa,
            bb,
            valExpect,
            valExpect2
        }) {
            let alpha = 2 * 1.0 / (4 + 1);
            let sqlBetween = "";
            let valActual;
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_ema1-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        win_ema1(${alpha}, value->>1) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}) {
                return Number(val.toFixed(4));
            });
            assertJsonEqual(valActual, valExpect);
            // test win_ema2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        doublearray_jsonto(win_ema2(
            ${alpha},
            value->>1,
            value->>1,
            value->>1,
            value->>1,
            value->>1,
            value->>1,
            value->>1,
            value->>1,
            value->>1,
            IIF(id = 1, -1, value->>1)
        ) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        )) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}, ii, list) {
                val = JSON.parse(val).map(function (elem, jj) {
                    elem = Number(elem.toFixed(4));
                    if (ii + (bb || 0) + 1 >= list.length && jj === 9) {
                        assertJsonEqual(elem, valExpect2, valActual);
                    } else {
                        assertJsonEqual(elem, valExpect[ii], valActual);
                    }
                    return elem;
                });
                return val[0];
            });
            assertJsonEqual(valActual, valExpect);
        }
        valIn = [
            [11, NaN],
            [10, "10"],
            [9, 9],
            [8, "8"],
            [7, 7],
            [6, 6],
            [5, Infinity],
            [4, "4"],
            [3, 3],
            [2, 2],
            [1, "1"],
            [0, undefined]
        ];
        await Promise.all([
            (async function () {
                let valActual;
                // test win_ema2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_ema2(1) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_ema2(NULL, 1) FROM (SELECT 1);
                        `)
                    });
                }, "invalid argument 'alpha'");
                // test win_ema1-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_ema1(1, 2) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
                // test win_ema2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT doublearray_jsonto(win_ema2(1, 2, 3)) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_emax-aggregate-normal handling-behavior
            test_win_emax_aggregate({
                valExpect: [
                    0.0000, 0.4000, 1.0400, 1.8240,
                    2.6944, 3.2166, 4.3300, 5.3980,
                    6.4388, 7.4633, 8.4780, 9.0868
                ],
                valExpect2: 4.6868
            }),
            // test win_emax-aggregate-window handling-behavior
            test_win_emax_aggregate({
                aa: 1,
                bb: 3,
                valExpect: [
                    1.824, 2.824, 3.424, 4.584,
                    5.680, 6.608, 7.824, 8.824,
                    9.424, 9.424, 9.424, 9.424
                ],
                valExpect2: 5.024
            }),
            test_win_emax_aggregate({
                aa: 3,
                bb: 1,
                valExpect: [
                    0.400, 1.040, 1.824, 2.824,
                    3.424, 4.584, 5.680, 6.608,
                    7.824, 8.824, 9.424, 9.424
                ],
                valExpect2: 5.024
            }),
            test_win_emax_aggregate({
                aa: 4,
                bb: 0,
                valExpect: [
                    0.000, 0.400, 1.040, 1.824,
                    2.824, 3.424, 4.584, 5.680,
                    6.608, 7.824, 8.824, 9.424
                ],
                valExpect2: 5.024
            })
        ]);
    });
    jstestIt((
        "test sqlite-extension-win_quantilex handling-behavior"
    ), async function test_sqlite_extension_win_quantilex() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valIn;
        async function test_win_quantilex_aggregate({
            aa,
            bb,
            quantile,
            valExpect,
            valExpect2
        }) {
            let sqlBetween = "";
            let valActual;
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_quantile1-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        win_quantile1(${quantile}, value->>1) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}) {
                return Number(val.toFixed(4));
            });
            assertJsonEqual(valActual, valExpect);
            // test win_quantile2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        doublearray_jsonto(win_quantile2(
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, value->>1,
            ${quantile}, IIF(id = 34, -1, value->>1)
        ) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        )) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}, ii) {
                val = JSON.parse(val).map(function (elem, jj) {
                    elem = Number(elem.toFixed(4));
                    if (ii === 11 && jj === 9) {
                        assertJsonEqual(elem, valExpect2, valActual);
                    } else {
                        assertJsonEqual(elem, valExpect[ii], valActual);
                    }
                    return elem;
                });
                return val[0];
            });
            assertJsonEqual(valActual, valExpect);
        }
        valIn = [
            [1, undefined],
            [2, "1"],
            [3, "2"],
            [4, 3],
            [5, 4],
            [6, "abcd"],
            [7, 6],
            [8, NaN],
            [9, 8],
            [10, 9],
            [11, 10],
            [12, 11]
        ];
        await Promise.all([
            (async function () {
                let valActual;
                // test win_quantile2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_quantile2(1) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_quantile2(NULL, 1) FROM (SELECT 1);
                        `)
                    });
                }, "argument 'quantile'");
                // test win_quantile1-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_quantile1(1, 2) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
                // test win_quantile2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT doublearray_jsonto(win_quantile2(1, 2, 3)) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_quantilex-aggregate-normal handling-behavior
            test_win_quantilex_aggregate({
                quantile: 0,
                valExpect: [
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000
                ],
                valExpect2: -1
            }),
            test_win_quantilex_aggregate({
                quantile: 0.25,
                valExpect: [
                    0.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 1.2500, 1.5000, 1.7500
                ],
                valExpect2: 0.7500
            }),
            test_win_quantilex_aggregate({
                quantile: 0.33333333,
                valExpect: [
                    0.0000, 0.3333, 0.6667, 1.0000,
                    1.3333, 0.6667, 1.0000, 1.3333,
                    1.6667, 2.0000, 2.3333, 2.6667
                ],
                valExpect2: 1.6667
            }),
            test_win_quantilex_aggregate({
                quantile: 0.5,
                valExpect: [
                    0.0000, 0.5000, 1.0000, 1.5000,
                    2.0000, 1.5000, 2.0000, 2.5000,
                    3.0000, 3.5000, 4.0000, 5.0000
                ],
                valExpect2: 3.5000
            }),
            test_win_quantilex_aggregate({
                quantile: 0.66666667,
                valExpect: [
                    0.0000, 0.6667, 1.3333, 2.0000,
                    2.6667, 2.3333, 3.0000, 3.6667,
                    4.6667, 6.0000, 6.0000, 6.6667
                ],
                valExpect2: 6.0000
            }),
            test_win_quantilex_aggregate({
                quantile: 0.75,
                valExpect: [
                    0.0000, 0.7500, 1.5000, 2.2500,
                    3.0000, 2.7500, 3.5000, 4.5000,
                    6.0000, 6.0000, 7.0000, 8.2500
                ],
                valExpect2: 6.5000
            }),
            test_win_quantilex_aggregate({
                quantile: 1,
                valExpect: [
                    0.0000, 1.0000, 2.0000, 3.0000,
                    4.0000, 4.0000, 6.0000, 6.0000,
                    8.0000, 9.0000, 10.0000, 11.0000
                ],
                valExpect2: 10.0000
            }),
            // test win_quantilex-aggregate-window handling-behavior
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0,
                valExpect: [
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000
                ],
                valExpect2: -1
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.25,
                valExpect: [
                    0.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 0.2500, 0.5000, 0.7500,
                    1.7500, 2.7500, 3.7500, 5.5000
                ],
                valExpect2: 3
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.33333333,
                valExpect: [
                    0.0000, 0.3333, 0.6667, 1.0000,
                    1.3333, 0.6667, 1.0000, 1.3333,
                    2.3333, 3.3333, 4.6667, 6.0000
                ],
                valExpect2: 4.6667
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.5000,
                valExpect: [
                    0.0000, 0.5000, 1.0000, 1.5000,
                    2.0000, 1.5000, 2.0000, 2.5000,
                    3.5000, 5.0000, 6.0000, 7.0000
                ],
                valExpect2: 6.0000
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.66666667,
                valExpect: [
                    0.0000, 0.6667, 1.3333, 2.0000,
                    2.6667, 2.3333, 3.0000, 3.6667,
                    5.3333, 6.0000, 7.3333, 8.6667
                ],
                valExpect2: 7.3333
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.75,
                valExpect: [
                    0.0000, 0.7500, 1.5000, 2.2500,
                    3.0000, 2.7500, 3.5000, 4.5000,
                    6.0000, 6.5000, 8.2500, 9.2500
                ],
                valExpect2: 8.2500
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 1.0000,
                valExpect: [
                    0.0000, 1.0000, 2.0000, 3.0000,
                    4.0000, 4.0000, 6.0000, 6.0000,
                    8.0000, 9.0000, 10.0000, 11.0000
                ],
                valExpect2: 10.0000
            })
        ]);
    });
    jstestIt((
        "test sqlite-extension-win_sinefit2 handling-behavior"
    ), async function test_sqlite_extension_win_sinefit2() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valExpect0;
        let valIn;
        function sqlSinefitExtractLnr(wsf, ii, suffix) {
            return (`
    ROUND(sinefit_extract(${wsf}, ${ii}, 'gyy', 0), 8) AS gyy${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'laa', 0), 8) AS laa${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'lbb', 0), 8) AS lbb${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'lee', 0), 8) AS lee${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'lxy', 0), 8) AS lxy${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'lyy', 0), 8) AS lyy${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'mee', 0), 8) AS mee${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'mxe', 0), 8) AS mxe${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'mxx', 0), 8) AS mxx${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'myy', 0), 8) AS myy${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'nnn', 0), 8) AS nnn${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'rr0', 0), 8) AS rr0${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'rr1', 0), 8) AS rr1${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'xx1', 0), 8) AS xx1${suffix},
    ROUND(sinefit_extract(${wsf}, ${ii}, 'yy1', 0), 8) AS yy1${suffix}
            `);
        }
        async function test_win_sinefit2_aggregate({
            aa,
            bb,
            valExpect,
            valExpect2,
            valExpect3
        }) {
            let id2 = 25;
            let id3 = 28;
            let sqlBetween = "";
            let valActual;
            let xx2 = 2;
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_sinefit2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn
                },
                db,
                sql: (`
DROP TABLE IF EXISTS __sinefit_win;
CREATE TEMP TABLE __sinefit_win AS
    SELECT
        id,
        __wsf,
        sinefit_extract(__wsf, 0, 'xx1', 0) AS xx11,
        sinefit_extract(__wsf, 0, 'yy1', 0) AS yy11,
        sinefit_extract(__wsf, 8, 'xx1', 0) AS xx12,
        sinefit_extract(__wsf, 8, 'yy1', 0) AS yy12,
        sinefit_extract(__wsf, 9, 'xx1', 0) AS xx13,
        sinefit_extract(__wsf, 9, 'yy1', 0) AS yy13
    FROM (
        SELECT
            id,
            win_sinefit2(
                1, ${xx2},
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, IIF(id = ${id2}, -1, value->>1)
            ) OVER (
                ORDER BY NULL ASC
                ${sqlBetween}
            ) AS __wsf
        FROM JSON_EAcH($valIn)
    );
UPDATE __sinefit_win
    SET
        __wsf = sinefit_refitlast(
            __wsf,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0
        )
    WHERE id = 28;
UPDATE __sinefit_win
    SET
        __wsf = sinefit_refitlast(
            __wsf,
            xx11, yy11,
            xx11, yy11,
            xx11, yy11,
            xx11, yy11,
            xx11, yy11,
            xx11, yy11,
            xx11, yy11,
            xx11, yy11,
            xx12, yy12,
            xx13, yy13
        )
    WHERE id = ${id3};
SELECT
        id,
        ${sqlSinefitExtractLnr("__wsf", 0, "1")},
        ${sqlSinefitExtractLnr("__wsf", 8, "2")},
        ${sqlSinefitExtractLnr("__wsf", 9, "3")}
    FROM __sinefit_win;
                `)
            });
            valActual = valActual[0].map(function ({
                id,
                laa1,
                laa2,
                laa3,
                lbb1,
                lbb2,
                lbb3,
                lee1,
                lee2,
                lee3,
                lxy1,
                lxy2,
                lxy3,
                lyy1,
                lyy2,
                lyy3,
                mee1,
                mee2,
                mee3,
                mxe1,
                mxe2,
                mxe3,
                mxx1,
                mxx2,
                mxx3,
                myy1,
                myy2,
                myy3,
                nnn1,
                nnn2,
                nnn3,
                rr01,
                rr02,
                rr03,
                rr11,
                rr12,
                rr13,
                xx11,
                xx12,
                xx13,
                yy11,
                yy12,
                yy13
            }, ii, list) {
                let obj1;
                let obj2;
                let obj3;
                obj1 = {
                    id,
                    laa: laa1,
                    lbb: lbb1,
                    lee: lee1,
                    lxy: lxy1,
                    lyy: lyy1,
                    mee: mee1,
                    mxe: mxe1,
                    mxx: mxx1,
                    myy: myy1,
                    nnn: nnn1,
                    rr0: rr01,
                    rr1: rr11,
                    xx1: xx11,
                    yy1: yy11
                };
                obj2 = {
                    id,
                    laa: laa2,
                    lbb: lbb2,
                    lee: lee2,
                    lxy: lxy2,
                    lyy: lyy2,
                    mee: mee2,
                    mxe: mxe2,
                    mxx: mxx2,
                    myy: myy2,
                    nnn: nnn2,
                    rr0: rr02,
                    rr1: rr12,
                    xx1: xx12,
                    yy1: yy12
                };
                obj3 = {
                    id,
                    laa: laa3,
                    lbb: lbb3,
                    lee: lee3,
                    lxy: lxy3,
                    lyy: lyy3,
                    mee: mee3,
                    mxe: mxe3,
                    mxx: mxx3,
                    myy: myy3,
                    nnn: nnn3,
                    rr0: rr03,
                    rr1: rr13,
                    xx1: xx13,
                    yy1: yy13
                };
                switch (list.length - ii) {
                case 1:
                    assertJsonEqual(obj2, obj1, valActual);
                    assertJsonEqual(obj3, valExpect3, valActual);
                    break;
                case 2:
                    assertJsonEqual(obj2, obj1, valActual);
                    assertJsonEqual(obj3, valExpect2, valActual);
                    break;
                default:
                    assertJsonEqual(obj2, obj1, valActual);
                    assertJsonEqual(obj3, obj1, valActual);
                }
                return obj1;
            });
            assertJsonEqual(valActual, valExpect);
        }
        valExpect0 = [
            {
                "id": 1,
                "laa": null,
                "lbb": null,
                "lee": null,
                "lxy": null,
                "lyy": null,
                "mee": null,
                "mxe": null,
                "mxx": 2,
                "myy": 0,
                "nnn": 1,
                "rr0": 0,
                "rr1": null,
                "xx1": 2,
                "yy1": 0
            },
            {
                "id": 4,
                "laa": null,
                "lbb": null,
                "lee": null,
                "lxy": null,
                "lyy": null,
                "mee": 0.70710678,
                "mxe": 0,
                "mxx": 2,
                "myy": 0.5,
                "nnn": 2,
                "rr0": 0,
                "rr1": null,
                "xx1": 2,
                "yy1": 1
            },
            {
                "id": 7,
                "laa": -4.5,
                "lbb": 2.5,
                "lee": 0.70710678,
                "lxy": 0.94491118,
                "lyy": 3,
                "mee": 1.52752523,
                "mxe": 0.57735027,
                "mxx": 2.33333333,
                "myy": 1.33333333,
                "nnn": 3,
                "rr0": 0,
                "rr1": 0,
                "xx1": 3,
                "yy1": 3
            },
            {
                "id": 10,
                "laa": -3,
                "lbb": 1.81818182,
                "lee": 0.67419986,
                "lxy": 0.95346259,
                "lyy": 4.27272727,
                "mee": 1.82574186,
                "mxe": 0.95742711,
                "mxx": 2.75,
                "myy": 2,
                "nnn": 4,
                "rr0": 0,
                "rr1": -0.27272727,
                "xx1": 4,
                "yy1": 4
            },
            {
                "id": 13,
                "laa": -2.29411765,
                "lbb": 1.52941176,
                "lee": 0.65678958,
                "lxy": 0.96164474,
                "lyy": 5.35294118,
                "mee": 2.07364414,
                "mxe": 1.30384048,
                "mxx": 3.2,
                "myy": 2.6,
                "nnn": 5,
                "rr0": 0,
                "rr1": -0.35294118,
                "xx1": 5,
                "yy1": 5
            },
            {
                "id": 16,
                "laa": -2.54385965,
                "lbb": 1.63157895,
                "lee": 0.62126074,
                "lxy": 0.97080629,
                "lyy": 5.61403509,
                "mee": 2.31660671,
                "mxe": 1.37840488,
                "mxx": 3.5,
                "myy": 3.16666667,
                "nnn": 6,
                "rr0": 0,
                "rr1": 0.38596491,
                "xx1": 5,
                "yy1": 6
            },
            {
                "id": 19,
                "laa": -2.65,
                "lbb": 1.675,
                "lee": 0.57445626,
                "lxy": 0.9752227,
                "lyy": 5.725,
                "mee": 2.37045304,
                "mxe": 1.38013112,
                "mxx": 3.71428571,
                "myy": 3.57142857,
                "nnn": 7,
                "rr0": 0,
                "rr1": 0.275,
                "xx1": 5,
                "yy1": 6
            },
            {
                "id": 22,
                "laa": -2.5,
                "lbb": 1.625,
                "lee": 0.54006172,
                "lxy": 0.97991187,
                "lyy": 7.25,
                "mee": 2.50713268,
                "mxe": 1.51185789,
                "mxx": 4,
                "myy": 4,
                "nnn": 8,
                "rr0": 0,
                "rr1": -0.25,
                "xx1": 6,
                "yy1": 7
            },
            {
                "id": 25,
                "laa": 0.75,
                "lbb": 0.85,
                "lee": 1.08781126,
                "lxy": 0.89597867,
                "lyy": 9.25,
                "mee": 2.26778684,
                "mxe": 2.39045722,
                "mxx": 5,
                "myy": 5,
                "nnn": 8,
                "rr0": 0,
                "rr1": -1.25,
                "xx1": 10,
                "yy1": 8
            },
            {
                "id": 28,
                "laa": 2.75,
                "lbb": 0.55,
                "lee": 0.99163165,
                "lxy": 0.81989159,
                "lyy": 3.85,
                "mee": 1.60356745,
                "mxe": 2.39045722,
                "mxx": 5,
                "myy": 5.5,
                "nnn": 8,
                "rr0": -0.87387387,
                "rr1": 1.15,
                "xx1": 2,
                "yy1": 5
            }
        ];
        valIn = [
            [2, "abcd"],
            [NaN, 1],
            [3, 3],
            [4, 4],
            [5, 5],
            [5, 6],
            [5, undefined],
            [6, 7],
            //
            [10, 8],
            [2, 5]
        ];
        await Promise.all([
            (async function () {
                let valActual;
                // test win_sinefit2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_sinefit2(1, 2, 3) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                // test win_sinefit2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT doublearray_jsonto(win_sinefit2(1, 2, 3, 4)) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_sinefit2-aggregate-normal handling-behavior
            (async function () {
                let valActual;
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            valIn
                        },
                        db,
                        sql: (`
SELECT
        ${sqlSinefitExtractLnr("__wsf", 0, "")}
    FROM (
        SELECT
            win_sinefit2(1, NULL, value->>0, value->>1) AS __wsf
        FROM JSON_EACH($valIn)
    );
                        `)
                    })
                )[0][0];
                assertJsonEqual(
                    valActual,
                    {
                        "gyy": 0.19611614,
                        "laa": 0.77941176,
                        "lbb": 0.84558824,
                        "lee": 1.56536502,
                        "lxy": 0.81541829,
                        "lyy": 2.47058824,
                        "mee": 2.54950976,
                        "mxe": 2.45854519,
                        "mxx": 4.4,
                        "myy": 4.5,
                        "nnn": 10,
                        "rr0": 0,
                        "rr1": 2.52941176,
                        "xx1": 2,
                        "yy1": 5
                    }
                );
            }()),
            // test win_sinefit2-aggregate-window handling-behavior
            (async function () {
                let valActual;
                let valExpect;
                valExpect = {
                    "gyy": -1.02062073,
                    "laa": -0.82025678,
                    "lbb": 0.14621969,
                    "lee": 2.74202904,
                    "lxy": 0.865665,
                    "lyy": 6.63694722,
                    "mee": 4.89897949,
                    "mxe": 29.00344807,
                    "mxx": 74,
                    "myy": 10,
                    "nnn": 6,
                    "rr0": 0,
                    "rr1": -1.63694722,
                    "xx1": 51,
                    "yy1": 5
                };
                valActual = noop(
                    await dbExecAsync({
                        db,
                        sql: (`
SELECT
        ${sqlSinefitExtractLnr("__wsf", 0, "")}
    FROM (
        SELECT
            win_sinefit2(1, NULL, xx, yy) AS __wsf
        FROM (
            SELECT 34 AS xx, 5 AS yy
            UNION ALL SELECT 108, 17
            UNION ALL SELECT 64, 11
            UNION ALL SELECT 88, 8
            UNION ALL SELECT 99, 14
            UNION ALL SELECT 51, 5
        )
    )
                        `)
                    })
                )[0][0];
                assertJsonEqual(valActual, valExpect);
            }()),
            // test win_sinefit2-aggregate-window handling-behavior
            test_win_sinefit2_aggregate({
                aa: 8,
                bb: 0,
                valExpect: valExpect0,
                valExpect2: {
                    "id": 25,
                    "laa": 5.25,
                    "lbb": -0.275,
                    "lee": 2.88241797,
                    "lxy": -0.23918696,
                    "lyy": 2.5,
                    "mee": 2.74837614,
                    "mxe": 2.39045722,
                    "mxx": 5,
                    "myy": 3.875,
                    "nnn": 8,
                    "rr0": 0,
                    "rr1": -3.5,
                    "xx1": 10,
                    "yy1": -1
                },
                valExpect3: {
                    "id": 28,
                    "laa": 7.25,
                    "lbb": -0.575,
                    "lee": 2.26016224,
                    "lxy": -0.5490214,
                    "lyy": 6.1,
                    "mee": 2.50356888,
                    "mxe": 2.39045722,
                    "mxx": 5,
                    "myy": 4.375,
                    "nnn": 8,
                    "rr0": -3.79279279,
                    "rr1": -1.1,
                    "xx1": 2,
                    "yy1": 5
                }
            }),
            // test win_sinefit2-spx handling-behavior
            (async function () {
                let testDataSpx;
                let ttSinefit = 128;
                let valActual;
                let valExpect;
                testDataSpx = (`
##
date close
2018-12-31 2506.85 2019-01-02 2510.03 2019-01-03 2447.89 2019-01-04 2531.94
2019-01-07 2549.69 2019-01-08 2574.41 2019-01-09 2584.96 2019-01-10 2596.64
2019-01-11 2596.26 2019-01-14 2582.61 2019-01-15 2610.30 2019-01-16 2616.10
2019-01-17 2635.96 2019-01-18 2670.71 2019-01-22 2632.90 2019-01-23 2638.70
2019-01-24 2642.33 2019-01-25 2664.76 2019-01-28 2643.85 2019-01-29 2640.00
2019-01-30 2681.05 2019-01-31 2704.10 2019-02-01 2706.53 2019-02-04 2724.87
2019-02-05 2737.70 2019-02-06 2731.61 2019-02-07 2706.05 2019-02-08 2707.88
2019-02-11 2709.80 2019-02-12 2744.73 2019-02-13 2753.03 2019-02-14 2745.73
2019-02-15 2775.60 2019-02-19 2779.76 2019-02-20 2784.70 2019-02-21 2774.88
2019-02-22 2792.67 2019-02-25 2796.11 2019-02-26 2793.90 2019-02-27 2792.38
2019-02-28 2784.49 2019-03-01 2803.69 2019-03-04 2792.81 2019-03-05 2789.65
2019-03-06 2771.45 2019-03-07 2748.93 2019-03-08 2743.07 2019-03-11 2783.30
2019-03-12 2791.52 2019-03-13 2810.92 2019-03-14 2808.48 2019-03-15 2822.48
2019-03-18 2832.94 2019-03-19 2832.57 2019-03-20 2824.23 2019-03-21 2854.88
2019-03-22 2800.71 2019-03-25 2798.36 2019-03-26 2818.46 2019-03-27 2805.37
2019-03-28 2815.44 2019-03-29 2834.40 2019-04-01 2867.19 2019-04-02 2867.24
2019-04-03 2873.40 2019-04-04 2879.39 2019-04-05 2892.74 2019-04-08 2895.77
2019-04-09 2878.20 2019-04-10 2888.21 2019-04-11 2888.32 2019-04-12 2907.41
2019-04-15 2905.58 2019-04-16 2907.06 2019-04-17 2900.45 2019-04-18 2905.03
2019-04-22 2907.97 2019-04-23 2933.68 2019-04-24 2927.25 2019-04-25 2926.17
2019-04-26 2939.88 2019-04-29 2943.03 2019-04-30 2945.83 2019-05-01 2923.73
2019-05-02 2917.52 2019-05-03 2945.64 2019-05-06 2932.47 2019-05-07 2884.05
2019-05-08 2879.42 2019-05-09 2870.72 2019-05-10 2881.40 2019-05-13 2811.87
2019-05-14 2834.41 2019-05-15 2850.96 2019-05-16 2876.32 2019-05-17 2859.53
2019-05-20 2840.23 2019-05-21 2864.36 2019-05-22 2856.27 2019-05-23 2822.24
2019-05-24 2826.06 2019-05-28 2802.39 2019-05-29 2783.02 2019-05-30 2788.86
2019-05-31 2752.06 2019-06-03 2744.45 2019-06-04 2803.27 2019-06-05 2826.15
2019-06-06 2843.49 2019-06-07 2873.34 2019-06-10 2886.73 2019-06-11 2885.72
2019-06-12 2879.84 2019-06-13 2891.64 2019-06-14 2886.98 2019-06-17 2889.67
2019-06-18 2917.75 2019-06-19 2926.46 2019-06-20 2954.18 2019-06-21 2950.46
2019-06-24 2945.35 2019-06-25 2917.38 2019-06-26 2913.78 2019-06-27 2924.92
2019-06-28 2941.76 2019-07-01 2964.33 2019-07-02 2973.01 2019-07-03 2995.82
2019-07-05 2990.41 2019-07-08 2975.95 2019-07-09 2979.63 2019-07-10 2993.07
2019-07-11 2999.91 2019-07-12 3013.77 2019-07-15 3014.30 2019-07-16 3004.04
2019-07-17 2984.42 2019-07-18 2995.11 2019-07-19 2976.61 2019-07-22 2985.03
2019-07-23 3005.47 2019-07-24 3019.56 2019-07-25 3003.67 2019-07-26 3025.86
2019-07-29 3020.97 2019-07-30 3013.18 2019-07-31 2980.38 2019-08-01 2953.56
2019-08-02 2932.05 2019-08-05 2844.74 2019-08-06 2881.77 2019-08-07 2883.98
2019-08-08 2938.09 2019-08-09 2918.65 2019-08-12 2882.70 2019-08-13 2926.32
2019-08-14 2840.60 2019-08-15 2847.60 2019-08-16 2888.68 2019-08-19 2923.65
2019-08-20 2900.51 2019-08-21 2924.43 2019-08-22 2922.95 2019-08-23 2847.11
2019-08-26 2878.38 2019-08-27 2869.16 2019-08-28 2887.94 2019-08-29 2924.58
2019-08-30 2926.46 2019-09-03 2906.27 2019-09-04 2937.78 2019-09-05 2976.00
2019-09-06 2978.71 2019-09-09 2978.43 2019-09-10 2979.39 2019-09-11 3000.93
2019-09-12 3009.57 2019-09-13 3007.39 2019-09-16 2997.96 2019-09-17 3005.70
2019-09-18 3006.73 2019-09-19 3006.79 2019-09-20 2992.07 2019-09-23 2991.78
2019-09-24 2966.60 2019-09-25 2984.87 2019-09-26 2977.62 2019-09-27 2961.79
2019-09-30 2976.74 2019-10-01 2940.25 2019-10-02 2887.61 2019-10-03 2910.63
2019-10-04 2952.01 2019-10-07 2938.79 2019-10-08 2893.06 2019-10-09 2919.40
2019-10-10 2938.13 2019-10-11 2970.27 2019-10-14 2966.15 2019-10-15 2995.68
2019-10-16 2989.69 2019-10-17 2997.95 2019-10-18 2986.20 2019-10-21 3006.72
2019-10-22 2995.99 2019-10-23 3004.52 2019-10-24 3010.29 2019-10-25 3022.55
2019-10-28 3039.42 2019-10-29 3036.89 2019-10-30 3046.77 2019-10-31 3037.56
2019-11-01 3066.91 2019-11-04 3078.27 2019-11-05 3074.62 2019-11-06 3076.78
2019-11-07 3085.18 2019-11-08 3093.08 2019-11-11 3087.01 2019-11-12 3091.84
2019-11-13 3094.04 2019-11-14 3096.63 2019-11-15 3120.46 2019-11-18 3122.03
2019-11-19 3120.18 2019-11-20 3108.46 2019-11-21 3103.54 2019-11-22 3110.29
2019-11-25 3133.64 2019-11-26 3140.52 2019-11-27 3153.63 2019-11-29 3140.98
2019-12-02 3113.87 2019-12-03 3093.20 2019-12-04 3112.76 2019-12-05 3117.43
2019-12-06 3145.91 2019-12-09 3135.96 2019-12-10 3132.52 2019-12-11 3141.63
2019-12-12 3168.57 2019-12-13 3168.80 2019-12-16 3191.45 2019-12-17 3192.52
2019-12-18 3191.14 2019-12-19 3205.37 2019-12-20 3221.22 2019-12-23 3224.01
2019-12-24 3223.38 2019-12-26 3239.91 2019-12-27 3240.02 2019-12-30 3221.29
2019-12-31 3230.78 2020-01-02 3257.85 2020-01-03 3234.85 2020-01-06 3246.28
2020-01-07 3237.18 2020-01-08 3253.05 2020-01-09 3274.70 2020-01-10 3265.35
2020-01-13 3288.13 2020-01-14 3283.15 2020-01-15 3289.29 2020-01-16 3316.81
2020-01-17 3329.62 2020-01-21 3320.79 2020-01-22 3321.75 2020-01-23 3325.54
2020-01-24 3295.47 2020-01-27 3243.63 2020-01-28 3276.24 2020-01-29 3273.40
2020-01-30 3283.66 2020-01-31 3225.52 2020-02-03 3248.92 2020-02-04 3297.59
2020-02-05 3334.69 2020-02-06 3345.78 2020-02-07 3327.71 2020-02-10 3352.09
2020-02-11 3357.75 2020-02-12 3379.45 2020-02-13 3373.94 2020-02-14 3380.16
2020-02-18 3370.29 2020-02-19 3386.15 2020-02-20 3373.23 2020-02-21 3337.75
2020-02-24 3225.89 2020-02-25 3128.21 2020-02-26 3116.39 2020-02-27 2978.76
2020-02-28 2954.22 2020-03-02 3090.23 2020-03-03 3003.37 2020-03-04 3130.12
2020-03-05 3023.94 2020-03-06 2972.37 2020-03-09 2746.56 2020-03-10 2882.23
2020-03-11 2741.38 2020-03-12 2480.64 2020-03-13 2711.02 2020-03-16 2386.13
2020-03-17 2529.19 2020-03-18 2398.10 2020-03-19 2409.39 2020-03-20 2304.92
2020-03-23 2237.40 2020-03-24 2447.33 2020-03-25 2475.56 2020-03-26 2630.07
2020-03-27 2541.47 2020-03-30 2626.65 2020-03-31 2584.59 2020-04-01 2470.50
2020-04-02 2526.90 2020-04-03 2488.65 2020-04-06 2663.68 2020-04-07 2659.41
2020-04-08 2749.98 2020-04-09 2789.82 2020-04-13 2761.63 2020-04-14 2846.06
2020-04-15 2783.36 2020-04-16 2799.55 2020-04-17 2874.56 2020-04-20 2823.16
2020-04-21 2736.56 2020-04-22 2799.31 2020-04-23 2797.80 2020-04-24 2836.74
2020-04-27 2878.48 2020-04-28 2863.39 2020-04-29 2939.51 2020-04-30 2912.43
2020-05-01 2830.71 2020-05-04 2842.74 2020-05-05 2868.44 2020-05-06 2848.42
2020-05-07 2881.19 2020-05-08 2929.80 2020-05-11 2930.19 2020-05-12 2870.12
2020-05-13 2820.00 2020-05-14 2852.50 2020-05-15 2863.70 2020-05-18 2953.91
2020-05-19 2922.94 2020-05-20 2971.61 2020-05-21 2948.51 2020-05-22 2955.45
2020-05-26 2991.77 2020-05-27 3036.13 2020-05-28 3029.73 2020-05-29 3044.31
2020-06-01 3055.73 2020-06-02 3080.82 2020-06-03 3122.87 2020-06-04 3112.35
2020-06-05 3193.93 2020-06-08 3232.39 2020-06-09 3207.18 2020-06-10 3190.14
2020-06-11 3002.10 2020-06-12 3041.31 2020-06-15 3066.59 2020-06-16 3124.74
2020-06-17 3113.49 2020-06-18 3115.34 2020-06-19 3097.74 2020-06-22 3117.86
2020-06-23 3131.29 2020-06-24 3050.33 2020-06-25 3083.76 2020-06-26 3009.05
2020-06-29 3053.24 2020-06-30 3100.29 2020-07-01 3115.86 2020-07-02 3130.01
2020-07-06 3179.72 2020-07-07 3145.32 2020-07-08 3169.94 2020-07-09 3152.05
2020-07-10 3185.04 2020-07-13 3155.22 2020-07-14 3197.52 2020-07-15 3226.56
2020-07-16 3215.57 2020-07-17 3224.73 2020-07-20 3251.84 2020-07-21 3257.30
2020-07-22 3276.02 2020-07-23 3235.66 2020-07-24 3215.63 2020-07-27 3239.41
2020-07-28 3218.44 2020-07-29 3258.44 2020-07-30 3246.22 2020-07-31 3271.12
2020-08-03 3294.61 2020-08-04 3306.51 2020-08-05 3327.77 2020-08-06 3349.16
2020-08-07 3351.28 2020-08-10 3360.47 2020-08-11 3333.69 2020-08-12 3380.35
2020-08-13 3373.43 2020-08-14 3372.85 2020-08-17 3381.99 2020-08-18 3389.78
2020-08-19 3374.85 2020-08-20 3385.51 2020-08-21 3397.16 2020-08-24 3431.28
2020-08-25 3443.62 2020-08-26 3478.73 2020-08-27 3484.55 2020-08-28 3508.01
2020-08-31 3500.31 2020-09-01 3526.65 2020-09-02 3580.84 2020-09-03 3455.06
2020-09-04 3426.96 2020-09-08 3331.84 2020-09-09 3398.96 2020-09-10 3339.19
2020-09-11 3340.97 2020-09-14 3383.54 2020-09-15 3401.20 2020-09-16 3385.49
2020-09-17 3357.01 2020-09-18 3319.47 2020-09-21 3281.06 2020-09-22 3315.57
2020-09-23 3236.92 2020-09-24 3246.59 2020-09-25 3298.46 2020-09-28 3351.60
2020-09-29 3335.47 2020-09-30 3363.00 2020-10-01 3380.80 2020-10-02 3348.42
2020-10-05 3408.60 2020-10-06 3360.97 2020-10-07 3419.44 2020-10-08 3446.83
2020-10-09 3477.14 2020-10-12 3534.22 2020-10-13 3511.93 2020-10-14 3488.67
2020-10-15 3483.34 2020-10-16 3483.81 2020-10-19 3426.92 2020-10-20 3443.12
2020-10-21 3435.56 2020-10-22 3453.49 2020-10-23 3465.39 2020-10-26 3400.97
2020-10-27 3390.68 2020-10-28 3271.03 2020-10-29 3310.11 2020-10-30 3269.96
2020-11-02 3310.24 2020-11-03 3369.16 2020-11-04 3443.44 2020-11-05 3510.45
2020-11-06 3509.44 2020-11-09 3550.50 2020-11-10 3545.53 2020-11-11 3572.66
2020-11-12 3537.01 2020-11-13 3585.15 2020-11-16 3626.91 2020-11-17 3609.53
2020-11-18 3567.79 2020-11-19 3581.87 2020-11-20 3557.54 2020-11-23 3577.59
2020-11-24 3635.41 2020-11-25 3629.65 2020-11-27 3638.35 2020-11-30 3621.63
2020-12-01 3662.45 2020-12-02 3669.01 2020-12-03 3666.72 2020-12-04 3699.12
2020-12-07 3691.96 2020-12-08 3702.25 2020-12-09 3672.82 2020-12-10 3668.10
2020-12-11 3663.46 2020-12-14 3647.49 2020-12-15 3694.62 2020-12-16 3701.17
2020-12-17 3722.48 2020-12-18 3709.41 2020-12-21 3694.92 2020-12-22 3687.26
2020-12-23 3690.01 2020-12-24 3703.06 2020-12-28 3735.36 2020-12-29 3727.04
2020-12-30 3732.04 2020-12-31 3756.07 2021-01-04 3700.65 2021-01-05 3726.86
2021-01-06 3748.14 2021-01-07 3803.79 2021-01-08 3824.68 2021-01-11 3799.61
2021-01-12 3801.19 2021-01-13 3809.84 2021-01-14 3795.54 2021-01-15 3768.25
2021-01-19 3798.91 2021-01-20 3851.85 2021-01-21 3853.07 2021-01-22 3841.47
2021-01-25 3855.36 2021-01-26 3849.62 2021-01-27 3750.77 2021-01-28 3787.38
2021-01-29 3714.24 2021-02-01 3773.86 2021-02-02 3826.31 2021-02-03 3830.17
2021-02-04 3871.74 2021-02-05 3886.83 2021-02-08 3915.59 2021-02-09 3911.23
2021-02-10 3909.88 2021-02-11 3916.38 2021-02-12 3934.83 2021-02-16 3932.59
2021-02-17 3931.33 2021-02-18 3913.97 2021-02-19 3906.71 2021-02-22 3876.50
2021-02-23 3881.37 2021-02-24 3925.43 2021-02-25 3829.34 2021-02-26 3811.15
2021-03-01 3901.82 2021-03-02 3870.29 2021-03-03 3819.72 2021-03-04 3768.47
2021-03-05 3841.94 2021-03-08 3821.35 2021-03-09 3875.44 2021-03-10 3898.81
2021-03-11 3939.34 2021-03-12 3943.34 2021-03-15 3968.94 2021-03-16 3962.71
2021-03-17 3974.12 2021-03-18 3915.46 2021-03-19 3913.10 2021-03-22 3940.59
2021-03-23 3910.52 2021-03-24 3889.14 2021-03-25 3909.52 2021-03-26 3974.54
2021-03-29 3971.09 2021-03-30 3958.55 2021-03-31 3972.89 2021-04-01 4019.87
2021-04-05 4077.91 2021-04-06 4073.94 2021-04-07 4079.95 2021-04-08 4097.17
2021-04-09 4128.80 2021-04-12 4127.99 2021-04-13 4141.59 2021-04-14 4124.66
2021-04-15 4170.42 2021-04-16 4185.47 2021-04-19 4163.26 2021-04-20 4134.94
2021-04-21 4173.42 2021-04-22 4134.98 2021-04-23 4180.17 2021-04-26 4187.62
2021-04-27 4186.72 2021-04-28 4183.18 2021-04-29 4211.47 2021-04-30 4181.17
2021-05-03 4192.66 2021-05-04 4164.66 2021-05-05 4167.59 2021-05-06 4201.62
2021-05-07 4232.60 2021-05-10 4188.43 2021-05-11 4152.10 2021-05-12 4063.04
2021-05-13 4112.50 2021-05-14 4173.85 2021-05-17 4163.29 2021-05-18 4127.83
2021-05-19 4115.68 2021-05-20 4159.12 2021-05-21 4155.86 2021-05-24 4197.05
2021-05-25 4188.13 2021-05-26 4195.99 2021-05-27 4200.88 2021-05-28 4204.11
2021-06-01 4202.04 2021-06-02 4208.12 2021-06-03 4192.85 2021-06-04 4229.89
2021-06-07 4226.52 2021-06-08 4227.26 2021-06-09 4219.55 2021-06-10 4239.18
2021-06-11 4247.44 2021-06-14 4255.15 2021-06-15 4246.59 2021-06-16 4223.70
2021-06-17 4221.86 2021-06-18 4166.45 2021-06-21 4224.79 2021-06-22 4246.44
2021-06-23 4241.84 2021-06-24 4266.49 2021-06-25 4280.70 2021-06-28 4290.61
2021-06-29 4291.80 2021-06-30 4297.50 2021-07-01 4319.94 2021-07-02 4352.34
2021-07-06 4343.54 2021-07-07 4358.13 2021-07-08 4320.82 2021-07-09 4369.55
2021-07-12 4384.63 2021-07-13 4369.21 2021-07-14 4374.30 2021-07-15 4360.03
2021-07-16 4327.16 2021-07-19 4258.49 2021-07-20 4323.06 2021-07-21 4358.69
2021-07-22 4367.48 2021-07-23 4411.79 2021-07-26 4422.30 2021-07-27 4401.46
2021-07-28 4400.64 2021-07-29 4419.15 2021-07-30 4395.26 2021-08-02 4387.16
2021-08-03 4423.15 2021-08-04 4402.66 2021-08-05 4429.10 2021-08-06 4436.52
2021-08-09 4432.35 2021-08-10 4436.75 2021-08-11 4442.41 2021-08-12 4460.83
2021-08-13 4468.00 2021-08-16 4479.71 2021-08-17 4448.08 2021-08-18 4400.27
2021-08-19 4405.80 2021-08-20 4441.67 2021-08-23 4479.53 2021-08-24 4486.23
2021-08-25 4496.19 2021-08-26 4470.00 2021-08-27 4509.37 2021-08-30 4528.79
2021-08-31 4522.68 2021-09-01 4524.09 2021-09-02 4536.95 2021-09-03 4535.43
2021-09-07 4520.03 2021-09-08 4514.07 2021-09-09 4493.28 2021-09-10 4458.58
2021-09-13 4468.73 2021-09-14 4443.05 2021-09-15 4480.70 2021-09-16 4473.75
2021-09-17 4432.99 2021-09-20 4357.73 2021-09-21 4354.19 2021-09-22 4395.64
2021-09-23 4448.98 2021-09-24 4455.48 2021-09-27 4443.11 2021-09-28 4352.63
2021-09-29 4359.46 2021-09-30 4307.54 2021-10-01 4357.04 2021-10-04 4300.46
2021-10-05 4345.72 2021-10-06 4363.55 2021-10-07 4399.76 2021-10-08 4391.34
2021-10-11 4361.19 2021-10-12 4350.65 2021-10-13 4363.80 2021-10-14 4438.26
2021-10-15 4471.37 2021-10-18 4486.46 2021-10-19 4519.63 2021-10-20 4536.19
2021-10-21 4549.78 2021-10-22 4544.90 2021-10-25 4566.48 2021-10-26 4574.79
2021-10-27 4551.68 2021-10-28 4596.42 2021-10-29 4605.38 2021-11-01 4613.67
2021-11-02 4630.65 2021-11-03 4660.57 2021-11-04 4680.06 2021-11-05 4697.53
2021-11-08 4701.70 2021-11-09 4685.25 2021-11-10 4646.71 2021-11-11 4649.27
2021-11-12 4682.85 2021-11-15 4682.80 2021-11-16 4700.90 2021-11-17 4688.67
2021-11-18 4704.54 2021-11-19 4697.96 2021-11-22 4682.94 2021-11-23 4690.70
2021-11-24 4701.46 2021-11-26 4594.62 2021-11-29 4655.27 2021-11-30 4567.00
2021-12-01 4513.04 2021-12-02 4577.10 2021-12-03 4538.43 2021-12-06 4591.67
2021-12-07 4686.75 2021-12-08 4701.21 2021-12-09 4667.45 2021-12-10 4712.02
2021-12-13 4668.97 2021-12-14 4634.09 2021-12-15 4709.85 2021-12-16 4668.67
2021-12-17 4620.64 2021-12-20 4568.02 2021-12-21 4649.23 2021-12-22 4696.56
2021-12-23 4725.79 2021-12-27 4791.19 2021-12-28 4786.35 2021-12-29 4793.06
2021-12-30 4778.73 2021-12-31 4766.18 2022-01-03 4796.56 2022-01-04 4793.54
2022-01-05 4700.58 2022-01-06 4696.05 2022-01-07 4677.03 2022-01-10 4670.29
2022-01-11 4713.07 2022-01-12 4726.35 2022-01-13 4659.03 2022-01-14 4662.85
2022-01-18 4577.11 2022-01-19 4532.76 2022-01-20 4482.73 2022-01-21 4397.94
2022-01-24 4410.13 2022-01-25 4356.45 2022-01-26 4349.93 2022-01-27 4326.51
2022-01-28 4431.85 2022-01-31 4515.55 2022-02-01 4546.54 2022-02-02 4589.38
2022-02-03 4477.44 2022-02-04 4500.53 2022-02-07 4483.87 2022-02-08 4521.54
2022-02-09 4587.18 2022-02-10 4504.08 2022-02-11 4418.64 2022-02-14 4401.67
2022-02-15 4471.07 2022-02-16 4475.01 2022-02-17 4380.26 2022-02-18 4348.87
2022-02-22 4304.76 2022-02-23 4225.50 2022-02-24 4288.70 2022-02-25 4384.65
2022-02-28 4373.94 2022-03-01 4306.26 2022-03-02 4386.54 2022-03-03 4363.49
2022-03-04 4328.87 2022-03-07 4201.09 2022-03-08 4170.70 2022-03-09 4277.88
2022-03-10 4259.52 2022-03-11 4204.31 2022-03-14 4173.11 2022-03-15 4262.45
2022-03-16 4357.86 2022-03-17 4411.67 2022-03-18 4463.12 2022-03-21 4461.18
2022-03-22 4511.61 2022-03-23 4456.24 2022-03-24 4520.16 2022-03-25 4543.06
2022-03-28 4575.52 2022-03-29 4631.60 2022-03-30 4602.45 2022-03-31 4530.41
2022-04-01 4545.86 2022-04-04 4582.64 2022-04-05 4525.12 2022-04-06 4481.15
2022-04-07 4500.21 2022-04-08 4488.28 2022-04-11 4412.53 2022-04-12 4397.45
2022-04-13 4446.59 2022-04-14 4392.59 2022-04-18 4391.69 2022-04-19 4462.21
2022-04-20 4459.45 2022-04-21 4393.66 2022-04-22 4271.78 2022-04-25 4296.12
2022-04-26 4175.20 2022-04-27 4183.96 2022-04-28 4287.50 2022-04-29 4131.93
2022-05-02 4155.38 2022-05-03 4175.48 2022-05-04 4300.17 2022-05-05 4146.87
2022-05-06 4123.34 2022-05-09 3991.24 2022-05-10 4001.05 2022-05-11 3935.18
2022-05-12 3930.08 2022-05-13 4023.89 2022-05-16 4008.01 2022-05-17 4088.85
2022-05-18 3923.68 2022-05-19 3900.79 2022-05-20 3901.36 2022-05-23 3973.75
2022-05-24 3941.48 2022-05-25 3978.73 2022-05-26 4057.84 2022-05-27 4158.24
2022-05-31 4132.15 2022-06-01 4101.23 2022-06-02 4176.82 2022-06-03 4108.54
2022-06-06 4121.43 2022-06-07 4160.68 2022-06-08 4115.77 2022-06-09 4017.82
2022-06-10 3900.86 2022-06-13 3749.63 2022-06-14 3735.48 2022-06-15 3789.99
2022-06-16 3666.77 2022-06-17 3674.84 2022-06-21 3764.79 2022-06-22 3759.89
2022-06-23 3795.73 2022-06-24 3911.74 2022-06-27 3900.11 2022-06-28 3821.55
2022-06-29 3818.83 2022-06-30 3785.38 2022-07-01 3825.33 2022-07-05 3831.39
2022-07-06 3845.08 2022-07-07 3902.62 2022-07-08 3899.38 2022-07-11 3854.43
2022-07-12 3818.80 2022-07-13 3801.78 2022-07-14 3790.38 2022-07-15 3863.16
2022-07-18 3830.85 2022-07-19 3936.69 2022-07-20 3959.90 2022-07-21 3998.95
2022-07-22 3961.63 2022-07-25 3966.84 2022-07-26 3921.05 2022-07-27 4023.61
2022-07-28 4072.43 2022-07-29 4130.29 2022-08-01 4118.63 2022-08-02 4091.19
2022-08-03 4155.17 2022-08-04 4151.94 2022-08-05 4145.19 2022-08-08 4140.06
2022-08-09 4122.47 2022-08-10 4210.24 2022-08-11 4207.27 2022-08-12 4280.15
2022-08-15 4297.14 2022-08-16 4305.20 2022-08-17 4274.04 2022-08-18 4283.74
2022-08-19 4228.48 2022-08-22 4137.99 2022-08-23 4128.73 2022-08-24 4140.77
2022-08-25 4199.12 2022-08-26 4057.66 2022-08-29 4030.61 2022-08-30 3986.16
2022-08-31 3955.00 2022-09-01 3966.85 2022-09-02 3924.26 2022-09-06 3908.19
2022-09-07 3979.87 2022-09-08 4006.18 2022-09-09 4067.36 2022-09-12 4110.41
2022-09-13 3932.69 2022-09-14 3946.01 2022-09-15 3901.35 2022-09-16 3873.33
2022-09-19 3899.89 2022-09-20 3855.93 2022-09-21 3789.93 2022-09-22 3757.99
2022-09-23 3693.23 2022-09-26 3655.04 2022-09-27 3647.29 2022-09-28 3719.04
2022-09-29 3640.47 2022-09-30 3585.62 2022-10-03 3678.43 2022-10-04 3790.93
2022-10-05 3783.28 2022-10-06 3744.52 2022-10-07 3639.66 2022-10-10 3612.39
2022-10-11 3588.84 2022-10-12 3577.03 2022-10-13 3669.91 2022-10-14 3583.07
2022-10-17 3677.95 2022-10-18 3719.98 2022-10-19 3695.16 2022-10-20 3665.78
2022-10-21 3752.75 2022-10-24 3797.34 2022-10-25 3859.11 2022-10-26 3830.60
2022-10-27 3807.30 2022-10-28 3901.06 2022-10-31 3871.98 2022-11-01 3856.10
2022-11-02 3759.69 2022-11-03 3719.89 2022-11-04 3770.55 2022-11-07 3806.80
2022-11-08 3828.11 2022-11-09 3748.57 2022-11-10 3956.37 2022-11-11 3992.93
2022-11-14 3957.25 2022-11-15 3991.73 2022-11-16 3958.79 2022-11-17 3946.56
2022-11-18 3965.34 2022-11-21 3949.94 2022-11-22 4003.58 2022-11-23 4027.26
2022-11-25 4026.12 2022-11-28 3963.94 2022-11-29 3957.63 2022-11-30 4080.11
2022-12-01 4076.57 2022-12-02 4071.70 2022-12-05 3998.84 2022-12-06 3941.26
2022-12-07 3933.92 2022-12-08 3963.51 2022-12-09 3934.38 2022-12-12 3990.56
2022-12-13 4019.65 2022-12-14 3995.32 2022-12-15 3895.75 2022-12-16 3852.36
2022-12-19 3817.66 2022-12-20 3821.62 2022-12-21 3878.44 2022-12-22 3822.39
2022-12-23 3844.82 2022-12-27 3829.25 2022-12-28 3783.22 2022-12-29 3849.28
2022-12-30 3839.50 2023-01-03 3824.14 2023-01-04 3852.97 2023-01-05 3808.10
##
                `);
                testDataSpx = testDataSpx.replace((/ (20..-)/g), "\n$1");
                testDataSpx = testDataSpx.trim().split("\n").slice(2, -1);
                testDataSpx = testDataSpx.map(function (elem, ii) {
                    elem = elem.split(" ");
                    return {
                        date: elem[0],
                        ii,
                        priceClose: Number(elem[1])
                    };
                });
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            testDataSpx
                        },
                        db,
                        sql: (`
DROP TABLE IF EXISTS __sinefit_csv;
CREATE TEMP TABLE __sinefit_csv AS
    SELECT
        *,
        win_sinefit2(1, NULL, ii, yy, ii, yy) OVER (
            ORDER BY date ASC
            ROWS BETWEEN ${ttSinefit - 1} PRECEDING AND 0 FOLLOWING
        ) AS __wsf
    FROM (
        SELECT
            value->>'ii' AS ii,
            value->>'date' AS date,
            value->>'priceClose' AS yy
        FROM JSON_EAcH($testDataSpx)
    );
SELECT
        *,
        sinefit_extract(__wsf, 0, 'saa', 0) AS saa,
        sinefit_extract(__wsf, 0, 'see', 0) AS see,
        sinefit_extract(__wsf, 0, 'spp', 0) AS spp,
        sinefit_extract(__wsf, 0, 'stt', 0) AS stt,
        sinefit_extract(__wsf, 0, 'sww', 0) AS sww,
        sinefit_extract(__wsf, 0, 'syy', 0) AS syy,
        sinefit_extract(__wsf, 0, 'vxx', 0) AS vxx,
        ${sqlSinefitExtractLnr("__wsf", 0, "")}
    FROM __sinefit_csv
    JOIN (
        SELECT
            MEDIAN(yy) AS yy_avg,
            STDEV(yy) AS yy_err
        FROM __sinefit_csv
    )
    LEFT JOIN (
        SELECT
            ii + 1 AS ii,
            sinefit_extract(__wsf, 0, 'predict_all', ii + 1) AS predict,
            sinefit_extract(__wsf, 0, 'predict_lnr', ii + 1) AS predict_lnr,
            sinefit_extract(__wsf, 0, 'predict_snr', ii + 1) AS predict_snr
        FROM __sinefit_csv
    ) USING (ii);
                        `)
                    })
                )[0];
                valActual = (
                    "date saa sww spp stt"
                    + " rr0 rr1 ii yy linear sinefit sine\n"
                    + valActual.map(function (elem) {
                        return [
                            elem.date,
                            elem.saa,
                            elem.sww,
                            elem.spp,
                            elem.stt,
                            elem.rr0,
                            elem.rr1,
                            elem.ii,
                            10 + (elem.yy - elem.yy_avg) / elem.yy_err,
                            10 + (elem.predict_lnr - elem.yy_avg) / elem.yy_err,
                            10 + (elem.predict - elem.yy_avg) / elem.yy_err,
                            10 + elem.predict_snr / 100
                        ].map(function (num) {
                            return (
                                typeof num === "number"
                                ? num.toFixed(4)
                                : num
                            );
                        }).join(" ");
                    }).join("\n")
                );
                valActual = valActual.replace((/  /g), " null ");
                valActual = valActual.replace((/ \n/g), "\n");
                valActual = valActual.replace((/ /g), "\t");
                valActual = valActual.trim() + "\n";
                await fsWriteFileUnlessTest(
                    "test_data_sinefit.csv",
                    valActual,
                    String("1").replace(npm_config_mode_test_save, "force")
                );
                valExpect = await fsReadFileUnlessTest(
                    "test_data_sinefit.csv",
                    "force"
                );
                assertJsonEqual(valActual, valExpect);
            }())
        ]);
    });
});

jstestDescribe((
    "test_sqlmathWebworkerInit"
), function test_sqlmathWebworkerInit() {
    jstestIt((
        "test sqlmathWebworkerInit handling-behavior"
    ), async function () {
        let db = await dbOpenAsync({
            dbData: new ArrayBuffer(),
            filename: ":memory:"
        });
        sqlmathWebworkerInit({
            db,
            modeTest: true
        });
    });
});
