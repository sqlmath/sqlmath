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
setup.py.

npm_config_mode_debug2=1 python setup.py build_ext && python setup.py test
python -m build
"""

__version__ = "2024.1.21"
__version_info__ = ("2024", "1", "21")

import asyncio
import base64
import hashlib
import json
import os
import pathlib
import re
import shutil
import subprocess
import sys
import sysconfig
import tempfile
import zipfile


def assert_or_throw(condition, message=None):
    """This function will throw <message> if <condition> is falsy."""
    if not condition:
        raise SetupError(message)


def build_ext():
    """This function will build c-extension."""
    subprocess.run(["python", "setup.py", "build_ext_async"], check=True)


async def build_ext_async(): # noqa: C901
    """This function will build c-extension."""

    async def build_ext_obj(cdefine):
        file_obj = pathlib.Path(f"build/{cdefine}.obj")
        match cdefine:
            case "SQLMATH_BASE":
                file_src = pathlib.Path("sqlmath_base.c")
            case "SQLMATH_CUSTOM":
                file_src = pathlib.Path("sqlmath_custom.c")
            case "SRC_SQLITE_SHELL":
                file_src = pathlib.Path("sqlmath_external_sqlite.c")
            case "SRC_PCRE2_BASE":
                file_src = pathlib.Path("sqlmath_external_pcre2.c")
            case "SRC_SQLITE_BASE":
                file_src = pathlib.Path("sqlmath_external_sqlite.c")
            case "SRC_ZLIB_BASE":
                file_src = pathlib.Path("sqlmath_external_zlib.c")
        match cdefine:
            case "SQLMATH_BASE":
                pass
            case "SQLMATH_CUSTOM":
                pass
            case "SRC_SQLITE_SHELL":
                pass
            case _:
                if (
                    file_obj.exists()
                    and file_obj.stat().st_mtime > file_src.stat().st_mtime
                ):
                    print(f"build_ext - skip {file_src}")
                    return
        arg_list = [
            *[f"-I{path}" for path in path_include],
            #
            f"-D{cdefine}_C2=",
            "-D_REENTRANT=1",
            "-DSQLMATH_PYTHON_C2=" if cdefine == "SQLMATH_CUSTOM" else "",
        ]
        if npm_config_mode_debug and is_win32:
            arg_list += ["/W3"]
        elif npm_config_mode_debug:
            arg_list += ["-Wextra"]
        elif is_win32:
            arg_list += [
                "/W3",
                "/wd4047",
                "/wd4244",
                "/wd4996",
            ]
        elif cdefine in [
            "SQLMATH_BASE",
            "SQLMATH_CUSTOM",
        ]:
            arg_list += ["-Wextra"]
        else:
            arg_list += [
                "-Wno-all",
                "-Wno-extra",
                "-Wno-implicit-fallthrough",
                "-Wno-incompatible-pointer-types",
                "-Wno-int-conversion",
                "-Wno-unused-parameter",
            ]
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
                # bugfix - fix multi-word cc_compiler="gcc -pthread"
                *cc_compiler.split(" "),
                *arg_list,
                #
                *cc_ccshared.strip().split(" "),
                *cc_cflags.strip().split(" "),
                #
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
        child = await asyncio.create_subprocess_exec(
            *[arg for arg in args if arg],
            **kwds,
        )
        await child.communicate()
        if child.returncode != 0:
            msg = f"returncode={child.returncode}"
            raise subprocess.SubprocessError(msg)
    #
    # build_ext - update version
    pathlib.Path("build").mkdir(parents=True, exist_ok=True)
    with pathlib.Path("package.json").open() as file1:
        package_json = json.load(file1)
        version = package_json["version"].split("-")[0]
    if package_json["name"] != "sqlmath":
        version = __version__
    for filename in [
        "PKG-INFO",
        "README.md",
        "pyproject.toml",
        "setup.py",
        "sqlmath/__init__.py",
    ]:
        with pathlib.Path(filename).open("r+", newline="\n") as file1:
            data0 = file1.read()
            data1 = data0
            # update version - PKG-INFO
            data1 = re.sub(
                "\nVersion: .*",
                f"\nVersion: {version}",
                data1,
            )
            # update version - README.md
            data1 = re.sub(
                "(sqlmath(?:-|==))\\d\\d\\d\\d\\.\\d\\d?\\.\\d\\d?",
                f"\\g<1>{version}",
                data1,
            )
            # update version - pyproject.toml
            data1 = re.sub("\nversion = .*", f'\nversion = "{version}"', data1)
            # update version - sqlmath/__init__.py
            data1 = re.sub(
                "\n__version__ = .*",
                f'\n__version__ = "{version}"',
                data1,
            )
            data1 = re.sub(
                "\n__version_info__ = .*",
                (
                    "\n__version_info__ = "
                    + str(tuple(version.split("."))).replace("'", '"')
                ),
                data1,
            )
            if (
                data1 != data0
                and (
                    package_json["name"] == "sqlmath"
                    or filename in ("PKG-INFO", "pyproject.toml")
                )
            ):
                print(f"build_ext - update file {file1.name}")
                file1.seek(0)
                file1.write(data1)
                file1.truncate()
    #
    # build_ext - init sysconfig
    cc_ccshared = sysconfig.get_config_var("CCSHARED") or ""
    cc_cflags = sysconfig.get_config_var("CFLAGS") or ""
    cc_compiler = sysconfig.get_config_var("CC") or ""
    if sys.platform == "linux" and cc_compiler.startswith("gcc"):
        cc_compiler += " -ldl"
    cc_ldflags = sysconfig.get_config_var("LDFLAGS") or ""
    cc_ldshared = sysconfig.get_config_var("LDSHARED") or ""
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
    npm_config_mode_debug = os.getenv("npm_config_mode_debug") # noqa: SIM112
    #
    # build_ext - init env
    env = os.environ
    if is_win32:
        env = env_vcvarsall()
        await_list = []
        for exe in ["cl.exe", "link.exe"]:
            await_list.append( # noqa: PERF401
                (
                    await asyncio.create_subprocess_exec(
                        *["where", exe],
                        env=env,
                        stdout=asyncio.subprocess.PIPE,
                    )
                ).stdout.readline(),
            )
        [exe_cl, exe_link] = [
            str(exe.splitlines()[0], "utf8")
            for exe in await asyncio.gather(*await_list)
        ]
    #
    # build_ext - virtualenv
    for arr in [path_include, path_library]:
        for path in arr:
            if path_prefix_base != path_prefix:
                path2 = path.replace(path_prefix, path_prefix_base)
                if path2 not in arr:
                    arr.append(path2)
    #
    # build_ext - compile .obj file
    await asyncio.gather(*[
        build_ext_obj(cdefine)
        for cdefine in [
            "SRC_PCRE2_BASE",
            "SRC_SQLITE_BASE",
            "SRC_ZLIB_BASE",
            #
            "SQLMATH_BASE",
            "SQLMATH_CUSTOM",
        ]
    ])
    #
    # build_ext - link c-extension
# https://github.com/kaizhu256/sqlmath/actions/runs/4886979281/jobs/8723014944
    arg_list = []
    arg_list += [ # must be ordered first
        "build/SRC_PCRE2_BASE.obj",
        "build/SRC_SQLITE_BASE.obj",
        "build/SRC_ZLIB_BASE.obj",
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
    # build_ext - copy c-extension to sqlmath/
    shutil.copyfile(f"build/{file_lib}", f"sqlmath/{file_lib}")


def build_pkg_info():
    """This function will build PKG-INFO."""
    # https://packaging.python.org/en/latest/specifications/core-metadata/#core-metadata
    toml = ""
    with pathlib.Path("pyproject.toml").open() as file1:
        toml = file1.read()
    data = ""
    data += "Metadata-Version: 2.1\n"
    for match in re.finditer(
        '\n(.*?) = .*?"(.*?)"',
        toml,
    ):
        match match[1]:
            case "authors":
                data += f"Author: {match[2]}\n"
            case "description":
                data += f"Summary: {match[2]}\n"
            case "license":
                data += f"{match[1].capitalize()}: {match[2]}\n"
            case "name":
                data += f"{match[1].capitalize()}: {match[2]}\n"
            case "requires-python":
                data += f"Requires-Python: {match[2]}\n"
            case "version":
                data += f"{match[1].capitalize()}: {match[2]}\n"
    for match in re.finditer(
        '\n    "(.*? :: .*)"',
        toml,
    ):
        data += f"Classifier: {match[1]}\n"
    for match in re.finditer(
        '\n(changelog|documentation|homepage|repository) = "(.*?)"',
        toml,
        re.IGNORECASE,
    ):
        data += f"Project-URL: {match[1]}, {match[2]}\n"
    data += "License-File: LICENSE\n"
    with pathlib.Path("README.md").open() as file1:
        data += "Description-Content-Type: text/markdown\n\n"
        data += file1.read().strip() + "\n"
    with pathlib.Path("PKG-INFO").open("w", newline="\n") as file1:
        file1.write(re.sub(" +\n", "\n", data))


def build_sdist(sdist_directory, config_settings=None):
    """
    `build_sdist`: build an sdist in the folder and return the basename.

    https://peps.python.org/pep-0517/#build-sdist
    """
    assert_or_throw(
        config_settings is None or config_settings == {},
        config_settings,
    )
    # build PKG-INFO
    build_pkg_info()
    # init sdist_directory
    sdist_directory = pathlib.Path(sdist_directory).resolve()
    # init file_sdist
    name_version = ""
    with pathlib.Path("pyproject.toml").open() as file1:
        toml = file1.read()
        name_version = (
            re.search('\nname = "(.*?)"', toml)[1]
            + "-"
            + re.search('\nversion = "(.*?)"', toml)[1]
        )
    file_sdist = sdist_directory / f"{name_version}.tar.gz"
    print(f"setup.py sdist - building file {file_sdist}")
    # Copy files from MANIFEST.in to dir_tmp and create tarball.
    with tempfile.TemporaryDirectory() as dir_tmp:
        script = ""
        with pathlib.Path("MANIFEST.in").open() as file1:
            script = file1.read()
        script = "\n".join(
            f"cp --parents '{file}' '{dir_tmp}/{name_version}/'"
            for file in re.sub(
                "\ninclude ",
                "\n",
                f"\n{script}\n",
            ).strip().split("\n")
        )
        if sys.platform == "darwin":
            script = script.replace("cp --parents", "rsync -R")
        script = f"""(set -e
        mkdir -p '{dir_tmp}/{name_version}/' '{sdist_directory}/'
        {script}
        (cd '{dir_tmp}' && tar -zcf out.tgz {name_version}/)
        cp '{dir_tmp}/out.tgz' '{file_sdist}'
        )
        """
        file_tmp = ""
        with tempfile.NamedTemporaryFile("w", delete=False) as file1:
            file1.write(script)
            file_tmp = file1.name
        subprocess.run(["sh", file_tmp], check=True)
        pathlib.Path(file_tmp).unlink()
    print(f"setup.py sdist - built file {file_sdist}")
    return file_sdist.name


def build_wheel(
    wheel_directory,
    config_settings=None,
    metadata_directory=None,
):
    """
    `build_wheel`: build a wheel in the folder and return the basename.

    https://peps.python.org/pep-0517/#build-wheel
    """
    assert_or_throw(
        config_settings is None or config_settings == {},
        config_settings,
    )
    noop(metadata_directory)
    wheel_directory = pathlib.Path(wheel_directory).resolve()
    #
    # build c-extension
    build_ext()
    #
    # init tag_xxx
    # {dist}-{version}(-{build tag})?-{python tag}-{abitag}-{platform tag}.whl
    # https://packaging.python.org/en/latest/specifications/platform-compatibility-tags/
    # The version is py_version_nodot.
    tag_python = f'cp{sysconfig.get_config_var("py_version_nodot")}'
    tag_abi = tag_python
    # The platform tag is simply sysconfig.get_platform()
    # with all hyphens - and periods . replaced with underscore _.
    tag_platform = re.sub("\\W", "_", sysconfig.get_platform())
    #
    # build file_wheel
    file_wheel = pathlib.Path(wheel_directory) / (
        f"sqlmath-{__version__}-{tag_python}-{tag_abi}-{tag_platform}.whl"
    )
    with zipfile.ZipFile(file_wheel, "w", zipfile.ZIP_DEFLATED) as file_zip:
        dir_distinfo = f"sqlmath-{__version__}.dist-info"
        file_lib = f"sqlmath/_sqlmath{sysconfig.get_config_var('EXT_SUFFIX')}"
        data_record = ""
        for bb, aa in (
            ("sqlmath/__init__.py", "sqlmath/__init__.py"),
            (file_lib, file_lib),
            # Place .dist-info at the end of the archive.
            (f"{dir_distinfo}/LICENSE", "LICENSE"),
            (f"{dir_distinfo}/METADATA", "PKG-INFO"),
            (f"{dir_distinfo}/WHEEL", ""),
        ):
            if bb == f"{dir_distinfo}/WHEEL":
                data = bytes(
                    f"""Wheel-Version: 1.0
Generator: bdist_wheel 1.0
Root-Is-Purelib: false
Tag: {tag_python}-{tag_abi}-{tag_platform}
""",
                    "utf-8",
                )
            else:
                with pathlib.Path(aa).open("rb") as file1:
                    data = file1.read()
            file_zip.writestr(bb, data)
            digest = base64.urlsafe_b64encode(
                hashlib.sha256(data).digest(),
            ).rstrip(b"=").decode("ascii")
            data_record += f"{bb},sha256={digest},{len(data)}\n"
        data_record += f"{dir_distinfo}/RECORD,,"
        file_zip.writestr(f"{dir_distinfo}/RECORD", data_record)
    return file_wheel.name


def debuginline(*argv):
    """This function will print <argv> to stderr and then return <argv>[0]."""
    arg0 = argv[0] if argv else None
    print("\n\ndebuginline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n", file=sys.stderr)
    return arg0


def env_vcvarsall():
    """This function will return vcvarsall <env>."""
    env = subprocess.check_output(
        [
            (
                (
                    os.getenv("PROGRAMFILES(X86)")
                    or os.getenv("PROGRAMFILES")
                )
                + "\\Microsoft Visual Studio"
                + "\\Installer"
                + "\\vswhere.exe"
            ),
            "-latest", "-prerelease",
            "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-property", "installationPath",
            "-products", "*",
        ],
        stderr=subprocess.STDOUT,
    ).decode(encoding="mbcs", errors="strict").strip()
    env = subprocess.check_output(
        'cmd /u /c "{}" {} && set'.format(
            f"{env}\\VC\\Auxiliary\\Build\\vcvarsall.bat",
            {
                "win-amd64": "x86_amd64",
                "win-arm32": "x86_arm",
                "win-arm64": "x86_arm64",
                "win32": "x86",
            }.get(sysconfig.get_platform()),
        ),
        stderr=subprocess.STDOUT,
    ).decode("utf-16le", errors="replace")
    env = {
        key.lower(): val
        for key, _, val in
        (line.partition("=") for line in env.splitlines())
        if (
            key and val
            and not re.search("\\W", key)
            and not re.search("[\"'\n\r]", val)
        )
    }
    with pathlib.Path("build/vcvarsall.sh").open("w") as file1:
        file1.write(
            "".join(f"export {key}='{val}'\n" for key, val in env.items()),
        )
    return env


def noop(*args, **kwargs): # noqa: ARG001
    """This function will do nothing."""
    return


def raise_setup_error(*args, **kwargs):
    """This function will raise SetupError."""
    raise SetupError({args, kwargs})


class SetupError(Exception):
    """Setup error."""


if __name__ == "__main__":
    match sys.argv[1]:
        case "bdist_wheel":
            build_wheel("dist")
        case "build_ext":
            build_ext()
        case "build_ext_async":
            asyncio.set_event_loop(asyncio.new_event_loop())
            asyncio.get_event_loop().run_until_complete(build_ext_async())
        case "build_pkg_info":
            build_pkg_info()
        case "env_vcvarsall":
            env_vcvarsall()
            with pathlib.Path("build/vcvarsall.sh").open() as file1:
                print(file1.read())
        case "sdist":
            build_sdist("dist")
        case "test":
            subprocess.run(["python", "test.py", "--verbose"], check=True)
        case _:
            raise_setup_error(sys.argv)
