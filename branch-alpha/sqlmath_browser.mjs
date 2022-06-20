/*jslint beta, browser*/
import {
    //!! assertJsonEqual,
    //!! assertNumericalEqual,
    //!! assertOrThrow,
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
    jQuery
} = globalThis;
let DBTABLE_DICT = Object.create(null);
let DB_DICT = new Map();
let DB_II = 1;
let DB_MAIN;
let SQLITE_EDITOR;
let SQLRESULT_LIST = [];

noop(
    SQLITE_EDITOR,
    debugInline,
    dbFileAttachAsync,
    dbFileExportAsync
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

async function init() {
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
        datatable.fnDestroy();
    });
    SQLRESULT_LIST = [];
    // 2. ui-render sqlresult to html
    data.forEach(function (rowList, ii) {
        let colList = rowList.shift();
        let hashtag;
        function ajax({
            draw,
            length,
            order,
            start
        }, callback) {
            let colIi = order[0].column;
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
            if (order[0].dir === "desc") {
                rowList.reverse();
            }
            callback({
                data: jsonHtmlSafe(
                    rowList.slice(start, start + length)
                ),
                draw,
                recordsFiltered: rowList.length,
                recordsTotal: rowList.length
            });
        }
        SQLRESULT_LIST.push({
            colList,
            datatableOption: {
                ajax,
                columns: colList.map(function (name) {
                    return {
                        defaultContent: "",
                        name,
                        title: name
                    };
                }),
                data: rowList,
                deferRender: true,
                dom: "rti",
                scrollX: true,
                scrollY: "10rem",
                scroller: true,
                searching: false,
                serverSide: true
            },
            rowList
        });
        hashtag = `sqlresult_${String(ii).padStart(2, "0")}`;
        html += (
            `<div class="contentElemTitle" id="${hashtag}">`
            + `sqlite query result ${ii + 1}`
            + `</div>\n`
        );
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
        datatable.fnDestroy();
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
                draw,
                length,
                order,
                start
            }, callback) {
                let colIi = order[0].column;
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
    ORDER BY ${columns[colIi]}
                    ${(
                        order[0].dir === "asc"
                        ? "ASC"
                        : "DESC"
                    )}
    LIMIT ${Number(length)}
    OFFSET ${Number(start)};
                    `)
                });
                callback({
                    data: (
                        data[1]
                        ? jsonHtmlSafe(data[1].slice(1))
                        : []
                    ),
                    draw,
                    recordsFiltered: data[0][1][0],
                    recordsTotal: data[0][1][0]
                });
            }
            dbtable.datatableOption = {
                ajax,
                columns: columns.map(function (name) {
                    return {
                        defaultContent: "",
                        name,
                        title: name
                    };
                }),
                deferRender: true,
                dom: "rti",
                scrollX: true,
                scrollY: "10rem",
                scroller: true,
                searching: false,
                serverSide: true
            };
            html += `<div class="contentElemTitle" id="${hashtag}">`;
            html += (
                dbName === "main"
                ? `table ${stringHtmlSafe(dbtableFullname)}`
                : `attached table ${stringHtmlSafe(dbtableFullname)}`
            );
            html += `</div>\n`;
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
