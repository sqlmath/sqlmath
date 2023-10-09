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
import weakref

from ._sqlmath import _pydbCall

JSBATON_ARGC = 16
JSBATON_OFFSET_ALL = 768
JSBATON_OFFSET_ARG0 = 2
JSBATON_OFFSET_ARGV = 8
JSBATON_OFFSET_BUFV = 136
JSBATON_OFFSET_CFUNCNAME = 552
JS_MAX_SAFE_INTEGER = 0x1fffffffffffff
JS_MIN_SAFE_INTEGER = -0x1fffffffffffff
SIZEOF_BLOB_MAX = 1000000000
SIZEOF_CFUNCNAME = 24
SIZEOF_MESSAGE = 256
SQLITE_DATATYPE_BLOB = 0x04
SQLITE_DATATYPE_FLOAT = 0x02
SQLITE_DATATYPE_INTEGER = 0x01
SQLITE_DATATYPE_INTEGER_0 = 0x00
SQLITE_DATATYPE_INTEGER_1 = 0x21
SQLITE_DATATYPE_NULL = 0x05
SQLITE_DATATYPE_SHAREDARRAYBUFFER = 0x71
SQLITE_DATATYPE_TEXT = 0x03
SQLITE_DATATYPE_TEXT_0 = 0x13
SQLITE_RESPONSETYPE_LASTBLOB = 1


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


class SqlmathDb:
    """Sqlmath database class."""

    closed = False
    filename = ""
    ptr = 0


class SqlmathError(Exception):
    """Sqlmath error."""


def asserterrorthrown(func, regexp):
    """This function will assert calling <func> throws an error."""
    err = None
    try:
        func()
    except Exception as err_caught: # noqa: BLE001
        err = err_caught
    assertorthrow(err, "No error thrown.")
    assertorthrow(not regexp or re.match(regexp, str(err)), err)


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

    def arg_serialize(argi, val):
        nonlocal baton
        if isinstance(val, (bool, int)):
            assertint64(val)
            struct.pack_into("q", baton, JSBATON_OFFSET_ARGV + argi * 8, val)
            return val
        if isinstance(val, float):
            assertorthrow(
                int(val) == val,
                f"db_call - float {val} is not an integer",
            )
            assertint64(val)
            struct.pack_into(
                "q",
                baton,
                JSBATON_OFFSET_ARGV + argi * 8,
                int(val),
            )
            return val
        if isinstance(val, str):
            baton = jsbaton_value_push(
                baton,
                argi,
                val if val.endswith("\u0000") else val + "\u0000",
            )
            return None
        if isinstance(val, memoryview):
            assertorthrow(
                val.contiguous,
                "db_call - memoryview-object must be contiguous",
            )
            return val
        return None
    assertorthrow(
        len(arglist) <= JSBATON_ARGC,
        f"db_call - len(arglist) must be less than than {JSBATON_ARGC}",
    )
    # init baton
    baton = memoryview(bytearray(1024))
    # init nalloc, nused
    struct.pack_into("i", baton, 4, JSBATON_OFFSET_ALL)
    # serialize js-value to c-value
    arglist = [arg_serialize(argi, val) for argi, val in enumerate(arglist)]
    # pad argList to length JSBATON_ARGC
    while len(arglist) < JSBATON_ARGC:
        arglist.append(0)
    # copy cFuncName into baton
    baton[
        JSBATON_OFFSET_CFUNCNAME:
        JSBATON_OFFSET_CFUNCNAME + len(bytes(cfuncname, "utf-8"))
    ] = bytes(cfuncname, "utf-8")
    # prepend baton, cfuncname to arglist
    arglist = [baton, cfuncname, *arglist]
    _pydbCall(baton, cfuncname, arglist)
    return arglist


def db_close(db):
    """This function will close sqlite-database-connection <db>."""
    # prevent segfault - do not close db if actions are pending
    if not db.closed:
        db.closed = True
        db_call(None, "_dbClose", db.ptr, db.filename)


# !! def db_exec(db, sql, bindList = []):
    # !! """This function will exec <sql> in <db> and return <result>."""
    # !! baton = jsbatonCreate()
    # !! bindByKey = !Array.isArray(bindList)
    # !! bindListLength = (
        # !! Array.isArray(bindList)
        # !! ? bindList.length
        # !! : Object.keys(bindList).length
    # !! )
    # !! externalbufferList = []
    # !! Object.entries(bindList).forEach(function ([key, val]) {
        # !! if (bindByKey) {
            # !! baton = jsbatonValuePush(baton, undefined, `:${key}\u0000`)
        # !! }
        # !! baton = jsbatonValuePush(baton, undefined, val,
        # !! externalbufferList)
    # !! })
    # !! result = await dbCallAsync(
        # !! baton,
        # !! "_dbExec",
        # !! db,                     // 0
        # !! String(sql) + "\n\nPRAGMA noop",   // 1
        # !! bindListLength,         // 2
        # !! bindByKey,              // 3
        # !! (                       // 4
            # !! responseType === "lastBlob"
            # !! ? SQLITE_RESPONSETYPE_LASTBLOB
            # !! : 0
        # !! ),
        # !! undefined, // 5
        # !! undefined, // 6
        # !! undefined, // 7 - response
        # !! ...externalbufferList        // 8
    # !! )
    # !! result = result[JSBATON_OFFSET_ARG0 + 0]
    # !! switch (responseType) {
    # !! case "arraybuffer":
    # !! case "lastBlob":
        # !! return result
    # !! case "list":
        # !! return JSON.parse(new TextDecoder().decode(result))
    # !! default:
        # !! result = JSON.parse(new TextDecoder().decode(result))
        # !! return result.map(function (rowList) {
            # !! colList = rowList.shift()
            # !! return rowList.map(function (row) {
                # !! dict = {}
                # !! colList.forEach(function (key, ii) {
                    # !! dict[key] = row[ii]
                # !! })
                # !! return dict
            # !! })
        # !! })


def db_noop(*arglist):
    """This function will do nothing except return <arglist>."""
    return db_call(None, "_dbNoop", *arglist)


def db_open(filename, flags=None):
    """
    This function will open and return sqlite-database-connection <db>.

    int sqlite3_open_v2(
        const char *filename,   /* Database filename (UTF-8) */
        sqlite3 **ppDb,         /* OUT: SQLite db handle */
        int flags,              /* Flags */
        const char *zVfs        /* Name of VFS module to use */
    )
    """
    assertorthrow(isinstance(filename, str), f"invalid filename {filename}")
    ptr = db_call(
        None,
        "_dbOpen",
        # 0. const char *filename,   Database filename (UTF-8)
        str(filename),
        # 1. sqlite3 **ppDb,         OUT: SQLite db handle
        None,
        # 2. int flags,              Flags
        flags or (
            SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
        ),
        # 3. const char *zVfs        Name of VFS module to use
        None,
    )[0]
    ptr = struct.unpack_from("q", ptr, JSBATON_OFFSET_ARGV + 0)[0]
    db = SqlmathDb()
    db.filename = filename
    db.ptr = ptr
    weakref.finalize(db, db_close, db)
    return db


def debuginline(*argv):
    """This function will print <arg> to stderr and then return <arg0>."""
    arg0 = argv[0] if argv else None
    print("\n\ndebuginline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n")
    return arg0


def jsbaton_value_push(baton, argi, val, externalbufferlist=None):
    """
    This function will push <val> to buffer <baton>.

    #define SQLITE_DATATYPE_BLOB            0x04
    #define SQLITE_DATATYPE_FLOAT           0x02
    #define SQLITE_DATATYPE_INTEGER         0x01
    #define SQLITE_DATATYPE_INTEGER_0       0x00
    #define SQLITE_DATATYPE_INTEGER_1       0x21
    #define SQLITE_DATATYPE_NULL            0x05
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
    if val == 1:
        val = True
    match (("1." if val else "0.") + type(val).__name__):
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
            vsize = 4 + len(val)
        # 19. 1.bytes
            vtype = SQLITE_DATATYPE_BLOB
            vsize = 4 + len(val)
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
            externalbufferlist.append(val)
            vtype = SQLITE_DATATYPE_SHAREDARRAYBUFFER
            vsize = 4
        # 27. 1.range
        # 28. 1.set
        # 29. 1.str
        case "1.str":
            val = bytes(val, "utf-8")
            vtype = SQLITE_DATATYPE_TEXT
            vsize = 4 + len(val)
        # 30. 1.tuple
        # 31. json
        case _:
            val = bytes(json.dumps(val), "utf-8")
            vtype = SQLITE_DATATYPE_TEXT
            vsize = 4 + len(val)
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
        # update nallc
        struct.pack_into("i", baton, 0, len(baton))
        # copy old-baton into new-baton
        baton[:len(tmp)] = tmp
    # push vtype
    struct.pack_into("b", baton, nused, vtype)
    # update nused
    struct.pack_into("i", baton, 4, nused + 1 + vsize)
    # handle blob-value
    if vtype in (SQLITE_DATATYPE_BLOB, SQLITE_DATATYPE_TEXT):
        # set argv[ii] to blob/text location
        if argi is not None:
            struct.pack_into("i", baton, JSBATON_OFFSET_ARGV + argi * 8, nused)
        vsize -= 4
        assertorthrow(
            0 <= vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                f" 0 to {SIZEOF_BLOB_MAX}"
            ),
        )
        # push vsize
        struct.pack_into("i", baton, nused + 1, vsize)
        # copy val into baton
        baton[nused + 1 + 4:nused + 1 + 4 + vsize] = val
        return baton
    if vtype == SQLITE_DATATYPE_FLOAT:
        struct.pack_into("d", baton, nused + 1, val)
        return baton
    if vtype == SQLITE_DATATYPE_INTEGER:
        assertint64(val)
        struct.pack_into("q", baton, nused + 1, val)
        return baton
    if vtype == SQLITE_DATATYPE_SHAREDARRAYBUFFER:
        vsize = len(val)
        assertorthrow(
            0 <= vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                f" 0 to {SIZEOF_BLOB_MAX}"
            ),
        )
        # push vsize
        struct.pack_into("i", baton, nused + 1, vsize)
        return baton
    return baton


def jsbaton_value_string(baton, argi):
    """This function will return string-value from <baton>."""
    """at given <offset>."""
    offset = struct.unpack_from("q", baton, JSBATON_OFFSET_ARGV + argi * 8)[0]
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
