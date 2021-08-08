/*mode-coverage-ignore-file*/
/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/LICENSE"
        },
        {
            "urlIgnore": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/createdb-electron.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/createdb.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/helper.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/script.sql"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/affected.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/backup.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/blob.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/cache.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/constants.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/database_fail.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/each.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/exec.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/extension.test.js"
        },
        {
            "urlIgnore": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/extension_functions.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/fts-content.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/interrupt.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/issue-108.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/json.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/map.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/named_columns.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/named_params.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/null_error.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/open_close.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/other_objects.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/parallel_insert.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/prepare.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/profile.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/rerun.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/scheduling.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/serialization.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/trace.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/unicode.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/upsert.test.js"
        },
        {
            "url": "https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/verbose.test.js"
        }
    ],
    "replaceList": [
        {
            "aa": "\n/\\*\nfile .*?/test/.*?([^/]*?)\n\\*\\/\n",
            "bb": "\n}());$&await (async function() {\nconsole.error(\"\\ntest-old.js - $1\");\n",
            "flags": "g"
        },
        {
            "aa": "\\(sqlite3.VERSION_NUMBER\\b.*?\\bit\\)",
            "bb": "it",
            "flags": "g"
        },
        {
            "aa": "\\b(before|it) *?\\(",
            "bb": "await $&",
            "flags": "g"
        },
        {
            "aa": "\\bdescribe *?\\([\\S\\s]*?,",
            "bb": "await $& async",
            "flags": "g"
        },
        {
            "aa": "\\brequire\\('\\.\\.'\\)",
            "bb": "require('.')",
            "flags": "g"
        },
        {
            "aa": "var helper = require\\('./support/helper'\\);",
            "bb": "// $&",
            "flags": "g"
        },
        {
            "aa": "\\btest/tmp\\b",
            "bb": ".tmp/test",
            "flags": "g"
        },
        {
            "aa": "'test/support/prepare.db'",
            "bb": "'test-old-prepare.db'",
            "flags": "g"
        }
    ]
}
-                    assert.equal(row.integer, integer);
-                    done();
-                });
-            });
-        });
-    });
+                    assert.equal(row.integer, integer);
+                    done();
+                });
+            });
+        });
+// hack-test
+    }

-            var buff = new Buffer(2);
+// hack-test
+            var buff = Buffer.alloc(2);

-        db = new sqlite3.Database('test/support/big.db', sqlite3.OPEN_READONLY, done);
+// hack-test
+        db = new sqlite3.Database('.tmp/test/big.db', sqlite3.OPEN_READONLY, done);

-        var sql = fs.readFileSync('test/support/script.sql', 'utf8');
+// hack-test
+        var sql = assetDict['test/support/script.sql'];

-    (sqlite3.VERSION_NUMBER < 3008000 ? describe.skip : describe)('open and close shared memory database', function() {
+// hack-test
+    describe('open and close shared memory database', async function() {

-    [
-        4294967296.249,
+// hack-test
+    var list = [
+        4294967296.249,

-    ].forEach(function(flt) {
+// hack-test
+    ];
+    while (true) {
+        var flt = list.shift();
+        if (!flt) {
+            break;
+        }

-    ].forEach(function(integer) {
+// hack-test
+    ];
+    while (true) {
+        var integer = list.shift();
+        if (!integer) {
+            break;
+        }

-    var count = 1000000;
-    var db_path = path.join(__dirname,'big.db');
+// hack-test
+    var count = 100000;
+    var db_path = path.join(__dirname,'.tmp/test/big.db');

-    var sqlite3 = require('../../lib/sqlite3');
+// hack-test
+    var sqlite3 = require('.');

-    });
-
-    [
-        4294967299,
+// hack-test
+    }
+
+    var list = [
+        4294967299,

-/\\*
-file none
-*\\/
+}());
+}());
+/\\*
+file none
+*\\/

-console.error("\ntest-old.js - script.sql");
+console.error("\ntest-old.js - script.sql");
+// hack-test
+assetDict["test/support/script.sql"] = `

-exports.deleteFile = function(name) {
+// hack-test
+helper.deleteFile = function(name) {

-exports.ensureExists = function(name,cb) {
+// hack-test
+helper.ensureExists = function(name,cb) {

-if (require.main === module) {
-    createdb();
+// hack-test
+if (true || require.main === module) {
+    await new Promise(function (resolve) {createdb(resolve);});

-var elmo = fs.readFileSync(__dirname + '/support/elmo.png');
+// hack-test
+var elmo = Buffer.from([1, 2, 3, 4]);

-}());
+// hack-test
+(async function() {
+/\\*jslint beta, name*\\/
+/\\*global console, require, process*\\/
+let after;
+let afterEach2;
+let assetDict = {};
+let before;
+let beforeEach2;
+let helper = {};
+function afterEach(fnc) {
+    afterEach2 = fnc;
+}
+async function awaitFnc(fnc) {
+    if (fnc.length === 0) {
+        return fnc();
+    }
+    await new Promise(function (resolve) {
+        fnc(resolve);
+    });
+}
+function beforeEach(fnc) {
+    beforeEach2 = fnc;
+}
+async function describe(testDescribe, fnc) {
+    console.error("    " + lineno() + " - describe - " + testDescribe);
+    afterEach2 = noop;
+    beforeEach2 = noop;
+    await fnc();
+}
+async function it(testShould, fnc) {
+    let timeStart = Date.now();
+    process.stderr.write(
+        "    " + lineno()
+        + " - it"
+        + " - " + testShould
+        + " ... "
+    );
+    await awaitFnc(beforeEach2);
+    await awaitFnc(fnc);
+    await awaitFnc(afterEach2);
+    console.error(
+        (Date.now() - timeStart) + " ms"
+        + " \u001b[32m\u2713\u001b[39m "
+    );
+}
+function lineno() {
+    return new Error().stack.match(
+        /(?:\n.*?){3}(\d+?):\d+?\)?\n/
+    )[1];
+}
+function noop() {
+    return;
+}
+after = awaitFnc;
+before = awaitFnc;
+require("fs").mkdirSync(".tmp/test", {recursive: true});

-}());
-/\\*
-file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/affected.test.js
-*\\/
+// hack-test
+`;
+}());
+/\\*
+file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/affected.test.js
+*\\/
*/


/*
repo https://github.com/mapbox/node-sqlite3/tree/v5.0.2
committed 2021-02-15T14:42:52Z
*/


/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/LICENSE
*/
/*
Copyright (c) MapBox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
- Neither the name "MapBox" nor the names of its contributors may be
  used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


// hack-test
(async function() {
/*jslint beta, name*/
/*global console, require, process*/
let after;
let afterEach2;
let assetDict = {};
let before;
let beforeEach2;
let helper = {};
function afterEach(fnc) {
    afterEach2 = fnc;
}
async function awaitFnc(fnc) {
    if (fnc.length === 0) {
        return fnc();
    }
    await new Promise(function (resolve) {
        fnc(resolve);
    });
}
function beforeEach(fnc) {
    beforeEach2 = fnc;
}
async function describe(testDescribe, fnc) {
    console.error("    " + lineno() + " - describe - " + testDescribe);
    afterEach2 = noop;
    beforeEach2 = noop;
    await fnc();
}
async function it(testShould, fnc) {
    let timeStart = Date.now();
    process.stderr.write(
        "    " + lineno()
        + " - it"
        + " - " + testShould
        + " ... "
    );
    await awaitFnc(beforeEach2);
    await awaitFnc(fnc);
    await awaitFnc(afterEach2);
    console.error(
        (Date.now() - timeStart) + " ms"
        + " \u001b[32m\u2713\u001b[39m "
    );
}
function lineno() {
    return new Error().stack.match(
        /(?:\n.*?){3}(\d+?):\d+?\)?\n/
    )[1];
}
function noop() {
    return;
}
after = awaitFnc;
before = awaitFnc;
require("fs").mkdirSync(".tmp/test", {recursive: true});
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/createdb.js
*/
await (async function() {
console.error("\ntest-old.js - createdb.js");
// #!/usr/bin/env node

function createdb(callback) {
    var existsSync = require('fs').existsSync || require('path').existsSync;
    var path = require('path');

// hack-test
    var sqlite3 = require('.');

// hack-test
    var count = 100000;
    var db_path = path.join(__dirname,'.tmp/test/big.db');

    function randomString() {
        var str = '';
        var chars = 'abcdefghijklmnopqrstuvwxzyABCDEFGHIJKLMNOPQRSTUVWXZY0123456789  ';
        for (var i = Math.random() * 100; i > 0; i--) {
            str += chars[Math.floor(Math.random() * chars.length)];
        }
        return str;
    };


    if (existsSync(db_path)) {
        console.log('okay: database already created (' + db_path + ')');
        if (callback) callback();
    } else {
        console.log("Creating test database... This may take several minutes.");
        var db = new sqlite3.Database(db_path);
        db.serialize(function() {
            db.run("CREATE TABLE foo (id INT, txt TEXT)");
            db.run("BEGIN TRANSACTION");
            var stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
            for (var i = 0; i < count; i++) {
                stmt.run(i, randomString());
            }
            stmt.finalize();
            db.run("COMMIT TRANSACTION", [], function () {
                db.close(callback);
            });
        });
    }
};

// hack-test
if (true || require.main === module) {
    await new Promise(function (resolve) {createdb(resolve);});
}

module.exports = createdb;


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/helper.js
*/
await (async function() {
console.error("\ntest-old.js - helper.js");
var assert = require('assert');
var fs = require('fs');
var pathExists = require('fs').existsSync || require('path').existsSync;

// hack-test
helper.deleteFile = function(name) {
    try {
        fs.unlinkSync(name);
    } catch(err) {
        if (err.errno !== process.ENOENT && err.code !== 'ENOENT' && err.syscall !== 'unlink') {
            throw err;
        }
    }
};

// hack-test
helper.ensureExists = function(name,cb) {
    if (!pathExists(name)) {
        fs.mkdirSync(name);
    };
}

assert.fileDoesNotExist = function(name) {
    try {
        fs.statSync(name);
    } catch(err) {
        if (err.errno !== process.ENOENT && err.code !== 'ENOENT' && err.syscall !== 'unlink') {
            throw err;
        }
    }
};

assert.fileExists = function(name) {
    try {
        fs.statSync(name);
    } catch(err) {
        throw err;
    }
};


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/support/script.sql
*/
await (async function() {
console.error("\ntest-old.js - script.sql");
// hack-test
assetDict["test/support/script.sql"] = `
CREATE TABLE IF NOT EXISTS map (
   zoom_level INTEGER,
   tile_column INTEGER,
   tile_row INTEGER,
   tile_id TEXT,
   grid_id TEXT
);

CREATE TABLE IF NOT EXISTS grid_key (
    grid_id TEXT,
    key_name TEXT
);

CREATE TABLE IF NOT EXISTS keymap (
    key_name TEXT,
    key_json TEXT
);

CREATE TABLE IF NOT EXISTS grid_utfgrid (
    grid_id TEXT,
    grid_utfgrid TEXT
);

CREATE TABLE IF NOT EXISTS images (
    tile_data blob,
    tile_id text
);

CREATE TABLE IF NOT EXISTS metadata (
    name text,
    value text
);


CREATE UNIQUE INDEX IF NOT EXISTS map_index ON map (zoom_level, tile_column, tile_row);
CREATE UNIQUE INDEX IF NOT EXISTS grid_key_lookup ON grid_key (grid_id, key_name);
CREATE UNIQUE INDEX IF NOT EXISTS keymap_lookup ON keymap (key_name);
CREATE UNIQUE INDEX IF NOT EXISTS grid_utfgrid_lookup ON grid_utfgrid (grid_id);
CREATE UNIQUE INDEX IF NOT EXISTS images_id ON images (tile_id);
CREATE UNIQUE INDEX IF NOT EXISTS name ON metadata (name);


CREATE VIEW IF NOT EXISTS tiles AS
    SELECT
        map.zoom_level AS zoom_level,
        map.tile_column AS tile_column,
        map.tile_row AS tile_row,
        images.tile_data AS tile_data
    FROM map
    JOIN images ON images.tile_id = map.tile_id;

CREATE VIEW IF NOT EXISTS grids AS
    SELECT
        map.zoom_level AS zoom_level,
        map.tile_column AS tile_column,
        map.tile_row AS tile_row,
        grid_utfgrid.grid_utfgrid AS grid
    FROM map
    JOIN grid_utfgrid ON grid_utfgrid.grid_id = map.grid_id;

CREATE VIEW IF NOT EXISTS grid_data AS
    SELECT
        map.zoom_level AS zoom_level,
        map.tile_column AS tile_column,
        map.tile_row AS tile_row,
        keymap.key_name AS key_name,
        keymap.key_json AS key_json
    FROM map
    JOIN grid_key ON map.grid_id = grid_key.grid_id
    JOIN keymap ON grid_key.key_name = keymap.key_name;


// hack-test
`;
}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/affected.test.js
*/
await (async function() {
console.error("\ntest-old.js - affected.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('query properties', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:');
        db.run("CREATE TABLE foo (id INT, txt TEXT)", done);
    });

    await it('should return the correct lastID', function(done) {
        var stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
        var j = 1;
        for (var i = 0; i < 5000; i++) {
            stmt.run(i, "demo", function(err) {
                if (err) throw err;
                // Relies on SQLite's row numbering to be gapless and starting
                // from 1.
                assert.equal(j++, this.lastID);
            });
        }
        db.wait(done);
    });

    await it('should return the correct changes count', function(done) {
        db.run("UPDATE foo SET id = id + 1 WHERE id % 2 = 0", function(err) {
            if (err) throw err;
            assert.equal(2500, this.changes);
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/backup.test.js
*/
await (async function() {
console.error("\ntest-old.js - backup.test.js");
var sqlite3 = require('.');
var assert = require('assert');
var fs = require('fs');
// var helper = require('./support/helper');

// Check that the number of rows in two tables matches.
function assertRowsMatchDb(db1, table1, db2, table2, done) {
    db1.get("SELECT COUNT(*) as count FROM " + table1, function(err, row) {
        if (err) throw err;
        db2.get("SELECT COUNT(*) as count FROM " + table2, function(err, row2) {
            if (err) throw err;
            assert.equal(row.count, row2.count);
            done();
        });
    });
}

// Check that the number of rows in the table "foo" is preserved in a backup.
function assertRowsMatchFile(db, backupName, done) {
    var db2 = new sqlite3.Database(backupName, sqlite3.OPEN_READONLY, function(err) {
        if (err) throw err;
        assertRowsMatchDb(db, 'foo', db2, 'foo', function() {
            db2.close(done);
        });
    });
}

await describe('backup', async function() {
    await before(function() {
        helper.ensureExists('.tmp/test');
    });

    var db;
    beforeEach(function(done) {
        helper.deleteFile('.tmp/test/backup.db');
        helper.deleteFile('.tmp/test/backup2.db');
        db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done);
    });

    afterEach(function(done) {
        if (!db) { done(); }
        db.close(done);
    });

    await it ('output db created once step is called', function(done) {
        var backup = db.backup('.tmp/test/backup.db', function(err) {
            if (err) throw err;
            backup.step(1, function(err) {
                if (err) throw err;
                assert.fileExists('.tmp/test/backup.db');
                backup.finish(done);
            });
        });
    });

    await it ('copies source fully with step(-1)', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.step(-1, function(err) {
            if (err) throw err;
            assert.fileExists('.tmp/test/backup.db');
            backup.finish(function(err) {
                if (err) throw err;
                assertRowsMatchFile(db, '.tmp/test/backup.db', done);
            });
        });
    });

    await it ('backup db not created if finished immediately', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.finish(function(err) {
            if (err) throw err;
            assert.fileDoesNotExist('.tmp/test/backup.db');
            done();
        });
    });

    await it ('error closing db if backup not finished', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        db.close(function(err) {
            db = null;
            if (!err) throw new Error('should have an error');
            if (err.errno == sqlite3.BUSY) {
                done();
            }
            else throw err;
        });
    });

    await it ('using the backup after finished is an error', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.finish(function(err) {
            if (err) throw err;
            backup.step(1, function(err) {
                if (!err) throw new Error('should have an error');
                if (err.errno == sqlite3.MISUSE &&
                    err.message === 'SQLITE_MISUSE: Backup is already finished') {
                    done();
                }
                else throw err;
            });
        });
    });

    await it ('remaining/pageCount are available after call to step', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.step(0, function(err) {
            if (err) throw err;
            assert.equal(typeof this.pageCount, 'number');
            assert.equal(typeof this.remaining, 'number');
            assert.equal(this.remaining, this.pageCount);
            var prevRemaining = this.remaining;
            var prevPageCount = this.pageCount;
            backup.step(1, function(err) {
                if (err) throw err;
                assert.notEqual(this.remaining, prevRemaining);
                assert.equal(this.pageCount, prevPageCount);
                backup.finish(done);
            });
        });
    });

    await it ('backup works if database is modified half-way through', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.step(-1, function(err) {
            if (err) throw err;
            backup.finish(function(err) {
                if (err) throw err;
                var db2 = new sqlite3.Database('.tmp/test/backup.db', function(err) {
                    if (err) throw err;
                    var backup2 = db2.backup('.tmp/test/backup2.db');
                    backup2.step(1, function(err, completed) {
                        if (err) throw err;
                        assert.equal(completed, false);  // Page size for the test db
                        // should not be raised to high.
                        db2.exec("insert into foo(txt) values('hello')", function(err) {
                            if (err) throw err;
                            backup2.step(-1, function(err, completed) {
                                if (err) throw err;
                                assert.equal(completed, true);
                                assertRowsMatchFile(db2, '.tmp/test/backup2.db', function() {
                                    backup2.finish(function(err) {
                                        if (err) throw err;
                                        db2.close(done);
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    await it ('can backup from temp to main', function(done) {
        db.exec("CREATE TEMP TABLE space (txt TEXT)", function(err) {
            if (err) throw err;
            db.exec("INSERT INTO space(txt) VALUES('monkey')", function(err) {
                if (err) throw err;
                var backup = db.backup('.tmp/test/backup.db', 'temp', 'main', true, function(err) {
                    if (err) throw err;
                    backup.step(-1, function(err) {
                        if (err) throw err;
                        backup.finish(function(err) {
                            if (err) throw err;
                            var db2 = new sqlite3.Database('.tmp/test/backup.db', function(err) {
                                if (err) throw err;
                                db2.get("SELECT * FROM space", function(err, row) {
                                    if (err) throw err;
                                    assert.equal(row.txt, 'monkey');
                                    db2.close(done);
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    await it ('can backup from main to temp', function(done) {
        var backup = db.backup('test-old-prepare.db', 'main', 'temp', false, function(err) {
            if (err) throw err;
            backup.step(-1, function(err) {
                if (err) throw err;
                backup.finish(function(err) {
                    if (err) throw err;
                    assertRowsMatchDb(db, 'temp.foo', db, 'main.foo', done);
                });
            });
        });
    });

    await it ('cannot backup to a locked db', function(done) {
        var db2 = new sqlite3.Database('.tmp/test/backup.db', function(err) {
            db2.exec("PRAGMA locking_mode = EXCLUSIVE");
            db2.exec("BEGIN EXCLUSIVE", function(err) {
                if (err) throw err;
                var backup = db.backup('.tmp/test/backup.db');
                backup.step(-1, function(stepErr) {
                    db2.close(function(err) {
                        if (err) throw err;
                        if (stepErr.errno == sqlite3.BUSY) {
                            backup.finish(done);
                        }
                        else throw stepErr;
                    });
                });
            });
        });
    });

    await it ('fuss-free incremental backups work', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        var timer;
        function makeProgress() {
            if (backup.idle) {
                backup.step(1);
            }
            if (backup.completed || backup.failed) {
                clearInterval(timer);
                assert.equal(backup.completed, true);
                assert.equal(backup.failed, false);
                done();
            }
        }
        timer = setInterval(makeProgress, 2);
    });

    await it ('setting retryErrors to empty disables automatic finishing', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.retryErrors = [];
        backup.step(-1, function(err) {
            if (err) throw err;
            db.close(function(err) {
                db = null;
                if (!err) throw new Error('should have an error');
                assert.equal(err.errno, sqlite3.BUSY);
                done();
            });
        });
    });

    await it ('setting retryErrors enables automatic finishing', function(done) {
        var backup = db.backup('.tmp/test/backup.db');
        backup.retryErrors = [sqlite3.OK];
        backup.step(-1, function(err) {
            if (err) throw err;
            db.close(function(err) {
                if (err) throw err;
                db = null;
                done();
            });
        });
    });

    await it ('default retryErrors will retry on a locked/busy db', function(done) {
        var db2 = new sqlite3.Database('.tmp/test/backup.db', function(err) {
            db2.exec("PRAGMA locking_mode = EXCLUSIVE");
            db2.exec("BEGIN EXCLUSIVE", function(err) {
                if (err) throw err;
                var backup = db.backup('.tmp/test/backup.db');
                backup.step(-1, function(stepErr) {
                    db2.close(function(err) {
                        if (err) throw err;
                        assert.equal(stepErr.errno, sqlite3.BUSY);
                        assert.equal(backup.completed, false);
                        assert.equal(backup.failed, false);
                        backup.step(-1, function(err) {
                            if (err) throw err;
                            assert.equal(backup.completed, true);
                            assert.equal(backup.failed, false);
                            done();
                        });
                    });
                });
            });
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/blob.test.js
*/
await (async function() {
console.error("\ntest-old.js - blob.test.js");
var sqlite3 = require('.'),
    fs = require('fs'),
    assert = require('assert'),
    Buffer = require('buffer').Buffer;

// lots of elmo
// hack-test
var elmo = Buffer.from([1, 2, 3, 4]);

await describe('blob', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:');
        db.run("CREATE TABLE elmos (id INT, image BLOB)", done);
    });

    var total = 10;
    var inserted = 0;
    var retrieved = 0;


    await it('should insert blobs', function(done) {
        for (var i = 0; i < total; i++) {
            db.run('INSERT INTO elmos (id, image) VALUES (?, ?)', i, elmo, function(err) {
                if (err) throw err;
                inserted++;
            });
        }
        db.wait(function() {
            assert.equal(inserted, total);
            done();
        });
    });

    await it('should retrieve the blobs', function(done) {
        db.all('SELECT id, image FROM elmos ORDER BY id', function(err, rows) {
            if (err) throw err;
            for (var i = 0; i < rows.length; i++) {
                assert.ok(Buffer.isBuffer(rows[i].image));
                assert.ok(elmo.length, rows[i].image);

                for (var j = 0; j < elmo.length; j++) {
                    if (elmo[j] !== rows[i].image[j]) {
                        assert.ok(false, "Wrong byte");
                    }
                }

                retrieved++;
            }

            assert.equal(retrieved, total);
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/cache.test.js
*/
await (async function() {
console.error("\ntest-old.js - cache.test.js");
var sqlite3 = require('.');
var assert = require('assert');
// var helper = require('./support/helper');

await describe('cache', async function() {
    await before(function() {
        helper.ensureExists('.tmp/test');
    });

    await it('should cache Database objects while opening', function(done) {
        var filename = '.tmp/test/test_cache.db';
        helper.deleteFile(filename);
        var opened1 = false, opened2 = false;
        var db1 = new sqlite3.cached.Database(filename, function(err) {
            if (err) throw err;
            opened1 = true;
            if (opened1 && opened2) done();
        });
        var db2 = new sqlite3.cached.Database(filename, function(err) {
            if (err) throw err;
            opened2 = true;
            if (opened1 && opened2) done();
        });
        assert.equal(db1, db2);
    });

    await it('should cache Database objects after they are open', function(done) {
        var filename = '.tmp/test/test_cache2.db';
        helper.deleteFile(filename);
        var db1, db2;
        db1 = new sqlite3.cached.Database(filename, function(err) {
            if (err) throw err;
            process.nextTick(function() {
                db2 = new sqlite3.cached.Database(filename, function(err) {
                    done();
                });
                assert.equal(db1, db2);
            });
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/constants.test.js
*/
await (async function() {
console.error("\ntest-old.js - constants.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('constants', async function() {
    await it('should have the right OPEN_* flags', function() {
        assert.ok(sqlite3.OPEN_READONLY === 1);
        assert.ok(sqlite3.OPEN_READWRITE === 2);
        assert.ok(sqlite3.OPEN_CREATE === 4);
        assert.ok(sqlite3.OPEN_URI === 0x00000040);
        assert.ok(sqlite3.OPEN_FULLMUTEX === 0x00010000);
        assert.ok(sqlite3.OPEN_SHAREDCACHE === 0x00020000);
        assert.ok(sqlite3.OPEN_PRIVATECACHE === 0x00040000);
    });

    await it('should have the right error flags', function() {
        assert.ok(sqlite3.OK === 0);
        assert.ok(sqlite3.ERROR === 1);
        assert.ok(sqlite3.INTERNAL === 2);
        assert.ok(sqlite3.PERM === 3);
        assert.ok(sqlite3.ABORT === 4);
        assert.ok(sqlite3.BUSY === 5);
        assert.ok(sqlite3.LOCKED === 6);
        assert.ok(sqlite3.NOMEM === 7);
        assert.ok(sqlite3.READONLY === 8);
        assert.ok(sqlite3.INTERRUPT === 9);
        assert.ok(sqlite3.IOERR === 10);
        assert.ok(sqlite3.CORRUPT === 11);
        assert.ok(sqlite3.NOTFOUND === 12);
        assert.ok(sqlite3.FULL === 13);
        assert.ok(sqlite3.CANTOPEN === 14);
        assert.ok(sqlite3.PROTOCOL === 15);
        assert.ok(sqlite3.EMPTY === 16);
        assert.ok(sqlite3.SCHEMA === 17);
        assert.ok(sqlite3.TOOBIG === 18);
        assert.ok(sqlite3.CONSTRAINT === 19);
        assert.ok(sqlite3.MISMATCH === 20);
        assert.ok(sqlite3.MISUSE === 21);
        assert.ok(sqlite3.NOLFS === 22);
        assert.ok(sqlite3.AUTH === 23);
        assert.ok(sqlite3.FORMAT === 24);
        assert.ok(sqlite3.RANGE === 25);
        assert.ok(sqlite3.NOTADB === 26);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/database_fail.test.js
*/
await (async function() {
console.error("\ntest-old.js - database_fail.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('error handling', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);
    });

    await it('throw when calling Database() without new', function() {
        assert.throws(function() {
            sqlite3.Database(':memory:');
        }, (/Class constructors cannot be invoked without 'new'/));

        assert.throws(function() {
            sqlite3.Statement();
        }, (/Class constructors cannot be invoked without 'new'/));
    });

    await it('should error when calling Database#get on a missing table', function(done) {
        db.get('SELECT id, txt FROM foo', function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#all prepare fail', function(done) {
        db.all('SELECT id, txt FROM foo', function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#run prepare fail', function(done) {
        db.run('SELECT id, txt FROM foo', function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#each prepare fail', function(done) {
        db.each('SELECT id, txt FROM foo', function(err, row) {
            assert.ok(false, "this should not be called");
        }, function(err, num) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#each prepare fail without completion handler', function(done) {
        db.each('SELECT id, txt FROM foo', function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#get prepare fail with param binding', function(done) {
        db.get('SELECT id, txt FROM foo WHERE id = ?', 1, function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#all prepare fail with param binding', function(done) {
        db.all('SELECT id, txt FROM foo WHERE id = ?', 1, function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#run prepare fail with param binding', function(done) {
        db.run('SELECT id, txt FROM foo WHERE id = ?', 1, function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#each prepare fail with param binding', function(done) {
        db.each('SELECT id, txt FROM foo WHERE id = ?', 1, function(err, row) {
            assert.ok(false, "this should not be called");
        }, function(err, num) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });

    await it('Database#each prepare fail with param binding without completion handler', function(done) {
        db.each('SELECT id, txt FROM foo WHERE id = ?', 1, function(err, row) {
            if (err) {
                assert.equal(err.message, 'SQLITE_ERROR: no such table: foo');
                assert.equal(err.errno, sqlite3.ERROR);
                assert.equal(err.code, 'SQLITE_ERROR');
                done();
            } else {
                done(new Error('Completed query without error, but expected error'));
            }
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/each.test.js
*/
await (async function() {
console.error("\ntest-old.js - each.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('each', async function() {
    var db;
    await before(function(done) {
// hack-test
        db = new sqlite3.Database('.tmp/test/big.db', sqlite3.OPEN_READONLY, done);
    });

    await it('retrieve 100,000 rows with Statement#each', function(done) {
        var total = 100000;
        var retrieved = 0;


        db.each('SELECT id, txt FROM foo LIMIT 0, ?', total, function(err, row) {
            if (err) throw err;
            retrieved++;

            if(retrieved === total) {
                assert.equal(retrieved, total, "Only retrieved " + retrieved + " out of " + total + " rows.");
                done();
            }
        });
    });

    await it('Statement#each with complete callback', function(done) {
        var total = 10000;
        var retrieved = 0;

        db.each('SELECT id, txt FROM foo LIMIT 0, ?', total, function(err, row) {
            if (err) throw err;
            retrieved++;
        }, function(err, num) {
            assert.equal(retrieved, num);
            assert.equal(retrieved, total, "Only retrieved " + retrieved + " out of " + total + " rows.");
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/exec.test.js
*/
await (async function() {
console.error("\ntest-old.js - exec.test.js");
var sqlite3 = require('.');
var assert = require('assert');
var fs = require('fs');

await describe('exec', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);
    });

    await it('Database#exec', function(done) {
// hack-test
        var sql = assetDict['test/support/script.sql'];
        db.exec(sql, done);
    });

    await it('retrieve database structure', function(done) {
        db.all("SELECT type, name FROM sqlite_master ORDER BY type, name", function(err, rows) {
            if (err) throw err;
            assert.deepEqual(rows, [
                { type: 'index', name: 'grid_key_lookup' },
                { type: 'index', name: 'grid_utfgrid_lookup' },
                { type: 'index', name: 'images_id' },
                { type: 'index', name: 'keymap_lookup' },
                { type: 'index', name: 'map_index' },
                { type: 'index', name: 'name' },
                { type: 'table', name: 'grid_key' },
                { type: 'table', name: 'grid_utfgrid' },
                { type: 'table', name: 'images' },
                { type: 'table', name: 'keymap' },
                { type: 'table', name: 'map' },
                { type: 'table', name: 'metadata' },
                { type: 'view', name: 'grid_data' },
                { type: 'view', name: 'grids' },
                { type: 'view', name: 'tiles' }
            ]);
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/extension.test.js
*/
await (async function() {
console.error("\ntest-old.js - extension.test.js");
var sqlite3 = require('.');
var assert = require('assert');
var exists = require('fs').existsSync || require('path').existsSync;

/*

// disabled because this is not a generically safe test to run on all systems

var spatialite_ext = '/usr/local/lib/libspatialite.dylib';

await describe('loadExtension', async function(done) {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);
    });

    if (exists(spatialite_ext)) {
        await it('libspatialite', function(done) {
            db.loadExtension(spatialite_ext, done);
        });
    } else {
        await it('libspatialite');
    }
});

*/


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/fts-content.test.js
*/
await (async function() {
console.error("\ntest-old.js - fts-content.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('fts', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);
    });

    await it('should create a new fts4 table', function(done) {
        db.exec('CREATE VIRTUAL TABLE t1 USING fts4(content="", a, b, c);', done);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/interrupt.test.js
*/
await (async function() {
console.error("\ntest-old.js - interrupt.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('interrupt', async function() {
    await it('should interrupt queries', function(done) {
        var interrupted = false;
        var saved = null;

        var db = new sqlite3.Database(':memory:', function() {
            db.serialize();

            var setup = 'create table t (n int);';
            for (var i = 0; i < 8; i += 1) {
                setup += 'insert into t values (' + i + ');';
            }

            db.exec(setup, function(err) {
                if (err) {
                    return done(err);
                }

                var query = 'select last.n ' +
                    'from t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t as last';

                db.each(query, function(err) {
                    if (err) {
                        saved = err;
                    } else if (!interrupted) {
                        interrupted = true;
                        db.interrupt();
                    }
                });

                db.close(function() {
                    if (saved) {
                        assert.equal(saved.message, 'SQLITE_INTERRUPT: interrupted');
                        assert.equal(saved.errno, sqlite3.INTERRUPT);
                        assert.equal(saved.code, 'SQLITE_INTERRUPT');
                        done();
                    } else {
                        done(new Error('Completed query without error, but expected error'));
                    }
                });
            });
        });
    });

    await it('should throw if interrupt is called before open', function(done) {
        var db = new sqlite3.Database(':memory:');

        assert.throws(function() {
            db.interrupt();
        }, (/Database is not open/));

        db.close();
        done();
    });

    await it('should throw if interrupt is called after close', function(done) {
        var db = new sqlite3.Database(':memory:');

        db.close(function() {
            assert.throws(function() {
                db.interrupt();
            }, (/Database is not open/));

            done();
        });
    });

    await it('should throw if interrupt is called during close', function(done) {
        var db = new sqlite3.Database(':memory:', function() {
            db.close();
            assert.throws(function() {
                db.interrupt();
            }, (/Database is closing/));
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/issue-108.test.js
*/
await (async function() {
console.error("\ntest-old.js - issue-108.test.js");
var sqlite3 = require('.'),
    assert = require('assert');

await describe('buffer', async function() {
    var db;
    // await before(function() {
    // });

    await it('should insert blobs', function(done) {
        db = new sqlite3.Database(':memory:');
        db.serialize(function () {

            db.run("CREATE TABLE lorem (info BLOB)");
            var stmt = db.prepare("INSERT INTO lorem VALUES (?)");

            stmt.on('error', function (err) {
                throw err;
            });

// hack-test
            var buff = Buffer.alloc(2);
            stmt.run(buff);
            stmt.finalize();
        });

        db.close(done);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/json.test.js
*/
await (async function() {
console.error("\ntest-old.js - json.test.js");
var sqlite3 = require('.');

if( process.env.NODE_SQLITE3_JSON1 === 'no' ){
    await describe('json', async function() {
        await it(
            'skips JSON tests when --sqlite=/usr (or similar) is tested',
            function(){}
        );
    });
} else {
    await describe('json', async function() {
        var db;

        await before(function(done) {
            db = new sqlite3.Database(':memory:', done);
        });

        await it('should select JSON', function(done) {
            db.run('SELECT json(?)', JSON.stringify({ok:true}), done);
        });
    });
}


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/map.test.js
*/
await (async function() {
console.error("\ntest-old.js - map.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('map', async function() {
    await it('test Database#map() with two columns', function(done) {
        var count = 10;
        var inserted = 0;

        var db = new sqlite3.Database(':memory:');
        db.serialize(function() {
            db.run("CREATE TABLE foo (id INT, value TEXT)");

            var stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
            for (var i = 5; i < count; i++) {
                stmt.run(i, 'Value for ' + i, function(err) {
                    if (err) throw err;
                    inserted++;
                });
            }
            stmt.finalize();

            db.map("SELECT * FROM foo", function(err, map) {
                if (err) throw err;
                assert.deepEqual(map, { 5: 'Value for 5', 6: 'Value for 6', 7: 'Value for 7', 8: 'Value for 8', 9: 'Value for 9' });
                assert.equal(inserted, 5);
                done();
            });
        });
    });

    await it('test Database#map() with three columns', function(done) {
        var db = new sqlite3.Database(':memory:');

        var count = 10;
        var inserted = 0;

        db.serialize(function() {
            db.run("CREATE TABLE foo (id INT, value TEXT, other TEXT)");

            var stmt = db.prepare("INSERT INTO foo VALUES(?, ?, ?)");
            for (var i = 5; i < count; i++) {
                stmt.run(i, 'Value for ' + i, null, function(err) {
                    if (err) throw err;
                    inserted++;
                });
            }
            stmt.finalize();

            db.map("SELECT * FROM foo", function(err, map) {
                if (err) throw err;
                assert.deepEqual(map, {
                    5: { id: 5, value: 'Value for 5', other: null },
                    6: { id: 6, value: 'Value for 6', other: null },
                    7: { id: 7, value: 'Value for 7', other: null },
                    8: { id: 8, value: 'Value for 8', other: null },
                    9: { id: 9, value: 'Value for 9', other: null }
                });
                assert.equal(inserted, 5);
                done();
            });
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/named_columns.test.js
*/
await (async function() {
console.error("\ntest-old.js - named_columns.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('named columns', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);
    });

    await it('should create the table', function(done) {
        db.run("CREATE TABLE foo (txt TEXT, num INT)", done);
    });

    await it('should insert a value', function(done) {
        db.run("INSERT INTO foo VALUES($text, $id)", {
            $id: 1,
            $text: "Lorem Ipsum"
        }, done);
    });

    await it('should retrieve the values', function(done) {
        db.get("SELECT txt, num FROM foo ORDER BY num", function(err, row) {
            if (err) throw err;
            assert.equal(row.txt, "Lorem Ipsum");
            assert.equal(row.num, 1);
            done();
        });
    });

    await it('should be able to retrieve rowid of last inserted value', function(done) {
        db.get("SELECT last_insert_rowid() as last_id FROM foo", function(err, row) {
            if (err) throw err;
            assert.equal(row.last_id, 1);
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/named_params.test.js
*/
await (async function() {
console.error("\ntest-old.js - named_params.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('named parameters', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);
    });

    await it('should create the table', function(done) {
        db.run("CREATE TABLE foo (txt TEXT, num INT)", done);
    });

    await it('should insert a value with $ placeholders', function(done) {
        db.run("INSERT INTO foo VALUES($text, $id)", {
            $id: 1,
            $text: "Lorem Ipsum"
        }, done);
    });

    await it('should insert a value with : placeholders', function(done) {
        db.run("INSERT INTO foo VALUES(:text, :id)", {
            ':id': 2,
            ':text': "Dolor Sit Amet"
        }, done);
    });

    await it('should insert a value with @ placeholders', function(done) {
        db.run("INSERT INTO foo VALUES(@txt, @id)", {
            "@id": 3,
            "@txt": "Consectetur Adipiscing Elit"
        }, done);
    });

    await it('should insert a value with @ placeholders using an array', function(done) {
        db.run("INSERT INTO foo VALUES(@txt, @id)", [ 'Sed Do Eiusmod', 4 ], done);
    });

    await it('should insert a value with indexed placeholders', function(done) {
        db.run("INSERT INTO foo VALUES(?2, ?4)",
            [ null, 'Tempor Incididunt', null, 5 ], done);
    });

    await it('should insert a value with autoindexed placeholders', function(done) {
        db.run("INSERT INTO foo VALUES(?, ?)", {
            2: 6,
            1: "Ut Labore Et Dolore"
        }, done);
    });

    await it('should retrieve all inserted values', function(done) {
        db.all("SELECT txt, num FROM foo ORDER BY num", function(err, rows) {
            if (err) throw err;
            assert.equal(rows[0].txt, "Lorem Ipsum");
            assert.equal(rows[0].num, 1);
            assert.equal(rows[1].txt, "Dolor Sit Amet");
            assert.equal(rows[1].num, 2);
            assert.equal(rows[2].txt, "Consectetur Adipiscing Elit");
            assert.equal(rows[2].num, 3);
            assert.equal(rows[3].txt, "Sed Do Eiusmod");
            assert.equal(rows[3].num, 4);
            assert.equal(rows[4].txt, "Tempor Incididunt");
            assert.equal(rows[4].num, 5);
            assert.equal(rows[5].txt, "Ut Labore Et Dolore");
            assert.equal(rows[5].num, 6);
            done();
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/null_error.test.js
*/
await (async function() {
console.error("\ntest-old.js - null_error.test.js");
var sqlite3 = require('.');
var assert = require('assert');
// var helper = require('./support/helper');

await describe('null error', async function() {
    var filename = '.tmp/test/test_sqlite_ok_error.db';
    var db;

    await before(function(done) {
        helper.ensureExists('.tmp/test');
        helper.deleteFile(filename);
        db = new sqlite3.Database(filename, done);
    });

    await it('should create a table', function(done) {
        db.run("CREATE TABLE febp_data (leacode TEXT, leaname TEXT, state TEXT, postcode TEXT, fips TEXT, titleistim TEXT, ideastim TEXT, ideapool TEXT, ideapoolname TEXT, localebasis TEXT, localetype2 TEXT, version TEXT, leacount_2006 TEXT, ppexpend_2005 TEXT, ppexpend_2006 TEXT, ppexpend_2007 TEXT, ppexpend_2008 TEXT, ppexpendrank_2006 TEXT, ppexpendrank_2007 TEXT, ppexpendrank_2008 TEXT, rankppexpend_2005 TEXT, opbud_2004 TEXT, opbud_2006 TEXT, opbud_2007 TEXT, opbud_2008 TEXT, titlei_2004 TEXT, titlei_2006 TEXT, titlei_2007 TEXT, titlei_2008 TEXT, titlei_2009 TEXT, titlei_2010 TEXT, idea_2004 TEXT, idea_2005 TEXT, idea_2006 TEXT, idea_2007 TEXT, idea_2008 TEXT, idea_2009 TEXT, ideaest_2010 TEXT, impact_2007 TEXT, impact_2008 TEXT, impact_2009 TEXT, impact_2010 TEXT, fedrev_2006 TEXT, fedrev_2007 TEXT, fedrev_2008 TEXT, schonut_2006 TEXT, schonut_2007 TEXT, schomeal_2006 TEXT, schomeal_2007 TEXT, schoco_2006 TEXT, schocom_2007 TEXT, medicaid_2006 TEXT, medicaid_2007 TEXT, medicaid_2008 TEXT, cenpov_2004 TEXT, cenpov_2007 TEXT, cenpov_2008 TEXT, rankcenpov_2004 TEXT, rankcenpov_2007 TEXT, rankcenpov_2008 TEXT, enroll_2006 TEXT, enroll_2007 TEXT, enroll_2008 TEXT, white_2006 TEXT, white_2007 TEXT, white_2008 TEXT, afam_2006 TEXT, afam_2007 TEXT, afam_2008 TEXT, amin_2006 TEXT, amin_2007 TEXT, amin_2008 TEXT, asian_2006 TEXT, asian_2007 TEXT, asian_2008 TEXT, hisp_2006 TEXT, hisp_2007 TEXT, hisp_2008 TEXT, frpl_2006 TEXT, frpl_2007 TEXT, frpl_2008 TEXT, ell_2006 TEXT, ell_2007 TEXT, ell_2008 TEXT, sped_2006 TEXT, sped_2007 TEXT, sped_2008 TEXT, state4read_2005 TEXT, state4read_2006 TEXT, state4read_2007 TEXT, state4read_2008 TEXT, state4read_2009 TEXT, state4math_2005 TEXT, state4math_2006 TEXT, state4math_2007 TEXT, state4math_2008 TEXT, state4math_2009 TEXT, minor_2007 TEXT, minor_2008 TEXT, state8math_2006 TEXT, state8math_2007 TEXT, state8math_2008 TEXT, state8math_2009 TEXT, state8read_2006 TEXT, state8read_2007 TEXT, state8read_2008 TEXT, state8read_2009 TEXT, statehsmath_2006 TEXT, statehsmath_2007 TEXT, statehsmath_2008 TEXT, statehsmath_2009 TEXT, statehsread_2006 TEXT, statehsread_2007 TEXT, statehsread_2008 TEXT, statehsread_2009 TEXT)", done);
    });

    await it('should insert rows with lots of null values', function(done) {
        var stmt = db.prepare('INSERT INTO febp_data VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)', function(err) {
            if (err) throw err;

            for (var i = 0; i < 100; i++) {
                stmt.run([ '100005', 'Albertville City School District', 'ALABAMA', 'AL', '1', '856031', '753000', 'NULL', 'NULL', '6-Small Town', 'Town', 21, '130', '6624', '7140', '8731', '8520', '102', '88', '100', '94', '23352000', '27280000', '30106000', '33028000', '768478', '845886', '782696', '1096819', '1279663', '1168521', '561522', '657649', '684366', '687531', '710543', '727276', '726647', 'N/A', 'N/A', 'N/A', 'N/A', '986', '977', '1006', '1080250', '1202325', '1009962', '1109310', '70287', '93015', '14693.56', '13634.58', 'N/A', '0.230', '0.301', '0.268882175', '73', '26', '29', '3718', '3747', '3790', '2663', '2615', '2575', '75', '82', '89', '3', '2', '6', '11', '9', '8', '955', '1028', '1102', '1991', '2061', '2146', '649', '729', '770', '443', '278', '267', '0.860', '0.86', '0.8474', '0.84', '0.8235', '0.810', '0.84', '0.7729', '0.75', '0.7843', '1121', '1205', '0.74', '0.6862', '0.72', '0.7317', '0.78', '0.7766', '0.79', '0.7387', '0.84', '0.9255', '0.86', '0.9302', '0.88', '0.9308', '0.84', '0.8605' ]);
            }

            stmt.finalize(function(err) {
                if (err) throw err;
                done();
            });
        });
    });

    await it('should have created the database', function() {
        assert.fileExists(filename);
    });

    after(function() {
        helper.deleteFile(filename);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/open_close.test.js
*/
await (async function() {
console.error("\ntest-old.js - open_close.test.js");
var sqlite3 = require('.');
var assert = require('assert');
var fs = require('fs');
// var helper = require('./support/helper');

await describe('open/close', async function() {
    await before(function() {
        helper.ensureExists('.tmp/test');
    });

    await describe('open and close non-existant database', async function() {
        await before(function() {
            helper.deleteFile('.tmp/test/test_create.db');
        });

        var db;
        await it('should open the database', function(done) {
            db = new sqlite3.Database('.tmp/test/test_create.db', done);
        });

        await it('should close the database', function(done) {
            db.close(done);
        });

        await it('should have created the file', function() {
            assert.fileExists('.tmp/test/test_create.db');
        });

        after(function() {
            helper.deleteFile('.tmp/test/test_create.db');
        });
    });

    await describe('open and close non-existant shared database', async function() {
        await before(function() {
            helper.deleteFile('.tmp/test/test_create_shared.db');
        });

        var db;
        await it('should open the database', function(done) {
            db = new sqlite3.Database('file:./.tmp/test/test_create_shared.db', sqlite3.OPEN_URI | sqlite3.OPEN_SHAREDCACHE | sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, done);
        });

        await it('should close the database', function(done) {
            db.close(done);
        });

        await it('should have created the file', function() {
            assert.fileExists('.tmp/test/test_create_shared.db');
        });

        after(function() {
            helper.deleteFile('.tmp/test/test_create_shared.db');
        });
    });


// hack-test
    describe('open and close shared memory database', async function() {

        var db1;
        var db2;

        await it('should open the first database', function(done) {
            db1 = new sqlite3.Database('file:./.tmp/test/test_memory.db?mode=memory', sqlite3.OPEN_URI | sqlite3.OPEN_SHAREDCACHE | sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, done);
        });

        await it('should open the second database', function(done) {
            db2 = new sqlite3.Database('file:./.tmp/test/test_memory.db?mode=memory', sqlite3.OPEN_URI | sqlite3.OPEN_SHAREDCACHE | sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, done);
        });

        await it('first database should set the user_version', function(done) {
            db1.exec('PRAGMA user_version=42', done);
        });

        await it('second database should get the user_version', function(done) {
            db2.get('PRAGMA user_version', function(err, row) {
                if (err) throw err;
                assert.equal(row.user_version, 42);
                done();
            });
        });

        await it('should close the first database', function(done) {
            db1.close(done);
        });

        await it('should close the second database', function(done) {
            db2.close(done);
        });
    });

    await it('should not be unable to open an inaccessible database', function(done) {
        // NOTE: test assumes that the user is not allowed to create new files
        // in /usr/bin.
        var db = new sqlite3.Database('/.tmp/test/directory-does-not-exist/test.db', function(err) {
            if (err && err.errno === sqlite3.CANTOPEN) {
                done();
            } else if (err) {
                done(err);
            } else {
                done('Opened database that should be inaccessible');
            }
        });
    });


    await describe('creating database without create flag', async function() {
        await before(function() {
            helper.deleteFile('.tmp/test/test_readonly.db');
        });

        await it('should fail to open the database', function(done) {
            new sqlite3.Database('tmp/test_readonly.db', sqlite3.OPEN_READONLY, function(err) {
                if (err && err.errno === sqlite3.CANTOPEN) {
                    done();
                } else if (err) {
                    done(err);
                } else {
                    done('Created database without create flag');
                }
            });
        });

        await it('should not have created the file', function() {
            assert.fileDoesNotExist('.tmp/test/test_readonly.db');
        });

        after(function() {
            helper.deleteFile('.tmp/test/test_readonly.db');
        });
    });

    await describe('open and close memory database queuing', async function() {
        var db;
        await it('should open the database', function(done) {
            db = new sqlite3.Database(':memory:', done);
        });

        await it('should close the database', function(done) {
            db.close(done);
        });

        await it('shouldn\'t close the database again', function(done) {
            db.close(function(err) {
                assert.ok(err, 'No error object received on second close');
                assert.ok(err.errno === sqlite3.MISUSE);
                done();
            });
        });
    });

    await describe('closing with unfinalized statements', async function(done) {
        var completed = false;
        var completedSecond = false;
        var closed = false;

        var db;
        await before(function() {
            db = new sqlite3.Database(':memory:', done);
        });

        await it('should create a table', function(done) {
            db.run("CREATE TABLE foo (id INT, num INT)", done);
        });

        var stmt;
        await it('should prepare/run a statement', function(done) {
            stmt = db.prepare('INSERT INTO foo VALUES (?, ?)');
            stmt.run(1, 2, done);
        });

        await it('should fail to close the database', function(done) {
            db.close(function(err) {
                assert.ok(err.message,
                    "SQLITE_BUSY: unable to close due to unfinalised statements");
                done();
            });
        });

        await it('should succeed to close the database after finalizing', function(done) {
            stmt.run(3, 4, function() {
                stmt.finalize();
                db.close(done);
            });
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/other_objects.test.js
*/
await (async function() {
console.error("\ntest-old.js - other_objects.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('data types', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:');
        db.run("CREATE TABLE txt_table (txt TEXT)");
        db.run("CREATE TABLE int_table (int INTEGER)");
        db.run("CREATE TABLE flt_table (flt FLOAT)");
        db.wait(done);
    });

    beforeEach(function(done) {
        db.exec('DELETE FROM txt_table; DELETE FROM int_table; DELETE FROM flt_table;', done);
    });

    await it('should serialize Date()', function(done) {
        var date = new Date();
        db.run("INSERT INTO int_table VALUES(?)", date, function (err) {
            if (err) throw err;
            db.get("SELECT int FROM int_table", function(err, row) {
                if (err) throw err;
                assert.equal(row.int, +date);
                done();
            });
        });
    });

    await it('should serialize RegExp()', function(done) {
        var regexp = /^f\noo/;
        db.run("INSERT INTO txt_table VALUES(?)", regexp, function (err) {
            if (err) throw err;
            db.get("SELECT txt FROM txt_table", function(err, row) {
                if (err) throw err;
                assert.equal(row.txt, String(regexp));
                done();
            });
        });
    });

// hack-test
    var list = [
        4294967296.249,
        Math.PI,
        3924729304762836.5,
        new Date().valueOf(),
        912667.394828365,
        2.3948728634826374e+83,
        9.293476892934982e+300,
        Infinity,
        -9.293476892934982e+300,
        -2.3948728634826374e+83,
        -Infinity
// hack-test
    ];
    while (true) {
        var flt = list.shift();
        if (!flt) {
            break;
        }
        await it('should serialize float ' + flt, function(done) {
            db.run("INSERT INTO flt_table VALUES(?)", flt, function (err) {
                if (err) throw err;
                db.get("SELECT flt FROM flt_table", function(err, row) {
                    if (err) throw err;
                    assert.equal(row.flt, flt);
                    done();
                });
            });
        });
// hack-test
    }

    var list = [
        4294967299,
        3924729304762836,
        new Date().valueOf(),
        2.3948728634826374e+83,
        9.293476892934982e+300,
        Infinity,
        -9.293476892934982e+300,
        -2.3948728634826374e+83,
        -Infinity
// hack-test
    ];
    while (true) {
        var integer = list.shift();
        if (!integer) {
            break;
        }
        await it('should serialize integer ' + integer, function(done) {
            db.run("INSERT INTO int_table VALUES(?)", integer, function (err) {
                if (err) throw err;
                db.get("SELECT int AS integer FROM int_table", function(err, row) {
                    if (err) throw err;
                    assert.equal(row.integer, integer);
                    done();
                });
            });
        });
// hack-test
    }
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/parallel_insert.test.js
*/
await (async function() {
console.error("\ntest-old.js - parallel_insert.test.js");
var sqlite3 = require('.');
var assert = require('assert');
// var helper = require('./support/helper');

await describe('parallel', async function() {
    var db;
    await before(function(done) {
        helper.deleteFile('.tmp/test/test_parallel_inserts.db');
        helper.ensureExists('.tmp/test');
        db = new sqlite3.Database('.tmp/test/test_parallel_inserts.db', done);
    });

    var columns = [];
    for (var i = 0; i < 128; i++) {
        columns.push('id' + i);
    }

    await it('should create the table', function(done) {
        db.run("CREATE TABLE foo (" + columns + ")", done);
    });

    await it('should insert in parallel', function(done) {
        for (var i = 0; i < 1000; i++) {
            for (var values = [], j = 0; j < columns.length; j++) {
                values.push(i * j);
            }
            db.run("INSERT INTO foo VALUES (" + values + ")");
        }

        db.wait(done);
    });

    await it('should close the database', function(done) {
        db.close(done);
    });

    await it('should verify that the database exists', function() {
        assert.fileExists('.tmp/test/test_parallel_inserts.db');
    });

    after(function() {
        helper.deleteFile('.tmp/test/test_parallel_inserts.db');
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/prepare.test.js
*/
await (async function() {
console.error("\ntest-old.js - prepare.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('prepare', async function() {
    await describe('invalid SQL', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database(':memory:', done); });

        var stmt;
        await it('should fail preparing a statement with invalid SQL', function(done) {
            stmt = db.prepare('CRATE TALE foo text bar)', function(err, statement) {
                if (err && err.errno == sqlite3.ERROR &&
                    err.message === 'SQLITE_ERROR: near "CRATE": syntax error') {
                    done();
                }
                else throw err;
            });
        });

        after(function(done) { db.close(done); });
    });

    await describe('simple prepared statement', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database(':memory:', done); });

        await it('should prepare, run and finalize the statement', function(done) {
            db.prepare("CREATE TABLE foo (text bar)")
                .run()
                .finalize(done);
        });

        after(function(done) { db.close(done); });
    });

    await describe('inserting and retrieving rows', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database(':memory:', done); });

        var inserted = 0;
        var retrieved = 0;

        // We insert and retrieve that many rows.
        var count = 1000;

        await it('should create the table', function(done) {
            db.prepare("CREATE TABLE foo (txt text, num int, flt float, blb blob)").run().finalize(done);
        });

        await it('should insert ' + count + ' rows', function(done) {
            for (var i = 0; i < count; i++) {
                db.prepare("INSERT INTO foo VALUES(?, ?, ?, ?)").run(
                    'String ' + i,
                    i,
                    i * Math.PI,
                    // null (SQLite sets this implicitly)
                    function(err) {
                        if (err) throw err;
                        inserted++;
                    }
                ).finalize(function(err) {
                    if (err) throw err;
                    if (inserted == count) done();
                });
            }
        });

        await it('should prepare a statement and run it ' + (count + 5) + ' times', function(done) {
            var stmt = db.prepare("SELECT txt, num, flt, blb FROM foo ORDER BY num", function(err) {
                if (err) throw err;
                assert.equal(stmt.sql, 'SELECT txt, num, flt, blb FROM foo ORDER BY num');
            });

            for (var i = 0; i < count + 5; i++) (function(i) {
                stmt.get(function(err, row) {
                    if (err) throw err;

                    if (retrieved >= 1000) {
                        assert.equal(row, undefined);
                    } else {
                        assert.equal(row.txt, 'String ' + i);
                        assert.equal(row.num, i);
                        assert.equal(row.flt, i * Math.PI);
                        assert.equal(row.blb, null);
                    }

                    retrieved++;
                });
            })(i);

            stmt.finalize(done);
        });

        await it('should have retrieved ' + (count + 5) + ' rows', function() {
            assert.equal(count + 5, retrieved, "Didn't retrieve all rows");
        });


        after(function(done) { db.close(done); });
    });

    await describe('inserting with accidental undefined', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database(':memory:', done); });

        var inserted = 0;
        var retrieved = 0;

        await it('should create the table', function(done) {
            db.prepare("CREATE TABLE foo (num int)").run().finalize(done);
        });

        await it('should insert two rows', function(done) {
            db.prepare('INSERT INTO foo VALUES(4)').run(function(err) {
                if (err) throw err;
                inserted++;
            }).run(undefined, function (err) {
                // The second time we pass undefined as a parameter. This is
                // a mistake, but it should either throw an error or be ignored,
                // not silently fail to run the statement.
                if (err) throw err;
                inserted++;
            }).finalize(function(err) {
                if (err) throw err;
                if (inserted == 2) done();
            });
        });

        await it('should retrieve the data', function(done) {
            var stmt = db.prepare("SELECT num FROM foo", function(err) {
                if (err) throw err;
            });

            for (var i = 0; i < 2; i++) (function(i) {
                stmt.get(function(err, row) {
                    if (err) throw err;
                    assert(row);
                    assert.equal(row.num, 4);
                    retrieved++;
                });
            })(i);

            stmt.finalize(done);
        });

        await it('should have retrieved two rows', function() {
            assert.equal(2, retrieved, "Didn't retrieve all rows");
        });

        after(function(done) { db.close(done); });
    });

    await describe('retrieving reset() function', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done); });

        var retrieved = 0;

        await it('should retrieve the same row over and over again', function(done) {
            var stmt = db.prepare("SELECT txt, num, flt, blb FROM foo ORDER BY num");
            for (var i = 0; i < 10; i++) {
                stmt.reset();
                stmt.get(function(err, row) {
                    if (err) throw err;
                    assert.equal(row.txt, 'String 0');
                    assert.equal(row.num, 0);
                    assert.equal(row.flt, 0.0);
                    assert.equal(row.blb, null);
                    retrieved++;
                });
            }
            stmt.finalize(done);
        });

        await it('should have retrieved 10 rows', function() {
            assert.equal(10, retrieved, "Didn't retrieve all rows");
        });

        after(function(done) { db.close(done); });
    });

    await describe('multiple get() parameter binding', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done); });

        var retrieved = 0;

        await it('should retrieve particular rows', function(done) {
            var stmt = db.prepare("SELECT txt, num, flt, blb FROM foo WHERE num = ?");

            for (var i = 0; i < 10; i++) (function(i) {
                stmt.get(i * 10 + 1, function(err, row) {
                    if (err) throw err;
                    var val = i * 10 + 1;
                    assert.equal(row.txt, 'String ' + val);
                    assert.equal(row.num, val);
                    assert.equal(row.flt, val * Math.PI);
                    assert.equal(row.blb, null);
                    retrieved++;
                });
            })(i);

            stmt.finalize(done);
        });

        await it('should have retrieved 10 rows', function() {
            assert.equal(10, retrieved, "Didn't retrieve all rows");
        });

        after(function(done) { db.close(done); });
    });

    await describe('prepare() parameter binding', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done); });

        var retrieved = 0;

        await it('should retrieve particular rows', function(done) {
            db.prepare("SELECT txt, num, flt, blb FROM foo WHERE num = ? AND txt = ?", 10, 'String 10')
                .get(function(err, row) {
                    if (err) throw err;
                    assert.equal(row.txt, 'String 10');
                    assert.equal(row.num, 10);
                    assert.equal(row.flt, 10 * Math.PI);
                    assert.equal(row.blb, null);
                    retrieved++;
                })
                .finalize(done);
        });

        await it('should have retrieved 1 row', function() {
            assert.equal(1, retrieved, "Didn't retrieve all rows");
        });

        after(function(done) { db.close(done); });
    });

    await describe('all()', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done); });

        var retrieved = 0;
        var count = 1000;

        await it('should retrieve particular rows', function(done) {
            db.prepare("SELECT txt, num, flt, blb FROM foo WHERE num < ? ORDER BY num", count)
                .all(function(err, rows) {
                    if (err) throw err;
                    for (var i = 0; i < rows.length; i++) {
                        assert.equal(rows[i].txt, 'String ' + i);
                        assert.equal(rows[i].num, i);
                        assert.equal(rows[i].flt, i * Math.PI);
                        assert.equal(rows[i].blb, null);
                        retrieved++;
                    }
                })
                .finalize(done);
        });

        await it('should have retrieved all rows', function() {
            assert.equal(count, retrieved, "Didn't retrieve all rows");
        });

        after(function(done) { db.close(done); });
    });

    await describe('all()', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done); });

        await it('should retrieve particular rows', function(done) {
           db.prepare("SELECT txt, num, flt, blb FROM foo WHERE num > 5000")
                .all(function(err, rows) {
                    if (err) throw err;
                    assert.ok(rows.length === 0);
                })
                .finalize(done);
        });

        after(function(done) { db.close(done); });
    });

    await describe('high concurrency', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database(':memory:', done); });

        function randomString() {
            var str = '';
            for (var i = Math.random() * 300; i > 0; i--) {
                str += String.fromCharCode(Math.floor(Math.random() * 256));
            }
            return str;
        }

        // Generate random data.
        var data = [];
        var length = Math.floor(Math.random() * 1000) + 200;
        for (var i = 0; i < length; i++) {
            data.push([ randomString(), i, i * Math.random(), null ]);
        }

        var inserted = 0;
        var retrieved = 0;

        await it('should create the table', function(done) {
            db.prepare("CREATE TABLE foo (txt text, num int, flt float, blb blob)").run().finalize(done);
        });

        await it('should insert all values', function(done) {
            for (var i = 0; i < data.length; i++) {
                var stmt = db.prepare("INSERT INTO foo VALUES(?, ?, ?, ?)");
                stmt.run(data[i][0], data[i][1], data[i][2], data[i][3], function(err) {
                    if (err) throw err;
                    inserted++;
                }).finalize(function(err) {
                    if (err) throw err;
                    if (inserted == data.length) done();
                });
            }
        });

        await it('should retrieve all values', function(done) {
            db.prepare("SELECT txt, num, flt, blb FROM foo")
                .all(function(err, rows) {
                    if (err) throw err;

                    for (var i = 0; i < rows.length; i++) {
                        assert.ok(data[rows[i].num] !== true);

                        assert.equal(rows[i].txt, data[rows[i].num][0]);
                        assert.equal(rows[i].num, data[rows[i].num][1]);
                        assert.equal(rows[i].flt, data[rows[i].num][2]);
                        assert.equal(rows[i].blb, data[rows[i].num][3]);

                        // Mark the data row as already retrieved.
                        data[rows[i].num] = true;
                        retrieved++;
                    }

                    assert.equal(retrieved, data.length);
                    assert.equal(retrieved, inserted);
                })
                .finalize(done);
        });

        after(function(done) { db.close(done); });
    });


    await describe('test Database#get()', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database('test-old-prepare.db', sqlite3.OPEN_READONLY, done); });

        var retrieved = 0;

        await it('should get a row', function(done) {
            db.get("SELECT txt, num, flt, blb FROM foo WHERE num = ? AND txt = ?", 10, 'String 10', function(err, row) {
                if (err) throw err;
                assert.equal(row.txt, 'String 10');
                assert.equal(row.num, 10);
                assert.equal(row.flt, 10 * Math.PI);
                assert.equal(row.blb, null);
                retrieved++;
                done();
            });
        });

        await it('should have retrieved all rows', function() {
            assert.equal(1, retrieved, "Didn't retrieve all rows");
        });

        after(function(done) { db.close(done); });
    });

    await describe('Database#run() and Database#all()', async function() {
        var db;
        await before(function(done) { db = new sqlite3.Database(':memory:', done); });

        var inserted = 0;
        var retrieved = 0;

        // We insert and retrieve that many rows.
        var count = 1000;

        await it('should create the table', function(done) {
            db.run("CREATE TABLE foo (txt text, num int, flt float, blb blob)", done);
        });

        await it('should insert ' + count + ' rows', function(done) {
            for (var i = 0; i < count; i++) {
                db.run("INSERT INTO foo VALUES(?, ?, ?, ?)",
                    'String ' + i,
                    i,
                    i * Math.PI,
                    // null (SQLite sets this implicitly)
                    function(err) {
                        if (err) throw err;
                        inserted++;
                        if (inserted == count) done();
                    }
                );
            }
        });

        await it('should retrieve all rows', function(done) {
            db.all("SELECT txt, num, flt, blb FROM foo ORDER BY num", function(err, rows) {
                if (err) throw err;
                for (var i = 0; i < rows.length; i++) {
                    assert.equal(rows[i].txt, 'String ' + i);
                    assert.equal(rows[i].num, i);
                    assert.equal(rows[i].flt, i * Math.PI);
                    assert.equal(rows[i].blb, null);
                    retrieved++;
                }

                assert.equal(retrieved, count);
                assert.equal(retrieved, inserted);

                done();
            });
        });

        after(function(done) { db.close(done); });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/profile.test.js
*/
await (async function() {
console.error("\ntest-old.js - profile.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('profiling', async function() {
    var create = false;
    var select = false;

    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:', done);

        db.on('profile', function(sql, nsecs) {
            assert.ok(typeof nsecs === "number");
            if (sql.match(/^SELECT/)) {
                assert.ok(!select);
                assert.equal(sql, "SELECT * FROM foo");
                console.log('profile select');
                select = true;
            }
            else if (sql.match(/^CREATE/)) {
                assert.ok(!create);
                assert.equal(sql, "CREATE TABLE foo (id int)");
                create = true;
            }
            else {
                assert.ok(false);
            }
        });
    });

    await it('should profile a create table', function(done) {
        assert.ok(!create);
        db.run("CREATE TABLE foo (id int)", function(err) {
            if (err) throw err;
            setImmediate(function() {
                assert.ok(create);
                done();
            });
        });
    });


    await it('should profile a select', function(done) {
        assert.ok(!select);
        db.run("SELECT * FROM foo", function(err) {
            if (err) throw err;
            setImmediate(function() {
                assert.ok(select);
                done();
            }, 0);
        });
    });

    after(function(done) {
        db.close(done);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/rerun.test.js
*/
await (async function() {
console.error("\ntest-old.js - rerun.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('rerunning statements', async function() {
    var db;
    await before(function(done) { db = new sqlite3.Database(':memory:', done); });

    var count = 10;
    var inserted = 0;
    var retrieved = 0;

    await it('should create the table', function(done) {
        db.run("CREATE TABLE foo (id int)", done);
    });

    await it('should insert repeatedly, reusing the same statement', function(done) {
        var stmt = db.prepare("INSERT INTO foo VALUES(?)");
        for (var i = 5; i < count; i++) {
            stmt.run(i, function(err) {
                if (err) throw err;
                inserted++;
            });
        }
        stmt.finalize(done);
    });

    await it('should retrieve repeatedly, resuing the same statement', function(done) {
        var collected = [];
        var stmt = db.prepare("SELECT id FROM foo WHERE id = ?");
        for (var i = 0; i < count; i++) {
            stmt.get(i, function(err, row) {
                if (err) throw err;
                if (row) collected.push(row);
            });
        }
        stmt.finalize(function(err) {
            if (err) throw err;
            retrieved += collected.length;
            assert.deepEqual(collected, [ { id: 5 }, { id: 6 }, { id: 7 }, { id: 8 }, { id: 9 } ]);
            done();
        });
    });

    await it('should have inserted and retrieved the right amount', function() {
        assert.equal(inserted, 5);
        assert.equal(retrieved, 5);
    });

    after(function(done) { db.close(done); });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/scheduling.test.js
*/
await (async function() {
console.error("\ntest-old.js - scheduling.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('scheduling', async function() {
    await it('scheduling after the database was closed', function(done) {
        var db = new sqlite3.Database(':memory:');
        db.on('error', function(err) {
            assert.ok(err.message && err.message.indexOf("SQLITE_MISUSE: Database handle is closed") > -1);
            done();
        });

        db.close();
        db.run("CREATE TABLE foo (id int)");
    });


    await it('scheduling a query with callback after the database was closed', function(done) {
        var db = new sqlite3.Database(':memory:');
        db.on('error', function(err) {
            assert.ok(false, 'Event was accidentally triggered');
        });

        db.close();
        db.run("CREATE TABLE foo (id int)", function(err) {
            assert.ok(err.message && err.message.indexOf("SQLITE_MISUSE: Database handle is closed") > -1);
            done();
        });
    });

    await it('running a query after the database was closed', function(done) {
        var db = new sqlite3.Database(':memory:');

        var stmt = db.prepare("SELECT * FROM sqlite_master", function(err) {
            if (err) throw err;
            db.close(function(err) {
                assert.ok(err);
                assert.ok(err.message && err.message.indexOf("SQLITE_BUSY: unable to close due to") > -1);

                // Running a statement now should not fail.
                stmt.run(done);
            });
        });
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/serialization.test.js
*/
await (async function() {
console.error("\ntest-old.js - serialization.test.js");
var sqlite3 = require('.');
var assert = require('assert');


await describe('serialize() and parallelize()', async function() {
    var db;
    await before(function(done) { db = new sqlite3.Database(':memory:', done); });

    var inserted1 = 0;
    var inserted2 = 0;
    var retrieved = 0;

    var count = 1000;

    await it('should toggle', function(done) {
        db.serialize();
        db.run("CREATE TABLE foo (txt text, num int, flt float, blb blob)");
        db.parallelize(done);
    });

    await it('should insert rows', function() {
        var stmt1 = db.prepare("INSERT INTO foo VALUES(?, ?, ?, ?)");
        var stmt2 = db.prepare("INSERT INTO foo VALUES(?, ?, ?, ?)");
        for (var i = 0; i < count; i++) {
            // Interleaved inserts with two statements.
            stmt1.run('String ' + i, i, i * Math.PI, function(err) {
                if (err) throw err;
                inserted1++;
            });
            i++;
            stmt2.run('String ' + i, i, i * Math.PI, function(err) {
                if (err) throw err;
                inserted2++;
            });
        }
        stmt1.finalize();
        stmt2.finalize();
    });

    await it('should have inserted all the rows after synchronizing with serialize()', function(done) {
        db.serialize();
        db.all("SELECT txt, num, flt, blb FROM foo ORDER BY num", function(err, rows) {
            if (err) throw err;
            for (var i = 0; i < rows.length; i++) {
                assert.equal(rows[i].txt, 'String ' + i);
                assert.equal(rows[i].num, i);
                assert.equal(rows[i].flt, i * Math.PI);
                assert.equal(rows[i].blb, null);
                retrieved++;
            }

            assert.equal(count, inserted1 + inserted2, "Didn't insert all rows");
            assert.equal(count, retrieved, "Didn't retrieve all rows");
            done();
        });
    });

    after(function(done) { db.close(done); });
});

await describe('serialize(fn)', async function() {
    var db;
    await before(function(done) { db = new sqlite3.Database(':memory:', done); });

    var inserted = 0;
    var retrieved = 0;

    var count = 1000;

    await it('should call the callback', function(done) {
        db.serialize(function() {
            db.run("CREATE TABLE foo (txt text, num int, flt float, blb blob)");

            var stmt = db.prepare("INSERT INTO foo VALUES(?, ?, ?, ?)");
            for (var i = 0; i < count; i++) {
                stmt.run('String ' + i, i, i * Math.PI, function(err) {
                    if (err) throw err;
                    inserted++;
                });
            }
            stmt.finalize();

            db.all("SELECT txt, num, flt, blb FROM foo ORDER BY num", function(err, rows) {
                if (err) throw err;
                for (var i = 0; i < rows.length; i++) {
                    assert.equal(rows[i].txt, 'String ' + i);
                    assert.equal(rows[i].num, i);
                    assert.equal(rows[i].flt, i * Math.PI);
                    assert.equal(rows[i].blb, null);
                    retrieved++;
                }
                done();
            });
        });
    });


    await it('should have inserted and retrieved all rows', function() {
        assert.equal(count, inserted, "Didn't insert all rows");
        assert.equal(count, retrieved, "Didn't retrieve all rows");
    });

    after(function(done) { db.close(done); });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/trace.test.js
*/
await (async function() {
console.error("\ntest-old.js - trace.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('tracing', async function() {
    await it('Database tracing', function(done) {
        var db = new sqlite3.Database(':memory:');
        var create = false;
        var select = false;

        db.on('trace', function(sql) {
            if (sql.match(/^SELECT/)) {
                assert.ok(!select);
                assert.equal(sql, "SELECT * FROM foo");
                select = true;
            }
            else if (sql.match(/^CREATE/)) {
                assert.ok(!create);
                assert.equal(sql, "CREATE TABLE foo (id int)");
                create = true;
            }
            else {
                assert.ok(false);
            }
        });

        db.serialize(function() {
            db.run("CREATE TABLE foo (id int)");
            db.run("SELECT * FROM foo");
        });

        db.close(function(err) {
            if (err) throw err;
            assert.ok(create);
            assert.ok(select);
            done();
        });
    });


    await it('test disabling tracing #1', function(done) {
        var db = new sqlite3.Database(':memory:');

        db.on('trace', function(sql) {});
        db.removeAllListeners('trace');
        db._events['trace'] = function(sql) {
            assert.ok(false);
        };

        db.run("CREATE TABLE foo (id int)");
        db.close(done);
    });


    await it('test disabling tracing #2', function(done) {
        var db = new sqlite3.Database(':memory:');

        var trace = function(sql) {};
        db.on('trace', trace);
        db.removeListener('trace', trace);
        db._events['trace'] = function(sql) {
            assert.ok(false);
        };

        db.run("CREATE TABLE foo (id int)");
        db.close(done);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/unicode.test.js
*/
await (async function() {
console.error("\ntest-old.js - unicode.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('unicode', async function() {
    var first_values = [],
        trailing_values = [],
        chars = [],
        subranges = new Array(2),
        len = subranges.length,
        db,
        i;

    await before(function(done) { db = new sqlite3.Database(':memory:', done); });

    for (i = 0x20; i < 0x80; i++) {
        first_values.push(i);
    }

    for (i = 0xc2; i < 0xf0; i++) {
        first_values.push(i);
    }

    for (i = 0x80; i < 0xc0; i++) {
        trailing_values.push(i);
    }

    for (i = 0; i < len; i++) {
        subranges[i] = [];
    }

    for (i = 0xa0; i < 0xc0; i++) {
        subranges[0].push(i);
    }

    for (i = 0x80; i < 0xa0; i++) {
        subranges[1].push(i);
    }

    function random_choice(arr) {
        return arr[Math.random() * arr.length | 0];
    }

    function random_utf8() {
        var first = random_choice(first_values);

        if (first < 0x80) {
            return String.fromCharCode(first);
        } else if (first < 0xe0) {
            return String.fromCharCode((first & 0x1f) << 0x6 | random_choice(trailing_values) & 0x3f);
        } else if (first == 0xe0) {
             return String.fromCharCode(((first & 0xf) << 0xc) | ((random_choice(subranges[0]) & 0x3f) << 6) | random_choice(trailing_values) & 0x3f);
        } else if (first == 0xed) {
            return String.fromCharCode(((first & 0xf) << 0xc) | ((random_choice(subranges[1]) & 0x3f) << 6) | random_choice(trailing_values) & 0x3f);
        } else if (first < 0xf0) {
            return String.fromCharCode(((first & 0xf) << 0xc) | ((random_choice(trailing_values) & 0x3f) << 6) | random_choice(trailing_values) & 0x3f);
        }
    }

    function randomString() {
        var str = '',
        i;

        for (i = Math.random() * 300; i > 0; i--) {
            str += random_utf8();
        }

        return str;
    }


        // Generate random data.
    var data = [];
    var length = Math.floor(Math.random() * 1000) + 200;
    for (var i = 0; i < length; i++) {
        data.push(randomString());
    }

    var inserted = 0;
    var retrieved = 0;

    await it('should create the table', function(done) {
        db.run("CREATE TABLE foo (id int, txt text)", done);
    });

    await it('should insert all values', function(done) {
        var stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
        for (var i = 0; i < data.length; i++) {
            stmt.run(i, data[i], function(err) {
                if (err) throw err;
                inserted++;
            });
        }
        stmt.finalize(done);
    });

    await it('should retrieve all values', function(done) {
        db.all("SELECT txt FROM foo ORDER BY id", function(err, rows) {
            if (err) throw err;

            for (var i = 0; i < rows.length; i++) {
                assert.equal(rows[i].txt, data[i]);
                retrieved++;
            }
            done();
        });
    });

    await it('should have inserted and retrieved the correct amount', function() {
        assert.equal(inserted, length);
        assert.equal(retrieved, length);
    });

    after(function(done) { db.close(done); });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/upsert.test.js
*/
await (async function() {
console.error("\ntest-old.js - upsert.test.js");
var sqlite3 = require('.');
var assert = require('assert');

await describe('query properties', async function() {
    var db;
    await before(function(done) {
        db = new sqlite3.Database(':memory:');
        db.run("CREATE TABLE foo (id INT PRIMARY KEY, count INT)", done);
    });

    await it('should upsert', function(done) {
        var stmt = db.prepare("INSERT INTO foo VALUES(?, ?)");
        stmt.run(1, 1, function(err) { // insert 1
            if (err) throw err;
            var upsert_stmt = db.prepare("INSERT INTO foo VALUES(?, ?) ON CONFLICT (id) DO UPDATE SET count = count + excluded.count");
            upsert_stmt.run(1, 2, function(err) { // add 2
                if (err) throw err;
                var select_stmt = db.prepare("SELECT count FROM foo WHERE id = ?");
                select_stmt.get(1, function(err, row) {
                    if (err) throw err;
                    assert.equal(row.count, 3); // equals 3
                });
            })
        });
        db.wait(done);
    });
});


}());
/*
file https://github.com/mapbox/node-sqlite3/blob/v5.0.2/test/verbose.test.js
*/
await (async function() {
console.error("\ntest-old.js - verbose.test.js");
var sqlite3 = require('.');
var assert = require('assert');

var invalid_sql = 'update non_existent_table set id=1';

var originalMethods = {
    Database: {},
    Statement: {},
};

function backupOriginalMethods() {
    for (var obj in originalMethods) {
        for (var attr in sqlite3[obj].prototype) {
            originalMethods[obj][attr] = sqlite3[obj].prototype[attr];
        }
    }
}

function resetVerbose() {
    for (var obj in originalMethods) {
        for (var attr in originalMethods[obj]) {
            sqlite3[obj].prototype[attr] = originalMethods[obj][attr];
        }
    }
}

await describe('verbose', async function() {
    await it('Shoud add trace info to error when verbose is called', function(done) {
        var db = new sqlite3.Database(':memory:');
        backupOriginalMethods();
        sqlite3.verbose();

        db.run(invalid_sql, function(err) {
            assert(err instanceof Error);

            assert(
                err.stack.indexOf(`Database#run('${invalid_sql}'`) > -1,
                `Stack shoud contain trace info, stack = ${err.stack}`
            );

            done();
            resetVerbose();
        });
    });

    await it('Shoud not add trace info to error when verbose is not called', function(done) {
        var db = new sqlite3.Database(':memory:');

        db.run(invalid_sql, function(err) {
            assert(err instanceof Error);

            assert(
                err.stack.indexOf(invalid_sql) === -1,
                `Stack shoud not contain trace info, stack = ${err.stack}`
            );

            done();
        });
    });
});


}());
}());
/*
file none
*/
/*jslint-enable*/
