"""
setup.py.

npm_config_mode_debug2=1 python setup.py build_ext && python setup.py test
python -m build
"""

import asyncio
import json
import os
import pathlib
import re
import subprocess
import sys
import sysconfig

import setuptools
import setuptools.command.build_ext
import setuptools.command.install_lib


def build_ext():
    """This function will build c-extension."""
    build_ext_init()
    subprocess.run(["python", "setup.py", "build_ext_async"], check=True)
    setuptools.setup(ext_modules=[setuptools.Extension("_sqlmath", [])])


async def build_ext_async(): # noqa=C901
    """This function will build c-extension."""

    async def build_ext_exe(cdefine):
        arg_list = []
        file_exe = f"build/{cdefine}.exe"
        file_obj_list = [f"build/{cdefine}.obj", "build/SRC_ZLIB_BASE.obj"]
        match cdefine:
            case "SRC_SQLITE_SHELL":
                file_exe = (
                    ".".join(
                        file_lib
                        .replace("_sqlmath", "_sqlmath_shell")
                        .split(".")[:-1],
                    )
                    + (".exe" if is_win32 else "")
                )
                file_obj_list = [
                    "build/SRC_ZLIB_BASE.obj",
                    #
                    "build/SRC_SQLITE_BASE.obj",
                    "build/SRC_SQLITE_SHELL.obj",
                    #
                    "build/SQLMATH_BASE.obj",
                    "build/SQLMATH_CUSTOM.obj",
                ]
        if cdefine != "SRC_SQLITE_PYTHON" and pathlib.Path(file_exe).exists():
            return
        if is_win32:
            arg_list = [
                exe_link,
                #
                *file_obj_list,
                #
                "/INCREMENTAL:NO", # optimization - reduce filesize
                "/LTCG", # from cl.exe /GL
                "/MANIFEST:EMBED",
                "/MANIFESTUAC:NO",
                #
                f"/OUT:{file_exe}",
                "/nologo",
            ]
        else:
            arg_list = [
                cc_compiler,
                #
                *file_obj_list,
                #
                "-lm", # link math
                "-o", file_exe,
            ]
        print(f"build_ext - link {file_exe}")
        await create_subprocess_exec_and_check(
            *arg_list,
            env=env,
            stdout=subprocess.DEVNULL if npm_config_mode_debug else None,
        )

    async def build_ext_obj(cdefine):
        file_obj = f"build/{cdefine}.obj"
        match cdefine:
            case "SQLMATH_BASE":
                pass
            case "SRC_SQLITE_PYTHON":
                pass
            case _:
                if pathlib.Path(file_obj).exists():
                    return
        file_src = f"build/{cdefine}.c"
        arg_list = [
            *[f"-I{path}" for path in path_include],
            #
            f"-D{cdefine}_C2=",
            "-DSRC_SQLITE_BASE_C2=",
            "-D_REENTRANT=1",
        ]
        if not npm_config_mode_debug and is_win32:
            arg_list += ["/W1"]
        elif not npm_config_mode_debug:
            arg_list += [
                "-Wno-all",
                "-Wno-implicit-fallthrough",
                "-Wno-unused-parameter",
            ]
        elif is_win32:
            arg_list += ["/W3"]
        else:
            arg_list += ["-Wextra"]
# https://github.com/nodejs/node-gyp/blob/v9.3.1/gyp/pylib/gyp/MSVSSettings.py
        if is_win32:
            arg_list = [
                exe_cl,
                *arg_list,
                #
                "/GL", # to link.exe /LTCG
                "/MT", # multithreaded, statically-linked
                "/O2",
                #
                "/c", f"/Tc{file_src}",
                f"/Fo{file_obj}",
                "/nologo",
            ]
        else:
            arg_list = [
                cc_compiler,
                *arg_list,
                #
                *cc_ccshared.strip().split(" "),
                *cc_cflags.strip().split(" "),
                #
                "-DHAVE_UNISTD_H=",
                "-c", file_src,
                "-o", file_obj,
            ]
        print(f"build_ext - compile {file_obj}")
        await create_subprocess_exec_and_check(
            *arg_list,
            env=env,
            stdout=subprocess.DEVNULL if npm_config_mode_debug else None,
        )

    async def create_subprocess_exec_and_check(*args, **kwds):
        child = await asyncio.create_subprocess_exec(*args, **kwds)
        await child.communicate()
        if child.returncode != 0:
            raise subprocess.SubprocessError("returncode=" + child.returncode)
    #
    # build_ext - update version
    with pathlib.Path("package.json").open() as file1:
        package_json = json.load(file1)
        version = package_json["version"].split("-")[0]
    for filename in [
        "README.md",
        "sqlmath/__init__.py",
    ]:
        with pathlib.Path(filename).open("r+", newline="\n") as file1:
            data0 = file1.read()
            data1 = data0
            # update version - README.md
            data1 = re.sub(
                "(sqlmath(?:-|==))\\d\\d\\d\\d\\.\\d\\d?\\.\\d\\d?",
                f"\\g<1>{version}",
                data1,
            )
            # update version - sqlmath/__init__.py
            data1 = re.sub(
                "__version__ = .*",
                f'__version__ = "{version}"',
                data1,
            )
            data1 = re.sub(
                "__version_info__ = .*",
                (
                    '__version_info__ = ("'
                    + '", "'.join(version.split("."))
                    + '")'
                ),
                data1,
            )
            if package_json["name"] == "sqlmath" and data1 != data0:
                print(f"build_ext - update file {file1.name}")
                file1.seek(0)
                file1.write(data1)
                file1.truncate()
    #
    # build_ext - init sysconfig
    cc_ccshared = sysconfig.get_config_var("CCSHARED") or ""
    cc_cflags = sysconfig.get_config_var("CFLAGS") or ""
    cc_compiler = sysconfig.get_config_var("CC")
    cc_ldflags = sysconfig.get_config_var("LDFLAGS") or ""
    cc_ldshared = sysconfig.get_config_var("LDSHARED") or ""
    dir_wheel = f"build/bdist.{sysconfig.get_platform()}/wheel/sqlmath"
    file_lib = f"_sqlmath{sysconfig.get_config_var('EXT_SUFFIX')}"
    is_win32 = sys.platform == "win32"
    path_include = [
        sysconfig.get_path("platinclude"),
        sysconfig.get_path("include"),
    ]
    path_prefix = sysconfig.get_config_var("prefix")
    path_prefix_base = sys.base_exec_prefix
    path_library = [
        f"{sysconfig.get_config_var('prefix')}{os.sep}libs",
        sysconfig.get_config_var("prefix"),
    ]
    platform_vcvarsall = {
        "win-amd64": "x86_amd64",
        "win-arm32": "x86_arm",
        "win-arm64": "x86_arm64",
        "win32": "x86",
    }.get(sysconfig.get_platform())
    npm_config_mode_debug = os.environ.get("npm_config_mode_debug") # noqa=SIM112
    #
    # build_ext - init env
    env = os.environ
    if is_win32:
        env = await asyncio.create_subprocess_exec(
            (
                (
                    os.environ.get("PROGRAMFILES(X86)")
                    or os.environ.get("PROGRAMFILES")
                )
                + "\\Microsoft Visual Studio"
                + "\\Installer"
                + "\\vswhere.exe"
            ),
            "-latest", "-prerelease",
            "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-property", "installationPath",
            "-products", "*",
            stdout=asyncio.subprocess.PIPE,
        )
        env = (
            await env.stdout.readline()
        ).decode("mbcs").strip()
        env = await asyncio.create_subprocess_exec(
            "cmd.exe",
            "/u",
            "/c",
            f"{env}\\VC\\Auxiliary\\Build\\vcvarsall.bat",
            platform_vcvarsall,
            "&&",
            "set",
            stdout=asyncio.subprocess.PIPE,
        )
        env = (
            await env.stdout.read()
        ).decode("utf-16le")
        env = {
            key.lower(): val
            for key, _, val in
            (line.partition("=") for line in env.splitlines())
            if key and val
        }
        await_list = []
        for exe in ["cl.exe", "link.exe"]:
            await_list.append( # noqa=PERF401
                (
                    await asyncio.create_subprocess_exec(
                        *["where", exe],
                        env=env,
                        stdout=asyncio.subprocess.PIPE,
                    )
                ).stdout.readline(),
            )
        [exe_cl, exe_link] = [
            exe.splitlines()[0] for exe in await asyncio.gather(*await_list)
        ]
    #
    # build_ext - virtualenv
    for arr in [path_include, path_library]:
        for path in arr:
            if path_prefix_base != path_prefix:
                arr.append(path.replace(path_prefix, path_prefix_base)) # noqa=PERF401
    #
    # build_ext - compile .obj file
    await asyncio.gather(*[
        build_ext_obj(cdefine)
        for cdefine in [
            "SRC_ZLIB_BASE",
            "SRC_ZLIB_TEST_EXAMPLE",
            "SRC_ZLIB_TEST_MINIGZIP",
            #
            "SRC_SQLITE_BASE",
            "SRC_SQLITE_PYTHON",
            "SRC_SQLITE_SHELL",
            #
            "SQLMATH_BASE",
            "SQLMATH_CUSTOM",
        ]
    ])
    #
    # build_ext - compile .exe file
    await asyncio.gather(*[
        build_ext_exe(cdefine)
        for cdefine in [
            "SRC_ZLIB_TEST_EXAMPLE",
            "SRC_ZLIB_TEST_MINIGZIP",
            #
            "SRC_SQLITE_SHELL",
        ]
    ])
    #
    # build_ext - link c-extension
# https://github.com/kaizhu256/sqlmath/actions/runs/4886979281/jobs/8723014944
    arg_list = []
    arg_list += [ # must be ordered first
        "build/SRC_ZLIB_BASE.obj",
        #
        "build/SRC_SQLITE_BASE.obj",
        "build/SRC_SQLITE_PYTHON.obj",
        #
        "build/SQLMATH_BASE.obj",
        "build/SQLMATH_CUSTOM.obj",
    ]
    if is_win32:
        arg_list = [
            exe_link,
            *[f"/LIBPATH:{path}" for path in path_library],
            *arg_list,
            #
            "/INCREMENTAL:NO", # optimization - reduce filesize
            "/LTCG", # from cl.exe /GL
            "/MANIFEST:EMBED",
            "/MANIFESTUAC:NO",
            #
            "/DLL",
            "/EXPORT:PyInit__sqlmath",
            #
            f"/OUT:build/{file_lib}",
            "/nologo",
        ]
    else:
        arg_list = [
            *cc_ldshared.strip().split(" "),
            *arg_list,
            #
            *cc_ldflags.strip().split(" "),
            #
            "-o", f"build/{file_lib}",
        ]
    await create_subprocess_exec_and_check(*arg_list, env=env)
    #
    # build_ext - copy c-extension to bdist
    await create_subprocess_exec_and_check(
        "sh",
        "-c",
        f"""
(set -e
    mkdir -p "{dir_wheel}/"
    cp "build/{file_lib}" "sqlmath/{file_lib}"
    cp "build/{file_lib}" "{dir_wheel}/{file_lib}"
    cp sqlmath/*.py "{dir_wheel}/"
)
        """,
    )


def build_ext_init():
    """This function will build c-extension."""
    if pathlib.Path("build/SRC_SQLITE_BASE.c").exists():
        return
    subprocess.run(["sh", "-c", """
(set -e
    mkdir -p build/
    for C_DEFINE in \\
        SRC_ZLIB_BASE \\
        SRC_ZLIB_TEST_EXAMPLE \\
        SRC_ZLIB_TEST_MINIGZIP \\
        \\
        SRC_SQLITE_BASE \\
        SRC_SQLITE_PYTHON \\
        SRC_SQLITE_SHELL
    do
        printf "
#define SRC_SQLITE_BASE_C2
#define ${C_DEFINE}_C2
#include \\"../sqlite_rollup.c\\"
    " > build/$C_DEFINE.c
    done
    #
    for C_DEFINE in \\
        SQLMATH_BASE \\
        SQLMATH_CUSTOM
    do
        printf "
#define SRC_SQLITE_BASE_C2
#define ${C_DEFINE}_C2
#include \\"../$(printf $C_DEFINE | tr \\"[:upper:]\\" \\"[:lower:]\\").c\\"
        " > build/$C_DEFINE.c
    done
)
    """], check=True)


# monkey-patch setuptools to accept c-extension compiled in nodejs
setuptools.command.build_ext.build_ext.run = lambda self: self
setuptools.command.install_lib.install_lib.install = lambda self: self

if __name__ == "__main__":
    match sys.argv[1]:
        case "build_ext":
            build_ext()
        case "build_ext_async":
            asyncio.get_event_loop().run_until_complete(build_ext_async())
        case "build_ext_init":
            build_ext_init()
        case "bdist_wheel":
            build_ext()
        case "test":
            import sqlmath
            sqlmath.test_python_run()
        case _:
            setuptools.setup()
