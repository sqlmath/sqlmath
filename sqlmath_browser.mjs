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
    //!! dbFileImportAsync,
    dbOpenAsync,
    debugInline,
    noop,
    //!! objectDeepCopyWithKeysSorted
    sqlmathWebworkerInit
} from "./sqlmath.mjs";

let DB_ATTACHED_DICT = new Map();
let DB_ATTACHED_II = 0;
let DB_MAIN;

noop(
    debugInline,
    dbFileAttachAsync
);

async function dbFileAttachAsync({
    db,
    dbData
}) {
    let dbAttached;
    let dbName;
    DB_ATTACHED_II += 1;
    dbName = `attach${String(DB_ATTACHED_II).padStart(2, "0")}`;
    dbAttached = await dbOpenAsync({
        dbData,
        filename: `file:${dbName}?mode=memory&cache=shared`
    });
    dbAttached.dbName = dbName;
    await dbExecAsync({
        db,
        sql: `ATTACH DATABASE '${dbAttached.filename}' AS ${dbName}`
    });
    DB_ATTACHED_DICT.set(dbName, dbAttached);
}

async function init() {
    let dbData;
    // init main db
    await sqlmathWebworkerInit({});
    DB_MAIN = await dbOpenAsync({
        filename: ":memory:"
    });
    DB_MAIN.dbName = "main";
    DB_ATTACHED_DICT.set(DB_MAIN.dbName, DB_MAIN);
    //
    dbData = await fetch("./.betadog_dtt0_public.sqlite");
    dbData = await dbData.arrayBuffer();
    await dbFileAttachAsync({
        db: DB_MAIN,
        dbData
    });
    //!! debugInline(noop(
        //!! await dbExecAsync({
            //!! db: DB_MAIN,
            //!! //!! responseType: "list",
            //!! //!! sql: "SELECT * FROM sqlite_schema"
            //!! sql: "PRAGMA database_list;"
        //!! })
    //!! ));
    debugInline(noop(
        await dbFileExportAsync({
            db: DB_MAIN
        })
    ));
    //!! debugInline(noop(
        //!! await dbExecAsync({
            //!! db: DB_MAIN,
            //!! //!! responseType: "list",
            //!! sql: "SELECT * FROM attach01.sqlite_schema"
            //!! //!! sql: "PRAGMA DATABASE_LIST;"
        //!! })
    //!! ));
    //!! debugInline(noop(
        //!! await dbExecAsync({
            //!! db: DB_MAIN,
            //!! //!! responseType: "list",
            //!! sql: "SELECT * FROM attach01.sqlite_schema"
            //!! //!! sql: "PRAGMA DATABASE_LIST;"
        //!! })
    //!! ));

    await navRender();
}

async function navRender() {
// this function will render #navPanel1
    let data;
    // sync DB_ATTACHED_DICT
    data = await dbExecAsync({
        db: DB_MAIN,
        sql: "PRAGMA database_list;"
    });
    data = new Set(data[0].map(function ({
        name
    }) {
        return name;
    }));
    debugInline(data);
    DB_ATTACHED_DICT.forEach(function (ignore, dbName) {
        if (!data.has(dbName)) {
            DB_ATTACHED_DICT.delete(dbName);
        }
    });
    //!! debugInline();
    await Promise.all(
        Array.from(DB_ATTACHED_DICT.values()).map(navTableRender)
    );
}

async function navTableRender(db) {
    // this function will render #navPanel1
    let dbName = db.dbName;
    let html = "";
    let tableList;
    tableList = await dbExecAsync({
        db: DB_MAIN,
        sql: `SELECT * FROM ${dbName}.sqlite_schema WHERE type = 'table';`
    });
    tableList = tableList[0];
    debugInline(tableList);
    html += `<tr><td>1</td><td>${dbName}.aa</td></tr>`;
    return html;
}

// init
await init();
