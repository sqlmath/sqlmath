/*jslint beta, browser, nomen*/
import {
    //!! assertJsonEqual,
    //!! assertNumericalEqual,
    assertOrThrow,
    //!! dbCloseAsync,
    dbExecAsync,
    //!! dbExecWithRetryAsync,
    //!! dbGetLastBlobAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbOpenAsync,
    debugInline,
    noop,
    //!! objectDeepCopyWithKeysSorted
    sqlmathWebworkerInit
} from "./sqlmath.mjs";

let {
    CodeMirror,
    getComputedStyle,
    jQuery
} = globalThis;
let DATATABLE_OPTION = {
    deferRender: true,
    dom: "rti",
    processing: true,
    scrollX: true,
    scrollY: "224px",
    scroller: true,
    searching: false,
    serverSide: true
};
let DBTABLE_DICT = Object.create(null);
let DB_DICT = new Map();
let DB_II = 1;
let DB_MAIN;
let DEBOUNCE_DICT = Object.create(null);
let SQLITE_EDITOR;
let SQLRESULT_LIST = [];

noop(
    SQLITE_EDITOR,
    assertOrThrow,
    dbFileExportAsync,
    debugInline
);

async function dbFileAttachAsync({
    db,
    dbData
}) {
    let dbAttached;
    let dbName;
    DB_II += 1;
    dbName = `db${String(DB_II).padStart(2, "0")}`;
    dbAttached = await dbOpenAsync({
        dbData,
        filename: `file:${dbName}?mode=memory&cache=shared`
    });
    dbAttached.dbName = dbName;
    await dbExecAsync({
        db,
        sql: `ATTACH DATABASE '${dbAttached.filename}' AS ${dbName}`
    });
    DB_DICT.set(dbName, dbAttached);
}

function debounce(key, func) {
    let val = DEBOUNCE_DICT[key] || {
        func,
        timerTimeout: setTimeout(function () {
            val.func();
            delete DEBOUNCE_DICT[key];
        }, 500)
    };
    val.func = func;
    DEBOUNCE_DICT[key] = val;
}

async function init() {
    // hack-jquery - optimization
    jQuery.cssHooks.height.get = function (elem, computed, extra) {
        return jQueryGetWidthOrHeight("height", elem, computed, extra);
    };
    jQuery.cssHooks.width.get = function (elem, computed, extra) {
        return jQueryGetWidthOrHeight("width", elem, computed, extra);
    };
    // init main db
    await sqlmathWebworkerInit({});
    DB_MAIN = await dbOpenAsync({
        filename: ":memory:"
    });
    DB_MAIN.dbName = "main";
    DB_DICT.set(DB_MAIN.dbName, DB_MAIN);
    //
    await uiRenderDbtableList();
    // init sqlEditor
    SQLITE_EDITOR = CodeMirror.fromTextArea(document.querySelector(
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
        [
            "#dbExec2", "click", onDbExec
        ],
        [
            "#fileOpenDb1", "click", onFileOpen
        ],
        [
            "#fileOpenDb2", "change", onFileOpen
        ],
        [
            "#fileOpenDbAttach1", "click", onFileOpen
        ],
        [
            "#fileOpenDbAttach2", "change", onFileOpen
        ],
        [
            "#fileOpenScript1", "click", onFileOpen
        ],
        [
            "#fileOpenScript2", "change", onFileOpen
        ]
    ].forEach(function ([
        selector, evt, cb
    ]) {
        document.querySelector(selector).addEventListener(evt, cb);
    });
    document.addEventListener("keyup", function (evt) {
        switch ((evt.ctrlKey || evt.metaKey) && evt.key) {
        case "Enter":
            onDbExec();
            break;
        default:
            return;
        }
        // disable default
        evt.preventDefault();
        evt.stopPropagation();
    });
}

function jQueryGetWidthOrHeight(name, elem, computed, extra) {
    // Use the active box-sizing model to add/subtract irrelevant styles
    let cssExpand = [
        "Top", "Right", "Bottom", "Left"
    ];
    let ii;
    let val = (
        name === "width"
        ? elem.offsetWidth
        : elem.offsetHeight
    );
    assertOrThrow(computed, computed);
    // Start with offset property, which is equivalent to the border-box value
    extra = extra || "content";
    // If we already have the right measurement, avoid augmentation
    if (extra === "border") {
        return val + "px";
    }
    // Otherwise initialize for horizontal or vertical properties
    computed = getComputedStyle(elem);
    ii = (
        name === "width"
        ? 1
        : 0
    );
    while (ii < 4) {
        // Both box models exclude margin, so add it if we want it
        if (extra === "margin") {
            val += parseFloat(computed[extra + cssExpand[ii]]);
        }
        // border-box includes padding, so remove it if we want content
        if (extra === "content") {
            val -= parseFloat(computed["padding" + cssExpand[ii]]);
        }
        // At this point, extra isn't border nor margin, so remove border
        if (extra !== "margin") {
            val -= parseFloat(computed["border" + cssExpand[ii] + "Width"]);
        }
        ii += 2;
    }
    return val + "px";
}

function jsonHtmlSafe(obj) {
// this function will make <obj> html-safe
// https://stackoverflow.com/questions/7381974
    return JSON.parse(JSON.stringify(obj).replace((
        /&/gu
    ), "&amp;").replace((
        /'/gu
    ), "&apos;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /&amp;(amp;|apos;|gt;|lt;|quot;)/igu
    ), "&$1"));
}

async function onDbExec() {
// this function will
// 1. exec sql-command in webworker
// 2. ui-render sqlresult to html
    let data;
    let html = "";
    // 1. exec sql-command in webworker
    data = await dbExecAsync({
        db: DB_MAIN,
        responseType: "list",
        sql: SQLITE_EDITOR.getValue()
    });
    // SQLRESULT_LIST - cleanup old datatables
    document.querySelector("#sqlresultList1").innerHTML = "";
    SQLRESULT_LIST.forEach(function ({
        colList,
        datatable,
        rowList
    }) {
        colList.length = 0;
        rowList.length = 0;
        datatable.api(true).destroy(true);
    });
    SQLRESULT_LIST = [];
    // 2. ui-render sqlresult to html
    data.forEach(function (rowList, ii) {
        let colList = rowList.shift();
        let hashtag = `sqlresult_${String(ii).padStart(2, "0")}`;
        function ajax({
            _iDisplayLength,
            _iDisplayStart,
            aaSorting
        }, callback) {
            let [
                colIi, direction
            ] = aaSorting[0];
            rowList.sort(function (aa, bb) {
                aa = aa[colIi];
                bb = bb[colIi];
                return (
                    aa < bb
                    ? -1
                    : aa > bb
                    ? 1
                    : 0
                );
            });
            if (direction === "desc") {
                rowList.reverse();
            }
            debounce(hashtag, function () {
                callback({
                    data: jsonHtmlSafe(
                        rowList.slice(
                            _iDisplayStart,
                            _iDisplayStart + _iDisplayLength
                        )
                    ),
                    iRecordsCount: rowList.length
                });
            });
        }
        SQLRESULT_LIST.push({
            colList,
            datatableOption: Object.assign({}, DATATABLE_OPTION, {
                ajax,
                columns: colList.map(function (name) {
                    return {
                        defaultContent: "",
                        name,
                        title: name
                    };
                }),
                data: rowList
            }),
            rowList
        });
        html += (
            `<div class="contentElemTitle" id="${hashtag}">`
            + `sqlite query result ${ii + 1}`
            + `</div>\n`
        );
        html += `<div class="contentTableContainer">\n`;
        html += `<table class="sqlresultTable" data-ii=${ii}>\n`;
        html += `<thead>\n`;
        html += `<tr>\n`;
        html += colList.map(function (col) {
            return `<th>${stringHtmlSafe(col)}</th>\n`;
        }).join("");
        html += `</tr>\n`;
        html += `</thead>\n`;
        html += `<tbody>\n`;
        html += `</tbody>\n`;
        html += `</table>\n`;
        html += `</div>\n`;
    });
    document.querySelector("#sqlresultList1").innerHTML = html;
    // init datatables
    document.querySelectorAll(
        "#sqlresultList1 .sqlresultTable"
    ).forEach(function (elem) {
        SQLRESULT_LIST[elem.dataset.ii].datatable = jQuery(elem).dataTable(
            SQLRESULT_LIST[elem.dataset.ii].datatableOption
        );
    });
    await uiRenderDbtableList();
}

async function onFileOpen({
    currentTarget
}) {
// this function will open db from file
    switch (currentTarget.id) {
    case "fileOpenDb1":
        document.querySelector("#fileOpenDb2").value = "";
        document.querySelector("#fileOpenDb2").click();
        return;
    case "fileOpenDbAttach1":
        document.querySelector("#fileOpenDbAttach2").value = "";
        document.querySelector("#fileOpenDbAttach2").click();
        return;
    case "fileOpenScript1":
        document.querySelector("#fileOpenScript2").value = "";
        document.querySelector("#fileOpenScript2").click();
        return;
    }
    switch (currentTarget.id) {
    case "fileOpenDb2":
        if (currentTarget.files.length === 0) {
            return;
        }
        await dbFileImportAsync({
            db: DB_MAIN,
            dbData: (
                await currentTarget.files[0].arrayBuffer()
            )
        });
        await uiRenderDbtableList();
        return;
    case "fileOpenDbAttach2":
        if (currentTarget.files.length === 0) {
            return;
        }
        await dbFileAttachAsync({
            db: DB_MAIN,
            dbData: (
                await currentTarget.files[0].arrayBuffer()
            )
        });
        await uiRenderDbtableList();
        return;
    case "fileOpenScript2":
        currentTarget = await currentTarget.files[0].text();
        SQLITE_EDITOR.setValue(currentTarget);
        await uiRenderDbtableList();
        return;
    }
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
        /"/gu
    ), "&quot;").replace((
        /'/gu
    ), "&apos;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /&amp;(amp;|apos;|gt;|lt;|quot;)/ig
    ), "&$1");
}

async function uiRenderDbtableList() {
// this function will render #dbtableList1
    let dbList;
    let html = "";
    // DB_DICT - sync
    dbList = await dbExecAsync({
        db: DB_MAIN,
        sql: "PRAGMA database_list;"
    });
    dbList = new Set(dbList[0].map(function ({
        name
    }) {
        return name;
    }));
    DB_DICT.forEach(function (ignore, dbName) {
        if (!dbList.has(dbName)) {
            DB_DICT.delete(dbName);
        }
    });
    // DBTABLE_DICT - cleanup old datatables
    document.querySelector("#dbtableList1").innerHTML = "";
    Object.values(DBTABLE_DICT).forEach(function ({
        datatable
    }) {
        datatable.api(true).destroy(true);
    });
    DBTABLE_DICT = Object.create(null);
    // DBTABLE_DICT - sync
    await Promise.all(Array.from(
        DB_DICT.values()
    ).map(async function (db) {
        let dbName = db.dbName;
        let dbtable;
        let dbtableList;
        let tmp;
        db.dbtableList = [];
        dbtableList = await dbExecAsync({
            db,
            sql: (`
SELECT * FROM sqlite_schema WHERE type = 'table' ORDER BY tbl_name;
            `)
        });
        dbtableList = dbtableList[0];
        if (!dbtableList) {
            return;
        }
        dbtableList = dbtableList.map(function ({
            sql,
            tbl_name
        }) {
            let dbtableFullname = `[${dbName}].[${tbl_name}]`;
            dbtable = {
                columns: [],
                dbName,
                dbtableFullname,
                dbtableName: `[${tbl_name}]`,
                hashtag: encodeURIComponent(dbtableFullname),
                rowcount: 0,
                sql
            };
            DBTABLE_DICT[dbtable.hashtag] = dbtable;
            return dbtable;
        });
        tmp = dbtableList.map(function ({
            dbtableFullname,
            dbtableName
        }) {
            return (`
SELECT '${dbtableFullname}' AS dbtableFullname;
PRAGMA table_info(${dbtableName});
SELECT COUNT(*) AS rowcount FROM ${dbtableName};
            `);
        }).join("");
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
                "cid", "dbtableFullname", "rowcount"
            ].forEach(function (key) {
                if (row0.hasOwnProperty(key)) {
                    switch (key) {
                    case "cid":
                        dbtable.columns = [
                            "rowid",
                            rowList.map(function ({
                                name
                            }) {
                                return name;
                            })
                        ].flat();
                        break;
                    case "dbtableFullname":
                        dbtable = DBTABLE_DICT[
                            encodeURIComponent(row0.dbtableFullname)
                        ];
                        break;
                    case "rowcount":
                        dbtable.rowcount = row0.rowcount;
                        break;
                    }
                }
            });
        });
        db.dbtableList = dbtableList;
    }));
    // ui-render databases and tables to html
    DB_DICT.forEach(function ({
        dbName,
        dbtableList
    }) {
        dbtableList.forEach(function (dbtable) {
            let {
                columns,
                dbtableFullname,
                hashtag
            } = dbtable;
            async function ajax({
                _iDisplayLength,
                _iDisplayStart,
                aaSorting
            }, callback) {
                let [
                    colIi, direction
                ] = aaSorting[0];
                // datatable - paginate
                let data = await dbExecAsync({
                    db: DB_MAIN,
                    responseType: "list",
                    sql: (`
SELECT COUNT(*) AS cnt FROM ${dbtableFullname};
SELECT
        rowid,
        *
    FROM ${dbtableFullname}
    ORDER BY [${columns[colIi]}]
                    ${(
                        direction === "asc"
                        ? "ASC"
                        : "DESC"
                    )}
    LIMIT ${Number(_iDisplayLength)}
    OFFSET ${Number(_iDisplayStart)};
                    `)
                });
                debounce(hashtag, function () {
                    callback({
                        data: (
                            data[1]
                            ? jsonHtmlSafe(data[1].slice(1))
                            : []
                        ),
                        iRecordsCount: data[0][1][0]
                    });
                });
            }
            dbtable.datatableOption = Object.assign({}, DATATABLE_OPTION, {
                ajax,
                columns: columns.map(function (name) {
                    return {
                        defaultContent: "",
                        name,
                        title: name
                    };
                })
            });
            html += `<div class="contentElemTitle" id="${hashtag}">`;
            html += (
                dbName === "main"
                ? `table ${stringHtmlSafe(dbtableFullname)}`
                : `attached table ${stringHtmlSafe(dbtableFullname)}`
            );
            html += `</div>\n`;
            html += `<div class="contentTableContainer">\n`;
            html += `<table class="dbtableTable" data-hashtag=${hashtag}>\n`;
            html += `<thead>\n`;
            html += `<tr>\n`;
            html += columns.map(function (col) {
                return `<th>${stringHtmlSafe(col)}</th>\n`;
            }).join("");
            html += `</tr>\n`;
            html += `</thead>\n`;
            html += `<tbody>\n`;
            html += `</tbody>\n`;
            html += `</table>\n`;
            html += `</div>\n`;
        });
    });
    document.querySelector("#dbtableList1").innerHTML = html;
    // init datatables
    document.querySelectorAll(
        "#dbtableList1 .dbtableTable"
    ).forEach(function (elem) {
        DBTABLE_DICT[elem.dataset.hashtag].datatable = jQuery(elem).dataTable(
            DBTABLE_DICT[elem.dataset.hashtag].datatableOption
        );
    });
    // ui-render #tocPanel1
    html = "";
    DB_DICT.forEach(function ({
        dbName,
        dbtableList
    }) {
        html += `<div class="tocTitle">\n`;
        html += (
            dbName === "main"
            ? `database [${dbName}]\n`
            : `database [${dbName}] (attached)\n`
        );
        html += `</div>\n`;
        html += dbtableList.map(function ({
            columns,
            dbtableFullname,
            hashtag,
            rowcount
        }) {
            return (
                `<a class="tocElemA" href="#${hashtag}" title="`
                + stringHtmlSafe((
                    `[right-click for crud operations]\n`
                ) + JSON.stringify({
                    columns,
                    dbtableFullname,
                    rowcount
                }, undefined, 4))
                + `">`
                + stringHtmlSafe(dbtableFullname)
                + `</a>\n`
            );
        }).join("");
    });
    document.querySelector("#tocDbList1").innerHTML = html;
}

// init
await init();
