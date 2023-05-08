#!/bin/sh

# sh one-liner
# sh jslint_ci.sh shCiBuildWasm
# sh jslint_ci.sh shSqlmathUpdate

# sqlite autoconf-3390400 version-3.39.4
# curl -L https://www.sqlite.org/2022/sqlite-autoconf-3390400.tar.gz | tar -xz
# https://www.sqlite.org/2022/sqlite-tools-linux-x86-3390400.zip
# https://www.sqlite.org/2022/sqlite-tools-osx-x86-3390400.zip
# https://www.sqlite.org/2022/sqlite-tools-win32-x86-3390400.zip

shCiArtifactUploadCustom() {(set -e
# this function will run custom-code to upload build-artifacts
    git fetch origin artifact
    git checkout origin/artifact "branch-$GITHUB_BRANCH0"
    mv "branch-$GITHUB_BRANCH0"/* .
    git add -f _sqlmath.* sqlmath_wasm.*
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
                ["jslint_ci.sh", "shBrowserScreenshot", url],
                {stdio: ["ignore", 1, 2]}
            ).on("exit", resolve);
        });
    }));
}());
' "$@" # '
)}

shCiBaseCustom() {(set -e
# this function will run custom-code for base-ci
    shCiEmsdkExport
    # .github_cache - restore
    if [ "$GITHUB_ACTION" ] && [ -d .github_cache ]
    then
        cp -a .github_cache/* . || true # js-hack - */
    fi
    # run nodejs-ci
    shCiBuildNodejs
    if (shCiMatrixIsmainName)
    then
        shImageLogoCreate &
        shCiBuildWasm
        # .github_cache - save
        if [ "$GITHUB_ACTION" ] && [ ! -d .github_cache/_emsdk ]
        then
            mkdir -p .github_cache
            cp -a "$EMSDK" .github_cache
        fi
    fi
    # upload artifact
    if (shCiMatrixIsmainNodeversion) && ( \
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
            if (node --input-type=module --eval '
import moduleChildProcess from "child_process";
(function () {
    moduleChildProcess.spawn(
        "sh",
        ["jslint_ci.sh", "shCiBaseCustomArtifactUpload"],
        {stdio: ["ignore", 1, 2]}
    ).on("exit", process.exit);
}());
' "$@") # '
            then
                break
            fi
        done
    fi
)}

shCiBaseCustomArtifactUpload() {(set -e
# this function will upload build-artifacts to branch-gh-pages
    COMMIT_MESSAGE="- upload artifact
- retry$GITHUB_UPLOAD_RETRY
- $GITHUB_BRANCH0
- $(printf "$GITHUB_SHA" | cut -c-8)
- $(uname)
"
    printf "\n\n$COMMIT_MESSAGE\n"
    # init .git/config
    git config --local user.email "github-actions@users.noreply.github.com"
    git config --local user.name "github-actions"
    # git clone origin/artifact
    rm -rf .tmp/artifact
    shGitCmdWithGithubToken clone origin .tmp/artifact \
        --branch=artifact --single-branch
    (
    cd .tmp/artifact/
    cp ../../.git/config .git/config
    # update dir branch-$GITHUB_BRANCH0
    mkdir -p "branch-$GITHUB_BRANCH0"
    cp ../../_sqlmath.* "branch-$GITHUB_BRANCH0"
    if [ -f ../../sqlmath_wasm.wasm ]
    then
        cp ../../sqlmath_wasm.* "branch-$GITHUB_BRANCH0"
    fi
    if [ -f ../../.artifact/asset_image_logo_512.png ]
    then
        cp ../../.artifact/asset_image_logo_* "branch-$GITHUB_BRANCH0"
    fi
    # git commit
    git add .
    git add -f "branch-$GITHUB_BRANCH0"/_sqlmath.*
    if (git commit -am "$COMMIT_MESSAGE")
    then
        # sync before push
        shGitCmdWithGithubToken pull origin artifact
        # git push
        if (shCiMatrixIsmainName) && [ "$GITHUB_BRANCH0" = alpha ]
        then
            shGitCommitPushOrSquash "" 20
        else
            shGitCmdWithGithubToken pull origin artifact
            shGitCmdWithGithubToken push origin artifact
        fi
    fi
    # debug
    shGitLsTree
    )
)}

shCiBuildext() {(set -e
# this function will build and compile c-extension
    unset npm_config_mode_test
    npm_config_mode_setup=1 node --input-type=module -e '
import {ciBuildext} from "./sqlmath.mjs";
ciBuildext({process});
' "$@" # '
)}

shCiBuildNodejs() {(set -e
# this function will build binaries in nodejs
    # cleanup
    rm -rf _sqlmath.* build/ sqlmath/_sqlmath.*
    mkdir -p build/
    shCiTestNodejs
)}

shCiBuildWasm() {(set -e
# this function will build binaries in wasm
    shCiEmsdkExport
    # install emsdk
    shCiEmsdkInstall
    # cd ${EMSDK} && . ./emsdk_env.sh && cd ..
    # build wasm
    printf "shCiBuildWasm\n" 1>&2
    OPTION1="$OPTION1 -Wall"
    OPTION1="$OPTION1 -flto"
    # debug
    # OPTION1="$OPTION1 -O0"
    OPTION1="$OPTION1 -Os"
    # OPTION2="$OPTION2 -Oz"
    # OPTION1="$OPTION1 -fsanitize=address"
    for FILE in \
        zlib_base.c \
        sqlite3_rollup.c \
        sqlite3_extfnc.c \
        sqlmath_base.c \
        sqlmath_custom.c
    do
        OPTION2=""
        FILE2="build/$(basename "$FILE").wasm.o"
        case "$FILE" in
        sqlite3_extfnc.c)
            FILE=sqlite3_rollup.c
            OPTION2="$OPTION2 -DSRC_SQLITE3_EXTFNC_C2="
            ;;
        zlib_base.c)
            FILE=sqlite3_rollup.c
            OPTION2="$OPTION2 -DSRC_ZLIB_BASE_C2="
            ;;
        esac
        # optimization - skip rebuild of sqlite3_rollup.c if possible
        if [ "$FILE2" -nt "$FILE" ] && [ "$FILE" = sqlite3_rollup.c ]
        then
            printf "shCiBuildWasm - skip $FILE\n" 1>&2
            continue
        fi
        OPTION2="$OPTION2 -DHAVE_UNISTD_H="
        OPTION2="$OPTION2 -DSQLITE3_C2="
        OPTION2="$OPTION2 -c $FILE -o $FILE2"
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
    OPTION2="$OPTION2 -s LLD_REPORT_UNDEFINED"
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
        -o build/sqlmath_wasm.js \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s ALLOW_TABLE_GROWTH=1 \
        -s NODEJS_CATCH_EXIT=0 \
        -s NODEJS_CATCH_REJECTION=0 \
        -s RESERVED_FUNCTION_POINTERS=64 \
        -s SINGLE_FILE=0 \
        -s WASM=1 \
        -s WASM_BIGINT \
        build/zlib_base.c.wasm.o \
        build/sqlite3_rollup.c.wasm.o \
        build/sqlite3_extfnc.c.wasm.o \
        build/sqlmath_base.c.wasm.o \
        build/sqlmath_custom.c.wasm.o \
        #
    printf '' > sqlmath_wasm.js
    printf "/*jslint-disable*/
// Copyright (c) 2021 Kai Zhu
// SPDX-License-Identifier: MIT
// $(date -u +"%Y-%m-%dT%H:%M:%S%z")
(function () {
\"use strict\";
" >> sqlmath_wasm.js
    cat build/sqlmath_wasm.js | tr -d "\r" >> sqlmath_wasm.js
    printf '
}());
/*jslint-enable*/
' >> sqlmath_wasm.js
    cp build/sqlmath_wasm.wasm .
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
    # touch "$EMSDK/.null.c"
    # emcc \
    #     -s USE_ZLIB \
    #     "$EMSDK/.null.c" -o "$EMSDK/.null_wasm.js"
)}

shIndentC() {(set -e
# this function will indent/prettify c file
    if (uname | grep -q "MING\|MSYS")
    then
        ./indent.exe \
            --blank-lines-after-commas \
            --braces-on-func-def-line \
            --break-function-decl-args \
            --break-function-decl-args-end \
            --dont-line-up-parentheses \
            --k-and-r-style \
            --line-length78 \
            --no-tabs \
            -bfde \
            $@
        dos2unix $@
    fi
)}

shLintPython() {(set -e
# this function will lint python file
    FILE_LIST="$@"
    (
    printf "\n\nlint ruff\n"
    OPTION=""
    # ANN flake8-annotations
    OPTION="$OPTION --ignore=ANN"
    # obsolete - one-blank-line-before-class (D203)
    # * 1 blank line required before class docstring
    OPTION="$OPTION --ignore=D203"
    # multi-line-summary-first-line (D212)
    # * Multi-line docstring summary should start at the first line
    OPTION="$OPTION --ignore=D212"
    # non-imperative-mood (D401)
    # * First line of docstring should be in imperative mood: "{first_line}"
    OPTION="$OPTION --ignore=D401"
    # docstring-starts-with-this (D404)
    # * First word of the docstring should not be "This"
    OPTION="$OPTION --ignore=D404"
    # commented-out-code (ERA001)
    # Commented-out code is dead code, and is often included inadvertently.
    OPTION="$OPTION --ignore=ERA001"
    # too-many-statements (PLR0915)
    # * Too many statements ({statements} > {max_statements})
    OPTION="$OPTION --ignore=PLR0915"
    # subprocess-without-shell-equals-true (S603)
    # * `subprocess` call: check for execution of untrusted input
    OPTION="$OPTION --ignore=S603"
    # start-process-with-partial-path (S607)
    # * Starting a process with a partial executable path
    OPTION="$OPTION --ignore=S607"
    # hardcoded-sql-expression (S608)
    # SQL injection is a common attack vector for web applications.
    OPTION="$OPTION --ignore=S608"
    # print (T201)
    # * `print` found
    OPTION="$OPTION --ignore=T201"
    OPTION="$OPTION --select=ALL"
    if [ "$npm_config_mode_lint_fix" ]
    then
        OPTION="$OPTION --fix"
    fi
    ruff check $OPTION $FILE_LIST
    ) &
    PID_LIST="$PID_LIST $!"
    #
    (
    printf "lint pycodestyle\n"
    OPTION="--ignore="
    # Unexpected indentation (comment) (E116)
    # Comments should be indented relative to the code in the block they are in.
    OPTION="$OPTION,E116"
    # At least two spaces before inline comment (E261)
    # Inline comments should have two spaces before them.
    OPTION="$OPTION,E261"
    # Line break occurred before a binary operator (W503)
    # Line breaks should occur after the binary operator to keep all variable
    # names aligned.
    OPTION="$OPTION,W503"
    pycodestyle $OPTION $FILE_LIST
    ) &
    PID_LIST="$PID_LIST $!"
    #
    shPidListWait shLintPython "$PID_LIST"
    printf "lint successful\n\n"
)}

shCiLintCustom() {(set -e
# this function will run custom-code to lint files
    if [ "$GITHUB_ACTION" ]
    then
        pip install pycodestyle ruff
    fi
    npm_config_mode_lint_fix=1 shLintPython \
        setup.py \
        sqlmath/__init__.py
)}

shCiNpmPublishCustom() {(set -e
# this function will run custom-code to npm-publish package
    # fetch artifact
    git fetch origin artifact --depth=1
    git checkout origin/artifact \
        "branch-beta/_sqlmath."* \
        "branch-beta/sqlmath_wasm"*
    cp -a branch-beta/_sqlmath.* .
    cp -a branch-beta/sqlmath_wasm.* .
    # npm-publish
    npm publish --access public
)}

shCiTestNodejs() {(set -e
# this function will run test in nodejs
    # init .tmp
    mkdir -p .tmp
    # rebuild c-module
    export npm_config_mode_test=1
    if [ "$npm_config_fast" != true ]
    then
        # lint c-file
        python cpplint.py \
            --filter=-whitespace/comments \
            sqlmath_base.c \
            sqlmath_custom.c \
            sqlmath_jenks.c
        # lint js-file
        node jslint.mjs .
        # create file Manifest.in
        if [ -d .git/ ]
        then
            git ls-tree -lr --name-only HEAD | sed "s|^|include |" > Manifest.in
        fi
        # build nodejs c-addon
        # build python c-extension
        python setup.py build_ext
    fi;
    # test nodejs
    rm -f *~ .*test.sqlite
    COVERAGE_EXCLUDE="--exclude=jslint.mjs"
    if (node --eval '
require("assert")(require("./package.json").name !== "sqlmath");
' >/dev/null 2>&1)
    then
        COVERAGE_EXCLUDE="$COVERAGE_EXCLUDE --exclude=sqlmath.mjs"
    fi
    # ugly-hack - github-action will flakily hang during test
    if [ "$GITHUB_ACTION" ] && (timeout --version &>/dev/null)
    then
        timeout 120 sh jslint_ci.sh \
            shRunWithCoverage $COVERAGE_EXCLUDE node test.mjs
    else
        shRunWithCoverage $COVERAGE_EXCLUDE node test.mjs
    fi
    # test python
    python setup.py test
)}

shSqlmathUpdate() {(set -e
# this function will update files with ~/Documents/sqlmath/
    . "$HOME/myci2.sh" : && shMyciUpdate
    if [ "$PWD/" = "$HOME/Documents/sqlmath/" ]
    then
        shRollupFetch asset_sqlmath_external_rollup.js
        shRollupFetch index.html
        shRollupFetch sqlite3_rollup.c
        shRollupFetch sqlmath/sqlmath_dbapi2.py
        git grep '3\.39\.[^4]' \
            ":(exclude)CHANGELOG.md" \
            ":(exclude)sqlite3_rollup.c" \
            || true
        git grep 'autoconf-[0-9]' | grep -v CHANGELOG \
            | grep -v '3390400' || true
        git grep 'sqlite.*version-[0-9]' | grep -v CHANGELOG \
            | grep -v '3\.39\.4' || true
        return
    fi
    if [ -d "$HOME/Documents/sqlmath/" ]
    then
        for FILE in \
            .ci.sh \
            asset_sqlmath_external_rollup.js \
            indent.exe \
            index.html \
            sqlite3_extension_functions.c \
            sqlite3_rollup.c \
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
