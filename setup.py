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
    """This function will build c-extension."""
    for ii in [1, 2, 3]:
        subprocess.run(
            ["sh", "jslint_ci.sh", "shCiBuildStep", str(ii)],
        ).check_returncode()
    compiler = new_compiler()
    objects = [
        "./build/.SRC_SQLITE.obj",
        "./build/.SRC_ZLIB.obj",
        "./build/sqlite3_extension_functions.obj",
        #
        "./build/.SQLMATH_BASE.obj",
        #
        "./build/.SQLMATH_CUSTOM.obj",
    ]
    # build executable
    print(f"\npython setup.py - link_executable - {objects}")
    if sys.platform == "win32":
        extra_link_args = []
    else:
        extra_link_args = compiler.extra_postargs
    compiler.link_executable(
        objects=[*objects, "./build/.SRC_SHELL.obj"],
        #
        output_progname=(
            f"_sqlite3.shell_{platform.system()}_{platform.machine()}"
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
# https://github.com/pypa/distutils/blob/main/distutils/command/build_ext.py
    print(f"\npython setup.py - build_ext - {objects}")
    subprocess.run(["rm", "-f", "*.pyd"]).check_returncode()
    setuptools.setup(
        ext_modules=[setuptools.Extension(
            define_macros=[],
            depends=[],
            export_symbols=None,
            extra_compile_args=compiler.extra_postargs,
            extra_link_args=extra_link_args,
            extra_objects=[*objects, "./build/.SRC_PYTHON.obj"],
            include_dirs=[],
            language=None,
            libraries=None,
            library_dirs=[],
            name="_sqlite3",
            optional=None,
            runtime_library_dirs=None,
            sources=[],
            swig_opts=None,
            undef_macros=None,
        )],
        package_dir={".": "."},
    )
    # create static-library ./build/sqlite3_rollup.lib
    print(f"\npython setup.py - create_static_lib - {objects}")
    compiler.create_static_lib(
        objects=objects,
        # !! objects=[*objects, "./build/.SQLMATH_NODEJS.obj"],
        #
        output_libname="sqlite3_rollup",
        debug=0,
        output_dir="./build",
        target_lang=None,
    )
    library_filename = compiler.library_filename(
        libname="sqlite3_rollup",
        lib_type="static",
        strip_dir=0,
        output_dir="./build",
    ).replace("\\", "/")
    if library_filename != "./build/sqlite3_rollup.lib":
        shutil.move(library_filename, "./build/sqlite3_rollup.lib")


def build_ext_compile():
    """This function will compile <sources> into static-objects."""
    compiler = new_compiler()
    objects = compiler.compile(
        sources=sys.argv[2].strip().split(" "),
        #
        debug=0,
        depends=None,
        extra_postargs=compiler.extra_postargs,
        extra_preargs=None,
        output_dir="./build",
    )
    objects2 = [obj.replace(compiler.obj_extension, ".obj") for obj in objects]
    for aa, bb in zip(objects, objects2, strict=True):
        if (aa != bb):
            shutil.move(aa, bb)
    return objects2


def debugInline(*argv): # noqa=N802
    """This function will print <arg> to stderr and then return <arg0>."""
    arg0 = argv[0] if argv else None
    print("\n\ndebugInline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n")
    return arg0


def new_compiler():
    """This function create new compiler."""
    # Work around clang raising hard error for unused arguments
    # if sys.platform == "darwin":
    #     os.environ["CFLAGS"] = "-Qunused-arguments"
# https://github.com/pypa/distutils/blob/main/distutils/command/build_clib.py
    compiler = distutils.ccompiler.new_compiler(
        compiler=None,
        dry_run=0,
        force=0,
        plat=None,
        verbose=1,
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
    # init extra_postargs
    compiler.define_macro("SQLITE3_C2", "")
    compiler.define_macro("_REENTRANT", "1")
    if sys.platform == "win32":
        compiler.extra_postargs = [
            # bugfix - LINK : warning LNK4098: defaultlib 'LIBCMT'
            # conflicts with use of other libs; use /NODEFAULTLIB:library
            "/MT",
            "/W4",
            "/wd4131",
        ]
    else:
        compiler.extra_postargs = [
            "-DHAVE_UNISTD_H",
            "-Wall",
            "-lm",
        ]
    return compiler


def noop(arg=None):
    """This function will do nothing."""
    return arg


def test_python_run():
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
    print(f"""\npython {" ".join(sys.argv)}""")
    match sys.argv[1]:
        case "build_ext":
            build_ext()
        case "build_ext_compile":
            build_ext_compile()
        case "bdist_wheel":
            build_ext()
        case "test":
            test_python_run()
        case _:
            setuptools.setup()
