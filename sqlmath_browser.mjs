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
let DATATABLE_OPTION = {
    sScrollY: "224px"
};
let DATATABLE_ROW_BUFFER_SCALE = 16;
let DATATABLE_ROW_HEIGHT = 16;
let DATATABLE_SCROLLER_BOUNDARY_SCALE = 0.5;
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

function datatableAjax(baton, json) {
    let boundaryPx;
    let displayEnd = baton.rowCount;
    let displayLen = baton.pageLimit;
    let displayStart = baton.pageOffset;
    let html = "";
    let iTableHeight = baton.nTable.offsetHeight;
    let rowCount = json?.rowCount;
    let rowList = json?.rowList;
    let scrollTop;
    let tableTop;
    switch (rowCount === undefined && baton.modeAjax) {
    case 0:
        debugInline("datatableAjax - modeAjax=0");
        // uiLoading - show
        baton.nLoading.style.display = "block";
        baton.modeAjax = 1;
        baton.ajax(baton, async function (json) {
            // recurse - draw
            datatableAjax(baton, json);
            // debounce - throttle
            await new Promise(function (resolve) {
                setTimeout(resolve, 250);
            });
            if (baton.modeAjax === 2) {
                baton.modeAjax = 0;
                datatableAjax(baton);
                return;
            }
            // cleanup
            baton.modeAjax = 0;
            // uiLoading - hide
            baton.nLoading.style.display = "none";
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
    // init scroller height-properties
    if (!baton.hasOwnProperty(rowCount)) {
        baton.rowCount = rowCount;
        baton.scrollerHeight = DATATABLE_ROW_HEIGHT * Math.max(1, rowCount);
        // Minimum height so there is always a row visible
        // (the 'no rows found' if reduced to zero filtering)
        baton.nScrollerForce.style.height = (
            `${Math.max(DATATABLE_ROW_HEIGHT, baton.scrollerHeight)}px`
        );
    }
    if (!(rowList.length && rowList[0].length)) {
        return;
    }
    baton.rowList = rowList;
    // Insert the required TR nodes into the table for display
    // html-sanitize rowList
    JSON.parse(
        JSON.stringify(baton.rowList).replace((
            /&/gu
        ), "&amp;").replace((
            /</gu
        ), "&lt;").replace((
            />/gu
        ), "&gt;")
    ).forEach(function (row) {
        html += `<tr>`;
        row.forEach(function (val) {
            html += `<td>`;
            html += val ?? "";
            html += `</td>`;
        });
        html += `</tr>`;
    });
    baton.nTable.children[1].innerHTML = html;
    // Call all required callback functions for the end of a draw
    // Update the scroller when the Datatable is redrawn
    //
// Draw callback function which is fired when the Datatable is redrawn.
// The main function of this method is to position the drawn table correctly
// the scrolling container for the rows that is displays as a result of the
// scrolling position.
    // Disable the scroll event listener while we are updating the DOM
    baton.skip = true;
    // Reposition the scrolling for the updated virtual position if needed
    if (displayStart === 0) {
        // Linear calculation at the top of the table
        scrollTop = baton.topRowFloat * DATATABLE_ROW_HEIGHT;
    } else if (displayStart + displayLen >= displayEnd) {
        // Linear calculation that the bottom as well
        scrollTop = (
            baton.scrollerHeight
            - ((displayEnd - baton.topRowFloat) * DATATABLE_ROW_HEIGHT)
        );
    } else {
        // Domain scaled in the middle
        scrollTop = baton.topRowFloat * DATATABLE_ROW_HEIGHT;
    }
    baton.nScroller.scrollTop = scrollTop;
    // Store positional information so positional calculations can be based
    // upon the current table draw position
    baton.scrollTop0 = scrollTop;
    baton.baseRowTop = baton.topRowFloat;
    /**
// Pixel location of the top of the drawn table in the viewport
//  @type     int
//  @default  0
      */
    // Position the table in the virtual scroller
    tableTop = (
        scrollTop
        - ((baton.topRowFloat - displayStart) * DATATABLE_ROW_HEIGHT)
    );
    if (displayStart === 0) {
        tableTop = 0;
    } else if (displayStart + displayLen >= displayEnd) {
        tableTop = baton.scrollerHeight - iTableHeight;
    }
    baton.nTable.style.top = tableTop + "px";
    // Calculate the boundaries for where a redraw will be triggered by the
    // scroll event listener
    boundaryPx = (
        (scrollTop - tableTop) * DATATABLE_SCROLLER_BOUNDARY_SCALE
    );
    baton.redrawTop = scrollTop - boundaryPx;
    baton.redrawBottom = (
        (scrollTop + boundaryPx > (
            baton.scrollerHeight
            - baton.viewport
            - DATATABLE_ROW_HEIGHT
        ))
        ? baton.scrollerHeight - baton.viewport - DATATABLE_ROW_HEIGHT
        : scrollTop + boundaryPx
    );
    baton.skip = false;
    baton.ignoreScroll = false;
}

function datatableCreate(nTable, baton) {
// Create a Datatables Api instance, with the currently selected tables for
// the Api's context.
    let nTableWrapper;
    baton = Object.assign({
        aaSorting: [
            [
                0,
                "asc"
            ]
        ],
        modeAjax: 0,
        pageOffset: 0,
        // Pixel location of the boundary for when the next data set
        // should be loaded and drawn when scrolling down the way.
        // Note that this is actually calculated as the offset from
        redrawBottom: 0,
        // Pixel location of the boundary for when the next data set
        // should be loaded and drawn when scrolling up the way.
        redrawTop: 0,
        topRowFloat: 0,
        // Number of rows calculated as visible in the visible viewport
        viewportRows: 0
    }, baton);
    baton.nTable = nTable;
    // Sanity check
    assertOrThrow(nTable.nodeName === "TABLE");
    nTable.classList.add("dataTable");
    // init columns
    nTable.children[1].innerHTML = (`
<tr><td colspan="${nTable.children[0].firstElementChild.children.length}">
No data available in table
</td></tr>
    `);
    // Show the display HTML options
    // All Datatables are wrapped in a div
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
    nTableWrapper = domDivCreate(`
<div class="dataTables_wrapper no-footer">
<div class="dataTables_processing" style="display: block;">
    Loading...
    <div>
        <div></div><div></div><div></div><div></div>
    </div>
</div>
<div class="dataTables_info">
Showing 0 to 0 of 0 entries
</div>
<div class="dataTables_scroll">
    <div
        class="dataTables_scroller"
        style="height: ${baton.sScrollY}; maxHeight: ${baton.sScrollY};"
    >
        <div class="dataTables_scrollerForce"></div>
    </div>
</div>
</div>
    `).firstElementChild;
    [
        baton.nLoading, baton.nScrollInfo, baton.nScroller
    ] = nTableWrapper.children;
    baton.nScroller = baton.nScroller.firstElementChild;
    baton.nScrollerForce = baton.nScroller.firstElementChild;
    // Built our DOM structure - replace the holding div with what we want
    nTable.before(nTableWrapper);
    baton.nScroller.append(nTable);
    // init event-handling - sorting
    nTable.querySelector(
        "thead tr"
    ).addEventListener("click", function ({
        currentTarget,
        target
    }) {
        datatableSort(
            baton,
            Array.from(currentTarget.children).indexOf(target.closest("th"))
        );
    });

    // Initialization for Scroller
    // Insert a div element that we can use to force the DT scrolling container
    // to the height required if the whole table was being displayed
    //
    // Calculate and store information about how many rows are to be displayed
    // in the scrolling viewport, based on current dimensions in the browser's
    // rendering. This can be particularly useful if the table is initially
    // drawn in a hidden element - for example in a tab.
    baton.viewport = baton.nScroller.clientHeight;
    baton.viewportRows = (
        parseInt(baton.viewport / DATATABLE_ROW_HEIGHT, 10) + 1
    );
    baton.pageLimit = baton.viewportRows * DATATABLE_ROW_BUFFER_SCALE;
    // Scrolling callback to see if a page change is needed - use a throttled
    // function for the save save callback so we aren't hitting it on every
    // scroll
    baton.ignoreScroll = true;
    baton.nScroller.addEventListener("scroll", function () {
        datatableScroll(baton);
    });
    // If there is default sorting required - let's do it. The sort function
    // will do the drawing for us. Otherwise we draw the table regardless of the
    // Ajax source - this allows the table to look initialized for Ajax sourcing
    // data (show 'loading' message possibly)
    datatableAjax(baton);
}

function datatableScroll(baton) {
// Scrolling function - fired whenever the scrolling position is changed.
// This method needs to use the stored values to see if the table should be
// redrawn as we are moving towards the end of the information that is
// currently drawn or not. If needed, then it will redraw the table based on
// the new position.
    // Update the table's information display for what is now in the viewport
    // Update any information elements that are controlled by the Datatable
    // based on the scrolling viewport and what rows are visible in it.
    let iTopRow;
    let scrollTop = baton.nScroller.scrollTop;
    let scrollTop0 = baton.scrollTop0;
    baton.nScrollInfo.textContent = (
        "Showing "
        + new Intl.NumberFormat("en-US").format(Math.floor(
            (scrollTop - scrollTop0) / DATATABLE_ROW_HEIGHT
            + baton.baseRowTop + 1
        ))
        + " to "
        + new Intl.NumberFormat("en-US").format(Math.min(
            baton.rowCount,
            Math.ceil(
                (scrollTop + baton.viewport - scrollTop0) / DATATABLE_ROW_HEIGHT
                + baton.baseRowTop
            )
        ))
        + " of "
        + new Intl.NumberFormat("en-US").format(baton.rowCount)
        + " entries"
    );
    if (baton.skip) {
        return;
    }
    if (baton.ignoreScroll) {
        return;
    }
    // If scroll point is inside the trigger boundary,
    // we can skip expensive table redraw.
    if (baton.redrawTop <= scrollTop && scrollTop <= baton.redrawBottom) {
        baton.topRowFloat = scrollTop0 + scrollTop - scrollTop0;
        return;
    }
    // Perform expensive table redraw.
    baton.topRowFloat = scrollTop / DATATABLE_ROW_HEIGHT;
    iTopRow = (
        Math.floor(baton.topRowFloat)
        - Math.ceil(
            0.5 * (DATATABLE_ROW_BUFFER_SCALE - 1) * baton.viewportRows
        )
    );
    if (iTopRow <= 0) {
        // At the start of the table
        iTopRow = 0;
    } else if (iTopRow + baton.pageLimit > baton.rowCount) {
        // At the end of the table
        iTopRow = Math.max(0, baton.rowCount - baton.pageLimit);
    } else if (iTopRow % 2 !== 0) {
        // For the row-striping classes (odd/even) we want only to start
        // on evens otherwise the stripes will change between draws and
        // look rubbish
        iTopRow += 1;
    }
    if (iTopRow !== baton.pageOffset) {
        // Do the Datatables redraw based on the calculated start point
        // - note that when using server-side processing
        baton.pageOffset = iTopRow;
        datatableAjax(baton);
    }
}

function datatableSort(baton, colIdx) {
// Function to run on user sort request
    let asSorting = ["asc", "desc"];
    let nextSortIdx;
    let sorting = baton.aaSorting;
    function next(a, overflow) {
        let idx = a._idx;
        if (idx === undefined) {
            idx = asSorting.indexOf(a[1]);
        }
        return (
            idx + 1 < asSorting.length
            ? idx + 1
            : overflow
            ? null
            : 0
        );
    }
    if (sorting.length && sorting[0][0] === colIdx) {
        // Single column - already sorting on this column, modify the sort
        nextSortIdx = next(sorting[0]);
        sorting.length = 1;
        sorting[0][1] = asSorting[nextSortIdx];
        sorting[0]._idx = nextSortIdx;
    } else {
        // Single column - sort only on this column
        sorting.length = 0;
        sorting.push([colIdx, asSorting[0]]);
        sorting[0]._idx = 0;
    }
    // Reset scroll to top in redraw.
    baton.pageOffset = 0;
    baton.topRowFloat = 0;
    // Run the sort by calling a full redraw
    datatableAjax(baton);
}

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

function domDivCreate(innerHTML) {
// this function will return div-element with rendered <innerHTML>
    let elem = document.createElement("div");
    elem.innerHTML = innerHTML;
    return elem;
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
            aaSorting,
            pageLimit,
            pageOffset
        }, datatableAjax) {
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
                datatableAjax({
                    rowCount: rowList.length,
                    rowList: jsonHtmlSafe(
                        rowList.slice(
                            pageOffset,
                            pageOffset + pageLimit
                        )
                    )
                });
            });
        }
        SQLRESULT_LIST.push({
            colList,
            datatableOption: Object.assign({}, DATATABLE_OPTION, {
                ajax,
                aoColumns: colList.map(function (name) {
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
        datatableCreate(elem, SQLRESULT_LIST[elem.dataset.ii].datatableOption);
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
    // DBTABLE_DICT - cleanup old datatables
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
                hashtag
            } = dbtable;
            async function ajax({
                aaSorting,
                pageLimit,
                pageOffset
            }, datatableAjax) {
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
    LIMIT ${Number(pageLimit)}
    OFFSET ${Number(pageOffset)};
                    `)
                });
                debounce(hashtag, function () {
                    datatableAjax({
                        rowCount: data[0][1][0],
                        rowList: (
                            data[1]
                            ? jsonHtmlSafe(data[1].slice(1))
                            : []
                        )
                    });
                });
            }
            dbtable.datatableOption = Object.assign({}, DATATABLE_OPTION, {
                ajax,
                aoColumns: columns.map(function (name) {
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
        datatableCreate(
            elem,
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
