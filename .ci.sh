#!/bin/sh

# curl -L https://www.sqlite.org/2021/sqlite-autoconf-3360000.tar.gz | tar -xz
# https://www.sqlite.org/2021/sqlite-tools-linux-x86-3360000.zip
# https://www.sqlite.org/2021/sqlite-tools-osx-x86-3360000.zip
# https://www.sqlite.org/2021/sqlite-tools-win32-x86-3360000.zip

shCiArtifactUploadCustom() {(set -e
    git fetch origin artifact
    git checkout origin/artifact branch-*
)}

shCiArtifactUpload2() {(set -e
# this function will upload build-artifacts to branch-gh-pages
    local BRANCH="$(git rev-parse --abbrev-ref HEAD)"
    local BRANCH_ARTIFACT=artifact
    local COMMIT_MESSAGE=\
" - upload artifact"\
" - retry$GITHUB_UPLOAD_RETRY"\
" - $BRANCH"\
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
    rm -rf ".tmp/$BRANCH_ARTIFACT"
    mkdir -p ".tmp/$BRANCH_ARTIFACT"
    cd ".tmp/$BRANCH_ARTIFACT"
    git init
    cp ../../.git/config .git/
    git fetch origin "$BRANCH_ARTIFACT"
    git checkout -b "$BRANCH_ARTIFACT" "origin/$BRANCH_ARTIFACT"
    # update dir branch-$BRANCH
    mkdir -p "branch-$BRANCH"
    cp ../../_binary_* "branch-$BRANCH"
    git add .
    git commit -am "$COMMIT_MESSAGE" || true
    # git commit --allow-empty -am "$COMMIT_MESSAGE" || true
    # squash commits
    if [ "$BRANCH" = alpha ] \
        && [ "$(git rev-list --count "$BRANCH_ARTIFACT")" -gt 10 ]
    then
        # squash commits
        git checkout --orphan squash1
        git commit --quiet -am "- squash $COMMIT_MESSAGE" || true
        # reset $BRANCH_ARTIFACT to squashed-commit
        git push . -f "squash1:$BRANCH_ARTIFACT"
        git checkout "$BRANCH_ARTIFACT"
        # force-push squashed-commit
        shGitCmdWithGithubToken push origin -f "$BRANCH_ARTIFACT"
    fi
    # sync before push
    git pull origin "$BRANCH_ARTIFACT"
    # push
    shGitCmdWithGithubToken push origin "$BRANCH_ARTIFACT"
    # debug
    shGitLsTree
)}

shCiBaseCustom() {(set -e
    local BRANCH="$(git rev-parse --abbrev-ref HEAD)"
    shCiBuild
    shCiTest
    # upload binary
    if [ "$GITHUB_ACTION" ] && ( \
        [ "$BRANCH" = alpha ] \
        || [ "$BRANCH" = beta ] \
        || [ "$BRANCH" = master ] \
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

shCiBuild() {(set -e
# this function will run base-ci
    local FILE
    # cleanup
    rm -f _binary_*
    rm -rf .tmp
    # init .tmp
    mkdir -p .tmp
    #
    # patch sqlite3.h
    printf '
#undef SQLITE3_C2
#define SQLITE3_H2
#undef SQLITE3_EXT_H2
#undef SQLITE3_SHELL_C2
#include "../sqlite3.c"
    ' > .tmp/sqlite3.h
    #
    # patch sqlite3ext.h
    printf '
#undef SQLITE3_C2
#undef SQLITE3_H2
#define SQLITE3_EXT_H2
#undef SQLITE3_SHELL_C2
#include "../sqlite3.c"
    ' > .tmp/sqlite3ext.h
    #
    # patch sqlmath_napi.cpp
    printf '
#define SQLMATH_NAPI
#include "../sqlmath_custom.cpp"
#include "../sqlmath_custom.c"
    ' > .tmp/sqlmath_napi.cpp
    #
    # node-gyp - run
    node -e '
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
    require("fs").writeFileSync("binding.gyp", JSON.stringify({ //jslint-quiet
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
                        "-O3",
                        "-Os"
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
                            "-O3",
                            "-Os"
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
                            "-O2",
                            "-Os"
                        ]
                    }
                }
            },
            "defines": [

// https://www.sqlite.org/compile.html#recommended_compile_time_options

                "SQLITE_DEFAULT_MEMSTATUS=0",
                "SQLITE_DEFAULT_WAL_SYNCHRONOUS=1",
                "SQLITE_DQS=0",
                "SQLITE_LIKE_DOESNT_MATCH_BLOBS",
                // "SQLITE_MAX_EXPR_DEPTH=0",
                // "SQLITE_OMIT_AUTOINIT",
                "SQLITE_OMIT_DECLTYPE",
                "SQLITE_OMIT_DEPRECATED",
                "SQLITE_OMIT_PROGRESS_CALLBACK",
                // "SQLITE_OMIT_SHARED_CACHE",
                // "SQLITE_THREADSAFE=0",
                "SQLITE_USE_ALLOCA",

// custom

                "SQLITE_ENABLE_MATH_FUNCTIONS",
                // "SQLITE_ENABLE_STMTVTAB",
                // "SQLITE_ENABLE_UNKNOWN_SQL_FUNCTION",
                "SQLITE_HAVE_ZLIB",

// node-sqlite3

                "HAVE_USLEEP=1",
                "NAPI_DISABLE_CPP_EXCEPTIONS=1",
                "SQLITE_ENABLE_DBSTAT_VTAB=1",
                "SQLITE_ENABLE_FTS3",
                "SQLITE_ENABLE_FTS4",
                "SQLITE_ENABLE_FTS5",
                "SQLITE_ENABLE_JSON1",
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
                    "SQLITE3_C2",
                    "SQLITE3_EXT_C2"
                ],
                "sources": [
                    "../sqlite3.c",
                    "../sqlite3_ext.c"
                ],
                "target_name": "sqlite3_c",
                "type": "static_library"
            }),
            targetWarningLevel(1, {
                "defines": [
                    "SQLMATH_C"
                ],
                "dependencies": [
                    "sqlite3_c"
                ],
                "sources": [
                    "../sqlmath_custom.c"
                ],
                "target_name": "sqlmath_c",
                "type": "static_library"
            }),
            targetWarningLevel(1, {
                "dependencies": [
                    "sqlmath_c"
                ],
                "sources": [
                    "./sqlmath_napi.cpp"
                ],
                "target_name": "<(target_node)"
            }),
/*
            targetWarningLevel(0, {
                "defines": [
                    "SQLITE3_SHELL_C2"
                ],
                "dependencies": [
                    "sqlmath_c"
                ],
                "sources": [
                    "../sqlite3_shell.c"
                ],
                "target_name": "<(target_shell)",
                "type": "executable"
            }),
*/
            {
                "copies": [
                    {
                        "destination": "..",
                        "files": [
                            "<(PRODUCT_DIR)/<(target_node).node"
                            // "<(PRODUCT_DIR)/<(target_shell)<(.exe)"
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
                "_binary_sqlmath_shell"
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
            require("path").resolve(
                require("path").dirname(process.execPath),
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
        if (require("child_process").spawnSync("node", action, { //jslint-quiet
            stdio: [
                "ignore", 1, 2
            ]
        }).status !== 0) {
            process.exit(1);
        }
    });
}());
' "$@" # '
    shCiTest
)}

shCiNpmPublishCustom() {(set -e
# this function will run custom-code to npm-publish package
    local FILE
    # fetch binaries
    git fetch origin artifact --depth=1
    git checkout origin/artifact "branch-beta/_binary_"*
    cp -a branch-beta/_binary_* .
    # npm-publish
    npm publish --access public
)}

shCiTest() {(set -e
    node jslint.mjs .
    export npm_config_mode_test=1
    node -e '
(function () {
    process.chdir(".tmp");
    [
        "build"
    ].forEach(function (action) {
        // node-gyp.js
        action = [
            require("path").resolve(
                require("path").dirname(process.execPath),
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
        if (require("child_process").spawnSync("node", action, { //jslint-quiet
            stdio: [
                "ignore", 1, 2
            ]
        }).status !== 0) {
            process.exit(1);
        }
    });
}());
' "$@" # '
    shRunWithCoverage --exclude=jslint.mjs node --input-type=module --eval '
import sqlmath from "./sqlmath_custom.mjs";
sqlmath.testAll();
' "$@" # '
)}

(set -e
    # init $UPSTREAM_OWNER
    export UPSTREAM_OWNER=sqlmath
    # init $UPSTREAM_REPO
    export UPSTREAM_REPO=sqlmath.js
)
