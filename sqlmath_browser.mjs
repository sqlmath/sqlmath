/*jslint beta, browser, devel, nomen*/
import {
    //!! assertJsonEqual,
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
let DBTABLE_DICT = new Map();
let DB_DICT = new Map();
let DB_II = 1;
let DB_MAIN;
let DB_QUERY;
let DEBOUNCE_DICT = Object.create(null);
let UI_EDITOR;
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
            delete DEBOUNCE_DICT[key];
            val.func();
        }, 250)
    };
    val.func = func;
    DEBOUNCE_DICT[key] = val;
}

function domDivCreate(innerHTML) {
// this function will return div-element with rendered <innerHTML>
    let elem = document.createElement("div");
    elem.innerHTML = innerHTML;
    return elem;
}

async function init() {
    // init db - query
    DB_QUERY = {
        dbName: "query",
        dbtableList: []
    };
    DB_DICT.set("query", DB_QUERY);
    // init db - main
    await sqlmathWebworkerInit({});
    DB_MAIN = await dbOpenAsync({
        filename: ":memory:"
    });
    DB_MAIN.dbName = "main";
    DB_DICT.set("main", DB_MAIN);
    //
    await uiRenderDbtableList();
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
        [
            ".modalClose", "click", onModalClose
        ],
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
        selector, evt, listener
    ]) {
        document.querySelectorAll(selector).forEach(function (elem) {
            elem.addEventListener(evt, function (evt) {
                evt.preventDefault();
                evt.stopPropagation();
                listener(evt);
            });
        });
    });
    document.addEventListener("keyup", function (evt) {
        switch ((evt.ctrlKey || evt.metaKey) && evt.key) {
        case "Enter":
            onDbExec({});
            break;
        default:
            return;
        }
        // disable default
        evt.preventDefault();
        evt.stopPropagation();
    });
    window.addEventListener("hashchange", uitableInitWithinView);
    window.addEventListener("scroll", uitableInitWithinView);
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
        dbName,
        rowList0
    }, key) {
        if (dbName === "query") {
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
        UI_EDITOR.setValue(currentTarget);
        await uiRenderDbtableList();
        return;
    }
}

function onModalClose({
    currentTarget
}) {
// this function will close current modal
    currentTarget.closest(".modalPanel").style.display = "none";
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
    let dbtableIi = 0;
    let html = "";
    let locationHash;
    // save location.hash
    locationHash = location.hash;
    location.hash = "";
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
        if (dbName !== "query" && !dbList.has(dbName)) {
            DB_DICT.delete(dbName);
        }
    });
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        dbName
    }, key) {
        if (dbName !== "query") {
            DBTABLE_DICT.delete(key);
        }
    });
    // DBTABLE_DICT - sync
    await Promise.all(Array.from(
        DB_DICT.values()
    ).map(async function (db) {
        let baton;
        let dbName = db.dbName;
        let dbtableList;
        let tmp;
        db.dbtableList = [];
        dbtableList = (
            db === DB_QUERY
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
            dbtableFullname = dbtableFullname || `[${dbName}].[${tbl_name}]`;
            dbtableIi += 1;
            baton = {
                colList,
                db,
                dbName,
                dbtableFullname,
                dbtableName: `[${tbl_name}]`,
                hashtag: `dbtable_${String(dbtableIi).padStart(2, "0")}`,
                rowCount,
                rowList0,
                sql,
                title: (
                    dbName === "query"
                    ? `${dbtableFullname}`
                    : dbName === "main"
                    ? `table ${dbtableFullname}`
                    : `attached table ${dbtableFullname}`
                )
            };
            DBTABLE_DICT.set(baton.hashtag, baton);
            return baton;
        });
        if (db !== DB_QUERY) {
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
                db === DB_QUERY
                ? "#sqlqueryList1"
                : "#dbtableList1"
            ).appendChild(uitableCreate(baton));
        });
    });
    // ui-render #tocPanel1
    html = "";
    DB_DICT.forEach(function ({
        dbName,
        dbtableList
    }) {
        html += (
            `<div class="tocTitle">`
            + (
                dbName === "query"
                ? `query results`
                : dbName === "main"
                ? `database [${dbName}]`
                : `database [${dbName}] (attached)`
            )
            + `</div>`
        );
        dbtableList.forEach(function ({
            colList,
            dbtableFullname,
            hashtag,
            rowCount
        }) {
            html += (
                `<a class="tocElemA" href="#${hashtag}" title="`
                + stringHtmlSafe((
                    `[right-click for crud operations]\n`
                ) + JSON.stringify({
                    colList,
                    dbtableFullname,
                    rowCount
                }, undefined, 4))
                + `">`
                + stringHtmlSafe(dbtableFullname)
                + `</a>\n`
            );
        });
    });
    document.querySelector("#tocDbList1").innerHTML = html;
    // restore location.hash
    location.hash = locationHash;
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
        if (db === DB_QUERY) {
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
<div class="contentElemTitle">${stringHtmlSafe(baton.title)}</div>
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
            return `<th>${col}</th>`;
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
