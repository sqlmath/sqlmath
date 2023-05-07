# Changelog

# Todo
- none

# v2023.5.1-beta
- ci - Rename shell-function shRawLibFetch() to shRollupFetch().
- sqlmath - migrate build-ext from python to nodejs with parallel compile
- sqlmath - add python c-extension
- sqlmath - streamline macros SQLITE3_C2, SQLITE_H2, SQLITE3EXT_H2, SQLMATH_C2 in prepartion for python-c-extension
- sqlmath - add file zlib_rollup.c so sqlmath can be compiled without external zlib-dependency
- sqlmath - add sql-function avg_ema()
- sqlmath - add sql-function fill_forward()

# v2023.4.22
- sqlmath - rename sql-function percentile() to quantile()
- ci - add shell-function shCiPreCustom2() to setup python in custom-ci
- ci - remove mandatory python ci
- sqlmath - update sql-function percentile() with off-by-one bugfix
- sqlmath - remove file sqlmath_fann.c
- sqlmath - add file sqlmath_fann.c

# v2023.3.21
- sqlmath - move c-function str99JsonAppendText() from file sqlmath_base.c to sqlite3.c
- sqlmath - merge file sqlite3_ext.c into sqlite3.c
- sqlmath - streamline str99 c-code
- sqlmath - add sql-functions jenks_concat(kk, col1, col2, ...), jenks_json(kk, jsonArray)
- chart - bugfix - fix special-character in col-name being sql-injected

# v2023.2.26
- chart - bugfix - use spy for reference previous-ydate instead of 1a_mybot
- sqlmath - add functions dbExecAndReturnLastJsonAsync(), dbExecAndReturnLastTextAsync()
- sqlmath - add sql-functions jfromfloat64array(), jtofloat64array()
- sqlite - update to sqlite v3.39.4
- ci - replace shell-function shGithubPushBackupAndSquash() with simplified shGitCommitPushOrSquash()
- ci - in windows-ci-env, alias node=node.exe instead of using winpty for pipes

# v2023.1.29
- ci - add auto-logo-creation in private repo
- ci - auto-create asset_image_logo_512.png from asset_image_logo_512.html
- jslint-ci - revamp auto-updating and add shell-function shGithubCheckoutRemote
- chart - remove unused external libraries chart.js, moment.js
- demo - bugfix - fix 1-week-chart ignoring friday before monday-holiday

# v2022.12.31
- chart - bugfix - fix intraday-weekly-chart truncating first datapoint
- demo - use intraday data for weekly chart
- demo - allow stock ivv to be used as reference if spy is unavailable
- demo - rename main-account from 01_mybot to 1a_mybot

# v2022.11.20
- ci - auto-update version-number in main mjs-module
- editor - update codemirror-editor to v5.65.10
- doc - add api-doc
- doc - document quickstart-build, quickstart-website
- sqlmath - bugfix - fix function dbFileImportOrExport() throwing incorrect error
- chart - improve ergonomics to attach database and import csv/json
- demo - add hotkeys ctrl-o to open database, ctrl-s to save database
- demo - streamline charts sector, subsector, stock into single logic
- sqlmath - remove unused sqlite-extensions carray, csv from file sqlite3_ext.c
- chart - add chart-options xstep, ystep
- ci - update to nodejs v18
- chart - redesign tooltip
- chart - add xvalueConvert option juliandayToDate
- chart - add dbtable-crud-operations dbtableImportCsv, dbtableImportJson
- chart - add intraday, 1-min chart

# v2022.10.20
- demo - add chart tradebot_historical_backtrack
- chart - allow hiding series by clicking it in plot-area
- coverage - add test-coverage for function sqlmathWebworkerInit()
- chart - use different color for category - short
- sqlmath - re-enable sql-function squared()

# v2022.9.20
- sqlmath - bugfix - handle null seriesList, xlabelList, xdata, ydata
- sqlmath - revamp str99 with sqlite3_str
- jslint - update jslint v2022.9.9
- sqlite - revert back to v3.38.5 due to performance-regression
- sqlmath - add sql-function tofloat64array()
- sqlite - update to sqlite v3.39.2

# v2022.8.20
- chart-revamp - switch from chart.js to using svg-elements for plots
--
- chart - add optional easeout ui-animation, in addition to linear
- chart - fix render-bug when zooming with no data
- chart - bugfix - fix xrangeMin being NaN
- chart - animate crosshair, similar to tooltip
- chart - remove gridband code
- chart - remove all classes, and instances of this
- chart - bugfix - fix x-axis having no padding
- chart - bugfix - fix chart failing to re-scale after hiding/showing series
- chart - fix tooltip-indent-bug
- chart - revamp tooltip-popup in barchart - part1
- chart - replace Tick class with static-functions uichartAxistickCreate and uichartAxistickRender
- chart - bugfix - fix tick-cleanup
- chart - fix seriestracker z-index in reverse-order of series-render
- chart - bugfix - fix annoying clip-path drifting down-right during startup-animation
- chart - remove jquery-dependency
- chart - fix again, zoomout cropping too much of one side, in line-charts
--
- remove unused files
    assets.bootstrap-v3.4.1.datatables-v1.10.21.chartjs-v2.9.4.codemirror-5.58.3.rollup.css
    assets.bootstrap-v3.4.1.datatables-v1.10.21.chartjs-v2.9.4.codemirror-5.58.3.rollup.js
    spa.sqlchart.html
    spa.sqlchart.js
- add dbrow-crud-operations dbrowDelete, dbrowInsert, dbrowUpdate
- chart - add charting functionality using chart.js
- add dbtable-crud-operations dbtableRename, dbcolumnAdd, dbcolumnRename, dbcolumnDrop
- demo - add web-demo in README.md
- webapp - add ui-loading when running onDbAction(), onDbExec()
- demo - add github-fork-banner

# v2022.6.30
- add dbtable-crud-operations dbquerySaveCsv, dbquerySaveJson, dbtableDrop, dbtableSaveCsv, dbtableSaveJson
- add contextmenu and dbtable-crud-operation dbtableDrop
- add database-crud-operations
- merge rendering of sql-queries and sql-tables into one
- optimization - defer rendering data in dttable until you scroll into it in viewport
- add modal to display sql-query errors
- bugfix - fix broken sorting
- merge datatables css into file index.html
- merge datatables code into file sqlmath_browser.mjs and remove jquery-dependency
- bugfix - fix ci-function shCiBuildWasm() not updating output sqlmath_wasm.wasm
- wasm - rewrite file sqlmath_wrapper_wasm.js to pass jslint
- datatables - rewrite datatables-function _fnDraw() for faster rendering
- sqlmath - milestone - functional index.html
- add files asset_sqlmath_external_rollup.css, asset_sqlmath_external_rollup.js
- bugfix - fix memory-leak by replacing all free()/malloc() with sqlite3_free()/sqlite3_malloc()
- wasm - update functions dbFileExportAsync(), dbFileImportAsync() to be able to export/import raw-db-arraybuffer
- remove obsolete csv-json-import functions like dbTableInsert(), superseded by json_each()
- wasm - update function dbOpen() to be able to import raw dbData arraybuffer
- merge function dbExecWithRetryAsync() into dbExecAsync()
- rename function dbMemoryLoadOrSave to dbFileImportOrExport()
- add file csslint.js
- wasm - replace sqljs-api with sqlmath-api in wasm - part1
- remove jslint-dependency in sqlmath-core to prepare sqlmath for browser/wasm env
- emscripten updated to v3.1.3, allowing es6 syntax in sqlmath_wrapper_wasm.js
- jenks - streamline jenksCreate() return object to simple double-array instead of struct
- jenks - inline jenks-function jenksCalcRange() from recursion to iteration
- jenks - optimize jenks-function jenksCalcRange() from recursion to iteration
- jenks - replace class-based api with static-function
- jsbaton - migrate function dbExec() to new unified jsbaton
- error - add sqlite errcode SQLITE_ERROR_ZSQL_NULL
- jsbaton - migrate function dbTableInsert() to new unified jsbaton
- jsbaton - migrate functions dbClose(), dbMemoryLoadOrSave(), dbNoop(), dbOpen() to new, streamlined jsbaton2
- add js-function jsbatonPushValue() to dynamically push value to jsbaton
- napi - decouple c-functions dbClose(), dbExec(), dbMemoryLoadOrSave(), dbNoop(), dbOpen(), dbTableInsert() from napi
- perf - reduce unnecessary copying, replacing SQLITE_TRANSIENT with SQLITE_STATIC
- sqlmath - wrap c-functions jsonInit(), jsonInitNoContext() with jsonInit2() that auto jsonGrow/malloc str99->zBuf
- sqlmath - streamline function cCall() to cCallAsync(), which only returns a promise
- wasm - migrate from sqljs-api to sqlmath-api in wasm - part1
- gc - replace node-specific-gc __dbFinalizerCreate() with more-webassembly-friendly FinalizationRegistry()
- sqlmath - add sql-extension copyblob(), matrix2d_concat()
- sqlmath - re-include carray-extension

# v2022.5.20
- sqlite - update to sqlite v3.38.5
- sqlmath - add sql-functions castrealorzero(), casttextorempty()
- sqlmath - migrate test to jsTestXxx
- jslint - update jslint v2022.05.13

# v2022.4.28
- jslint - update jslint v2022.4.28
- sqlite - update to sqlite v3.38.2
- sqlmath - bugfix - fix crash from sql-fnc kthpercentile(), when passed with empty-list

# v2022.3.20
- sqlmath - add builtin sql-function jenks() which is a fast 1d, ml classifier
- sqlmath - merge/streamline buildstep sqlmath_custom.c into sqlmath_base.c
- sqlmath - remove unused responseType = lastMatrixDouble
- sqlmath - export c-function dbExec to wasm
- sqlmath - replace Str99 with sqlite-builtin JsonString
- update to sqlite v3.38.1
- ci - add fileCount check
- sqlmath - re-enable building shell-executable
- sqlmath - use sqlite-builtin jsonstring instead of str99 for extension kthpercentile
- sqlmath - remove zlib.h and use handcoded headers instead
- split build-step sqlite3_c to sqlite3_c, sqlite3_ext_c, to deduplicate definition SQLITE3_C2
- add shell-function shCiBuildWasm() to build wasm binaries with online demo spa.sqlchart.html
- update to sqlite v3.38.0
- create github-branch-artifact to upload shared binaries

# v2022.3.5
- update jslint v2022.2.20
- add sqlite-function RANDOM1() generating random-float between 0 <= xx < 1
- remove file sqlmath_old.js
- add sql-function kthpercentile(val, percentile)
- update sql-functions compress(), uncompress() with null check/guard

# v2021.11.20
- npm publish v2021.10.20
- update to jslint v2021.10.20
- refactor - replace nodejs-buffer-api-calls with DataView / TextDecoder / TextEncoder

# v2021.10.20
- revamp - update to jslint_ci.sh from 2020.10.2

# v2021.9.9
- add sql-function slr_ohlcv() with tests
- update file jslint_ci.sh with chorish shell-functions
- update function dbOpenAsync with ability to create thread-pool with multiple connections

# v2021.9.8
- merge files in sqlite-autoconf-3360000 into sqlite3.c, sqlite3_ext.c, sqlite3_shell.c

# v2021.9.4
- add functions dbGetLastBlob, dbTableInsertAsync
- merge file sqlmath_napi.c into sqlmath.c
- merge file test.js into sqlmath.mjs
- migrate repo to https://github.com/sqlmath/sqlmath
- migrate to esm and rename sqlmath.js to sqlmath.mjs

# v2021.8.26
- merge files sqlmath.h and sqlmath_napi.c into sqlmath.c
- add async functions dbOpen, dbExec, dbClose
- inline function sqlite3_extension_functions_init without auto_extension
- disable warnings for all targets except sqlmath.c and sqlmath_napi.c
- fix build for darwin and win32.

# v2021.8.24
- add addon functions dbOpen(), dbClose(), dbExec(). but ci works only in linux.

# v2021.8.16
- add extension-function b64decode()
- add c-function sqlite3_exec_get_tables().
- add target_cli.
- binaries - upload darwin/linux/win32 binaries to gh-pages.
- begin compiling sqlmath.c.
- jslint file sqlmath.js.
- merge file test-old-prepare.db into test-old.js.

# v2021.8.6
- add coverage testing.
- add extension-function support.
- fork repo as sqlmath.

## 5.0.2
- build: rebuild binaries before publishing [#1426](https://github.com/mapbox/node-sqlite3/pull/1426)

## 5.0.1
- dep: node-addon-api to ^3.0.0 [#1367](https://github.com/mapbox/node-sqlite3/pull/1367)
- bug: bad comparison of c string [#1347](https://github.com/mapbox/node-sqlite3/pull/1347)
- build: Install files to be deployed [#1352](https://github.com/mapbox/node-sqlite3/pull/1352)
- sqlite3: upgrade to 3.32.3 [#1351](https://github.com/mapbox/node-sqlite3/pull/1351)
- bug: worker threads crash [#1367](https://github.com/mapbox/node-sqlite3/pull/1367)
- bug: segfaults [#1368](https://github.com/mapbox/node-sqlite3/pull/1368)
- typo: broken link to MapBox site [#1369](https://github.com/mapbox/node-sqlite3/pull/1369)

## 5.0.0
- prebuilt: Node 14 support, dropped support for all version of Node < 10 [#1304](https://github.com/mapbox/node-sqlite3/pull/1304)
- prebuilt: add electron 7.2 [#1324](https://github.com/mapbox/node-sqlite3/pull/1324)
- napi: refactor codebase to use N-API instead of NAN (+ various improvements) [#1304](https://github.com/mapbox/node-sqlite3/pull/1304)
- trace: don't require throw to add trace info for verbose [#1317](https://github.com/mapbox/node-sqlite3/pull/1317)
- ci: remove permission setting [#1319](https://github.com/mapbox/node-sqlite3/pull/1319)

## 4.2.0
- electron: Electron v8, v8.1.x & v8.2.x [#1294](https://github.com/mapbox/node-sqlite3/pull/1294) [#1308](https://github.com/mapbox/node-sqlite3/pull/1308)
- sqlite3: update to 3.31.1 (3310100) [#1289](https://github.com/mapbox/node-sqlite3/pull/1289)
- webpack: split sqlite3-binding.js out so that it could be override by webpack [#1268](https://github.com/mapbox/node-sqlite3/pull/1268)
- sqlite3: enable 'SQLITE_ENABLE_DBSTAT_VTAB=1' [#1281](https://github.com/mapbox/node-sqlite3/pull/1281)
- deps: remove request [#1287](https://github.com/mapbox/node-sqlite3/pull/1287)
- deps: alternative update of node-gyp for electron (v1 - v4), windows [#1283](https://github.com/mapbox/node-sqlite3/pull/1283)
- electron: fix dist url [#1282](https://github.com/mapbox/node-sqlite3/pull/1282)
- docs: Added json1 support note [#1303](https://github.com/mapbox/node-sqlite3/pull/1303)

## 4.1.1
- Electron v6.1 and v7 support [#1237](https://github.com/mapbox/node-sqlite3/pull/1237)
- Electron v7.1 support [#1254](https://github.com/mapbox/node-sqlite3/pull/1254)
- SQLite3 update to 3.30.1 [#1238](https://github.com/mapbox/node-sqlite3/pull/1238)
- Overwrite 'msbuild_toolset' only if 'toolset' is defined [#1242](https://github.com/mapbox/node-sqlite3/pull/1242)
- Upgrade CI to node-gyp 6.x for Windows Electron v5 & v6 builds [#1245](https://github.com/mapbox/node-sqlite3/pull/1245)
- Node v13 support [#1247](https://github.com/mapbox/node-sqlite3/pull/1247)
- Use minimum supported node version for Electron 7 [#1255](https://github.com/mapbox/node-sqlite3/pull/1255)

## 4.1.0

- Electron v6 support [#1195](https://github.com/mapbox/node-sqlite3/pull/1195)
- Electron v4.1 and v4.2 support [#1180](https://github.com/mapbox/node-sqlite3/pull/1180)
- Custom file header with `--sqlite_magic` [#1144](https://github.com/mapbox/node-sqlite3/pull/1144)
- https everywhere [#1177](https://github.com/mapbox/node-sqlite3/pull/1177)

## 4.0.9
- Use trusty as the base for prebuilts [#1167](https://github.com/mapbox/node-sqlite3/pull/1167)

## 4.0.8
- Rerelease of 4.0.7 but removed excess .vscode files [0df90c7](https://github.com/mapbox/node-sqlite3/commit/0df90c7811331169ad5f8fbad396422e72757af3)

## 4.0.7

- Node v12 support
- Electron v5 support
- Fix backup API tests
- HAVE_USLEEP=1 for all platforms
- docker suport

## 4.0.6
- Release of 4.0.5 (again due CI)

## 4.0.5
- **SECURITY:** Upgrade SQLite to 3.26.0 [#1088](https://github.com/mapbox/node-sqlite3/pull/1088)
- add constants for file open (shared databases) [#1078](https://github.com/mapbox/node-sqlite3/pull/1078)
- Allow specifying the python to use [#1089](https://github.com/mapbox/node-sqlite3/pull/1089)

## 4.0.4
- Add NodeJS 11 support [#1072](https://github.com/mapbox/node-sqlite3/pull/1072)
- Add electron osx 3.0.0 support [#1071](https://github.com/mapbox/node-sqlite3/pull/1071)

## 4.0.3

- Increase electron/osx binary coverage [#1041](https://github.com/mapbox/node-sqlite3/pull/1041) (@kewde)

## 4.0.2

- Fixed HTTP proxy support by using `request` over `needle` in node-pre-gyp

## 4.0.1

- Node v10 support
- Upgrade to node-pre-gyp@0.10.1
- Upgrade to nan@2.10.0
- Upgrade to sqlite v3.24.0
- Stopped bundling node-pre-gyp
- Upgrade to mocha@5
- Now building electron binaries (@kewde)
- Add OPEN_FULLMUTEX constant

## 4.0.0

 - Drop support for Node v0.10 and v.12
 - Upgrade to node-pre-gyp@0.9.0
 - Upgrade to nan@2.9.2

## 3.1.13

- Attempt to fix regression of #866

## 3.1.12

- Fixed to ensure the binaries do not rely on `GLIBC_2.14` and only `GLIBC_2.2.5`. This regressed in v3.1.11.

## 3.1.11

- Fixed building from source on alpine linux

## 3.1.10

- Removed `npm ls` from `prepublish` hook per mapbox/node-pre-gyp#291
- Upgraded node-pre-gyp to v0.6.37
- Removed accidentally committed large file

## 3.1.9

- Added support for node v8 and upgraded `nan`, `node-pre-gyp` deps.

## 3.1.8

- Added support for node v7 (pre-compiled binaries available)

## 3.1.7

- Upgrade sqlite to 3.15, enable FTS4, FTS5 (@wmertens)
- Upgrade to node-pre-gyp@0.6.31 and nan@2.4.0

## 3.1.6

- Starts bundling node-pre-gyp again to avoid #720

## 3.1.5

- [Added support for sqlite3_interrupt](https://github.com/mapbox/node-sqlite3/pull/518): this makes
  it possible to interrupt a long-running query.
- [Fixes uv_ref race](https://github.com/mapbox/node-sqlite3/pull/705).

## 3.1.4

 - Added support for node v6

## 3.1.3

 - Upgrade to node-pre-gyp@0.6.26 with better support for Electron

## 3.1.2

 - Only providing binaries for node v0.10x, v0.12.x, v4, and v5
 - Upgrade to nan@2.2.x
 - Upgrade to node-pre-gyp@0.6.24


## 3.1.1

 - Support for node 5.x
 - Upgraded SQLite to 3.9.1: https://www.sqlite.org/releaselog/3_9_1.html
 - Enabled json1 extension by default

## 3.1.0

 - Support for node 3.x and 4.x
 - Stopped producing binaries for node-webkit and 32 bit linux

## 3.0.11

 - Support for io.js 3.x (upgrade to Nan 2.x) @kkoopa

## 3.0.10

 - Upgraded SQLite to 3.8.11.1: https://www.sqlite.org/releaselog/3_8_11_1.html
 - Fixed binary compatibility regression with old centos/rhel glibc GLIBC_2.14 (re-introduced alpine linux (musl) build regression)
 - Now providing binaries against Visual Studio 2015 (pass --toolset=v140) and use binaries from https://github.com/mapbox/node-cpp11

## 3.0.9

 - Fixed build regression against alpine linux (musl)
 - Upgraded node-pre-gyp@0.6.8

## 3.0.8

 - Fixed build regression against FreeBSD
 - Upgraded node-pre-gyp@0.6.7

## 3.0.7

 - Fixed build regression against ARM and i386 linux
 - Upgraded node-pre-gyp@0.6.6
 - Added support for io.js 2.0.0

## 3.0.6

 - Upgraded node-pre-gyp@0.6.5
 - Upgraded nan@1.8.4
 - Fixed binaries to work on older linux systems (circa GLIBC_2.2.5 like centos 6) @bnoordhuis
 - Updated internal libsqlite3 from 3.8.7.1 -> 3.8.9 (https://www.sqlite.org/news.html)

## 3.0.5

 - IO.js and Node v0.12.x support.
 - Node-webkit v0.11.x support regressed in this release, sorry (https://github.com/mapbox/node-sqlite3/issues/404).

## 3.0.4

 - Upgraded node-pre-gyp@0.6.1

## 3.0.3

 - Upgraded to node-pre-gyp@0.6.0 which should fix crashes against node v0.11.14
 - Now providing binaries against Visual Studio 2014 (pass --toolset=v140) and use binaries from https://github.com/mapbox/node-cpp11

## 3.0.2

 - Republish for possibly busted npm package.

## 3.0.1

 - Use ~ in node-pre-gyp semver for more flexible dep management.

## 3.0.0

Released September 20nd, 2014

 - Backwards-incompatible change: node versions 0.8.x are no longer supported.
 - Updated to node-pre-gyp@0.5.27
 - Updated NAN to 1.3.0
 - Updated internal libsqlite3 to v3.8.6

## 2.2.7

Released August 6th, 2014

 - Removed usage of `npm ls` with `prepublish` target (which breaks node v0.8.x)

## 2.2.6

Released August 6th, 2014

 - Fix bundled version of node-pre-gyp

## 2.2.5

Released August 5th, 2014

 - Fix leak in complete() callback of Database.each() (#307)
 - Started using `engineStrict` and improved `engines` declaration to make clear only >= 0.11.13 is supported for the 0.11.x series.

## 2.2.4

Released July 14th, 2014

 - Now supporting node v0.11.x (specifically >=0.11.13)
 - Fix db opening error with absolute path on windows
 - Updated to node-pre-gyp@0.5.18
 - updated internal libsqlite3 from 3.8.4.3 -> 3.8.5 (https://www.sqlite.org/news.html)

## 2.2.3

 - Fixed regression in v2.2.2 for installing from binaries on windows.

## 2.2.2

 - Fixed packaging problem whereby a `config.gypi` was unintentially packaged and could cause breakages for OS X builds.

## 2.2.1

 - Now shipping with 64bit FreeBSD binaries against both node v0.10.x and node v0.8.x.
 - Fixed solaris/smartos source compile by passing `-std=c99` when building internally bundled libsqlite3 (#201)
 - Reduced size of npm package by ignoring tests and examples.
 - Various fixes and improvements for building against node-webkit
 - Upgraded to node-pre-gyp@0.5.x from node-pre-gyp@0.2.5
 - Improved ability to build from source against `sqlcipher` by passing custom library name: `--sqlite_libname=sqlcipher`
 - No changes to C++ Core / Existing binaries are exactly the same

## 2.2.0

Released Jan 13th, 2014

 - updated internal libsqlite3 from 3.7.17 -> 3.8.2 (https://www.sqlite.org/news.html) which includes the next-generation query planner https://www.sqlite.org/queryplanner-ng.html
 - improved binary deploy system using https://github.com/springmeyer/node-pre-gyp
 - binary install now supports http proxies
 - source compile now supports freebsd
 - fixed support for node-webkit

## 2.1.19

Released October 31st, 2013

 - Started respecting `process.env.npm_config_tmp` as location to download binaries
 - Removed uneeded `progress` dependency

## 2.1.18

Released October 22nd, 2013

 - `node-sqlite3` moved to mapbox github group
 - Fixed reporting of node-gyp errors
 - Fixed support for node v0.6.x

## 2.1.17
 - Minor fixes to binary deployment

## 2.1.16
 - Support for binary deployment

## 2.1.15

Released August 7th, 2013

 - Minor readme additions and code optimizations
