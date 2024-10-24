/*jslint-disable*/
window.addEventListener("load", async function () {
    "use strict";
    let {
        $,
        Blob,
        Chart,
        CodeMirror,
        blobDownload,
        contextmenuDataset,
        jsScript0,
        onDbCrud,
        onDbRefresh,
        sqlCallbackDict,
        sqlCallbackId,
        sqlDb0,
        sqlEditor,
        sqlPostMessage,
        sqlResultDict,
        sqlScript0,
        sqlWorker,
        uiLoaderInfo,
        uiLoaderInfo0,
        uiRenderSqlResultTableIi
    } = window;
    // init debugInline
    if (!globalThis.debugInline) {
        let consoleError;
        consoleError = console.error;
        globalThis.debugInline = function (...argList) {
        /*
         * this function will both print <argList> to stderr and
         * return <argList>[0]
         */
            consoleError("\n\ndebugInline");
            consoleError(...argList);
            consoleError("\n");
            return argList[0];
        };
    }

    function assertOrThrow(passed, msg) {
    /*
     * this function will throw <msg> if <passed> is falsy
     */
        if (passed) {
            return;
        }
        throw (
            (
                msg &&
                typeof msg.message === "string" &&
                typeof msg.stack === "string"
            )
            // if msg is err, then leave as is
            ? msg
            : new Error(
                typeof msg === "string"
                // if msg is string, then leave as is
                ? msg
                // else JSON.stringify(msg)
                : JSON.stringify(msg, undefined, 4)
            )
        );
    }

    function domStyleValidate() {
    /*
     * this function will validate <style> tags
     */
        let list;
        let rgx;
        rgx = (
            /^0\u0020(?:(body\u0020>\u0020)?(?:\.test-report-div\u0020.+|\.x-istanbul\u0020.+|\.button|\.styleColorError|\.readonly|\.textarea|\.uiAnimateSlide|a|body|code|div|input|pre|textarea)(?:,|\u0020\{))|^[1-9]\d*?\u0020#/m
        );
        list = [];
        document.querySelectorAll("style").forEach(function (elem, ii) {
            elem.innerHTML.replace((
                /\/\*[\S\s]*?\*\/|;|\}/g
            ), "\n").replace((
                /^([^\n\u0020@].*?)[,{:].*?$/gm
            ), function (match0, match1) {
                try {
                    ii = document.querySelectorAll(match1).length;
                } catch (errCaught) {
                    console.error(errCaught);
                }
                if (!(ii > 1) && !rgx.test(elem)) {
                    list.push(ii + " " + match0);
                }
            });
        });
        list.filter(function (elem) {
            return !rgx.test(elem);
        }).sort().reverse().forEach(function (elem, ii, list) {
            console.error(
                "domStyleValidateUnmatched " + (list.length - ii) + ". " + elem
            );
        });
    }

    function jsonHtmlSafe(obj) {
    /*
     * this function will make <obj> html-safe
     * https://stackoverflow.com/questions/7381974/which-characters-need-to-be-escaped-on-html
     */
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

    function jsonRowListFromCsv({
        csv
    }) {
    /*
     * this function will convert <csv>-text to json list-of-list
     */
/*
https://tools.ietf.org/html/rfc4180#section-2
Definition of the CSV Format
    While there are various specifications and implementations for the
    CSV format (for ex. [4], [5], [6] and [7]), there is no formal
    specification in existence, which allows for a wide variety of
    interpretations of CSV files.  This section documents the format that
    seems to be followed by most implementations:
1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
2.  The last record in the file may or may not have an ending line
    break.  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx
3.  There maybe an optional header line appearing as the first line
    of the file with the same format as normal record lines.  This
    header will contain names corresponding to the fields in the file
    and should contain the same number of fields as the records in
    the rest of the file (the presence or absence of the header line
    should be indicated via the optional "header" parameter of this
    MIME type).  For example:
    field_name,field_name,field_name CRLF
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:
    aaa,bbb,ccc
5.  Each field may or may not be enclosed in double quotes (however
    some programs, such as Microsoft Excel, do not use double quotes
    at all).  If fields are not enclosed with double quotes, then
    double quotes may not appear inside the fields.  For example:
    "aaa","bbb","ccc" CRLF
    zzz,yyy,xxx
6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:
    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx
7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:
    "aaa","b""bb","ccc"
*/
        let csvRowList;
        let match;
        let quote;
        let rgx;
        let row;
        let val;
        // normalize "\r\n" to "\n"
        csv = csv.replace((
            /\r\n?/gu
        ), "\n");
        rgx = (
            /(.*?)(""|"|,|\n)/gu
        );
        csvRowList = [];
        // reset row
        row = [];
        val = "";
        while (true) {
            match = rgx.exec(csv);
            if (!match) {
/*
2.  The last record in the file may or may not have an ending line
    break.  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx
*/
                if (!row.length) {
                    break;
                }
                // if eof missing crlf, then mock it
                rgx.lastIndex = csv.length;
                match = [
                    "\n", "", "\n"
                ];
            }
            // build val
            val += match[1];
            if (match[2] === "\"") {
/*
5.  Each field may or may not be enclosed in double quotes (however
    some programs, such as Microsoft Excel, do not use double quotes
    at all).  If fields are not enclosed with double quotes, then
    double quotes may not appear inside the fields.  For example:
    "aaa","bbb","ccc" CRLF
    zzz,yyy,xxx
*/
                quote = !quote;
            } else if (quote) {
/*
7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:
    "aaa","b""bb","ccc"
*/
                if (match[2] === "\"\"") {
                    val += "\"";
/*
6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:
    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx
*/
                } else {
                    val += match[2];
                }
            } else if (match[2] === ",") {
/*
4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:
    aaa,bbb,ccc
*/
                // delimit val
                row.push(val);
                val = "";
            } else if (match[2] === "\n") {
/*
1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
*/
                // delimit val
                row.push(val);
                val = "";
                // append row
                csvRowList.push(row);
                // reset row
                row = [];
            }
        }
        // append row
        if (row.length) {
            csvRowList.push(row);
        }
        return csvRowList;
    }

    function jsonRowListNormalize({
        colList,
        colListPriority = [],
        rowList
    }) {
    /*
     * this function will normalize <rowList> with given <colList>
     */
        // convert list-of-dict to list-of-list
        if (!Array.isArray(rowList[0])) {
            colList = new Map(Array.from(
                colList || []
            ).map(function (key, ii) {
                return [
                    key, ii
                ];
            }));
            rowList = rowList.map(function (row) {
                Object.keys(row).forEach(function (key) {
                    if (!colList.has(key)) {
                        colList.set(key, colList.size);
                    }
                });
                return Array.from(colList.keys()).map(function (key) {
                    return row[key];
                });
            });
            colList = Array.from(colList.keys());
        }
        if (!colList) {
            colList = rowList[0];
            rowList = rowList.slice(1);
        }
        // normalize rowList
        rowList = rowList.map(function (row) {
            return (
                row.length === colList.length
                ? row
                : colList.map(function (ignore, ii) {
                    return row[ii];
                })
            );
        });
        // sort colList by colListPriority
        if (!colListPriority) {
            return {
                colList,
                rowList
            };
        }
        colListPriority = new Map([].concat(
            colListPriority,
            colList
        ).map(function (key) {
            return [
                key, colList.indexOf(key)
            ];
        }).filter(function ([
            ignore, ii
        ]) {
            return ii >= 0;
        }));
        colList = Array.from(colListPriority.keys());
        colListPriority = Array.from(colListPriority.values());
        rowList = rowList.map(function (row) {
            return colListPriority.map(function (ii) {
                return row[ii];
            });
        });
        rowList.unshift(colList);
        return rowList;
    }

    function jsonRowListToCsv({
        colList,
        colListPriority = [],
        rowList
    }) {
    /*
     * this function will convert <rowList> to csv-text
     */
/*
https://tools.ietf.org/html/rfc4180#section-2
Definition of the CSV Format
    While there are various specifications and implementations for the
    CSV format (for ex. [4], [5], [6] and [7]), there is no formal
    specification in existence, which allows for a wide variety of
    interpretations of CSV files.  This section documents the format that
    seems to be followed by most implementations:
1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
2.  The last record in the file may or may not have an ending line
    break.  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx
3.  There maybe an optional header line appearing as the first line
    of the file with the same format as normal record lines.  This
    header will contain names corresponding to the fields in the file
    and should contain the same number of fields as the records in
    the rest of the file (the presence or absence of the header line
    should be indicated via the optional "header" parameter of this
    MIME type).  For example:
    field_name,field_name,field_name CRLF
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:
    aaa,bbb,ccc
5.  Each field may or may not be enclosed in double quotes (however
    some programs, such as Microsoft Excel, do not use double quotes
    at all).  If fields are not enclosed with double quotes, then
    double quotes may not appear inside the fields.  For example:
    "aaa","bbb","ccc" CRLF
    zzz,yyy,xxx
6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:
    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx
7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:
    "aaa","b""bb","ccc"
*/
        // handle null-case
        if (!rowList.length) {
            return "";
        }
        // normalize colList, rowList
        rowList = jsonRowListNormalize({
            colList,
            colListPriority,
            rowList
        });
        return rowList.map(function (row) {
            return row.map(function (val) {
                if (val === undefined || val === null) {
                    return "";
                }
                if (typeof val === "string") {
/*
7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:
    "aaa","b""bb","ccc"
*/
                    val = val.replace((
                        /"/gu
                    ), "\"\"");
/*
6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:
    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx
*/
                    if ((
                        /[\r\n",]/
                    ).test(val)) {
                        val = "\"" + val + "\"";
                    }
                    return val;
                }
                return String(val);
/*
4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:
    aaa,bbb,ccc
*/
/*
1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF
*/
/*
2.  The last record in the file may or may not have an ending line
    break.  For example:
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx
*/
            }).join(",") + "\r\n";
        }).join("");
    }

    function jsonRowListUnnormalize({
        colList,
        rowList
    }) {
    /*
     * this function will convert <rowList> from list-of-list to list-of-dict
     */
        let dict;
        let ii;
        let jj;
        let row;
        let rowList2;
        rowList2 = [];
        ii = 0;
        while (ii < rowList.length) {
            dict = {};
            row = rowList[ii];
            jj = 0;
            while (jj < row.length) {
                dict[colList[jj]] = row[jj];
                jj += 1;
            }
            rowList2.push(dict);
            ii += 1;
        }
        return rowList2;
    }

    function noop() {
    /*
     * this function will do nothing
     */
        return;
    }

    function sqlOnMessage({
        data
    }) {
    /*
     * this function will hande <data>-response from <sqlWorker>
     */
        sqlCallbackDict[data.id](data);
    }

    function stringHtmlSafe(str) {
    /*
     * this function will make <str> html-safe
     * https://stackoverflow.com/questions/7381974/which-characters-need-to-be-escaped-on-html
     */
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
            /&amp;(amp;|apos;|gt;|lt;|quot;)/igu
        ), "&$1");
    }

    function stringRegexpEscape(str) {
    /*
     * this function will regexp-escape <str>
     * https://stackoverflow.com/questions/3561493/is-there-a-regexp-escape-function-in-javascript
     */
        return str.replace((
            /[\-\/\\\^$*+?.()|\[\]{}]/g
        ), "\\$&");
    }

    function stringSqlnameSafe(str) {
    /*
     * this function will make <str> sqlname-safe
     */
        return str.toLowerCase().trim().replace((
            /[^0-9_a-z]/g
        ), "_").replace((
            /^[0-9]/g
        ), "a$&");
    }

    function uiHashtagScrollto({
        target
    }) {
    /*
     * this function will scrollto <target>'s hashtag
     */
        let hashtag;
        try {
            hashtag = target.closest("[data-hashtag]").dataset.hashtag;
        } catch (ignore) {
            return;
        }
        location.hash = "undefined";
        location.hash = hashtag || "";
    }

    function uiLoaderEnd({
        key,
        modeForce,
        type
    }) {
    /*
     * this function will end ui-loading-animation
     */
        if (type) {
            if (type === "keydown" && key === "Escape") {
                uiLoaderEnd({
                    modeForce: true
                });
            }
            return;
        }
        if (modeForce) {
            uiLoaderInfo = uiLoaderInfo0;
        }
        uiLoaderInfo(false);
    }

    function uiLoaderStart({
        msg,
        modeForce
    }) {
    /*
     * this function will start ui-loading-animation
     */
        if (modeForce || !document.querySelector("#datatables_buttons_info")) {
            uiLoaderInfo(
                msg || "loading",
                "<div class=\"loader\"></div>"
            );
        }
    }

    function uiRenderError(err, mode) {
    /*
     * this function will ui-render <err>
     */
        // if err is function, wrap in try-catch-block
        if (typeof err === "function") {
            return function (...argList) {
                return err(...argList).catch(uiRenderError);
            };
        }
        // err - confirm
        if (mode === "modeConfirm") {
            uiLoaderEnd({
                modeForce: true
            });
            return;
        }
        // err - debug
        console.error(err);
        // err - ui-render
        uiLoaderInfo(
            "<div style=\"color: #d00;\">sql error</div>",
            (`
<div style="color: #d00;">${stringHtmlSafe(err.message)}</div><br>
<button class="button" id="buttonOk1">ok</button>
            `)
        );
        document.querySelector("#buttonOk1").addEventListener(
            "click",
            uiRenderError.bind(undefined, undefined, "modeConfirm")
        );
        uiLoaderInfo = noop;
        return new Promise(function (ignore, reject) {
            setTimeout(function () {
                reject(err);
            });
        });
    }

    function uiRenderSqlResultIi({
        htmlData,
        htmlMeta,
        sqlResultIi,
        tableList
    }) {
    /*
     * this function will ui-render <tableList>
     */
        // cleanup sqlResultDict
        Object.keys(sqlResultDict).forEach(function (key) {
            if (key.indexOf("sqlResult-" + sqlResultIi + "-") === 0) {
                delete sqlResultDict[key];
            }
        });
        htmlData = "";
        htmlMeta = "";
        // ui-render - static-html
        tableList.forEach(function (table, ii) {
            let {
                colList,
                description,
                hashtag,
                rowcount = (
                    table.rowList && table.rowList.length
                ),
                t_name,
                t_sql = "",
                t_type,
                tbl_name,
                title
            } = table;
            t_type = t_type || "result";
            description = (
                t_type === "index"
                ? "I" + (ii + 1) + " - " + tbl_name + "." + t_name
                : t_type === "table"
                ? "T" + (ii + 1) + " - " + t_name
                : "R" + (ii + 1) + " - result"
            );
            hashtag = "sqlResult-" + sqlResultIi + "-" + ii;
            sqlResultDict[hashtag] = table;
            Object.assign(table, {
                description,
                hashtag,
                t_type
            });
            title = {
                description: description ?? undefined,
                rowcount: rowcount ?? undefined,
                columns: colList ?? undefined,
                sql: t_sql.replace((
                    /\s+/g
                ), " ").slice(0, 100) || undefined
            };
            title = JSON.stringify(title, undefined, 1);
            htmlData += (`
<div class="sqlResultData" data-t_type="${t_type}" id="${hashtag}">
    <div class="title">${stringHtmlSafe(description)}</div>
    <table></table>
    <div class="chartContainer">
    <button class="chartResetZoom">reset zoom</button>
    <canvas></canvas>
    </div>
</div>
            `);
            htmlMeta += (`
<tr
    data-hashtag="${hashtag}"
    data-t_type="${t_type}" title="${stringHtmlSafe(title)}"
>
    <td>${t_type[0].toUpperCase()}</td>
    <td>${stringHtmlSafe(description).replace((/\u0020/g), "&nbsp;")}</td>
</tr>
            `);
        });
        document.querySelector(
            "#sqlResultData" + sqlResultIi
        ).innerHTML = htmlData;
        document.querySelector(
            "#sqlResultMeta" + sqlResultIi + " tbody"
        ).innerHTML = htmlMeta;
        // ui-render - datatable and chart
        return Promise.all(Array.from(document.querySelectorAll(
            "#sqlResultData" + sqlResultIi + " .sqlResultData"
        )).map(function (elem, elemIi) {
            uiRenderSqlResultTableIi(Object.assign({
                elem,
                elemIi,
                sqlResultIi
            }, tableList[elemIi]));
        }));
    }

    uiRenderSqlResultTableIi = uiRenderError(async function ({
        ajax,
        chart,
        colList,
        description,
        elem,
        elemIi,
        elemTr,
        promise2,
        resolve2,
        rowList,
        sqlResultIi,
        t_type,
        tbl_name
    }) {
    /*
     * this function will ui-render datatable and chart
     */
        ajax = uiRenderError(async function ({
            colIi,
            data,
            draw,
            length,
            order,
            start
        }, callback) {
            colIi = order[0].column;
            // datatable - full
            if (sqlResultIi === 1) {
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
                return;
            }
            // datatable - paginate
            data = await sqlPostMessage({
                sql: (
                    "SELECT COUNT(*) FROM \"" + tbl_name + "\";\n" +
                    "SELECT rowid, * FROM \"" + tbl_name + "\"\n" +
                    "ORDER BY \"" + colList[colIi] + "\"\n" +
                    (
                        order[0].dir === "asc"
                        ? "ASC"
                        : "DESC"
                    ) + "\n" +
                    "LIMIT " + Number(length) + "\n" +
                    "OFFSET " + Number(start) + ";\n"
                )
            });
            callback({
                data: (
                    data.tableList[1]
                    ? jsonHtmlSafe(data.tableList[1].rowList)
                    : []
                ),
                draw,
                recordsFiltered: data.tableList[0].rowList[0][0],
                recordsTotal: data.tableList[0].rowList[0][0]
            });
            if (draw === 1) {
                resolve2();
                return;
            }
            uiLoaderEnd({});
        });
        if (t_type === "index") {
            return;
        }
        promise2 = new Promise(function (resolve) {
            resolve2 = resolve;
        });
        if (sqlResultIi === 2) {
            colList.unshift("rowid");
        }
        // ui-render datatable
        $(elem.querySelector("table")).datatable({
            ajax,
            columns: colList.map(function (name) {
                return {
                    defaultContent: "",
                    name,
                    title: name
                };
            }),
            createdRow: sqlResultIi === 2 && function ({
                dataset
            }, [
                rowid
            ]) {
                Object.assign(dataset, {
                    hashtag: elem.id,
                    rowid,
                    t_type: "row"
                });
            },
            data: rowList,
            // dom: "lfrtip",
            dom: "rti",
            pageLength: 100,
            scrollX: true,
            scrollY: 200,
            scroller: true,
            searching: false,
            serverSide: true
        });
        if (!(
            t_type === "result" &&
            rowList.length >= 3 &&
            rowList[0][0] === "#!sqlchart" &&
            JSON.stringify(rowList[1]) === "[null,null]"
        )) {
            return promise2;
        }
        // ui-render chart
        let {
            chartTitle,
            chartType,
            datasets = [],
            label0,
            xData,
            xLabel,
            xType,
            yLabel
        } = JSON.parse(rowList[0][1]);
        elem.classList.add("sqlResultChart");
        await Promise.all(rowList.slice(2).map(async function ([
            key, sql
        ]) {
            let data;
            let ii;
            ii = 0;
            switch (key) {
            case "x":
            case "xy":
            case "y":
                data = await sqlPostMessage({
                    sql
                });
                data = Object.assign({
                    colList: [],
                    rowList: []
                }, data.tableList[0]);
                switch (key) {
                case "x":
                    xData = data.rowList.flat();
                    xLabel = data.colList[0];
                    return;
                case "xy":
                    if (chartType === "line" && !xType) {
                        xType = "linear";
                    }
                    xLabel = data.colList[1];
                    yLabel = data.colList[2];
                    break;
                case "y":
                    yLabel = data.colList[1];
                    break;
                }
                data.rowList.forEach(function ([
                    label, x, y
                ]) {
                    if (label0 !== label) {
                        data = [];
                        ii += 1;
                        datasets.push({
                            borderWidth: 1.5,
                            data,
                            fill: false,
                            label: ii + ". " + label,
                            lineTension: 0,
                            pointRadius: 0.5
                        });
                        label0 = label;
                    }
                    data.push(
                        key === "xy"
                        ? {
                            x,
                            y
                        }
                        : x
                    );
                });
                break;
            }
        }));
        chart = new Chart(elem.querySelector("canvas"), {
            data: {
                datasets,
                labels: xData
            },
            options: {
                animation: {
                    duration: 0
                },
                maintainAspectRatio: false,
                legend: {
                    align: "start",
                    labels: {
                        boxWidth: 15,
                        fontSize: 10,
                        padding: 5
                    }
                },
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
                scales: {
                    xAxes: [
                        {
                            distribution: "series",
                            scaleLabel: {
                                display: true,
                                labelString: xLabel
                            },
                            type: xType
                        }
                    ],
                    yAxes: [
                        {
                            gridLines: {
                                drawBorder: false
                            },
                            scaleLabel: {
                                display: true,
                                labelString: yLabel
                            }
                        }
                    ]
                },
                tooltips: {
                    intersect: false,
                    mode: (
                        chartType === "line"
                        ? "nearest"
                        : "index"
                    )
                }
            },
            plugins: [
                {
                    // column-align legend
                    beforeInit: function ({
                        legend
                    }) {
                        legend.afterFit = function () {
                            let {
                                lineWidths,
                                maxWidth
                            } = legend;
                            legend.height += 5;
                            if (legend.lineWidths.length < 2) {
                                legend.legendItemWidth = 0;
                                return;
                            }
                            lineWidths.forEach(function (ii) {
                                lineWidths[ii] = maxWidth;
                            });
                        };
                        legend.beforeFit = function () {
                            let {
                                ctx,
                                legendItems,
                                maxWidth,
                                padding = 5,
                                textWidth
                            } = legend;
                            if (legendItems.length < 3) {
                                return;
                            }
                            // column-align legend by max-textWidth
                            textWidth = 1;
                            Object.values(legendItems).forEach(function ({
                                text
                            }) {
                                textWidth = Math.max(
                                    textWidth,
                                    ctx.measureText(text).width
                                );
                            });
                            // column-align legend by max-lines
                            textWidth = Math.min(
                                textWidth,
                                Math.floor(
                                    (maxWidth - padding) / Math.ceil(
                                        Object.keys(legendItems).length / 8
                                    ) - padding
                                )
                            );
                            Object.values(legendItems).forEach(function (elem) {
                                while (
                                    elem.text.length > 1 &&
                                    ctx.measureText(elem.text).width >=
                                    textWidth + 2 * padding
                                ) {
                                    elem.text = elem.text.slice(0, -1);
                                }
                            });
                            legend.legendItemWidth = textWidth;
                        };
                    }
                }
            ],
            type: chartType
        });
        // init resetZoom
        elem.querySelector(".chartResetZoom").addEventListener(
            "click",
            chart.resetZoom.bind(chart)
        );
        // bug-workaround - resetZoom
        chart.resetZoom();
        // update description
        description = "R" + (elemIi + 1) + " - chart - " + stringHtmlSafe(
            chartTitle || (yLabel + " vs. " + xLabel)
        );
        elem.querySelector(".title").textContent = description;
        elemTr = document.querySelector(
            "#sqlResultMeta1 tr[data-hashtag=" + elem.id + "]"
        );
        elemTr.children[1].textContent = description;
        elemTr.title = elemTr.title.replace((
            /:\u0020".*"/
        ), ": " + JSON.stringify(description));
        return promise2;
    });

/* validateLineSortedReset */
    sqlPostMessage = uiRenderError(async function sqlPostMessage({
        action = "exec",
        data,
        params,
        sql
    }) {
    /*
     * this function will post msg to <sqlWorker> and return result
     */
        let err;
        let id;
        let timeElapsed;
        uiLoaderStart({});
        timeElapsed = Date.now();
        sqlCallbackId += 1;
        id = sqlCallbackId;
        sqlWorker.postMessage({
            action,
            buffer: data,
            id,
            params,
            sql
        });
        err = new Error();
        err.msg = {
            action,
            data,
            params,
            sql
        };
        let {
            buffer,
            error,
            results = []
        } = await new Promise(function (resolve) {
            sqlCallbackDict[sqlCallbackId] = resolve;
        });
        delete sqlCallbackDict[id];
        timeElapsed = Date.now() - timeElapsed;
        console.error(
            "sqlPostMessage - " + JSON.stringify({
                action,
                timeElapsed
            })
        );
        if (error) {
            throw (Object.assign(err, {
                message: error,
                timeElapsed
            }));
        }
        results = results.map(function ({
            columns,
            ii,
            jj,
            values
        }) {
            ii = 0;
            while (ii < values.length) {
                jj = 0;
                while (jj < values[ii].length) {
                    if (values[ii][jj]?.constructor === Uint8Array) {
                        values[ii][jj] = "<blob>";
                    }
                    jj += 1;
                }
                ii += 1;
            }
            return {
                colList: columns,
                rowList: values,
                sql
            };
        });
        return {
            buffer,
            sql,
            tableList: results
        };
    });

    function onContextmenu(evt) {
    /*
     * this function will hande contextmenu-evt
     */
        let elemCm;
        let {
            clientX,
            clientY,
            ctrlKey,
            metaKey,
            shiftKey,
            target,
            type
        } = evt;
        elemCm = document.querySelector("#contextmenu1");
        // contextmenu - onclick
        if (type !== "contextmenu") {
            // contextmenu - hide
            elemCm.style.display = "none";
            // contextmenu - run crud
            target = target.closest("#contextmenu1 tr[data-t_type]");
            if (contextmenuDataset && target) {
                onDbCrud(Object.assign(
                    {},
                    sqlResultDict[contextmenuDataset.hashtag],
                    contextmenuDataset,
                    target.dataset
                ));
            }
            return;
        }
        // contextmenu - oncontextmenu
        // contextmenu - enable default
        if (ctrlKey || metaKey || shiftKey) {
            return;
        }
        // contextmenu - disable default
        evt.preventDefault();
        evt.stopPropagation();
        target = target.closest(
            "#panelContent1 tr[data-t_type=row]," +
            "#panelSidebar1 tr[data-t_type]"
        );
        // contextmenu - hide
        if (!target) {
            elemCm.style.display = "none";
            return;
        }
        // init contextmenuDataset
        contextmenuDataset = target.dataset;
        elemCm.querySelectorAll("tr[data-t_type]").forEach(function ({
            children,
            dataset,
            style
        }) {
            style.display = "none";
            if (
                dataset.t_type && dataset.t_type === contextmenuDataset.t_type
            ) {
                style.display = "table-row";
                if (dataset.crud) {
                    children[0].textContent = dataset.t_type;
                    children[1].textContent = " - " + dataset.crud;
                }
            }
        });
        elemCm.style.left = Math.min(
            clientX,
            window.innerWidth - elemCm.offsetWidth - 10
        ) + "px";
        elemCm.style.top = Math.min(
            clientY,
            window.innerHeight - elemCm.offsetHeight - 20
        ) + "px";
        // contextmenu - show
        elemCm.style.display = "block";
    }

    async function onDbExec({
        data,
        sql
    }) {
    /*
     * this function will
     * 1. exec <sql>-command in webworker
     * 2. ui-render result to html
     */
        // 1. exec <sql>-command in webworker
        uiLoaderEnd({
            modeForce: true
        });
        uiLoaderStart({
            msg: "script editor - execute",
            modeForce: true
        });
        sql = sql || sqlEditor.getValue();
        try {
            data = await sqlPostMessage({
                action: "exec",
                sql
            });
        } catch (ignore) {
            onDbRefresh({});
            return;
        }
        // 2. ui-render result to html
        await uiRenderSqlResultIi({
            sqlResultIi: 1,
            tableList: data.tableList
        });
        onDbRefresh({});
    }

    onDbRefresh = uiRenderError(async function onDbRefresh({
        data,
        promiseWait,
        table0,
        tableList,
        tmp
    }) {
    /*
     * this function will
     * 1. query tables and indices from db
     * 2. ui-render tables and indices to html
     */
        // 1. query tables and indices from db
        promiseWait = new Promise(function (resolve) {
            setTimeout(resolve, 500);
        });
        // query-info - sqlite_schema
        data = await sqlPostMessage({
            sql: (`
SELECT type AS t_type, name AS t_name, tbl_name, sql AS t_sql
FROM sqlite_schema
WHERE sql IS NOT NULL
ORDER BY tbl_name, type DESC, name;
            `)
        });
        tableList = Array.from(
            data.tableList.length === 0
            ? []
            : data.tableList[0].rowList
        ).map(function ([
            t_type, t_name, tbl_name, t_sql
        ]) {
            let table;
            table = {
                t_name,
                t_sql,
                t_type,
                tbl_name,
                i_list: []
            };
            if (t_type === "index") {
                table0.i_list.push(table);
            }
            if (t_type === "table") {
                table0 = table;
            }
            return table;
        });
        // query-info - colList, rowcount
        tmp = await sqlPostMessage({
            sql: tableList.map(function ({
                t_name,
                t_type
            }) {
                return (
                    t_type === "index"
                    ? "SELECT NULL;\nSELECT NULL;\n"
                    : (
                        "PRAGMA table_info(\"" + t_name + "\");\n" +
                        "SELECT COUNT(*) AS rowcount FROM \"" + t_name + "\";\n"
                    )
                );
            }).join("")
        });
        tableList.forEach(function (result, ii) {
            if (result.t_type === "table") {
                Object.assign(result, {
                    colList: tmp.tableList[2 * ii].rowList.map(function ([
                        ignore, name
                    ]) {
                        return name;
                    }),
                    rowcount: tmp.tableList[2 * ii + 1].rowList[0][0],
                    // [cid, name, type, notnull, dflt_value, pk]
                    table_info: jsonRowListUnnormalize({
                        colList: tmp.tableList[2 * ii].colList,
                        rowList: tmp.tableList[2 * ii].rowList
                    })
                });
            }
        });
        // 2. ui-render tables and indices to html
        await Promise.all([
            promiseWait, uiRenderSqlResultIi({
                sqlResultIi: 2,
                tableList
            })
        ]);
        uiLoaderEnd({});
    });

    async function onDbSave({
        data,
        elem,
        file = "sqlite-database.sqlite"
    }) {
    /*
     * this function will save db to file
     */
        // cleanup
        URL.revokeObjectURL(blobDownload);
        uiLoaderStart({});
        if (data === undefined) {
            data = await sqlPostMessage({
                action: "export"
            });
            data = data.buffer;
        }
        blobDownload = URL.createObjectURL(new Blob([
            data
        ]));
        elem = document.createElement("a");
        elem.href = blobDownload;
        // cleanup
        setTimeout(function () {
            URL.revokeObjectURL(elem.href);
        }, 30000);
        elem.download = file.replace(
            ".",
            "-" + new Date().toISOString().slice(0, 19).replace((
                /[\-:T]/g
            ), "") + "."
        );
        uiLoaderEnd({});
        elem.click();
    }

    onDbCrud = uiRenderError(async function onDbCrud({
        colList = [],
        crud = "",
        data,
        i_list = [],
        modeConfirm,
        rowid,
        rowList,
        sql,
        t_name = "",
        t_sql = "",
        t_type = "",
        tmp
    }) {
    /*
     * this function will run <crud>-action on db
     */
        function tableRename({
            i_name,
            i_name2,
            t_name,
            t_name2,
            t_sql
        }) {
        /*
         * this function will replace <t_name> with <t_name2> in <t_sql>
         */
            [
                [
                    i_name, i_name2
                ],
                [
                    t_name, t_name2
                ]
            ].forEach(function ([
                aa, bb
            ]) {
                // replace aa with bb
                if (aa) {
                    t_sql = t_sql.replace(new RegExp(
                        "\\s+?[\"\\[']?" +
                        stringRegexpEscape(aa) +
                        "\\b[\"\\]']?"
                    ), "\n    \"" + bb + "\"\n");
                }
            });
            // normalize t_sql
            return t_sql.replace((
                /[\r\n]+/g
            ), "\n").trim() + ";\n";
        }
        uiLoaderEnd({
            modeForce: true
        });
        // modal - confirm
        if (modeConfirm) {
            await sqlPostMessage({
                // ui-render sql-template from .inputDbCrud-xxx
                sql: document.querySelector(
                    "#datatables_buttons_info textarea"
                ).value.replace((
                    /"\{\{([_a-z]*?)\}\}"/g
                ), function (match0, identifier) {
                    document.querySelectorAll(
                        ".inputDbCrud-" + identifier + " input, " +
                        ".inputDbCrud-" + identifier + " select"
                    ).forEach(function (elem) {
                        match0 = "\"" + (
                            elem.tagName === "SELECT"
                            ? elem.selectedOptions[0].value
                            : elem.value
                        ) + "\"";
                    });
                    return match0;
                })
            });
            await onDbRefresh({});
            return;
        }
        // modal - ui-render
        crud = t_type + " - " + crud;
        tmp = Math.random().toString(16).slice(2);
        switch (crud) {
        case "editor - none":
            return;
        case "result - save to csv":
            data = jsonRowListToCsv({
                colList,
                rowList
            });
            await onDbSave({
                data,
                file: "sqlite-result.csv"
            });
            return;
        case "table - save to csv":
            data = await sqlPostMessage({
                sql: "SELECT * FROM \"" + t_name + "\";\n"
            });
            data = jsonRowListToCsv(data.tableList[0]);
            await onDbSave({
                data,
                file: "sqlite-table.csv"
            });
            return;
        }
        String(`
-- index - drop
DROP INDEX
    "{{t_name}}";

-- index - rename
DROP INDEX
    "{{t_name}}";
` + tableRename({
            t_name,
            t_name2: "{{new_index}}",
            t_sql
        }) + `;

-- row - delete
DELETE FROM
    "{{t_name}}"
WHERE
    rowid = ${rowid};

-- row - insert
INSERT INTO
    "{{t_name}}"
(
    ${"\"" + colList.slice(1).join("\",\n    \"") + "\""}
)
VALUES
(
    ${new Array(Math.max(colList.length - 1, 0)).fill("NULL").join(",\n    ")}
);

-- row - update
UPDATE
    "{{t_name}}"
SET
    ${"\"" + colList.slice(1).join("\" = NULL,\n    \"") + "\" = NULL"}
WHERE
    rowid = ${rowid};

-- table - add column
ALTER TABLE
    "{{t_name}}"
ADD
    "{{new_column}}" TEXT NOT NULL DEFAULT '';

-- table - add index
CREATE
    -- UNIQUE
INDEX
    "{{new_index}}"
ON "{{t_name}}" (
    "{{selected_column}}"
);

-- table - clone data
CREATE TABLE
    "{{new_table}}"
AS SELECT * FROM
    "{{t_name}}";

-- table - clone data, schema, index
` + tableRename({
            t_name,
            t_name2: "{{new_table}}",
            t_sql
        }) + i_list.map(function ({
            t_name,
            t_sql,
            tbl_name
        }) {
            return tableRename({
                i_name: t_name,
                i_name2: t_name + "_" + tmp,
                t_name: tbl_name,
                t_name2: "{{new_table}}",
                t_sql
            });
        }).join("") + `;
INSERT INTO
    "{{new_table}}"
SELECT * FROM
    "{{t_name}}";

-- table - create
CREATE TABLE
    "{{new_table}}"
(
    "id" INTEGER NOT NULL PRIMARY KEY,
    "column_real" REAL NOT NULL DEFAULT 0.0,
    "column_text" TEXT NOT NULL DEFAULT '',
    "column_blob" BLOB
);

-- table - drop
DROP TABLE
    "{{t_name}}";

-- table - rename
ALTER TABLE
    "{{t_name}}"
RENAME TO
    "{{new_table}}";

-- table - rename column
ALTER TABLE
    "{{t_name}}"
RENAME
    "{{selected_column}}"
TO
    "{{new_column}}";
        `).split("\n\n").some(function (match0) {
            match0 = match0.trim();
            if (match0.indexOf("-- " + crud + "\n") === 0) {
                sql = match0.replace((
                    /\{\{t_name\}\}/g
                ), t_name).replace((
                    /\{\{t_type\}\}/g
                ), t_type);
                return true;
            }
        });
        assertOrThrow(sql, "invalid crud-action " + JSON.stringify(crud));
        colList = colList.map(function (key) {
            return (
                "<option value=\"" + stringHtmlSafe(key) + "\">" +
                stringHtmlSafe(key) +
                "</option>\n"
            );
        }).join("");
        uiLoaderInfo(crud, (`
<div>
    <table>
    <tbody>
    <tr class="inputDbCrud-new_table">
        <td><span>"{{new_table}}"</span></td>
        <td><input type="text" value="new_table_1"></td>
    </tr>
    <tr class="inputDbCrud-new_index">
        <td><span>"{{new_index}}"</span></td>
        <td><input type="text" value="new_index_1"></td>
    </tr>
    <tr class="inputDbCrud-selected_column">
        <td>
            <span>"{{selected_column}}"</span>
        </td>
        <td>
            <select type="text" value="selected_column_1">${colList}</select>
        </td>
    </tr>
    <tr class="inputDbCrud-new_column">
        <td><span>"{{new_column}}"</span></td>
        <td><input type="text" value="new_column_1"></td>
    </tr>
    </tbody>
    </table>
</div>
<div>
    <div>inspect and/or edit sql, then click "ok"</div>
    <textarea>${stringHtmlSafe(sql)}</textarea>
</div>
<div style="text-align: right;">
    <button id="dbCrudConfirm1" class="button">ok</button>
    <button
        id="dbCrudCancel1"
        class="button"
        style="margin-left: 20px;"
    >cancel</button>
</div>
        `));
        uiLoaderInfo = noop;
        // modal - vertical-center
        setTimeout(function () {
            document.querySelectorAll(
                "#datatables_buttons_info"
            ).forEach(function (elem) {
                elem.style.marginTop = -(0.5 * elem.clientHeight + 20) + "px";
            });
        }, 10);
        sql.replace((
            /"\{\{([_a-z]*?)\}\}"/g
        ), function (ignore, identifier) {
            document.querySelectorAll(
                ".inputDbCrud-" + identifier
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            return "";
        });
        // init evt-handling
        [
            [
                "#dbCrudCancel1", "click", uiLoaderEnd.bind(undefined, {
                    modeForce: true
                })
            ],
            [
                "#dbCrudConfirm1", "click", onDbCrud.bind(undefined, {
                    modeConfirm: true
                })
            ],
            [
                "#dbCrudConfirm1", "click", uiLoaderStart.bind(undefined, {
                    modeForce: true
                })
            ]
        ].forEach(function ([
            selector, evt, cb
        ]) {
            document.querySelector(selector).addEventListener(evt, cb);
        });
    });

    async function onFileOpen({
        colList,
        currentTarget,
        rowList,
        set
    }) {
    /*
     * this function will open db from file
     */
        switch (currentTarget.id) {
        case "fileOpenCsv1":
            document.querySelector("#fileOpenCsv2").value = "";
            document.querySelector("#fileOpenCsv2").click();
            return;
        case "fileOpenDb1":
            document.querySelector("#fileOpenDb2").value = "";
            document.querySelector("#fileOpenDb2").click();
            return;
        case "fileOpenScript1":
            document.querySelector("#fileOpenScript2").value = "";
            document.querySelector("#fileOpenScript2").click();
            return;
        }
        switch (currentTarget.id) {
        case "fileOpenCsv2":
            uiLoaderStart({});
            rowList = await currentTarget.files[0].text();
            rowList = jsonRowListFromCsv({
                csv: rowList
            });
            set = new Set();
            colList = rowList.shift().map(function (col) {
                let col2;
                let ii;
                ii = 0;
                // sanitize col
                col = stringSqlnameSafe(col);
                // ensure unique col2
                col2 = col;
                while (true) {
                    if (!set.has(col2)) {
                        set.add(col2);
                        return col2;
                    }
                    ii += 1;
                    col2 = col + ii;
                }
            }).join(
                " TEXT NOT NULL DEFAULT '',\n"
            ) + " TEXT NOT NULL DEFAULT ''\n";
            rowList = "('" + rowList.map(function (row) {
                return row.join("\r");
            }).join("\n").replace((
                /'/g
            ), "''").replace((
                /\r/g
            ), "','").replace((
                /\n/g
            ), "'),\n('") + "');\n";
            try {
                await sqlPostMessage({
                    sql: (`
BEGIN TRANSACTION;
DROP TABLE IF EXISTS __csv1__;
CREATE TABLE __csv1__ (
${colList}
);
INSERT INTO __csv1__ VALUES
${rowList}
COMMIT;
                    `)
                });
            } catch (errCaught) {
                await sqlPostMessage({
                    sql: "ROLLBACK;\n"
                });
                assertOrThrow(undefined, errCaught);
            }
            onDbRefresh({});
            return;
        case "fileOpenDb2":
            if (currentTarget.files.length === 0) {
                return;
            }
            sqlPostMessage({
                action: "close"
            });
            await sqlPostMessage({
                action: "open",
                data: (
                    await currentTarget.files[0].arrayBuffer()
                )
            });
            onDbRefresh({});
            return;
        case "fileOpenScript2":
            uiLoaderStart({});
            currentTarget = await currentTarget.files[0].text();
            sqlEditor.setValue(currentTarget);
            onDbRefresh({});
            return;
        }
    }

    // init $
    $.prototype.datatable = $.prototype.DataTable;
    // init Chart.defaults.global.plugins.colorschemes
    // https://nagix.github.io/chartjs-plugin-colorschemes/colorchart.html
    Chart.defaults.global.plugins.colorschemes = {
        fillAlpha: 0.5,
        override: false,
        reverse: false,
        // scheme: 'brewer.Paired12'
        "brewer.Paired10": [
            "#a6cee3", "#1f78b4", "#b2df8a", "#33a02c",
            "#fb9a99", "#e31a1c", "#fdbf6f", "#ff7f00",
            "#cab2d6", "#6a3d9a"
        ],
        "brewer.Paired12": [
            "#a6cee3", "#1f78b4", "#b2df8a", "#33a02c",
            "#fb9a99", "#e31a1c", "#fdbf6f", "#ff7f00",
            "#cab2d6", "#6a3d9a", "#ffff99", "#b15928"
        ],
        "highcharts": [
            "#4572a7", "#aa4643", "#89a54e", "#80699b",
            "#3d96ae", "#db843d", "#92a8cd", "#a47d7c",
            "#b5ca92"
        ],
        "tableau.Tableau20": [
            "#4e79a7", "#a0cbe8", "#f28e2b", "#ffbe7d",
            "#59a14f", "#8cd17d", "#b6992d", "#f1ce63",
            "#499894", "#86bcb6", "#e15759", "#ff9d9a",
            "#79706e", "#bab0ac", "#d37295", "#fabfd2",
            "#b07aa1", "#d4a6c8", "#9d7660", "#d7b5a6"
        ],
        scheme: Array.from([
            "#a6cee3", "#1f78b4", "#b2df8a", "#33a02c",
            "#fb9a99", "#e31a1c", "#fdbf6f", "#ff7f00",
            "#cab2d6", "#6a3d9a", "#d2b48c", "#b15928"
        ]).map(function (rgb, ii) {
            return "#" + Array.from([
                rgb.slice(1, 3),
                rgb.slice(3, 5),
                rgb.slice(5, 7)
            ]).map(function (elem) {
                elem = Number("0x" + elem);
                if (ii % 2 === 0) {
                    elem *= 0.75;
                }
                return Math.round(elem).toString(16).padStart(2, "0");
            }).join("");
        })
    };
    // init uiLoaderInfo
    uiLoaderInfo = $("#uiLoader1").datatable({
        dom: ""
    }).buttons;
    uiLoaderInfo = uiLoaderInfo.info.bind(uiLoaderInfo);
    uiLoaderInfo0 = uiLoaderInfo;
    uiLoaderStart({});
    // init sqlWorker
    sqlCallbackDict = {};
    sqlCallbackId = 1;
    sqlResultDict = {};
    sqlWorker = new Worker("sqlmath_wasm.js?initSqlJsWorker=1");
    sqlWorker.onmessage = sqlOnMessage;
    sqlPostMessage({
        action: "open"
    });
    // init sqlEditor
    sqlEditor = CodeMirror.fromTextArea(document.querySelector(
        "#editor1 textarea"
    ), {
        extraKeys: {
            "Ctrl-Enter": onDbExec,
            "Ctrl-S": onDbSave.bind(undefined, {}),
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
    // init evt-handling
    [
        [
            "#dbExec1", "click", onDbExec
        ],
        [
            "#dbExec2", "click", onDbExec
        ],
        [
            "#dbRefresh1", "click", onDbRefresh
        ],
        [
            "#dbSave1", "click", onDbSave.bind(undefined, {})
        ],
        [
            "#dbTableCreate1", "click", onDbCrud.bind(undefined, {
                crud: "create",
                t_type: "table"
            })
        ],
        [
            "#fileOpenCsv2", "change", onFileOpen
        ],
        [
            "#fileOpenCsv1", "click", onFileOpen
        ],
        [
            "#fileOpenDb2", "change", onFileOpen
        ],
        [
            "#fileOpenDb1", "click", onFileOpen
        ],
        [
            "#fileOpenScript2", "change", onFileOpen
        ],
        [
            "#fileOpenScript1", "click", onFileOpen
        ],
        [
            "#panelSidebar1", "click", uiHashtagScrollto
        ],
        [
            "#scriptSave1", "click", function () {
                onDbSave({
                    data: sqlEditor.getValue(),
                    file: "sqlite-script.txt"
                });
            }
        ],
        [
            "body", "click", onContextmenu
        ],
        [
            "body", "keydown", uiLoaderEnd
        ],
        [
            "body", "contextmenu", onContextmenu
        ]
    ].forEach(function ([
        selector, evt, cb
    ]) {
        document.querySelector(selector).addEventListener(evt, cb);
    });
    // reset location.hash
    location.hash = "";
    // style - validate
    setTimeout(domStyleValidate, 1000);
    // export
    Object.assign(window, {
        domStyleValidate,
        onDbCrud,
        onDbExec,
        onDbRefresh,
        sqlEditor,
        sqlPostMessage,
        sqlResultDict,
        uiLoaderInfo,
        uiRenderError
    });
    // init jsScript0
    jsScript0 = (
        /\bjsScript=([^&]+)/
    ).exec(location.search);
    if (jsScript0) {
        jsScript0.elem = document.createElement("script");
        jsScript0.elem.src = jsScript0[1];
        document.head.appendChild(jsScript0.elem);
    }
    // init sqlDb0
    sqlDb0 = (
        /\bsqlDb=([^&]+)/
    ).exec(location.search);
    if (sqlDb0) {
        sqlDb0 = await fetch(sqlDb0[1]);
        sqlDb0 = await sqlDb0.arrayBuffer();
        await sqlPostMessage({
            action: "open",
            data: sqlDb0
        });
    }
    // init sqlScript0
    sqlScript0 = (
        /\bsqlScript=([^&]+)/
    ).exec(location.search);
    sqlScript0 = sqlScript0 && sqlScript0[1];
    if (sqlScript0) {
        sqlScript0 = await fetch(sqlScript0);
        sqlScript0 = await sqlScript0.text();
        sqlEditor.setValue(sqlScript0);
    }
    // db - auto-exec-enable
    if (sqlScript0 || !(sqlDb0 || jsScript0)) {
        await onDbExec({});
    // db - auto-exec-disable
    } else {
        await onDbRefresh({});
    }
});
/*jslint-enable*/
