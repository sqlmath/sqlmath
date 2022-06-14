/*jslint beta, browser*/
import {
    //!! assertJsonEqual,
    //!! assertNumericalEqual,
    assertOrThrow,
    //!! dbCloseAsync,
    dbExecAsync,
    //!! dbExecWithRetryAsync,
    //!! dbGetLastBlobAsync,
    dbFileExportAsync,
    //!! dbFileImportAsync,
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
let SQLRESULT_DICT = Object.create(null);

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
    let dbData;
    // init main db
    await sqlmathWebworkerInit({});
    DB_MAIN = await dbOpenAsync({
        filename: ":memory:"
    });
    DB_MAIN.dbName = "main";
    DB_DICT.set(DB_MAIN.dbName, DB_MAIN);
    //
    dbData = await fetch("./.betadog_dtt0_public.sqlite");
    dbData = await dbData.arrayBuffer();
    await dbFileAttachAsync({
        db: DB_MAIN,
        dbData
    });
    await uiRenderNav();
    // init sqlEditor
    SQLITE_EDITOR = CodeMirror.fromTextArea(document.querySelector(
        "#sqliteEditor1"
    ), {
        extraKeys: {
            "Ctrl-Enter": onDbExec,
            //!! "Ctrl-S": onDbSave.bind(undefined, {}),
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
        ]
    ].forEach(function ([
        selector, evt, cb
    ]) {
        document.querySelector(selector).addEventListener(evt, cb);
    });
}

async function onDbExec({
    sql
}) {
// this function will
// 1. exec <sql>-command in webworker
// 2. ui-render result to html
    let data;
    // 1. exec <sql>-command in webworker
    //!! uiLoaderEnd({
        //!! modeForce: true
    //!! });
    //!! uiLoaderStart({
        //!! msg: "script editor - execute",
        //!! modeForce: true
    //!! });
    sql = sql || SQLITE_EDITOR.getValue();
    try {
        data = await dbExecAsync({
            db: DB_MAIN,
            responseType: "list",
            sql
        });
    } catch (err) {
        //!! onDbRefresh({});
        //!! return;
        assertOrThrow(undefined, err);
    }
    debugInline(data);
    // 2. ui-render result to html
    SQLRESULT_DICT = Object.create(null);
    data = await Promise.all(data.map(uiRenderSqlresultIi));
    document.querySelector("#sqlresultList1").innerHTML = data.join("");
    //!! onDbRefresh({});
    //
    document.querySelectorAll(
        ".sqlresultTable"
    ).forEach(function (elem) {
        jQuery(elem).dataTable({
            dom: "rti",
            scrollX: true,
            scrollY: 200
            //!! serverSide: true
        });
    });
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

async function uiRenderNav() {
// this function will render #navPanel1
    let data;
    let html = "";
    // sync DB_DICT
    data = await dbExecAsync({
        db: DB_MAIN,
        sql: "PRAGMA database_list;"
    });
    data = new Set(data[0].map(function ({
        name
    }) {
        return name;
    }));
    DB_DICT.forEach(function (ignore, dbName) {
        if (!data.has(dbName)) {
            DB_DICT.delete(dbName);
        }
    });
    // uiRenderNavDbtable()
    DBTABLE_DICT = Object.create(null);
    data = await Promise.all(
        Array.from(DB_DICT.values()).map(uiRenderNavDbtable)
    );
    html += data.join("\n");
    document.querySelector("#navPanel1").innerHTML = html;
}

async function uiRenderNavDbtable(db) {
    // this function will render #navPanel1
    let dbName = db.dbName;
    let dbtable;
    let dbtableList;
    let html = "";
    let tmp;
    html += (
        dbName === "main"
        ? `<div class="navTitle">database ${dbName}</div>\n`
        : `<div class="navTitle">database ${dbName} (attached)</div>\n`
    );
    dbtableList = await dbExecAsync({
        db,
        sql: (`
SELECT * FROM sqlite_schema WHERE type = 'table' ORDER BY tbl_name;
        `)
    });
    dbtableList = dbtableList[0];
    if (!dbtableList) {
        return html;
    }
    dbtableList = dbtableList.map(function ({
        sql,
        tbl_name
    }) {
        let dbtableFullname = `${dbName}.${tbl_name}`;
        dbtable = {
            columns: [],
            dbName,
            dbtableFullname,
            dbtableName: tbl_name,
            hashtag: encodeURIComponent(dbtableFullname),
            rowcount: 0,
            sql
        };
        DBTABLE_DICT[dbtableFullname] = dbtable;
        return dbtable;
    });
    tmp = dbtableList.map(function ({
        dbtableFullname,
        dbtableName
    }) {
        return (`
SELECT '${dbtableFullname}' AS dbtableFullname;
PRAGMA table_info("${dbtableName}");
SELECT COUNT(*) AS rowcount FROM "${dbtableName}";
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
                    dbtable.columns = rowList.map(function ({
                        name
                    }) {
                        return name;
                    });
                    break;
                case "dbtableFullname":
                    dbtable = DBTABLE_DICT[row0.dbtableFullname];
                    break;
                case "rowcount":
                    dbtable.rowcount = row0.rowcount;
                    break;
                }
            }
        });
    });
    html += "<ol>\n";
    html += dbtableList.map(function ({
        columns,
        dbtableFullname,
        rowcount,
        sql
    }) {
        return (
            `<li class="navDbtable" title="`
            + stringHtmlSafe((
                `[right-click for crud operations]\n`
            ) + JSON.stringify({
                columns,
                dbtableFullname,
                rowcount,
                sql
            }, undefined, 4))
            + `">`
            + stringHtmlSafe(dbtableFullname)
            + `</li>\n`
        );
    }).join("");
    html += "</ol>\n";
    return html;
}

function uiRenderSqlresultIi(rowList, ii) {
/*
 * this function will ui-render <tableList>
 */
    let colList = rowList.shift();
    let html = "";
    let sqlresult;
    ii += 1;
    sqlresult = {
        colList,
        hashtag: `sqlresult_${String(ii).padStart(2, "0")}`,
        rowList
    };
    SQLRESULT_DICT[sqlresult.hashtag] = sqlresult;
    debugInline({
        colList,
        html,
        rowList
    });
    html += `<div class="contentTitle">sqlite result ${ii}</div>\n`;
    html += `<table class="sqlresultTable">\n`;
    html += `<thead>\n`;
    html += `<tr>\n`;
    html += colList.map(function (col) {
        return `<th>${stringHtmlSafe(col)}</th>\n`;
    }).join("");
    html += `</tr>\n`;
    html += `</thead>\n`;
    html += `<tbody>\n`;
    rowList.forEach(function (row) {
        html += `<tr>\n`;
        row.forEach(function (val) {
            html += `<td>${stringHtmlSafe(String(val))}</td>\n`;
        });
        html += `</tr>\n`;
    });
    html += `</tbody>\n`;
    html += `</table>\n`;
    //
    //!! // cleanup SQLRESULT_DICT
    //!! Object.keys(SQLRESULT_DICT).forEach(function (key) {
        //!! if (key.indexOf("sqlresult-" + sqlresultIi + "-") === 0) {
            //!! delete SQLRESULT_DICT[key];
        //!! }
    //!! });
    //!! htmlData = "";
    //!! htmlMeta = "";
    //!! // ui-render - static-html
    //!! tableList.forEach(function (table, ii) {
        //!! let {
            //!! colList,
            //!! description,
            //!! hashtag,
            //!! rowcount = (
                //!! table.rowList && table.rowList.length
            //!! ),
            //!! t_name,
            //!! t_sql = "",
            //!! t_type,
            //!! tbl_name,
            //!! title
        //!! } = table;
        //!! t_type = t_type || "result";
        //!! description = (
            //!! t_type === "index"
            //!! ? "I" + (ii + 1) + " - " + tbl_name + "." + t_name
            //!! : t_type === "table"
            //!! ? "T" + (ii + 1) + " - " + t_name
            //!! : "R" + (ii + 1) + " - result"
        //!! );
        //!! hashtag = "sqlresult-" + sqlresultIi + "-" + ii;
        //!! SQLRESULT_DICT[hashtag] = table;
        //!! Object.assign(table, {
            //!! description,
            //!! hashtag,
            //!! t_type
        //!! });
        //!! title = {
            //!! columns: colList ?? undefined,
            //!! description: description ?? undefined,
            //!! rowcount: rowcount ?? undefined,
            //!! sql: t_sql.replace((
                //!! /\s+/g
            //!! ), " ").slice(0, 100) || undefined
        //!! };
        //!! title = JSON.stringify(title, undefined, 1);
        //!! htmlData += (`
//!! <div class="sqlresultData" data-t_type="${t_type}" id="${hashtag}">
//!! <div class="title">${stringHtmlSafe(description)}</div>
//!! <table></table>
//!! <div class="chartContainer">
//!! <button class="chartResetZoom">reset zoom</button>
//!! <canvas></canvas>
//!! </div>
//!! </div>
        //!! `);
        //!! htmlMeta += (`
//!! <tr
//!! data-hashtag="${hashtag}"
//!! data-t_type="${t_type}" title="${stringHtmlSafe(title)}"
//!! >
//!! <td>${t_type[0].toUpperCase()}</td>
//!! <td>${stringHtmlSafe(description).replace((/\u0020/g), "&nbsp;")}</td>
//!! </tr>
        //!! `);
    //!! });
    //!! document.querySelector(
        //!! "#sqlresultData" + sqlresultIi
    //!! ).innerHTML = htmlData;
    //!! document.querySelector(
        //!! "#sqlresultMeta" + sqlresultIi + " tbody"
    //!! ).innerHTML = htmlMeta;
    //!! // ui-render - datatable and chart
    //!! return Promise.all(Array.from(document.querySelectorAll(
        //!! "#sqlresultData" + sqlresultIi + " .sqlresultData"
    //!! )).map(function (elem, elemIi) {
        //!! uiRenderSqlresultTableIi(Object.assign({
            //!! elem,
            //!! elemIi,
            //!! sqlresultIi
        //!! }, tableList[elemIi]));
    //!! }));
    return html;
}

// init
await init();
