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
test_lgbm_sqlite.py.

python test_lgbm_sqlite.py --verbose
"""

import unittest

import sqlmath
from sqlmath import (
    debuginline,
    noop,
)

noop(debuginline)

FILE_BINARY_TEST = "test_lgbm_binary.test"
FILE_BINARY_TRAIN = "test_lgbm_binary.train"
LGBM_PREDICT_NORMAL = 0

class TestCaseSqlmath(unittest.TestCase):
    """Custom TestCase."""

    def test_lgbm(self):
        """Test lgbm handling-behavior."""
        filePreb = "test_lgbm_preb.txt"
        fileTest = "test_lgbm_binary.test"
        fileTrain = "test_lgbm_binary.train"
        sqlDataFile = f"""
UPDATE __lgbm_state
    SET
        data_train_handle = (
            SELECT
                LGBM_DATASETCREATEFROMFILE(
                    '{fileTrain}', -- filename
                    'max_bin=15', -- param_data
                    NULL -- reference
                )
        );
UPDATE __lgbm_state
    SET
        data_test_handle = (
            SELECT
                LGBM_DATASETCREATEFROMFILE(
                    '{fileTest}', -- filename
                    'max_bin=15', -- param_data
                    data_train_handle -- reference
                )
        );
        """
        sqlDataTable = """
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
        sqlIi = 0
        sqlPredictFile = f"""
SELECT
        LGBM_PREDICTFORFILE(
            model,                      -- model
            {LGBM_PREDICT_NORMAL},      -- predict_type
            0,                          -- start_iteration
            25,                         -- num_iteration
            '',                         -- param_pred
            --
            '{fileTest}',               -- data_filename
            0,                          -- data_has_header
            'file_actual'                -- result_filename
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
            '{fileTest}',               -- data_filename
            0,                          -- data_has_header
            'file_actual'                -- result_filename
        )
    FROM __lgbm_state;
        """
        sqlPredictTable = f"""
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
        sqlTrainData = """
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
        sqlTrainFile = f"""
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
            '{fileTrain}', -- file_train
            'max_bin=15', -- param_data
            '{fileTest}' -- file_test
        );
        """
        sqlTrainTable = """
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
        def run_test_lgbm(sqlDataXxx, sqlTrainXxx, sqlPredictXxx, sqlIi):
            db = sqlmath.db_open(":memory:")
            file_actual = f".tmp/test_lgbm_preb_{sqlIi}.txt"

            # Import initial data
            for filename, table_name in [(filePreb, "__lgbm_file_preb"),
                                    (fileTest, "__lgbm_file_test"),
                                    (fileTrain, "__lgbm_file_train")]:
                sqlmath.db_table_import(
                    db=db,
                    filename=filename,
                    header_missing=True,
                    mode="tsv",
                    table_name=table_name,
                )
                # !! sqlmath.db_exec(db=db, sql=f"""
# !! CREATE VIRTUAL TABLE {table_name} USING tsv(filename='{filename}');
                # !! """)

            # !! return
            # Setup state and train
            sqlmath.db_exec(db=db, mode_debug=1, sql="""
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
--------------------
            """)
            # !! sqlmath.db_exec(db=db, mode_debug=1, sql=f"""
# !! -- lgbm - init
# !! CREATE TABLE __lgbm_state(
    # !! data_test_handle INTEGER,
    # !! data_test_num_data REAL,
    # !! data_test_num_feature REAL,
    # !! --
    # !! data_train_handle INTEGER,
    # !! data_train_num_data REAL,
    # !! data_train_num_feature REAL,
    # !! --
    # !! model BLOB
# !! );
# !! INSERT INTO __lgbm_state(rowid) SELECT 1;

# !! -- lgbm - data
# !! {sqlDataXxx};
# !! UPDATE __lgbm_state
    # !! SET
        # !! data_test_num_data = LGBM_DATASETGETNUMDATA(data_test_handle),
        # !! data_test_num_feature = LGBM_DATASETGETNUMFEATURE(data_test_handle),
        # !! data_train_num_data = LGBM_DATASETGETNUMDATA(data_train_handle),
        # !! data_train_num_feature = LGBM_DATASETGETNUMFEATURE(data_train_handle);

# !! -- lgbm - train
# !! {sqlTrainXxx};
            # !! """)
            sqlmath.db_exec(db=db, sql=f"""
-- lgbm - predict
{sqlPredictXxx.replace("file_actual", file_actual)};

-- lgbm - cleanup
SELECT
        LGBM_DATASETFREE(data_test_handle),
        LGBM_DATASETFREE(data_train_handle)
    FROM __lgbm_state;
            """)

            # Predict and Cleanup
            # Note: .replace() handles the dynamic filename as the JS regex did
            sqlmath.db_exec(db=db, sql=f"""
-- lgbm - predict
{sqlPredictXxx.replace("file_actual", file_actual)};

-- lgbm - cleanup
SELECT
        LGBM_DATASETFREE(data_test_handle),
        LGBM_DATASETFREE(data_train_handle)
    FROM __lgbm_state;
            """)

            if sqlPredictXxx == sqlPredictFile:
                db_table_import(db=db, filename=file_actual, header_missing=True, mode="tsv", table_name="__lgbm_table_preb")

            db_file_save(db=db, filename=f".tmp/test_lgbm_{sqlIi}.sqlite")

            # Assertions
            state = sqlmath.db_exec_and_return_last_row(db=db, sql="""
                SELECT data_test_num_data, data_test_num_feature, data_train_num_data, data_train_num_feature
                FROM __lgbm_state;
            """)
            assert state == {
                "data_test_num_data": 500,
                "data_test_num_feature": 28,
                "data_train_num_data": 7000,
                "data_train_num_feature": 28,
            }

        # --- Combinatorial Loop ---
        for dataSql in [sqlDataFile, sqlDataTable]:
            for trainSql in [sqlTrainData, sqlTrainFile]:
                for predictSql in [sqlPredictFile, sqlPredictTable]:
                    sqlIi += 1
                    run_test_lgbm(dataSql, trainSql, predictSql, sqlIi)

if __name__ == "__main__":
    unittest.main()

