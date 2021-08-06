#!/bin/sh

# curl https://www.sqlite.org/2021/sqlite-autoconf-3360000.tar.gz | tar -xz
# https://www.sqlite.org/2021/sqlite-tools-linux-x86-3360000.zip
# https://www.sqlite.org/2021/sqlite-tools-osx-x86-3360000.zip
# https://www.sqlite.org/2021/sqlite-tools-win32-x86-3360000.zip

shCiBase() {(set -e
# this function will run base-ci
    rm -f .sqlmath-binding-*.node test/support/big.db-journal
    # init dir .tmp
    rm -rf .tmp && mkdir -p .tmp
    cp -a  sqlite-autoconf-3360000 .tmp/src
    # amalgamate extension-functions.c
    cp extension-functions.c .tmp/src
    node -e '
(async function () {
    let fileDict = {};
    process.chdir(".tmp/src");
    await Promise.all([
        "extension-functions.c",
        "sqlite3.c"
    ].map(async function (file) {
        fileDict[file] = await require("fs").promises.readFile(file, "utf8");
    }));
    fileDict["sqlite3.c"] = fileDict["sqlite3.c"].replace((
        "/************** End of json1.c "
        + "***********************************************/"
    ), function (match0) {
        return match0 + "\n" + fileDict["extension-functions.c"];
    }).replace(
        "SQLITE_PRIVATE int sqlite3Json1Init(sqlite3*);",
        "$&\nSQLITE_PRIVATE int RegisterExtensionFunctions(sqlite3*);"
    ).replace(
        "sqlite3Json1Init,",
        "$&\nRegisterExtensionFunctions,"
    );
    await require("fs").promises.writeFile("sqlite3.c", fileDict["sqlite3.c"]);
}());
' "$@" # '
    # node-gyp - init
    # https://www.sqlite.org/compile.html
    # shJsonNormalize .tmp/binding.gyp && cat .tmp/binding.gyp
    printf '
{
    "targets": [
        {
            "cflags": [
                "-std=c99"
            ],
            "cflags!": [
                "-fno-exceptions"
            ],
            "cflags_cc": [
                "-Wno-unused-value"
            ],
            "cflags_cc!": [
                "-fno-exceptions"
            ],
            "default_configuration": "Release",
            "defines": [
                "HAVE_USLEEP=1",
                "NAPI_DISABLE_CPP_EXCEPTIONS=1",
                "NAPI_VERSION=<(napi_build_version)",
                "NDEBUG",
                "SQLITE_DEFAULT_MEMSTATUS=0",
                "SQLITE_DQS=0",
                "SQLITE_ENABLE_DBSTAT_VTAB",
                "SQLITE_ENABLE_FTS3",
                "SQLITE_ENABLE_FTS4",
                "SQLITE_ENABLE_FTS5",
                "SQLITE_ENABLE_JSON1",
                "SQLITE_ENABLE_MATH_FUNCTIONS",
                "SQLITE_ENABLE_RTREE",
                "SQLITE_ENABLE_STMTVTAB",
                "SQLITE_ENABLE_UNKNOWN_SQL_FUNCTION",
                "SQLITE_THREADSAFE=1",
                "_REENTRANT=1"
            ],
            "include_dirs": [
                "..",
                "src"
            ],
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "ExceptionHandling": 1
                }
            },
            "sources": [
                "../node_sqlite3.cc",
                "src/sqlite3.c"
            ],
            "target_name": "<(module_name)",
            "xcode_settings": {
                "CLANG_CXX_LIBRARY": "libc++",
                "DEAD_CODE_STRIPPING": "YES",
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "GCC_GENERATE_DEBUGGING_SYMBOLS": "NO",
                "GCC_INLINES_ARE_PRIVATE_EXTERN": "YES",
                "GCC_OPTIMIZATION_LEVEL": "3",
                "MACOSX_DEPLOYMENT_TARGET": "10.7",
                "OTHER_CPLUSPLUSFLAGS!": [
                    "-Os",
                    "-O2"
                ]
            }
        },
        {
            "copies": [
                {
                    "destination": "..",
                    "files": [
                        "<(PRODUCT_DIR)/<(module_name).node"
                    ]
                }
            ],
            "dependencies": [
                "<(module_name)"
            ],
            "target_name": "target_copy",
            "type": "none"
        }
    ]
}
' > .tmp/binding.gyp
    # node-gyp - run
    node -e '
/*jslint name*/
(function () {
    process.chdir(".tmp");
    let fileNodeGypJs = require("path").resolve(
        require("path").dirname(process.execPath),
        "node_modules/npm/node_modules/node-gyp/bin/node-gyp.js"
    ).replace("/bin/node_modules/", "/lib/node_modules/");
    [
        "clean",
        "configure",
        "build"
    ].forEach(function (action) {
        require("child_process").spawnSync(process.execPath, [
            fileNodeGypJs,
            action,
            (
                "--module_name=.sqlmath-binding"
                + "-" + "napi" + process.versions.napi
                + "-" + process.platform
                + "-" + process.arch
            )
        ], {
            stdio: [
                "ignore", 1, 2
            ]
        });
    });
}());
' "$@" # '
    #!! return
    # run test
    npm install mocha@5.2.0
    node test/support/createdb.js
    node node_modules/mocha/bin/mocha -R spec --timeout 480000
    node test.js
)}
