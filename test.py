# Copyright (c) 2021 Kai Zhu
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


"""
test.py.

python test.py --verbose
"""

import json
import unittest

import sqlmath
from sqlmath import (
    INFINITY,
    NAN,
    asserterrorthrown,
    assertjsonequal,
    assertorthrow,
    debuginline,
    jsbaton_get_int64,
    jsbaton_get_string,
    noop,
)

noop(debuginline)


class TestCaseSqlmath(unittest.TestCase):
    """Custom TestCase."""

    def test_db_bind(self): # noqa: C901
        """Test db_bind handling-behavior."""
        db = sqlmath.db_open(":memory:")
        # test bigint-error handling-behavior
        for val_input in [-(2 ** 63 + 1), 2 ** 63]:
            asserterrorthrown(
                lambda val_input=val_input:
                    sqlmath.db_exec_and_return_lastblob(
                        bind_list=[val_input],
                        db=db,
                        sql="SELECT 1, 2, 3; SELECT 1, 2, ?;",
                    ),
                "outside signed-64-bit inclusive-range",
            )
        # test datatype handling-behavior
        for ii, (val_input, val_expected) in enumerate([
            #  1. 0.NoneType
            [None, None],
            #  2. 0.bool
            [False, 0],
            [True, 1],
            #  3. 0.bytearray
            [bytearray(b""), None],
            [bytearray(bytes("\u0000", "utf-8")), None],
            [bytearray(bytes("\u0000\U0001f600}\u0000", "utf-8")), None],
            #  4. 0.bytes
            [b"", None],
            [bytes("\u0000", "utf-8"), None],
            [bytes("\u0000\U0001f600}\u0000", "utf-8"), None],
            #  5. 0.complex
            #  6. 0.dict
            [{}, "{}"],
            #  7. 0.float
            [-0.5, -0.5],
            [-1e-999, 0],
            [-1e999, None],
            [-INFINITY, None],
            # !! [-NAN, 0],
            [0.5, 0.5],
            [0x7fffffffffffffff, "9223372036854775807"],
            [1e-999, 0],
            [1e999, None],
            [INFINITY, None],
            # !! [NAN, 0],
            #  8. 0.frozenset
            #  9. 0.int
            [-0, -0],
            [-0, 0],
            [-0x8000000000000000, "-9223372036854775808"],
            [-1, -1],
            [-2, -2],
            [0, 0],
            [1, 1],
            [2, 2],
            # 10. 0.list
            [[0], "[0]"],
            [[None], "[null]"],
            [[], "[]"],
            # 11. 0.memoryview
            [memoryview(b""), None],
            [memoryview(bytes("\u0000", "utf-8")), None],
            [memoryview(bytes("\u0000\U0001f600}\u0000", "utf-8")), None],
            # 12. 0.range
            # 13. 0.set
            # 14. 0.str
            ["", ""],
            ["0", "0"],
            ["1", "1"],
            ["2", "2"],
            ["\u0000", "\u0000"],
            ["\u0000\U0001f600\u0000", "\u0000\U0001f600\u0000"],
            ["a" * 9999, "a" * 9999],
            # 15. 0.tuple
            [(), "[]"],
            [(0,), "[0]"],
            [(None,), "[null]"],
            # 16. 0.misc
            [noop, Exception],
            # !! [new Date(0), "1970-01-01T00:00:00.000Z"],
            # !! [new RegExp(), "{}"],
        ]):
            # test db_exec_and_return_last_blob's bind handling-behavior
            def test_db_exec_and_return_last_blob(ii, val_input, val_expected):
                if val_expected is Exception:
                    asserterrorthrown(
                        lambda:
                            sqlmath.db_exec_and_return_lastblob(
                                bind_list=[val_input],
                                db=db,
                                sql="SELECT 1, 2, 3; SELECT 1, 2, ?;",
                            ),
                        "not JSON serializable",
                    )
                    return
                buf_actual = bytes(
                    sqlmath.db_exec_and_return_lastblob(
                        bind_list=[val_input],
                        db=db,
                        sql="SELECT 1, 2, 3; SELECT 1, 2, ?;",
                    ),
                ).decode()
                buf_expected = str(val_expected)
                #  1. 0.NoneType
                if val_input is None:
                    buf_expected = ""
                #  3. 0.bytearray
                #  4. 0.bytes
                if isinstance(val_input, (bytearray, bytes)):
                    buf_expected = val_input.decode()
                if isinstance(val_input, memoryview):
                    buf_expected = val_input.obj.decode()
                    # !! debuginline(buf_actual, buf_expected, {
                        # !! "buf_actual": buf_actual,
                        # !! "buf_expected": buf_expected,
                        # !! "ii": ii,
                        # !! "val_expected": val_expected,
                        # !! "val_input": val_input,
                    # !! })
                # !! switch (typeof(val_input)) {
                # !! case "function":
                # !! case "symbol":
                # !! case "None":
                    # !! buf_expected = ""
                    # !! break
                #  7. 0.float
                if isinstance(val_input, float):
                    if val_input == -INFINITY:
                        buf_expected = "-Inf"
                    if val_input == INFINITY:
                        buf_expected = "Inf"
                assertjsonequal(buf_actual, buf_expected, {
                    "buf_actual": buf_actual,
                    "buf_expected": buf_expected,
                    "ii": ii,
                    "val_expected": val_expected,
                    "val_input": str(val_input),
                })
            test_db_exec_and_return_last_blob(ii, val_input, val_expected)

            # test db_exec's response_type handling-behavior

            def test_db_exec_response_type(ii, val_input, val_expected):
                for response_type in [
                    "arraybuffer",
                    "list",
                    None,
                ]:
                    if val_expected is Exception:
                        asserterrorthrown(
                            lambda response_type=response_type:
                                sqlmath.db_exec(
                                    bind_list=[val_input],
                                    db=db,
                                    response_type=response_type,
                                    sql="SELECT ? AS val",
                                ),
                            "not JSON serializable",
                        )
                        continue
                    buf_actual = sqlmath.db_exec(
                        bind_list=[val_input],
                        db=db,
                        response_type=response_type,
                        sql="SELECT ? AS val",
                    )
                    match response_type:
                        case "arraybuffer":
                            buf_actual = json.loads(
                                buf_actual.tobytes().decode(),
                            )[0][1][0]
                        case "list":
                            buf_actual = buf_actual[0][1][0]
                        case _:
                            buf_actual = buf_actual[0][0]["val"]
                    assertjsonequal(buf_actual, val_expected, {
                        "buf_actual": buf_actual,
                        "ii": ii,
                        "response_type": response_type,
                        "val_expected": val_expected,
                        "val_input": str(val_input),
                    })
            test_db_exec_response_type(ii, val_input, val_expected)

            # !! skip
            return

            # test db_exec's bind handling-behavior

            def test_db_exec_bind(ii, val_input, val_expected):
                for bind_list, sql in [
                    [
                        [
                            val_expected, val_expected, 0,
                        ],
                        (
                            "SELECT 0;"
                            " SELECT ? AS c1, ? AS c2, ? AS c3, ? AS c4"
                            " UNION ALL SELECT ?1, ?2, ?3, ?4"
                            " UNION ALL SELECT ?1, ?2, ?3, ?4"
                        ),
                    ],
                    [
                        {
                            "k1": val_expected,
                            "k2": val_expected,
                            "k3": 0,
                        },
                        (
                            "SELECT 0;"
                            " SELECT $k1 AS c1, $k2 AS c2,"
                            "     $k3 AS c3, $k4 AS c4"
                            " UNION ALL SELECT :k1, :k2, :k3, :k4"
                            " UNION ALL SELECT @k1, @k2, @k3, @k4"
                        ),
                    ],
                ]:
                    if val_expected is Exception:
                        asserterrorthrown(
                            lambda bind_list=bind_list, sql=sql:
                                sqlmath.db_exec(
                                    bind_list=bind_list,
                                    db=db,
                                    response_type="list",
                                    sql=sql,
                                ),
                            "not JSON serializable",
                        )
                        continue
                    buf_actual = sqlmath.db_exec(
                        bind_list=bind_list,
                        db=db,
                        response_type="list",
                        sql=sql,
                    )
                    buf_expected = [
                        [
                            ["0"],
                            [0],
                        ],
                        [
                            ["c1", "c2", "c3", "c4"],
                            [val_expected, val_expected, 0, None],
                            [val_expected, val_expected, 0, None],
                            [val_expected, val_expected, 0, None],
                        ],
                    ]
                    assertjsonequal(buf_actual, buf_expected, {
                        "buf_actual": buf_actual,
                        "ii": ii,
                        "val_expected": val_expected,
                        "val_input": str(val_input),
                    })
            test_db_exec_bind(ii, val_input, val_expected)

    def test_db_close(self):
        """Test db_close handling-behavior."""
        db = sqlmath.db_open(":memory:")
        # test null-case handling-behavior
        asserterrorthrown(lambda: sqlmath.db_close(None), "NoneType")
        # test close handling-behavior
        sqlmath.db_close(db)

    def test_db_noop(self):
        """Test db_noop handling-behavior."""
        for val_input, val_expected in [
            # 1. None
            [None, 0],
            # 2. bool
            [False, 0],
            [True, 1],
            # 3. numeric
            [-0, -0],
            [-0, 0],
            [-0.5, Exception],
            [-0x8000000000000000, -0x8000000000000000],
            [-0x8000000000000001, Exception],
            [-1, -1],
            [-1e-999, 0],
            [-1e999, Exception],
            [-2, -2],
            [-INFINITY, Exception],
            [-NAN, Exception],
            [0, 0],
            [0.5, Exception],
            [0j, 0],
            [0x7fffffffffffffff, 0x7fffffffffffffff],
            [0x8000000000000000, Exception],
            [1, 1],
            [1e-999, 0],
            [1e999, Exception],
            [1j, 0],
            [2, 2],
            [INFINITY, Exception],
            [NAN, Exception],
            # 4. str
            ["", ""],
            ["\u0000", ""],
            ["aa", "aa"],
            # 5. misc
            [(), 0],
            [(0), 0],
            [[0], 0],
            [[], 0],
            [b"", 0],
            [b"0", 0],
            [b"\x00", 0],
            [bytearray(b""), 0],
            [bytearray(b"0"), 0],
            [bytearray(b"\x00"), 0],
            [noop, 0],
            [{0: 0}, 0],
            [{}, 0],
        ]:
            if val_expected is Exception:
                asserterrorthrown(
                    lambda val_input=val_input: sqlmath.db_noop(
                        None,
                        val_input,
                        None,
                    ),
                    "",
                )
                continue
            baton = sqlmath.db_noop(None, val_input, None)[0]
            val_actual = (
                jsbaton_get_string(baton, 1)
                if isinstance(val_input, str)
                else str(jsbaton_get_int64(baton, 1))
            )
            assertjsonequal(val_actual, str(val_expected), {
                "val_actual": val_actual,
                "val_expected": val_expected,
                "val_input": val_input,
            })

    def test_db_open(self):
        """Test db_open handling-behavior."""
        # test null-case handling-behavior
        asserterrorthrown(lambda: sqlmath.db_open(None), "invalid filename")

    def test_db_exec(self):
        """Test db_exec handling-behavior."""
        db = sqlmath.db_open(":memory:")
        # test null-case handling-behavior
        asserterrorthrown(
            lambda: sqlmath.db_exec(db=db),
            "syntax error",
        )
        # test race-condition handling-behavior
        for _ignore in range(4):
            try:
                result = sqlmath.db_exec(
                    bind_list=[
                        bytes("foob", "utf-8"),
                        bytearray("fooba", "utf-8"),
                        bytearray("foobar", "utf-8"),
                    ],
                    db=db,
                    response_type="list",
                    sql="""
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
                    """,
                )
                assertjsonequal(
                    result,
                    [
                        [
                            ["c101", "c102"],
                            [101, 102],
                            [201, 202],
                            [301, None],
                        ],
                        [
                            ["c401", "c402", "c403"],
                            [401, 402, 403],
                            [501, 502.0123, 5030123456789],
                            [601, "602", '603_"\u0001\b\t\n\u000b\f\r\u000e'],
                            [None, None, None],
                            ["foob", "fooba", "foobar"],
                            ["foob", "fooba", "foobar"],
                        ],
                    ],
                )
            except Exception as err: # noqa: BLE001, PERF203
                assertorthrow(
                    "table testDbExecAsync1 already exists" in str(err),
                    err,
                )


if __name__ == "__main__":
    unittest.main()
