#!/bin/sh

# curl https://www.sqlite.org/2021/sqlite-autoconf-3360000.tar.gz | tar -xz
# https://www.sqlite.org/2021/sqlite-tools-linux-x86-3360000.zip
# https://www.sqlite.org/2021/sqlite-tools-osx-x86-3360000.zip
# https://www.sqlite.org/2021/sqlite-tools-win32-x86-3360000.zip

shCiArtifactUpload() {(set -e
# this function will upload build-artifacts to branch-gh-pages
    local BRANCH
    node --input-type=module -e '
process.exit(Number(
    `${process.version.split(".")[0]}.${process.arch}.${process.platform}`
    !== process.env.CI_NODE_VERSION_ARCH_PLATFORM
));
' || return 0
    # init .git/config
    git config --local user.email "github-actions@users.noreply.github.com"
    git config --local user.name "github-actions"
    # init $BRANCH
    BRANCH="$(git rev-parse --abbrev-ref HEAD)"
    git pull --unshallow origin "$BRANCH"
    # init $UPSTREAM_OWNER
    export UPSTREAM_OWNER="${UPSTREAM_OWNER:-jslint-org}"
    # init $UPSTREAM_REPO
    export UPSTREAM_REPO="${UPSTREAM_REPO:-jslint}"
    # screenshot asset-image-jslint
    shImageLogoCreate &
    # screenshot web-demo
    shBrowserScreenshot \
        "https://$UPSTREAM_OWNER.github.io/\
$UPSTREAM_REPO/branch-beta/index.html"
    # screenshot changelog and files
    node --input-type=module -e '
import moduleChildProcess from "child_process";
(function () {
    [
        // parallel-task - screenshot changelog
        [
            "jslint_ci.sh",
            "shRunWithScreenshotTxt",
            ".build/screenshot-changelog.svg",
            "head",
            "-n50",
            "CHANGELOG.md"
        ],
        // parallel-task - screenshot files
        [
            "jslint_ci.sh",
            "shRunWithScreenshotTxt",
            ".build/screenshot-files.svg",
            "shGitLsTree"
        ]
    ].forEach(function (argList) {
        moduleChildProcess.spawn("sh", argList, {
            stdio: [
                "ignore", 1, 2
            ]
        }).on("exit", function (exitCode) {
            if (exitCode) {
                process.exit(exitCode);
            }
        });
    });
}());
' # '
    # screenshot curl
    if [ -f jslint.mjs ]
    then
        node --input-type=module -e '
import moduleFs from "fs";
import moduleChildProcess from "child_process";
(async function () {
    let screenshotCurl;
    screenshotCurl = await moduleFs.promises.stat("jslint.mjs");
    screenshotCurl = String(`
echo "\
% Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                     Dload  Upload   Total   Spent    Left  Speed
100  250k  100  250k    0     0   250k      0  0:00:01 --:--:--  0:00:01  250k\
"
    `).trim().replace((
        /250/g
    ), Math.floor(screenshotCurl.size / 1024));
    // parallel-task - screenshot example-shell-commands in README.md
    Array.from(String(
        await moduleFs.promises.readFile("README.md", "utf8")
    ).matchAll(
        /\n```shell\u0020<!--\u0020shRunWithScreenshotTxt\u0020(.*?)\u0020-->\n([\S\s]*?\n)```\n/g
    )).forEach(async function ([
        ignore, file, script
    ]) {
        await moduleFs.promises.writeFile(file + ".sh", (
            "printf \u0027"
            + script.trim().replace((
                /[%\\]/gm
            ), "$&$&").replace((
                /\u0027/g
            ), "\u0027\"\u0027\"\u0027").replace((
                /^/gm
            ), "> ")
            + "\n\n\n\u0027\n"
            + script.replace(
                "curl -L https://www.jslint.com/jslint.mjs > jslint.mjs",
                screenshotCurl
            )
        ));
        moduleChildProcess.spawn(
            "sh",
            [
                "jslint_ci.sh",
                "shRunWithScreenshotTxt",
                file,
                "sh",
                file + ".sh"
            ],
            {
                stdio: [
                    "ignore", 1, 2
                ]
            }
        );
    });
}());
' # '
    fi
    # seo - inline css-assets and invalidate cached-assets
    node --input-type=module -e '
import moduleFs from "fs";
(async function () {
    let cacheKey = Math.random().toString(36).slice(-4);
    let fileDict = {};
    await Promise.all([
        "asset-codemirror-rollup.css",
        "browser.mjs",
        "index.html"
    ].map(async function (file) {
        try {
            fileDict[file] = await moduleFs.promises.readFile(file, "utf8");
        } catch (ignore) {
            process.exit();
        }
    }));

// Inline css-assets.

    fileDict["index.html"] = fileDict["index.html"].replace((
        "\n<link rel=\"stylesheet\" href=\"asset-codemirror-rollup.css\">\n"
    ), function () {
        return (
            "\n<style>\n"
            + fileDict["asset-codemirror-rollup.css"].trim()
            + "\n</style>\n"
        );
    });
    fileDict["index.html"] = fileDict["index.html"].replace((
        "\n<style class=\"JSLINT_REPORT_STYLE\"></style>\n"
    ), function () {
        return fileDict["browser.mjs"].match(
            /\n<style\sclass="JSLINT_REPORT_STYLE">\n[\S\s]*?\n<\/style>\n/
        )[0];
    });

// Invalidate cached-assets.

    fileDict["browser.mjs"] = fileDict["browser.mjs"].replace((
        /^import\u0020.+?\u0020from\u0020".+?\.(?:js|mjs)\b/gm
    ), function (match0) {
        return `${match0}?cc=${cacheKey}`;
    });
    fileDict["index.html"] = fileDict["index.html"].replace((
        /\b(?:href|src)=".+?\.(?:css|js|mjs)\b/g
    ), function (match0) {
        return `${match0}?cc=${cacheKey}`;
    });

// Write file.

    await Promise.all(Object.entries(fileDict).map(function ([
        file, data
    ]) {
        moduleFs.promises.writeFile(file, data);
    }));
}());
' # '
    git add -f jslint.cjs jslint.js || true
    # add dir .build
    git add -f .build
    git commit -am "add dir .build"
    # checkout branch-gh-pages
    git fetch origin gh-pages
    git checkout -b gh-pages origin/gh-pages
    # update dir branch-$BRANCH
    rm -rf "branch-$BRANCH"
    mkdir -p "branch-$BRANCH"
    (set -e
        cd "branch-$BRANCH"
        git init -b branch1
        git pull --depth=1 .. "$BRANCH"
        rm -rf .git
        git add -f .
    )
    # update root-dir with branch-beta
    if [ "$BRANCH" = beta ]
    then
        rm -rf .build
        git checkout beta .
    fi
    # update README.md with branch-$BRANCH and $GITHUB_REPOSITORY
    sed -i \
        -e "s|/branch-[0-9A-Z_a-z]*/|/branch-$BRANCH/|g" \
        -e "s|\b$UPSTREAM_OWNER/$UPSTREAM_REPO\b|$GITHUB_REPOSITORY|g" \
        -e "s|\b$UPSTREAM_OWNER\.github\.io/$UPSTREAM_REPO\b|$(
            printf "$GITHUB_REPOSITORY" | sed -e "s|/|.github.io/|"
        )|g" \
        "branch-$BRANCH/README.md"
    git status
    git commit -am "update dir branch-$BRANCH" || true
    # if branch-gh-pages has more than 100 commits,
    # then backup and squash commits
    if [ "$(git rev-list --count gh-pages)" -gt 100 ]
    then
        # backup
        shGitCmdWithGithubToken push origin -f gh-pages:gh-pages-backup
        # squash commits
        git checkout --orphan squash1
        git commit --quiet -am squash || true
        # reset branch-gh-pages to squashed-commit
        git push . -f squash1:gh-pages
        git checkout gh-pages
        # force-push squashed-commit
        shGitCmdWithGithubToken push origin -f gh-pages
    fi
    # list files
    shGitLsTree
    # push branch-gh-pages
    shGitCmdWithGithubToken push origin gh-pages
    #!! # validate http-links
    #!! (set -e
        #!! cd "branch-$BRANCH"
        #!! sleep 15
        #!! shDirHttplinkValidate
    #!! )
)}

shCiArtifactUpload2() {(set -e
# this function will upload build-artifacts to branch-gh-pages
    local BRANCH
    # init .git/config
    git config --local user.email "github-actions@users.noreply.github.com"
    git config --local user.name "github-actions"
    # init $BRANCH
    BRANCH="$(git rev-parse --abbrev-ref HEAD)"
    # save binaries
    git add -f _binary_*
    git commit -am binaries
    # checkout branch-gh-pages
    git fetch origin gh-pages --depth=1
    git checkout -b gh-pages origin/gh-pages
    # update dir branch-$BRANCH
    (set -e
        cd "branch-$BRANCH"
        cp -a ../.git .
        git checkout "$BRANCH" _binary_*
        rm -rf .git
        git add -f .
    )
    # update root-dir with branch-beta
    if [ "$BRANCH" = beta ]
    then
        git rm -rf .build
        git checkout beta .
    fi
    git status
    git commit -am "update dir branch-$BRANCH" || true
    # push branch-gh-pages
    shGitCmdWithGithubToken push origin gh-pages
)}

shCiTest() {(set -e
    node -e '
/*jslint beta, name*/
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
        if (require("child_process").spawnSync("node", action, {
            stdio: [
                "ignore", 1, 2
            ]
        }).status !== 0) {
            process.exit(1);
        }
    });
}());
' "$@" # '
    shRunWithCoverage node --input-type=module -e '
import sqlmath from "./sqlmath.mjs";
sqlmath.testAll();
' "$@" # "'
)}

shCiBase() {(set -e
    shCiBuild
    shCiTest
)}

shCiBuild() {(set -e
# this function will run base-ci
    local FILE
    # cleanup
    rm -f _binary_*
    rm -rf .tmp
    # init .tmp
    mkdir -p .tmp
    cp -a sqlite-autoconf-3360000 .tmp/src
    # cd .tmp/src - start
    (set -e
    cd .tmp/src
    # remove trailing-whitespace
    sed -i.bak "s/  *$//" *.c
    #
    # patch sqlite3.c
    printf '
@@ -192171,7 +192171,12 @@
 ** functions and the virtual table implemented by this file.
 ****************************************************************************/

+// hack-sqlite - inline sqlite3_extension_functions_init
+int sqlite3_extension_functions_init(sqlite3*, char**, const sqlite3_api_routines*);
+int sqlite3_sqlmath_init(sqlite3*, char**, const sqlite3_api_routines*);
 SQLITE_PRIVATE int sqlite3Json1Init(sqlite3 *db){
+  sqlite3_extension_functions_init(db, NULL, &sqlite3Apis);
+  sqlite3_sqlmath_init(db, NULL, &sqlite3Apis);
   int rc = SQLITE_OK;
   unsigned int i;
   static const struct {
    ' | patch -ut sqlite3.c
    #
    # patch sqlmath_napi.cpp
    printf '
#define SQLMATH_NAPI
extern "C" {
#include "../../sqlmath.c"
}
    ' > sqlmath_napi.cpp
    #
    # patch shell.c
    printf '
@@ -6637,7 +6637,9 @@
 #ifdef _WIN32

 #endif
-int sqlite3_regexp_init(
+// hack-sqlite - disable redundant sqlite3_regexp_init
+int sqlite3_regexp_init2(sqlite3 *, char **, const sqlite3_api_routines *);
+int sqlite3_regexp_init2(
   sqlite3 *db,
   char **pzErrMsg,
   const sqlite3_api_routines *pApi
    ' | patch -ut shell.c
    #
    )
    # cd .tmp/src - end
    # node-gyp - run
    node -e '
/*jslint beta, name*/
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
    require("fs").writeFileSync("binding.gyp", JSON.stringify({
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
                "src"
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
                "sources": [
                    "../sqlmath_ext.c",
                    "src/sqlite3.c"
                ],
                "target_name": "sqlite3_c",
                "type": "static_library"
            }),
            targetWarningLevel(1, {
                "dependencies": [
                    "sqlite3_c"
                ],
                "sources": [
                    "../sqlmath.c"
                ],
                "target_name": "sqlmath_c",
                "type": "static_library"
            }),
            targetWarningLevel(0, {
                "dependencies": [
                    "sqlmath_c"
                ],
                "sources": [
                    "src/shell.c"
                ],
                "target_name": "<(target_cli)",
                "type": "executable"
            }),
            targetWarningLevel(1, {
                "dependencies": [
                    "sqlmath_c"
                ],
                "sources": [
                    "src/sqlmath_napi.cpp"
                ],
                "target_name": "<(target_node)"
            }),
            {
                "copies": [
                    {
                        "destination": "..",
                        "files": [
                            "<(PRODUCT_DIR)/<(target_cli)<(.exe)",
                            "<(PRODUCT_DIR)/<(target_node).node"
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
            "target_cli": (
                "_binary_sqlmath_cli"
                + "_" + process.platform
                + "_" + process.arch
            ),
            "target_node": (
                "_binary_sqlmath_napi"
                + "_" + process.versions.napi
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
        if (require("child_process").spawnSync("node", action, {
            stdio: [
                "ignore", 1, 2
            ]
        }).status !== 0) {
            process.exit(1);
        }
    });
}());
' "$@" # '
    npm test --mode-fast
)}

(set -e
    # init $UPSTREAM_OWNER
    export UPSTREAM_OWNER=sqlmath
    # init $UPSTREAM_REPO
    export UPSTREAM_REPO=sqlmath.js
)
