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
    CodeMirror
} = globalThis;
let DBTABLE_DICT = Object.create(null);
let DB_DICT = new Map();
let DB_II = 1;
let DB_MAIN;
let SQLITE_EDITOR;
let SQLRESULT_LIST = [];
let UI_PAGE_SIZE = 256;
let UI_ROW_HEIGHT = 16;
let UI_VIEW_SIZE = 10;

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

function dttableAjax(baton, {
    rowList,
    type
}) {
    let {
        ajax,
        elemInfo,
        elemLoading,
        elemScroller,
        elemTable,
        rowCount,
        rowOffset
    } = baton;
    let html = "";
    let viewRowBeg;
    let viewRowEnd;
    if (baton.rowCount === 0) {
        // uiLoading - hide
        elemLoading.style.display = "none";
        return;
    }
    switch (type) {
    // dttableScroll
    case "dttableInit":
    case "scroll":
        elemScroller.focus();
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
            type !== "dttableInit"
            && rowOffset <= Math.max(0, viewRowBeg - 2 * UI_VIEW_SIZE)
            && (
                Math.min(rowCount, viewRowEnd + 2 * UI_VIEW_SIZE)
                <= rowOffset + UI_PAGE_SIZE
            )
        ) {
            return;
        }
        // Do the dttable redraw based on the calculated start point
        baton.rowOffset = Math.max(0, Math.round(
            viewRowBeg + 0.5 * UI_VIEW_SIZE - 0.5 * UI_PAGE_SIZE
        ));
        break;
    }
    switch (type !== "dttableDraw" && baton.modeAjax) {
    case 0:
        // uiLoading - show
        elemLoading.style.display = "block";
        baton.modeAjax = 1;
        ajax(baton, async function (json) {
            // recurse - draw
            dttableAjax(baton, json);
            // debounce - throttle
            await new Promise(function (resolve) {
                setTimeout(resolve, 500);
            });
            // debounce - next
            if (baton.modeAjax === 2) {
                baton.modeAjax = 0;
                dttableAjax(baton, {});
                return;
            }
            // cleanup
            baton.modeAjax = 0;
            // uiLoading - hide
            elemLoading.style.display = "none";
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
    // dttableDraw
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
}

function dttableInit(elemContainer, baton) {
// this function will create a dom-dttable-view of sqlite queries and tables
    let elemInfo;
    let elemLoading;
    let elemScroller;
    let elemTable;
    // All dttables are wrapped in a div
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
    elemContainer.innerHTML = (`
<div class="dttableLoading" style="display: block;">Loading</div>
<div class="dttablePageInfo">Showing 0 to 0 of 0 entries</div>
<div
    class="dttableScroller"
    style="height: ${(UI_VIEW_SIZE + 2) * UI_ROW_HEIGHT}px;"
>
    <div
        class="dttableScrollerDummy"
        style="height: ${baton.rowCount * UI_ROW_HEIGHT}px;"
    ></div>
    <table class="dttable">
        <thead>
            <tr>
    `) + jsonHtmlSafe(baton.colList).map(function (col) {
        return `<th>${col}</th>`;
    }).join("") + (`
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
    `);
    [
        elemLoading,
        elemInfo,
        elemScroller
    ] = elemContainer.children;
    elemTable = elemScroller.children[1];
    // init event-handling - sorting
    elemContainer.querySelector(
        "thead tr"
    ).addEventListener("click", function (evt) {
        dttableSort(baton, evt);
    });
    // init event-handling - scrolling
    elemContainer.querySelector(
        ".dttableScroller"
    ).addEventListener("scroll", function (evt) {
        dttableAjax(baton, evt);
    });
    Object.assign(baton, {
        elemInfo,
        elemLoading,
        elemScroller,
        elemTable,
        modeAjax: 0,
        rowOffset: 0
    });
    dttableAjax(baton, {
        type: "dttableInit"
    });
}

function dttableSort(baton, {
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
    baton.orderBy = [
        colIi, direction
    ];
    // Reset scroll to top in redraw.
    baton.elemScroller.scrollTop = 0;
    baton.rowOffset = 0;
    dttableAjax(baton, {});
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
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;"));
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
    // SQLRESULT_LIST - cleanup old dttables
    SQLRESULT_LIST.forEach(function ({
        colList,
        rowList
    }) {
        colList.length = 0;
        rowList.length = 0;
    });
    SQLRESULT_LIST = [];
    // 2. ui-render sqlresult to html
    data.forEach(function (rowList, ii) {
        let colList = rowList.shift();
        let hashtag = `sqlresult_${String(ii).padStart(2, "0")}`;
        function ajax({
            orderBy = [],
            rowOffset
        }, callbackDraw) {
            let [
                colIi, direction
            ] = orderBy;
            if (direction) {
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
            }
            callbackDraw({
                rowList: rowList.slice(rowOffset, rowOffset + UI_PAGE_SIZE),
                type: "dttableDraw"
            });
        }
        SQLRESULT_LIST.push({
            baton: {
                ajax,
                colList,
                rowCount: rowList.length
            },
            colList,
            rowList
        });
        html += (`
<div class="contentElemTitle" id="${hashtag}">
sqlite query result ${ii + 1}
</div>
<div class="dttableContainer" data-hashtag="${ii}"></div>
        `);
    });
    document.querySelector("#sqlresultList1").innerHTML = html;
    // init dttables
    document.querySelectorAll(
        "#sqlresultList1 .dttableContainer"
    ).forEach(function (elem) {
        dttableInit(
            elem,
            SQLRESULT_LIST[elem.dataset.hashtag].baton
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
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /"/gu
    ), "&quot;");
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
    // DBTABLE_DICT - cleanup old dttables
    document.querySelector("#dbtableList1").innerHTML = "";
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
                hashtag,
                rowcount
            } = dbtable;
            async function ajax({
                orderBy = [],
                rowOffset
            }, callbackDraw) {
                let [
                    colIi, direction
                ] = orderBy;
                // dttable - paginate
                let data = await dbExecAsync({
                    db: DB_MAIN,
                    responseType: "list",
                    sql: (`
SELECT
        rowid,
        *
    FROM ${dbtableFullname}
                    `) + (
                        direction
                        ? `ORDER BY [${columns[colIi]}] ${direction}`
                        : ""
                    ) + (`
    LIMIT ${Number(UI_PAGE_SIZE)}
    OFFSET ${Number(rowOffset)};
                    `)
                });
                callbackDraw({
                    rowList: (
                        data[0]
                        ? data[0].slice(1)
                        : []
                    ),
                    type: "dttableDraw"
                });
            }
            dbtable.baton = {
                ajax,
                colList: columns,
                rowCount: rowcount
            };
            html += (`
<div class="contentElemTitle" id="${hashtag}">
            `) + (
                dbName === "main"
                ? `table ${stringHtmlSafe(dbtableFullname)}`
                : `attached table ${stringHtmlSafe(dbtableFullname)}`
            ) + (`
</div>
<div class="dttableContainer" data-hashtag="${hashtag}"></div>
            `);
        });
    });
    document.querySelector("#dbtableList1").innerHTML = html;
    // init dttables
    document.querySelectorAll(
        "#dbtableList1 .dttableContainer"
    ).forEach(function (elem) {
        dttableInit(
            elem,
            DBTABLE_DICT[elem.dataset.hashtag].baton
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
