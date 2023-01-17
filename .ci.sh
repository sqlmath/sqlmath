#!/bin/sh

# sh one-liner
# sh jslint_ci.sh shCiBuildWasm
# sh jslint_ci.sh shSqlmathUpdate

# sqlite autoconf-3380500 version-3.38.5
# curl -L https://www.sqlite.org/2022/sqlite-autoconf-3380500.tar.gz | tar -xz
# https://www.sqlite.org/2022/sqlite-tools-linux-x86-3380500.zip
# https://www.sqlite.org/2022/sqlite-tools-osx-x86-3380500.zip
# https://www.sqlite.org/2022/sqlite-tools-win32-x86-3380500.zip

shCiArtifactUploadCustom() {(set -e
# this function will run custom-code to upload build-artifacts
    git fetch origin artifact
    git checkout origin/artifact "branch-$GITHUB_BRANCH0"
    mv "branch-$GITHUB_BRANCH0"/* .
    git add -f _binary_* sqlmath_wasm.*
    # screenshot html
    node --input-type=module --eval '
import moduleChildProcess from "child_process";
(async function () {
    let {
        GITHUB_BRANCH0,
        GITHUB_GITHUB_IO
    } = process.env;
    await Promise.all([
        (
            `https://${GITHUB_GITHUB_IO}/branch-${GITHUB_BRANCH0}`
            + `/index.html`
        ),
        ".artifact/apidoc.html"
    ].map(async function (url) {
        await new Promise(function (resolve) {
            moduleChildProcess.spawn(
                "sh",
                [
                    "jslint_ci.sh", "shBrowserScreenshot", url
                ],
                {
                    stdio: [
                        "ignore", 1, 2
                    ]
                }
            ).on("exit", resolve);
        });
    }));
}());
' "$@" # '
)}

shCiArtifactUpload2() {(set -e
# this function will upload build-artifacts to branch-gh-pages
    local BRANCH_ARTIFACT=artifact
    local COMMIT_MESSAGE=\
" - upload artifact"\
" - retry$GITHUB_UPLOAD_RETRY"\
" - $GITHUB_BRANCH0"\
" - $(printf "$GITHUB_SHA" | cut -c-8)"\
" - $(uname)"\
""
    node --input-type=module --eval '
process.exit(Number(
    `${process.version.split(".")[0]}`
    !== process.env.GITHUB_ARTIFACT_UPLOAD_NODE_VERSION
));
' || return 0
    # init .git/config
    git config --local user.email "github-actions@users.noreply.github.com"
    git config --local user.name "github-actions"
    # git clone origin/artifact
    rm -rf ".tmp/$BRANCH_ARTIFACT"
    shGitCmdWithGithubToken clone origin ".tmp/$BRANCH_ARTIFACT" \
        --branch="$BRANCH_ARTIFACT" \
        --single-branch
    (set -e
    cd ".tmp/$BRANCH_ARTIFACT"
    cp ../../.git/config .git/config
    # update dir branch-$GITHUB_BRANCH0
    mkdir -p "branch-$GITHUB_BRANCH0"
    cp ../../_binary_* "branch-$GITHUB_BRANCH0"
    if [ -f ../../sqlmath_wasm.wasm ]
    then
        cp ../../sqlmath_wasm.* "branch-$GITHUB_BRANCH0"
    fi
    git add .
    git commit -am "$COMMIT_MESSAGE" || true
    # git commit --allow-empty -am "$COMMIT_MESSAGE" || true
    # git push
    if [ "$GITHUB_BRANCH0" = alpha ]
    then
        shGithubPushBackupAndSquash origin "$BRANCH_ARTIFACT" 20
    fi
    # sync before push
    shGitCmdWithGithubToken pull origin "$BRANCH_ARTIFACT"
    # push
    shGitCmdWithGithubToken push origin "$BRANCH_ARTIFACT"
    # debug
    shGitLsTree
    )
)}

shCiBaseCustom() {(set -e
# this function will run custom-code for base-ci
    shCiEmsdkExport
    # .github_cache - restore
    if [ "$GITHUB_ACTION" ] && [ -d .github_cache ]
    then
        cp -a .github_cache/* . || true # js-hack - */
    fi
    shCiBuildNodejs
    if (shCiIsMainJob)
    then
        shCiBuildWasm
        # .github_cache - save
        if [ "$GITHUB_ACTION" ] && [ ! -d .github_cache/_emsdk ]
        then
            mkdir -p .github_cache
            cp -a "$EMSDK" .github_cache
        fi
    fi
    # upload artifact
    if [ "$GITHUB_ACTION" ] && ( \
        [ "$GITHUB_BRANCH0" = alpha ] \
        || [ "$GITHUB_BRANCH0" = beta ] \
        || [ "$GITHUB_BRANCH0" = master ] \
    )
    then
        export GITHUB_UPLOAD_RETRY=-1
        while true
        do
            GITHUB_UPLOAD_RETRY="$((GITHUB_UPLOAD_RETRY + 1))"
            if [ "$GITHUB_UPLOAD_RETRY" -gt 4 ]
            then
                return 1
            fi
            if (shCiArtifactUpload2)
            then
                break
            fi
        done
    fi
)}

shCiBuildNodejs() {(set -e
# this function will build binaries in nodejs
    local FILE
    # cleanup
    rm -f _binary_*
    rm -rf .tmp
    # init .tmp
    mkdir -p .tmp
    #
    # patch sqlite3.h
    printf '
#ifndef SQLITE3_H_INCLUDED
    #define SQLITE3_H_INCLUDED
    #define SQLITE3_H2
    #undef SQLITE3_C2
    #undef SQLITE3_EXT_H2
    #include "../sqlite3.c"
#endif
    ' > .tmp/sqlite3.h
    #
    # patch sqlite3ext.h
    printf '
#ifndef SQLITE3EXT_H_INCLUDED
    #define SQLITE3EXT_H_INCLUDED
    #define SQLITE3_EXT_H2
    #undef SQLITE3_C2
    #undef SQLITE3_H2
    #include "../sqlite3.c"
#endif
    ' > .tmp/sqlite3ext.h
    #
    # node-gyp - run
    node --input-type=module --eval '
import moduleChildProcess from "child_process";
import moduleFs from "fs";
import modulePath from "path";
(function () {
    let cflags = {
        "cflags": [
            "-fdiagnostics-show-option",
            "-std=c99"
        ],
        "cflags!": [
            "-fno-exceptions"
        ],
        "cflags_cc": [
        ],
        "cflags_cc!": [
            "-fno-exceptions"
        ]
    };
    function targetWarningLevel(level, target) {
// this function will set <target>"s warnings to <level>
        let warningList = ["-Wall", "-w"];
        if (!level) {
            warningList.reverse();
        }
        target.msvs_settings = target.msvs_settings || {
            "VCCLCompilerTool": {
                "WarningLevel": (
                    level
                    ? 4
                    : 0
                )
            }
        };
        target.xcode_settings = target.xcode_settings || {};
        [
            target, "cflags", "cflags!",
            target, "cflags_cc", "cflags_cc!",
            target.xcode_settings, "OTHER_CFLAGS", "OTHER_CFLAGS!",
            target.xcode_settings,
            "OTHER_CPLUSPLUSFLAGS",
            "OTHER_CPLUSPLUSFLAGS!"
        ].forEach(function (dict, ii, list) {
            if (ii % 3 !== 0) {
                return;
            }
            [
                list[ii + 1], list[ii + 2]
            ].forEach(function (key, ii) {
                dict[key] = dict[key] || [];
                dict[key].push(warningList[ii]);
            });
        });
        return target;
    }
    process.chdir(".tmp");
    moduleFs.writeFileSync("binding.gyp", JSON.stringify({ //jslint-quiet
        "target_defaults": {
            "cflags": cflags.cflags,
            "cflags!": cflags["cflags!"],
            "cflags_cc": cflags.cflags_cc,
            "cflags_cc!": cflags["cflags_cc!"],
            "conditions": [
                [
                    "OS == \u0027win\u0027",
                    {
                        "defines": [
                            "WIN32"
                        ]
                    },
                    {
                        "link_settings": {
                            "libraries": [
                                "-ldl",
                                "-lm",
                                "-lz"
                            ]
                        }
                    }
                ]
            ],
            "configurations": {
                "Debug": {
                    "cflags_cc!": [
                        "-DNDEBUG",
                        "-O3"
                    ],
                    "defines": [
                        "DEBUG",
                        "_DEBUG"
                    ],
                    "defines!": [
                        "NDEBUG"
                    ],
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "RuntimeLibrary": 1
                        },
                        "VCLinkerTool": {
                            "GenerateDebugInformation": "true"
                        }
                    },
                    "xcode_settings": {
                        "GCC_GENERATE_DEBUGGING_SYMBOLS": "YES",
                        "GCC_OPTIMIZATION_LEVEL": "0",
                        "OTHER_CPLUSPLUSFLAGS!": [
                            "-DNDEBUG",
                            "-O3"
                        ]
                    }
                },
                "Release": {
                    "defines": [
                        "NDEBUG"
                    ],
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "Optimization": 2
                        }
                    },
                    "xcode_settings": {
                        "DEAD_CODE_STRIPPING": "YES",
                        "GCC_GENERATE_DEBUGGING_SYMBOLS": "NO",
                        "GCC_INLINES_ARE_PRIVATE_EXTERN": "YES",
                        "GCC_OPTIMIZATION_LEVEL": "3",
                        "OTHER_CPLUSPLUSFLAGS!": [
                            "-O3"
                        ]
                    }
                }
            },
            "defines": [
                //
// https://www.sqlite.org/compile.html#recommended_compile_time_options
                //
                "SQLITE_DQS=0",
                // "SQLITE_THREADSAFE=0",
                "SQLITE_DEFAULT_MEMSTATUS=0",
                "SQLITE_DEFAULT_WAL_SYNCHRONOUS=1",
                "SQLITE_LIKE_DOESNT_MATCH_BLOBS",
                "SQLITE_MAX_EXPR_DEPTH=0",
                "SQLITE_OMIT_DECLTYPE",
                "SQLITE_OMIT_DEPRECATED",
                "SQLITE_OMIT_PROGRESS_CALLBACK",
                // "SQLITE_OMIT_SHARED_CACHE",
                "SQLITE_USE_ALLOCA",
                "SQLITE_OMIT_AUTOINIT",
                //
                // extra optimization
                //
                "HAVE_MALLOC_USABLE_SIZE",
                "SQLITE_ENABLE_NULL_TRIM",
                "SQLITE_ENABLE_SORTER_REFERENCES",
                //
                // extra feature
                //
                // "SQLITE_ENABLE_ICU",
                "SQLITE_ENABLE_MATH_FUNCTIONS",
                // "SQLITE_ENABLE_STMTVTAB",
                "SQLITE_HAVE_ZLIB",
                //
                // node-sqlite3
                //
                "HAVE_USLEEP=1",
                "NAPI_DISABLE_CPP_EXCEPTIONS=1",
                "SQLITE_ENABLE_DBSTAT_VTAB=1",
                "SQLITE_ENABLE_FTS3",
                "SQLITE_ENABLE_FTS4",
                "SQLITE_ENABLE_FTS5",
                "SQLITE_ENABLE_RTREE",
                "SQLITE_THREADSAFE=1",
                "_REENTRANT=1"
            ],
            "include_dirs": [
                "."
            ],
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "ExceptionHandling": 1,
                    // https://github.com/nodejs/node-gyp/issues/1686
                    "RuntimeLibrary": 0, // 0 - MultiThreaded (/MT)
                    // "RuntimeLibrary": 1, // 1 - MultiThreadedDebug (/MTd)
                    // "RuntimeLibrary": 2, // 2 - MultiThreadedDLL (/MD)
                    // "RuntimeLibrary": 3, // 3 - MultiThreadedDebugDLL (/MDd)
                    "WarningLevel": 3
                },
                "VCLinkerTool": {
                    "GenerateDebugInformation": "true"
                }
            },
            "xcode_settings": {
                "CLANG_CXX_LIBRARY": "libc++",
                "GCC_C_LANGUAGE_STANDARD": "c99",
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "OTHER_CFLAGS": cflags.cflags,
                "OTHER_CFLAGS!": cflags["cflags!"],
                "OTHER_CPLUSPLUSFLAGS": cflags.cflags_cc,
                "OTHER_CPLUSPLUSFLAGS!": cflags["cflags_cc!"]
            }
        },
        "targets": [
            targetWarningLevel(0, {
                "defines": [
                    "SQLITE3_C2"
                ],
                "sources": [
                    "../sqlite3.c"
                ],
                "target_name": "sqlite3_c",
                "type": "static_library"
            }),
            targetWarningLevel(1, {
                "defines": [
                    "SQLITE3_EXT_C2"
                ],
                "sources": [
                    "../sqlite3_ext.c",
                    "../sqlmath_custom.c"
                ],
                "target_name": "sqlmath_c",
                "type": "static_library"
            }),
            targetWarningLevel(1, {
                "defines": [
                    "SQLMATH_NAPI"
                ],
                "dependencies": [
                    "sqlite3_c",
                    "sqlmath_c"
                ],
                "sources": [
                    "../sqlmath_base.c"
                ],
                "target_name": "<(target_node)"
            }),
            targetWarningLevel(0, {
                "defines": [
                    "SQLITE3_SHELL_C2"
                ],
                "dependencies": [
                    "sqlite3_c",
                    "sqlmath_c"
                ],
                "sources": [
                    "../sqlite3_shell.c"
                ],
                "target_name": "<(target_shell)",
                "type": "executable"
            }),
            {
                "copies": [
                    {
                        "destination": "..",
                        "files": [
                            "<(PRODUCT_DIR)/<(target_node).node",
                            "<(PRODUCT_DIR)/<(target_shell)<(.exe)"
                        ]
                    }
                ],
                "dependencies": [
                    "<(target_node)"
                ],
                "target_name": "target_install",
                "type": "none"
            }
        ],
        "variables": {
            ".exe": (
                process.platform === "win32"
                ? ".exe"
                : ""
            ),
            "target_node": (
                "_binary_sqlmath"
                + "_napi8"
                + "_" + process.platform
                + "_" + process.arch
            ),
            "target_shell": (
                "_binary_sqlmath"
                + "_shell"
                + "_" + process.platform
                + "_" + process.arch
            )
        }
    }, undefined, 4));
    [
        "clean",
        "configure",
        "build"
    ].forEach(function (action) {
        // node-gyp.js
        action = [
            modulePath.resolve(
                modulePath.dirname(process.execPath),
                "node_modules/npm/node_modules/node-gyp/bin/node-gyp.js"
            ).replace("/bin/node_modules/", "/lib/node_modules/"),
            action,
            (
                process.env.npm_config_mode_debug
                ? "--debug"
                : "--release"
            )
        ];
        console.error(
            "(cd .tmp && node " + action.map(function (elem) {
                return "\u0027" + elem + "\u0027";
            }).join(" ") + ")"
        );
        if (moduleChildProcess.spawnSync("node", action, { //jslint-quiet
            stdio: [
                "ignore", 1, 2
            ]
        }).status !== 0) {
            process.exit(1);
        }
    });
}());
' "$@" # '
    shCiTestNodejs
)}

shCiBuildWasm() {(set -e
# this function will build binaries in wasm
    shCiEmsdkExport
    # install emsdk
    shCiEmsdkInstall
    # cd ${EMSDK} && . ./emsdk_env.sh && cd ..
    # build wasm
    local FILE
    local FILE2
    local OPTION1
    local OPTION2
    printf "shCiBuildWasm\n" 1>&2
    OPTION1="$OPTION1 -I.tmp"
    OPTION1="$OPTION1 -Wall"
    OPTION1="$OPTION1 -flto"
    # debug
    # OPTION1="$OPTION1 -O0"
    OPTION1="$OPTION1 -Os"
    # OPTION1="$OPTION1 -fsanitize=address"
    for FILE in \
        sqlite3.c \
        sqlite3_ext.c \
        sqlmath_custom.c
    do
        FILE2=".tmp/$(basename "$FILE").wasm.o"
        # optimization - skip rebuild of sqlite3.c if possible
        if [ "$FILE2" -nt "$FILE" ] && [ "$FILE" = sqlite3.c ]
        then
            printf "shCiBuildWasm - skip $FILE\n" 1>&2
            continue
        fi
        OPTION2=""
        #
# https://www.sqlite.org/compile.html#recommended_compile_time_options
        #
        OPTION2="$OPTION2 -DSQLITE_DQS=0"
        OPTION2="$OPTION2 -DSQLITE_THREADSAFE=0"
        OPTION2="$OPTION2 -DSQLITE_DEFAULT_MEMSTATUS=0"
        OPTION2="$OPTION2 -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
        OPTION2="$OPTION2 -DSQLITE_LIKE_DOESNT_MATCH_BLOBS"
        OPTION2="$OPTION2 -DSQLITE_MAX_EXPR_DEPTH=0"
        OPTION2="$OPTION2 -DSQLITE_OMIT_DECLTYPE"
        OPTION2="$OPTION2 -DSQLITE_OMIT_DEPRECATED"
        OPTION2="$OPTION2 -DSQLITE_OMIT_PROGRESS_CALLBACK"
        # OPTION2="$OPTION2 -DSQLITE_OMIT_SHARED_CACHE"
        # OPTION2="$OPTION2 -DSQLITE_USE_ALLOCA"
        OPTION2="$OPTION2 -DSQLITE_OMIT_AUTOINIT"
        #
        # extra optimization
        #
        OPTION2="$OPTION2 -DHAVE_MALLOC_USABLE_SIZE"
        OPTION2="$OPTION2 -DSQLITE_ENABLE_NULL_TRIM"
        OPTION2="$OPTION2 -DSQLITE_ENABLE_SORTER_REFERENCES"
        #
        # extra feature
        #
        OPTION2="$OPTION2 -DEMSCRIPTEN"
        OPTION2="$OPTION2 -DSQLITE_DISABLE_LFS"
        OPTION2="$OPTION2 -DSQLITE_ENABLE_FTS3"
        OPTION2="$OPTION2 -DSQLITE_ENABLE_FTS3_PARENTHESIS"
        OPTION2="$OPTION2 -DSQLITE_ENABLE_MATH_FUNCTIONS"
        OPTION2="$OPTION2 -DSQLITE_ENABLE_NORMALIZE"
        OPTION2="$OPTION2 -DSQLITE_HAVE_ZLIB"
        # file
        OPTION2="$OPTION2 -c $FILE -o $FILE2"
        case "$FILE" in
        sqlite3.c)
            OPTION2="$OPTION2 -DSQLITE3_C2"
            ;;
        *)
            OPTION2="$OPTION2 -DSQLITE3_EXT_C2"
            ;;
        esac
        emcc $OPTION1 $OPTION2
    done
    OPTION2=""
    #
    OPTION2="$OPTION2 -s EXPORTED_FUNCTIONS=_sqlite3_initialize"
    OPTION2="$OPTION2,___dbFileImportOrExport"
    OPTION2="$OPTION2,_dbClose"
    OPTION2="$OPTION2,_dbExec"
    OPTION2="$OPTION2,_dbFileImportOrExport"
    OPTION2="$OPTION2,_dbNoop"
    OPTION2="$OPTION2,_dbOpen"
    OPTION2="$OPTION2,_jsbatonValueErrmsg"
    OPTION2="$OPTION2,_jsbatonValueStringArgi"
    OPTION2="$OPTION2,_sqlite3_errmsg"
    OPTION2="$OPTION2,_sqlite3_free"
    OPTION2="$OPTION2,_sqlite3_malloc"
    #
    OPTION2="$OPTION2 -s EXPORTED_RUNTIME_METHODS=cwrap"
    # OPTION2="$OPTION2,AsciiToString"
    #
    case "$1" in
    --debug)
        OPTION2="$OPTION2 -s ASSERTIONS=1 -s SAFE_HEAP=1"
        ;;
    *)
        OPTION2="$OPTION2 --closure 1"
        ;;
    esac
    emcc $OPTION1 $OPTION2 \
        --memory-init-file 0 \
        --pre-js sqlmath_wrapper_wasm.js \
        -Wall \
        -o .tmp/sqlmath_wasm.js \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s ALLOW_TABLE_GROWTH=1 \
        -s NODEJS_CATCH_EXIT=0 \
        -s NODEJS_CATCH_REJECTION=0 \
        -s RESERVED_FUNCTION_POINTERS=64 \
        -s SINGLE_FILE=0 \
        -s USE_ZLIB \
        -s WASM=1 \
        -s WASM_BIGINT \
        .tmp/sqlite3.c.wasm.o \
        .tmp/sqlite3_ext.c.wasm.o \
        .tmp/sqlmath_custom.c.wasm.o \
        #
    printf '' > sqlmath_wasm.js
    printf "/*jslint-disable*/
// Copyright (c) 2021 Kai Zhu
// SPDX-License-Identifier: MIT
// $(date -u +"%Y-%m-%dT%H:%M:%S%z")
(function () {
\"use strict\";
" >> sqlmath_wasm.js
    cat .tmp/sqlmath_wasm.js | tr -d "\r" >> sqlmath_wasm.js
    printf '
}());
/*jslint-enable*/
' >> sqlmath_wasm.js
    cp .tmp/sqlmath_wasm.wasm .
    ls -l sqlmath_wasm.*
)}

shCiEmsdkExport() {
# this function will export emsdk env
    export EMSCRIPTEN_VERSION=3.1.3
    export EMSDK="$PWD/_emsdk"
    # https://github.com/sql-js/sql.js/blob/v1.6.2/.devcontainer/Dockerfile
    if [ ! "$PATH_EMSDK" ]
    then
        export PATH_EMSDK="$EMSDK:$EMSDK/upstream/emscripten/"
        export PATH="$PATH_EMSDK:$PATH"
    fi
}

shCiEmsdkInstall() {(set -e
# this function will install emsdk
    shCiEmsdkExport
    if [ -d "$EMSDK" ]
    then
        exit
    fi
    # https://github.com/emscripten-core/emsdk/blob/2.0.34/docker/Dockerfile
    git clone https://github.com/emscripten-core/emsdk.git $EMSDK
    #
    echo "## Install Emscripten"
    cd ${EMSDK}
    ./emsdk install ${EMSCRIPTEN_VERSION}
    echo "## Done"
    #
    # This generates configuration that contains all valid paths according to
    # installed SDK
    # TODO(sbc): We should be able to use just emcc -v here but it doesn't
    # currently create the sanity file.
    cd ${EMSDK}
    echo "## Generate standard configuration"
    ./emsdk activate ${EMSCRIPTEN_VERSION}
    chmod 777 ${EMSDK}/upstream/emscripten
    chmod -R 777 ${EMSDK}/upstream/emscripten/cache
    echo "int main() { return 0; }" > hello.c
    ${EMSDK}/upstream/emscripten/emcc -c hello.c
    cat ${EMSDK}/upstream/emscripten/cache/sanity.txt
    echo "## Done"
    #
    # Cleanup Emscripten installation and strip some symbols
    echo "## Aggressive optimization: Remove debug symbols"
    cd ${EMSDK} && . ./emsdk_env.sh
    # Remove debugging symbols from embedded node (extra 7MB)
    strip -s `which node`
    # Tests consume ~80MB disc space
    rm -fr ${EMSDK}/upstream/emscripten/tests
    # Fastcomp is not supported
    rm -fr ${EMSDK}/upstream/fastcomp
    # strip out symbols from clang (~extra 50MB disc space)
    find ${EMSDK}/upstream/bin -type f -exec strip -s {} + || true
    echo "## Done"
    #
    # download ports
    touch "$EMSDK/.null.c"
    emcc \
        -s USE_ZLIB \
        "$EMSDK/.null.c" -o "$EMSDK/.null_wasm.js"
)}

shCiNpmPublishCustom() {(set -e
# this function will run custom-code to npm-publish package
    local FILE
    # fetch artifact
    git fetch origin artifact --depth=1
    git checkout origin/artifact \
        "branch-beta/_binary_"* \
        "branch-beta/sqlmath_wasm"*
    cp -a branch-beta/_binary_* .
    cp -a branch-beta/sqlmath_wasm.* .
    # npm-publish
    npm publish --access public
)}

shCiTestNodejs() {(set -e
# this function will run test in nodejs
    local COVERAGE_EXCLUDE
    # init .tmp
    mkdir -p .tmp
    # rebuild c-module
    export npm_config_mode_test=1
    if [ "$npm_config_fast" != true ]
    then
        # # indent c-file
        # if (uname | grep -q "MING\|MSYS")
        # then
        #     ./indent.exe \
        #         --blank-lines-after-commas \
        #         --braces-on-func-def-line \
        #         --break-function-decl-args \
        #         --break-function-decl-args-end \
        #         --dont-line-up-parentheses \
        #         --k-and-r-style \
        #         --line-length78 \
        #         --no-tabs \
        #         -bfde \
        #         sqlite3_ext.c \
        #         sqlmath_base.c \
        #         sqlmath_custom.c \
        #         sqlmath_jenks.c
        #     dos2unix *.c
        # fi
        # lint c-file
        python cpplint.py \
            --filter=-whitespace/comments \
            sqlmath_base.c \
            sqlmath_custom.c \
            sqlmath_jenks.c
        # lint js-file
        node jslint.mjs .
        node --input-type=module --eval '
import moduleChildProcess from "child_process";
import modulePath from "path";
(function () {
    process.chdir(".tmp");
    [
        "build"
    ].forEach(function (action) {
        // node-gyp.js
        action = [
            modulePath.resolve(
                modulePath.dirname(process.execPath),
                "node_modules/npm/node_modules/node-gyp/bin/node-gyp.js"
            ).replace("/bin/node_modules/", "/lib/node_modules/"),
            action,
            (
                process.env.npm_config_mode_debug
                ? "--debug"
                : "--release"
            )
        ];
        console.error(
            "(cd .tmp && node " + action.map(function (elem) {
                return "\u0027" + elem + "\u0027";
            }).join(" ") + ")"
        );
        if (moduleChildProcess.spawnSync("node", action, { //jslint-quiet
            stdio: [
                "ignore", 1, 2
            ]
        }).status !== 0) {
            process.exit(1);
        }
    });
}());
' "$@" # '
    fi;
    rm -f *~ .*test.sqlite
    COVERAGE_EXCLUDE="--exclude=jslint.mjs"
    if (node --eval '
require("assert")(require("./package.json").name !== "sqlmath");
' > /dev/null 2>&1)
    then
        COVERAGE_EXCLUDE="$COVERAGE_EXCLUDE --exclude=sqlmath.mjs"
    fi
    shRunWithCoverage $COVERAGE_EXCLUDE node test.mjs
)}

shSqlmathUpdate() {(set -e
# this function will update files with ~/Documents/sqlmath/
    local FILE
    sh myci2.sh shMyciUpdate
    if [ "$PWD/" = "$HOME/Documents/sqlmath/" ]
    then
        shRawLibFetch asset_sqlmath_external_rollup.js
        shRawLibFetch index.html
        shRawLibFetch sqlite3.c
        shRawLibFetch sqlite3_ext.c
        shRawLibFetch sqlite3_shell.c
        git grep '3\.38\.[^5]' \
            ":(exclude)CHANGELOG.md" \
            ":(exclude)sqlite3.c" \
            || true
        git grep 'autoconf-[0-9]' | grep -v CHANGELOG \
            | grep -v '3380500' || true
        git grep 'sqlite.*version-[0-9]' | grep -v CHANGELOG \
            | grep -v '3\.38\.5' || true
        return
    fi
    if [ -d "$HOME/Documents/sqlmath/" ]
    then
        for FILE in \
            .ci.sh \
            asset_sqlmath_external_rollup.js \
            indent.exe \
            index.html \
            sqlite3.c \
            sqlite3_ext.c \
            sqlite3_shell.c \
            sqlmath.mjs \
            sqlmath_base.c \
            sqlmath_browser.mjs \
            sqlmath_jenks.c \
            sqlmath_wrapper_wasm.js
        do
            ln -f "$HOME/Documents/sqlmath/$FILE" "$FILE"
        done
    fi
    git --no-pager diff
)}
