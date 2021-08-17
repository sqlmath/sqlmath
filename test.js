/*jslint beta, node*/

let sqlite3 = require(".");

function assertJsonEqual(aa, bb) {

// This function will assert JSON.stringify(<aa>) === JSON.stringify(<bb>).

    aa = JSON.stringify(objectDeepCopyWithKeysSorted(aa));
    bb = JSON.stringify(objectDeepCopyWithKeysSorted(bb));
    if (aa !== bb) {
        throw new Error(JSON.stringify(aa) + " !== " + JSON.stringify(bb));
    }
}

function assertOrThrow(passed, msg) {

// This function will throw <msg> if <passed> is falsy.

    if (!passed) {
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
}

function lineno() {
    return new Error().stack.match(
        /(?:\n.*?){3}(\d+?):\d+?\)?$/m
    )[1];
}

function objectDeepCopyWithKeysSorted(obj) {

// This function will recursively deep-copy <obj> with keys sorted.

    let sorted;
    if (typeof obj !== "object" || !obj) {
        return obj;
    }
    // recursively deep-copy list with child-keys sorted
    if (Array.isArray(obj)) {
        return obj.map(objectDeepCopyWithKeysSorted);
    }
    // recursively deep-copy obj with keys sorted
    sorted = {};
    Object.keys(obj).sort().forEach(function (key) {
        sorted[key] = objectDeepCopyWithKeysSorted(obj[key]);
    });
    return sorted;
}

async function promiseDescribe(testDescribe, fnc) {
    console.error("    " + lineno() + " - describe - " + testDescribe);
    await fnc();
}

async function promiseIt(testShould, fnc) {
    let timeStart = Date.now();
    process.stderr.write(
        "    " + lineno()
        + " - it"
        + " - " + testShould
        + " ... "
    );
    await promisify(fnc);
    console.error(
        (Date.now() - timeStart) + " ms"
        + " \u001b[32m\u2713\u001b[39m "
    );
}

async function promisify(fnc) {
    if (fnc.length === 0) {
        return fnc();
    }
    await new Promise(function (resolve) {
        fnc(resolve);
    });
}

(async function () {
    console.error(require("path").basename(__filename));
    await promiseDescribe("test sqlmath.noop", async function () {
        let db = new sqlite3.Database(":memory:");
        await promiseIt("test sqlmath.noop", function (done) {
            db.all("SELECT noop('hello world') AS val", function (err, rows) {
                assertOrThrow(!err, err);
                assertJsonEqual(rows[0].val, "hello world");
                done();
            });
        });
    });
    await promiseDescribe("test extension-functions", async function () {
        let db = new sqlite3.Database(":memory:");
        await promiseIt("test atn2", function (done) {
            db.all("SELECT atn2(0, 0) AS val", function (err, rows) {
                assertOrThrow(!err, err);
                assertJsonEqual(rows[0].val, 0);
                done();
            });
        });
    });
    (function () {
        let db = new sqlite3.Database(":memory:");
        db.exec(`
CREATE VIRTUAL TABLE aa USING sqlmatrix(2);
INSERT INTO aa VALUES (NULL,2),(3,4),(5,6);
        `);
        db.all("SELECT * FROM aa;", function (err, rows) {
            assertOrThrow(!err, err);
            console.error(rows);
        });
        db.all("pragma table_info(aa);", function (err, rows) {
            assertOrThrow(!err, err);
            console.error(rows);
        });
        /*
        db.all("SELECT noop2(ptr), cols, rows FROM aa;", function (err, rows) {
            assertOrThrow(!err, err);
            console.error(rows);
        });
        */
        db.all((`
CREATE TABLE tt1 AS
SELECT 101 AS c102, 102 AS c102
UNION ALL
VALUES (201, 202),
       (301, NULL);

CREATE TABLE tt2 AS
SELECT 401 AS c402, 402 AS c402, 403 AS c403
UNION ALL
VALUES (501, 502.0123, 5030123456789),
       (601, NULL, 603),
       (701, b64decode('0123456789'), b64decode('8J+YgQ'));

SELECT * FROM tt1;
SELECT * FROM tt2;
        `), function (err, rows) {
            assertOrThrow(!err, err);
            console.error(rows);
        });
    }());

    // run legacy-test.
    if (!process.env.npm_config_mode_fast) {
        require("./test-old.js");
    }
}());
