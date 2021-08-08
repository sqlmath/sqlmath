/*jslint beta, name, node, this*/
let Backup;
let Database;
let EventEmitter = require("events").EventEmitter;
let Statement;
let isVerbose = false;
let sqlite3 = require(
    "./.binding-sqlmath"
    + "-" + "napi" + process.versions.napi
    + "-" + process.platform
    + "-" + process.arch
    + ".node"
);
let supportedEvents = [
    "trace", "profile", "insert", "update", "delete"
];

function databaseAddListener(type, ...argList) {
    let val = EventEmitter.prototype.addListener.call(this, type, ...argList);
    if (supportedEvents.indexOf(type) >= 0) {
        this.configure(type, true);
    }
    return val;
}

function databaseAll(statement, params) {

// Database#all(sql, [bind1, bind2, ...], [callback])

    statement.all.apply(statement, params).finalize();
    return this;
}

function databaseBackup(...argList) {

// Database#backup(filename, [callback])
// Database#backup(filename, destName, sourceName, filenameIsDest, [callback])

    let backup;
    if (argList.length <= 2) {

// By default, we write the main database out to the main database of the named
// file.
// This is the most likely use of the backup api.

        backup = new Backup(this, argList[0], "main", "main", true, argList[1]);
    } else {

// Otherwise, give the user full control over the sqlite3_backup_init arguments.

        backup = new Backup(this, ...argList);
    }

// Per the sqlite docs, exclude the following errors as non-fatal by default.

    backup.retryErrors = [
        sqlite3.BUSY, sqlite3.LOCKED
    ];
    return backup;
}

function databaseEach(statement, params) {

// Database#each(sql, [bind1, bind2, ...], [callback], [complete])

    statement.each.apply(statement, params).finalize();
    return this;
}

function databaseGet(statement, params) {

// Database#get(sql, [bind1, bind2, ...], [callback])

    statement.get.apply(statement, params).finalize();
    return this;
}

function databaseMap(statement, params) {
    statement.map.apply(statement, params).finalize();
    return this;
}

function databasePrepare(statement, params) {

// Database#prepare(sql, [bind1, bind2, ...], [callback])

    return (
        params.length
        ? statement.bind.apply(statement, params)
        : statement
    );
}

function databaseRemoveAllListeners(type, ...argList) {
    let val = EventEmitter.prototype.removeAllListeners.call(
        this,
        type,
        ...argList
    );
    if (supportedEvents.indexOf(type) >= 0) {
        this.configure(type, false);
    }
    return val;
}

function databaseRemoveListener(type, ...argList) {
    let val = EventEmitter.prototype.removeListener.call(
        this,
        type,
        ...argList
    );
    if (supportedEvents.indexOf(type) >= 0 && !this._events[type]) {
        this.configure(type, false);
    }
    return val;
}

function databaseRun(statement, params) {

// Database#run(sql, [bind1, bind2, ...], [callback])

    statement.run.apply(statement, params).finalize();
    return this;
}

function normalizeMethod(fnc) {
    return function (sql, ...argList) {
        let onError;
        if (typeof argList[argList.length - 1] === "function") {
            onError = function (err) {
                if (err) {
                    argList[argList.length - 1](err);
                }
            };
        }
        return fnc.call(this, new Statement(this, sql, onError), argList);
    };
}

function sqliteCachedDatabase(file, aa, bb) {
    let callback;
    let db;
    if (file === "" || file === ":memory:") {

// Don't cache special databases.

        return new Database(file, aa, bb);
    }

    file = require("path").resolve(file);
    db = sqlite3.cached.objects[file];
    if (!db) {
        db = new Database(file, aa, bb);
        sqlite3.cached.objects[file] = db;
        return db;
    }

// Make sure the callback is called.

    callback = (
        (typeof aa === "function")
        ? aa
        : bb
    );
    if (typeof callback === "function" && db.open) {
        process.nextTick(callback.bind(db));
        return db;
    }
    if (typeof callback === "function") {
        db.once("open", callback.bind(db));
        return db;
    }
    return db;
}

function sqliteVerbose() {
    if (!isVerbose) {
        [
            "prepare",
            "get",
            "run",
            "all",
            "each",
            "map",
            "close",
            "exec"
        ].forEach(function (name) {
            sqliteVerboseTrace(Database.prototype, name);
        });
        [
            "bind",
            "get",
            "run",
            "all",
            "each",
            "map",
            "reset",
            "finalize"
        ].forEach(function (name) {
            sqliteVerboseTrace(Statement.prototype, name);
        });
        isVerbose = true;
    }
    return this;
}

function sqliteVerboseTrace(object, property, pos) {

// Save the stack trace over EIO callbacks.
// Inspired by https://github.com/tlrobinson/long-stack-traces

    let old = object[property];
    object[property] = function (...argList) {
        let cb;
        let error = new Error();
        let name = (
            object.constructor.name + "#" + property + "("
            + argList.map(function (elem) {
                return require("util").inspect(elem, false, 0);
            }).join(", ") + ")"
        );
        if (pos === undefined) {
            pos = -1;
        }
        if (pos < 0) {
            pos += argList.length;
        }
        cb = argList[pos];
        if (typeof argList[pos] === "function") {
            argList[pos] = function replacement(err, ...argList) {
                if (err && err.stack && !err.__augmented) {
                    err.stack = err.stack.split("\n").filter(function (line) {
                        return line.indexOf(__filename) < 0;
                    }).join("\n");
                    err.stack += "\n--> in " + name;
                    err.stack += "\n" + error.stack.split(
                        "\n"
                    ).filter(function (line) {
                        return line.indexOf(__filename) < 0;
                    }).slice(1).join("\n");
                    err.__augmented = true;
                }
                return cb.call(this, err, ...argList);
            };
        }
        return old.apply(this, argList);
    };
}

function statementMap(...params) {
    let callback = params.pop();
    params.push(function (err, rowList) {
        let ii;
        let key;
        let keyList;
        let result;
        let val;
        if (err) {
            return callback(err);
        }
        result = {};
        if (rowList.length) {
            keyList = Object.keys(rowList[0]);
            key = keyList[0];
            if (keyList.length > 2) {

// val is an object.

                ii = 0;
                while (ii < rowList.length) {
                    result[rowList[ii][key]] = rowList[ii];
                    ii += 1;
                }
            } else {
                val = keyList[1];

// val is a plain value.

                ii = 0;
                while (ii < rowList.length) {
                    result[rowList[ii][key]] = rowList[ii][val];
                    ii += 1;
                }
            }
        }
        callback(err, result);
    });
    return this.all.apply(this, params);
}

Backup = sqlite3.Backup;
Database = sqlite3.Database;
Statement = sqlite3.Statement;
Object.assign(Backup.prototype, EventEmitter.prototype);
Object.assign(Database.prototype, EventEmitter.prototype, {
    addListener: databaseAddListener,
    all: normalizeMethod(databaseAll),
    backup: databaseBackup,
    each: normalizeMethod(databaseEach),
    get: normalizeMethod(databaseGet),
    map: normalizeMethod(databaseMap),
    on: databaseAddListener,
    prepare: normalizeMethod(databasePrepare),
    removeAllListeners: databaseRemoveAllListeners,
    removeListener: databaseRemoveListener,
    run: normalizeMethod(databaseRun)
});
Object.assign(Statement.prototype, EventEmitter.prototype, {
    map: statementMap
});
Object.assign(sqlite3, {
    cached: {
        Database: sqliteCachedDatabase,
        objects: {}
    },
    verbose: sqliteVerbose
});
module.exports = sqlite3;
