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

__version__ = "2024.1.21"
__version_info__ = ("2024", "1", "21")

import json
import math
import re
import struct
import sys
import weakref

from ._sqlmath import _pybatonSetMemoryview, _pybatonStealCbuffer, _pydbCall

JSBATON_ARGC = 8
JSBATON_OFFSET_ALL = 256
JSBATON_OFFSET_ARGV = 128
JSBATON_OFFSET_FUNCNAME = 8
SIZEOF_BLOB_MAX = 1_000_000_000
SQLITE_DATATYPE_BLOB = 0x04
SQLITE_DATATYPE_EXTERNALBUFFER = 0x71
SQLITE_DATATYPE_FLOAT = 0x02
SQLITE_DATATYPE_INTEGER = 0x01
SQLITE_DATATYPE_INTEGER_0 = 0x00
SQLITE_DATATYPE_INTEGER_1 = 0x21
SQLITE_DATATYPE_NULL = 0x05
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


INFINITY = float("inf")
NAN = float("nan")


class SqlmathDb:
    """Sqlmath database class."""

    closed = False
    filename = ""
    ptr = 0


class SqlmathError(Exception):
    """Sqlmath error."""


def assert_error_thrown(func, regexp):
    """This function will assert calling <func> throws an error."""
    err = None
    try:
        func()
    except Exception as err_caught: # noqa: BLE001
        err = err_caught
    assert_or_throw(err, "No error thrown.")
    assert_or_throw(not regexp or re.search(regexp, str(err)), err)


def assert_int64(val):
    """This function will assert <val> is within range."""
    """of c99-signed-long-long."""
    assert_or_throw(
        -9_223_372_036_854_775_808 <= val <= 9_223_372_036_854_775_807, # noqa: PLR2004
        (
            f"integer {val} outside signed-64-bit inclusive-range"
            " -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807"
        ),
    )


def assert_json_equal(aa, bb, message=None):
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


def assert_or_throw(condition, message):
    """This function will throw <message> if <condition> is falsy."""
    if not condition:
        raise (
            SqlmathError(str(message)[:2048])
            if not message or isinstance(message, str)
            else message
        )


def db_call(baton, arglist):
    """This function will call c-function dbXxx() with given <funcname>."""
    """and return [<baton>, ...arglist]."""
    assert_or_throw(
        len(arglist) <= JSBATON_ARGC,
        f"db_call - len(arglist) must be less than than {JSBATON_ARGC}",
    )
    # serialize js-value to c-value
    for argi, val in enumerate(arglist):
        if val is None:
            # ctype-q = long-long
            struct.pack_into("q", baton, JSBATON_OFFSET_ARGV + argi * 8, 0)
            continue
        if isinstance(val, (bool, float, int)):
            val2 = val
            if isinstance(val, float):
                assert_or_throw(
                    int(val) == val,
                    f"db_call - float {val} is not an integer",
                )
                val2 = int(val)
            assert_int64(val2)
            # ctype-q = long-long
            struct.pack_into("q", baton, JSBATON_OFFSET_ARGV + argi * 8, val2)
            continue
        if isinstance(val, str):
            baton = jsbaton_set_value(
                baton,
                argi,
                val if val.endswith("\u0000") else val + "\u0000",
                None,
                None,
            )
            continue
        msg = f'db_call - invalid arg-type "{type(val)}"'
        raise SqlmathError(msg)
    _pydbCall(baton)
    return [baton]


def db_close(db):
    """This function will close sqlite-database-connection <db>."""
    # prevent segfault - do not close db if actions are pending
    if not db.closed:
        db.closed = True
        db_call(jsbaton_create("_dbClose"), [db.ptr, db.filename])


def db_exec(
    bind_list=None,
    db=None,
    response_type=None,
    sql=None,
):
    """This function will exec <sql> in <db> and return <result>."""
    if bind_list is None:
        bind_list = []
    baton = jsbaton_create("_dbExec")
    bind_by_key = callable(getattr(bind_list, "items", None))
    bufi = [0]
    externalbuffer_list = []
    if bind_by_key:
        for key, val in bind_list.items():
            baton = jsbaton_set_value(baton, None, f":{key}\u0000", None, None)
            baton = jsbaton_set_value(
                baton,
                None,
                val,
                bufi,
                externalbuffer_list,
            )
    else:
        for val in bind_list:
            baton = jsbaton_set_value(
                baton,
                None,
                val,
                bufi,
                externalbuffer_list,
            )
    db_call(
        baton,
        [
            # 0. db
            db.ptr,
            # 1. sql
            str(sql) + "\n;\nPRAGMA noop",
            # 2. len(bind_list)
            len(bind_list),
            # 3. bind_by_key
            bind_by_key,
            # 4. response_type
            (
                SQLITE_RESPONSETYPE_LASTBLOB
                if response_type == "lastblob"
                else 0
            ),
        ],
    )
    match response_type:
        case "arraybuffer":
            return memoryview(_pybatonStealCbuffer(baton, 0, 0))
        case "lastblob":
            return memoryview(_pybatonStealCbuffer(baton, 0, 0))
        case "list":
            return json.loads(_pybatonStealCbuffer(baton, 0, 1))
        case _:
            table_list = []
            for table in json.loads(_pybatonStealCbuffer(baton, 0, 1)):
                col_list = tuple(enumerate(table.pop(0)))
                table_list.append([
                    {key: row[ii] for ii, key in col_list}
                    for row in table
                ])
            return table_list


def db_exec_and_return_lastblob(
    bind_list=None,
    db=None,
    sql=None,
):
    """This function will exec <sql> in <db> and return last value."""
    """ retrieved. from execution as raw blob/buffer."""
    return db_exec(
        bind_list=bind_list,
        db=db,
        response_type="lastblob",
        sql=sql,
    )


def db_file_load(
    db=None,
    filename=None,
    mode_save=0,
):
    """This function will load <filename> to <db>."""
    assert_or_throw(
        isinstance(filename, str) and filename,
        f"invalid filename {filename}",
    )
    db_call(
        jsbaton_create("_dbFileLoad"),
        [
            # 0. sqlite3 * pInMemory
            db.ptr,
            # 1. char *zFilename
            filename,
            # 2. const int isSave
            mode_save,
        ],
    )


def db_file_save(
    db=None,
    filename=None,
):
    """This function will save <db> to <filename>."""
    return db_file_load(
        db=db,
        filename=filename,
        mode_save=1,
    )


def db_noop(*arglist):
    """This function will do nothing except return <arglist>."""
    return db_call(jsbaton_create("_dbNoop"), arglist)


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
    assert_or_throw(isinstance(filename, str), f"invalid filename {filename}")
    ptr = db_call(
        jsbaton_create("_dbOpen"),
        [
            # 0. const char *filename,   Database filename (UTF-8)
            filename,
            # 1. sqlite3 **ppDb,         OUT: SQLite db handle
            None,
            # 2. int flags,              Flags
            (
                SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
                if flags is None
                else flags
            ),
            # 3. const char *zVfs        Name of VFS module to use
            None,
        ],
    )[0]
    # ctype-q = long-long
    ptr = struct.unpack_from("q", ptr, JSBATON_OFFSET_ARGV + 0)[0]
    db = SqlmathDb()
    db.filename = filename
    db.ptr = ptr
    weakref.finalize(db, db_close, db)
    return db


def debuginline(*argv):
    """This function will print <argv> to stderr and then return <argv>[0]."""
    arg0 = argv[0] if argv else None
    print("\n\ndebuginline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n", file=sys.stderr)
    return arg0


def jsbaton_create(funcname):
    """This function will create buffer <baton>."""
    baton = memoryview(bytearray(1024))
    # init nalloc, nused
    struct.pack_into("i", baton, 4, JSBATON_OFFSET_ALL) # ctype-i = int
    # copy funcname into baton
    funcname = bytes(funcname, "utf-8")
    baton[
        JSBATON_OFFSET_FUNCNAME: JSBATON_OFFSET_FUNCNAME + len(funcname)
    ] = funcname
    return baton


def jsbaton_get_int64(baton, argi):
    """This function will return int64-value from <baton> at <argi>."""
    # ctype-q = long-long
    return struct.unpack_from("q", baton, JSBATON_OFFSET_ARGV + argi * 8)[0]


def jsbaton_get_string(baton, argi):
    """This function will return string-value from <baton> at <argi>."""
    # ctype-q = long-long
    offset = struct.unpack_from("q", baton, JSBATON_OFFSET_ARGV + argi * 8)[0]
    return str(
        baton[
            offset + 1 + 4:
            # remove null-terminator from string
            # ctype-i = int
            offset + 1 + 4 + struct.unpack_from("i", baton, offset + 1)[0] - 1
        ],
        "utf-8",
    )


def jsbaton_set_value(baton, argi, val, bufi, reference_list):
    """
    This function will push <val> to buffer <baton>.

    #define SQLITE_DATATYPE_BLOB            0x04
    #define SQLITE_DATATYPE_EXTERNALBUFFER          0x71
    #define SQLITE_DATATYPE_FLOAT           0x02
    #define SQLITE_DATATYPE_INTEGER         0x01
    #define SQLITE_DATATYPE_INTEGER_0       0x00
    #define SQLITE_DATATYPE_INTEGER_1       0x21
    #define SQLITE_DATATYPE_NULL            0x05
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

    Format  C Type    Python type Standard size
    x       pad       byte        no value
    c       char      byte        1
    b       signed    char        integer    1
    B       unsigned  char        integer    1
    ?       _Bool     bool        1
    h       short     integer     2
    H       unsigned  short       integer    2
    i       int       integer     4
    I       unsigned  int         integer    4
    l       long      integer     4
    L       unsigned  long        integer    4
    q       long long integer     8
    Q       unsigned  long long   integer    8
    n       ssize_t   integer
    N       size_t    integer
    e       half      float       2
    f       float     float       4
    d       double    float       8
    s       char[]    bytes
    p       char[]    bytes
    P       void*     integer
    """
    # 17. 1.bool
    if val == 1:
        val = True
    match ("1." if val else "0.") + type(val).__name__:
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
        case "1.bytearray":
            vtype = SQLITE_DATATYPE_BLOB
            vsize = 4 + len(val)
        # 19. 1.bytes
        case "1.bytes":
            vtype = SQLITE_DATATYPE_BLOB
            vsize = 4 + len(val)
        # 20. 1.complex
        # 21. 1.dict
        # 22. 1.float
        case "1.float":
            if math.isnan(val):
                vtype = SQLITE_DATATYPE_NULL
                vsize = 0
            else:
                vtype = SQLITE_DATATYPE_FLOAT
                vsize = 8
        # 23. 1.frozenset
        # 24. 1.int
        case "1.int":
            vtype = SQLITE_DATATYPE_INTEGER
            vsize = 8
        # 25. 1.list
        # 26. 1.memoryview
        case "1.memoryview":
            vtype = SQLITE_DATATYPE_EXTERNALBUFFER
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
    nused = struct.unpack_from("i", baton, 4)[0] # ctype-i = int
    nn = nused + 1 + vsize
    assert_or_throw(
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
        struct.pack_into("i", baton, 0, len(baton)) # ctype-i = int
        # copy old-baton into new-baton
        baton[:len(tmp)] = tmp
    # push vtype - 1-byte
    struct.pack_into("b", baton, nused, vtype) # ctype-b = signed-char
    # update nused
    struct.pack_into("i", baton, 4, nused + 1 + vsize) # ctype-i = int
    # handle blob-value
    if vtype in (SQLITE_DATATYPE_BLOB, SQLITE_DATATYPE_TEXT):
        # set argv[ii] to blob/text location
        if argi is not None:
            # ctype-i = int
            struct.pack_into("i", baton, JSBATON_OFFSET_ARGV + argi * 8, nused)
        vsize -= 4
        assert_or_throw(
            0 <= vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                f" 0 to {SIZEOF_BLOB_MAX}"
            ),
        )
        # push vsize - 4-byte
        struct.pack_into("i", baton, nused + 1, vsize) # ctype-i = int
        # push SQLITE-BLOB/TEXT - vsize-byte
        baton[nused + 1 + 4:nused + 1 + 4 + vsize] = val
        return baton
    if vtype == SQLITE_DATATYPE_EXTERNALBUFFER:
        vsize = len(val)
        assert_or_throw(
            0 <= vsize <= SIZEOF_BLOB_MAX,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                f" 0 to {SIZEOF_BLOB_MAX}"
            ),
        )
        assert_or_throw(
            bufi[0] < JSBATON_ARGC,
            f"cannot pass more than {JSBATON_ARGC} arraybuffers",
        )
        # push externalbuffer - 4-byte
        struct.pack_into("i", baton, nused + 1, bufi[0]) # ctype-i = int
        # set buffer
        _pybatonSetMemoryview(baton, bufi[0], val)
        # increment bufi
        bufi[0] += 1
        # add buffer to reference_list to prevent gc during db_call.
        reference_list.append(val)
        return baton
    if vtype == SQLITE_DATATYPE_FLOAT:
        # push SQLITE-REAL - 8-byte
        struct.pack_into("d", baton, nused + 1, val) # ctype-d = double
        return baton
    if vtype == SQLITE_DATATYPE_INTEGER:
        assert_int64(val)
        # push SQLITE-INTEGER - 8-byte
        struct.pack_into("q", baton, nused + 1, val) # ctype-q = long-long
        return baton
    return baton


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
