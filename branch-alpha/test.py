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
import pathlib
import re
import sys
import unittest

from sqlmath import (
    DB_STATE,
    INFINITY,
    LGBM_PREDICT_NORMAL,
    NAN,
    assert_error_thrown,
    assert_json_equal,
    assert_or_throw,
    db_close,
    db_exec,
    db_exec_and_return_lastblob,
    db_file_load,
    db_file_save,
    db_noop,
    db_open,
    db_table_import,
    debuginline,
    jsbaton_get_int64,
    jsbaton_get_string,
    noop,
)

noop(debuginline)


class TestCaseSqlmath(unittest.TestCase):
    """Custom TestCase."""

    def test_db_bind(self):
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
                            db_exec(
                                bind_list=bind_list,
                                db=db,
                                response_type="list",
                                sql=sql,
                            ),
                        "inclusive-range|not JSON serializable",
                    )
                    continue
                buf_actual = db_exec(
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
                        db_exec_and_return_lastblob(
                            bind_list=[val_in],
                            db=db,
                            sql="SELECT 1, 2, 3; SELECT 1, 2, ?;",
                        ),
                    "inclusive-range|not JSON serializable",
                )
                return
            buf_actual = bytes(
                db_exec_and_return_lastblob(
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
                            db_exec(
                                bind_list=[val_in],
                                db=db,
                                response_type=response_type,
                                sql="SELECT ? AS val",
                            ),
                        "inclusive-range|not JSON serializable",
                    )
                    continue
                buf_actual = db_exec(
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
        db = db_open()
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
            # ugly-hack - segmentation-fault
            # [memoryview(bytes("\u0000", "utf-8")), None],
            # [memoryview(bytes("\u0000\U0001f600\u0000", "utf-8")), None],
            # 12. 0.range
            [range(0), Exception],
            [range(1), Exception],
            # 13. 0.set
            [set(), Exception],
            [set(), Exception],
            [{None}, Exception],
            # 14. 0.str
            ["", ""],
            ["'", "'"],
            ["0", "0"],
            ["1", "1"],
            ["2", "2"],
            ['"', '"'],
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
        db = db_open(":memory:")
        # test null-case handling-behavior
        assert_error_thrown(lambda: db_close(None), "NoneType")
        # test close handling-behavior
        db_close(db)

    def test_db_exec(self):
        """Test db_exec handling-behavior."""
        db = db_open(":memory:")
        # test null-case handling-behavior
        assert_error_thrown(
            lambda: db_exec(db=db),
            "syntax error",
        )
        # test race-condition handling-behavior
        for _ignore in range(4):
            try:
                result = db_exec(
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
        CAST(GZIP_UNCOMPRESS(GZIP_COMPRESS(?1)) AS TEXT),
        CAST(GZIP_UNCOMPRESS(GZIP_COMPRESS(?2)) AS TEXT),
        CAST(GZIP_UNCOMPRESS(GZIP_COMPRESS(?3)) AS TEXT)
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
        db = db_open(":memory:")
        assert_error_thrown(
            lambda: db_file_load(db=db),
            "invalid filename None",
        )
        assert_error_thrown(
            lambda: db_file_save(db=db),
            "invalid filename None",
        )
        db_exec(
            db=db,
            sql="CREATE TABLE t01 AS SELECT 1 AS c01",
        )
        db_file_save(
            db=db,
            filename=".test_db_file_xxx.sqlite",
        )
        db = db_open(
            filename=":memory:",
        )
        db_file_load(
            db=db,
            filename=".test_db_file_xxx.sqlite",
        )
        val_actual = db_exec(
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
                    lambda val_in=val_in: db_noop(None, val_in, None),
                    "invalid arg|integer",
                )
                continue
            baton = db_noop(None, val_in, None)[0]
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
        assert_error_thrown(lambda: db_open(None), "invalid filename")

    def test_lgbm(self):
        """Test lgbm handling-behavior."""
        file_preb = "test_lgbm_preb.txt"
        file_test = "test_lgbm_binary.test"
        file_train = "test_lgbm_binary.train"
        sql_data_file = f"""
UPDATE __lgbm_state
    SET
        data_train_handle = (
            SELECT
                LGBM_DATASETCREATEFROMFILE(
                    '{file_train}', -- filename
                    'max_bin=15', -- param_data
                    NULL -- reference
                )
        );
UPDATE __lgbm_state
    SET
        data_test_handle = (
            SELECT
                LGBM_DATASETCREATEFROMFILE(
                    '{file_test}', -- filename
                    'max_bin=15', -- param_data
                    data_train_handle -- reference
                )
        );
        """
        sql_data_table = """
UPDATE __lgbm_state
    SET
        data_train_handle = (
            SELECT
                LGBM_DATASETCREATEFROMTABLE(
                    'max_bin=15', -- param_data
                    NULL, -- reference
                    --
                    _1,  _2,  _3,  _4,
                    _5,  _6,  _7,  _8,
                    _9,  _10, _11, _12,
                    _13, _14, _15, _16,
                    _17, _18, _19, _20,
                    _21, _22, _23, _24,
                    _25, _26, _27, _28,
                    _29
                )
            FROM __lgbm_file_train
        );
UPDATE __lgbm_state
    SET
        data_test_handle = (
            SELECT
                LGBM_DATASETCREATEFROMTABLE(
                    'max_bin=15', -- param_data
                    data_train_handle, -- reference
                    --
                    _1,  _2,  _3,  _4,
                    _5,  _6,  _7,  _8,
                    _9,  _10, _11, _12,
                    _13, _14, _15, _16,
                    _17, _18, _19, _20,
                    _21, _22, _23, _24,
                    _25, _26, _27, _28,
                    _29
                )
            FROM __lgbm_file_test
        );
        """
        sql_predict_file = f"""
SELECT
        LGBM_PREDICTFORFILE(
            model,                      -- model
            {LGBM_PREDICT_NORMAL},      -- predict_type
            0,                          -- start_iteration
            25,                         -- num_iteration
            '',                         -- param_pred
            --
            '{file_test}',              -- data_filename
            0,                          -- data_has_header
            'file_actual'               -- result_filename
        )
    FROM __lgbm_state;
SELECT
        LGBM_PREDICTFORFILE(
            model,                      -- model
            {LGBM_PREDICT_NORMAL},      -- predict_type
            10,                         -- start_iteration
            25,                         -- num_iteration
            '',                         -- param_pred
            --
            '{file_test}',              -- data_filename
            0,                          -- data_has_header
            'file_actual'               -- result_filename
        )
    FROM __lgbm_state;
        """
        sql_predict_table = f"""
DROP TABLE IF EXISTS __lgbm_table_preb;
CREATE TABLE __lgbm_table_preb AS
    SELECT
        DOUBLEARRAY_EXTRACT(__lgp, 0) AS prediction
    FROM (
        SELECT
            LGBM_PREDICTFORTABLE(
                (SELECT model FROM __lgbm_state),   -- model
                {LGBM_PREDICT_NORMAL},     -- predict_type
                0,                          -- start_iteration
                25,                         -- num_iteration
                '',                         -- param_pred
                --
                _2,  _3,  _4,
                _5,  _6,  _7,  _8,
                _9,  _10, _11, _12,
                _13, _14, _15, _16,
                _17, _18, _19, _20,
                _21, _22, _23, _24,
                _25, _26, _27, _28,
                _29
            ) OVER (
                ORDER BY rowid ASC
                ROWS BETWEEN 0 PRECEDING AND 0 FOLLOWING
            ) AS __lgp
        FROM __lgbm_file_test
    );
DROP TABLE IF EXISTS __lgbm_table_preb;
CREATE TABLE __lgbm_table_preb AS
    SELECT
        DOUBLEARRAY_EXTRACT(__lgp, 0) AS _1
    FROM (
        SELECT
            LGBM_PREDICTFORTABLE(
                (SELECT model FROM __lgbm_state),   -- model
                {LGBM_PREDICT_NORMAL},     -- predict_type
                10,                         -- start_iteration
                25,                         -- num_iteration
                '',                         -- param_pred
                --
                _2,  _3,  _4,
                _5,  _6,  _7,  _8,
                _9,  _10, _11, _12,
                _13, _14, _15, _16,
                _17, _18, _19, _20,
                _21, _22, _23, _24,
                _25, _26, _27, _28,
                _29
            ) OVER (
                ORDER BY rowid ASC
                ROWS BETWEEN 0 PRECEDING AND 0 FOLLOWING
            ) AS __lgp
        FROM __lgbm_file_test
    );
        """
        sql_train_data = """
UPDATE __lgbm_state
    SET
        model = LGBM_TRAINFROMDATASET(
            -- param_train
            (
                'objective=binary'
                || ' learning_rate=0.1' -- default=0.1
                || ' max_depth=-1' -- default=-1
                || ' metric=auc' -- default=""
                || ' min_data_in_leaf=20' -- default=20
                || ' num_class=1' -- default=1
                || ' num_leaves=31' -- default=31
                || ' verbosity=0' -- default=1
            ),
            50, -- num_iteration
            10, -- eval_step
            --
            data_train_handle, -- train_data
            data_test_handle -- test_data
        );
        """
        sql_train_file = f"""
UPDATE __lgbm_state
    SET
        model = LGBM_TRAINFROMFILE(
            -- param_train
            (
                'objective=binary'
                || ' learning_rate=0.1' -- default=0.1
                || ' max_depth=-1' -- default=-1
                || ' metric=auc' -- default=""
                || ' min_data_in_leaf=20' -- default=20
                || ' num_class=1' -- default=1
                || ' num_leaves=31' -- default=31
                || ' verbosity=0' -- default=1
            ),
            50, -- num_iteration
            10, -- eval_step
            --
            '{file_train}', -- file_train
            'max_bin=15', -- param_data
            '{file_test}' -- file_test
        );
        """
        sql_train_table = """
UPDATE __lgbm_state
    SET
        model = (
            SELECT
                LGBM_TRAINFROMTABLE(
                    -- param_train
                    (
                        'objective=binary'
                        || ' learning_rate=0.1' -- default=0.1
                        || ' max_depth=-1' -- default=-1
                        || ' metric=auc' -- default=""
                        || ' min_data_in_leaf=20' -- default=20
                        || ' num_class=1' -- default=1
                        || ' num_leaves=31' -- default=31
                        || ' verbosity=0' -- default=1
                    ),
                    50, -- num_iteration
                    10, -- eval_step
                    --
                    'max_bin=15', -- param_data
                    NULL, -- reference
                    --
                    _1,  _2,  _3,  _4,
                    _5,  _6,  _7,  _8,
                    _9,  _10, _11, _12,
                    _13, _14, _15, _16,
                    _17, _18, _19, _20,
                    _21, _22, _23, _24,
                    _25, _26, _27, _28,
                    _29
                )
            FROM __lgbm_file_train
        );
SELECT 1;
        """

        # --- Test Execution Function ---
        def run_test_lgbm(
            sql_data_xxx,
            sql_train_xxx,
            sql_predict_xxx, sql_ii,
        ):
            db = db_open(":memory:")
            file_actual = f".tmp/test_lgbm_preb_{sql_ii}.py.txt"
            # Import initial data
            for filename, table_name in [
                (file_preb, "__lgbm_file_preb"),
                (file_test, "__lgbm_file_test"),
                (file_train, "__lgbm_file_train"),
            ]:
                # ugly-hack - Fix unknown file-not-exist-bug in wheel.
                if not (
                    DB_STATE["lgbm"]
                    and pathlib.Path(filename).exists()
                ):
                    print(f"test_lgbm - {filename} - not exist", sys.stderr)
                    return
                db_table_import(
                    db=db,
                    filename=filename,
                    header_missing=True,
                    mode="tsv",
                    table_name=table_name,
                )
            db_exec(
                db=db,
                sql=f"""
-- lgbm - init
CREATE TABLE __lgbm_state(
    data_test_handle INTEGER,
    data_test_num_data REAL,
    data_test_num_feature REAL,
    --
    data_train_handle INTEGER,
    data_train_num_data REAL,
    data_train_num_feature REAL,
    --
    model BLOB
);
INSERT INTO __lgbm_state(rowid) SELECT 1;
-- lgbm - data
{sql_data_xxx};
UPDATE __lgbm_state
    SET
        data_test_num_data = LGBM_DATASETGETNUMDATA(data_test_handle),
        data_test_num_feature = LGBM_DATASETGETNUMFEATURE(data_test_handle),
        data_train_num_data = LGBM_DATASETGETNUMDATA(data_train_handle),
        data_train_num_feature = LGBM_DATASETGETNUMFEATURE(data_train_handle);
-- lgbm - train
{sql_train_xxx};
                """,
            )
            db_exec(
                db=db,
                sql=f"""
-- lgbm - predict
{sql_predict_xxx.replace("file_actual", file_actual)};
-- lgbm - cleanup
SELECT
        LGBM_DATASETFREE(data_test_handle),
        LGBM_DATASETFREE(data_train_handle)
    FROM __lgbm_state;
                """,
            )
            if sql_predict_xxx == sql_predict_file:
                db_table_import(
                    db=db,
                    filename=file_actual,
                    header_missing=True,
                    mode="tsv",
                    table_name="__lgbm_table_preb",
                )
            db_file_save(
                db=db,
                filename=f".tmp/test_lgbm_{sql_ii}.py.sqlite",
            )
            # Assertions
            assert_json_equal(
                db_exec(
                    db=db,
                    sql="""
SELECT
        data_test_num_data,
        data_test_num_feature,
        data_train_num_data,
        data_train_num_feature
    FROM __lgbm_state;
                    """,
                )[-1][-1],
                {
                    "data_test_num_data": 500.0,
                    "data_test_num_feature": 28.0,
                    "data_train_num_data": 7000.0,
                    "data_train_num_feature": 28.0,
                },
            )
            if sql_predict_xxx == sql_predict_file:
                with pathlib.Path(file_actual).open(encoding="utf8") as ff:
                    data_actual = ff.read()
                with pathlib.Path(file_preb).open(encoding="utf8") as ff:
                    data_preb = ff.read()
                assert_json_equal(data_actual, data_preb)
            assert_json_equal(
                db_exec(
                    db=db,
                    sql="""
SELECT ROUND(_1, 8) AS _1 FROM __lgbm_table_preb;
                    """,
                )[-1],
                db_exec(
                    db=db,
                    sql="""
SELECT ROUND(_1, 8) AS _1 FROM __lgbm_file_preb;
                    """,
                )[-1],
            )
        # --- Combinatorial Loop ---
        sql_ii = 0
        for data_sql in [sql_data_file, sql_data_table]:
            for train_sql in [sql_train_data, sql_train_file, sql_train_table]:
                for predict_sql in [sql_predict_file, sql_predict_table]:
                    sql_ii += 1
                    run_test_lgbm(data_sql, train_sql, predict_sql, sql_ii)


if __name__ == "__main__":
    unittest.main()
