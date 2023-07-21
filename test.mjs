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


/*jslint beta, node*/
import jslint from "./jslint.mjs";
import {
    SQLITE_WIN_SLR_ELEM_SIZE,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    childProcessSpawn2,
    ciBuildExt,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAndReturnLastJsonAsync,
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
    sqlmathWebworkerInit,
    version
} from "./sqlmath.mjs";
let VECTOR99_NOFFSET = 6;
let {
    assertErrorThrownAsync,
    jstestDescribe,
    jstestIt
} = jslint;
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
    "test_ccallAsync"
), function test_ccallAsync() {
    jstestIt((
        "test cCallAsync handling-behavior"
    ), async function () {
        // test datatype handling-behavior
        await Promise.all([
            ["", ""],
            ["\u0000", ""],
            ["aa", "aa"],
            [-0, 0],
            [-0.5, undefined],
            [-0n, -0],
            [-0x8000000000000000n, -0x8000000000000000n],
            [-0x8000000000000001n, 0x7fffffffffffffffn],
            [-1 / 0, undefined],
            [-1e-999, 0],
            [-1e999, undefined],
            [-1n, -1],
            [-2, -2],
            [-2n, -2],
            [-Infinity, undefined],
            [-NaN, undefined],
            [0, 0],
            [0.5, undefined],
            [0n, 0],
            [0x7fffffffffffffffn, 0x7fffffffffffffffn],
            [0x8000000000000000n, -0x8000000000000000n],
            [1 / 0, undefined],
            [1e-999, 0],
            [1e999, undefined],
            [1n, 1],
            [2, 2],
            [2n, 2],
            [Infinity, undefined],
            [NaN, undefined],
            [Symbol(), 0],
            [false, 0],
            [noop, 0],
            [null, 0],
            [true, 1],
            [undefined, 0],
            [{}, 0]
        ].map(async function ([
            valInput, valExpected
        ]) {
            let baton;
            let valActual;
            if (valExpected === undefined) {
                assertErrorThrownAsync(function () {
                    return dbNoopAsync(undefined, valInput, undefined);
                });
                return;
            }
            baton = await dbNoopAsync(undefined, valInput, undefined);
            baton = baton[0];
            valActual = (
                typeof valInput === "string"
                ? jsbatonValueString({
                    argi: 1,
                    baton
                })
                : String(baton.getBigInt64(4 + 4 + 8, true))
            );
            valExpected = String(valExpected);
            if (typeof valInput === "bigint") {
                valInput = String(valInput);
            }
            assertJsonEqual(valActual, valExpected, {
                valActual,
                valExpected,
                valInput
            });
        }));
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
        "test db-bind-value handling-behavior"
    ), async function test_dbBindValue() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        async function test_dbExecAndReturnLastBlobAsync(val) {
            return await dbExecAndReturnLastBlobAsync({
                bindList: [
                    val
                ],
                db,
                sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
            });
        }
        // test bigint-error handling-behavior
        noop([
            -(2n ** 63n + 1n),
            2n ** 63n
        ]).forEach(function (val) {
            assertErrorThrownAsync(
                test_dbExecAndReturnLastBlobAsync.bind(undefined, val)
            );
        });
        // test datatype handling-behavior
        await Promise.all([
            // 1. bigint
            [-0n, -0],
            [-0x8000000000000000n, "-9223372036854775808"],
            [-1n, -1],
            [-2n, -2],
            [0n, 0],
            [0x7fffffffffffffffn, "9223372036854775807"],
            [1n, 1],
            [2n, 2],
            // 2. boolean
            [false, 0],
            [true, 1],
            // 3. function
            [noop, null],
            // 4. number
            [-0, 0],
            [-0.5, -0.5],
            [-1 / 0, null],
            [-1e-999, 0],
            [-1e999, null],
            [-2, -2],
            [-Infinity, null],
            [-NaN, 0],
            [0, 0],
            [0.5, 0.5],
            [1 / 0, null],
            [1e-999, 0],
            [1e999, null],
            [2, 2],
            [Infinity, null],
            [NaN, 0],
            // 5. object
            [[], "[]"],
            [new Date(0), "1970-01-01T00:00:00.000Z"],
            [new RegExp(), "{}"],
            [new SharedArrayBuffer(0), null],
            [new TextEncoder().encode(""), null],
            [new TextEncoder().encode("\u0000"), null],
            [new TextEncoder().encode("\u0000\u{1f600}\u0000"), null],
            [new Uint8Array(0), null],
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
        ].map(async function ([
            valInput, valExpected
        ], ii) {
            // test dbExecAndReturnLastBlobAsync's bind handling-behavior
            await Promise.all([
                valInput
            ].map(async function (valInput) {
                let bufActual = new TextDecoder().decode(
                    await test_dbExecAndReturnLastBlobAsync(valInput)
                );
                let bufExpected = String(valExpected);
                switch (typeof(valInput)) {
                case "bigint":
                    valInput = Number(valInput);
                    break;
                case "function":
                case "symbol":
                case "undefined":
                    bufExpected = "";
                    break;
                case "number":
                    switch (valInput) {
                    case -2:
                        bufExpected = "-2.0";
                        break;
                    case -Infinity:
                        bufExpected = "-Inf";
                        break;
                    case 2:
                        bufExpected = "2.0";
                        break;
                    case Infinity:
                        bufExpected = "Inf";
                        break;
                    }
                    break;
                case "object":
                    if (ArrayBuffer.isView(valInput)) {
                        bufExpected = new TextDecoder().decode(valInput);
                        break;
                    }
                    if (
                        valInput === null
                        || valInput.constructor === SharedArrayBuffer
                    ) {
                        bufExpected = "";
                    }
                    break;
                }
                assertJsonEqual(bufActual, bufExpected, {
                    bufActual,
                    bufExpected,
                    ii,
                    valExpected,
                    valInput
                });
            }));
            // test dbExecAsync's responseType handling-behavior
            await Promise.all([
                "arraybuffer",
                "list",
                undefined
            ].map(async function (responseType) {
                let bufActual = await dbExecAsync({
                    bindList: [
                        valInput
                    ],
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
                assertJsonEqual(bufActual, valExpected, {
                    bufActual,
                    ii,
                    responseType,
                    valExpected,
                    valInput
                });
            }));
            // test dbExecAsync's bind handling-behavior
            await Promise.all([
                [
                    [
                        valExpected, valExpected, 0
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
                        k1: valExpected,
                        k2: valExpected,
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
                let valActual = await dbExecAsync({
                    bindList,
                    db,
                    responseType: "list",
                    sql
                });
                assertJsonEqual(
                    [
                        [
                            ["0"],
                            [0]
                        ],
                        [
                            ["c1", "c2", "c3", "c4"],
                            [valExpected, valExpected, 0, undefined],
                            [valExpected, valExpected, 0, undefined],
                            [valExpected, valExpected, 0, undefined]
                        ]
                    ],
                    valActual,
                    {
                        ii,
                        valActual,
                        valExpected,
                        valInput
                    }
                );
            }));
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
        }, "invalid or closed db");
        // test close handling-behavior
        dbCloseAsync({
            db
        });
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
    (btotext(?1), btotext(?2), btotext(?3)),
    (btobase64(?1), btobase64(?2), btobase64(?3)),
    (
        btobase64(uncompress(compress(?1))),
        btobase64(uncompress(compress(?2))),
        btobase64(uncompress(compress(?3)))
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
                        [null, null, null],
                        ["foob", "fooba", "foobar"],
                        ["Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"],
                        ["Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"]
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
        // test retry handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecAsync({
                modeRetry: 1
            });
        }, "invalid or closed db");
    });
    jstestIt((
        "test dbFileXxx handling-behavior"
    ), async function test_dbFileXxx() {
        let data;
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        dbFileExportAsync({
            modeNoop: true
        });
        assertErrorThrownAsync(function () {
            return dbFileImportAsync({
                db
            });
        }, "invalid filename undefined");
        assertErrorThrownAsync(function () {
            return dbFileExportAsync({
                db
            });
        }, "invalid filename undefined");
        await dbExecAsync({
            db,
            sql: "CREATE TABLE t01 AS SELECT 1 AS c01"
        });
        await dbFileExportAsync({
            db,
            filename: ".testDbFileXxx.sqlite"
        });
        db = await dbOpenAsync({
            filename: ":memory:"
        });
        await dbFileImportAsync({
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
        }, "invalid filename undefined");
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
        });
        // test assertJsonEqual error handling-behavior
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2);
        });
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2, "undefined");
        });
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2, {});
        });
        // test assertOrThrow error handling-behavior
        await assertErrorThrownAsync(function () {
            assertOrThrow(undefined, "undefined");
        });
        await assertErrorThrownAsync(function () {
            assertOrThrow(undefined, new Error());
        });
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
        "test sqlite-extension-base64 handling-behavior"
    ), async function test_sqlite_extension_base64() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await Promise.all([
            {
                sql: "SELECT btobase64(NULL) AS c01",
                valExpected: [
                    [
                        {
                            "c01": ""
                        }
                    ]
                ]
            },
            {
                sql: "SELECT btobase64(?) AS c01",
                valExpected: [
                    [
                        {
                            "c01": ""
                        }
                    ]
                ]
            },
            {
                bindList: [
                    new Uint8Array(8)
                ],
                sql: "SELECT btobase64(uncompress(compress(?))) AS c01",
                valExpected: [
                    [
                        {
                            "c01": "AAAAAAAAAAA="
                        }
                    ]
                ]
            }
        ].map(async function ({
            bindList,
            sql,
            valExpected
        }) {
            let valActual = await dbExecAsync({
                bindList,
                db,
                sql
            });
            assertJsonEqual(valActual, valExpected);
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
        "test sqlite-extension-jenks handling-behavior"
    ), async function test_sqlite_extension_jenks() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await Promise.all([
            [
                4,
                Array.from(new Array(2 ** 20)).map(function (ignore, ii) {
                    return ii;
                }),
                [
                    0, 262144, 524288, 786432,
                    262144, 262144, 262144, 262144
                ]
            ],
            [
                9999,
                [1.5],
                [
                    1.5,
                    1
                ]
            ],
            [
                2,
                [3, 2, 1],
                [
                    1, 2,
                    1, 2
                ]
            ],
            [
                3,
                [13.7, 0.1, 0.2, -0.4],
                [
                    -0.4, 0.1, 13.7,
                    1, 2, 1
                ]
            ],
            [
                4,
                [13.7, 0.1, 0.2, -0.4, 0.5],
                [
                    -0.4, 0.1, 0.5, 13.7,
                    1, 2, 1, 1
                ]
            ],
            [
                4,
                [13.7, 0.1, 0.2, -0.4, 0.5, 0.5],
                [
                    -0.4, 0.1, 0.5, 13.7,
                    1, 2, 2, 1
                ]
            ],
            [
                4,
                [13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5],
                [
                    -0.4, 0.1, 12.5, 13.7,
                    1, 4, 1, 1
                ]
            ],
            [
                4,
                [13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5, 0],
                [
                    -0.4, 0.2, 12.5, 13.7,
                    3, 3, 1, 1
                ]
            ],
            [
                4,
                [7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1],
                [
                    0.1, 1.1, 2.2, 7.1,
                    1, 3, 6, 1
                ]
            ],
            [
                14,
                [7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1],
                [
                    0.1, 1.1, 1.2, 1.6, 2.2, 2.5, 2.7, 2.8, 3, 3.1, 7.1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                ]
            ],
            [
                1,
                [7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1],
                [
                    0.1,
                    11
                ]
            ],
            [
                4,
                [
                    -9, -6, -3, -3, -1, 1, 2, 2,
                    2, 3, 3, 4, 5, 5, 6, 6,
                    7, 8, 8, 9, 10, 11, 11, 12,
                    12, 12, 12, 13, 15, 15, 15, 15,
                    16, 16, 17, 18, 18, 19, 19, 19,
                    21, 22, 23, 23, 23, 23, 23, 26,
                    28, 28, 30, 31, 31, 32, 32, 33,
                    34, 34, 34, 34, 35, 35, 35, 36,
                    36, 41, 41, 41, 42, 43, 43, 43,
                    44, 44, 45, 45, 46, 46, 47, 48,
                    51, 51, 52, 53, 53, 54, 54, 56,
                    56, 58, 60, 61, 62, 64, 67, 70,
                    73, 74, 78, 78, 78, 79, 83, 93
                ],
                [
                    -9, 18, 41, 64,
                    35, 30, 28, 11
                ]
            // 1-peak
            ],
            [
                2,
                [0, 0.1, 5, 5.1, 5.2, 9, 9.1],
                [
                    0, 5,
                    2, 5
                ]
            ],
            [
                3,
                [0, 0.1, 5, 5.1, 5.2, 9, 9.1],
                [
                    0, 5, 9,
                    2, 3, 2
                ]
            ],
            [
                3,
                [0, -0.1, -5, -5.1, -5.2, -9, -9.1],
                [
                    -9.1, -5.2, -0.1,
                    2, 3, 2
                ]
            ],
            [
                4,
                [0, 0.1, 5, 5.1, 5.2, 9, 9.1],
                [
                    0, 5, 5.1, 9,
                    2, 1, 2, 2
                ]
            // 2-peak
            ],
            [
                2,
                [0, 0.1, 2.5, 2.6, 7.5, 7.6, 9, 9.1],
                [
                    0, 7.5,
                    4, 4
                ]
            ],
            [
                3,
                [0, 0.1, 2.5, 2.6, 7.5, 7.6, 9, 9.1],
                [
                    0, 2.5, 7.5,
                    2, 2, 4
                ]
            ],
            [
                3,
                [0, -0.1, -2.5, -2.6, -7.5, -7.6, -9, -9.1],
                [
                    -9.1, -2.6, -0.1,
                    4, 2, 2
                ]
            ],
            [
                4,
                [0, 0.1, 2.5, 2.6, 7.5, 7.6, 9, 9.1],
                [
                    0, 2.5, 7.5, 9,
                    2, 2, 2, 2
                ]
            ]
        ].map(async function ([
            kk, input, expected
        ], iiInput) {
            await Promise.all([
                (`
SELECT JSONFROMFLOAT64ARRAY(JENKS_BLOB($kk, JSONTOFLOAT64ARRAY($input)));
                `),
                (`
SELECT JENKS_JSON($kk, $input);
                `),
                (`
SELECT JENKS_CONCAT($kk, value) FROM JSON_EACH($input);
                `)
            ].map(async function (sql, iiSql) {
                // performance-hack
                if (iiInput === 0 && iiSql > 0) {
                    return;
                }
                await Promise.all([
                    input, input.slice().reverse()
                ].map(async function (input) {
                    let result = await dbExecAndReturnLastJsonAsync({
                        bindList: {
                            input: JSON.stringify(input),
                            kk
                        },
                        db,
                        sql
                    });
                    if (Array.isArray(result[0])) {
                        result = result[0];
                    }
                    kk = result[0];
                    assertJsonEqual(kk, expected.length / 2);
                    assertJsonEqual(
                        result.slice(1).filter(function (ignore, ii) {
                            return ii % 2 === 0;
                        }),
                        expected.slice(0, kk)
                    );
                    assertJsonEqual(
                        result.slice(1).filter(function (ignore, ii) {
                            return ii % 2 === 1;
                        }),
                        expected.slice(kk, kk + kk)
                    );
                }));
            }));
        }));
    });
    jstestIt((
        "test sqlite-extension-jsonfromfloat64array handling-behavior"
    ), async function test_sqlite_extension_jsonfromfloat64array() {
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
        ].map(async function ([
            valInput, valExpected
        ], ii) {
            let valActual;
            try {
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            valInput
                        },
                        db,
                        sql: (`
SELECT JSONFROMFLOAT64ARRAY(JSONTOFLOAT64ARRAY($valInput)) AS result;
                        `)
                    })
                )[0][0].result;
            } catch (ignore) {
                assertOrThrow(valExpected === "error", JSON.stringify({
                    ii,
                    valActual,
                    valExpected,
                    valInput
                }, undefined, 4));
                return;
            }
            assertJsonEqual(valActual, valExpected, {
                ii,
                valActual,
                valExpected,
                valInput
            });
        }));
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
                "sign": -1
            },
            "'0'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": "0",
                "cot": null,
                "coth": null,
                "sign": 0
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
                "sign": 1
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
                "sign": -1
            },
            "-1": {
                "castrealornull": -1,
                "castrealorzero": -1,
                "casttextorempty": "-1",
                "copyblob": -1,
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1
            },
            "-1e999": {
                "sign": -1
            },
            "0": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": 0,
                "cot": null,
                "coth": null,
                "sign": 0
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
                "sign": 1
            },
            "0x8000000000000000": {
                "sign": -1
            },
            "0xffffffffffffffff": {
                "sign": -1
            },
            "1": {
                "castrealornull": 1,
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": 1,
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1
            },
            "1e999": {
                "castrealornull": null,
                "castrealorzero": 0,
                "sign": 1
            },
            "null": {
                "castrealornull": null,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null,
                "cot": null,
                "coth": null,
                "sign": null
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
                func, valExpected
            ]) {
                let sql = `SELECT ${func}(${arg}) AS val`;
                let valActual = noop(
                    await dbExecAsync({
                        db,
                        sql
                    })
                )[0][0].val;
                assertJsonEqual(valActual, valExpected, {
                    sql,
                    valActual,
                    valExpected
                });
            });
        });
    });
    jstestIt((
        "test sqlite-extension-matrix2d_concat handling-behavior"
    ), async function test_sqlite_extension_matrix2d_concat() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await Promise.all([
            [
                (`
SELECT matrix2d_concat();
                `),
                []
            ],
            [
                (`
SELECT matrix2d_concat() FROM (SELECT 1 UNION ALL SELECT 2);
                `),
                []
            ],
            [
                (`
SELECT
        matrix2d_concat(aa, aa)
    FROM (
        SELECT NULL AS aa
        UNION ALL SELECT '12.34'
        UNION ALL SELECT 'abcd'
        UNION ALL SELECT 12.34
        UNION ALL SELECT zeroblob(0)
        UNION ALL SELECT zeroblob(1)
        UNION ALL SELECT NULL
    );
                `),
                [
                    7, 2,
                    0, 0,
                    12.34, 12.34,
                    0, 0,
                    12.34, 12.34,
                    0, 0,
                    0, 0,
                    0, 0
                ]
            ]
        ].map(async function ([
            sql, valExpected
        ], ii) {
            let valActual = Array.from(new Float64Array(
                await dbExecAndReturnLastBlobAsync({
                    db,
                    sql
                })
            ));
            assertJsonEqual(valActual, valExpected, {
                ii,
                sql,
                valActual,
                valExpected
            });
        }));
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
                data, kk, valExpected
            ]) {
                return [
                    data.concat([
                        undefined, -Infinity, Infinity, NaN,
                        "8", 7, 6, "5", "4", 3, 2, "1",
                        undefined
                    ]),
                    kk,
                    valExpected
                ];
            })
        ].flat().map(async function ([
            data, kk, valExpected
        ]) {
            let avg = 0;
            let data2;
            let valExpectedMdn;
            let valExpectedStd = 0;
            data2 = data.map(function (elem) {
                return Number(elem);
            }).filter(function (elem) {
                return Number.isFinite(elem);
            }).sort();
            valExpectedMdn = (
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
                valExpectedStd += (elem - avg) ** 2;
            });
            valExpectedStd = (
                data2.length <= 0
                ? null
                // : data2.length === 1
                // ? 0
                : Number(Math.sqrt(
                    valExpectedStd / (data2.length - 1)
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
                        mdn: valExpectedMdn,
                        qnt: valExpected ?? null,
                        std: valExpectedStd
                    },
                    {
                        data,
                        kk,
                        valActual,
                        valExpected,
                        valExpectedMdn,
                        valExpectedStd
                    }
                );
            }));
        }));
    });
    jstestIt((
        "test sqlite-extension-vec_concat handling-behavior"
    ), async function test_sqlite_extension_vec_concat() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await Promise.all([
            [
                (`
SELECT vec_concat(NULL);
                `),
                [null]
            ],
            [
                (`
SELECT vec_concat(NULL) FROM (SELECT 1 UNION ALL SELECT 2);
                `),
                [null, null]
            ],
            [
                (`
SELECT
        vec_concat(aa)
    FROM (
        SELECT NULL AS aa
        UNION ALL SELECT '12.34'
        UNION ALL SELECT NULL
        UNION ALL SELECT 43.21
        UNION ALL SELECT NULL
        UNION ALL SELECT NULL
        UNION ALL SELECT NULL
    );
                `),
                [
                    12.34,
                    12.34,
                    12.34,
                    43.21,
                    43.21,
                    43.21,
                    43.21
                ]
            ]
        ].map(async function ([
            sql, valExpected
        ], ii) {
            let valActual = Array.from(new Float64Array(
                await dbExecAndReturnLastBlobAsync({
                    db,
                    sql
                })
            )).slice(VECTOR99_NOFFSET);
            assertJsonEqual(valActual, valExpected, {
                ii,
                sql,
                valActual,
                valExpected
            });
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
            valExpected,
            valExpected2
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
        win_ema1(value->>1, ${alpha}) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}) {
                return Number(val.toFixed(4));
            });
            assertJsonEqual(valActual, valExpected);
            // test win_ema2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        win_ema2(
            value->>1,
            IIF(id = 1, -1, value->>1),
            ${alpha}
        ) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}, ii, list) {
                val = JSON.parse(val).map(function (elem) {
                    return Number(elem.toFixed(4));
                });
                if (ii + (bb || 0) + 1 >= list.length) {
                    assertJsonEqual(val[1], valExpected2, valActual);
                } else {
                    assertJsonEqual(val[1], val[0], valActual);
                }
                return val[0];
            });
            assertJsonEqual(valActual, valExpected);
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
SELECT win_ema2(1, NULL) FROM (SELECT 1);
                        `)
                    });
                }, "invalid argument 'alpha'");
                // test win_ema1-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_ema1(1, 1) FROM __tmp1;
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
SELECT win_ema2(1, 2, 3) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_emax-aggregate-normal handling-behavior
            test_win_emax_aggregate({
                valExpected: [
                    0.0000, 0.4000, 1.0400, 1.8240,
                    2.6944, 3.2166, 4.3300, 5.3980,
                    6.4388, 7.4633, 8.4780, 9.0868
                ],
                valExpected2: 4.6868
            }),
            // test win_emax-aggregate-window handling-behavior
            test_win_emax_aggregate({
                aa: 1,
                bb: 3,
                valExpected: [
                    1.824, 2.824, 3.424, 4.584,
                    5.680, 6.608, 7.824, 8.824,
                    9.424, 9.424, 9.424, 9.424
                ],
                valExpected2: 5.024
            }),
            test_win_emax_aggregate({
                aa: 3,
                bb: 1,
                valExpected: [
                    0.400, 1.040, 1.824, 2.824,
                    3.424, 4.584, 5.680, 6.608,
                    7.824, 8.824, 9.424, 9.424
                ],
                valExpected2: 5.024
            }),
            test_win_emax_aggregate({
                aa: 4,
                bb: 0,
                valExpected: [
                    0.000, 0.400, 1.040, 1.824,
                    2.824, 3.424, 4.584, 5.680,
                    6.608, 7.824, 8.824, 9.424
                ],
                valExpected2: 5.024
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
            valExpected,
            valExpected2
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
        win_quantile1(value->>1, ${quantile}) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}) {
                return Number(val.toFixed(4));
            });
            assertJsonEqual(valActual, valExpected);
            // test win_quantile2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        win_quantile2(
            value->>1,
            IIF(id = 34, -1, value->>1),
            ${quantile}
        ) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}, ii) {
                val = JSON.parse(val).map(function (elem) {
                    return Number(elem.toFixed(4));
                });
                if (ii === 11) {
                    assertJsonEqual(val[1], valExpected2, valActual);
                } else {
                    assertJsonEqual(val[1], val[0], valActual);
                }
                return val[0];
            });
            assertJsonEqual(valActual, valExpected);
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
SELECT win_quantile2(1, NULL) FROM (SELECT 1);
                        `)
                    });
                }, "invalid argument 'quantile'");
                // test win_quantile1-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_quantile1(1, 1) FROM __tmp1;
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
SELECT win_quantile2(1, 2, 3) FROM __tmp1;
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
                valExpected: [
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000
                ],
                valExpected2: -1
            }),
            test_win_quantilex_aggregate({
                quantile: 0.25,
                valExpected: [
                    0.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 1.2500, 1.5000, 1.7500
                ],
                valExpected2: 0.7500
            }),
            test_win_quantilex_aggregate({
                quantile: 0.33333333,
                valExpected: [
                    0.0000, 0.3333, 0.6667, 1.0000,
                    1.3333, 0.6667, 1.0000, 1.3333,
                    1.6667, 2.0000, 2.3333, 2.6667
                ],
                valExpected2: 1.6667
            }),
            test_win_quantilex_aggregate({
                quantile: 0.5,
                valExpected: [
                    0.0000, 0.5000, 1.0000, 1.5000,
                    2.0000, 1.5000, 2.0000, 2.5000,
                    3.0000, 3.5000, 4.0000, 5.0000
                ],
                valExpected2: 3.5000
            }),
            test_win_quantilex_aggregate({
                quantile: 0.66666667,
                valExpected: [
                    0.0000, 0.6667, 1.3333, 2.0000,
                    2.6667, 2.3333, 3.0000, 3.6667,
                    4.6667, 6.0000, 6.0000, 6.6667
                ],
                valExpected2: 6.0000
            }),
            test_win_quantilex_aggregate({
                quantile: 0.75,
                valExpected: [
                    0.0000, 0.7500, 1.5000, 2.2500,
                    3.0000, 2.7500, 3.5000, 4.5000,
                    6.0000, 6.0000, 7.0000, 8.2500
                ],
                valExpected2: 6.5000
            }),
            test_win_quantilex_aggregate({
                quantile: 1,
                valExpected: [
                    0.0000, 1.0000, 2.0000, 3.0000,
                    4.0000, 4.0000, 6.0000, 6.0000,
                    8.0000, 9.0000, 10.0000, 11.0000
                ],
                valExpected2: 10.0000
            }),
            // test win_quantilex-aggregate-window handling-behavior
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0,
                valExpected: [
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000
                ],
                valExpected2: -1
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.25,
                valExpected: [
                    0.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 0.2500, 0.5000, 0.7500,
                    1.7500, 2.7500, 3.7500, 5.5000
                ],
                valExpected2: 3
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.33333333,
                valExpected: [
                    0.0000, 0.3333, 0.6667, 1.0000,
                    1.3333, 0.6667, 1.0000, 1.3333,
                    2.3333, 3.3333, 4.6667, 6.0000
                ],
                valExpected2: 4.6667
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.5000,
                valExpected: [
                    0.0000, 0.5000, 1.0000, 1.5000,
                    2.0000, 1.5000, 2.0000, 2.5000,
                    3.5000, 5.0000, 6.0000, 7.0000
                ],
                valExpected2: 6.0000
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.66666667,
                valExpected: [
                    0.0000, 0.6667, 1.3333, 2.0000,
                    2.6667, 2.3333, 3.0000, 3.6667,
                    5.3333, 6.0000, 7.3333, 8.6667
                ],
                valExpected2: 7.3333
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.75,
                valExpected: [
                    0.0000, 0.7500, 1.5000, 2.2500,
                    3.0000, 2.7500, 3.5000, 4.5000,
                    6.0000, 6.5000, 8.2500, 9.2500
                ],
                valExpected2: 8.2500
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 1.0000,
                valExpected: [
                    0.0000, 1.0000, 2.0000, 3.0000,
                    4.0000, 4.0000, 6.0000, 6.0000,
                    8.0000, 9.0000, 10.0000, 11.0000
                ],
                valExpected2: 10.0000
            })
        ]);
    });
    jstestIt((
        "test sqlite-extension-win_slr2 handling-behavior"
    ), async function test_sqlite_extension_win_slr2() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valActual;
        let valExpected;
        let valIn;
        async function test_win_slr2_aggregate({
            aa,
            bb,
            valExpected,
            valExpected2
        }) {
            let sqlBetween = "";
            let valActual; //jslint-ignore-line
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_slr2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        --
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 0), 8) AS nnn1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 1), 8) AS mxx1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 2), 8) AS myy1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 3), 8) AS exx1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 4), 8) AS eyy1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 5), 8) AS crr1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 6), 8) AS cbb1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 7), 8) AS caa1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 8), 8) AS cyy1,
        ROUND(slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 9), 8) AS cee1,
        --
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 0), 8) AS nnn2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 1), 8) AS mxx2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 2), 8) AS myy2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 3), 8) AS exx2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 4), 8) AS eyy2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 5), 8) AS crr2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 6), 8) AS cbb2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 7), 8) AS caa2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 8), 8) AS cyy2,
        ROUND(slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 9), 8) AS cee2
    FROM (
        SELECT
            id,
            win_slr2(
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, IIF(id = 28, -1, value->>1)
            ) OVER (
                ORDER BY NULL ASC
                ${sqlBetween}
            ) AS slr
        FROM JSON_EAcH($valIn)
    );
                `)
            });
            valActual = valActual[0].map(function ({
                caa1,
                caa2,
                cbb1,
                cbb2,
                cee1,
                cee2,
                crr1,
                crr2,
                cyy1,
                cyy2,
                exx1,
                exx2,
                eyy1,
                eyy2,
                mxx1,
                mxx2,
                myy1,
                myy2,
                nnn1,
                nnn2
            }, ii, list) {
                let obj1;
                let obj2;
                obj1 = {
                    caa: caa1,
                    cbb: cbb1,
                    cee: cee1,
                    crr: crr1,
                    cyy: cyy1,
                    exx: exx1,
                    eyy: eyy1,
                    mxx: mxx1,
                    myy: myy1,
                    nnn: nnn1
                };
                obj2 = {
                    caa: caa2,
                    cbb: cbb2,
                    cee: cee2,
                    crr: crr2,
                    cyy: cyy2,
                    exx: exx2,
                    eyy: eyy2,
                    mxx: mxx2,
                    myy: myy2,
                    nnn: nnn2
                };
                if (ii + (bb || 0) + 1 >= list.length) {
                    assertJsonEqual(obj2, valExpected2, valActual);
                } else {
                    assertJsonEqual(obj2, obj1, valActual);
                }
                return obj1;
            });
            assertJsonEqual(valActual, valExpected);
        }
        noop(test_win_slr2_aggregate);
        valExpected = [
            {
                "caa": null,
                "cbb": null,
                "cee": null,
                "crr": null,
                "cyy": null,
                "exx": null,
                "eyy": null,
                "mxx": 2,
                "myy": 0,
                "nnn": 1
            },
            {
                "caa": null,
                "cbb": null,
                "cee": null,
                "crr": null,
                "cyy": null,
                "exx": 0,
                "eyy": 0.70710678,
                "mxx": 2,
                "myy": 0.5,
                "nnn": 2
            },
            {
                "caa": -4.5,
                "cbb": 2.5,
                "cee": 0.70710678,
                "crr": 0.94491118,
                "cyy": 3,
                "exx": 0.57735027,
                "eyy": 1.52752523,
                "mxx": 2.33333333,
                "myy": 1.33333333,
                "nnn": 3
            },
            {
                "caa": -3,
                "cbb": 1.81818182,
                "cee": 0.67419986,
                "crr": 0.95346259,
                "cyy": 4.27272727,
                "exx": 0.95742711,
                "eyy": 1.82574186,
                "mxx": 2.75,
                "myy": 2,
                "nnn": 4
            },
            {
                "caa": -2.29411765,
                "cbb": 1.52941176,
                "cee": 0.65678958,
                "crr": 0.96164474,
                "cyy": 5.35294118,
                "exx": 1.30384048,
                "eyy": 2.07364414,
                "mxx": 3.2,
                "myy": 2.6,
                "nnn": 5
            },
            {
                "caa": -2.54385965,
                "cbb": 1.63157895,
                "cee": 0.62126074,
                "crr": 0.97080629,
                "cyy": 5.61403509,
                "exx": 1.37840488,
                "eyy": 2.31660671,
                "mxx": 3.5,
                "myy": 3.16666667,
                "nnn": 6
            },
            {
                "caa": -2.65,
                "cbb": 1.675,
                "cee": 0.57445626,
                "crr": 0.9752227,
                "cyy": 5.725,
                "exx": 1.38013112,
                "eyy": 2.37045304,
                "mxx": 3.71428571,
                "myy": 3.57142857,
                "nnn": 7
            },
            {
                "caa": -2.5,
                "cbb": 1.625,
                "cee": 0.54006172,
                "crr": 0.97991187,
                "cyy": 7.25,
                "exx": 1.51185789,
                "eyy": 2.50713268,
                "mxx": 4,
                "myy": 4,
                "nnn": 8
            },
            {
                "caa": 0.75,
                "cbb": 0.85,
                "cee": 1.08781126,
                "crr": 0.89597867,
                "cyy": 9.25,
                "exx": 2.39045722,
                "eyy": 2.26778684,
                "mxx": 5,
                "myy": 5,
                "nnn": 8
            },
            {
                "caa": 2.75,
                "cbb": 0.55,
                "cee": 0.99163165,
                "crr": 0.81989159,
                "cyy": 3.85,
                "exx": 2.39045722,
                "eyy": 1.60356745,
                "mxx": 5,
                "myy": 5.5,
                "nnn": 8
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
                // test win_slr2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_slr2(1) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                // test win_slr2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_slr2(1, 1) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_slr2-aggregate-normal handling-behavior
            (async function () {
                valActual = await dbExecAndReturnLastJsonAsync({
                    bindList: {
                        valIn: JSON.stringify(valIn)
                    },
                    db,
                    sql: (`
SELECT win_slr2(value->>0, value->>1) AS slr FROM JSON_EACH($valIn);
                    `)
                });
                valActual = valActual.map(function (xx) {
                    return Number(xx.toFixed(8));
                });
                assertJsonEqual(
                    valActual,
                    [
                        10, // nnn
                        4.4, // mxx
                        4.5, // myy
                        2.45854519, // exx
                        2.54950976, // eyy
                        0.81541829, // crr
                        0.84558824, // cbb
                        0.77941176, // caa
                        2.47058824, // cyy
                        1.56536502 // cee
                    ]
                );
            }()),
            // test win_slr2-aggregate-window handling-behavior
            test_win_slr2_aggregate({
                aa: 8,
                bb: 0,
                valExpected,
                valExpected2: {
                    caa: -0.25,
                    cbb: 1,
                    cee: 1.60727513,
                    crr: 0.84895272,
                    cyy: 1.75,
                    exx: 2.39045722,
                    eyy: 2.81577191,
                    mxx: 5,
                    myy: 4.75,
                    nnn: 8
                }
            })
        ]);
        valActual = {
            "caa": -0.820256776034237,
            "cbb": 0.146219686162625,
            "cee": 2.74202903932406,
            "crr": 0.865664999629448,
            "cyy": 6.63694721825963,
            "exx": 29.003448070876,
            "eyy": 4.89897948556636,
            "mxx": 74,
            "myy": 10,
            "nnn": 6
        };
        valExpected = noop(
            await dbExecAsync({
                db,
                sql: (`
SELECT
        __slr->>0 AS nnn,
        __slr->>1 AS mxx,
        __slr->>2 AS myy,
        __slr->>3 AS exx,
        __slr->>4 AS eyy,
        __slr->>5 AS crr,
        __slr->>6 AS cbb,
        __slr->>7 AS caa,
        __slr->>8 AS cyy,
        __slr->>9 AS cee
    FROM (
        SELECT
            win_slr2(xx, yy) AS __slr
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
        assertJsonEqual(valActual, valExpected);
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
