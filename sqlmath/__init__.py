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


"""sqlmath.py."""

__version__ = "2023.10.1"
__version_info__ = ("2023", "10", "1")

import json
import math
import re
import struct
import sys

from ._sqlmath import _pybatonCall

JSBATON_ARGC = 16
SQLITE_DATATYPE_BLOB = 0x04
SQLITE_DATATYPE_FLOAT = 0x02
SQLITE_DATATYPE_INTEGER = 0x01
SQLITE_DATATYPE_INTEGER_0 = 0x00
SQLITE_DATATYPE_INTEGER_1 = 0x21
SQLITE_DATATYPE_NULL = 0x05
SQLITE_DATATYPE_OFFSET = 768
SQLITE_DATATYPE_SHAREDARRAYBUFFER = 0x71
SQLITE_DATATYPE_TEXT = 0x03
SQLITE_DATATYPE_TEXT_0 = 0x13
SQLITE_MAX_LENGTH2 = 1_000_000_000
SQLITE_OPEN_AUTOPROXY = 0x00000020     # VFS only
SQLITE_OPEN_CREATE = 0x00000004        # Ok for sqlite3_open_v2()
SQLITE_OPEN_DELETEONCLOSE = 0x00000008 # VFS only
SQLITE_OPEN_EXCLUSIVE = 0x00000010     # VFS only
SQLITE_OPEN_FULLMUTEX = 0x00010000     # Ok for sqlite3_open_v2()
SQLITE_OPEN_MAIN_DB = 0x00000100       # VFS only
SQLITE_OPEN_MAIN_JOURNAL = 0x00000800  # VFS only
SQLITE_OPEN_MEMORY = 0x00000080        # Ok for sqlite3_open_v2()
SQLITE_OPEN_NOFOLLOW = 0x01000000      # Ok for sqlite3_open_v2()
SQLITE_OPEN_NOMUTEX = 0x00008000       # Ok for sqlite3_open_v2()
SQLITE_OPEN_PRIVATECACHE = 0x00040000  # Ok for sqlite3_open_v2()
SQLITE_OPEN_READONLY = 0x00000001      # Ok for sqlite3_open_v2()
SQLITE_OPEN_READWRITE = 0x00000002     # Ok for sqlite3_open_v2()
SQLITE_OPEN_SHAREDCACHE = 0x00020000   # Ok for sqlite3_open_v2()
SQLITE_OPEN_SUBJOURNAL = 0x00002000    # VFS only
SQLITE_OPEN_SUPER_JOURNAL = 0x00004000 # VFS only
SQLITE_OPEN_TEMP_DB = 0x00000200       # VFS only
SQLITE_OPEN_TEMP_JOURNAL = 0x00001000  # VFS only
SQLITE_OPEN_TRANSIENT_DB = 0x00000400  # VFS only
SQLITE_OPEN_URI = 0x00000040           # Ok for sqlite3_open_v2()
SQLITE_OPEN_WAL = 0x00080000           # VFS only


class SqlmathError(Exception):
    """Custom error."""


def asserterrorthrown(func, regexp=None):
    """This function will assert calling <func> throws an error."""
    err = None
    try:
        func()
    except Exception as err_caught: # noqa: BLE001
        err = err_caught
    assertorthrow(err, "No error thrown.")
    assertorthrow(regexp is None or re.match(regexp, str(err)), err)


def assertjsonequal(aa, bb, message):
    """This function will assert."""
    """JSON.stringify(<aa>) === JSON.stringify(<bb>)."""
    aa = json.dumps(objectdeepcopywithkeyssorted(aa), indent=1)
    bb = json.dumps(objectdeepcopywithkeyssorted(bb), indent=1)
    if aa != bb:
        raise SqlmathError(
            f"\n{aa}\n!==\n{bb}"
            + (
                " - " + message
                if isinstance(message, str)
                else " - " + json.dumps(message)
                if message
                else ""
            ),
        )


def assertint64(val):
    """This function will assert <val> is within range."""
    """of c99-signed-long-long."""
    assertorthrow(
        -9_223_372_036_854_775_808 <= val <= 9_223_372_036_854_775_807, # noqa: PLR2004
        (
            f"integer {val} outside signed-64-bit inclusive-range"
            " -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807"
        ),
    )


def assertorthrow(condition, message):
    """This function will throw <message> if <condition> is falsy."""
    if not condition:
        raise (
            SqlmathError(str(message)[:2048])
            if not message or isinstance(message, str)
            else message
        )


def db_call(baton, cfuncname, *arglist):
    """This function will serialize <arglist> to a c <baton>."""
    """suitable for passing into napi."""

    def arg_serialize(argi, value):
        nonlocal baton
        if isinstance(value, (bool, int)):
            assertint64(value)
            struct.pack_into("q", baton, 8 + argi * 8, value)
            return value
        if isinstance(value, float):
            assertorthrow(
                int(value) == value,
                f"db_call - float {value} is not an integer",
            )
            assertint64(value)
            struct.pack_into("q", baton, 8 + argi * 8, int(value))
            return value
        if isinstance(value, str):
            baton = jsbaton_value_push(
                baton,
                argi,
                value if value.endswith("\u0000") else value + "\u0000",
            )
            return None
        if isinstance(value, memoryview):
            assertorthrow(
                value.contiguous,
                "db_call - memoryview-object must be contiguous",
            )
            return value
        return None
    assertorthrow(
        len(arglist) <= JSBATON_ARGC,
        f"db_call - len(arglist) must be less than than {JSBATON_ARGC}",
    )
    # init baton
    baton = memoryview(bytearray(1024))
    # offset nalloc, nused
    struct.pack_into("i", baton, 4, SQLITE_DATATYPE_OFFSET)
    # serialize js-value to c-value
    arglist = [
        arg_serialize(argi, value) for argi, value in enumerate(arglist)
    ]
    # pad argList to length JSBATON_ARGC
    while len(arglist) < 2 * JSBATON_ARGC:
        arglist.append(0)
    # encode cfuncname into baton
    baton = jsbaton_value_push(baton, 2 * JSBATON_ARGC, f"{cfuncname}\u0000")
    # prepend baton, cfuncname to arglist
    arglist = [baton, cfuncname, *arglist]
    _pybatonCall(baton, cfuncname, arglist)
    return arglist


def db_noop(*arglist):
    """This function will do nothing except return <arglist>."""
    return db_call(None, "_dbNoop", *arglist)


def db_open(*arglist):
    """This function will open and return sqlite-database-connection <db>."""
    db_call(None, "_dbNoop", *arglist)


def debuginline(*argv):
    """This function will print <arg> to stderr and then return <arg0>."""
    arg0 = argv[0] if argv else None
    print("\n\ndebuginline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n")
    return arg0


def jsbaton_value_push(baton, argi, value, externalbufferlist=None):
    """
    This function will push <value> to buffer <baton>.

    #define SQLITE_DATATYPE_BLOB            0x04
    #define SQLITE_DATATYPE_FLOAT           0x02
    #define SQLITE_DATATYPE_INTEGER         0x01
    #define SQLITE_DATATYPE_INTEGER_0       0x00
    #define SQLITE_DATATYPE_INTEGER_1       0x21
    #define SQLITE_DATATYPE_NULL            0x05
    #define SQLITE_DATATYPE_OFFSET          768
    #define SQLITE_DATATYPE_SHAREDARRAYBUFFER       0x71
    #define SQLITE_DATATYPE_TEXT            0x03
    #define SQLITE_DATATYPE_TEXT_0          0x13
        #  1. 0.NoneType
        #  2. 0.bool
        #  3. 0.bytearray
        #  4. 0.bytes
        #  5. 0.complex
        #  6. 0.dict
        #  7. 0.float
        #  8. 0.frozenset
        #  9. 0.int
        # 10. 0.list
        # 11. 0.memoryview
        # 12. 0.range
        # 13. 0.set
        # 14. 0.str
        # 15. 0.tuple
        # 16. 1.NoneType
        # 17. 1.bool
        # 18. 1.bytearray
        # 19. 1.bytes
        # 20. 1.complex
        # 21. 1.dict
        # 22. 1.float
        # 23. 1.frozenset
        # 24. 1.int
        # 25. 1.list
        # 26. 1.memoryview
        # 27. 1.range
        # 28. 1.set
        # 29. 1.str
        # 20. 1.tuple
        # 21. json
    """
    # 17. 1.bool
    if value == 1:
        value = True
    match (("1." if value else "0.") + type(value).__name__):
        #  1. 0.NoneType
        case "0.NoneType":
            vtype = SQLITE_DATATYPE_NULL
            vsize = 0
        #  2. 0.bool
        case "0.bool":
            vtype = SQLITE_DATATYPE_INTEGER_0
            vsize = 0
        #  3. 0.bytearray
        case "0.bytearray":
            vtype = SQLITE_DATATYPE_NULL
            vsize = 0
        #  4. 0.bytes
        case "0.bytes":
            vtype = SQLITE_DATATYPE_NULL
            vsize = 0
        #  5. 0.complex
        #  6. 0.dict
        #  7. 0.float
        case "0.float":
            vtype = SQLITE_DATATYPE_INTEGER_0
            vsize = 0
        #  8. 0.frozenset
        #  9. 0.int
        case "0.int":
            vtype = SQLITE_DATATYPE_INTEGER_0
            vsize = 0
        # 10. 0.list
        # 11. 0.memoryview
        case "0.memoryview":
            vtype = SQLITE_DATATYPE_NULL
            vsize = 0
        # 12. 0.range
        # 13. 0.set
        # 14. 0.str
        case "0.str":
            vtype = SQLITE_DATATYPE_TEXT_0
            vsize = 0
        # 15. 0.tuple
        # 16. 1.NoneType
        # 17. 1.bool
        case "1.bool":
            vtype = SQLITE_DATATYPE_INTEGER_1
            vsize = 0
        # 18. 1.bytearray
            vtype = SQLITE_DATATYPE_BLOB
            vsize = 4 + len(value)
        # 19. 1.bytes
            vtype = SQLITE_DATATYPE_BLOB
            vsize = 4 + len(value)
        # 20. 1.complex
        # 21. 1.dict
        # 22. 1.float
        case "1.float":
            vtype = SQLITE_DATATYPE_FLOAT
            vsize = 8
        # 23. 1.frozenset
        # 24. 1.int
        case "1.int":
            vtype = SQLITE_DATATYPE_INTEGER
            vsize = 8
        # 25. 1.list
        # 26. 1.memoryview
            assertorthrow(
                len(externalbufferlist) <= 8, # noqa: PLR2004
                "len(externalbufferList) must be less than or equal to 8",
            )
            externalbufferlist.append(value)
            vtype = SQLITE_DATATYPE_SHAREDARRAYBUFFER
            vsize = 4
        # 27. 1.range
        # 28. 1.set
        # 29. 1.str
        case "1.str":
            value = bytes(value, "utf-8")
            vtype = SQLITE_DATATYPE_TEXT
            vsize = 4 + len(value)
        # 30. 1.tuple
        # 31. json
        case _:
            value = bytes(json.dumps(value), "utf-8")
            vtype = SQLITE_DATATYPE_TEXT
            vsize = 4 + len(value)
    nused = struct.unpack_from("i", baton, 4)[0]
    nn = nused + 1 + vsize
    assertorthrow(
        nn <= 0xffff_ffff, # noqa: PLR2004
        "jsbaton cannot exceed 0x7fff_ffff / 2,147,483,647 bytes",
    )
    # exponentially grow baton as needed
    if len(baton) < nn:
        tmp = baton
        baton = memoryview(
            bytearray(min(2 ** math.ceil(math.log(nn, 2)), 0x7fff_ffff)),
        )
        # update nalloc
        struct.pack_into("i", baton, 0, len(baton))
        # copy tmp to baton
        baton[:len(tmp)] = tmp
    # push vtype
    struct.pack_into("b", baton, nused, vtype)
    # update nused
    struct.pack_into("i", baton, 4, nused + 1 + vsize)
    # handle blob-value
    if vtype in (SQLITE_DATATYPE_BLOB, SQLITE_DATATYPE_TEXT):
        # set argv[ii] to blob/text location
        if argi is not None:
            struct.pack_into("i", baton, 8 + argi * 8, nused)
        vsize -= 4
        # push vsize
        assertorthrow(
            vsize >= 0 and vsize <= SQLITE_MAX_LENGTH2,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                f" 0 to {SQLITE_MAX_LENGTH2}"
            ),
        )
        struct.pack_into("i", baton, nused + 1, vsize)
        baton[nused + 1 + 4:nused + 1 + 4 + vsize] = value
        return baton
    if vtype == SQLITE_DATATYPE_FLOAT:
        struct.pack_into("d", baton, nused + 1, value)
        return baton
    if vtype == SQLITE_DATATYPE_INTEGER:
        assertint64(value)
        struct.pack_into("q", baton, nused + 1, value)
        return baton
    if vtype == SQLITE_DATATYPE_SHAREDARRAYBUFFER:
        vsize = len(value)
        assertorthrow(
            vsize >= 0 and vsize <= 1_000_000_000, # noqa: PLR2004
            (
                "sqlite-blob byte-length must be within inclusive-range"
                " 0 to 1,000,000,000"
            ),
        )
        struct.pack_into("i", baton, nused + 1, value)
        return baton
    return baton


def jsbaton_value_string(baton, argi):
    """This function will return string-value from <baton>."""
    """at given <offset>."""
    offset = struct.unpack_from("q", baton, 4 + 4 + argi * 8)[0]
    return str(
        baton[
            offset + 1 + 4:
            # remove null-terminator from string
            offset + 1 + 4 + struct.unpack_from("i", baton, offset + 1)[0] - 1
        ],
        "utf-8",
    )


def noop(val=None, *_, **__):
    """This function will do nothing except return <val>."""
    return val


def objectdeepcopywithkeyssorted(obj):
    """This function will recursively deep-copy <obj> with keys sorted."""
    if isinstance(obj, float):
        return int(obj) if int(obj) == obj else obj
    if not obj or not isinstance(obj, dict):
        return obj
    # Recursively deep-copy list with child-keys sorted.
    if isinstance(obj, list):
        return [objectdeepcopywithkeyssorted(elem) for elem in obj]
    # Recursively deep-copy obj with keys sorted.
    return dict(sorted((str(key), val) for key, val in obj.items()))
