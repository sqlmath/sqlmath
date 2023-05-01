"""
setup.py.

python -m build --wheel
rm -rf build/ && python setup.py build_ext -i
"""

import os
import subprocess
import sys
import unittest

import setuptools


def build_ext():
    """This function will build extension."""
    # Work around clang raising hard error for unused arguments
    if sys.platform == "darwin":
        os.environ["CFLAGS"] = "-Qunused-arguments"
    # https://setuptools.pypa.io/en/latest/userguide/ext_modules.html
    build_ext_option = {
        "define_macros": [
            ("MODULE_NAME", '"sqlmath"'),
            ("SQLMATH_PYTHON_C2", ""),
        ],
        "depends": [
            "sqlmath_dbapi2.py",
        ],
        "extra_compile_args": [],
        "extra_link_args": [],
        "extra_objects": [],
        "include_dirs": [],
        "library_dirs": [],
        "libraries": [],
        "name": "_sqlite3",
        "sources": [
            "sqlite3_rollup.c",
        ],
    }
    if sys.platform == "win32":
        build_ext_option["libraries"] += [
            ".tmp/build/Release/sqlite3_c",
            ".tmp/build/Release/sqlmath_c",
            ".tmp/build/Release/zlib_c",
        ]
        # bugfix - LINK : warning LNK4098: defaultlib 'LIBCMT'
        # conflicts with use of other libs; use /NODEFAULTLIB:library
        build_ext_option["extra_compile_args"] += [
            "/MT",
            "/W4",
        ]
    else:
        build_ext_option["extra_objects"] += [
            ".tmp/build/Release/sqlite3_c.a",
            ".tmp/build/Release/sqlmath_c.a",
            ".tmp/build/Release/zlib_c.a",
        ]
        build_ext_option["define_macros"] += []
        build_ext_option["extra_compile_args"] += [
            "-Wall",
        ]
    subprocess.run(["rm", "-f", "*.pyd"]).check_returncode()
    setuptools.setup(
        description="sqlite for datascience",
        ext_modules=[
            setuptools.Extension(**build_ext_option),
        ],
        package_dir={".": "."},
    )


def debugInline(*argv): # noqa=N802
    """This function will print <arg> to stderr and then return <arg0>."""
    arg0 = argv[0] if argv else None
    print("\n\ndebugInline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n")
    return arg0


def noop(arg=None):
    """This function will do nothing."""
    return arg


def test_dbapi2_run():
    """This function will run dbapi2 tests."""
    import sqlmath_dbapi2
    for test_suite in sqlmath_dbapi2.test_suite_list:
        results = unittest.TextTestRunner(
            verbosity=1,
            failfast=False,
        ).run(test_suite())
        if results.failures or results.errors:
            sys.exit(1)


if __name__ == "__main__":
    # debug sys.argv
    print(sys.argv)
    match sys.argv[1]:
        case "build_ext":
            build_ext()
        case "bdist_wheel":
            subprocess.run(["npm", "run", "test2"]).check_returncode()
        case "test":
            test_dbapi2_run()
