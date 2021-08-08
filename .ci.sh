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
    git fetch origin gh-pages --depth=1
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
    # save binding.node
    git add -f .binding-*.node
    git commit -am binding.node
    # checkout branch-gh-pages
    git fetch origin gh-pages --depth=1
    git checkout -b gh-pages origin/gh-pages
    # update dir branch-$BRANCH
    (set -e
        cd "branch-$BRANCH"
        cp -a ../.git .
        git checkout "$BRANCH" .binding-*.node
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

shCiBase() {(set -e
# this function will run base-ci
    # cleanup
    rm -f .binding-*.node .test.big.db-journal
    # init dir .tmp
    rm -rf .tmp
    mkdir -p .tmp
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
                "--module_name=.binding-sqlmath"
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
    # run test
    shRunWithCoverage node test.js
)}

(set -e
    # init $UPSTREAM_OWNER
    export UPSTREAM_OWNER=sqlmath
    # init $UPSTREAM_REPO
    export UPSTREAM_REPO=sqlmath.js
)
