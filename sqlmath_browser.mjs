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
let CRISPX = -0.5;
let CRISPY = 0.5;
let {
    CodeMirror
} = window;
let DBTABLE_DICT = new Map();
let DB_CHART;
let DB_DICT = new Map();
let DB_INIT = Promise.resolve();
let DB_MAIN;
let DB_QUERY;
let DEBOUNCE_DICT = Object.create(null);
let UI_ANIMATE_DATENOW;
let UI_ANIMATE_DURATION = 250;
let UI_ANIMATE_DURATION_INV = 1 / UI_ANIMATE_DURATION;
let UI_ANIMATE_LIST = [];
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
let UI_VIEW_SIZE = 20;

noop({
    UI_EDITOR,
    assertOrThrow,
    dbFileExportAsync,
    debugInline
});

async function dbFileAttachAsync({
    db,
    dbData
}) {
// this function will attach database <dbData> to <db>
    let dbAttached;
    let dbName = dbNameNext("attach{{ii}}", new Set(DB_DICT.keys()));
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
    // normalize order
    DB_DICT = Array.from(DB_DICT.values());
    DB_DICT = new Map([
        DB_DICT.slice(0, 3),
        DB_DICT.slice(3).sort(function (aa, bb) {
            aa = aa.dbName;
            bb = bb.dbName;
            return (
                aa < bb
                ? -1
                : 1
            );
        })
    ].flat().map(function (db) {
        return [
            db.dbName, db
        ];
    }));
}

function dbNameNext(template, bag) {
// this function will get next incremental name in <bag> from given <template>
    let ii = 0;
    let name;
    while (true) {
        ii += 1;
        name = template.replace("{{ii}}", String(ii).padStart(2, "0"));
        if (!bag.has(name)) {
            return name;
        }
    }
}

async function dbTableImportAsync({
    db,
    mode,
    tableName,
    textData
}) {
// this function will create table from imported csv/json <textData>
    let colList;
    let rowList;
    let rowidList;
    let tmp;
    switch (mode) {
    case "csv":
        rowList = jsonRowListFromCsv({
            csv: textData
        });
        break;
    // case "json":
    default:
        rowList = JSON.parse(textData);
    }
    if (!(typeof rowList === "object" && rowList)) {
        rowList = [];
    }
    // normalize rowList to list
    if (!Array.isArray(rowList)) {
        rowidList = [];
        rowList = Object.entries(rowList).map(function ([
            key, val
        ]) {
            rowidList.push(key);
            return val;
        });
    }
    // normalize rowList[ii] to list
    if (rowList.length === 0) {
        rowList.push([
            "undefined"
        ]);
    }
    if (!Array.isArray(rowList[0])) {
        colList = Array.from(
            new Set(
                rowList.map(function (obj) {
                    return Object.keys(obj);
                }).flat()
            )
        );
        rowList = rowList.map(function (obj) {
            return colList.map(function (key) {
                return obj[key];
            });
        });
        rowList.unshift(colList);
    }
    // init colList
    colList = rowList.shift();
    // preserve rowid
    if (rowidList) {
        colList.unshift("rowid");
        rowList.forEach(function (row, ii) {
            row.unshift(rowidList[ii]);
        });
    }
    // normalize colList
    tmp = new Set();
    colList = colList.map(function (colName) {
        let colName2;
        let duplicate = 0;
        colName = "c_" + colName.toLowerCase().replace((
            /\W/g
        ), "_");
        while (true) {
            duplicate += 1;
            colName2 = (
                duplicate === 1
                ? colName
                : colName + "_" + duplicate
            );
            if (!tmp.has(colName2)) {
                tmp.add(colName2);
                return colName2;
            }
        }
    });
    // create dbtable from rowList
    await dbExecAsync({
        bindList: {
            rowList: JSON.stringify(rowList)
        },
        db,
        sql: (
            rowList.length === 0
            ? `CREATE TABLE ${tableName} (${colList.join(",")});`
            : (
                `CREATE TABLE ${tableName} AS SELECT `
                + colList.map(function (colName, ii) {
                    return "value->>" + ii + " AS " + colName;
                }).join(",")
                + " FROM JSON_EACH($rowList);"
            )
        )
    });
}

function debounce(key, func, ...argList) {
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
            val.func(...argList);
        }, 250)
    };
    DEBOUNCE_DICT[key] = val;
    // if first-time, then immediately call <func>
    func(...argList);
}

async function demoDefault() {
// this function will run demo-default
    // attach demo-db
    await dbFileAttachAsync({
        db: DB_MAIN,
        dbData: new ArrayBuffer(0)
    });
    UI_EDITOR.setValue(String(`
DROP TABLE IF EXISTS __stock_historical;
CREATE TABLE __stock_historical(sym TEXT, date TEXT, price REAL);
INSERT INTO __stock_historical (sym, date, price) VALUES
    ('aapl', '2020-01-01', 77.37),  ('aapl', '2020-02-01', 68.33),
    ('aapl', '2020-03-01', 63.57),  ('aapl', '2020-04-01', 73.44),
    ('aapl', '2020-05-01', 79.48),  ('aapl', '2020-06-01', 91.19),
    ('aapl', '2020-07-01', 106.26),  ('aapl', '2020-08-01', 129.03),
    ('aapl', '2020-09-01', 115.80),  ('aapl', '2020-10-01', 108.86),
    ('aapl', '2020-11-01', 119.05),  ('aapl', '2020-12-01', 132.69),
    ('goog', '2020-01-01', 1434.23),  ('goog', '2020-02-01', 1339.33),
    ('goog', '2020-03-01', 1162.81),  ('goog', '2020-04-01', 1348.66),
    ('goog', '2020-05-01', 1428.92),  ('goog', '2020-06-01', 1413.61),
    ('goog', '2020-07-01', 1482.96),  ('goog', '2020-08-01', 1634.18),
    ('goog', '2020-09-01', 1469.60),  ('goog', '2020-10-01', 1621.01),
    ('goog', '2020-11-01', 1760.74),  ('goog', '2020-12-01', 1751.88);

DROP TABLE IF EXISTS __test1;
CREATE TABLE __test1(
    col1,
    col2,
    column_long_name_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
);

DROP TABLE IF EXISTS __test2;
CREATE TABLE __test2 AS
    SELECT * FROM __test1
    UNION ALL SELECT 1, 2, 3
    UNION ALL SELECT 4, 5, 6;

DROP TABLE IF EXISTS attach01.__test3;
CREATE TABLE attach01.__test3 AS SELECT * FROM __test2;

SELECT
        *,
        random() AS c1,
        random() AS c2,
        random() AS c3,
        random() AS c4,
        random(),
        random(),
        random(),
        random(),
        1 AS sentinel
    FROM __stock_historical
    LEFT JOIN __test1 ON __test1.col1 = __stock_historical.sym
    CROSS JOIN (SELECT random() FROM __stock_historical);

DROP TABLE IF EXISTS chart.__stock_chart;
CREATE TABLE chart.__stock_chart (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart.__stock_chart (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "title": "price vs. date comparison of multiple stocks",
            "xaxisTitle": "date",
            "yaxisTitle": "percent gain",
            "yvalueSuffix": " %"
        }' AS options;
INSERT INTO chart.__stock_chart (datatype, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY sym) AS rownum,
            sym
        FROM (SELECT DISTINCT sym FROM __stock_historical)
        WHERE
            sym IS NOT NULL
    );
INSERT INTO chart.__stock_chart (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        date AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY date) AS rownum,
            date
        FROM (SELECT DISTINCT date FROM __stock_historical)
    );
INSERT INTO chart.__stock_chart (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        price AS yy
    FROM (
        SELECT
            series_index,
            series_label,
            xx,
            xx_label
        FROM (
            SELECT
                series_index,
                series_label
            FROM chart.__stock_chart
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM chart.__stock_chart
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN __stock_historical ON sym = series_label AND date = xx_label;
UPDATE chart.__stock_chart
    SET
        yy = yy * inv - 1
    FROM (
    --
    SELECT
        1.0 / yy AS inv,
        series_index
    FROM (
        SELECT
            ROW_NUMBER() OVER (
                PARTITION BY series_index ORDER BY xx
            ) AS rownum,
            yy,
            series_index
        FROM chart.__stock_chart
        WHERE
            datatype = 'yy_value'
            AND yy > 0
    )
    WHERE
        rownum = 1
    --
    ) AS __join1 WHERE __join1.series_index = __stock_chart.series_index;
    `).trim() + "\n");
    // exec demo-sql-query
    await onDbExec({});
    return true;
}

async function demoTradebot() {
// this function will run demo-tradebot
    let tmp;
    let tradebotState;
    try {
        tmp = await fetch(`.tradebot_public.sqlite?aa=${Date.now()}`);
        if (tmp.status !== 200) {
            return;
        }
    } catch (ignore) {
        return;
    }
    tmp = await tmp.arrayBuffer();
    await dbFileImportAsync({
        db: DB_MAIN,
        dbData: tmp
    });
    tradebotState = noop(
        await dbExecAsync({
            db: DB_MAIN,
            sql: "SELECT * FROM tradebot_state;"
        })
    )[0][0];
    UI_EDITOR.setValue([
        (`
-- table - tradebot_intraday_day - insert
DROP TABLE IF EXISTS tradebot_intraday_day;
CREATE TABLE tradebot_intraday_day AS
    SELECT
        tradebot_intraday.*
    FROM tradebot_intraday
    JOIN tradebot_state
    WHERE
        DATE(ydate) >= datemkt0;
INSERT INTO tradebot_intraday_day
    SELECT
        sym,
        DATETIME(__ydate, '-1 MINUTE'),
        0 AS ttt,
        price,
        0 AS ydate2
    FROM tradebot_historical
    JOIN (SELECT MIN(ydate) AS __ydate FROM tradebot_intraday_day)
    JOIN (
        SELECT
            MAX(ydate) AS ydate
        FROM tradebot_historical
        JOIN tradebot_state
        WHERE
            sym = 'spy'
            AND ydate < datemkt0
    ) USING (ydate);

-- table - tradebot_intraday_week - insert
DROP TABLE IF EXISTS tradebot_intraday_week;
CREATE TABLE tradebot_intraday_week AS
    SELECT
        tradebot_intraday.*
    FROM tradebot_intraday
    JOIN (SELECT DATE(datemkt0, '-6 DAY') AS ydate_week FROM tradebot_state)
    WHERE
        ydate = ydate2
        AND ydate > ydate_week;
INSERT INTO tradebot_intraday_week
    SELECT
        sym,
        ydate || ' ' || hhmmss AS ydate,
        0 AS ttt,
        price,
        0 AS ydate2
    FROM tradebot_historical
    JOIN (
        SELECT
            MAX(ydate) AS ydate
        FROM tradebot_historical
        JOIN tradebot_state
        WHERE
            ydate < DATE(datemkt0, '-6 DAY')
    ) USING (ydate)
    JOIN (SELECT TIME(MAX(ydate)) AS hhmmss FROM tradebot_intraday_week);

-- table - tradebot_technical_week - insert - lmt
DROP TABLE IF EXISTS tradebot_technical_week;
CREATE TABLE tradebot_technical_week(tname TEXT, tt REAL, tval REAL);
INSERT INTO tradebot_technical_week
    SELECT
        tname,
        datemkt_beg AS tt,
        stk_beg0 AS tval
    FROM tradebot_state
    JOIN (
                  SELECT '1b_stk_lmt' AS tname
        UNION ALL SELECT '1c_stk_pct'
        UNION ALL SELECT '1d_stk_lmb'
        UNION ALL SELECT '1e_stk_lms'
        UNION ALL SELECT '1f_stk_pnl'
        UNION ALL SELECT '2b_sqq_lmt'
        UNION ALL SELECT '2c_sqq_pct'
        UNION ALL SELECT '2d_sqq_lmb'
        UNION ALL SELECT '2e_sqq_lms'
        UNION ALL SELECT '2f_sqq_pnl'
    );
INSERT INTO tradebot_technical_week
    SELECT
        tname,
        datemkt_beg AS tt,
        IIF(tname LIKE '1%', stk_beg0, sqq_beg0) AS tval
    FROM tradebot_state
    JOIN (
                  SELECT '1b_stk_lmt' AS tname
        UNION ALL SELECT '1c_stk_pct'
        UNION ALL SELECT '1d_stk_lmb'
        UNION ALL SELECT '1e_stk_lms'
        UNION ALL SELECT '1f_stk_pnl'
        UNION ALL SELECT '2b_sqq_lmt'
        UNION ALL SELECT '2c_sqq_pct'
        UNION ALL SELECT '2d_sqq_lmb'
        UNION ALL SELECT '2e_sqq_lms'
        UNION ALL SELECT '2f_sqq_pnl'
    )
    --
    UNION ALL
    --
    SELECT '1b_stk_lmt', ydate, stk_lmt FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '1c_stk_pct', ydate, stk_pct FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '1d_stk_lmb', ydate, stk_lmb FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '1e_stk_lms', ydate, stk_lms FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '1f_stk_pnl', ydate, stk_pnl FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '2b_sqq_lmt', ydate, sqq_lmt FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '2c_sqq_pct', ydate, sqq_pct FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '2d_sqq_lmb', ydate, sqq_lmb FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '2e_sqq_lms', ydate, sqq_lms FROM tradebot_intraday_lmt
    --
    UNION ALL
    --
    SELECT '2f_sqq_pnl', ydate, sqq_pnl FROM tradebot_intraday_lmt;

-- table - tradebot_technical_week - insert - spy
INSERT INTO tradebot_technical_week
    SELECT
        '1a_spy' AS tname,
        ydate AS tt,
        price
    FROM tradebot_intraday_day
    WHERE
        sym = 'spy';
INSERT OR IGNORE INTO tradebot_technical_week
    SELECT
        '1a_spy' AS tname,
        ydate AS tt,
        price
    FROM tradebot_intraday_week
    WHERE
        sym = 'spy';

-- table - tradebot_technical_day - insert
DROP TABLE IF EXISTS tradebot_technical_day;
CREATE table tradebot_technical_day AS
    SELECT
        tradebot_technical_week.*
    FROM tradebot_state
    JOIN tradebot_technical_week
    WHERE
        datemkt_beg <= tt;

-- table - tradebot_technical_week - cleanup - date2
DELETE FROM tradebot_technical_week
    WHERE
        tt NOT IN (SELECT DISTINCT ydate2 FROM tradebot_intraday_week);
        `),
        [
            "1 day",
            "1 week",
            "1 month",
            "6 month",
            "ytd",
            "1 year",
            "5 year",
            "5 year reverse timeline"
        ].map(function (dateInterval) {
            let optionDict;
            let tableChart;
            let tableData;
            tableData = (
                dateInterval === "1 day"
                ? "tradebot_intraday_day"
                : dateInterval === "1 week"
                ? "tradebot_intraday_week"
                : "tradebot_historical"
            );
            tableChart = (
                "chart._{{ii}}_tradebot_historical_"
                + dateInterval.replace((
                    /\W/g
                ), "_")
            );
            optionDict = {
                title: (
                    "tradebot historical performance vs market - "
                    + dateInterval
                    + (
                        dateInterval === "1 day"
                        ? "\n[ updated " + new Date(
                            tradebotState.datenow + "Z"
                        ).toUTCString() + " ]"
                        : ""
                    )
                ),
                xaxisTitle: "date",
                xstep: (
                    dateInterval === "1 day"
                    ? 60
                    : dateInterval === "1 week"
                    ? 15 * 60
                    : 1
                ),
                xvalueConvert: (
                    (dateInterval === "1 day" || dateInterval === "1 week")
                    ? "unixepochToTimeutc"
                    : "juliandayToDate"
                ),
                yaxisTitle: "percent gain",
                yvalueSuffix: " %"
            };
            return (`
-- chart - ${tableChart} - create
DROP TABLE IF EXISTS ${tableChart};
CREATE TABLE ${tableChart} (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '${JSON.stringify(optionDict)}' AS options;
INSERT INTO ${tableChart} (datatype, options, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT(
            'isDummy', is_dummy,
            'isHidden', sym NOT in ('11_mybot', 'spy', 'dia', 'qqq')
        ) AS options,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            sym LIKE '-%' AS is_dummy,
            ROW_NUMBER() OVER (
                ORDER BY
                    sym = '11_mybot' DESC,
                    sym = '----' DESC,
                    sym = 'spy' DESC,
                    sym = 'dia' DESC,
                    sym = 'qqq' DESC,
                    sym = '---- ' DESC,
                    sym
            ) AS rownum,
            sym
        FROM (
            SELECT DISTINCT sym FROM ${tableData}
            --
            UNION ALL
            --
            SELECT '----'
            --
            UNION ALL
            --
            SELECT '---- '
        )
    );
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS
    SELECT
        *
    FROM (SELECT DISTINCT ydate FROM ${tableData})
    JOIN (SELECT MIN(ydate) AS aa, MAX(ydate) AS bb FROM ${tableData});
UPDATE __tmp1
    SET
        aa = aa2
    FROM (
        SELECT
            ydate AS aa2
        FROM __tmp1
        JOIN (
            SELECT
                MAX(aa,
            ${
                (
                    dateInterval === "5 year reverse timeline"
                    ? "DATE(bb, '-5 YEAR')"
                    : dateInterval === "ytd"
                    ? "DATE(STRFTIME('%Y', bb) || '-01-01', '-1 DAY')"
                    : "DATE(bb, '-" + dateInterval + "')"
                )
            }
                ) AS aa2
            FROM (SELECT aa, bb FROM __tmp1 LIMIT 1)
        )
        WHERE
            ydate <= aa2
        ORDER BY
            ydate DESC
        LIMIT 1
    );
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        ydate AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY ydate ASC) AS rownum,
            ydate
        FROM __tmp1
        WHERE
            aa <= ydate AND ydate <= bb
    );
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        price AS yy
    FROM (
        SELECT
            series_index,
            series_label,
            xx,
            xx_label
        FROM (
            SELECT
                series_index,
                series_label
            FROM ${tableChart}
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM ${tableChart}
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN ${tableData} ON sym = series_label AND ydate = xx_label;
UPDATE ${tableChart}
    SET
            ${
                (
                    dateInterval === "5 year reverse timeline"
                    ? "yy = ROUND(100 * (1.0 / (yy * inv) - 1), 4)"
                    : "yy = ROUND(100 * (yy * inv - 1), 4)"
                )
            }
    FROM (
    --
    SELECT
        1.0 / yy AS inv,
        series_index
    FROM (
        SELECT
            ROW_NUMBER() OVER (
                PARTITION BY series_index ORDER BY xx
            ${
                (
                    dateInterval === "5 year reverse timeline"
                    ? "DESC"
                    : "ASC"
                )
            }
            ) AS rownum,
            yy,
            series_index
        FROM ${tableChart}
        WHERE
            datatype = 'yy_value'
            AND yy > 0
    )
    WHERE
        rownum = 1
    --
    ) AS __join1 WHERE __join1.series_index = ${tableChart}.series_index;
UPDATE ${tableChart}
    SET
        series_label = printf(
            '%+06.2f%% - %s%s',
            yy_today,
            series_label,
            IIF(CASTTEXTOREMPTY(company_name) = '', '', ' - ' || company_name)
        )
    FROM (
    --
    SELECT
        ${tableChart}.rowid,
        --
        company_name,
        yy_today
    FROM ${tableChart}
    --
    LEFT JOIN (
        SELECT
            series_index,
            yy_today
        FROM (
            SELECT
                ROW_NUMBER() OVER (
                    PARTITION BY series_index ORDER BY xx DESC
                ) AS rownum,
                series_index,
                yy AS yy_today
            FROM ${tableChart}
            WHERE
                datatype = 'yy_value'
        )
        WHERE
            rownum = 1
    ) USING (series_index)
    LEFT JOIN tradebot_stock_basket ON sym = series_label
    WHERE
        datatype = 'series_label'
    --
    ) AS __join1 WHERE __join1.rowid = ${tableChart}.rowid;
-- chart - tradebot_historical - normalize xx to unixepoch
UPDATE ${tableChart}
    SET
        xx = ${(
                (dateInterval === "1 day" || dateInterval === "1 week")
                ? "UNIXEPOCH(tt)"
                : "JULIANDAY(tt)"
            )}
    FROM (
    --
    SELECT
        ${tableChart}.rowid,
        --
        tt
    FROM ${tableChart}
    --
    JOIN (
        SELECT
            xx,
            xx_label AS tt
        FROM ${tableChart}
        WHERE
            datatype = 'xx_label'
    ) USING (xx)
    WHERE
        datatype = 'yy_value'
    --
    ) AS __join1 WHERE __join1.rowid = ${tableChart}.rowid;
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx0 AS xx,
        0 AS yy
    FROM (
        SELECT
            series_index
        FROM ${tableChart}
        WHERE
            datatype = 'series_label'
    )
    JOIN (
        SELECT
            MIN(xx) AS xx0
        FROM ${tableChart}
        WHERE
            datatype = 'yy_value'
    )
    WHERE
        ${(dateInterval === "1 day" || dateInterval === "1 week")};
DELETE FROM ${tableChart} WHERE datatype = 'xx_label';
            `);
        }),
        [
            "sector",
            "subsector",
            "stock"
        ].map(function (grouping) {
            return [
                "performance",
                "holding"
            ].map(function (ptype) {
                let columnData;
                let optionDict;
                let sqlSelect;
                let tableChart;
                columnData = (
                    ptype === "performance"
                    ? "perc_gain_today"
                    : "perc_holding"
                );
                optionDict = {
                    isBarchart: true,
                    title: `tradebot ${ptype} today by ${grouping}`,
                    xaxisTitle: grouping,
                    yaxisTitle: (
                        ptype === "performance"
                        ? "percent gain"
                        : "percent holding"
                    ),
                    yvalueSuffix: " %"
                };
                sqlSelect = (
                    grouping === "sector"
                    ? (`
SELECT
        IIF(category LIKE 'short%', 1, grouping_index) AS series_color,
        category LIKE '-%' AS is_dummy,
        grouping IN ('account', 'exchange') AS is_hidden,
        printf(
            '%05.4f%% - %s - %s',
            ${columnData},
            grouping,
            category
        ) AS series_label,
        ROW_NUMBER() OVER (
            ORDER BY
                grouping_index,
                category != '----' DESC,
                ${columnData} DESC
        ) AS xx,
        category AS xx_label,
        ${columnData} AS yy
    FROM (
        SELECT
            category,
            grouping,
            grouping_index,
            ${columnData},
            perc_holding
        FROM tradebot_position_category
        WHERE
            grouping != 'subsector'
        --
        UNION ALL
        --
        SELECT
            '----' AS category,
            '----' AS grouping,
            grouping_index,
            NULL AS ${columnData},
            NULL perc_holding
        FROM (
            SELECT DISTINCT
                grouping,
                grouping_index
            FROM tradebot_position_category
        )
    )
                    `)
                    : grouping === "subsector"
                    ? (`
SELECT
        IIF(category LIKE 'short%', 1, grouping_index) AS series_color,
        category LIKE '-%' AS is_dummy,
        0 AS is_hidden,
        printf('%05.4f%% - %s', ${columnData}, category) AS series_label,
        ROW_NUMBER() OVER (
            ORDER BY
                grouping_index,
                category != '----' DESC,
                ${columnData} DESC
        ) AS xx,
        SUBSTR(category, INSTR(category, '____') + 4) AS xx_label,
        ${columnData} AS yy
    FROM (
        SELECT
            category,
            grouping,
            grouping_index,
            ${columnData},
            perc_holding
        FROM tradebot_position_category
        WHERE
            grouping = 'subsector'
    )
                    `)
                    : (`
SELECT
        IIF(is_short, 1, 2) AS series_color,
        0 AS is_dummy,
        0 AS is_hidden,
        printf(
            '%+.4f%% - %s - %s - %s',
            ${columnData},
            IIF(is_short, 'short', 'long'),
            sym,
            company_name
        ) AS series_label,
        ROW_NUMBER() OVER (ORDER BY ${columnData} DESC) AS xx,
        sym AS xx_label,
        ${columnData} AS yy
    FROM tradebot_position_stock
                    `)
                );
                tableChart = `chart._{{ii}}_tradebot_${grouping}_${ptype}`;
                return (`
-- chart - ${tableChart} - create
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS SELECT * FROM (${sqlSelect}) ORDER BY xx;
DROP TABLE IF EXISTS ${tableChart};
CREATE TABLE ${tableChart} (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '${JSON.stringify(optionDict)}' AS options;
INSERT INTO ${tableChart} (
    datatype,
    options,
    series_index,
    series_label
)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT(
            'isDummy', is_dummy,
            'isHidden', is_hidden,
            'seriesColor', series_color
        ) AS options,
        xx AS series_index,
        series_label
    FROM __tmp1;
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        xx,
        xx_label
    FROM __tmp1;
INSERT INTO ${tableChart} (
    datatype,
    series_index,
    xx,
    yy
)
    SELECT
        'yy_value' AS datatype,
        xx AS series_index,
        xx,
        ROUNDORZERO(yy, 4) AS yy
    FROM __tmp1;
                `);
            });
        }),
        (`
-- chart - tradebot_buysell_history
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS
    SELECT
        series_color,
        printf(
            '%s %s',
            xx_label,
            series_label
        ) AS series_label,
        xx,
        xx_label,
        yy
    FROM (
        SELECT
            IIF(buy_or_sell = 'buy', 2, 1) AS series_color,
            (
                buy_or_sell
                || ' '
                || sym
                || ' - '
                || company_name
            ) AS series_label,
            UNIXEPOCH(time_filled) AS xx,
            TIME(time_filled) AS xx_label,
            ROUNDORZERO(
                (
                    100 * 1.0 / asset_under_mgmt
                    * IIF(buy_or_sell = 'buy', 1, -1)
                    * order_value
                ),
                4
            ) AS yy
        FROM tradebot_account
        JOIN tradebot_buysell_history
        ORDER BY
            time_filled
    );
DROP TABLE IF EXISTS chart._{{ii}}_tradebot_buysell_history;
CREATE TABLE chart._{{ii}}_tradebot_buysell_history (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart._{{ii}}_tradebot_buysell_history (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "isBarchart": true,
            "title": "tradebot buy/sell history today\\n[ updated '
                || '${new Date(tradebotState.datenow + "Z").toUTCString()}'
                || ' ]",
            "xaxisTitle": "time",
            "xvalueConvert": "unixepochToTimeutc",
            "yaxisTitle": "buy/sell value as percentage of account",
            "yvalueSuffix": " %"
        }' AS options
    FROM tradebot_state;
INSERT INTO chart._{{ii}}_tradebot_buysell_history (
    datatype,
    options,
    series_index,
    series_label
)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT('seriesColor', series_color) AS options,
        __tmp1.rowid AS series_index,
        series_label
    FROM __tmp1;
INSERT INTO chart._{{ii}}_tradebot_buysell_history (
    datatype,
    series_index,
    xx,
    yy
)
    SELECT
        'yy_value' AS datatype,
        __tmp1.rowid AS series_index,
        xx,
        yy
    FROM __tmp1;
        `),
        [
            "1 day",
            "1 week"
        ].map(function (dateInterval) {
            let optionDict;
            let tableChart;
            let tableData;
            tableData = (
                dateInterval === "1 day"
                ? "tradebot_technical_day"
                : dateInterval === "1 week"
                ? "tradebot_technical_week"
                : "tradebot_technical"
            );
            tableChart = (
                "chart._{{ii}}_tradebot_technical_"
                + dateInterval.replace((
                    /\W/g
                ), "_")
            );
            optionDict = {
                title: (
                    "tradebot technical - "
                    + dateInterval
                    + (
                        dateInterval === "1 day"
                        ? "\n[ updated " + new Date(
                            tradebotState.datenow + "Z"
                        ).toUTCString() + " ]"
                        : ""
                    )
                ),
                xaxisTitle: "date",
                xstep: (
                    dateInterval === "1 day"
                    ? 60
                    : dateInterval === "1 week"
                    ? 15 * 60
                    : 1
                ),
                xvalueConvert: (
                    (dateInterval === "1 day" || dateInterval === "1 week")
                    ? "unixepochToTimeutc"
                    : "juliandayToDate"
                ),
                yaxisTitle: "percent holding",
                yvalueSuffix: " %"
            };
            return (`
-- table - ${tableData} - normalize
UPDATE ${tableData}
    SET
        tval = (CASE
            WHEN (tname = '1a_spy') THEN
                (lmt_eee * 1.0 / spy_eee) * (tval - spy_avg) + lmt_avg
            WHEN (tname = '1f_stk_pnl') THEN
                (lmt_eee * 1.0 / pnl_eee) * (tval - pnl_avg) + lmt_avg
        END)
    FROM (SELECT
    -- __join1
        lmt_avg,
        lmt_eee,
        pnl_avg,
        pnl_eee,
        spy_avg,
        spy_eee
    FROM (SELECT 0)
    JOIN (SELECT
        MEDIAN(tval) AS lmt_avg,
        STDEV(tval) AS lmt_eee
        FROM ${tableData}
        WHERE tname = '1b_stk_lmt'
    )
    JOIN (SELECT
        MEDIAN(tval) AS pnl_avg,
        STDEV(tval) AS pnl_eee
        FROM ${tableData}
        WHERE tname = '1f_stk_pnl'
    )
    JOIN (SELECT
        MEDIAN(tval) AS spy_avg,
        STDEV(tval) AS spy_eee
        FROM ${tableData}
        WHERE tname = '1a_spy'
    )
    --
    ) AS __join1
    WHERE
        tname IN ('1a_spy', '1f_stk_pnl');
UPDATE ${tableData}
    SET
        tt = UNIXEPOCH(tt),
        tval = ROUNDORZERO(100 * tval, 4);

-- chart - ${tableChart} - create
DROP TABLE IF EXISTS ${tableChart};
CREATE TABLE ${tableChart} (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '${JSON.stringify(optionDict)}' AS options;

INSERT INTO ${tableChart} (datatype, options, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT(
            'isHidden', tname NOT IN ('1a_spy', '1b_stk_lmt', '1c_stk_pct'),
            'seriesColor', (CASE
            WHEN (tname LIKE '%_lmb' OR tname LIKE '%_lms') THEN
                '#999'
            ELSE
                NULL
                -- (
                --     '#'
                --     || printf('%x', 12 - 2 * rownum)
                --     || printf('%x',  0 + 2 * rownum)
                --     || printf('%x', 16 - 2 * rownum)
                -- )
            END)
        ) AS options,
        rownum AS series_index,
        tname AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY tname) AS rownum,
            tname
        FROM (SELECT DISTINCT tname FROM ${tableData})
        WHERE
            tname IS NOT NULL
    );
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        tt AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY tt) AS rownum,
            tt
        FROM (SELECT DISTINCT tt FROM ${tableData})
    );
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx_label AS xx,
        tval AS yy
    FROM (
        SELECT
            series_index,
            series_label,
            xx,
            xx_label
        FROM (
            SELECT
                series_index,
                series_label
            FROM ${tableChart}
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM ${tableChart}
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN ${tableData} ON tname = series_label AND tt = xx_label;
DELETE FROM ${tableChart} WHERE datatype = 'xx_label';
UPDATE ${tableChart}
    SET
        series_label = (CASE
            WHEN (series_label = '1a_spy') THEN '1a spy change'
            WHEN (series_label = '1b_stk_lmt') THEN '1b stk holding ideal'
            WHEN (series_label = '1c_stk_pct') THEN '1c stk holding actual'
            WHEN (series_label = '1d_stk_lmb') THEN '1d stk holding bracket min'
            WHEN (series_label = '1e_stk_lms') THEN '1e stk holding bracket max'
            WHEN (series_label = '1f_stk_pnl') THEN '1f stk gain'
            WHEN (series_label = '2b_sqq_lmt') THEN '2b sqq holding ideal'
            WHEN (series_label = '2c_sqq_pct') THEN '2c sqq holding actual'
            WHEN (series_label = '2d_sqq_lmb') THEN '2d sqq holding bracket min'
            WHEN (series_label = '2e_sqq_lms') THEN '2e sqq holding bracket max'
            WHEN (series_label = '2f_sqq_pnl') THEN '2f sqq gain'
        END)
    WHERE
        datatype = 'series_label';
            `);
        })
    ].flat().flat().map(function (sql, ii) {
        return sql.trim().replace((
            /\{\{ii\}\}/g
        ), String(ii).padStart(2, "0"));
    }).join("\n\n\n\n") + "\n");
    await onDbExec({});
    return true;
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
    let modeDemo = true;
    await sqlmathWebworkerInit({});
    // init DB_XXX
    [
        DB_CHART, DB_QUERY, DB_MAIN
    ] = await Promise.all([{
        dbName: "chart",
        filename: "file:dbchart?mode=memory&cache=shared",
        isDbchart: true
    }, {
        dbName: "query",
        filename: "file:dbquery?mode=memory&cache=shared",
        isDbquery: true
    }, {
        dbName: "main",
        filename: ":memory:",
        isDbmain: true
    }].map(async function (db) {
        db = Object.assign(noop(
            await dbOpenAsync({
                filename: db.filename
            })
        ), db);
        // save db
        DB_DICT.set(db.dbName, db);
        return db;
    }));
    // attach db
    await Promise.all([
        DB_CHART, DB_QUERY
    ].map(async function (db) {
        await dbExecAsync({
            db: DB_MAIN,
            sql: `ATTACH DATABASE [${db.filename}] AS ${db.dbName};`
        });
    }));
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
        [UI_FILE_OPEN, "change", onDbAction],
        [document, "keydown", onKeyDown],
        [window, "hashchange", uitableInitWithinView],
        [window, "resize", onResize]
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
    // init event-handling - override window.onscroll
    window.onscroll = uitableInitWithinView;
    window.scroll({
        behavior: "smooth",
        top: 0
    });
    // init location.search
    await Promise.all(Array.from(
        location.search.slice(1).split("&")
    ).map(async function (elem) {
        let [
            key, val
        ] = elem.split("=");
        switch (key) {
        case "demo":
            switch (val) {
            case "demoDefault":
                modeDemo = undefined;
                await demoDefault();
                return;
            case "demoTradebot":
                modeDemo = undefined;
                await demoTradebot();
                return;
            }
            return;
        case "jsScript":
            modeDemo = undefined;
            key = document.createElement("script");
            key.src = val;
            if (val.endsWith(".mjs")) {
                key.type = "module";
            }
            document.head.appendChild(key);
            return;
        case "modeExpert":
            if (val === "1") {
                document.head.appendChild(domDivCreate(`
<style>
#contentPanel1 th {
    max-width: 48px;
}
</style>
                `).firstElementChild);
            }
            return;
        case "sqlDb":
            modeDemo = undefined;
            DB_INIT = new Promise(async function (resolve) {
                val = await fetch(val);
                val = await val.arrayBuffer();
                await dbFileImportAsync({
                    db: DB_MAIN,
                    dbData: val
                });
                resolve();
            });
            return;
        case "sqlScript":
            modeDemo = undefined;
            await DB_INIT;
            val = await fetch(val);
            val = await val.text();
            UI_EDITOR.setValue(val);
            return;
        }
    }));
    // init UI_ANIMATE_TIMER_INTERVAL
    setInterval(function () {
        UI_ANIMATE_DATENOW = Date.now();
        UI_ANIMATE_LIST = UI_ANIMATE_LIST.filter(function (
            svgAnimateStep
        ) {
            return !svgAnimateStep();
        });
    }, 16);
    if (!modeDemo) {
        await DB_INIT;
        await onDbExec({});
        return;
    }
    // init demo
    if (
        await demoTradebot()
    ) {
        return;
    }
    await demoDefault();
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

function jsonRowListFromCsv({
    csv
}) {
// this function will convert <csv>-text to json list-of-list
//
// https://tools.ietf.org/html/rfc4180#section-2
// Definition of the CSV Format
// While there are various specifications and implementations for the
// CSV format (for ex. [4], [5], [6] and [7]), there is no formal
// specification in existence, which allows for a wide variety of
// interpretations of CSV files.  This section documents the format that
// seems to be followed by most implementations:
//
// 1.  Each record is located on a separate line, delimited by a line
//     break (CRLF).  For example:
//     aaa,bbb,ccc CRLF
//     zzz,yyy,xxx CRLF
//
// 2.  The last record in the file may or may not have an ending line
//     break.  For example:
//     aaa,bbb,ccc CRLF
//     zzz,yyy,xxx
//
// 3.  There maybe an optional header line appearing as the first line
//     of the file with the same format as normal record lines.  This
//     header will contain names corresponding to the fields in the file
//     and should contain the same number of fields as the records in
//     the rest of the file (the presence or absence of the header line
//     should be indicated via the optional "header" parameter of this
//     MIME type).  For example:
//     field_name,field_name,field_name CRLF
//     aaa,bbb,ccc CRLF
//     zzz,yyy,xxx CRLF
//
// 4.  Within the header and each record, there may be one or more
//     fields, separated by commas.  Each line should contain the same
//     number of fields throughout the file.  Spaces are considered part
//     of a field and should not be ignored.  The last field in the
//     record must not be followed by a comma.  For example:
//     aaa,bbb,ccc
//
// 5.  Each field may or may not be enclosed in double quotes (however
//     some programs, such as Microsoft Excel, do not use double quotes
//     at all).  If fields are not enclosed with double quotes, then
//     double quotes may not appear inside the fields.  For example:
//     "aaa","bbb","ccc" CRLF
//     zzz,yyy,xxx
//
// 6.  Fields containing line breaks (CRLF), double quotes, and commas
//     should be enclosed in double-quotes.  For example:
//     "aaa","b CRLF
//     bb","ccc" CRLF
//     zzz,yyy,xxx
//
// 7.  If double-quotes are used to enclose fields, then a double-quote
//     appearing inside a field must be escaped by preceding it with
//     another double quote.  For example:
//     "aaa","b""bb","ccc"
    let match;
    let quote;
    let rgx;
    let row;
    let rowList;
    let val;
    // normalize "\r\n" to "\n"
    csv = csv.replace((
        /\r\n?/gu
    ), "\n");
    rgx = (
        /(.*?)(""|"|,|\n)/gu
    );
    rowList = [];
    // reset row
    row = [];
    val = "";
    while (true) {
        match = rgx.exec(csv);
        if (!match) {
// 2.  The last record in the file may or may not have an ending line
//     break.  For example:
//     aaa,bbb,ccc CRLF
//     zzz,yyy,xxx
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
// 5.  Each field may or may not be enclosed in double quotes (however
//     some programs, such as Microsoft Excel, do not use double quotes
//     at all).  If fields are not enclosed with double quotes, then
//     double quotes may not appear inside the fields.  For example:
//     "aaa","bbb","ccc" CRLF
//     zzz,yyy,xxx
            quote = !quote;
        } else if (quote) {
// 7.  If double-quotes are used to enclose fields, then a double-quote
//     appearing inside a field must be escaped by preceding it with
//     another double quote.  For example:
//     "aaa","b""bb","ccc"
            if (match[2] === "\"\"") {
                val += "\"";
// 6.  Fields containing line breaks (CRLF), double quotes, and commas
//     should be enclosed in double-quotes.  For example:
//     "aaa","b CRLF
//     bb","ccc" CRLF
//     zzz,yyy,xxx
            } else {
                val += match[2];
            }
        } else if (match[2] === ",") {
// 4.  Within the header and each record, there may be one or more
//     fields, separated by commas.  Each line should contain the same
//     number of fields throughout the file.  Spaces are considered part
//     of a field and should not be ignored.  The last field in the
//     record must not be followed by a comma.  For example:
//     aaa,bbb,ccc
            // delimit val
            row.push(val);
            val = "";
        } else if (match[2] === "\n") {
// 1.  Each record is located on a separate line, delimited by a line
//     break (CRLF).  For example:
//     aaa,bbb,ccc CRLF
//     zzz,yyy,xxx CRLF
            // delimit val
            row.push(val);
            val = "";
            // append row
            rowList.push(row);
            // reset row
            row = [];
        }
    }
    // append row
    if (row.length) {
        rowList.push(row);
    }
    return rowList;
}

function onContextmenu(evt) {
// this function will handle contextmenu-event
    let baton;
    let {
        clientX,
        clientY,
        ctrlKey,
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
    target = target.closest(`.tocElemA[data-dbtype], tr[data-dbtype="row"]`);
    // contextmenu - hide
    if (!target) {
        uiFadeOut(UI_CONTEXTMENU);
        return;
    }
    // init UI_CONTEXTMENU_BATON
    UI_CONTEXTMENU_BATON = DBTABLE_DICT.get(target.dataset.hashtag) || {};
    baton = UI_CONTEXTMENU_BATON;
    baton.rowid = target.dataset.rowid;
    // show / hide .contextmenuElem
    UI_CONTEXTMENU.querySelectorAll(
        ".contextmenuDivider, .contextmenuElem"
    ).forEach(function ({
        dataset,
        style
    }) {
        style.display = "none";
        if (dataset.dbtype !== target.dataset.dbtype) {
            return;
        }
        switch (dataset.action) {
        case "dbDetach":
            if (baton.isDbmain) {
                return;
            }
            break;
        case "dbrowDelete":
        case "dbrowUpdate":
            if (target.dataset.rowid === undefined) {
                return;
            }
            break;
        }
        style.display = "block";
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
    let baton = UI_CONTEXTMENU_BATON;
    let data;
    let target;
    let title;
    target = evt.target.closest("[data-action]") || evt.target;
    action = target.dataset.action;
    if (!action) {
        return;
    }
    // fast actions that do not require loading
    switch (target !== UI_FILE_OPEN && action) {
    case "dbAttach":
    case "dbOpen":
    case "dbscriptOpen":
    case "dbtableImportCsv":
    case "dbtableImportJson":
        UI_FILE_OPEN.dataset.action = action;
        UI_FILE_OPEN.value = "";
        UI_FILE_OPEN.click();
        return;
    case "dbcolumnAdd":
    case "dbcolumnDrop":
    case "dbcolumnRename":
    case "dbrowInsert":
    case "dbrowUpdate":
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
        UI_CRUD.querySelector("textarea").value = String(
            `

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

-- row - insert
INSERT INTO ${baton.dbtableFullname} (`
            + JSON.stringify(baton.colList.slice(1), undefined, 4).slice(1, -1)
            + `) VALUES (\n`
            + `${"    NULL,\n".repeat(baton.colList.length - 2)}    NULL`
            + `
);

-- row - update
UPDATE
    ${baton.dbtableFullname}
SET
`
            + baton.colList.slice(1).map(function (col) {
                return `    "${col}" = NULL`;
            }).join(",\n")
            + `
WHERE
    rowid = ${baton.rowid};

-- table - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME TO
    "{{new_table}}";
            `
        ).trim().split("\n\n").filter(function (sql) {
            return sql.indexOf(title) === 3;
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
        UI_CRUD.style.zIndex = 1;
        // ergonomy - auto-focus first input
        Array.from(UI_CRUD.querySelectorAll(
            ".modalContent input"
        )).every(function (elem) {
            elem.focus();
        });
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
    case "dbtableImportCsv":
    case "dbtableImportJson":
        if (target.files.length === 0) {
            return;
        }
        await dbTableImportAsync({
            db: DB_MAIN,
            mode: (
                action === "dbtableImportCsv"
                ? "csv"
                : "json"
            ),
            tableName: dbNameNext(
                "[__file{{ii}}]",
                new Set(DB_MAIN.dbtableList.keys())
            ).slice(1, -1),
            textData: (
                await target.files[0].text()
            )
        });
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
    case "dbExportMain":
        data = await dbFileExportAsync({
            db: DB_MAIN
        });
        data = data[6];
        fileSave({
            buf: data,
            filename: `sqlite_database_${DB_MAIN.dbName}.sqlite`
        });
        return;
    case "dbRefresh":
        await uiRenderDb();
        return;
    case "dbrowDelete":
        if (!window.confirm(
            `are you sure you want to delete row with rowid = ${baton.rowid}`
            + ` in table ${baton.dbtableFullname} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: baton.db,
            sql: (`
DELETE FROM ${baton.dbtableName} WHERE rowid = ${baton.rowid};
            `)
        });
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
// 2. save query-result
// 3. ui-render sql-queries to html
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
    FROM sqlite_master
    WHERE
        type = 'table';
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
    // 2. save query-result
    await Promise.all(dbqueryList.map(async function (rowList, ii) {
        let colList = rowList.shift().map(function (col, ii) {
            // bugfix - escape special-character in col
            return `value->>${ii} AS "${col.replace((/"/g), "\"\"")}"`;
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
    FROM JSON_EACH($tmp1);
END TRANSACTION;
            `)
        });
    }));
    // 3. ui-render sql-queries to html
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
    UI_CRUD.style.zIndex = -1;
}

function onKeyDown(evt) {
// this function will handle event keyup
    switch (evt.key) {
    case "Escape":
        // close error-modal
        uiFadeOut(document.querySelector("#errorPanel1"));
        // close contextmenu
        uiFadeOut(UI_CONTEXTMENU);
        return;
    }
    switch ((evt.ctrlKey || evt.metaKey) && evt.key) {
    // database - execute
    case "Enter":
        onDbExec({});
        return;
    // database - open
    case "o":
        evt.preventDefault();
        evt.stopPropagation();
        document.querySelector("button[data-action='dbOpen']").click();
        return;
    // database - save
    case "s":
        evt.preventDefault();
        evt.stopPropagation();
        document.querySelector("button[data-action='dbExportMain']").click();
        return;
    }
}

function onModalClose({
    currentTarget
}) {
// this function will close current modal
    uiFadeOut(currentTarget.closest(".modalPanel"));
}

function onResize() {
// this function will handle resize-event
    document.querySelectorAll(
        "#dbchartList1 .contentElem"
    ).forEach(function (elem) {
        elem.dataset.init = "0";
    });
    uitableInitWithinView({});
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

function svgAnimate(elem, attrDict, mode) {
// this function will animate <elem> with given <fxattr> in <attrDict>
    let {
        childNodes,
        fx_rotate,
        fx_seriesShape,
        nodeName
    } = elem;
    let datebeg = UI_ANIMATE_DATENOW;
    let fxstateDict = {};
    let {
        translateY
    } = attrDict;
    // optimization - skip animation if hidden
    if (attrDict.visibility === "hidden") {
        svgAttrSet(elem, attrDict);
        return;
    }
    // init fxstateDict
    Object.entries(attrDict).forEach(function ([
        fxattr, fxend
    ]) {
        let dpathList;
        let fxattr2 = "fx_" + fxattr;
        let fxbeg;
        let fxstate;
        // crispify height, width, x, y
        switch (fxattr) {
        case "height":
        case "width":
        case "x":
        case "y":
            fxend = Math.round(fxend);
            switch (fxattr) {
            case "x":
                fxend += CRISPX;
                break;
            case "y":
                fxend += CRISPY;
                break;
            }
            break;
        }
        fxstate = {
            fxend
        };
        switch (fxattr) {
        case "d":
            dpathList = fxend.split(" ");
            fxbeg = noop(elem.getAttribute(fxattr) ?? "").split(" ");
            if (!(fxbeg.length > 0 && fxbeg.length === dpathList.length)) {
                elem[fxattr2] = fxend;
                elem.setAttribute(fxattr, fxend);
                return;
            }
            fxstate.dpathList = dpathList;
            fxstate.fxbeg = fxbeg;
            fxstateDict[fxattr] = fxstate;
            break;
        case "height":
        case "translateX":
        case "translateY":
        case "width":
        case "x":
        case "y":
            fxstate.fxbeg = Number(
                elem[fxattr2] ?? elem.getAttribute(fxattr) ?? 0
            );
            if (fx_seriesShape) {
                fxstate.fxbeg = fxstate.fxbeg || fxend;
            }
            fxstateDict[fxattr] = fxstate;
            break;
        case "visibility":
            elem.setAttribute(fxattr, fxend);
            return;
        default:
            throw new Error(`svgAnimate - invalid attribute - ${fxattr}`);
        }
    });
    function svgAnimateStep() {
        let fxprg = 1;
        let isDone = datebeg + UI_ANIMATE_DURATION <= UI_ANIMATE_DATENOW;
        // animate - linear fxnow, fxprg
        if (!isDone) {
            fxprg = (
                UI_ANIMATE_DURATION_INV
                * (UI_ANIMATE_DATENOW - datebeg)
            );
            if (mode === "easeout") {
                fxprg = Math.sqrt(fxprg);
            }
        }
        Object.entries(fxstateDict).forEach(function ([
            fxattr, fxstate
        ]) {
            let {
                dpathList,
                fxbeg,
                fxend
            } = fxstate;
            let fxattr2 = "fx_" + fxattr;
            let fxnow = fxend;
            switch (fxattr) {
            // Perform the next step of the animation on "d"
            case "d":
                if (!dpathList) {
                    return;
                }
                // interpolate fxnow from dpathList, fxbeg
                if (!isDone) {
                    fxnow = fxbeg.map(function (char, ii) {
                        let num;
                        if ("CLMZ".indexOf(char) !== -1) {
                            return char;
                        }
                        num = Number(char);
                        return num + fxprg * (dpathList[ii] - num);
                    }).join(" ");
                }
                // cache attribute
                elem[fxattr2] = fxnow;
                elem.setAttribute("d", fxnow);
                return;
            case "height":
            case "translateX":
            case "translateY":
            case "width":
            case "x":
            case "y":
                if (!isDone) {
                    fxnow = fxbeg + fxprg * (fxend - fxbeg);
                }
                // cache attribute
                elem[fxattr2] = fxnow;
                if (fx_seriesShape || translateY) {
                    return;
                }
                // update child tspans x values
                if (fxattr === "x" && nodeName === "text") {
                    childNodes.forEach(function (child) {
                        // if the x values are equal, the tspan represents a
                        // linebreak
                        child.setAttribute("x", fxnow);
                    });
                    if (fx_rotate) {
                        elem.setAttribute(
                            "transform",
                            `rotate(-15 ${fxnow} ${elem.fx_y || 0})`
                        );
                    }
                }
                elem.setAttribute(fxattr, fxnow);
                return;
            }
        });
        if (fx_seriesShape) {
            elem.setAttribute("d", svgShapeDraw(
                fx_seriesShape,
                elem.fx_x,
                elem.fx_y,
                elem.fx_width,
                elem.fx_height
            ));
        }
        if (translateY) {
            elem.setAttribute(
                "transform",
                `translate(${elem.fx_translateX},${elem.fx_translateY})`
            );
        }
        return isDone;
    }
    // animate - stop existing animation for given elem
    svgAnimateStep.elem2 = elem;
    UI_ANIMATE_LIST = UI_ANIMATE_LIST.filter(function ({
        elem2
    }) {
        return elem2 !== elem;
    });
    // animate - svgAnimateStep()
    svgAnimateStep();
    // animate - setInterval()
    UI_ANIMATE_LIST.push(svgAnimateStep);
}

function svgAttrSet(elem, attrDict = {}) {
// this function will set-attribute items in <attrDict> to <elem>
    if (typeof elem === "string") {
        elem = document.createElementNS("http://www.w3.org/2000/svg", elem);
    }
    Object.entries(attrDict).forEach(function ([
        key, val
    ]) {
        if (val !== null && val !== undefined) {
            elem.setAttribute(key, val);
            switch (key) {
            case "clip-path":
            case "d":
            case "fill":
            case "stroke":
            case "stroke-linecap":
            case "stroke-linejoin":
            case "stroke-width":
            case "text-anchor":
            case "transform":
            case "visibility":
                elem.setAttribute(key, val);
                return;
            // crispify height, width, x, y
            case "height":
            case "width":
            case "x":
            case "y":
                val = Math.round(val);
                switch (key) {
                case "x":
                    val += CRISPX;
                    break;
                case "y":
                    val += CRISPY;
                    break;
                }
                elem.setAttribute(key, val);
                // cache attribute
                elem["fx_" + key] = val;
                return;
            default:
                throw new Error(`svgAttrSet - invalid attribute - ${key}`);
            }
        }
    });
    return elem;
}

function svgShapeDraw(seriesShape, x, y, w, h) {
// this function will create svg-dpath for given <seriesShape>
    let tmp;
    switch (seriesShape) {
    case "circle":
        tmp = 0.166 * w;
        return [
            "M", x + w / 2, y,
            "C", x + w + tmp, y, x + w + tmp, y + h, x + w / 2, y + h,
            "C", x - tmp, y + h, x - tmp, y, x + w / 2, y,
            "Z"
        ].join(" ");
    case "diamond":
        return [
            "M", x + w / 2, y,
            "L", x + w, y + h / 2,
            x + w / 2, y + h,
            x, y + h / 2,
            "Z"
        ].join(" ");
    case "square":
        x = Math.round(x + 0.0625 * w);
        y = Math.round(y + 0.0625 * h);
        w = Math.round(0.875 * w);
        h = Math.round(0.875 * h);
        return [
            "M", x, y,
            "L", x + w, y,
            x + w, y + h,
            x, y + h,
            "Z"
        ].join(" ");
    case "triangle":
        return [
            "M", x + w / 2, y,
            "L", x + w, y + h,
            x, y + h,
            "Z"
        ].join(" ");
    case "triangle-down":
        return [
            "M", x, y,
            "L", x + w, y,
            x + w / 2, y + h,
            "Z"
        ].join(" ");
    }
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
        return `${name}`;
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
    await Promise.all(Array.from(DB_DICT.values()).map(async function (db) {
        let baton;
        let {
            dbName,
            isDbchart,
            isDbmain,
            isDbquery
        } = db;
        let dbtableList;
        let tmp;
        db.dbtableList = new Map();
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
        dbtableList = new Map(dbtableList.map(function ({
            colList = [],
            dbtableFullname,
            rowCount = 0,
            rowList0,
            sql,
            tbl_name
        }, ii) {
            dbtableFullname = dbtableFullname || `${dbName}.[${tbl_name}]`;
            dbtableIi += 1;
            baton = {
                colList,
                db,
                dbName,
                dbtableFullname,
                dbtableIi,
                dbtableName: `[${tbl_name}]`,
                hashtag: `dbtable_${String(dbtableIi).padStart(2, "0")}`,
                ii,
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
            return [
                baton.dbtableName, baton
            ];
        }));
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
            : `database ${dbName}`
        ) + `</div>`;
        dbtableList.forEach(function ({
            colList,
            dbtableFullname,
            dbtableName,
            hashtag,
            ii,
            rowCount
        }) {
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
                    dbtableFullname,
                    rowCount,
                    colList //jslint-ignore-line
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
    window.scroll({
        behavior: "smooth",
        top: windowScrollY
    });
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
        await waitAsync(500);
        UI_LOADING_COUNTER -= 1;
        if (
            UI_LOADING_COUNTER === 0
            && UI_LOADING.style.visibility === "visible"
        ) {
            uiFadeOut(UI_LOADING);
        }
    }
}

async function uichartCreate(baton) {
// this function will create xy-line-chart from given sqlite table <baton>
    // init pre-var
    let {
        contentElem,
        db,
        dbtableName
    } = baton;
    let elemCanvasFlex;
    let elemLegend;
    let elemLegendWidth = 256;
    let elemUichart = contentElem.querySelector(".uichart");
    let elemUichartHeight = 384;
    let uichart = await dbExecAsync({
        db,
        sql: (`
-- table - __chart_options1 - insert
DROP TABLE IF EXISTS __chart_options1;
CREATE TEMP TABLE __chart_options1 AS
    SELECT
        JSON(options) AS options,
        CASTREALORZERO(options->>'$.xstep') AS xstep,
        CASTREALORZERO(1.0 / options->>'$.xstep') AS xstep_inv,
        CASTREALORZERO(options->>'$.ystep') AS ystep,
        CASTREALORZERO(1.0 / options->>'$.ystep') AS ystep_inv
    FROM (
        SELECT options FROM ${dbtableName} WHERE datatype = 'options' LIMIT 1
    );

-- table - __chart_series_xy1 - insert
DROP TABLE IF EXISTS __chart_series_xy1;
CREATE TEMP TABLE __chart_series_xy1 AS
    SELECT
        series_index,
        xx,
        LAG(xx, 1, NULL) OVER (
            PARTITION BY series_index ORDER BY xx
        ) AS xx_lag,
        LEAD(xx, 1, NULL) OVER (
            PARTITION BY series_index ORDER BY xx
        ) AS xx_lead,
        yy
    FROM (
        SELECT
            series_index,
            xx,
            yy
        FROM (
            SELECT
                ROW_NUMBER() OVER (
                    PARTITION BY series_index, xx ORDER BY rowid DESC
                ) AS rownum,
                series_index,
                xx,
                yy
            FROM (
                SELECT
                    ${dbtableName}.rowid,
                    --
                    series_index,
                    IIF(xstep_inv, ROUND(xstep_inv * xx), xx) AS xx,
                    IIF(ystep_inv, ROUND(ystep_inv * yy), yy) AS yy
                FROM ${dbtableName}
                JOIN __chart_options1
                WHERE
                    datatype = 'yy_value'
                    AND xx IS NOT NULL
            )
            ORDER BY
                series_index,
                xx
        )
        WHERE
            rownum = 1
    );

-- table - __chart_series_maxmin1 - insert
DROP TABLE IF EXISTS __chart_series_maxmin1;
CREATE TEMP TABLE __chart_series_maxmin1 AS
    SELECT
        series_index,
        MAX(xx) AS xdataMax,
        MIN(xx) AS xdataMin,
        MAX(yy) AS ydataMax,
        MIN(yy) AS ydataMin
    FROM __chart_series_xy1
    WHERE
        yy IS NOT NULL
    GROUP BY series_index;

-- table - __chart_options1 - select - options
SELECT
        JSON_SET(
            options,
            '$.seriesList', JSON(COALESCE(seriesList, '[]')),
            '$.xdataDxx', COALESCE(xdataDxx, 1),
            '$.xdataMax', xdataMax,
            '$.xdataMin', xdataMin,
            '$.xlabelList', JSON(COALESCE(xlabelList, '[]')),
            '$.ydataMax', ydataMax,
            '$.ydataMin', ydataMin
        ) AS options
    FROM __chart_options1
    JOIN (
        SELECT
            JSON_GROUP_ARRAY(xx_label) AS xlabelList
        FROM ${dbtableName}
        WHERE
            datatype = 'xx_label'
    )
    JOIN (
        SELECT
            JSON_GROUP_ARRAY(JSON(JSON_SET(
                COALESCE(options, '{}'),
                '$.seriesName', series_label,
                '$.xdata', JSON(COALESCE(xdata, '[]')),
                '$.ydata', JSON(COALESCE(ydata, '[]'))
            ))) AS seriesList
        FROM (
            SELECT
                options,
                series_index,
                (
                    ROW_NUMBER() OVER (ORDER BY series_index)
                    || '. '
                    || series_label
                ) AS series_label
            FROM ${dbtableName}
            WHERE
                datatype = 'series_label'
            ORDER BY
                series_index
        )
        -- calculate series.xdata, series.ydata
        LEFT JOIN (
            SELECT
                series_index,
                JSON_GROUP_ARRAY(xx) AS xdata,
                JSON_GROUP_ARRAY(yy) AS ydata
            FROM __chart_series_xy1
            GROUP BY series_index
        ) USING (series_index)
    )
    JOIN (
        SELECT
            MAX(xdataMax) AS xdataMax,
            MIN(xdataMin) AS xdataMin,
            MAX(ydataMax) AS ydataMax,
            MIN(ydataMin) AS ydataMin
        FROM __chart_series_maxmin1
    )
    -- calculate uichart.xdataDxx
    JOIN (
        SELECT
            MIN(ABS(xx - xx_prv)) AS xdataDxx
        FROM (
            SELECT
                xx,
                LAG(xx, 1, NULL) OVER (ORDER BY xx) AS xx_prv
            FROM (
                SELECT DISTINCT
                    xx
                FROM __chart_series_xy1
            )
        )
    );
        `)
    });
    uichart = JSON.parse(uichart[0][0].options);
    contentElem.querySelector(".uitable").style.display = "none";
    elemUichart.style.display = "flex";
    elemUichart.style.height = `${elemUichartHeight}px`;
    elemUichart.innerHTML = (`
<div
    class="uichartNav"
    style="height: ${elemUichartHeight}px; width: ${elemLegendWidth}px;"
>
    <button
        class="uichartAction"
        data-action="uichartZoomReset"
    >reset zoom</button>
    <button
        class="uichartAction"
        data-action="uichartSeriesHideAll"
    >hide all</button>
    <button
        class="uichartAction"
        data-action="uichartSeriesShowAll"
    >show all</button>
    <div
        class="uichartLegend"
        style="height: ${elemUichartHeight - 64}px;"
    ></div>
</div>
<div style="position: relative; margin-left: 16px; width: 16px;">
    <div class="uichartAxislabel1">${stringHtmlSafe(uichart.yaxisTitle)}</div>
</div>
<div style="display: flex; flex: 1; flex-direction: column; padding: 5px 0;">
    <div class="uichartTitle">
    ${
        stringHtmlSafe(uichart.title).replace((
            /\n/g
        ), "<br>")
    }
    </div>
    <div class="uichartCanvasFlex" style="flex: 1;">
    <svg
        class="uichartCanvasFixed"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg"
    >
        <clipPath class="uichartClip" id="uichartClip${baton.dbtableIi}">
            <rect fill="none" height="0" width="0" x="0" y="0"></rect>
        </clipPath>
        <g class="uichartGridlineList"></g>
        <g class="uichartAxistickList"></g>
        <g class="uichartSeriesList"></g>
        <g class="uichartCrosshairList">
            <path stroke-width="1" stroke="#333" visibility="hidden"></path>
            <path stroke-width="1" stroke="#333" visibility="hidden"></path>
        </g>
        <g class="uichartTooltip" visibility="hidden">
            <rect
                class="uichartTooltipBorder"
                fill-opacity="0.8000"
                fill="#fff"
                rx="5"
                ry="5"
                stroke-width="3"
                x="0"
                y="0"
            >
            </rect>
            <text class="uichartTooltipText"></text>
        </g>
        <g class="uichartMousetrackerList">/g>
    </svg>
    </div>
    <div class="uichartAxislabel0">${stringHtmlSafe(uichart.xaxisTitle)}</div>
</div>
    `);
    elemCanvasFlex = elemUichart.querySelector(".uichartCanvasFlex");
    elemLegend = elemUichart.querySelector(".uichartLegend");
    // init var
    let ELEM_GRAPH_LINE_WIDTH = 1; //jslint-ignore-line
    let ELEM_POINT_BORDER_WIDTH = 0;
    let ELEM_POINT_RADIUS = 4;
    let canvasHeight = elemCanvasFlex.clientHeight;
    let canvasWidth = elemCanvasFlex.clientWidth;
    let counterColor = 1;
    let counterShape = 1;
    let elemCanvasFixed = elemCanvasFlex.firstElementChild;
    let [
        elemClip,
        elemGridlineList,
        elemAxistickList,
        elemSeriesList,
        elemCrosshairList,
        elemTooltip,
        elemMousetrackerList
    ] = elemCanvasFixed.children;
    let [
        elemTooltipBorder,
        elemTooltipText
    ] = elemTooltip.children;
    let {
        isBarchart,
        seriesList,
        xdataDxx,
        xdataMax,
        xdataMin,
        xlabelList
    } = uichart;
    let isResizing = 0;
    let plotBottom;
    let plotHeight;
    let plotLeft;
    let plotRight;
    let plotTop;
    let plotWidth;
    let plotbarMargin;
    let plotbarYaxis;
    let pointHovered;
    let pointListBarchart = [];
    let redrawTimer;
    let seriesColorList = [
        /*
        // highcharts-v4
        "#7cb5ec", // light-blue - highcharts-v4
        "#434348", // black - highcharts-v4
        "#90ed7d", // light-green - highcharts-v4
        "#f7a35c", // orange - highcharts-v4
        "#8085e9", // lavender-blue - highcharts-v4
        "#f15c80", // pink - highcharts-v4
        "#e4d354", // yellow - highcharts-v4
        "#2b908f", // teal - highcharts-v4
        "#f45b5b", // orange-red - highcharts-v4
        "#91e8e1" // aqua - highcharts-v4
        // highcharts-v3
        "#2f7ed8", // blue - highcharts-v3
        "#0d233a", // dark-teal - highcharts-v3
        "#8bbc21", // olive - highcharts-v3
        "#910000", // maroon - highcharts-v3
        "#1aadce", // aqua - highcharts-v3
        "#492970", // purple - highcharts-v3
        "#f28f43", // orange - highcharts-v3
        "#77a1e5", // light-blue - highcharts-v3
        "#c42525", // red - highcharts-v3
        "#a6c96a" // light-olive - highcharts-v3
        // highcharts-v2
        "#4572a7", // blue - highcharts-v2
        "#aa4643", // red - highcharts-v2
        "#89a54e", // olive - highcharts-v2
        "#80699b", // purple - highcharts-v2
        "#3d96ae", // aqua - highcharts-v2
        "#db843d", // orange - highcharts-v2
        "#92a8cd", // light-navy - highcharts-v2
        "#a47d7c", // brown - highcharts-v2
        "#b5ca92" // light-olive - highcharts-v2
        */
        //
        // "#aa4643", // red - highcharts-v2
        "#a44", // red
        // "#4572a7", // blue - highcharts-v2
        "#57b", // blue
        "#7ecf6d", // light-green - highcharts-v4
        // "#80699b", // purple - highcharts-v2
        "#86a", // purple
        //
        "#3d96ae", // aqua - highcharts-v2
        "#f15c80", // pink - highcharts-v4
        "#8085e9", // lavender-blue - highcharts-v4
        // "#a47d7c", // brown - highcharts-v2
        // "#b88", // brown
        "#89a54e", // olive - highcharts-v2
        // "#0d233a", // dark-teal - highcharts-v3
        "#357", // dark-teal
        "#492970" // purple - highcharts-v3
    ];
    let seriesHovered;
    let seriesShapeList = [
        // "circle", "diamond", "square", "triangle", "triangle-down"
        // "circle",
        "square",
        "triangle",
        "diamond",
        "triangle-down"
    ];
    let xaxisMax;
    let xaxisMin;
    let xaxistickDict = {};
    let xpixelToPointDictHovered = [];
    let xtransOffset;
    let xtransWidth;
    let xzoomMax;
    let xzoomMin;
    let yaxisMax;
    let yaxisMin;
    let yaxistickDict = {};
    let ytransWidth;
    function axistickCreate(isXaxis, tickx) {
    // this function will create <elemGridline>, <elemTick>, <elemTicklabel>
    // at given <tickx> in <axis>
        let elemTick;
        let elemTicklabel;
        let elemTspan;
        // create elemTick
        elemTick = svgAttrSet("path", {
            fill: "none",
            stroke: "#33b",
            "stroke-width": 3
        });
        elemTick.isFirstDraw = true;
        elemTick.isXaxis = isXaxis;
        // if isXaxis, then draw tick
        if (isXaxis) {
            elemAxistickList.appendChild(elemTick);
        // create elemGridline
        } else {
            elemTick.elemGridline = svgAttrSet("path", {
                fill: "none",
                stroke: "#999",
                "stroke-width": 1
            });
            elemGridlineList.appendChild(elemTick.elemGridline);
        }
        // skip ticklabel, if elemTick is not contained in xlabelList
        if (
            isXaxis
            && xlabelList.length
            && !xlabelList.hasOwnProperty(tickx - 1)
        ) {
            return elemTick;
        }
        // create ticklabel
        elemTspan = svgAttrSet("tspan");
        elemTspan.textContent = (function () {
            let num;
            let numDigitList;
            let xOrY = (
                isXaxis
                ? "x"
                : "y"
            );
            num = numberFormat({
                convert: uichart[xOrY + "valueConvert"],
                modeTick: true,
                num: (
                    isXaxis
                    ? xlabelList[tickx - 1] ?? tickx
                    : tickx
                ),
                prefix: uichart[xOrY + "valuePrefix"],
                step: uichart[xOrY + "step"],
                suffix: uichart[xOrY + "valueSuffix"]
            }).slice(0, 16);
            // number already formatted
            if (typeof num !== "number") {
                return num;
            }
            // small number
            if (Math.abs(num) < 1000) {
                return num.toLocaleString();
            }
            // large number
            num = Math.round(num).toLocaleString();
            numDigitList = num.split(
                /[^+\-0-9]/
            );
            switch (numDigitList.length) {
            // kilo
            case 2:
                return numDigitList[0] + "k";
            // mega
            case 3:
                return numDigitList[0] + "M";
            // giga
            case 4:
                return numDigitList[0] + "G";
            // tera
            case 5:
                return numDigitList[0] + "T";
            // peta
            case 6:
                return numDigitList[0] + "P";
            // exa
            case 7:
                return numDigitList[0] + "E";
            default:
                return num;
            }
        }());
        elemTicklabel = svgAttrSet("text", {
            "text-anchor": (
                isXaxis
                ? "middle"
                : "end"
            )
        });
        elemTick.elemTicklabel = elemTicklabel;
        elemTicklabel.fx_rotate = isXaxis;
        elemTicklabel.appendChild(elemTspan);
        elemAxistickList.appendChild(elemTicklabel);
        return elemTick;
    }
    function floatCorrect(num) {
    // this function will correct float-error in <num>
        return parseFloat(
            num.toPrecision(12)
        );
    }
    function numberFormat({
        convert,
        modeTick,
        num,
        prefix,
        step,
        suffix
    }) {
    // this function will format <num>
        if (step && typeof num === "number") {
            num *= step;
        }
        switch (convert) {
        case "juliandayToDate":
            num = new Date((num - 2440587.5) * 86400 * 1000);
            // num = num.toUTCString().slice(0, 16);
            num = (
                modeTick
                ? num.toUTCString().slice(5, 16)
                : num.toUTCString().slice(0, 16)
            );
            break;
        case "unixepochToTimelocal":
            num = new Date(num * 1000);
            num = (
                modeTick
                ? num.toLocaleTimeString()
                : num.toLocaleString()
            );
            break;
        case "unixepochToTimeutc":
            // num = new Date(num * 1000).toISOString().slice(11, 19) + "Z";
            num = new Date(num * 1000);
            num = (
                modeTick
                ? num.toUTCString().slice(17)
                : num.toUTCString()
            );
            break;
        }
        if (prefix) {
            num = prefix + num;
        }
        if (suffix) {
            num += suffix;
        }
        return String(num);
    }
    function onCanvasZoom(evt) {
    // this function will zoom/un-zoom at current mouse-location on canvas-area
        let xmid = 0.5;
        let xscale = 1.2500; // zoom-out
        evt.preventDefault();
        evt.stopPropagation();
        if (!evt.modeDebounce) {
            debounce("onCanvasZoom", onCanvasZoom, Object.assign(evt, {
                modeDebounce: true
            }));
            return;
        }
        if (evt.deltaY < 0) {
            xmid = (
                evt.pageX
                - elemCanvasFixed.getBoundingClientRect().left
                - window.scrollX
                + document.documentElement.clientLeft
                - plotLeft
            ) / plotWidth;
            xscale = 0.8000; // zoom-in
        }
        xmid += 0.5 * (xmid - 0.4000);
        xmid = xaxisMin + xmid * (xaxisMax - xaxisMin);
        xzoomMax = xmid + xscale * (xaxisMax - xmid);
        xzoomMin = xmid + xscale * (xaxisMin - xmid);
        // uichartRedraw - uichartZoom
        uichartRedraw();
    }
    function onPointHover(evt) {
    // this function will handle <evt> when mouse hover over point
        let mouseX;
        let pointObj;
        let rect;
        let tooltipBbox;
        let tooltipHeight;
        let tooltipMargin = 32;
        let tooltipWidth;
        let tooltipX;
        let tooltipY;
        // get mouse-position
        rect = elemCanvasFixed.getBoundingClientRect();
        mouseX = Math.round(evt.pageX - rect.left - window.scrollX - plotLeft);
        // if mouse is outside canvas-area, then return
        if (!(0 <= mouseX && mouseX <= plotWidth)) {
            return;
        }
        // get closest-series-point to mouse as pointHovered
        pointObj = xpixelToPointDictHovered[mouseX];
        if (
            !pointObj
            || pointObj === pointHovered
            || pointObj.series.isHidden
        ) {
            return;
        }
        pointHovered = pointObj;
        // redraw seriesHovered
        onSeriesHover(pointObj.series, true);
        // redraw tooltip around pointHovered
        let { //jslint-ignore-line
            pointX,
            pointY,
            xval,
            yval
        } = pointHovered;
        let xlabel;
        let ylabel;
        if (redrawTimer || pointY === undefined) {
            return;
        }
        xlabel = numberFormat({
            convert: uichart.xvalueConvert,
            num: xlabelList[xval - 1] ?? xval,
            prefix: uichart.xvaluePrefix,
            step: uichart.xstep,
            suffix: uichart.xvalueSuffix
        });
        ylabel = numberFormat({
            convert: uichart.yvalueConvert,
            num: yval,
            prefix: uichart.yvaluePrefix,
            step: uichart.ystep,
            suffix: uichart.yvalueSuffix
        });
        // update elemTooltipText
        elemTooltip.setAttribute("visibility", "visible");
        elemTooltipText.innerHTML = (`
<tspan dy="17" x="6">${stringHtmlSafe(seriesHovered.seriesName)}</tspan>
<tspan dy="17" x="6">x: ${stringHtmlSafe(xlabel)}</tspan>
<tspan
    dy="19"
    style="font-size: 14px; font-weight: bold;"
    x="6"
>y: ${stringHtmlSafe(ylabel)}</tspan>
        `);
        // update elemTooltipBorder after text-update
        tooltipBbox = elemTooltipText.getBBox();
        tooltipWidth = tooltipBbox.width + 10;
        tooltipHeight = tooltipBbox.height + 10;
        svgAttrSet(elemTooltipBorder, {
            height: tooltipHeight,
            stroke: seriesHovered.seriesColor,
            width: tooltipWidth
        });
        // calculate tooltipX
        tooltipX = pointX + plotLeft - 0.5 * tooltipWidth;
        tooltipX = Math.max(tooltipX, plotLeft + tooltipMargin);
        tooltipX = Math.min(
            tooltipX,
            canvasWidth - tooltipWidth - tooltipMargin
        );
        // calculate tooltipY
        tooltipY = pointY + plotTop - tooltipHeight - tooltipMargin;
        if (tooltipY < plotTop) {
            tooltipY = pointY + plotTop + tooltipMargin;
        }
        // animate-move tooltip to tooltipX, tooltipY
        svgAnimate(elemTooltip, {
            translateX: tooltipX,
            translateY: tooltipY
        }, "easeout");
        // animate-move crosshair
        [
            true, false
        ].forEach(function (isXaxis, ii) {
            let d;
            if (isXaxis) {
                d = xaxisTranslate(xval) + CRISPX;
                d = `M ${d} 0 L ${d} ${canvasHeight}`;
            } else {
                d = yaxisTranslate(yval) + CRISPY;
                d = `M 0 ${d} L ${canvasWidth} ${d}`;
            }
            svgAnimate(elemCrosshairList.children[ii], {
                d,
                visibility: "visible"
            }, "easeout");
        });
    }
    function onPointUnhover() {
    // this function will handle <evt> when mouse un-hover from point
        onSeriesUnhover();
        pointHovered = undefined;
        // hide elemcrosshairList, elemTooltip
        elemCrosshairList.children[0].setAttribute("visibility", "hidden");
        elemCrosshairList.children[1].setAttribute("visibility", "hidden");
        elemTooltip.setAttribute("visibility", "hidden");
    }
    function onSeriesHover(evt, scrollTo) {
    // this function will handle <evt> when mouse hover over series
        let series = (
            evt.target
            ? seriesList[
                evt.target.closest("[data-series-ii]")?.dataset.seriesIi
            ]
            : evt
        );
        let seriesColor;
        if (!series || series.isHidden) {
            onSeriesUnhover();
            return;
        }
        // un-hover previous seriesHovered
        if (series !== seriesHovered) {
            onSeriesUnhover();
        }
        seriesHovered = series;
        // darken series-color
        if (isBarchart) {
            seriesColor = series.seriesColor.replace((
                /^#(..)(..)(..)$/
            ), function (ignore, rr, gg, bb) {
                return (
                    "rgb("
                    + Math.round(0.5 * parseInt(rr, 16)) + ","
                    + Math.round(0.5 * parseInt(gg, 16)) + ","
                    + Math.round(0.5 * parseInt(bb, 16))
                    + ")"
                );
            });
            seriesHovered.pointListSeries.forEach(function (pointObj) {
                pointObj.elemPoint.setAttribute("fill", seriesColor);
            });
        }
        // thicken series-line-width
        if (!isBarchart) {
            xpixelToPointDictHovered = series.xpixelToPointDict;
            svgAttrSet(seriesHovered.elemGraph, {
                "stroke-width": ELEM_GRAPH_LINE_WIDTH + 2
            });
        }
        Array.from(elemLegend.children).forEach(function (elem, seriesIi) {
            if (seriesIi !== seriesHovered.seriesIi) {
                elem.style.background = "none";
                return;
            }
            // hover series in legend
            elem.style.background = "#ccc";
            // scroll to series in legend
            if (scrollTo) {
                debounce("onSeriesScroll", function () {
                    elemLegend.scroll({
                        behavior: "smooth",
                        top: elem.offsetTop - elemLegend.offsetHeight
                    });
                });
            }
        });
    }
    function onSeriesHoveredHide() {
    // this function will handle <evt> to hide hovered-series
        if (seriesHovered) {
            uichartSeriesHideOrShow(seriesHovered, true);
            uichartRedraw();
            return;
        }
    }
    function onSeriesUnhover() {
    // this function will handle <evt> when mouse un-hover from series
        let seriesColor;
        if (!seriesHovered) {
            return;
        }
        // un-darken series-color
        if (isBarchart) {
            seriesColor = seriesHovered.seriesColor;
            seriesHovered.pointListSeries.forEach(function (pointObj) {
                pointObj.elemPoint.setAttribute("fill", seriesColor);
            });
        }
        // un-thicken series-line-width
        if (!isBarchart) {
            svgAttrSet(seriesHovered.elemGraph, {
                "stroke-width": ELEM_GRAPH_LINE_WIDTH
            });
        }
        // un-hover series in legend
        elemLegend.children[seriesHovered.seriesIi].style.background = "none";
        seriesHovered = undefined;
    }
    async function onUichartAction(evt) {
    // this function will handle uichart event <evt>
        let action;
        let modeHide;
        let target;
        evt.preventDefault();
        evt.stopPropagation();
        if (!evt.modeDebounce) {
            debounce("onUichartAction", onUichartAction, Object.assign(evt, {
                modeDebounce: true
            }));
            return;
        }
        target = evt.target.closest("[data-action]");
        if (!target) {
            return;
        }
        action = target.dataset.action;
        switch (action) {
        case "uichartSeriesHideAll":
        case "uichartSeriesShowAll":
            uiFadeIn(baton.elemLoading);
            await waitAsync(50);
            modeHide = action === "uichartSeriesHideAll";
            // hide or show series
            seriesList.forEach(function (series) {
                uichartSeriesHideOrShow(series, modeHide);
            });
            uichartRedraw();
            await waitAsync(200);
            uiFadeOut(baton.elemLoading);
            return;
        case "uichartSeriesHideOrShow":
            modeHide = target.dataset.hidden !== "1";
            // hide or show series
            uichartSeriesHideOrShow(
                seriesList[target.dataset.seriesIi],
                modeHide
            );
            uichartRedraw();
            return;
        case "uichartZoomReset":
            uiFadeIn(baton.elemLoading);
            await waitAsync(50);
            xzoomMax = undefined;
            xzoomMin = undefined;
            // uichartRedraw - uichartZoomReset
            uichartRedraw();
            await waitAsync(200);
            uiFadeOut(baton.elemLoading);
            return;
        }
    }
    // uichartRedraw - start
    function uichartRedraw(modeDebounce) {
    // this function will redraw <uichart>
        if (!modeDebounce) {
            clearTimeout(redrawTimer);
            redrawTimer = setTimeout(uichartRedraw, 0, true);
            return;
        }
        redrawTimer = undefined;
        //
        // calculate plotBottom, plotHeight, plotTop
        //
        plotTop = 16;
        plotHeight = canvasHeight - plotTop - 32;
        plotBottom = canvasHeight - plotHeight - plotTop;
        //
        // pre-calculate plotLeft, plotRight, plotWidth
        //
        plotLeft = 32;
        plotWidth = canvasWidth - plotLeft - 32;
        plotRight = canvasWidth - plotWidth - plotLeft;
        //
        // calculate axisMax, axisMin - start
        //
        [
            true, false
        ].forEach(function (isXaxis) {
            let axisMax;
            let axisMin;
            let tickExponent;
            let tickInterval;
            let tickMultiple;
            let tickx;
            let tickxDict;
            let tickxList;
            let tickxMax;
            let tickxMin;
            let tickxPrv;
            if (isResizing) {
                return;
            }
            //
            // if isXaxis, then calculate xaxismax, xaxismin
            //
            if (isXaxis) {
                axisMax = Math.min(xdataMax, xzoomMax ?? xdataMax);
                axisMin = Math.max(xdataMin, xzoomMin ?? xdataMin);
                // ensure xaxisMax - xaxisMin >= xdataDxx
                if (axisMax - axisMin < 4 * xdataDxx) {
                    axisMax = Math.min(xdataMax, axisMin + 4 * xdataDxx);
                    axisMin = Math.max(xdataMin, axisMax - 4 * xdataDxx);
                }
                // pad axisMax, axisMin
                axisMax += 0.02 * (axisMax - axisMin);
                axisMin -= 0.02 * (axisMax - axisMin);
            }
            //
            // if not isXaxis, then calculate yaxisMax, yaxisMin
            //
            // if isBarchart, then make sure yaxis is visible
            if (!isXaxis && isBarchart) {
                axisMax = 0;
                axisMin = 0;
            }
            seriesList.forEach(function (series) {
                let ii;
                let nn;
                let xcropEnd;
                let xcropStart;
                let {
                    xdata,
                    ydata
                } = series;
                let yval;
                if (isXaxis || series.isHidden) {
                    return;
                }
                nn = xdata.length;
                // calculate xcropEnd, xcropStart
                // xdata inside xcrop-range
                if (xaxisMin <= xdata[0] && xdata[nn - 1] <= xaxisMax) {
                    xcropEnd = nn;
                    xcropStart = 0;
                // xdata outside xcrop-range
                } else if (xdata[nn - 1] < xaxisMin || xaxisMax < xdata[0]) {
                    xcropEnd = 0;
                    xcropStart = 0;
                // init xcropStart
                } else {
                    ii = 0;
                    while (ii < nn && xdata[ii] < xaxisMin) {
                        ii += 1;
                    }
                    xcropStart = Math.max(0, ii - 1);
                    // init xcropEnd
                    while (ii < nn && xdata[ii] < xaxisMax) {
                        ii += 1;
                    }
                    xcropEnd = ii;
                }
                // calculate yaxisMax, yaxisMin
                ii = xcropStart;
                while (ii < xcropEnd) {
                    yval = ydata[ii];
                    if (!Number.isNaN(yval)) {
                        axisMax = Math.max(axisMax ?? yval, yval);
                        axisMin = Math.min(axisMin ?? yval, yval);
                    }
                    ii += 1;
                }
            });
            axisMax = axisMax ?? 0;
            axisMin = axisMin ?? 0;
            axisMax += 0.01 * (axisMax - axisMin);
            axisMin -= 0.01 * (axisMax - axisMin);
            if (axisMax === axisMin) {
                axisMax = Math.max(0.99 * axisMax, 1.01 * axisMax);
                axisMin = Math.min(0.99 * axisMin, 1.01 * axisMin);
            }
            if (axisMax === axisMin) {
                axisMax += 1;
                axisMin -= 1;
            }
            //
            // add/remove elemGridline, elemTick, elemTicklabel
            //
            // calculate tickInterval
            tickInterval = (
                isXaxis
                ? (axisMax - axisMin) * 100 / plotWidth
                : (axisMax - axisMin) * 72 / plotHeight
            ) || 1;
            // normalize tickInterval to within 0...10
            tickExponent = Math.pow(
                10,
                Math.floor(Math.log(tickInterval) / Math.LN10)
            );
            tickInterval = tickInterval / tickExponent;
            // round tickInterval to 1, 2, 5, or 10
            Array.from([
                1, 2, 5, 10
            ]).some(function (multiple, ii, list) {
                tickMultiple = multiple;
                return (
                    2 * tickInterval
                    <=
                    tickMultiple + (list[ii + 1] || tickMultiple)
                );
            });
            // after rounding, un-normalize tickInterval from within 0...10
            tickInterval = tickMultiple * tickExponent;
            // calculate tickxList from tickInterval
            tickx = floatCorrect(
                Math.floor(axisMin / tickInterval) * tickInterval
            );
            tickxList = [];
            tickxMax = floatCorrect(
                Math.ceil(axisMax / tickInterval) * tickInterval
            );
            while (true) {
                tickxList.push(tickx);
                tickx = floatCorrect(tickx + tickInterval);
                if (tickx > tickxMax || tickx === tickxPrv) {
                    break;
                }
                tickxPrv = tickx;
            }
            // sync tickxMax, tickxMin with axisMax, axisMin
            tickxMax = tickxList[tickxList.length - 1];
            tickxMin = tickxList[0];
            if (isXaxis) {
                if (tickxMax > axisMax + 0.5 * xdataDxx) {
                    tickxList.pop();
                }
                if (tickxMin < axisMin - 0.5 * xdataDxx) {
                    tickxList.shift();
                }
            } else {
                axisMax = tickxMax;
                axisMin = tickxMin;
            }
            // add elemGridline, elemTick, elemTicklabel
            tickxDict = (
                isXaxis
                ? xaxistickDict
                : yaxistickDict
            );
            tickxList.forEach(function (tickx) {
                tickxDict[tickx] = tickxDict[tickx] || axistickCreate(
                    isXaxis,
                    tickx
                );
            });
            // remove elemGridline, elemTick, elemTicklabel
            Object.entries(tickxDict).forEach(function ([
                tickx, elemTick
            ]) {
                if (
                    elemTick.isXaxis !== isXaxis
                    || tickxList.indexOf(Number(tickx)) !== -1
                ) {
                    return;
                }
                elemTick.remove();
                if (elemTick.elemGridline) {
                    elemTick.elemGridline.remove();
                    delete elemTick.elemGridline;
                }
                if (elemTick.elemTicklabel) {
                    elemTick.elemTicklabel.remove();
                    delete elemTick.elemTicklabel;
                }
                delete tickxDict[tickx];
            });
            //
            // re-calculate plotLeft, plotRight, plotWidth
            //
            if (!isXaxis) {
                plotLeft = 0;
                tickxList.forEach(function (tickx) {
                    plotLeft = Math.max(
                        plotLeft,
                        tickxDict[tickx].elemTicklabel.getBBox().width
                    );
                });
                plotLeft = Math.round(16 + plotLeft);
                plotWidth = Math.round(canvasWidth - plotLeft - 32);
                plotRight = Math.round(canvasWidth - plotWidth - plotLeft);
            }
            // save axisMax, axisMin
            if (isXaxis) {
                xaxisMax = axisMax;
                xaxisMin = axisMin;
            } else {
                yaxisMax = axisMax;
                yaxisMin = axisMin;
            }
        });
        //
        // calculate axisMax, axisMin - end
        //
        //
        // calculate xtransOffset, xtransWidth, ytransWidth
        //
        xtransWidth = plotWidth / (xaxisMax - xaxisMin + xdataDxx);
        xtransOffset = xtransWidth * 0.5 * xdataDxx;
        ytransWidth = plotHeight / (yaxisMax - yaxisMin);
        //
        // calculate plotbarMargin, plotbarWidth, plotbarYaxis
        plotbarMargin = Math.max(2, 0.24 * xtransWidth * xdataDxx);
        plotbarYaxis = yaxisTranslate(0);
        //
        // update crop-area for
        // elemCrosshairList, elemMousetrackerList, elemSeriesList
        //
        svgAttrSet(elemClip.firstElementChild, {
            height: plotHeight,
            width: plotWidth
        });
        [
            elemCrosshairList, elemMousetrackerList, elemSeriesList
        ].forEach(function (elem) {
            svgAttrSet(elem, {
                "clip-path": `url(#${elemClip.id})`,
                transform: `translate(${plotLeft},${plotTop})`
            });
        });
        //
        // redraw elemGridline, elemTick, elemTicklabel - start
        //
        [
            Object.entries(xaxistickDict),
            Object.entries(yaxistickDict)
        ].flat().forEach(function ([
            tickx, elemTick
        ]) {
            let {
                elemGridline,
                elemTicklabel,
                isXaxis
            } = elemTick;
            let xx;
            let yy;
            // init xx, yy for first-draw
            if (elemTick.isFirstDraw) {
                xx = plotLeft + 0.5 * plotWidth;
                yy = canvasHeight - plotBottom - 0.5 * plotHeight;
                if (isXaxis) {
                    yy = canvasHeight + 0.125 * plotHeight;
                } else {
                    xx = -0.125 * plotWidth;
                }
                delete elemTick.isFirstDraw;
                svgAttrSet(elemTick, {
                    d: `M ${xx} ${yy} L ${xx} ${yy + 5}`
                });
                if (elemGridline) {
                    svgAttrSet(elemGridline, {
                        d: (
                            `M ${plotLeft} ${yy}`
                            + ` L ${canvasWidth - plotRight} ${yy}`
                        )
                    });
                }
                if (elemTicklabel) {
                    svgAttrSet(elemTicklabel, {
                        x: xx,
                        y: yy
                    });
                }
            }
            // init xx, yy for animation
            if (isXaxis) {
                xx = plotLeft + xaxisTranslate(tickx);
                yy = canvasHeight - plotBottom;
            } else {
                xx = plotLeft;
                yy = plotTop + yaxisTranslate(tickx);
            }
            // redraw elemTick
            if (isXaxis) {
                svgAnimate(elemTick, {
                    d: `M ${xx - 0.5} ${yy + 0.5} L ${xx - 0.5} ${yy + 5.5}`
                });
            }
            // redraw elemTicklabel
            if (elemTicklabel) {
                svgAnimate(elemTicklabel, (
                    isXaxis
                    ? {
                        x: xx,
                        y: yy + 18
                    }
                    : {
                        x: xx - 8,
                        y: yy + 14 * 0.9 - 0.5 * elemTicklabel.getBBox().height
                    }
                ));
            }
            // redraw elemGridline
            if (elemGridline) {
                svgAnimate(elemGridline, {
                    d: (
                        `M ${plotLeft} ${yy + CRISPY}`
                        + ` L ${canvasWidth - plotRight} ${yy + CRISPY}`
                    )
                });
            }
        });
        //
        // redraw elemGridline, elemTick, elemTicklabel - end
        //
        //
        // redraw seriesList - start
        //
        seriesList.forEach(function (series) {
            let {
                elemGraph,
                elemGraphtracker,
                pointListSeries
            } = series;
            let elemGraphD = "";
            let pointXPrv = 0;
            let pointYPrv = 0;
            if (series.isHidden) {
                return;
            }
            // redraw pointListSeries
            pointListSeries.forEach(function (pointObj, ii) {
                let barY;
                let {
                    elemPoint,
                    xval,
                    yval
                } = pointObj;
                let pointX;
                let pointY;
                // pixelate xval, yval to pointX, pointY
                pointX = xaxisTranslate(xval);
                pointY = (
                    yval === undefined
                    ? undefined
                    : yaxisTranslate(yval)
                );
                // save pointX, pointY
                pointObj.pointX = pointX;
                pointObj.pointY = pointY;
                if (pointY === undefined) {
                    return;
                }
                if (isBarchart) {
                    // if isBarchart, then redraw point as bar
                    barY = Math.min(pointY, plotbarYaxis);
                    svgAnimate(elemPoint, {
                        height: Math.max(
                            4,
                            Math.max(pointY, plotbarYaxis) - barY
                        ),
                        width: 2 * plotbarMargin,
                        x: pointX - plotbarMargin,
                        y: barY
                    });
                    return;
                }
                if (
                    128 <= (
                        Math.pow(pointX - pointXPrv, 2)
                        + Math.pow(pointY - pointYPrv, 2)
                    )
                    || ii + 1 === pointListSeries.length
                ) {
                    pointXPrv = pointX;
                    pointYPrv = pointY;
                }
                // if not isBarchart, then redraw point as shape
                svgAnimate(elemPoint, {
                    height: 2 * ELEM_POINT_RADIUS,
                    // visibility: (
                    //     pointX === pointXPrv
                    //     ? "inherit"
                    //     : "hidden"
                    // ),
                    width: 2 * ELEM_POINT_RADIUS,
                    x: pointXPrv - ELEM_POINT_RADIUS,
                    y: pointYPrv - ELEM_POINT_RADIUS
                });
                // if not isBarchart, then calculate elemGraphD
                elemGraphD += (
                    (
                        elemGraphD === ""
                        // moveto
                        ? "M"
                        // lineto
                        : "L"
                    )
                    // + ` ${pointX} ${pointY} `
                    + ` ${pointXPrv} ${pointYPrv} `
                );
            });
            // if not isBarchart, then redraw elemGraph
            if (!isBarchart) {
                elemGraphD = elemGraphD.trim();
                svgAnimate(elemGraph, {
                    d: elemGraphD
                });
                elemGraphtracker.setAttribute("d", elemGraphD);
            }
            // calculate pixelToPointDict
            series.xpixelToPointDict = xpixelToPointDictCreate(
                pointListSeries
            );
        });
        // calculate xpixelToPointDictHovered
        if (isBarchart) {
            xpixelToPointDictHovered = xpixelToPointDictCreate(
                pointListBarchart
            );
        }
        // reset seriesHovered
        seriesList.forEach(function (series) {
            if (!seriesHovered || seriesHovered.isHidden) {
                onSeriesHover(series);
                onSeriesUnhover();
            }
        });
        //
        // redraw seriesList - end
        //
    }
    // uichartRedraw - end
    function uichartResize() {
    // this function will resize <uichart>
        // temporarily remove elemCanvasFixed
        // to calculate canvasHeight, canvasWidth
        elemCanvasFixed.remove();
        canvasHeight = Math.round(elemCanvasFlex.clientHeight);
        canvasWidth = Math.round(elemCanvasFlex.clientWidth);
        // restore elemCanvasFixed
        elemCanvasFlex.appendChild(elemCanvasFixed);
        // increment / decrement isResizing
        isResizing += 1;
        setTimeout(function () {
            isResizing -= 1;
        }, UI_ANIMATE_DURATION);
        svgAttrSet(elemCanvasFixed, {
            height: canvasHeight,
            width: canvasWidth
        });
        // redraw from uichartResize
        uichartRedraw();
    }
    function uichartSeriesHideOrShow(series, modeHide) {
    // this function will hide-or-show <series>
        if (!series || series.isDummy) {
            return;
        }
        // reset previously hidden points to yaxis
        if (isBarchart && !modeHide && series.isHidden) {
            series.pointListSeries.forEach(function ({
                elemPoint,
                pointY
            }) {
                if (pointY !== undefined) {
                    svgAttrSet(elemPoint, {
                        height: 0,
                        y: plotbarYaxis
                    });
                }
            });
        }
        series.isHidden = modeHide;
        if (!modeHide) {
            onSeriesHover(series);
        }
        [
            series.elemSeries, series.elemGraphtracker
        ].forEach(function (elem) {
            if (elem) {
                elem.setAttribute("visibility", (
                    modeHide
                    ? "hidden"
                    : "visible"
                ));
            }
        });
        // hide or show legend
        elemLegend.children[series.seriesIi].dataset.hidden = Number(modeHide);
    }
    function xaxisTranslate(xval) {
    // this function will translate <xval> to xpixel position on chart
        return xtransOffset + xtransWidth * (xval - xaxisMin);
    }
    function xpixelToPointDictCreate(pointList) {
    // this function create dict mapping <xpixel> to nearest point in
    // <pointList> along xaxis
        let dict = [];
        let ii = 0;
        let nn = pointList.length;
        let pointObj;
        let pointObjPrv;
        let xpixel = 0;
        let xpixelEnd;
        while (ii < nn) {
            pointObj = pointList[ii];
            if (pointObj.pointY !== undefined && !pointObj.series.isHidden) {
                if (pointObjPrv) {
                    xpixelEnd = 0.5 * (pointObjPrv.pointX + pointObj.pointX);
                    while (xpixel < xpixelEnd) {
                        dict[xpixel] = pointObjPrv;
                        xpixel += 1;
                    }
                }
                pointObjPrv = pointObj;
            }
            ii += 1;
        }
        while (pointObjPrv && xpixel < plotWidth) {
            dict[xpixel] = pointObjPrv;
            xpixel += 1;
        }
        return dict;
    }
    function yaxisTranslate(yval) {
    // this function will translate <yval> to ypixel position on chart
        return plotHeight - ytransWidth * (yval - yaxisMin);
    }
    //
    // uichartCreate - start
    //
    // Resize the box and re-align all aligned elements
    svgAttrSet(elemCanvasFixed, {
        height: canvasHeight,
        width: canvasWidth
    });
    // init event-handling
    elemCanvasFlex.onclick = onSeriesHoveredHide;
    elemCanvasFlex.onmouseenter = onPointUnhover;
    elemCanvasFlex.onmouseleave = onPointUnhover;
    elemCanvasFlex.onmousemove = onPointHover;
    elemCanvasFlex.onwheel = onCanvasZoom;
    elemLegend.onmouseleave = onPointUnhover;
    elemLegend.onmouseover = onSeriesHover;
    elemUichart.querySelector(".uichartNav").onclick = onUichartAction;
    // init seriesList
    seriesList.forEach(function (series, seriesIi) {
        let elemGraph;
        let elemGraphtracker;
        let elemSeries = svgAttrSet("g");
        let {
            isDummy,
            seriesColor,
            xdata,
            ydata
        } = series;
        let pointListSeries;
        let seriesShape;
        elemSeries.classList.add(`elemSeries_${seriesIi}`);
        // init seriesColor
        if (isDummy) {
            seriesColor = "rgba(192,192,192,0)";
        } else if (!(seriesColor && typeof seriesColor === "string")) {
            seriesColor = seriesColorList[
                ((seriesColor ?? counterColor) - 1) % seriesColorList.length
            ];
            counterColor += 1;
        }
        seriesColor = seriesColor.replace((
            /^#(.)(.)(.)$/
        ), "#$1$1$2$2$3$3");
        // init seriesShape
        if (isBarchart || isDummy) {
            seriesShape = "square";
        } else {
            seriesShape = seriesShapeList[
                ((seriesShape ?? counterShape) - 1) % seriesShapeList.length
            ];
            counterShape += 1;
        }
        // init xdata, ydata
        xdata = new Float64Array(xdata);
        ydata = new Float64Array(ydata.map(function (yval) {
            return yval ?? NaN;
        }));
        if (!isBarchart) {
            // init elemGraph
            elemGraph = svgAttrSet("path", {
                fill: "none",
                stroke: seriesColor,
                "stroke-linecap": "round",
                "stroke-linejoin": "round",
                "stroke-width": ELEM_GRAPH_LINE_WIDTH
            });
            elemSeries.appendChild(elemGraph);
            // init elemGraphtracker
            elemGraphtracker = svgAttrSet("path", {
                fill: "none",
                stroke: "rgba(192,192,192,0.0001)",
                "stroke-linecap": "round",
                "stroke-linejoin": "round",
                "stroke-width": ELEM_GRAPH_LINE_WIDTH + 20,
                visibility: "inherit"
            });
            elemGraphtracker.classList.add(`series_${seriesIi}`);
            // init event-handling
            elemGraphtracker.dataset.seriesIi = seriesIi;
            elemGraphtracker.onmouseover = function (evt) {
                onSeriesHover(evt, true);
            };
        }
        // init pointListSeries
        pointListSeries = Array.from(ydata).map(function (yval, ii) {
            let elemPoint;
            let pointObj;
            let xval = xdata[ii];
            elemPoint = (
                isBarchart
                ? svgAttrSet("rect", {
                    fill: seriesColor,
                    stroke: "#333",
                    "stroke-width": ELEM_POINT_BORDER_WIDTH,
                    visibility: "inherit",
                    y: 0.5 * canvasHeight
                })
                : svgAttrSet("path", {
                    fill: seriesColor,
                    stroke: "#333",
                    "stroke-width": ELEM_POINT_BORDER_WIDTH,
                    visibility: "inherit",
                    y: 0.5 * canvasHeight
                })
            );
            // init fx_seriesShape
            if (!isBarchart) {
                elemPoint.fx_seriesShape = seriesShape;
            }
            pointObj = {
                elemPoint,
                series,
                xval,
                yval: (
                    Number.isNaN(yval)
                    ? undefined
                    : yval
                )
            };
            if (isBarchart) {
                pointListBarchart.push(pointObj);
            }
            return pointObj;
        });
        // save series-properties
        Object.assign(series, {
            elemGraph,
            elemGraphtracker,
            elemSeries,
            isHidden: series.isDummy || series.isHidden,
            pointListSeries,
            seriesColor,
            seriesIi,
            seriesShape,
            xdata,
            xpixelToPointDict: [],
            ydata
        });
    });
    if (isBarchart) {
        pointListBarchart.sort(function (aa, bb) {
            return aa.xval - bb.xval;
        });
    }
    // draw seriesList in reverse, so first series has highest z-index
    Array.from(seriesList).reverse().forEach(function ({
        elemGraphtracker,
        elemSeries,
        pointListSeries
    }) {
        elemSeriesList.appendChild(elemSeries);
        Array.from(pointListSeries).reverse().forEach(function ({
            elemPoint
        }) {
            elemSeries.appendChild(elemPoint);
        });
        if (!isBarchart) {
            elemMousetrackerList.appendChild(elemGraphtracker);
        }
    });
    // draw elemLegend
    elemLegend.innerHTML = uichart.seriesList.map(function (series, ii) {
        return (`
<a
    class="uichartAction uichartLegendElem"
    data-action="uichartSeriesHideOrShow"
    data-dummy="${series.isDummy | 0}"
    data-hidden="${series.isHidden | 0}"
    data-series-ii="${ii}"
    title="${stringHtmlSafe(series.seriesName)}"
>
<svg class="uichartLegendElemSvg" xmlns="http://www.w3.org/2000/svg">
<g>
<path
    d="${svgShapeDraw(series.seriesShape, 0, 2, 10, 10)}"
    fill="${series.seriesColor}"
    stroke-width="0"
>
</path>
</g>
</svg>
<span style="margin-left: 12px; position: absolute;">${series.seriesName}</span>
</a>
        `);
    }).join("");
    // first-draw
    uichartRedraw(true);
    // after first-draw, init startup-animation
    svgAttrSet(elemClip.firstElementChild, {
        width: 0
    });
    svgAnimate(elemClip.firstElementChild, {
        width: plotWidth
    });
    return Object.assign(uichart, {
        uichartRedraw,
        uichartResize
    });
    //
    // uichartCreate - end
    //
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
        hashtag,
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
            uiFadeIn(elemLoading);
            await uiTryCatch(async function () {
                // resize uichart
                if (baton.uichart) {
                    baton.uichart.uichartResize();
                    return;
                }
                // create uichart
                baton.uichart = await uichartCreate(baton);
            });
            await waitAsync(500);
            uiFadeOut(elemLoading);
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
            + " rows"
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
        --
        ${dbtableName}.*
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
        ).map(function (row) {
            return row.map(function (val) {
                return (
                    // bugfix - truncate large text to avoid freezing browser
                    (typeof val === "string" && val.length > 65536)
                    ? val.slice(0, 65536)
                    : val
                );
            });
        });
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
        html += (`
<tr data-dbtype="row" data-hashtag="${hashtag}" data-rowid="${row[0]}">
        `);
        row.forEach(function (val) {
            html += "<td>" + (val ?? "") + "</td>";
        });
        html += "</tr>";
    });
    elemTable.children[1].innerHTML = html;
    // debounce - throttle
    await waitAsync(500);
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
<div class="uichart" style="display: none;"></div>
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
                <tr data-dbtype="row" data-hashtag="${baton.hashtag}">
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
    // init event-handling - crud
    contentElem.querySelector(
        "tbody"
    ).oncontextmenu = onContextmenu;
    // init event-handling - sorting
    contentElem.querySelector(
        "thead tr"
    ).onclick = uitableSort.bind(undefined, baton);
    // init event-handling - scrolling
    contentElem.querySelector(
        ".uitableScroller"
    ).onscroll = uitableAjax.bind(undefined, baton);
    contentElem.addEventListener("uitableInit", function (evt) {
        uitableAjax(baton, evt);
    });
    Object.assign(baton, {
        contentElem,
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
        debounce("uitableInitWithinView", uitableInitWithinView, {
            modeDebounce: true
        });
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

function waitAsync(timeout) {
// this function will wait <timeout> milliseconds
    return new Promise(function (resolve) {
        setTimeout(resolve, timeout);
    });
}

// init
window.addEventListener("load", init);
