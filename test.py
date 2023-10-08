"""
test.py.

python test.py --verbose
"""

import struct
import unittest

import sqlmath
from sqlmath import (
    asserterrorthrown,
    assertjsonequal,
    assertorthrow,
    debuginline,
    jsbaton_value_string,
    noop,
)

INFINITY = float("inf")
NAN = float("nan")
noop(assertorthrow, asserterrorthrown, assertjsonequal, debuginline)


class TestCaseSqlmath(unittest.TestCase):
    """Custom TestCase."""

    def test_db_noop(self):
        """Test db_noop."""
        for val_input, val_expected in [
            ["", ""],
            ["\u0000", ""],
            ["aa", "aa"],
            [-0, -0],
            [-0, 0],
            [-0.5, None],
            [-0x8000000000000000, -0x8000000000000000],
            [-0x8000000000000001, 0x7fffffffffffffff],
            [-1, -1],
            [-1e-999, 0],
            [-1e999, None],
            [-2, -2],
            [-INFINITY, None],
            [-NAN, None],
            [0, 0],
            [0.5, None],
            [0x7fffffffffffffff, 0x7fffffffffffffff],
            [0x8000000000000000, -0x8000000000000000],
            [1, 1],
            [1e-999, 0],
            [1e999, None],
            [2, 2],
            [False, 0],
            [INFINITY, None],
            [NAN, None],
            [None, 0],
            [True, 1],
            [noop, 0],
            [{}, 0],
        ]:
            if val_expected is None:
                asserterrorthrown(
                    lambda val_input=val_input: sqlmath.db_noop(
                        None,
                        val_input,
                        None,
                    ),
                )
                return
            baton = sqlmath.db_noop(None, val_input, None)[0]
            val_actual = (
                jsbaton_value_string(baton, 1)
                if isinstance(val_input, str)
                else str(struct.unpack_from("q", baton, 4 + 4 + 8)[0])
            )
            assertjsonequal(val_actual, str(val_expected), {
                "val_actual": val_actual,
                "val_expected": val_expected,
                "val_input": val_input,
            })


if __name__ == "__main__":
    unittest.main()
