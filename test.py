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

import datetime
import json
import math
import re
import unittest

import sqlmath
from sqlmath import (
    INFINITY,
    NAN,
    assert_error_thrown,
    assert_json_equal,
    assert_or_throw,
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
        def test_db_bind_exec(ii, val_in, val_expect):
            for bind_list, sql in [
                [
                    [
                        val_expect, val_expect, 0,
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
                        "k1": val_expect,
                        "k2": val_expect,
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
                if val_expect is Exception:
                    assert_error_thrown(
                        lambda bind_list=bind_list, sql=sql:
                            sqlmath.db_exec(
                                bind_list=bind_list,
                                db=db,
                                response_type="list",
                                sql=sql,
                            ),
                        "inclusive-range|not JSON serializable",
                    )
                    continue
                buf_actual = sqlmath.db_exec(
                    bind_list=bind_list,
                    db=db,
                    response_type="list",
                    sql=sql,
                )
                buf_expect = [
                    [
                        ["0"],
                        [0],
                    ],
                    [
                        ["c1", "c2", "c3", "c4"],
                        [val_expect, val_expect, 0, None],
                        [val_expect, val_expect, 0, None],
                        [val_expect, val_expect, 0, None],
                    ],
                ]
                assert_json_equal(buf_actual, buf_expect, {
                    "buf_actual": buf_actual,
                    "ii": ii,
                    "val_expect": val_expect,
                    "val_in": str(val_in),
                })

        def test_db_bind_lastblob(ii, val_in, val_expect):
            if val_expect is Exception:
                assert_error_thrown(
                    lambda:
                        sqlmath.db_exec_and_return_lastblob(
                            bind_list=[val_in],
                            db=db,
                            sql="SELECT 1, 2, 3; SELECT 1, 2, ?;",
                        ),
                    "inclusive-range|not JSON serializable",
                )
                return
            buf_actual = bytes(
                sqlmath.db_exec_and_return_lastblob(
                    bind_list=[val_in],
                    db=db,
                    sql="SELECT 1, 2, 3; SELECT 1, 2, ?;",
                ),
            ).decode()
            buf_expect = str(val_expect)
            #  1. 0.NoneType
            if val_in is None:
                buf_expect = ""
            #  3. 0.bytearray
            #  4. 0.bytes
            if isinstance(val_in, (bytearray, bytes)):
                buf_expect = val_in.decode()
            if isinstance(val_in, memoryview):
                buf_expect = val_in.obj.decode()
            #  7. 0.float
            if isinstance(val_in, float):
                if math.isnan(val_in):
                    buf_expect = ""
                elif val_in == -INFINITY:
                    buf_expect = "-Inf"
                elif val_in == INFINITY:
                    buf_expect = "Inf"
            assert_json_equal(buf_actual, buf_expect, {
                "buf_actual": buf_actual,
                "buf_expect": buf_expect,
                "ii": ii,
                "val_expect": val_expect,
                "val_in": str(val_in),
            })

        def test_db_bind_response_type(ii, val_in, val_expect):
            for response_type in [
                "arraybuffer",
                "list",
                None,
            ]:
                if val_expect is Exception:
                    assert_error_thrown(
                        lambda response_type=response_type:
                            sqlmath.db_exec(
                                bind_list=[val_in],
                                db=db,
                                response_type=response_type,
                                sql="SELECT ? AS val",
                            ),
                        "inclusive-range|not JSON serializable",
                    )
                    continue
                buf_actual = sqlmath.db_exec(
                    bind_list=[val_in],
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
                assert_json_equal(buf_actual, val_expect, {
                    "buf_actual": buf_actual,
                    "ii": ii,
                    "response_type": response_type,
                    "val_expect": val_expect,
                    "val_in": str(val_in),
                })
        db = sqlmath.db_open(":memory:")
        # test datatype handling-behavior
        for ii, (val_in, val_expect) in enumerate([
            #  1. 0.NoneType
            [None, None],
            #  2. 0.bool
            [False, 0],
            [True, 1],
            #  3. 0.bytearray
            [bytearray(b""), None],
            [bytearray(b"0"), None],
            [bytearray(bytes("\u0000", "utf-8")), None],
            [bytearray(bytes("\u0000\U0001f600\u0000", "utf-8")), None],
            #  4. 0.bytes
            [b"", None],
            [b"0", None],
            [bytes("\u0000", "utf-8"), None],
            [bytes("\u0000\U0001f600\u0000", "utf-8"), None],
            #  5. 0.complex
            [0j, Exception],
            [1j, Exception],
            #  6. 0.dict
            [{}, "{}"],
            [{0: 0}, '{"0": 0}'],
            #  7. 0.float
            [-0.5, -0.5],
            [-1e-999, 0],
            [-1e999, None],
            [-INFINITY, None],
            [-NAN, None],
            [0.5, 0.5],
            [1e-999, 0],
            [1e999, None],
            [INFINITY, None],
            [NAN, None],
            #  8. 0.frozenset
            #  9. 0.int
            [-0, -0],
            [-0, 0],
            [-0x8000000000000000, "-9223372036854775808"],
            [-0x8000000000000001, Exception],
            [-1, -1],
            [-2, -2],
            [0, 0],
            [0x7fffffffffffffff, "9223372036854775807"],
            [0x8000000000000000, Exception],
            [1, 1],
            [2, 2],
            # 10. 0.list
            [[0], "[0]"],
            [[None], "[null]"],
            [[], "[]"],
            # 11. 0.memoryview
            [memoryview(b""), None],
            # !! [memoryview(bytes("\u0000", "utf-8")), None],
            # !! [memoryview(bytes("\u0000\U0001f600\u0000", "utf-8")), None],
            # 12. 0.range
            [range(0), Exception],
            [range(1), Exception],
            # 13. 0.set
            [set(), Exception],
            [set(), Exception],
            [{None}, Exception],
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
            [(None,), "[null]"],
            # 16. 0.misc
            [datetime.datetime.now(datetime.timezone.utc), Exception],
            [noop, Exception],
            [re.compile(""), Exception],
        ]):
            test_db_bind_exec(ii, val_in, val_expect)
            test_db_bind_lastblob(ii, val_in, val_expect)
            test_db_bind_response_type(ii, val_in, val_expect)

    def test_db_close(self):
        """Test db_close handling-behavior."""
        db = sqlmath.db_open(":memory:")
        # test null-case handling-behavior
        assert_error_thrown(lambda: sqlmath.db_close(None), "NoneType")
        # test close handling-behavior
        sqlmath.db_close(db)

    def test_db_exec(self):
        """Test db_exec handling-behavior."""
        db = sqlmath.db_open(":memory:")
        # test null-case handling-behavior
        assert_error_thrown(
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
                assert_json_equal(
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
                assert_or_throw(
                    "table testDbExecAsync1 already exists" in str(err),
                    err,
                )

    def test_db_file_xxx(self):
        """Test db_file_xxx handling-behavior."""
        db = sqlmath.db_open(":memory:")
        assert_error_thrown(
            lambda: sqlmath.db_file_load(db=db),
            "invalid filename None",
        )
        assert_error_thrown(
            lambda: sqlmath.db_file_save(db=db),
            "invalid filename None",
        )
        sqlmath.db_exec(
            db=db,
            sql="CREATE TABLE t01 AS SELECT 1 AS c01",
        )
        sqlmath.db_file_save(
            db=db,
            filename=".test_db_file_xxx.sqlite",
        )
        db = sqlmath.db_open(
            filename=":memory:",
        )
        sqlmath.db_file_load(
            db=db,
            filename=".test_db_file_xxx.sqlite",
        )
        val_actual = sqlmath.db_exec(
            db=db,
            sql="SELECT * FROM t01",
        )
        val_expect = [
            [
                {"c01": 1},
            ],
        ]
        assert_json_equal(val_actual, val_expect)

    def test_db_noop(self):
        """Test db_noop handling-behavior."""
        for val_in, val_expect in [
            #  1. 0.NoneType
            [None, 0],
            #  2. 0.bool
            [False, 0],
            [True, 1],
            #  3. 0.bytearray
            [bytearray(b""), Exception],
            [bytearray(b"0"), Exception],
            [bytearray(bytes("\u0000", "utf-8")), Exception],
            [bytearray(bytes("\u0000\U0001f600\u0000", "utf-8")), Exception],
            #  4. 0.bytes
            [b"", Exception],
            [b"0", Exception],
            [bytes("\u0000", "utf-8"), Exception],
            [bytes("\u0000\U0001f600\u0000", "utf-8"), Exception],
            #  5. 0.complex
            [0j, Exception],
            [1j, Exception],
            #  6. 0.dict
            [{}, Exception],
            [{0: 0}, Exception],
            #  7. 0.float
            [-0.5, Exception],
            [-1e-999, 0],
            [-1e999, Exception],
            [-INFINITY, Exception],
            [-NAN, Exception],
            [0.5, Exception],
            [1e-999, 0],
            [1e999, Exception],
            [INFINITY, Exception],
            [NAN, Exception],
            #  8. 0.frozenset
            #  9. 0.int
            [-0, -0],
            [-0, 0],
            [-0x8000000000000000, -0x8000000000000000],
            [-0x8000000000000001, Exception],
            [-1, -1],
            [-2, -2],
            [0, -0],
            [0, 0],
            [0x7fffffffffffffff, 0x7fffffffffffffff],
            [0x8000000000000000, Exception],
            [1, 1],
            [2, 2],
            # 10. 0.list
            [[0], Exception],
            [[None], Exception],
            [[], Exception],
            # 11. 0.memoryview
            [memoryview(b""), Exception],
            [memoryview(b"0"), Exception],
            [memoryview(bytes("\u0000", "utf-8")), Exception],
            [memoryview(bytes("\u0000\U0001f600\u0000", "utf-8")), Exception],
            # 12. 0.range
            [range(0), Exception],
            [range(1), Exception],
            # 13. 0.set
            [set(), Exception],
            [set(), Exception],
            [{None}, Exception],
            # 14. 0.str
            ["", ""],
            ["0", "0"],
            ["1", "1"],
            ["2", "2"],
            ["\u0000", ""],
            ["\u0000\U0001f600\u0000", "\u0000\ud83d\ude00"],
            ["a" * 9999, "a" * 9999],
            # 15. 0.tuple
            [(), Exception],
            [(None,), Exception],
            # 16. 0.misc
            [datetime.datetime.now(datetime.timezone.utc), Exception],
            [noop, Exception],
            [re.compile(""), Exception],
        ]:
            if val_expect is Exception:
                assert_error_thrown(
                    lambda val_in=val_in: sqlmath.db_noop(None, val_in, None),
                    "invalid arg|integer",
                )
                continue
            baton = sqlmath.db_noop(None, val_in, None)[0]
            val_actual = (
                jsbaton_get_string(baton, 1)
                if isinstance(val_in, str)
                else str(jsbaton_get_int64(baton, 1))
            )
            assert_json_equal(val_actual, str(val_expect), {
                "val_actual": val_actual,
                "val_expect": val_expect,
                "val_in": val_in,
            })

    def test_db_open(self):
        """Test db_open handling-behavior."""
        # test null-case handling-behavior
        assert_error_thrown(lambda: sqlmath.db_open(None), "invalid filename")


if __name__ == "__main__":
    unittest.main()
