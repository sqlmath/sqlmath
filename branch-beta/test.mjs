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
    debugInline,
    jsbatonValueString,
    noop,
    sqlmathWebworkerInit,
    version
} from "./sqlmath.mjs";
let {
    assertErrorThrownAsync,
    jstestDescribe,
    jstestIt
} = jslint;
noop(debugInline);

jstestDescribe((
    "test apidoc handling-behavior"
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
    "test assertXxx handling-behavior"
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
    "test cCallAsync handling-behavior"
), function test_ccall() {
    jstestIt((
        "test cCallAsync handling-behavior"
    ), function () {
        // test datatype handling-behavior
        [
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
        ].forEach(async function ([
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
        });
    });
});

jstestDescribe((
    "test db-bind handling-behavior"
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
        [
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
        ].forEach(function ([
            valInput, valExpected
        ], ii) {
            // test dbExecAndReturnLastBlobAsync's bind handling-behavior
            [
                valInput
            ].forEach(async function (valInput) {
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
            });
            // test dbExecAsync's responseType handling-behavior
            [
                "arraybuffer",
                "list",
                undefined
            ].forEach(async function (responseType) {
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
            });
            // test dbExecAsync's bind handling-behavior
            [
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
                ], [
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
            ].forEach(async function ([
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
                            [
                                "0"
                            ], [
                                0
                            ]
                        ], [
                            [
                                "c1", "c2", "c3", "c4"
                            ], [
                                valExpected, valExpected, 0, undefined
                            ], [
                                valExpected, valExpected, 0, undefined
                            ], [
                                valExpected, valExpected, 0, undefined
                            ]
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
            });
        });
    });
});

jstestDescribe((
    "test dbXxxAsync handling-behavior"
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
    "test misc handling-behavior"
), function test_misc() {
    jstestIt((
        "test misc handling-behavior"
    ), async function () {
        // test debugInline handling-behavior
        noop(debugInline);
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
    "test sqlite handling-behavior"
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
        [
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
        ].forEach(function ([
            errno, errmsg
        ]) {
            assertErrorThrownAsync(function () {
                return dbExecAsync({
                    db,
                    sql: `SELECT throwerror(${errno})`
                });
            }, errmsg);
        });
    });
    jstestIt((
        "test sqlite-extension-base64 handling-behavior"
    ), async function test_sqliteExtensionBase64() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
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
        ].forEach(async function ({
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
        });
    });
    jstestIt((
        "test sqlite-extension-jenks handling-behavior"
    ), async function test_sqliteExtensionJenks() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
            [
                [
                    1.5
                ],
                9999,
                [
                    1.5,
                    1
                ]
            ], [
                [
                    3, 2, 1
                ],
                2,
                [
                    1, 2,
                    1, 2
                ]
            ], [
                [
                    13.7, 0.1, 0.2, -0.4
                ],
                3,
                [
                    -0.4, 0.1, 13.7,
                    1, 2, 1
                ]
            ], [
                [
                    13.7, 0.1, 0.2, -0.4, 0.5
                ],
                4,
                [
                    -0.4, 0.1, 0.5, 13.7,
                    1, 2, 1, 1
                ]
            ], [
                [
                    13.7, 0.1, 0.2, -0.4, 0.5, 0.5
                ],
                4,
                [
                    -0.4, 0.1, 0.5, 13.7,
                    1, 2, 2, 1
                ]
            ], [
                [
                    13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5
                ],
                4,
                [
                    -0.4, 0.1, 12.5, 13.7,
                    1, 4, 1, 1
                ]
            ], [
                [
                    13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5, 0
                ],
                4,
                [
                    -0.4, 0.2, 12.5, 13.7,
                    3, 3, 1, 1
                ]
            ], [
                [
                    7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1
                ],
                4,
                [
                    0.1, 1.1, 2.2, 7.1,
                    1, 3, 6, 1
                ]
            ], [
                [
                    7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1
                ],
                14,
                [
                    0.1, 1.1, 1.2, 1.6, 2.2, 2.5, 2.7, 2.8, 3, 3.1, 7.1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                ]
            ], [
                [
                    7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1
                ],
                1,
                [
                    0.1,
                    11
                ]
            ], [
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
                4,
                [
                    -9, 18, 41, 64,
                    35, 30, 28, 11
                ]
                // [-9, 17, 36, 58, 93]
            // 1-peak
            ], [
                [
                    0, 0.1, 5, 5.1, 5.2, 9, 9.1
                ],
                2,
                [
                    0, 5,
                    2, 5
                ]
            ], [
                [
                    0, 0.1, 5, 5.1, 5.2, 9, 9.1
                ],
                3,
                [
                    0, 5, 9,
                    2, 3, 2
                ]
            ], [
                [
                    0, -0.1, -5, -5.1, -5.2, -9, -9.1
                ],
                3,
                [
                    -9.1, -5.2, -0.1,
                    2, 3, 2
                ]
            ], [
                [
                    0, 0.1, 5, 5.1, 5.2, 9, 9.1
                ],
                4,
                [
                    0, 5, 5.1, 9,
                    2, 1, 2, 2
                ]
            // 2-peak
            ], [
                [
                    0, 0.1, 2.5, 2.6, 7.5, 7.6, 9, 9.1
                ],
                2,
                [
                    0, 7.5,
                    4, 4
                ]
            ], [
                [
                    0, 0.1, 2.5, 2.6, 7.5, 7.6, 9, 9.1
                ],
                3,
                [
                    0, 2.5, 7.5,
                    2, 2, 4
                ]
            ], [
                [
                    0, -0.1, -2.5, -2.6, -7.5, -7.6, -9, -9.1
                ],
                3,
                [
                    -9.1, -2.6, -0.1,
                    4, 2, 2
                ]
            ], [
                [
                    0, 0.1, 2.5, 2.6, 7.5, 7.6, 9, 9.1
                ],
                4,
                [
                    0, 2.5, 7.5, 9,
                    2, 2, 2, 2
                ]
            ], [
                new Float64Array(
                    Array.from(new Array(2 ** 20)).map(function (ignore, ii) {
                        return ii;
                    })
                ),
                4,
                [
                    0, 262144, 524288, 786432,
                    262144, 262144, 262144, 262144
                ]
            ]
        ].forEach(function ([
            input, kk, expected
        ]) {
            [
                input, input.slice().reverse()
            ].forEach(async function (input) {
                let result = Array.from(new Float64Array(
                    await dbExecAndReturnLastBlobAsync({
                        bindList: {
                            input: new Float64Array(input),
                            kk
                        },
                        db,
                        sql: "SELECT jenks($kk, $input) AS result;"
                    })
                ));
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
            });
        });
    });
    jstestIt((
        "test sqlite-extension-kthpercentile handling-behavior"
    ), async function test_sqliteExtensionKthpercentile() {
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
            data, kk, valExpected
        ]) {
            let valActual;
            data = data.concat([
                undefined, undefined, 8, 7, 6, 5, 4, 3, 2, 1, undefined
            ]);
            valActual = noop(
                await dbExecAsync({
                    bindList: {
                        tmp1: JSON.stringify(data)
                    },
                    db,
                    sql: (`
SELECT kthpercentile(value, ${kk}) AS val FROM json_each($tmp1);
-- test null-case handling-behavior
SELECT kthpercentile(value, ${kk}) AS val FROM json_each($tmp1) WHERE 0;
                    `)
                })
            )[0][0].val;
            assertJsonEqual(valActual, valExpected, {
                data,
                kk
            });
        });
    });
    jstestIt((
        "test sqlite-extension-math handling-behavior"
    ), async function test_sqliteExtensionMath() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test sqlmath-defined-func handling-behavior
        Object.entries({
            "''": {
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": ""
            },
            "'-0.5'": {
                "castrealorzero": -0.5,
                "casttextorempty": "-0.5",
                "copyblob": "-0.5"
            },
            "'-1'": {
                "castrealorzero": -1,
                "casttextorempty": "-1",
                "copyblob": "-1",
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1
            },
            "'0'": {
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": "0",
                "cot": null,
                "coth": null,
                "sign": 0
            },
            "'0.5'": {
                "castrealorzero": 0.5,
                "casttextorempty": "0.5",
                "copyblob": "0.5"
            },
            "'1'": {
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": "1",
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1
            },
            "'aa'": {
                "castrealorzero": 0,
                "casttextorempty": "aa",
                "copyblob": "aa"
            },
            "'hello'": {
                "castrealorzero": 0,
                "casttextorempty": "hello",
                "copyblob": "hello"
            },
            "-0.5": {
                "castrealorzero": -0.5,
                "casttextorempty": "-0.5",
                "copyblob": -0.5
            },
            "-0x7fffffffffffffff": {
                "sign": -1
            },
            "-1": {
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
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": 0,
                "cot": null,
                "coth": null,
                "sign": 0
            },
            "0.5": {
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
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": 1,
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1
            },
            "1e999": {
                "sign": 1
            },
            "null": {
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
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null
            },
            "zeroblob(1)": {
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
    ), async function test_sqliteExtensionMatrix2dConcat() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
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
        ].forEach(async function ([
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
        });
    });
});

jstestDescribe((
    "test sqlmathWebworkerInit handling-behavior"
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
