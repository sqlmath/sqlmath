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
    aa = json.dumps(aa, indent=1)
    bb = json.dumps(bb, indent=1)
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


def assertorthrow(condition, message):
    """This function will throw <message> if <condition> is falsy."""
    if not condition:
        raise (
            SqlmathError(str(message)[:2048])
            if not message or isinstance(message, str)
            else message
        )


def c_call(baton, c_func_name, *arglist):
    """This function will serialize <arglist> to a c <baton>."""
    """suitable for passing into napi."""

    def arg_serialize(argi, value):
        nonlocal baton
        if isinstance(value, (bool, int)):
            struct.pack_into("q", baton, 8 + argi * 8, value)
        elif isinstance(value, float):
            assertorthrow(
                int(value) == value,
                f"c_call - float {value} is not an integer",
            )
            struct.pack_into("q", baton, 8 + argi * 8, int(value))
        elif isinstance(value, str):
            baton = jsbaton_value_push(
                baton,
                argi,
                value if value.endswith("\u0000") else value + "\u0000",
            )
    assertorthrow(
        len(arglist) < JSBATON_ARGC,
        f"c_call - arglist.length must be less than than {JSBATON_ARGC}",
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
    # encode c_func_name into baton
    baton = jsbaton_value_push(baton, 2 * JSBATON_ARGC, f"{c_func_name}\u0000")
    arglist = [baton, c_func_name, *arglist]
    _pybatonCall(baton, c_func_name, arglist)
    return arglist


def db_noop(*arglist):
    """This function will do nothing except return <arglist>."""
    return c_call(None, "_dbNoop", *arglist)


def db_open(*arglist):
    """This function will open and return sqlite-database-connection <db>."""
    c_call(None, "_dbNoop", *arglist)


def debuginline(*argv):
    """This function will print <arg> to stderr and then return <arg0>."""
    arg0 = argv[0] if argv else None
    print("\n\ndebuginline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n")
    return arg0


def jsbaton_value_push(baton, argi, value):
    """This function will push <value> to buffer <baton>."""
    noop(argi)
    match (f"{bool(value)}.{type(value)}"):
        case "false.str":
            vtype = SQLITE_DATATYPE_TEXT_0
            vsize = 0
        # case "true.str":
        case _:
            # 14. true.string
            value = bytes(value, "utf-8")
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
