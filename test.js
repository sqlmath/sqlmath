/*jslint beta, for, this*/
/*global require*/ //jslint-quiet

let sqlite3 = require("./sqlmath.js");

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


// test/affected.test.js
(async function () {
    let db = new sqlite3.Database(":memory:");

    await new Promise(function (done) {
        db.run("CREATE TABLE foo (id INT, txt TEXT)", done);
    });

// it should return the correct lastID

    await new Promise(function (done) {
        let ii = 0;
        let jj = 1;
        let stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
        function onError(err) {
            assertOrThrow(!err, err);

// Relies on SQLite's row numbering to be gapless and starting from 1.

            assertJsonEqual(jj, this.lastID);
            jj += 1;
        }
        for (ii = 0; ii < 5000; ii += 1) {
            stmt.run(ii, "demo", onError);
        }
        db.wait(done);
    });

// it should return the correct changes count

    await new Promise(function (done) {
        db.run("UPDATE foo SET id = id + 1 WHERE id % 2 = 0", function (err) {
            assertOrThrow(!err, err);
            assertJsonEqual(2500, this.changes);
            done();
        });
    });
}());


//!! // test/backup.test.js
//!! (async function () {
    //!! let db = new sqlite3.Database(":memory:");

    //!! await new Promise(function (done) {
        //!! db.run("CREATE TABLE foo (id INT, txt TEXT)", done);
    //!! });

//!! // it should return the correct lastID

    //!! await new Promise(function (done) {
        //!! let ii = 0;
        //!! let jj = 1;
        //!! let stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
        //!! function onError(err) {
            //!! assertOrThrow(!err, err);

//!! // Relies on SQLite's row numbering to be gapless and starting from 1.

            //!! assertJsonEqual(jj, this.lastID);
            //!! jj += 1;
        //!! }
        //!! for (ii = 0; ii < 5000; ii += 1) {
            //!! stmt.run(ii, "demo", onError);
        //!! }
        //!! db.wait(done);
    //!! });

//!! // it should return the correct changes count

    //!! await new Promise(function (done) {
        //!! db.run("UPDATE foo SET id = id + 1 WHERE id % 2 = 0",
        //function (err) {
            //!! assertOrThrow(!err, err);
            //!! assertJsonEqual(2500, this.changes);
            //!! done();
        //!! });
    //!! });
//!! }());
