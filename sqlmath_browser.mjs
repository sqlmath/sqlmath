/*jslint beta, browser, devel, nomen*/
import {
    assertOrThrow,
    dbCloseAsync,
    dbExecAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbOpenAsync,
    debugInline,
    noop,
    sqlmathWebworkerInit
} from "./sqlmath.mjs";

let BLOB_SAVE;
let {
    CodeMirror
} = globalThis;
let DBTABLE_DICT = new Map();
let DB_DICT = new Map();
let DB_II = 1;
let DB_MAIN;
let DB_QUERY;
let DEBOUNCE_DICT = Object.create(null);
let UI_CONTEXTMENU = document.getElementById("contextmenu1");
let UI_CONTEXTMENU_BATON;
let UI_EDITOR;
let UI_FILE_OPEN = document.createElement("input");
let UI_FILE_SAVE = document.createElement("a");
let UI_PAGE_SIZE = 256;
let UI_ROW_HEIGHT = 16;
let UI_VIEW_SIZE = 10;

noop(
    UI_EDITOR,
    assertOrThrow,
    dbFileExportAsync,
    debugInline
);

async function dbFileAttachAsync({
    db,
    dbData
}) {
// this function will attach database <dbData> to <db>
    let dbAttached;
    let dbName;
    DB_II += 1;
    dbName = `[db${String(DB_II).padStart(2, "0")}]`;
    dbAttached = await dbOpenAsync({
        dbData,
        filename: `file:${dbName.slice(1, -1)}?mode=memory&cache=shared`
    });
    dbAttached.dbName = dbName;
    await dbExecAsync({
        db,
        sql: `ATTACH DATABASE '${dbAttached.filename}' AS ${dbName}`
    });
    DB_DICT.set(dbName, dbAttached);
}

function debounce(key, func) {
// this function will debounce <func> with given <key>
    let val = DEBOUNCE_DICT[key];
    if (val) {
        val.func = func;
        return;
    }
    val = {
        func: noop,
        timerTimeout: setTimeout(function () {
            delete DEBOUNCE_DICT[key];
            val.func();
        }, 500)
    };
    DEBOUNCE_DICT[key] = val;
    // if first-time, then immediately call <func>
    func();
}

function domDivCreate(innerHTML) {
// this function will return div-element with rendered <innerHTML>
    let elem = document.createElement("div");
    elem.innerHTML = innerHTML;
    return elem;
}

function fileSave({
    buf,
    filename
}) {
// this function will save <buf> with given <filename>
    // cleanup previous blob to prevent memory-leak
    URL.revokeObjectURL(BLOB_SAVE);
    //!! uiLoaderStart({});
    // create new blob
    BLOB_SAVE = URL.createObjectURL(new Blob([
        buf
    ]));
    UI_FILE_SAVE.href = BLOB_SAVE;
    // cleanup blob to prevent memory-leak
    setTimeout(function () {
        URL.revokeObjectURL(UI_FILE_SAVE.href);
    }, 30000);
    UI_FILE_SAVE.download = filename.toLowerCase().replace((
        /[^0-9a-z]+/g
    ), "_").replace((
        /_([^_]+)$/
    ), (
        "_"
        + new Date().toISOString().slice(0, 10).replace((
            /-/g
        ), "")
        + ".$1"
    ));
    //!! uiLoaderEnd({});
    UI_FILE_SAVE.click();
}

async function init() {
    // init db - query
    DB_QUERY = {
        dbName: "query",
        dbtableList: [],
        isDbquery: true
    };
    DB_DICT.set("query", DB_QUERY);
    // init db - main
    await sqlmathWebworkerInit({});
    DB_MAIN = Object.assign(noop(
        await dbOpenAsync({
            filename: ":memory:"
        })
    ), {
        dbName: "[main]",
        isDbmain: true
    });
    DB_DICT.set("[main]", DB_MAIN);
    // init UI_FILE_OPEN
    UI_FILE_OPEN.type = "file";
    // init sqlEditor
    UI_EDITOR = CodeMirror.fromTextArea(document.querySelector(
        "#sqliteEditor1"
    ), {
        extraKeys: {
            Tab: function (cm) {
                cm.replaceSelection(
                    new Array(cm.getOption("indentUnit") + 1).join(" ")
                );
            }
        },
        lineNumbers: true,
        lineWrapping: true,
        matchBrackets: true,
        mode: "text/x-mysql"
    });
    // init event-handling
    [
        ["#dbExec2", "click", onDbExec],
        ["#tocPanel1", "click", onDbAction],
        [".modalClose", "click", onModalClose],
        ["body", "click", onContextmenu],
        ["body", "contextmenu", onContextmenu],
        [UI_FILE_OPEN, "change", onDbAction]
    ].forEach(function ([
        selector, evt, listener
    ]) {
        if (typeof selector !== "string") {
            selector.addEventListener(evt, listener);
            return;
        }
        selector = document.querySelectorAll(selector);
        assertOrThrow(selector.length > 0);
        selector.forEach(function (elem) {
            elem.addEventListener(evt, listener);
        });
    });
    document.addEventListener("keyup", onKeyUp);
    window.addEventListener("hashchange", uitableInitWithinView);
    window.addEventListener("scroll", uitableInitWithinView);
    window.scroll(0, 0);
    // attach demo-db
    await dbFileAttachAsync({
        db: DB_MAIN,
        dbData: new ArrayBuffer(0)
    });
    // exec demo-sql-query
    onDbExec({});
}

function jsonHtmlSafe(obj) {
// this function will make <obj> html-safe
// https://stackoverflow.com/questions/7381974
    return JSON.parse(JSON.stringify(obj).replace((
        /&/gu
    ), "&amp;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;"));
}

function onContextmenu(evt) {
// this function will handle contextmenu-event
    let baton;
    let {
        clientX,
        clientY,
        ctrlKey,
        datasetCm,
        metaKey,
        shiftKey,
        target,
        type
    } = evt;
    // contextmenu - left-click
    if (type !== "contextmenu") {
        // contextmenu - hide
        UI_CONTEXTMENU.style.display = "none";
        // contextmenu - action
        onDbAction(evt);
        return;
    }
    // contextmenu - right-click
    // contextmenu - enable default
    if (ctrlKey || metaKey || shiftKey) {
        return;
    }
    // contextmenu - disable default
    evt.preventDefault();
    evt.stopPropagation();
    // init target
    target = target.closest(".tocElemA[data-dbtype]");
    // contextmenu - hide
    if (!target) {
        UI_CONTEXTMENU.style.display = "none";
        return;
    }
    // init UI_CONTEXTMENU_BATON
    UI_CONTEXTMENU_BATON = DBTABLE_DICT.get(target.dataset.hashtag) || {};
    baton = UI_CONTEXTMENU_BATON;
    // show / hide .contextmenuElem
    datasetCm = target.dataset;
    UI_CONTEXTMENU.querySelectorAll(
        ".contextmenuDivider, .contextmenuElem"
    ).forEach(function ({
        dataset,
        style
    }) {
        style.display = "none";
        if (
            dataset.dbtype === "all"
            || (dataset.dbtype && dataset.dbtype === datasetCm.dbtype)
        ) {
            if (baton.isDbmain && dataset.action === "dbDetach") {
                return;
            }
            style.display = "block";
        }
    });
    // contextmenu - show
    UI_CONTEXTMENU.children[1].textContent = (
        `${baton.dbtableFullname || "script editor"}`
    );
    UI_CONTEXTMENU.style.display = "block";
    UI_CONTEXTMENU.style.left = Math.max(0, Math.min(
        clientX,
        window.innerWidth - UI_CONTEXTMENU.offsetWidth - 10
    )) + "px";
    UI_CONTEXTMENU.style.top = Math.max(0, Math.min(
        clientY,
        window.innerHeight - UI_CONTEXTMENU.offsetHeight - 20
    )) + "px";
}

async function onDbAction(evt) {
// this function will open db from file
    let baton;
    let data;
    let target = evt.target.closest("[data-action]") || evt.target;
    if (target.dataset.action) {
        evt.preventDefault();
        evt.stopPropagation();
    }
    baton = UI_CONTEXTMENU_BATON;
    switch (target === UI_FILE_OPEN && target.dataset.action) {
    case "dbAttach":
        if (target.files.length === 0) {
            return;
        }
        await dbFileAttachAsync({
            db: DB_MAIN,
            dbData: (
                await target.files[0].arrayBuffer()
            )
        });
        await uiRenderDb();
        return;
    case "dbOpen":
        if (target.files.length === 0) {
            return;
        }
        await dbFileImportAsync({
            db: DB_MAIN,
            dbData: (
                await target.files[0].arrayBuffer()
            )
        });
        await uiRenderDb();
        return;
    case "dbscriptOpen":
        UI_EDITOR.setValue(
            await target.files[0].text()
        );
        await uiRenderDb();
        return;
    }
    switch (target.dataset.action) {
    case "dbAttach":
    case "dbOpen":
    case "dbscriptOpen":
        UI_FILE_OPEN.dataset.action = target.dataset.action;
        UI_FILE_OPEN.value = "";
        UI_FILE_OPEN.click();
        return;
    case "dbDetach":
        if (!window.confirm(
            "are you sure you want to detach and close database"
            + ` ${baton.dbName} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: DB_MAIN,
            sql: `DETACH ${baton.dbName};`
        });
        await dbCloseAsync({
            db: baton.db
        });
        await uiRenderDb();
        return;
    case "dbExec":
        await onDbExec({});
        return;
    case "dbExport":
        data = await dbFileExportAsync({
            db: baton.db
        });
        data = data[6];
        fileSave({
            buf: data,
            filename: `sqlite_database_${baton.dbName}.sqlite`
        });
        return;
    case "dbRefresh":
        await uiRenderDb();
        return;
    case "dbquerySaveCsv":
        data = rowListToCsv({
            colList: baton.colList,
            rowList: baton.rowList0
        });
        fileSave({
            buf: data,
            filename: `sqlite_${baton.dbtableFullname}.csv`
        });
        return;
    case "dbquerySaveJson":
        data = rowListToJson({
            colList: baton.colList,
            rowList: baton.rowList0
        });
        fileSave({
            buf: data,
            filename: `sqlite_${baton.dbtableFullname}.json`
        });
        return;
    case "dbscriptSave":
        fileSave({
            buf: UI_EDITOR.getValue(),
            filename: `sqlite_script.json`
        });
        return;
    case "dbtableDrop":
        if (!window.confirm(
            `are you sure you want to drop table ${baton.dbtableFullname} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: baton.db,
            sql: `DROP TABLE ${baton.dbtableName};`
        });
        await uiRenderDb();
        return;
    case "dbtableSaveCsv":
        data = await dbExecAsync({
            db: baton.db,
            responseType: "list",
            sql: `SELECT rowid, * FROM ${baton.dbtableName};`
        });
        data = data[0] || [];
        data.shift();
        data = rowListToCsv({
            colList: baton.colList,
            rowList: data
        });
        fileSave({
            buf: data || "",
            filename: `sqlite_table_${baton.dbtableName}.csv`
        });
        return;
    case "dbtableSaveJson":
        data = await dbExecAsync({
            db: baton.db,
            sql: `SELECT rowid, * FROM ${baton.dbtableName};`
        });
        data = JSON.stringify(data[0] || []);
        fileSave({
            buf: data,
            filename: `sqlite_table_${baton.dbtableName}.json`
        });
        return;
    }
}

async function onDbExec({
    modeTryCatch,
    sql
}) {
// this function will
// 1. exec sql-command in webworker
// 2. ui-render sql-queries to html
    let dbtableList0;
    if (!modeTryCatch) {
        try {
            await onDbExec({
                modeTryCatch: true,
                sql
            });
        } catch (err) {
            console.error(err);
            document.querySelector(
                "#errorPanel1 .modalContent"
            ).textContent = err;
            document.querySelector(
                "#errorPanel1"
            ).style.display = "block";
        }
        return;
    }
    // close error modal
    document.querySelector("#errorPanel1").style.display = "none";
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        colList,
        isDbquery,
        rowList0
    }, key) {
        if (isDbquery) {
            colList.length = 0;
            rowList0.length = 0;
            DBTABLE_DICT.delete(key);
        }
    });
    DB_QUERY.dbtableList0 = [];
    // 1. exec sql-command in webworker
    dbtableList0 = await dbExecAsync({
        db: DB_MAIN,
        responseType: "list",
        sql: sql || UI_EDITOR.getValue()
    });
    // 2. ui-render sql-queries to html
    dbtableList0 = dbtableList0.map(function (rowList0, ii) {
        let colList = rowList0.shift();
        return {
            colList,
            dbtableFullname: `query result ${ii + 1}`,
            rowCount: rowList0.length,
            rowList0
        };
    });
    DB_QUERY.dbtableList0 = dbtableList0;
    await uiRenderDb();
}

function onKeyUp(evt) {
// this function will close current modal
    if (!evt.modeDebounce) {
        debounce(
            "onKeyUp",
            onKeyUp.bind(undefined, Object.assign(evt, {
                modeDebounce: true
            }))
        );
        return;
    }
    switch (evt.key) {
    case "Escape":
        // close error-modal
        document.querySelector("#errorPanel1").style.display = "none";
        return;
    }
    switch ((evt.ctrlKey || evt.metaKey) && evt.key) {
    case "Enter":
        onDbExec({});
        return;
    }
}

function onModalClose({
    currentTarget
}) {
// this function will close current modal
    currentTarget.closest(".modalPanel").style.display = "none";
}

function rowListToCsv({
    colList,
    rowList
}) {
// this function will convert json <rowList> to csv with given <colList>
    let data = JSON.stringify([[colList], rowList].flat(), undefined, 1);
    // convert data to csv
    data = data.replace((
        /\n  /g
    ), "");
    data = data.replace((
        /\n \[/g
    ), "");
    data = data.replace((
        /\n \],?/g
    ), "\r\n");
    data = data.slice(1, -2);
    // sqlite-strings are c-strings which should never contain null-char
    data = data.replace((
        /\u0000/g
    ), "");
    // hide double-backslash `\\\\` as null-char
    data = data.replace((
        /\\\\/g
    ), "\u0000");
    // escape double-quote `\\"` to `""`
    data = data.replace((
        /\\"/g
    ), "\"\"");
    // replace newline with space
    data = data.replace((
        /\\r\\n|\\r|\\n/g
    ), " ");
    // restore double-backslash `\\\\` from null-char
    data = data.replace((
        /\u0000/g
    ), "\\\\");
    return data;
}

function rowListToJson({
    colList,
    rowList
}) {
// this function will convert list-of-list <rowList> to list-of-object
// with given <colList>
    return JSON.stringify(rowList.map(function (row) {
        let dict = {};
        colList.forEach(function (col, ii) {
            dict[col] = row[ii];
        });
        return dict;
    }));
}

function stringHtmlSafe(str) {
// this function will make <str> html-safe
// https://stackoverflow.com/questions/7381974
    if (typeof str !== "string") {
        str = String(str);
    }
    return str.replace((
        /&/gu
    ), "&amp;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /"/gu
    ), "&quot;");
}

async function uiRenderDb() {
// this function will render #dbtableList1
    let dbList;
    let dbtableIi = 0;
    let html = "";
    let windowScrollY;
    // reset location.hash
    location.hash = "0";
    // save window.scrollY
    windowScrollY = window.scrollY;
    // DB_DICT - sync
    dbList = await dbExecAsync({
        db: DB_MAIN,
        sql: "PRAGMA database_list;"
    });
    dbList = new Set(dbList[0].map(function ({
        name
    }) {
        return `[${name}]`;
    }));
    DB_DICT.forEach(function ({
        dbName,
        isDbquery
    }) {
        if (!isDbquery && !dbList.has(dbName)) {
            DB_DICT.delete(dbName);
        }
    });
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        isDbquery
    }, key) {
        if (!isDbquery) {
            DBTABLE_DICT.delete(key);
        }
    });
    // DBTABLE_DICT - sync
    await Promise.all(Array.from(
        DB_DICT.values()
    ).map(async function (db) {
        let baton;
        let {
            dbName,
            isDbmain,
            isDbquery
        } = db;
        let dbtableList;
        let tmp;
        db.dbtableList = [];
        dbtableList = (
            db.isDbquery
            ? db.dbtableList0
            : noop(
                await dbExecAsync({
                    db,
                    sql: (`
SELECT * FROM sqlite_schema WHERE type = 'table' ORDER BY tbl_name;
                    `)
                })
            )[0]
        );
        if (!dbtableList) {
            return;
        }
        dbtableList = dbtableList.map(function ({
            colList = [],
            dbtableFullname,
            rowCount = 0,
            rowList0,
            sql,
            tbl_name
        }) {
            dbtableFullname = dbtableFullname || `${dbName}.[${tbl_name}]`;
            dbtableIi += 1;
            baton = {
                colList,
                db,
                dbName,
                dbtableFullname,
                dbtableName: `[${tbl_name}]`,
                hashtag: `dbtable_${String(dbtableIi).padStart(2, "0")}`,
                isDbmain,
                isDbquery,
                rowCount,
                rowList0,
                sql,
                title: (
                    isDbquery
                    ? `${dbtableFullname}`
                    : isDbmain
                    ? `table ${dbtableFullname}`
                    : `attached table ${dbtableFullname}`
                )
            };
            DBTABLE_DICT.set(baton.hashtag, baton);
            return baton;
        });
        if (!isDbquery) {
            tmp = "";
            dbtableList.forEach(function ({
                dbtableName,
                hashtag
            }) {
                tmp += (`
SELECT '${hashtag}' AS hashtag;
PRAGMA table_info(${dbtableName});
SELECT COUNT(*) AS rowcount FROM ${dbtableName};
                `);
            });
            tmp = await dbExecAsync({
                db,
                sql: tmp
            });
            tmp.forEach(function (rowList) {
                let row0 = rowList[0];
                if (!row0) {
                    return;
                }
                [
                    "cid", "hashtag", "rowcount"
                ].forEach(function (key) {
                    switch (row0.hasOwnProperty(key) && key) {
                    case "cid":
                        baton.colList = [
                            "rowid",
                            rowList.map(function ({
                                name
                            }) {
                                return name;
                            })
                        ].flat();
                        break;
                    case "hashtag":
                        baton = DBTABLE_DICT.get(row0.hashtag);
                        break;
                    case "rowcount":
                        baton.rowCount = row0.rowcount;
                        break;
                    }
                });
            });
        }
        db.dbtableList = dbtableList;
    }));
    // ui-render databases and tables to html
    document.querySelector("#sqlqueryList1").innerHTML = "";
    document.querySelector("#dbtableList1").innerHTML = "";
    DB_DICT.forEach(function (db) {
        db.dbtableList.forEach(function (baton) {
            // create uitable
            document.querySelector(
                db.isDbquery
                ? "#sqlqueryList1"
                : "#dbtableList1"
            ).appendChild(uitableCreate(baton));
        });
    });
    // ui-render #tocPanel1
    html = "";
    DB_DICT.forEach(function ({
        dbName,
        dbtableList,
        isDbmain,
        isDbquery
    }) {
        html += (
            `<div class="tocTitle">`
            + (
                isDbquery
                ? `sqlite query result`
                : isDbmain
                ? `database ${dbName}`
                : `database ${dbName} (attached)`
            )
            + `</div>`
        );
        dbtableList.forEach(function ({
            colList,
            dbtableFullname,
            hashtag,
            rowCount
        }) {
            html += `<a class="tocElemA"`;
            html += ` data-hashtag="${hashtag}"`;
            html += ` href="#${hashtag}"`;
            html += (
                isDbquery
                ? ` data-dbtype="query"`
                : ` data-dbtype="table"`
            );
            html += (
                ` title="`
                + stringHtmlSafe((
                    `right-click for crud operation\n\n`
                ) + JSON.stringify({
                    colList,
                    dbtableFullname,
                    rowCount
                }, undefined, 4))
                + `"`
            );
            html += `">`;
            html += stringHtmlSafe(dbtableFullname);
            html += `</a>\n`;
        });
    });
    document.querySelector("#tocDbList1").innerHTML = html;
    // restore window.scrollY
    window.scroll(0, windowScrollY);
    uitableInitWithinView({});
}

async function uitableAjax(baton, {
    rowList,
    type
}) {
    let {
        contentElem,
        db,
        elemInfo,
        elemLoading,
        elemScroller,
        elemTable,
        isDbquery,
        rowCount,
        rowList0,
        rowOffset,
        sortCol,
        sortDir
    } = baton;
    let html = "";
    let viewRowBeg;
    let viewRowEnd;
    if (baton.rowCount === 0) {
        // uitableLoading - hide
        elemLoading.style.display = "none";
        return;
    }
    switch (type) {
    // uitableScroll
    case "scroll":
    case "uitableInit":
        viewRowBeg = Math.max(0, Math.round(
            rowCount
            * elemScroller.scrollTop
            / (elemScroller.scrollHeight - 1 * UI_ROW_HEIGHT)
        ));
        viewRowEnd = Math.min(rowCount, Math.round(viewRowBeg + UI_VIEW_SIZE));
        // update table-view info
        elemInfo.textContent = (
            "Showing "
            + new Intl.NumberFormat("en-US").format(viewRowBeg + 1)
            + " to "
            + new Intl.NumberFormat("en-US").format(viewRowEnd)
            + " of "
            + new Intl.NumberFormat("en-US").format(rowCount)
            + " entries"
        );
        // skip expensive table-redraw, if scroll-point is within boundaries
        if (
            contentElem.dataset.init !== "0"
            && rowOffset <= Math.max(0, viewRowBeg - 1 * UI_VIEW_SIZE)
            && (
                Math.min(rowCount, viewRowEnd + 1 * UI_VIEW_SIZE)
                <= rowOffset + UI_PAGE_SIZE
            )
        ) {
            return;
        }
        // Do the uitable redraw based on the calculated start point
        baton.rowOffset = Math.max(0, Math.round(
            viewRowBeg + 0.5 * UI_VIEW_SIZE - 0.5 * UI_PAGE_SIZE
        ));
        break;
    }
    switch (type !== "uitableDraw" && baton.modeAjax) {
    // uitableAjax
    case 0:
        // uitableLoading - show
        elemLoading.style.display = "block";
        baton.modeAjax = 1;
        if (isDbquery) {
            if (sortDir) {
                rowList0.sort(function (aa, bb) {
                    aa = aa[sortCol];
                    bb = bb[sortCol];
                    return (
                        aa < bb
                        ? -1
                        : aa > bb
                        ? 1
                        : 0
                    );
                });
                if (sortDir === "desc") {
                    rowList0.reverse();
                }
            }
            rowList = rowList0.slice(
                rowOffset,
                rowOffset + UI_PAGE_SIZE
            );
        } else {
            // uitable - paginate
            rowList = await dbExecAsync({
                db,
                responseType: "list",
                sql: String(`
SELECT
        rowid,
        *
    FROM ${baton.dbtableName}
    ORDER BY
    LIMIT ${Number(UI_PAGE_SIZE)}
    OFFSET ${Number(rowOffset)};
                `).replace("ORDER BY", (
                    sortDir
                    ? `ORDER BY [${baton.colList[sortCol]}] ${sortDir}`
                    : ""
                ))
            });
            rowList = (
                rowList[0]
                ? rowList[0].slice(1)
                : []
            );
        }
        // recurse - draw
        await uitableAjax(baton, {
            rowList,
            type: "uitableDraw"
        });
        return;
    // debounce
    case 1:
        baton.modeAjax = 2;
        return;
    // debounce
    case 2:
        return;
    }
    // uitableDraw
    // Position the table in the virtual scroller
    elemTable.style.top = Math.max(0, Math.round(
        elemScroller.scrollHeight * rowOffset / (baton.rowCount + 1)
    )) + "px";
    // Insert the required TR nodes into the table for display
    jsonHtmlSafe(rowList).forEach(function (row) {
        html += "<tr>";
        row.forEach(function (val) {
            html += "<td>" + (val ?? "") + "</td>";
        });
        html += "</tr>";
    });
    elemTable.children[1].innerHTML = html;
    // debounce - throttle
    await new Promise(function (resolve) {
        setTimeout(resolve, 500);
    });
    // debounce - next
    if (baton.modeAjax === 2) {
        baton.modeAjax = 0;
        // keep focus on current scroller when debouncing
        if (type === "scroll") {
            elemScroller.focus();
        }
        await uitableAjax(baton, {});
        return;
    }
    // cleanup
    baton.modeAjax = 0;
    // uitableLoading - hide
    elemLoading.style.display = "none";
}

function uitableCreate(baton) {
// this function will create a dom-datatable-view of sqlite queries and tables
    let contentElem;
    let elemInfo;
    let elemLoading;
    let elemScroller;
    let elemTable;
    let elemTitle;
    // All uitables are wrapped in a div
    // Generate the node required for the processing node
    // The HTML structure that we want to generate in this function is:
    //  div - scroller
    //    div - scroll head
    //      div - scroll head inner
    //        table - scroll head table
    //          thead - thead
    //    div - scroll body
    //      table - table (master table)
    //        thead - thead clone for sizing
    //        tbody - tbody
    contentElem = domDivCreate(
        (`
<div class="contentElem" data-init="0" id="${baton.hashtag}">
<div class="contentElemTitle title">${stringHtmlSafe(baton.title)}</div>
<div class="uitablePageInfo">Showing 0 to 0 of 0 entries</div>
<div
    class="uitableScroller"
    style="height: ${(UI_VIEW_SIZE + 2) * UI_ROW_HEIGHT}px;"
    tabindex="-1"
>
    <div
        class="uitableScrollerDummy"
        style="height: ${baton.rowCount * UI_ROW_HEIGHT}px;"
    ></div>
    <table class="uitable">
        <thead>
            <tr>
        `)
        + jsonHtmlSafe(baton.colList).map(function (col) {
            return `<th title="${col}">${col}</th>`;
        }).join("")
        + (`
            </tr>
        </thead>
        <tbody>
            <tr>
            <td colspan="${baton.colList.length}">
            No data available in table
            </td>
            </tr>
        </tbody>
    </table>
</div>
<div class="uitableLoading">Loading</div>
</div>
        `)
    ).firstElementChild;
    [
        elemTitle,
        elemInfo,
        elemScroller,
        elemLoading
    ] = contentElem.children;
    elemTable = elemScroller.children[1];
    // init event-handling - sorting
    contentElem.querySelector(
        "thead tr"
    ).addEventListener("click", function (evt) {
        uitableSort(baton, evt);
    });
    // init event-handling - scrolling
    contentElem.querySelector(
        ".uitableScroller"
    ).addEventListener("scroll", function (evt) {
        uitableAjax(baton, evt);
    });
    contentElem.addEventListener("uitableInit", function (evt) {
        uitableAjax(baton, evt);
    });
    Object.assign(baton, {
        contentElem,
        elemInfo,
        elemLoading,
        elemScroller,
        elemTable,
        modeAjax: 0,
        modeInit: 0,
        rowOffset: 0
    });
    return contentElem;
}

function uitableInitWithinView({
    modeDebounce
}) {
// this function will defer-init uitables when visible in viewport
    if (!modeDebounce) {
        debounce(
            "uitableInitWithinView",
            uitableInitWithinView.bind(undefined, {
                modeDebounce: true
            })
        );
        return;
    }
    document.querySelectorAll(
        `#contentPanel1 .contentElem[data-init="0"]`
    ).forEach(function (elem) {
        let rect = elem.getBoundingClientRect();
        if (0 <= rect.bottom && rect.top < window.innerHeight) {
            elem.dispatchEvent(new window.CustomEvent("uitableInit"));
            elem.dataset.init = "1";
        }
    });
}

function uitableSort(baton, {
    currentTarget,
    target
}) {
// Function to run on user sort request
    let colIi;
    let direction;
    let elem = target.closest("th");
    if (!elem) {
        return;
    }
    direction = elem.dataset.sort;
    direction = (
        direction === "asc"
        ? "desc"
        : "asc"
    );
    Array.from(currentTarget.children).forEach(function (elemTh, ii) {
        if (elemTh !== elem) {
            elemTh.dataset.sort = "";
            return;
        }
        colIi = ii;
    });
    elem.dataset.sort = direction;
    baton.sortCol = colIi;
    baton.sortDir = direction;
    // Reset scroll to top in redraw.
    baton.elemScroller.scrollTop = 0;
    baton.rowOffset = 0;
    uitableAjax(baton, {});
}

// init
await init();
