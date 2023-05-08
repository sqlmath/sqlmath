// MIT License
//
// Copyright (c) 2021 Kai Zhu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


/*jslint beta, bitwise, name, node*/
/*global FinalizationRegistry*/
"use strict";

let FILENAME_DBTMP = "/tmp/__dbtmp1";
let IS_BROWSER;
let JSBATON_ARGC = 16;
let SQLITE_DATATYPE_BLOB = 0x04;
let SQLITE_DATATYPE_FLOAT = 0x02;
let SQLITE_DATATYPE_INTEGER = 0x01;
let SQLITE_DATATYPE_INTEGER_0 = 0x11;
let SQLITE_DATATYPE_INTEGER_1 = 0x21;
let SQLITE_DATATYPE_NULL = 0x05;
let SQLITE_DATATYPE_OFFSET = 768;
let SQLITE_DATATYPE_SHAREDARRAYBUFFER = 0x71;
let SQLITE_DATATYPE_TEXT = 0x03;
let SQLITE_DATATYPE_TEXT_0 = 0x13;
let SQLITE_MAX_LENGTH2 = 1_000_000_000;
let SQLITE_OPEN_AUTOPROXY = 0x00000020;     /* VFS only */
let SQLITE_OPEN_CREATE = 0x00000004;        /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_DELETEONCLOSE = 0x00000008; /* VFS only */
let SQLITE_OPEN_EXCLUSIVE = 0x00000010;     /* VFS only */
let SQLITE_OPEN_FULLMUTEX = 0x00010000;     /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_MAIN_DB = 0x00000100;       /* VFS only */
let SQLITE_OPEN_MAIN_JOURNAL = 0x00000800;  /* VFS only */
let SQLITE_OPEN_MEMORY = 0x00000080;        /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_NOFOLLOW = 0x01000000;      /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_NOMUTEX = 0x00008000;       /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_PRIVATECACHE = 0x00040000;  /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_READONLY = 0x00000001;      /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_READWRITE = 0x00000002;     /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_SHAREDCACHE = 0x00020000;   /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_SUBJOURNAL = 0x00002000;    /* VFS only */
let SQLITE_OPEN_SUPER_JOURNAL = 0x00004000; /* VFS only */
let SQLITE_OPEN_TEMP_DB = 0x00000200;       /* VFS only */
let SQLITE_OPEN_TEMP_JOURNAL = 0x00001000;  /* VFS only */
let SQLITE_OPEN_TRANSIENT_DB = 0x00000400;  /* VFS only */
let SQLITE_OPEN_URI = 0x00000040;           /* Ok for sqlite3_open_v2() */
let SQLITE_OPEN_WAL = 0x00080000;           /* VFS only */
let cModule;
let cModulePath;
let consoleError = console.error;
let dbDict = new WeakMap(); // private-dict of sqlite-database-connections
let dbFinalizationRegistry;
// init debugInline
let debugInline = (function () {
    let __consoleError = function () {
        return;
    };
    function debug(...argv) {

// This function will print <argv> to stderr and then return <argv>[0].

        __consoleError("\n\ndebugInline");
        __consoleError(...argv);
        __consoleError("\n");
        return argv[0];
    }
    debug(); // Coverage-hack.
    __consoleError = console.error; //jslint-ignore-line
    return debug;
}());
let moduleChildProcess;
let moduleChildProcessSpawn;
let moduleFs;
let moduleFsInitResolveList;
let modulePath;
let moduleUrl;
let {
    npm_config_mode_debug,
    npm_config_mode_setup,
    npm_config_mode_test
} = typeof process === "object" && process?.env;
let sqlMessageDict = {}; // dict of web-worker-callbacks
let sqlMessageId = 0;
let sqlWorker;
let version = "v2023.5.1-beta";

function assertJsonEqual(aa, bb, message) {

// This function will assert JSON.stringify(<aa>) === JSON.stringify(<bb>).

    aa = JSON.stringify(objectDeepCopyWithKeysSorted(aa), undefined, 1);
    bb = JSON.stringify(objectDeepCopyWithKeysSorted(bb), undefined, 1);
    if (aa !== bb) {
        throw new Error(
            "\n" + aa + "\n!==\n" + bb
            + (
                typeof message === "string"
                ? " - " + message
                : message
                ? " - " + JSON.stringify(message)
                : ""
            )
        );
    }
}

function assertNumericalEqual(aa, bb, message) {

// This function will assert aa - bb <= Number.EPSILON.

    assertOrThrow(aa, "value cannot be 0 or falsy");
    if (!(Math.abs((aa - bb) / Math.max(aa, bb)) <= 256 * Number.EPSILON)) {
        throw new Error(
            JSON.stringify(aa) + " != " + JSON.stringify(bb) + (
                message
                ? " - " + message
                : ""
            )
        );
    }
}

function assertOrThrow(condition, message) {

// This function will throw <message> if <condition> is falsy.

    if (!condition) {
        throw (
            (!message || typeof message === "string")
            ? new Error(String(message).slice(0, 2048))
            : message
        );
    }
}

async function cCallAsync(baton, cFuncName, ...argList) {

// This function will serialize <argList> to a c <baton>,
// suitable for passing into napi.

    let argi = 0;
    let errStack;
    assertOrThrow(
        argList.length < 16,
        "cCallAsync - argList.length must be less than than 16"
    );
    baton = baton || jsbatonCreate();
    // pad argList to length JSBATON_ARGC
    while (argList.length < 2 * JSBATON_ARGC) {
        argList.push(0n);
    }
    // serialize js-value to c-value
    argList = argList.map(function (value, ii) {
        argi = ii;
        switch (typeof value) {
        case "bigint":
        case "boolean":
            baton.setBigInt64(8 + argi * 8, BigInt(value), true);
            return value;
        case "number":
            // check for min/max safe-integer
            assertOrThrow(
                (
                    -9_007_199_254_740_991 <= value
                    && value <= 9_007_199_254_740_991
                ),
                (
                    "non-bigint integer must be within inclusive-range"
                    + " -9,007,199,254,740,991 to 9,007,199,254,740,991"
                )
            );
            baton.setBigInt64(8 + argi * 8, BigInt(value), true);
            return value;
        // case "object":
        //     break;
        case "string":
            baton = jsbatonValuePush({
                argi,
                baton,
                value: (
                    value.endsWith("\u0000")
                    ? value
                    // append null-terminator to string
                    : value + "\u0000"
                )
            });
            return;
        }
        if (ArrayBuffer.isView(value)) {
            return new DataView(
                value.buffer,
                value.byteOffset,
                value.byteLength
            );
        }
        if (isExternalBuffer(value)) {
            return value;
        }
    });
    // encode cFuncName into baton
    argi += 1;
    baton = jsbatonValuePush({
        argi,
        baton,
        value: cFuncName + "\u0000"
    });
    // prepend baton, cFuncName to argList
    argList = [
        baton, cFuncName, ...argList
    ];
    // preserve stack-trace
    errStack = new Error().stack.replace((
        /.*$/m
    ), "");
    try {
        return (
            IS_BROWSER
            ? await sqlMessagePost(...argList)
            : await cModule[cFuncName](argList)
        );
    } catch (err) {
        err.stack += errStack;
        assertOrThrow(undefined, err);
    }
}

async function childProcessSpawn2(command, args, options) {

// This function will run child_process.spawn as a promise.

    return await new Promise(function (resolve, reject) {
        let bufList = [[], [], []];
        let child;
        let {
            modeCapture,
            modeDebug,
            stdio
        } = options;
        if (modeDebug) {
            consoleError(
                `childProcessSpawn2 - ${command} ${JSON.stringify(args)}`
            );
        }
        child = moduleChildProcessSpawn(
            command,
            args,
            Object.assign({}, options, {
                stdio: [
                    "ignore",
                    (
                        modeCapture
                        ? "pipe"
                        : stdio[1]
                    ),
                    (
                        modeCapture
                        ? "pipe"
                        : stdio[2]
                    )
                ]
            })
        );
        if (modeCapture) {
            [
                child.stdin, child.stdout, child.stderr
            ].forEach(function (pipe, ii) {
                if (ii === 0) {
                    return;
                }
                pipe.on("data", function (chunk) {
                    bufList[ii].push(chunk);
                    if (stdio[ii] !== "ignore") {
                        switch (ii) {
                        case 1:
                            process.stdout.write(chunk);
                            break;
                        case 2:
                            process.stderr.write(chunk);
                            break;
                        }
                    }
                });
            });
        }
        child.on("exit", function (exitCode) {
            let resolve0 = resolve;
            let stderr;
            let stdout;
            // coverage-hack
            if (exitCode || npm_config_mode_test) {
                resolve = reject;
            }
            // coverage-hack
            if (npm_config_mode_test) {
                resolve = resolve0;
            }
            [stdout, stderr] = bufList.slice(1).map(function (buf) {
                return (
                    typeof modeCapture === "string"
                    ? Buffer.concat(buf).toString(modeCapture)
                    : Buffer.concat(buf)
                );
            });
            resolve({exitCode, stderr, stdout});
        });
    });

}

async function ciBuildext({
    process
}) {

// This function will build sqlmath from c.

    let env;
    let fileList = [];
    let isWin32 = process.platform === "win32";
    let modeTestZlib = process.env?.GITHUB_ACTION || npm_config_mode_test;
    let pyinfo;
    [env, pyinfo] = await ciBuildext1Env({isWin32, process});
    await moduleFs.promises.mkdir("build/", {recursive: true});
    fileList = fileList.concat([
        {cDefine: "SRC_ZLIB_BASE", fileSrc: "sqlite3_rollup.c"},
        //
        {cDefine: "SRC_SQLITE3_BASE", fileSrc: "sqlite3_rollup.c"},
        {cDefine: "SRC_SQLITE3_EXTFNC", fileSrc: "sqlite3_rollup.c"},
        {cDefine: "SRC_SQLITE3_SHELL", fileSrc: "sqlite3_rollup.c"},
        {cDefine: "SRC_SQLITE3_PYTHON", fileSrc: "sqlite3_rollup.c"},
        //
        {cDefine: "SQLMATH_BASE", fileSrc: "sqlmath_base.c"},
        //
        {cDefine: "SQLMATH_CUSTOM", fileSrc: "sqlmath_custom.c"}
    ]);
    if (modeTestZlib) {
        fileList = fileList.concat([
            {cDefine: "SRC_ZLIB_TEST_EXAMPLE", fileSrc: "sqlite3_rollup.c"},
            {cDefine: "SRC_ZLIB_TEST_MINIGZIP", fileSrc: "sqlite3_rollup.c"}
        ]);
    }
    fileList = fileList.map(function ({
        cDefine,
        fileSrc
    }) {
        return {
            cDefine,
            env,
            fileSrc,
            isWin32,
            modeDebug: npm_config_mode_debug,
            process,
            pyinfo
        };
    });
    await Promise.all(fileList.map(ciBuildext2Obj));
    await Promise.all([
        Promise.all(fileList.map(ciBuildext3Exe)),
        Promise.all(fileList.map(ciBuildext4Nodejs)),
        Promise.all(fileList.map(ciBuildext5Python))
    ]);
    if (modeTestZlib) {
        await childProcessSpawn2(
            "sh",
            [
                "-c",
                (`
(set -e
    printf "\ntest zlib\n"
    if [ "Hello world!" = "$( \
        printf "Hello world!\n" \
            | ./build/SRC_ZLIB_TEST_MINIGZIP.exe \
            | ./build/SRC_ZLIB_TEST_MINIGZIP.exe -d \
        )" ] \
        && ./build/SRC_ZLIB_TEST_EXAMPLE.exe ./build/zlib_test_file
    then
        printf "\n    *** zlib test OK ***\n"
    else
        printf "\n    *** zlib test FAILED ***\n"
        exit 1
    fi
)
                `)
            ],
            {env, stdio: ["ignore", 1, 2]}
        );
    }
}

async function ciBuildext1Env({
    isWin32,
    process
}) {

// This function will fetch posix/win32 env for building c-extension.

    let data;
    let env;
    let pyinfo;
    pyinfo = JSON.parse(noop(
        await childProcessSpawn2(
            "python",
            ["-c", (`
import json
import platform
import sys
import sysconfig
pyinfo = {
    "base_exec_prefix": sys.base_exec_prefix,
    "plat_name": sysconfig.get_platform()
}
# https://github.com/python/cpython/blob/v3.10.10/Lib/sysconfig.py
for key in [
    "ABIFLAGS",
    "ANDROID_API_LEVEL",
    "AR",
    "BINDIR",
    "CC",
    "CCSHARED",
    "CFLAGS",
    "CUSTOMIZED_OSX_COMPILER",
    "EXE",
    "EXT_SUFFIX",
    "GNULD",
    "INCLUDEPY",
    "LDFLAGS",
    "LDSHARED",
    "LDVERSION",
    "LIBDIR",
    "MACHDEP",
    "MACOSX_DEPLOYMENT_TARGET",
    "Py_ENABLED_SHARED",
    "Py_ENABLE_SHARED",
    "RUNSHARED",
    "SO",
    "VERSION",
    "prefix",
    "projectbase",
    "srcdir",
]:
    pyinfo[key] = sysconfig.get_config_var(key);
for key in [
    "data",
    "include",
    "platinclude",
    "platlib",
    "platstdlib",
    "purelib",
    "scripts",
    "stdlib"
]:
    pyinfo[key] = sysconfig.get_path(key);
print(json.dumps(pyinfo))
            `)],
            {modeCapture: "utf8", stdio: ["ignore", "ignore", 2]}
        )
    ).stdout || `{"prefix":"/Python/3.1.2/"}`);
    pyinfo = Object.assign({
        LDFLAGS: "",
        LDSHARED: ""
    }, pyinfo);
    // debug pyinfo
    consoleError(pyinfo);
    pyinfo.pathInclude = [
        `${pyinfo.platinclude}`,
        `${pyinfo.include}`
    ];
    pyinfo.pathLibrary = [
        `${pyinfo.prefix}/libs/`,
        `${pyinfo.prefix}/`
    ];
    // virtualenv
    [pyinfo.pathInclude, pyinfo.pathLibrary].forEach(function (list) {
        list.forEach(function (path) {
            if (pyinfo.base_exec_prefix !== pyinfo.prefix) {
                list.push(path.replace(pyinfo.prefix, pyinfo.base_exec_prefix));
            }
        });
    });
    if (!isWin32) {
        return [process.env, pyinfo];
    }
    // win32 - replace slash with backslash
    [pyinfo.pathInclude, pyinfo.pathLibrary].forEach(function (list) {
        list.forEach(function (path, ii) {
            list[ii] = path.replace((/\//g), "\\");
        });
    });
    data = await childProcessSpawn2(
        (
            process.env["ProgramFiles(x86)"]
            + "\\Microsoft Visual Studio\\Installer\\vswhere.exe"
        ),
        [
            "-latest",
            "-prerelease",
            "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-property", "installationPath",
            "-products", "*"
        ],
        {modeCapture: "utf8", stdio: ["ignore", "ignore", 2]}
    );
    data = await childProcessSpawn2(
        "cmd",
        [
            "/u",
            "/c",
            `${data.stdout.trim()}\\VC\\Auxiliary\\Build\\vcvarsall.bat`,
            (
                process.arch === "arm"
                ? "x86_arm"
                : process.arch === "arm64"
                ? "x86_arm64"
                : process.arch === "ia32"
                ? "x86"
                : "x86_amd64"
            ),
            "&&",
            "set"
        ],
        {modeCapture: "utf16le", stdio: ["ignore", "ignore", 2]}
    );
    data = data.stdout.trim();
    // mock data
    if (npm_config_mode_test) {
        data = "aa=bb";
    }
    env = {};
    data.replace((/([^\r\n]*?)=(.*?)$/gm), function (ignore, key, val) {
        env[key] = val;
    });
    return [env, pyinfo];
}

async function ciBuildext2Obj({
    cDefine,
    env,
    fileSrc,
    isWin32,
    modeDebug,
    pyinfo
}) {

// This function will compile <fileSrc> to <fileObj>

    let argList = [];
    let fileObj = `build/${cDefine}.obj`;
    switch (cDefine) {
    case "SRC_SQLITE3_BASE":
    case "SRC_SQLITE3_EXTFNC":
    case "SRC_SQLITE3_PYTHON":
    case "SRC_SQLITE3_SHELL":
    case "SRC_ZLIB_BASE":
    case "SRC_ZLIB_TEST_EXAMPLE":
    case "SRC_ZLIB_TEST_MINIGZIP":
        break;
    default:
        if (isWin32) {
            argList = argList.concat([
                `/W3`,
                `/std:c11`
            ]);
        } else {
            argList = argList.concat([
                `-Wextra`,
                `-std=c11`
            ]);
        }
    }
    switch (cDefine) {
    case "SRC_SQLITE3_PYTHON":
        argList = argList.concat(pyinfo.pathInclude.map(function (path) {
            return `-I${path}`;
        }));
        break;
    }
    argList = argList.concat([
        `-D${cDefine}_C2=`,
        `-DSQLITE3_C2=`,
        `-D_REENTRANT=1`
    ]);
    if (isWin32) {
// https://github.com/nodejs/node-gyp/blob/v9.3.1/gyp/pylib/gyp/MSVSSettings.py
        argList = argList.concat([
            `/GL`, // to link.exe /LTCG
            `/MT`, // multithreaded, statically-linked
            `/O2`,
            //
            `/Fo${fileObj}`,
            `/c`, `/Tc${fileSrc}`,
            `/nologo`
        ]);
    } else {
        argList = argList.concat([
            `-DHAVE_UNISTD_H=`,
            `-O3`,
            `-c`, `${fileSrc}`,
            `-fPIC`,
            //
            `-o`, `${fileObj}`
        ]);
    }
    if (noop(
        await ciBuildextModified([fileSrc], fileObj)
    )) {
        consoleError(`ciBuildext2Obj - compiling obj ${fileObj}`);
        await childProcessSpawn2(
            (
                isWin32
                ? "cl.exe"
                : "gcc"
            ),
            argList,
            {env, modeDebug, stdio: ["ignore", 1, 2]}
        );
    }
}

async function ciBuildext3Exe({
    cDefine,
    env,
    isWin32,
    modeDebug,
    process
}) {

// This function will link <fileObj> into <fileExe>

    let argList = [];
    let fileExe = `build/${cDefine}.exe`;
    let fileObj = `build/${cDefine}.obj`;
    let fileObjList;
    switch (cDefine) {
    case "SRC_SQLITE3_SHELL":
        fileExe = (
            "_sqlmath.shell"
            + "_" + process.platform
            + "_" + process.arch
            + ".exe"
        );
        fileObjList = [
            "build/SRC_ZLIB_BASE.obj",
            "build/SRC_SQLITE3_BASE.obj",
            "build/SRC_SQLITE3_EXTFNC.obj",
            "build/SRC_SQLITE3_SHELL.obj",
            //
            "build/SQLMATH_BASE.obj",
            "build/SQLMATH_CUSTOM.obj"
        ];
        break;
    case "SRC_ZLIB_TEST_EXAMPLE":
    case "SRC_ZLIB_TEST_MINIGZIP":
        fileObjList = [fileObj, `build/SRC_ZLIB_BASE.obj`];
        break;
    default:
        return;
    }
    argList = argList.concat(fileObjList); // must be ordered first
    if (isWin32) {
        argList = argList.concat([
            `/INCREMENTAL:NO`, // optimization - reduce filesize
            `/LTCG`, // from cl.exe /GL
            `/MANIFEST:EMBED`,
            `/MANIFESTUAC:NO`,
            //
            `/OUT:${fileExe}`,
            `/nologo`
        ]);
    } else {
        argList = argList.concat([
            `-lm`, // link math
            `-o`, `${fileExe}`
        ]);
    }
    if (noop(
        await ciBuildextModified(fileObjList, fileExe)
    )) {
        consoleError(
            `ciBuildext3Exe - linking exe ${modulePath.resolve(fileExe)}`
        );
        await childProcessSpawn2(
            (
                isWin32
                ? "link.exe"
                : "gcc"
            ),
            argList,
            {env, modeDebug, stdio: ["ignore", 1, 2]}
        );
    }
}

async function ciBuildext4Nodejs({
    cDefine,
    env,
    isWin32,
    modeDebug
}) {

// This function will archive <fileObj> into <fileLib>

    let argList = [];
    let binNodegyp;
    let fileLib = `build/${cDefine}.lib`;
    let fileObjList;
    switch (cDefine) {
// https://github.com/kaizhu256/sqlmath/actions/runs/4886979281/jobs/8723014944
    case "SRC_SQLITE3_BASE":
        fileObjList = [
            "build/SRC_ZLIB_BASE.obj",
            "build/SRC_SQLITE3_BASE.obj",
            "build/SRC_SQLITE3_EXTFNC.obj",
            // "build/SRC_SQLITE3_SHELL.obj",
            // "build/SRC_SQLITE3_PYTHON.obj",
            //
            "build/SQLMATH_BASE.obj",
            "build/SQLMATH_CUSTOM.obj"
        ];
        break;
    default:
        return;
    }
    if (isWin32) {
        argList = argList.concat([
            `/LTCG`, // from cl.exe /GL
            `/OUT:${fileLib}`,
            `/nologo`
        ]);
    } else {
        argList = argList.concat([
            `rcs`,
            `${fileLib}` // must be ordered last
        ]);
    }
    argList = argList.concat(fileObjList); // must be ordered last
    if (noop(
        await ciBuildextModified(fileObjList, fileLib)
    )) {
        consoleError(`ciBuildext4Nodejs - archiving lib ${fileLib}`);
        await childProcessSpawn2(
            (
                isWin32
                ? "lib.exe"
                : "ar"
            ),
            argList,
            {env, modeDebug, stdio: ["ignore", 1, 2]}
        );
    }
    consoleError(
        `ciBuildext4Nodejs - linking lib ${modulePath.resolve(cModulePath)}`
    );
    await moduleFs.promises.writeFile("binding.gyp", JSON.stringify({
        "targets": [
            {
                "cflags": ["-Wall", "-std=c11"],
                "conditions": [
                    [
                        "OS == \u0027win\u0027",
                        {"defines": ["WIN32"]},
                        {"defines": ["HAVE_UNISTD_H"]}
                    ]
                ],
                "defines": ["SQLMATH_NODEJS_C2"],
                "libraries": ["SRC_SQLITE3_BASE.lib"],
// https://github.com/nodejs/node-gyp/blob/v9.3.1/gyp/pylib/gyp/MSVSSettings.py
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "WarningLevel": 3
                    }
                },
                "sources": ["sqlmath_base.c"],
                "target_name": "binding",
                "xcode_settings": {"OTHER_CFLAGS": ["-Wall", "-std=c11"]}
            },
            {
                "copies": [
                    {
                        "destination": "build/",
                        "files": ["<(PRODUCT_DIR)/binding.node"]
                    }
                ],
                "dependencies": ["binding"],
                "target_name": "target_copy",
                "type": "none"
            }
        ]
    }, undefined, 4));
    binNodegyp = modulePath.resolve(
        modulePath.dirname(process.execPath),
        "node_modules/npm/node_modules/node-gyp/bin/node-gyp.js"
    ).replace("/bin/node_modules/", "/lib/node_modules/");
    await childProcessSpawn2(
        "sh",
        [
            "-c",
            (`
(set -e
    # node "${binNodegyp}" clean
    node "${binNodegyp}" configure
    node "${binNodegyp}" build --release
    cp build/binding.node "${cModulePath}"
)
            `)
        ],
        {env, modeDebug, stdio: ["ignore", 1, 2]}
    );
}

async function ciBuildext5Python({
    cDefine,
    env,
    isWin32,
    modeDebug,
    pyinfo
}) {

// This function will build python c-extension.

    let argList = [];
    let dirWheel = `build/bdist.${pyinfo.plat_name}/wheel/sqlmath`;
    let fileLib = `_sqlmath${pyinfo.EXT_SUFFIX}`;
// https://github.com/kaizhu256/sqlmath/actions/runs/4886979281/jobs/8723014944
    let fileObjList = [
        "build/SRC_ZLIB_BASE.obj",
        //
        "build/SRC_SQLITE3_BASE.obj",
        "build/SRC_SQLITE3_EXTFNC.obj",
        // "build/SRC_SQLITE3_SHELL.obj",
        "build/SRC_SQLITE3_PYTHON.obj",
        //
        "build/SQLMATH_BASE.obj",
        //
        "build/SQLMATH_CUSTOM.obj"
    ];
    switch (cDefine) {
    case "SRC_SQLITE3_PYTHON":
        break;
    default:
        return;
    }
    argList = argList.concat(fileObjList); // must be ordered first
    if (isWin32) {
        argList = argList.concat(pyinfo.pathLibrary.map(function (path) {
            return `/LIBPATH:${path}`;
        }));
        argList = argList.concat([
            `/INCREMENTAL:NO`, // optimization - reduce filesize
            `/LTCG`, // from cl.exe /GL
            `/MANIFEST:EMBED`,
            `/MANIFESTUAC:NO`,
            //
            `/DLL`,
            `/EXPORT:PyInit__sqlmath`,
            `/IMPLIB:${fileLib}`,
            `/nologo`,
            //
            `/OUT:build/${fileLib}`
        ]);
    } else {
        argList = argList.concat(
            pyinfo.LDFLAGS.trim().split(/\s{1,}/),
            pyinfo.LDSHARED.trim().split(/\s{1,}/).slice(1),
            [`-o`, `build/${fileLib}`]
        );
    }
    if (noop(
        await ciBuildextModified(fileObjList, `sqlmath/${fileLib}`)
    )) {
        consoleError(
            `ciBuildext4Python - linking lib ${modulePath.resolve(fileLib)}`
        );
        await childProcessSpawn2(
            (
                isWin32
                ? "link.exe"
                : pyinfo.LDSHARED.trim().split(/\s{1,}/)[0]
            ),
            argList,
            {env, modeDebug, stdio: ["ignore", 1, 2]}
        );
        await childProcessSpawn2(
            "sh",
            [
                "-c",
                (`
(set -e
    mkdir -p "${dirWheel}/"
    cp "build/${fileLib}" "sqlmath/${fileLib}"
    cp "build/${fileLib}" "${dirWheel}/${fileLib}"
    cp sqlmath/*.py "${dirWheel}/"
)
                `)
            ],
            {env, modeDebug, stdio: ["ignore", 1, 2]}
        );
    }
}

async function ciBuildextModified(fileSrcList, fileObj) {

// This function will detect if <fileSrcList> was modified after <fileObj>.

    fileSrcList = fileSrcList.concat([fileObj]);
    fileSrcList = await Promise.all(fileSrcList.map(async function (file) {
        try {
            return noop(
                await moduleFs.promises.stat(file)
            ).mtimeMs;
        } catch (ignore) {}
    }));
    fileObj = fileSrcList.pop();
    return !fileSrcList.every(function (fileSrc) {
        return fileSrc < fileObj && !npm_config_mode_test;
    });
}

function dbCallAsync(baton, cFuncName, db, ...argList) {

// This function will call <cFuncName> using db <argList>[0].

    let __db = dbDeref(db);
    // increment __db.busy
    __db.busy += 1;
    return cCallAsync(
        baton,
        cFuncName,
        __db.ptr,
        ...argList
    ).finally(function () {
        // decrement __db.busy
        __db.busy -= 1;
        assertOrThrow(__db.busy >= 0, `invalid __db.busy ${__db.busy}`);
    });
}

async function dbCloseAsync({
    db
}) {

// This function will close sqlite-database-connection <db>.

    let __db = dbDeref(db);
    // prevent segfault - do not close db if actions are pending
    assertOrThrow(
        __db.busy === 0,
        "db cannot close with " + __db.busy + " actions pending"
    );
    // cleanup connPool
    await Promise.all(__db.connPool.map(async function (ptr) {
        let val = ptr[0];
        ptr[0] = 0n;
        await cCallAsync(
            undefined,
            "_dbClose",
            val,
            __db.filename
        );
    }));
    dbDict.delete(db);
}

function dbDeref(db) {

// This function will get private-object mapped to <db>.

    let __db = dbDict.get(db);
    assertOrThrow(__db?.connPool[0] > 0, "invalid or closed db");
    assertOrThrow(__db.busy >= 0, "invalid db.busy " + __db.busy);
    __db.ii = (__db.ii + 1) % __db.connPool.length;
    __db.ptr = __db.connPool[__db.ii][0];
    assertOrThrow(__db.ptr > 0n, "invalid or closed db");
    return __db;
}

function dbExecAndReturnLastBlobAsync({
    bindList = [],
    db,
    sql
}) {

// This function will exec <sql> in <db> and return last value retrieved
// from execution as raw blob/buffer.

    return dbExecAsync({
        bindList,
        db,
        responseType: "lastBlob",
        sql
    });
}

async function dbExecAndReturnLastJsonAsync(option) {

// This function will exec <sql> in <db> and return last value retrieved
// from execution as raw text.

    return JSON.parse(
        await dbExecAndReturnLastTextAsync(option)
    );
}

async function dbExecAndReturnLastTextAsync(option) {

// This function will exec <sql> in <db> and return last value retrieved
// from execution as raw text.

    return new TextDecoder().decode(
        await dbExecAndReturnLastBlobAsync(option)
    );
}

async function dbExecAsync({
    bindList = [],
    db,
    modeRetry,
    responseType,
    sql
}) {

// This function will exec <sql> in <db> and return <result>.

    let baton;
    let bindByKey;
    let bindListLength;
    let externalbufferList;
    let result;
    while (modeRetry > 0) {
        try {
            return await dbExecAsync({
                bindList,
                db,
                responseType,
                sql
            });
        } catch (err) {
            assertOrThrow(modeRetry > 0, err);
            consoleError(err);
            consoleError(
                "dbExecAsync - retry failed sql-query with "
                + modeRetry
                + " remaining retries"
            );
            modeRetry -= 1;
            await new Promise(function (resolve) {
                setTimeout(resolve, 5_000 * !npm_config_mode_test);
            });
        }
    }
    baton = jsbatonCreate();
    bindByKey = !Array.isArray(bindList);
    bindListLength = (
        Array.isArray(bindList)
        ? bindList.length
        : Object.keys(bindList).length
    );
    externalbufferList = [];
    Object.entries(bindList).forEach(function ([
        key, val
    ]) {
        if (bindByKey) {
            baton = jsbatonValuePush({
                baton,
                value: ":" + key + "\u0000"
            });
        }
        baton = jsbatonValuePush({
            baton,
            externalbufferList,
            value: val
        });
    });
    result = await dbCallAsync(
        baton,
        "_dbExec",
        db,                     // 0
        String(sql) + "\n;\nPRAGMA noop",   // 1
        bindListLength,         // 2
        bindByKey,              // 3
        (                       // 4
            responseType === "lastBlob"
            ? 1
            : 0
        ),
        undefined, // 5
        undefined, // 6
        undefined, // 7 - response
        ...externalbufferList        // 8
    );
    result = result[2 + 7];
    switch (responseType) {
    case "arraybuffer":
    case "lastBlob":
        return result;
    case "list":
        return JSON.parse(new TextDecoder().decode(result));
    default:
        result = JSON.parse(new TextDecoder().decode(result));
        return result.map(function (rowList) {
            let colList = rowList.shift();
            return rowList.map(function (row) {
                let dict = {};
                colList.forEach(function (key, ii) {
                    dict[key] = row[ii];
                });
                return dict;
            });
        });
    }
}

async function dbFileExportAsync({
    db,
    dbData,
    filename,
    modeExport = 1,
    modeNoop
}) {

// This function will export <db> to <filename>.

    if (modeNoop) {
        return;
    }
    if (IS_BROWSER) {
        filename = FILENAME_DBTMP;
    }
    assertOrThrow(
        typeof filename === "string" && filename,
        `invalid filename ${filename}`
    );
    return await dbCallAsync(
        undefined,
        "_dbFileImportOrExport",
        db,                     // 0. sqlite3 * pInMemory,
        String(filename),       // 1. char *zFilename,
        modeExport,             // 2. const int isSave
        undefined,              // 3. undefined
        dbData                  // 4. dbData
    );
}

async function dbFileImportAsync({
    db,
    dbData,
    filename
}) {

// This function will import <filename> to <db>.

    await dbFileExportAsync({
        db,
        dbData,
        filename,
        modeExport: 0
    });
}

async function dbNoopAsync(...argList) {

// This function will do nothing except return argList.

    return await cCallAsync(undefined, "_dbNoop", ...argList);
}

async function dbOpenAsync({
    afterFinalization,
    dbData,
    filename,
    flags,
    threadCount = 1
}) {

// This function will open and return sqlite-database-connection <db>.

// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
    let connPool;
    let db = {
        filename
    };
    assertOrThrow(
        typeof filename === "string",
        `invalid filename ${filename}`
    );
    assertOrThrow(
        !dbData || isExternalBuffer(dbData),
        "dbData must be ArrayBuffer"
    );
    connPool = await Promise.all(Array.from(new Array(
        threadCount
    ), async function () {
        let ptr = await cCallAsync(
            undefined,
            "_dbOpen",
            // 0. const char *filename,   Database filename (UTF-8)
            String(filename),
            // 1. sqlite3 **ppDb,         OUT: SQLite db handle
            undefined,
            // 2. int flags,              Flags
            flags ?? (
                SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
            ),
            // 3. const char *zVfs        Name of VFS module to use
            undefined,
            // 4. wasm-only - arraybuffer of raw sqlite-database to open in wasm
            dbData
        );
        ptr = [
            ptr[0].getBigInt64(4 + 4, true)
        ];
        dbFinalizationRegistry.register(db, {
            afterFinalization,
            ptr
        });
        return ptr;
    }));
    dbDict.set(db, {
        busy: 0,
        connPool,
        filename,
        ii: 0
    });
    return db;
}

function isExternalBuffer(buf) {

// This function will check if <buf> is ArrayBuffer or SharedArrayBuffer.

    return buf && (
        buf.constructor === ArrayBuffer
        || (
            typeof SharedArrayBuffer === "function"
            && buf.constructor === SharedArrayBuffer
        )
    );
}

function jsbatonCreate() {

// This function will create buffer <baton>.

    let baton = new DataView(new ArrayBuffer(1024));
    // offset nalloc, nused
    baton.setInt32(4, SQLITE_DATATYPE_OFFSET, true);
    return baton;
}

function jsbatonValuePush({
    argi,
    baton,
    externalbufferList,
    value
}) {

// This function will push <value> to buffer <baton>.

    let nn;
    let nused;
    let tmp;
    let vsize;
    let vtype;
/*
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_OFFSET          768
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       0x71
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
    //  1. false.bigint
    //  2. false.boolean
    //  3. false.function
    //  4. false.number
    //  5. false.object
    //  6. false.string
    //  7. false.symbol
    //  8. false.undefined
    //  9. true.bigint
    // 10. true.boolean
    // 11. true.function
    // 12. true.number
    // 13. true.object
    // 14. true.string
    // 15. true.symbol
    // 16. true.undefined
    // 17. true.buffer
    // 18. true.externalbuffer
*/
    // 10. true.boolean
    if (value === 1 || value === 1n) {
        value = true;
    }
    switch (Boolean(value) + "." + typeof(value)) {
    //  1. false.bigint
    case "false.bigint":
    //  2. false.boolean
    case "false.boolean":
    //  4. false.number
    case "false.number":
        vtype = SQLITE_DATATYPE_INTEGER_0;
        vsize = 0;
        break;
    //  3. false.function
    // case "false.function":
    //  5. false.object
    case "false.object":
    //  7. false.symbol
    case "false.symbol":
    //  8. false.undefined
    case "false.undefined":
    // 11. true.function
    case "true.function":
    // 15. true.symbol
    case "true.symbol":
    // 16. true.undefined
    // case "true.undefined":
        vtype = SQLITE_DATATYPE_NULL;
        vsize = 0;
        break;
    //  6. false.string
    case "false.string":
        vtype = SQLITE_DATATYPE_TEXT_0;
        vsize = 0;
        break;
    //  9. true.bigint
    case "true.bigint":
        vtype = SQLITE_DATATYPE_INTEGER;
        vsize = 8;
        break;
    // 10. true.boolean
    case "true.boolean":
        vtype = SQLITE_DATATYPE_INTEGER_1;
        vsize = 0;
        break;
    // 12. true.number
    case "true.number":
        vtype = SQLITE_DATATYPE_FLOAT;
        vsize = 8;
        break;
    // 13. true.object
    // 14. true.string
    default:
        // 18. true.externalbuffer
        if (isExternalBuffer(value)) {
            assertOrThrow(
                !IS_BROWSER,
                "external ArrayBuffer cannot be passed directly to wasm"
            );
            assertOrThrow(
                externalbufferList.length <= 8,
                "externalbufferList.length must be less than 8"
            );
            externalbufferList.push(new DataView(value));
            vtype = SQLITE_DATATYPE_SHAREDARRAYBUFFER;
            vsize = 4;
            break;
        }
        // 17. true.buffer
        if (ArrayBuffer.isView(value)) {
            if (value.byteLength === 0) {
                vtype = SQLITE_DATATYPE_NULL;
                vsize = 0;
                break;
            }
            vtype = SQLITE_DATATYPE_BLOB;
            vsize = 4 + value.byteLength;
            break;
        }
        // 13. true.object
        value = String(
            typeof value === "string"
            ? value
            : typeof value.toJSON === "function"
            ? value.toJSON()
            : JSON.stringify(value)
        );
        // 14. true.string
        value = new TextEncoder().encode(value);
        vtype = SQLITE_DATATYPE_TEXT;
        vsize = 4 + value.byteLength;
    }
    nused = baton.getInt32(4, true);
    nn = nused + 1 + vsize;
    assertOrThrow(
        nn <= 0xffff_ffff,
        "jsbaton cannot exceed 0x7fff_ffff / 2,147,483,647 bytes"
    );
    // exponentially grow baton as needed
    if (baton.byteLength < nn) {
        tmp = baton;
        baton = new DataView(new ArrayBuffer(
            Math.min(2 ** Math.ceil(Math.log2(nn)), 0x7fff_ffff)
        ));
        // update nalloc
        baton.setInt32(0, baton.byteLength, true);
        // copy tmp to baton
        new Uint8Array(
            baton.buffer,
            baton.byteOffset,
            nused
        ).set(new Uint8Array(tmp.buffer, tmp.byteOffset, nused), 0);
    }
    // push vtype
    baton.setUint8(nused, vtype);
    // update nused
    baton.setInt32(4, nused + 1 + vsize, true);
    // handle blob-value
    switch (vtype) {
    case SQLITE_DATATYPE_BLOB:
    case SQLITE_DATATYPE_TEXT:
        // set argv[ii] to blob/text location
        if (argi !== undefined) {
            baton.setInt32(8 + argi * 8, nused, true);
        }
        vsize -= 4;
        // push vsize
        assertOrThrow(
            0 <= vsize && vsize <= 1_000_000_000,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + " 0 to 1,000,000,000"
            )
        );
        baton.setInt32(nused + 1, vsize, true);
        new Uint8Array(
            baton.buffer,
            nused + 1 + 4,
            vsize
        ).set(new Uint8Array(value.buffer, value.byteOffset, vsize), 0);
        break;
    case SQLITE_DATATYPE_FLOAT:
        baton.setFloat64(nused + 1, value, true);
        break;
    case SQLITE_DATATYPE_INTEGER:
        assertOrThrow(
            (
                -9_223_372_036_854_775_808n <= value
                && value <= 9_223_372_036_854_775_807n
            ),
            (
                "sqlite-integer must be within inclusive-range "
                + "-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807"
            )
        );
        baton.setBigInt64(nused + 1, value, true);
        break;
    case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
        vsize = value.byteLength;
        // push vsize
        assertOrThrow(
            0 <= vsize && vsize <= 1_000_000_000,
            (
                "sqlite-blob byte-length must be within inclusive-range"
                + " 0 to 1,000,000,000"
            )
        );
        baton.setInt32(nused + 1, vsize, true);
        break;
    }
    return baton;
}

function jsbatonValueString({
    argi,
    baton
}) {

// This function will return string-value from <baton> at given <offset>.

    let offset = baton.getInt32(4 + 4 + argi * 8, true);
    return new TextDecoder().decode(new Uint8Array(
        baton.buffer,
        offset + 1 + 4,
        // remove null-terminator from string
        baton.getInt32(offset + 1, true) - 1
    ));
}

async function moduleFsInit() {

// This function will import nodejs builtin-modules if they have not yet been
// imported.

// State 3 - Modules already imported.

    if (moduleFs !== undefined) {
        return;
    }

// State 2 - Wait while modules are importing.

    if (moduleFsInitResolveList !== undefined) {
        return new Promise(function (resolve) {
            moduleFsInitResolveList.push(resolve);
        });
    }

// State 1 - Start importing modules.

    moduleFsInitResolveList = [];
    [
        moduleChildProcess,
        moduleFs,
        modulePath,
        moduleUrl
    ] = await Promise.all([
        import("child_process"),
        import("fs"),
        import("path"),
        import("url")
    ]);
    while (moduleFsInitResolveList.length > 0) {
        moduleFsInitResolveList.shift()();
    }
}

function noop(val) {

// This function will do nothing except return <val>.

    return val;
}

function objectDeepCopyWithKeysSorted(obj) {

// This function will recursively deep-copy <obj> with keys sorted.

    let sorted;
    if (typeof obj !== "object" || !obj) {
        return obj;
    }

// Recursively deep-copy list with child-keys sorted.

    if (Array.isArray(obj)) {
        return obj.map(objectDeepCopyWithKeysSorted);
    }

// Recursively deep-copy obj with keys sorted.

    sorted = Object.create(null);
    Object.keys(obj).sort().forEach(function (key) {
        sorted[key] = objectDeepCopyWithKeysSorted(obj[key]);
    });
    return sorted;
}

async function sqlMessagePost(baton, cFuncName, ...argList) {

// This function will post msg to <sqlWorker> and return result.

    let errStack;
    let id;
    let result;
    let timeElapsed = Date.now();
    // increment sqlMessageId
    sqlMessageId += 1;
    id = sqlMessageId;
    // postMessage to web-worker
    sqlWorker.postMessage(
        {
            argList,
            baton,
            cFuncName,
            id
        },
        // transfer arraybuffer without copying
        [
            baton.buffer,
            ...argList.filter(function (elem) {
                return elem && elem.constructor === ArrayBuffer;
            })
        ]
    );
    // preserve stack-trace
    errStack = new Error().stack.replace((
        /.*$/m
    ), "");
    // await result from web-worker
    result = await new Promise(function (resolve) {
        sqlMessageDict[id] = resolve;
    });
    // cleanup sqlMessageDict
    delete sqlMessageDict[id];
    // debug slow postMessage
    timeElapsed = Date.now() - timeElapsed;
    if (timeElapsed > 500 || cFuncName === "testTimeElapsed") {
        consoleError(
            "sqlMessagePost - " + JSON.stringify({
                cFuncName,
                timeElapsed
            }) + errStack
        );
    }
    assertOrThrow(!result.errmsg, result.errmsg);
    return [
        result.baton, result.cFuncName, ...result.argList
    ];
}

async function sqlmathInit() {

// This function will init sqlmath.

    let moduleModule;
    dbFinalizationRegistry = (
        dbFinalizationRegistry
    ) || new FinalizationRegistry(function ({
        afterFinalization,
        ptr
    }) {

// This function will auto-close any open sqlite3-db-pointer,
// after its js-wrapper has been garbage-collected.

        cCallAsync(undefined, "_dbClose", ptr[0]);
        if (afterFinalization) {
            afterFinalization();
        }
    });

// Feature-detect nodejs.

    if (
        !(
            typeof process === "object"
            && typeof process?.versions?.node === "string"
        )
        || cModule
    ) {
        return;
    }

// Init moduleFs.

    await moduleFsInit();
    moduleFsInit(); // coverage-hack
    moduleChildProcessSpawn = moduleChildProcess.spawn;

// Init moduleFs.

    await moduleFsInit();
    moduleFsInit(); // coverage-hack
    moduleChildProcessSpawn = moduleChildProcess.spawn;
    cModulePath = moduleUrl.fileURLToPath(import.meta.url).replace(
        (/\bsqlmath\.mjs$/),
        `_sqlmath.napi6_${process.platform}_${process.arch}.node`
    );

// Import napi c-addon.

    if (!npm_config_mode_setup) {
        moduleModule = await import("module");
        if (!cModule) {
            cModule = moduleModule.createRequire(cModulePath);
            cModule = cModule(cModulePath);
        }
    }
    if (npm_config_mode_test) {

// Mock consoleError.

        consoleError = noop;

// Mock moduleChildProcessSpawn.

        moduleChildProcessSpawn = function () {
            let child = {
                end: noop,
                on: function (onType, resolve) {
                    switch (onType) {
                    case "data":
                        resolve(Buffer.alloc(0));
                        return;
                    default:
                        resolve(0);
                    }
                },
                setEncoding: noop,
                write: noop
            };
            child.stderr = child;
            child.stdin = child;
            child.stdout = child;
            return child;
        };
    }
}

function sqlmathWebworkerInit({
    db,
    modeTest
}) {

// This function will init sqlmath web-worker.

// Feature-detect browser.

    let Worker = globalThis.Worker;
    IS_BROWSER = true;
    if (modeTest) {
        Worker = function () {
            return;
        };
    }
    sqlWorker = new Worker("sqlmath_wasm.js");
    sqlWorker.onmessage = function ({
        data
    }) {
        sqlMessageDict[data.id](data);
    };
    if (modeTest) {
        sqlWorker.postMessage = function (data) {
            setTimeout(function () {
                sqlWorker.onmessage({
                    data
                });
            });
        };
        // test cCallAsync handling-behavior
        cCallAsync(undefined, "testTimeElapsed", true);
        // test dbFileExportAsync handling-behavior
        dbFileExportAsync({
            db,
            filename: "aa",
            modeTest
        });
        // revert IS_BROWSER
        IS_BROWSER = undefined;
    }
}

sqlmathInit(); // coverage-hack
await sqlmathInit();
sqlmathInit(); // coverage-hack

export {
    SQLITE_MAX_LENGTH2,
    SQLITE_OPEN_AUTOPROXY,
    SQLITE_OPEN_CREATE,
    SQLITE_OPEN_DELETEONCLOSE,
    SQLITE_OPEN_EXCLUSIVE,
    SQLITE_OPEN_FULLMUTEX,
    SQLITE_OPEN_MAIN_DB,
    SQLITE_OPEN_MAIN_JOURNAL,
    SQLITE_OPEN_MEMORY,
    SQLITE_OPEN_NOFOLLOW,
    SQLITE_OPEN_NOMUTEX,
    SQLITE_OPEN_PRIVATECACHE,
    SQLITE_OPEN_READONLY,
    SQLITE_OPEN_READWRITE,
    SQLITE_OPEN_SHAREDCACHE,
    SQLITE_OPEN_SUBJOURNAL,
    SQLITE_OPEN_SUPER_JOURNAL,
    SQLITE_OPEN_TEMP_DB,
    SQLITE_OPEN_TEMP_JOURNAL,
    SQLITE_OPEN_TRANSIENT_DB,
    SQLITE_OPEN_URI,
    SQLITE_OPEN_WAL,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    ciBuildext,
    childProcessSpawn2,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAndReturnLastJsonAsync,
    dbExecAndReturnLastTextAsync,
    dbExecAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbNoopAsync,
    dbOpenAsync,
    debugInline,
    jsbatonValueString,
    noop,
    objectDeepCopyWithKeysSorted,
    sqlmathWebworkerInit,
    version
};
