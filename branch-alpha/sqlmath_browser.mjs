/*jslint beta, bitwise, browser, devel, nomen*/
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
    Chart,
    CodeMirror
} = globalThis;
let DBTABLE_DICT = new Map();
let DB_CHART;
let DB_DICT = new Map();
let DB_II = 0;
let DB_MAIN;
let DB_QUERY;
let DEBOUNCE_DICT = Object.create(null);
let UI_CHART_HEIGHT = 256;
let UI_CHART_LEGEND_WIDTH = 128;
let UI_CONTEXTMENU = document.getElementById("contextmenu1");
let UI_CONTEXTMENU_BATON;
let UI_CRUD = document.getElementById("crudPanel1");
let UI_EDITOR;
let UI_FILE_OPEN = document.createElement("input");
let UI_FILE_SAVE = document.createElement("a");
let UI_LOADING = document.getElementById("loadingPanel1");
let UI_LOADING_COUNTER = 0;
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
    dbName = `[attached_${String(DB_II).padStart(2, "0")}]`;
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
    UI_FILE_SAVE.click();
}

async function init() {
    await sqlmathWebworkerInit({});
    // init DB_CHART
    DB_CHART = Object.assign(noop(
        await dbOpenAsync({
            filename: "file:dbchart?mode=memory&cache=shared"
        })
    ), {
        dbName: "[chart]",
        isDbchart: true
    });
    DB_DICT.set("[chart]", DB_CHART);
    // init DB_QUERY
    DB_QUERY = Object.assign(noop(
        await dbOpenAsync({
            filename: ":memory:"
        })
    ), {
        dbName: "[query]",
        isDbquery: true
    });
    DB_DICT.set("[query]", DB_QUERY);
    // init DB_MAIN
    DB_MAIN = Object.assign(noop(
        await dbOpenAsync({
            filename: ":memory:"
        })
    ), {
        dbName: "[main]",
        isDbmain: true
    });
    DB_DICT.set("[main]", DB_MAIN);
    // attach DB_CHART
    await dbExecAsync({
        db: DB_MAIN,
        sql: `ATTACH DATABASE [${DB_CHART.filename}] AS chart;`
    });
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
        ["#tocPanel1", "click", onDbAction],
        [".dbExec", "click", onDbExec],
        [".dbcrudExec", "click", onDbcrudExec],
        [".modalCancel", "click", onModalClose],
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
        uiFadeOut(UI_CONTEXTMENU);
        // contextmenu - action
        if (target.closest(".contextmenuElem")) {
            onDbAction(evt);
        }
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
        uiFadeOut(UI_CONTEXTMENU);
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
    UI_CONTEXTMENU.children[0].innerHTML = (
        "crud operation for:<br>"
        + stringHtmlSafe(baton.dbtableFullname || "script editor")
    );
    uiFadeIn(UI_CONTEXTMENU);
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
    let action;
    let baton;
    let data;
    let target = evt.target.closest("[data-action]") || evt.target;
    let title;
    let uichart = target.closest(".uichart");
    action = target.dataset.action;
    if (!action) {
        return;
    }
    baton = UI_CONTEXTMENU_BATON;
    // fast actions that do not require loading
    switch (target !== UI_FILE_OPEN && action) {
    case "dbAttach":
    case "dbOpen":
    case "dbscriptOpen":
        UI_FILE_OPEN.dataset.action = action;
        UI_FILE_OPEN.value = "";
        UI_FILE_OPEN.click();
        return;
    case "dbcolumnAdd":
    case "dbcolumnDrop":
    case "dbcolumnRename":
    case "dbtableRename":
        title = target.textContent.trim().replace(/\s+/g, " ");
        UI_CRUD.querySelector(".modalTitle").innerHTML = (
            `${stringHtmlSafe(baton.dbtableFullname)}<br>${title}`
        );
        UI_CRUD.querySelector("tbody").innerHTML = (
            (`
<tr class="crudInput-new_table" style="display: none;">
    <td><span class="crudLabel">{{new_table}}</span></td>
    <td class="tdInput">
    <input class="crudInput" type="text" value="new_table_1">
    </td>
</tr>
<tr class="crudInput-selected_column" style="display: none;">
    <td><span class="crudLabel">{{selected_column}}</span></td>
    <td class="tdInput">
    <select class="crudInput">
            `)
            + baton.colList.slice(1).map(function (col) {
                return (
                    `<option>${stringHtmlSafe(col)}</option>`
                );
            }).join("")
            + (`
    </select>
    </td>
</tr>
<tr class="crudInput-new_column" style="display: none;">
    <td><span class="crudLabel">{{new_column}}</span></td>
    <td class="tdInput">
    <input class="crudInput" type="text" value="new_column_1">
    </td>
</tr>
            `)
        );
        UI_CRUD.querySelector("textarea").value = String(`
-- column - add
ALTER TABLE
    ${baton.dbtableFullname}
ADD
    "{{new_column}}" TEXT NOT NULL DEFAULT '';

-- column - drop
ALTER TABLE
    ${baton.dbtableFullname}
DROP COLUMN
    "{{selected_column}}";

-- column - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME
    "{{selected_column}}"
TO
    "{{new_column}}";

-- table - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME TO
    "{{new_table}}";
        `).trim().split("\n\n").filter(function (sql) {
            return sql.indexOf(title) >= 0;
        })[0] + "\n";
        switch (action) {
        case "dbcolumnAdd":
            UI_CRUD.querySelectorAll(
                ".crudInput-new_column"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        case "dbcolumnDrop":
            UI_CRUD.querySelectorAll(
                ".crudInput-selected_column"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        case "dbcolumnRename":
            UI_CRUD.querySelectorAll(
                ".crudInput-new_column, .crudInput-selected_column"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        case "dbtableRename":
            UI_CRUD.querySelectorAll(
                ".crudInput-new_table"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        }
        uiFadeIn(UI_CRUD);
        return;
    case "uichartDatasetHideAll":
    case "uichartDatasetShowAll":
        data = Number(action === "uichartDatasetHideAll");
        uichart.chart.data.datasets.forEach(function (dataset) {
            dataset.hidden = data;
        });
        uichart.querySelectorAll(".uichartLegendElem").forEach(function (elem) {
            elem.dataset.hidden = data;
        });
        uichart.chart.update();
        return;
    case "uichartDatasetHideOrShow":
        data = uichart.chart.data.datasets[target.dataset.ii].hidden ^ 1;
        uichart.chart.data.datasets[target.dataset.ii].hidden = data;
        target.dataset.hidden = data;
        uichart.chart.update();
        return;
    case "uichartZoomReset":
        uichart.chart.resetZoom();
        return;
    }
    // slow actions that require loading
    if (!evt.modeTryCatch) {
        evt.modeTryCatch = true;
        await uiTryCatch(onDbAction, evt);
        return;
    }
    evt.preventDefault();
    evt.stopPropagation();
    switch (target === UI_FILE_OPEN && action) {
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
    switch (action) {
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
    case "dbscriptSave":
        fileSave({
            buf: UI_EDITOR.getValue(),
            filename: `sqlite_script.sql`
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
    throw new Error(`onDbAction - invalid action ${action}`);
}

async function onDbExec({
    modeTryCatch
}) {
// this function will
// 1. exec sql-command in webworker
// 2. ui-render sql-queries to html
    let dbqueryList;
    if (!modeTryCatch) {
        await uiTryCatch(onDbExec, {
            modeTryCatch: true
        });
        return;
    }
    // close error modal
    uiFadeOut(document.querySelector("#errorPanel1"));
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        isDbchart,
        isDbquery
    }, key) {
        if (isDbchart || isDbquery) {
            DBTABLE_DICT.delete(key);
        }
    });
    await Promise.all([
        DB_CHART, DB_QUERY
    ].map(async function (db) {
        let sqlCleanup = noop(
            await dbExecAsync({
                db,
                sql: (`
BEGIN TRANSACTION;
SELECT
        group_concat('DROP TABLE [' || name || '];', '') AS sql
    FROM sqlite_master WHERE type = 'table';
END TRANSACTION;
                `)
            })
        )[0][0].sql || "";
        await dbExecAsync({
            db,
            sql: sqlCleanup
        });
    }));
    // 1. exec sql-command in webworker
    dbqueryList = await dbExecAsync({
        db: DB_MAIN,
        responseType: "list",
        sql: UI_EDITOR.getValue()
    });
    // 1a. save query-result
    await Promise.all(dbqueryList.map(async function (rowList, ii) {
        let colList = rowList.shift().map(function (col, ii) {
            return `value->>${ii} AS [${col}]`;
        }).join(",");
        await dbExecAsync({
            bindList: {
                tmp1: JSON.stringify(rowList)
            },
            db: DB_QUERY,
            sql: (`
BEGIN TRANSACTION;
CREATE TABLE result_${String(ii).padStart(2, "0")} AS
    SELECT
        ${colList}
    FROM json_each($tmp1);
END TRANSACTION;
            `)
        });
    }));
    // 2. ui-render sql-queries to html
    await uiRenderDb();
}

async function onDbcrudExec({
    modeTryCatch
}) {
// this function will exec crud operation
    let sql;
    if (!modeTryCatch) {
        await uiTryCatch(onDbcrudExec, {
            modeTryCatch: true
        });
        return;
    }
    sql = document.querySelector("#crudPanel1 textarea").value.replace((
        /\{\{\w+?\}\}/g
    ), function (match0) {
        let val = document.querySelector(
            `#crudPanel1 .crudInput-${match0.slice(2, -2)} .crudInput`
        );
        if (!val) {
            return match0;
        }
        if (val.tagName === "SELECT") {
            return val.selectedOptions[0].textContent;
        }
        return val.value.trim();
    });
    await dbExecAsync({
        db: DB_MAIN,
        sql
    });
    await uiRenderDb();
    uiFadeOut(UI_CRUD);
}

function onKeyUp(evt) {
// this function will handle event keyup
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
        uiFadeOut(document.querySelector("#errorPanel1"));
        // close contextmenu
        uiFadeOut(UI_CONTEXTMENU);
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
    uiFadeOut(currentTarget.closest(".modalPanel"));
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

function uiFadeIn(elem) {
// this function will fade-in <elem>
    elem.style.opacity = (
        elem === UI_CRUD
        ? "1"
        : "0.875"
    );
    elem.style.visibility = "visible";
}

function uiFadeOut(elem) {
// this function will fade-out <elem>
    elem.style.opacity = "0";
    elem.style.visibility = "hidden";
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
        isDbchart,
        isDbquery
    }) {
        if (!isDbchart && !isDbquery && !dbList.has(dbName)) {
            DB_DICT.delete(dbName);
        }
    });
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        isDbchart,
        isDbquery
    }, key) {
        if (!isDbchart && !isDbquery) {
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
            isDbchart,
            isDbmain,
            isDbquery
        } = db;
        let dbtableList;
        let tmp;
        db.dbtableList = [];
        dbtableList = noop(
            await dbExecAsync({
                db,
                sql: (`
SELECT * FROM sqlite_schema WHERE type = 'table' ORDER BY tbl_name;
                `)
            })
        )[0];
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
                isDbchart,
                isDbmain,
                isDbquery,
                rowCount,
                rowList0,
                sortCol: 0,
                sortDir: "asc",
                sql,
                title: `table ${dbtableFullname}`
            };
            DBTABLE_DICT.set(baton.hashtag, baton);
            return baton;
        });
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
        db.dbtableList = dbtableList;
    }));
    // ui-render databases and tables to html
    document.querySelector("#dbchartList1").innerHTML = "";
    document.querySelector("#dbqueryList1").innerHTML = "";
    document.querySelector("#dbtableList1").innerHTML = "";
    DB_DICT.forEach(function ({
        dbtableList,
        isDbchart,
        isDbquery
    }) {
        dbtableList.forEach(function (baton) {
            // create uitable
            document.querySelector(
                isDbchart
                ? "#dbchartList1"
                : isDbquery
                ? "#dbqueryList1"
                : "#dbtableList1"
            ).appendChild(uitableCreate(baton));
        });
    });
    // ui-render #tocPanel1
    html = "";
    DB_DICT.forEach(function ({
        dbName,
        dbtableList,
        isDbchart,
        isDbquery
    }) {
        html += `<div class="tocTitle">` + (
            isDbchart
            ? "chart"
            : isDbquery
            ? `query result`
            : `database ${dbName.slice(1, -1)}`
        ) + `</div>`;
        dbtableList.forEach(function ({
            colList,
            dbtableFullname,
            dbtableName,
            hashtag,
            rowCount
        }, ii) {
            html += `<a class="tocElemA"`;
            html += ` data-hashtag="${hashtag}"`;
            html += ` href="#${hashtag}"`;
            html += (
                isDbchart
                ? ` data-dbtype="chart"`
                : isDbquery
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
            html += `>${ii + 1}. `;
            html += (
                isDbchart
                ? "chart"
                : isDbquery
                ? "query"
                : "table"
            );
            html += ` ${stringHtmlSafe(dbtableName.slice(1, -1))}</a>\n`;
        });
    });
    document.querySelector("#tocDbList1").innerHTML = html;
    // restore window.scrollY
    window.scroll(0, windowScrollY);
    uitableInitWithinView({});
}

async function uiTryCatch(func, ...argList) {
// this function will call <func> in a try-catch-block
// that will display any error thrown to user
    try {
        UI_LOADING_COUNTER += 1;
        uiFadeIn(UI_LOADING);
        await func(...argList);
    } catch (err) {
        console.error(err);
        document.querySelector(
            "#errorPanel1 .modalContent"
        ).textContent = err;
        uiFadeIn(document.querySelector(
            "#errorPanel1"
        ));
    } finally {
        setTimeout(function () {
            UI_LOADING_COUNTER -= 1;
            if (
                UI_LOADING_COUNTER === 0
                && UI_LOADING.style.visibility === "visible"
            ) {
                uiFadeOut(UI_LOADING);
            }
        }, 500);
    }
}

async function uichartCreate(baton) {
// this function will create xy-line-chart from given sqlite table <baton>
    let chart;
    let colorList;
    let {
        contentElem,
        db,
        dbtableName,
        elemChart
    } = baton;
    let options;
    options = await dbExecAsync({
        db,
        sql: (`
SELECT
        json_insert(options, '$.data', json(data)) AS data
    FROM (SELECT options FROM ${dbtableName} LIMIT 1)
    JOIN (
        SELECT
                json_group_array(data) AS data
            FROM (
                SELECT
                    json_object(
                        'label', label,
                        'data', json_group_array(json_array(xx, yy))
                    ) AS data
                FROM (SELECT * FROM ${dbtableName} ORDER BY label, xx, yy)
                WHERE
                    label IS NOT NULL
                GROUP BY label
            )
    )
;
        `)
    });
    contentElem.querySelector(".uitable").style.display = "none";
    elemChart.style.display = "block";
    options = JSON.parse(options[0][0].data || []);
    if (options.title) {
        contentElem.querySelector(".contentElemTitle").textContent += (
            " - " + options.title
        );
    }
    options = {
        data: {
            datasets: options.data.map(function ({
                data,
                label
            }, ii) {
                return {
                    borderWidth: 1.5,
                    data: data.map(function ([
                        x, y
                    ]) {
                        return {
                            x,
                            y
                        };
                    }),
                    fill: false,
                    label: `${ii + 1}. ${label}`,
                    lineTension: 0,
                    pointRadius: 0.5,
                    showLine: true
                };
            })
        },
        options: {
            animation: {
                duration: 0
            },
            legend: {
                display: false
            },
            maintainAspectRatio: false,
            plugins: {
                zoom: {
                    zoom: {
                        drag: {
                            animationDuration: 500,
                            backgroundColor: "rgba(127,127,127,0.5)"
                        },
                        enabled: true,
                        mode: "xy"
                    }
                }
            },
            scales: JSON.parse(JSON.stringify({
                xAxes: [
                    {
                        scaleLabel: {
                            display: true,
                            labelString: options.xLabel
                        },
                        type: options.xAxesType || undefined
                    }
                ],
                yAxes: [
                    {
                        scaleLabel: {
                            display: true,
                            labelString: options.yLabel
                        },
                        type: options.yAxesType || undefined
                    }
                ]
            })),
            tooltips: {
                intersect: false,
                mode: "nearest"
            }
        },
        type: "scatter"
    };
    // init colorList - brewer.Paired12
    colorList = [
        "#a6cee3", "#1f78b4", "#b2df8a", "#33a02c",
        "#fb9a99", "#e31a1c", "#fdbf6f", "#ff7f00",
        "#cab2d6", "#6a3d9a", "#d2b48c", "#b15928"
    ].map(function (color, ii) {
        let whiteness = (
            ii % 2 === 0
            ? 0.875
            : 1
        );
        return (
            "rgb("
            + Math.round(whiteness * parseInt(color.slice(1, 3), 16)) + ","
            + Math.round(whiteness * parseInt(color.slice(3, 5), 16)) + ","
            + Math.round(whiteness * parseInt(color.slice(5, 7), 16))
            + ")"
        );
    });
    options.data.datasets.forEach(function (dataset, ii) {
        let color = colorList[ii % colorList.length];
        let colorWithAlpha = color.replace("rgb", "rgba").replace(")", ",0.5)");
        dataset.backgroundColor = colorWithAlpha;
        dataset.borderColor = color;
        dataset.pointBackgroundColor = colorWithAlpha;
        dataset.pointBorderColor = color;
    });
    chart = new Chart(elemChart.querySelector("canvas"), options);
    elemChart.chart = chart;
    // init zoom-reset
    chart.resetZoom();
    // init .uichartLegend
    elemChart.querySelector(
        ".uichartLegend"
    ).innerHTML = chart.data.datasets.map(function ({
        label,
        pointBorderColor
    }, ii) {
        return (`
<a
    class="uichartAction uichartLegendElem"
    data-action="uichartDatasetHideOrShow"
    data-hidden="0"
    data-ii="${ii}"
    title="${stringHtmlSafe(label)}"
>
<span
    class="uichartLegendElemColor"
    style="background: ${pointBorderColor};"
>&nbsp;</span>
<span>${label}</span>
</a>
        `);
    }).join("");
    elemChart.querySelector(".uichartNav").onclick = onDbAction;
}

async function uitableAjax(baton, {
    rowList,
    type
}) {
    let {
        colList,
        contentElem,
        db,
        dbtableName,
        elemInfo,
        elemLoading,
        elemScroller,
        elemTable,
        isDbchart,
        rowCount,
        sortCol,
        sortDir
    } = baton;
    let html = "";
    let viewRowBeg;
    let viewRowEnd;
    if (baton.rowCount === 0) {
        // uitableLoading - hide
        uiFadeOut(elemLoading);
        return;
    }
    switch (type) {
    // uitableScroll
    case "scroll":
    case "uitableInit":
        if (type === "uitableInit" && isDbchart) {
            await uiTryCatch(uichartCreate.bind(undefined, baton));
            return;
        }
        viewRowBeg = Math.max(0, Math.round(
            rowCount
            * elemScroller.scrollTop
            / (elemScroller.scrollHeight - 1 * UI_ROW_HEIGHT)
        ));
        viewRowEnd = Math.min(rowCount, Math.round(viewRowBeg + UI_VIEW_SIZE));
        // update table-view info
        elemInfo.textContent = (
            "showing "
            + new Intl.NumberFormat().format(viewRowBeg + 1)
            + " to "
            + new Intl.NumberFormat().format(viewRowEnd)
            + " of "
            + new Intl.NumberFormat().format(rowCount)
            + " entries"
        );
        // skip expensive table-redraw, if scroll-point is within boundaries
        if (
            contentElem.dataset.init !== "0"
            && baton.rowOffset <= Math.max(0, viewRowBeg - 1 * UI_VIEW_SIZE)
            && (
                Math.min(rowCount, viewRowEnd + 1 * UI_VIEW_SIZE)
                <= baton.rowOffset + UI_PAGE_SIZE
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
        uiFadeIn(elemLoading);
        baton.modeAjax = 1;
        // uitable - paginate
        rowList = await dbExecAsync({
            db,
            responseType: "list",
            sql: (`
SELECT
        rowid,
        *
    FROM ${dbtableName}
    ORDER BY [${colList[sortCol]}] ${sortDir}
    LIMIT ${Number(UI_PAGE_SIZE)}
    OFFSET ${Number(baton.rowOffset)};
            `)
        });
        rowList = (
            rowList[0]
            ? rowList[0].slice(1)
            : []
        );
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
        elemScroller.scrollHeight * baton.rowOffset / (baton.rowCount + 1)
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
    uiFadeOut(elemLoading);
}

function uitableCreate(baton) {
// this function will create a dom-datatable-view of sqlite queries and tables
    let contentElem;
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
<div class="uitableLoading">loading</div>
<div
    class="uichart"
    style="
    display: none;
    height: ${UI_CHART_HEIGHT}px;
    padding-left: ${UI_CHART_LEGEND_WIDTH}px;
    "
>
    <div
        class="uichartNav"
        style="
        height: ${UI_CHART_HEIGHT}px;
        margin-left: -${UI_CHART_LEGEND_WIDTH}px;
        position: absolute;
        width: ${UI_CHART_LEGEND_WIDTH}px;
        "
    >
        <button
            class="uichartAction"
            data-action="uichartZoomReset"
        >reset zoom</button>
        <button
            class="uichartAction"
            data-action="uichartDatasetHideAll"
        >hide all</button>
        <button
            class="uichartAction"
            data-action="uichartDatasetShowAll"
        >show all</button>
        <div
            class="uichartLegend"
            style="height: ${UI_CHART_HEIGHT - 64}px;"
        ></div>
    </div>
    <canvas class="uichartCanvas"></canvas>
</div>
<div class="uitable">
    <div class="uitableInfo">showing 0 to 0 of 0 entries</div>
    <div
        class="uitableScroller"
        style="height: ${(UI_VIEW_SIZE + 2) * UI_ROW_HEIGHT}px;"
        tabindex="-1"
    >
        <div
            class="uitableScrollerDummy"
            style="height: ${baton.rowCount * UI_ROW_HEIGHT}px;"
        ></div>
        <table class="uitableTable">
            <thead>
                <tr>
        `)
        + jsonHtmlSafe(baton.colList).map(function (col, ii) {
            return (
                ii === 0
                ? `<th title="${col}" data-sort="asc">${col}</th>`
                : `<th title="${col}">${col}</th>`
            );
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
</div>
</div>
        `)
    ).firstElementChild;
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
        elemChart: contentElem.querySelector(".uichart"),
        elemInfo: contentElem.querySelector(".uitableInfo"),
        elemLoading: contentElem.querySelector(".uitableLoading"),
        elemScroller: contentElem.querySelector(".uitableScroller"),
        elemTable: contentElem.querySelector(".uitableTable"),
        modeAjax: 0,
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
