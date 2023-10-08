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
            # 1. none
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
            [b"\u0000", 0],
            [bytearray(b""), 0],
            [bytearray(b"0"), 0],
            [bytearray(b"\u0000"), 0],
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
                )
                continue
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
