"""
setup.py.

python -m build --wheel
rm -rf build/ && python setup.py build_ext -i
"""

# import setuptools before distutils
import setuptools # noqa=I001

import distutils.ccompiler
import distutils.sysconfig
import os
import pathlib
import platform
import shutil
import subprocess
import sys
import unittest


def build_ext():
    """This function will build extension."""
    subprocess.run([
        "sh",
        "jslint_ci.sh",
        "shCiBuildPythonSrc",
    ])
    # Work around clang raising hard error for unused arguments
    if sys.platform == "darwin":
        os.environ["CFLAGS"] = "-Qunused-arguments"
# https://github.com/pypa/distutils/blob/main/distutils/command/build_clib.py
    compiler = distutils.ccompiler.new_compiler(
        compiler=None,
        dry_run=0,
        force=0,
        plat=None,
        verbose=2,
    )
    distutils.sysconfig.customize_compiler(compiler)
    # Make sure Python's include directories (for Python.h, pyconfig.h,
    # etc.) are in the include search path.
    py_include = distutils.sysconfig.get_python_inc()
    plat_py_include = distutils.sysconfig.get_python_inc(plat_specific=1)
    include_dirs = []
    # If in a virtualenv, add its include directory
    # Issue 16116
    if sys.exec_prefix != sys.base_exec_prefix:
        include_dirs.append(pathlib.Path(sys.exec_prefix) / "include")
    # Put the Python "system" include dir at the end, so that
    # any local include dirs take precedence.
    include_dirs.extend(py_include.split(os.path.pathsep))
    if plat_py_include != py_include:
        include_dirs.extend(plat_py_include.split(os.path.pathsep))
    compiler.set_include_dirs(include_dirs)
    # build static-library sqlite3_rollup.a
    compiler.define_macro("SQLITE3_C2", "")
    extra_link_args = []
    extra_postargs = []
    if sys.platform == "win32":
        # bugfix - LINK : warning LNK4098: defaultlib 'LIBCMT'
        # conflicts with use of other libs; use /NODEFAULTLIB:library
        extra_postargs += [
            "/MT",
            "/O2",
            "/W4",
            "/wd4131",
        ]
    else:
        extra_link_args += ["-lm"]
        extra_postargs += [
            "-DHAVE_UNISTD_H",
            "-O3",
            "-Wall",
        ]
    sources = [
        ".SRC_SHELL.c",
        ".SRC_ZLIB.c",
        "src_extension_functions.c",
        "sqlite3_rollup.c",
        "sqlmath_base.c",
        "sqlmath_custom.c",
    ]
    objects = compiler.compile(
        sources,
        debug=0,
        depends=[*sources, "setup.py"],
        extra_postargs=extra_postargs,
        extra_preargs=None,
        output_dir=".tmp",
    )
    compiler.create_static_lib(
        objects=[obj for obj in objects if not obj.startswith(".src_shell.")],
        output_libname="sqlite3_rollup",
        debug=0,
        output_dir=".tmp",
        target_lang=None,
    )
    # build executable
    compiler.link_executable(
        objects=objects,
        output_progname=(
            f"_binary_sqlmath_shell_{platform.system()}_{platform.machine()}"
        ).lower(),
        output_dir=None,
        libraries=None,
        library_dirs=None,
        runtime_library_dirs=None,
        debug=0,
        extra_preargs=None,
        extra_postargs=extra_link_args,
        target_lang=None,
    )
    # build c-extension
    if sys.platform == "win32":
        extra_objects = []
        libraries = [".tmp/sqlite3_rollup"]
    else:
        shutil.copyfile(
            compiler.library_filename(
                libname="sqlite3_rollup",
                lib_type="static",
                strip_dir=0,
                output_dir=".tmp",
            ),
            ".tmp/sqlite3_rollup.lib",
        )
        extra_objects = [".tmp/sqlite3_rollup.lib"]
        libraries = []
# https://github.com/pypa/distutils/blob/main/distutils/command/build_ext.py
    build_ext_option = {
        "define_macros": [],
        "depends": [],
        "export_symbols": None,
        "extra_compile_args": extra_postargs,
        "extra_link_args": extra_link_args,
        "extra_objects": extra_objects,
        "include_dirs": [],
        "language": None,
        "libraries": libraries,
        "library_dirs": [],
        "name": "_sqlite3",
        "optional": None,
        "runtime_library_dirs": None,
        "sources": [
            ".SRC_PYTHON_DBAPI2.c",
        ],
        "swig_opts": None,
        "undef_macros": None,
    }
    subprocess.run(["rm", "-f", "*.pyd"]).check_returncode()
    setuptools.setup(
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
            build_ext()
        case "test":
            test_dbapi2_run()
        case _:
            setuptools.setup()
