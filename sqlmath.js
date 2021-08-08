let path = require("path");
let EventEmitter = require("events").EventEmitter;
let sqlite3 = require(
    "./.binding-sqlmath"
    + "-" + "napi" + process.versions.napi
    + "-" + process.platform
    + "-" + process.arch
    + ".node"
);
module.exports = sqlite3;

function normalizeMethod (fn) {
    return function (sql) {
        let errBack;
        let args = Array.prototype.slice.call(arguments, 1);
        if (typeof args[args.length - 1] === "function") {
            let callback = args[args.length - 1];
            errBack = function(err) {
                if (err) {
                    callback(err);
                }
            };
        }
        let statement = new Statement(this, sql, errBack);
        return fn.call(this, statement, args);
    };
}

sqlite3.cached = {
    Database: function(file, a, b) {
        if (file === "" || file === ":memory:") {
            // Don't cache special databases.
            return new Database(file, a, b);
        }

        let db;
        file = path.resolve(file);
        function cb() { callback.call(db, null); }

        if (!sqlite3.cached.objects[file]) {
            db = sqlite3.cached.objects[file] = new Database(file, a, b);
        }
        else {
            // Make sure the callback is called.
            db = sqlite3.cached.objects[file];
            var callback = (typeof a === "number") ? b : a;
            if (typeof callback === "function") {
                if (db.open) process.nextTick(cb);
                else db.once("open", cb);
            }
        }

        return db;
    },
    objects: {}
};


var Database = sqlite3.Database;
var Statement = sqlite3.Statement;
let Backup = sqlite3.Backup;

Object.assign(Database.prototype, EventEmitter.prototype);
Object.assign(Statement.prototype, EventEmitter.prototype);
Object.assign(Backup.prototype, EventEmitter.prototype);

// Database#prepare(sql, [bind1, bind2, ...], [callback])
Database.prototype.prepare = normalizeMethod(function(statement, params) {
    return params.length
        ? statement.bind.apply(statement, params)
        : statement;
});

// Database#run(sql, [bind1, bind2, ...], [callback])
Database.prototype.run = normalizeMethod(function(statement, params) {
    statement.run.apply(statement, params).finalize();
    return this;
});

// Database#get(sql, [bind1, bind2, ...], [callback])
Database.prototype.get = normalizeMethod(function(statement, params) {
    statement.get.apply(statement, params).finalize();
    return this;
});

// Database#all(sql, [bind1, bind2, ...], [callback])
Database.prototype.all = normalizeMethod(function(statement, params) {
    statement.all.apply(statement, params).finalize();
    return this;
});

// Database#each(sql, [bind1, bind2, ...], [callback], [complete])
Database.prototype.each = normalizeMethod(function(statement, params) {
    statement.each.apply(statement, params).finalize();
    return this;
});

Database.prototype.map = normalizeMethod(function(statement, params) {
    statement.map.apply(statement, params).finalize();
    return this;
});

// Database#backup(filename, [callback])
// Database#backup(filename, destName, sourceName, filenameIsDest, [callback])
Database.prototype.backup = function() {
    let backup;
    if (arguments.length <= 2) {
        // By default, we write the main database out to the main database of the named file.
        // This is the most likely use of the backup api.
        backup = new Backup(this, arguments[0], "main", "main", true, arguments[1]);
    } else {
        // Otherwise, give the user full control over the sqlite3_backup_init arguments.
        backup = new Backup(this, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
    }
    // Per the sqlite docs, exclude the following errors as non-fatal by default.
    backup.retryErrors = [sqlite3.BUSY, sqlite3.LOCKED];
    return backup;
};

Statement.prototype.map = function() {
    let params = Array.prototype.slice.call(arguments);
    let callback = params.pop();
    params.push(function(err, rows) {
        if (err) return callback(err);
        let result = {};
        if (rows.length) {
            let keys = Object.keys(rows[0]),
                key = keys[0];
            if (keys.length > 2) {
                // Value is an object
                for (var i = 0; i < rows.length; i++) {
                    result[rows[i][key]] = rows[i];
                }
            } else {
                let value = keys[1];
                // Value is a plain value
                for (i = 0; i < rows.length; i++) {
                    result[rows[i][key]] = rows[i][value];
                }
            }
        }
        callback(err, result);
    });
    return this.all.apply(this, params);
};

let isVerbose = false;

let supportedEvents = [ "trace", "profile", "insert", "update", "delete" ];

Database.prototype.addListener = Database.prototype.on = function(type) {
    let val = EventEmitter.prototype.addListener.apply(this, arguments);
    if (supportedEvents.indexOf(type) >= 0) {
        this.configure(type, true);
    }
    return val;
};

Database.prototype.removeListener = function(type) {
    let val = EventEmitter.prototype.removeListener.apply(this, arguments);
    if (supportedEvents.indexOf(type) >= 0 && !this._events[type]) {
        this.configure(type, false);
    }
    return val;
};

Database.prototype.removeAllListeners = function(type) {
    let val = EventEmitter.prototype.removeAllListeners.apply(this, arguments);
    if (supportedEvents.indexOf(type) >= 0) {
        this.configure(type, false);
    }
    return val;
};

// Save the stack trace over EIO callbacks.
// Inspired by https://github.com/tlrobinson/long-stack-traces
let util = require("util");
function trace(object, property, pos) {
    let old = object[property];
    object[property] = function() {
        let error = new Error();
        let name = object.constructor.name + "#" + property + "(" +
            Array.prototype.slice.call(arguments).map(function(el) {
                return util.inspect(el, false, 0);
            }).join(", ") + ")";

        if (typeof pos === "undefined") pos = -1;
        if (pos < 0) pos += arguments.length;
        let cb = arguments[pos];
        if (typeof arguments[pos] === "function") {
            arguments[pos] = function replacement() {
                let err = arguments[0];
                if (err && err.stack && !err.__augmented) {
                    err.stack = err.stack.split("\n").filter(function(line) {
                        return line.indexOf(__filename) < 0;
                    }).join("\n");
                    err.stack += "\n--> in " + name;
                    err.stack += "\n" + error.stack.split("\n").filter(function(line) {
                        return line.indexOf(__filename) < 0;
                    }).slice(1).join("\n");
                    err.__augmented = true;
                }
                return cb.apply(this, arguments);
            };
        }
        return old.apply(this, arguments);
    };
}
sqlite3.verbose = function() {
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
            trace(Database.prototype, name);
        });
        [
            "bind",
            "get",
            "run",
            "all",
            "each",
            "map",
            "reset",
            "finalize",
        ].forEach(function (name) {
            trace(Statement.prototype, name);
        });
        isVerbose = true;
    }

    return this;
};
