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
import sqlmath from "./sqlmath.mjs";
let {
    assertErrorThrownAsync,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    cCall,
    dbCloseAsync,
    dbExecAsync,
    dbExecWithRetryAsync,
    dbGetLastBlobAsync,
    dbMemoryLoadAsync,
    dbMemorySaveAsync,
    dbOpenAsync,
    dbTableInsertAsync,
    debugInline,
    jstestDescribe,
    jstestIt,
    noop
} = sqlmath;
noop(debugInline);

jstestDescribe((
    "test assertXxx handling-behavior"
), function testAssertXxx() {
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
    "test cCall handling-behavior"
), function testCcall() {
    jstestIt((
        "test cCall handling-behavior"
    ), function () {
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
            valInput, valExpected
        ]) {
            let valActual;
            valActual = String(
                await cCall("noopAsync", [
                    valInput
                ])
            )[0][0];
            assertJsonEqual(valActual, valExpected, {
                valActual,
                valExpected,
                valInput
            });
            valActual = String(cCall("noopSync", [
                valInput
            ]))[0][0];
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
), function testDbBind() {
    jstestIt((
        "test db-bind-value handling-behavior"
    ), async function testDbBindValue() {
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
            // 1. bigint
            [-0n, -0],
            [-0x7fffffffffffffffn, "-9223372036854775807"],
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
            [-1 / 0, null],
            [-1e-999, 0],
            [-1e999, null],
            [-2, -2],
            [-Infinity, null],
            [-NaN, 0],
            [0, 0],
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
            // test dbGetLastBlobAsync's bind handling-behavior
            [
                valInput
            ].forEach(async function (valInput) {
                let bufActual = new TextDecoder().decode(
                    await testDbGetLastBlobAsync(valInput)
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
                let bufActual = noop(
                    await dbExecAsync({
                        bindList: [
                            valInput
                        ],
                        db,
                        responseType,
                        sql: "SELECT ? AS val"
                    })
                );
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
                let tblActual = noop(
                    await dbExecAsync({
                        bindList,
                        db,
                        responseType: "list",
                        sql
                    })
                );
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
                    tblActual,
                    {
                        ii,
                        tblActual,
                        valExpected,
                        valInput
                    }
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
                            valInput, valInput
                        ]
                    ]
                }, {
                    // test list-of-dict handling-behavior
                    rowList: [
                        {
                            "c1": valInput,
                            "c2": valInput,
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
                            valInput, valInput
                        ]
                    ]
                }, {
                    // test colList and list-of-dict handling-behavior
                    colList: [
                        "c1", "c2", "c3"
                    ],
                    rowList: [
                        {
                            "c1": valInput,
                            "c2": valInput,
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
                            valInput, undefined, valInput
                        ]
                    ]
                }
            ].forEach(async function ({
                colList,
                colListPriority,
                rowList
            }, jj) {
                let tblActual;
                try {
                    tblActual = noop(
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
                } catch (err) {
                    assertOrThrow((
                        valInput
                        && valInput.constructor === SharedArrayBuffer
                        && err.message.startsWith("sqlite - invalid datatype")
                    ), err);
                    return;
                }
                assertJsonEqual([
                    [
                        "c1", "c2", "c3"
                    ], [
                        valExpected, valExpected, undefined
                    ]
                ], tblActual, {
                    ii,
                    jj,
                    tblActual,
                    valExpected,
                    valInput
                });
            });
        });
    });
});

jstestDescribe((
    "test dbXxxAsync handling-behavior"
), function testDbXxxAsync() {
    jstestIt((
        "test dbCloseAsync handling-behavior"
    ), async function testDbCloseAsync() {
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
    ), async function testDbExecAsync() {
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
    });
    jstestIt((
        "test dbExecWithRetryAsync handling-behavior"
    ), function testDbExecWithRetryAsync() {
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecWithRetryAsync({});
        }, "invalid or closed db");
    });
    jstestIt((
        "test dbMemoryXxx handling-behavior"
    ), async function testDbMemoryXxx() {
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
    });
    jstestIt((
        "test dbOpenAsync handling-behavior"
    ), function testDbOpenAsync() {
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbOpenAsync({});
        }, "invalid filename undefined");
    });
    jstestIt((
        "test dbTableInsertAsync handling-behavior"
    ), async function testDbTableInsertAsync() {
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
            valInput, valExpected
        ], ii) {
            let valActual = noop(
                await dbExecAsync({
                    db,
                    responseType: "list",
                    sql: `SELECT * FROM temp.csv_${ii}`,
                    tmpCsv: valInput,
                    tmpTableName: "csv_" + ii
                })
            )[0];
            assertJsonEqual(
                valActual,
                valExpected,
                JSON.stringify({
                    ii,
                    valActual,
                    valExpected,
                    valInput
                }, undefined, 4)
            );
        });
    });
});

jstestDescribe((
    "test sqlite handling-behavior"
), function testSqlite() {
    jstestIt((
        "test sqlite-error handling-behavior"
    ), async function testSqliteError() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
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
    ), async function testSqliteExtensionBase64() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        [
            {
                sql: "SELECT tobase64(NULL) AS c01",
                valExpected: [
                    [
                        {
                            "c01": ""
                        }
                    ]
                ]
            },
            {
                sql: "SELECT tobase64(?) AS c01",
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
                sql: "SELECT tobase64(uncompress(compress(?))) AS c01",
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
    ), async function testSqliteExtensionJenks() {
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
                let output = new Float64Array(new SharedArrayBuffer(
                    (2 + 2 * kk) * 8
                ));
                input = new Float64Array(input);
                await dbExecAsync({
                    bindList: {
                        input: new Float64Array(input),
                        kk,
                        nn: input.length,
                        output: output.buffer
                    },
                    db,
                    sql: "SELECT jenks($input, $nn, $kk, $output);"
                });
                kk = output[1];
                assertJsonEqual(
                    Array.from(output.slice(2, 2 + kk)),
                    expected.slice(0, kk)
                );
                assertJsonEqual(
                    Array.from(output.slice(2 + kk, 2 + kk + kk)),
                    expected.slice(kk, kk + kk)
                );
            });
        });
    });
    jstestIt((
        "test sqlite-extension-kthpercentile handling-behavior"
    ), async function testSqliteExtensionKthpercentile() {
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
        ], ii) {
            let valActual;
            data = data.concat([
                undefined, undefined, 8, 7, 6, 5, 4, 3, 2, 1, undefined
            ]);
            valActual = noop(
                await dbExecAsync({
                    db,
                    sql: (`
SELECT kthpercentile(val, ${kk}) AS val FROM __tmp${ii};
-- test null-case handling-behavior
SELECT kthpercentile(val, ${kk}) AS val FROM __tmp${ii} WHERE 0;
                    `),
                    tmpRowList: data.map(function (val) {
                        return {
                            val
                        };
                    }),
                    tmpTableName: `__tmp${ii}`
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
    ), async function testSqliteExtensionMath() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test sqlmath-defined-func handling-behavior
        [
            ["CASTREALORZERO('')", 0],
            ["CASTREALORZERO('-0.5')", -0.5],
            ["CASTREALORZERO('0.5')", 0.5],
            ["CASTREALORZERO('hello')", 0],
            ["CASTREALORZERO(-0.5)", -0.5],
            ["CASTREALORZERO(0.5)", 0.5],
            ["CASTREALORZERO(NULL)", 0],
            ["CASTTEXTOREMPTY('')", ""],
            ["CASTTEXTOREMPTY('-0.5')", "-0.5"],
            ["CASTTEXTOREMPTY('0.5')", "0.5"],
            ["CASTTEXTOREMPTY('hello')", "hello"],
            ["CASTTEXTOREMPTY(-0.5)", "-0.5"],
            ["CASTTEXTOREMPTY(0.5)", "0.5"],
            ["CASTTEXTOREMPTY(NULL)", ""],
            ["COT('-1')", -0.642092615934331],
            ["COT('0')", null],
            ["COT('1')", 0.642092615934331],
            ["COT(-1)", -0.642092615934331],
            ["COT(0)", null],
            ["COT(1)", 0.642092615934331],
            ["COT(NULL)", null],
            ["COTH('-1')", -1.31303528549933],
            ["COTH('0')", null],
            ["COTH('1')", 1.31303528549933],
            ["COTH(-1)", -1.31303528549933],
            ["COTH(0)", null],
            ["COTH(1)", 1.31303528549933],
            ["COTH(NULL)", null],
            ["ROUNDORZERO(0.5, 0.5)", 1],
            ["ROUNDORZERO(0.5, 1)", 0.5],
            ["ROUNDORZERO(0.5, NULL)", 1],
            ["ROUNDORZERO(NULL, 0)", 0],
            ["ROUNDORZERO(NULL, 0.5)", 0],
            ["ROUNDORZERO(NULL, NULL)", 0],
            ["SIGN('-1')", -1],
            ["SIGN('0')", 0],
            ["SIGN('1')", 1],
            ["SIGN(-0x7fffffffffffffff)", -1],
            ["SIGN(-1)", -1],
            ["SIGN(-1e999)", -1],
            ["SIGN(0)", 0],
            ["SIGN(0x7fffffffffffffff)", 1],
            ["SIGN(0x8000000000000000)", -1],
            ["SIGN(0xffffffffffffffff)", -1],
            ["SIGN(1)", 1],
            ["SIGN(1e999)", 1],
            ["SIGN(NULL)", null],
            // sentinel
            ["NULL", null]
        ].forEach(async function ([
            sql, valExpected
        ], ii) {
            let valActual = noop(
                await dbExecAsync({
                    db,
                    responseType: "dict",
                    sql: `SELECT ${sql} AS val`
                })
            )[0][0].val;
            assertJsonEqual(valActual, valExpected, {
                ii,
                sql,
                valActual,
                valExpected
            });
        });
    });
    jstestIt((
        "test sqlite-extension-matrix2d handling-behavior"
    ), async function testSqliteExtensionMatrix2d() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        debugInline(
            await dbExecAsync({
            //!! await dbGetLastBlobAsync({
                db,
                sql: (`
select * FROM blob_each(blob_create(16));
--!! SELECT
        --!! *
    --!! FROM (
        --!! SELECT
            --!! matrix2d_concat(1, 2) AS foo
        --!! FROM (
            --!! SELECT 1
        --!! )
    --!! )
--!! ;
                `)
            })
        );
    });
});
