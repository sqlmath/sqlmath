/*jslint beta, debug, name, node*/
"use strict";


/*
file debugInlne.js
*/
(function () {
    let consoleError = function () {
        return;
    };
    function debugInline(...argList) {
// this function will both print <argList> to stderr and return <argList>[0]
        consoleError("\n\ndebugInline");
        consoleError(...argList);
        consoleError("\n");
        return argList[0];
    }
    // coverage-hack
    debugInline();
    consoleError = console.error;
    globalThis.debugInline = globalThis.debugInline || debugInline;
}());


/*
file helper.js
*/
function noop(val) {
// this function will do nothing except return val
    return val;
}


/*
file cbor.js
*/
/*jslint-disable*/
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016 Patrick Gansterer <paroga@paroga.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

(function(global, undefined) { "use strict";
var POW_2_24 = 5.960464477539063e-8,
    POW_2_32 = 4294967296,
    POW_2_53 = 9007199254740992;

function encode(value) {
  var data = new ArrayBuffer(256);
  var dataView = new DataView(data);
  var lastLength;
  var offset = 0;

  function prepareWrite(length) {
    var newByteLength = data.byteLength;
    var requiredLength = offset + length;
    while (newByteLength < requiredLength)
      newByteLength <<= 1;
    if (newByteLength !== data.byteLength) {
      var oldDataView = dataView;
      data = new ArrayBuffer(newByteLength);
      dataView = new DataView(data);
      var uint32count = (offset + 3) >> 2;
      for (var i = 0; i < uint32count; ++i)
        dataView.setUint32(i << 2, oldDataView.getUint32(i << 2));
    }

    lastLength = length;
    return dataView;
  }
  function commitWrite() {
    offset += lastLength;
  }
  function writeFloat64(value) {
    commitWrite(prepareWrite(8).setFloat64(offset, value));
  }
  function writeUint8(value) {
    commitWrite(prepareWrite(1).setUint8(offset, value));
  }
  function writeUint8Array(value) {
    var dataView = prepareWrite(value.length);
    for (var i = 0; i < value.length; ++i)
      dataView.setUint8(offset + i, value[i]);
    commitWrite();
  }
  function writeUint16(value) {
    commitWrite(prepareWrite(2).setUint16(offset, value));
  }
  function writeUint32(value) {
    commitWrite(prepareWrite(4).setUint32(offset, value));
  }
  function writeUint64(value) {
    var low = value % POW_2_32;
    var high = (value - low) / POW_2_32;
    var dataView = prepareWrite(8);
    dataView.setUint32(offset, high);
    dataView.setUint32(offset + 4, low);
    commitWrite();
  }
  function writeTypeAndLength(type, length) {
    if (length < 24) {
      writeUint8(type << 5 | length);
    } else if (length < 0x100) {
      writeUint8(type << 5 | 24);
      writeUint8(length);
    } else if (length < 0x10000) {
      writeUint8(type << 5 | 25);
      writeUint16(length);
    } else if (length < 0x100000000) {
      writeUint8(type << 5 | 26);
      writeUint32(length);
    } else {
      writeUint8(type << 5 | 27);
      writeUint64(length);
    }
  }

  function encodeItem(value) {
    var i;

    if (value === false)
      return writeUint8(0xf4);
    if (value === true)
      return writeUint8(0xf5);
    if (value === null)
      return writeUint8(0xf6);
    if (value === undefined)
      return writeUint8(0xf7);

    switch (typeof value) {
      case "number":
        if (Math.floor(value) === value) {
          if (0 <= value && value <= POW_2_53)
            return writeTypeAndLength(0, value);
          if (-POW_2_53 <= value && value < 0)
            return writeTypeAndLength(1, -(value + 1));
        }
        writeUint8(0xfb);
        return writeFloat64(value);

      case "string":
        var utf8data = [];
        for (i = 0; i < value.length; ++i) {
          var charCode = value.charCodeAt(i);
          if (charCode < 0x80) {
            utf8data.push(charCode);
          } else if (charCode < 0x800) {
            utf8data.push(0xc0 | charCode >> 6);
            utf8data.push(0x80 | charCode & 0x3f);
          } else if (charCode < 0xd800) {
            utf8data.push(0xe0 | charCode >> 12);
            utf8data.push(0x80 | (charCode >> 6)  & 0x3f);
            utf8data.push(0x80 | charCode & 0x3f);
          } else {
            charCode = (charCode & 0x3ff) << 10;
            charCode |= value.charCodeAt(++i) & 0x3ff;
            charCode += 0x10000;

            utf8data.push(0xf0 | charCode >> 18);
            utf8data.push(0x80 | (charCode >> 12)  & 0x3f);
            utf8data.push(0x80 | (charCode >> 6)  & 0x3f);
            utf8data.push(0x80 | charCode & 0x3f);
          }
        }

        writeTypeAndLength(3, utf8data.length);
        return writeUint8Array(utf8data);

      default:
        var length;
        if (Array.isArray(value)) {
          length = value.length;
          writeTypeAndLength(4, length);
          for (i = 0; i < length; ++i)
            encodeItem(value[i]);
        } else if (value instanceof Uint8Array) {
          writeTypeAndLength(2, value.length);
          writeUint8Array(value);
        } else {
          var keys = Object.keys(value);
          length = keys.length;
          writeTypeAndLength(5, length);
          for (i = 0; i < length; ++i) {
            var key = keys[i];
            encodeItem(key);
            encodeItem(value[key]);
          }
        }
    }
  }

  encodeItem(value);

  if ("slice" in data)
    return data.slice(0, offset);

  var ret = new ArrayBuffer(offset);
  var retView = new DataView(ret);
  for (var i = 0; i < offset; ++i)
    retView.setUint8(i, dataView.getUint8(i));
  return ret;
}

function decode(data, tagger, simpleValue) {
  var dataView = new DataView(data);
  var offset = 0;

  if (typeof tagger !== "function")
    tagger = function(value) { return value; };
  if (typeof simpleValue !== "function")
    simpleValue = function() { return undefined; };

  function commitRead(length, value) {
    offset += length;
    return value;
  }
  function readArrayBuffer(length) {
    return commitRead(length, new Uint8Array(data, offset, length));
  }
  function readFloat16() {
    var tempArrayBuffer = new ArrayBuffer(4);
    var tempDataView = new DataView(tempArrayBuffer);
    var value = readUint16();

    var sign = value & 0x8000;
    var exponent = value & 0x7c00;
    var fraction = value & 0x03ff;

    if (exponent === 0x7c00)
      exponent = 0xff << 10;
    else if (exponent !== 0)
      exponent += (127 - 15) << 10;
    else if (fraction !== 0)
      return (sign ? -1 : 1) * fraction * POW_2_24;

    tempDataView.setUint32(0, sign << 16 | exponent << 13 | fraction << 13);
    return tempDataView.getFloat32(0);
  }
  function readFloat32() {
    return commitRead(4, dataView.getFloat32(offset));
  }
  function readFloat64() {
    return commitRead(8, dataView.getFloat64(offset));
  }
  function readUint8() {
    return commitRead(1, dataView.getUint8(offset));
  }
  function readUint16() {
    return commitRead(2, dataView.getUint16(offset));
  }
  function readUint32() {
    return commitRead(4, dataView.getUint32(offset));
  }
  function readUint64() {
    return readUint32() * POW_2_32 + readUint32();
  }
  function readBreak() {
    if (dataView.getUint8(offset) !== 0xff)
      return false;
    offset += 1;
    return true;
  }
  function readLength(additionalInformation) {
    if (additionalInformation < 24)
      return additionalInformation;
    if (additionalInformation === 24)
      return readUint8();
    if (additionalInformation === 25)
      return readUint16();
    if (additionalInformation === 26)
      return readUint32();
    if (additionalInformation === 27)
      return readUint64();
    if (additionalInformation === 31)
      return -1;
    throw "Invalid length encoding";
  }
  function readIndefiniteStringLength(majorType) {
    var initialByte = readUint8();
    if (initialByte === 0xff)
      return -1;
    var length = readLength(initialByte & 0x1f);
    if (length < 0 || (initialByte >> 5) !== majorType)
      throw "Invalid indefinite length element";
    return length;
  }

  function appendUtf16Data(utf16data, length) {
    for (var i = 0; i < length; ++i) {
      var value = readUint8();
      if (value & 0x80) {
        if (value < 0xe0) {
          value = (value & 0x1f) <<  6
                | (readUint8() & 0x3f);
          length -= 1;
        } else if (value < 0xf0) {
          value = (value & 0x0f) << 12
                | (readUint8() & 0x3f) << 6
                | (readUint8() & 0x3f);
          length -= 2;
        } else {
          value = (value & 0x0f) << 18
                | (readUint8() & 0x3f) << 12
                | (readUint8() & 0x3f) << 6
                | (readUint8() & 0x3f);
          length -= 3;
        }
      }

      if (value < 0x10000) {
        utf16data.push(value);
      } else {
        value -= 0x10000;
        utf16data.push(0xd800 | (value >> 10));
        utf16data.push(0xdc00 | (value & 0x3ff));
      }
    }
  }

  function decodeItem() {
    var initialByte = readUint8();
    var majorType = initialByte >> 5;
    var additionalInformation = initialByte & 0x1f;
    var i;
    var length;

    if (majorType === 7) {
      switch (additionalInformation) {
        case 25:
          return readFloat16();
        case 26:
          return readFloat32();
        case 27:
          return readFloat64();
      }
    }

    length = readLength(additionalInformation);
    if (length < 0 && (majorType < 2 || 6 < majorType))
      throw "Invalid length";

    switch (majorType) {
      case 0:
        return length;
      case 1:
        return -1 - length;
      case 2:
        if (length < 0) {
          var elements = [];
          var fullArrayLength = 0;
          while ((length = readIndefiniteStringLength(majorType)) >= 0) {
            fullArrayLength += length;
            elements.push(readArrayBuffer(length));
          }
          var fullArray = new Uint8Array(fullArrayLength);
          var fullArrayOffset = 0;
          for (i = 0; i < elements.length; ++i) {
            fullArray.set(elements[i], fullArrayOffset);
            fullArrayOffset += elements[i].length;
          }
          return fullArray;
        }
        return readArrayBuffer(length);
      case 3:
        var utf16data = [];
        if (length < 0) {
          while ((length = readIndefiniteStringLength(majorType)) >= 0)
            appendUtf16Data(utf16data, length);
        } else
          appendUtf16Data(utf16data, length);
        return String.fromCharCode.apply(null, utf16data);
      case 4:
        var retArray;
        if (length < 0) {
          retArray = [];
          while (!readBreak())
            retArray.push(decodeItem());
        } else {
          retArray = new Array(length);
          for (i = 0; i < length; ++i)
            retArray[i] = decodeItem();
        }
        return retArray;
      case 5:
        var retObject = {};
        for (i = 0; i < length || length < 0 && !readBreak(); ++i) {
          var key = decodeItem();
          retObject[key] = decodeItem();
        }
        return retObject;
      case 6:
        return tagger(decodeItem(), length);
      case 7:
        switch (length) {
          case 20:
            return false;
          case 21:
            return true;
          case 22:
            return null;
          case 23:
            return undefined;
          default:
            return simpleValue(length);
        }
    }
  }

  var ret = decodeItem();
  if (offset !== data.byteLength)
    throw "Remaining bytes";
  return ret;
}

var obj = { encode: encode, decode: decode };

/*
if (typeof define === "function" && define.amd)
  define("cbor/cbor", obj);
else if (typeof module !== "undefined" && module.exports)
  module.exports = obj;
else if (!global.CBOR)
  global.CBOR = obj;
*/
})(this);
/*jslint-enable*/


/*
file sqlmath.js
*/
(function () {
    let SQLITE_MAX_LENGTH = 1000000000;
    let SQLITE_OPEN_AUTOPROXY = 0x00000020;     /* VFS only */
    let SQLITE_OPEN_CREATE = 0x00000004;        /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_DELETEONCLOSE = 0x00000008; /* VFS only */
    let SQLITE_OPEN_EXCLUSIVE = 0x00000010;     /* VFS only */
    let SQLITE_OPEN_FULLMUTEX = 0x00010000;     /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_MAIN_DB = 0x00000100;       /* VFS only */
    let SQLITE_OPEN_MAIN_JOURNAL = 0x00000800;  /* VFS only */
    let SQLITE_OPEN_MEMORY = 0x00000080;        /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_NOFOLLOW = 0x01000000;      /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_NOMUTEX = 0x00008000;       /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_PRIVATECACHE = 0x00040000;  /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_READONLY = 0x00000001;      /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_READWRITE = 0x00000002;     /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_SHAREDCACHE = 0x00020000;   /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_SUBJOURNAL = 0x00002000;    /* VFS only */
    let SQLITE_OPEN_SUPER_JOURNAL = 0x00004000; /* VFS only */
    let SQLITE_OPEN_TEMP_DB = 0x00000200;       /* VFS only */
    let SQLITE_OPEN_TEMP_JOURNAL = 0x00001000;  /* VFS only */
    let SQLITE_OPEN_TRANSIENT_DB = 0x00000400;  /* VFS only */
    let SQLITE_OPEN_URI = 0x00000040;           /* Ok for sqlite3_open_v2() */
    let SQLITE_OPEN_WAL = 0x00080000;           /* VFS only */

    let addon = require(
        "./_binary_sqlmath_napi"
        + "_" + process.versions.napi
        + "_" + process.platform
        + "_" + process.arch
        + ".node"
    );
    // private map of sqlite-database-connections
    let dbOpenMap = new WeakMap();

    function __dbBusyDec(db) {
// this function will decrement db.busy
        db.busy -= 1;
        assertOrThrow(db.busy >= 0, "invalid db.busy " + db.busy);
    }

    function __dbBusyInc(db) {
// this function will increment db.busy
        db.busy += 1;
    }

    function __dbGet(db) {
// this function will get private-object mapped to <db>
        db = dbOpenMap.get(db);
        assertOrThrow(db, "invalid or closed db");
        assertOrThrow(db.busy >= 0, "invalid db.busy " + db.busy);
        return db;
    }

    function assertJsonEqual(aa, bb) {
// this function will assert JSON.stringify(<aa>) === JSON.stringify(<bb>)
        aa = JSON.stringify(objectDeepCopyWithKeysSorted(aa));
        bb = JSON.stringify(objectDeepCopyWithKeysSorted(bb));
        if (aa !== bb) {
            throw new Error(
                JSON.stringify(aa) + " !== " + JSON.stringify(bb)
            );
        }
    }

    function assertOrThrow(cond, msg) {
// this function will throw <msg> if <cond> is falsy
        if (!cond) {
            throw (
                typeof msg === "string"
                ? new Error(msg)
                : msg
            );
        }
    }

    function cCall(fnc, argList) {
// this function will serialize <argList> to a c <baton>,
// suitable for passing into napi
        let baton = new BigInt64Array(512);
        // serialize js-args to c-args
        argList = argList.map(function (arg, ii) {
            switch (typeof arg) {
            case "bigint":
            case "boolean":
            case "number":
                try {
                    baton[ii] = BigInt(arg);
                } catch (ignore) {
                    return;
                }
                break;
            case "string":
                // append null-terminator to string
                arg = Buffer.from(arg + "\u0000");
                break;
            }
            if (Buffer.isBuffer(arg)) {
                baton[ii] = BigInt(arg.byteLength);
            }
            return arg;
        });
        // pad argList to length = 8
        argList = argList.concat([
            undefined, undefined, undefined, undefined,
            undefined, undefined, undefined, undefined
        ]).slice(0, 8);
        // prepend baton to argList
        argList.unshift(baton);
        // call napi with fnc and argList
        return addon[fnc](argList);
    }

    async function dbClose({
        db
    }) {
// this function will close sqlite-database-connection <db>
        let __db = __dbGet(db);
        // prevent segfault - do not close db if actions are pending
        assertOrThrow(
            __db.busy === 0,
            "db cannot close with " + __db.busy + " actions pending"
        );
        dbOpenMap.delete(db);
        await cCall("__sqlite3_close_v2", [
            __db.ptr
        ]);
    }

    async function dbExec({
        db,
        sql
    }) {
// this function will exec <sql> in <db> and return result
        let result;
        db = __dbGet(db);
        __dbBusyInc(db);
        try {
            result = await cCall("_dbExec", [
                db.ptr, sql
            ]);
        } finally {
            __dbBusyDec(db);
        }
        return Buffer.from(result[1], 0, result[1].byteLength - 1);
    }

    async function dbOpen({
        filename,
        flags = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI
    }) {
// this function will open and return sqlite-database-connection <db>
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
        let db = {};
        let ptr = noop(await cCall("__sqlite3_open_v2", [
            filename, undefined, flags, undefined
        ]))[0][0];
        dbOpenMap.set(db, {
            busy: 0,
            ptr
        });
        return db;
    }

    async function dbVtmptableCreate({
        db,
        rowList,
        tableName = "tmp1"
    }) {
// this function will create-or-replace Vtmptable <tablename> and insert f
        let buf = Buffer.allocUnsafe(4096);
        let offset = 0;
        function bufAppend(type, val) {
            let alloced = buf.byteLength;
            let nn = offset + type.length + (
                val === undefined
                ? 0
                : typeof val === "number"
                ? 8
                : Buffer.byteLength(val)
            );
            let tmp;
            // exponentially grow buf as needed
            while (alloced < nn) {
                alloced *= 2;
                assertOrThrow(alloced <= SQLITE_MAX_LENGTH, "");
            }
            if (alloced > buf.byteLength) {
                tmp = Buffer.allocUnsafe(alloced);
                buf.copy(tmp);
                buf = tmp;
            }
            offset += buf.write(type, offset);
            if (typeof val === "number") {
                offset = buf.writeDoubleLE(val, offset);
            }
            if (typeof val === "string") {
                offset += buf.write(val, offset);
            }
        }
        if (rowList?.length === 0) {
            return;
        }
        rowList = jsonRowListNormalize({
            rowList
        });
        // type - js
        // 1. bigint
        // 2. boolean
        // 3. null
        // 4. number
        // 5. object
        // 6. string
        // 7. symbol
        // 8. undefined
        // type - sqlite
        // 1. blob
        // 2. integer
        // 3. null
        // 4. real
        // 5. text
        rowList.forEach(function (row) {
            row.forEach(function (val) {
                if (!val) {
                    switch (typeof val) {
                    case "bigint":
                    case "boolean":
                    case "number":
                        // 2. integer
                        bufAppend("i\u0000", undefined);
                        break;
                    case "string":
                        // 5. text
                        bufAppend("t\u0000", undefined);
                        break;
                    // case "null":
                    // case "object":
                    // case "symbol":
                    // case "undefined":
                    default:
                        // 3. null
                        bufAppend("\u0000", undefined);
                    }
                    return;
                }
                switch (typeof val) {
                case "bigint":
                    // 5. text
                    bufAppend("t", val.toString());
                    break;
                case "boolean":
                    // 2. integer
                    bufAppend("i\u0001", undefined);
                    break;
                case "number":
                    // 4. real
                    bufAppend("r", val);
                    break;
                case "string":
                    // 5. text
                    bufAppend("t", val);
                    break;
                case "symbol":
                    // 3. null
                    bufAppend("\u0000", undefined);
                    break;
                // case "null":
                // case "object":
                // case "undefined":
                default:
                    // 5. text
                    bufAppend("t", JSON.stringify(val));
                }
            });
        });
        await cCall("_jstableInsert", [
            db.ptr, tableName, buf
        ]);
    }

    function jsonRowListNormalize({
        colList,
        colListPriority = [],
        rowList
    }) {
// this function will normalize <rowList> with given <colList>
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
            rowList.unshift(colList);
            return rowList;
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

    function objectDeepCopyWithKeysSorted(obj) {
// this function will recursively deep-copy <obj> with keys sorted
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

    (function testAssertXxx() {
// this function will test assertXxx's handling-behavior
        // test assertJsonEqual handling-behavior
        assertJsonEqual([
            {
                aa: 1,
                bb: 2
            }
        ], [
            {
                bb: 2,
                aa: 1 //jslint-quiet
            }
        ]);
        try {
            assertJsonEqual("aa", "bb");
        } catch (err) {
            assertJsonEqual(err.message, "\"\\\"aa\\\"\" !== \"\\\"bb\\\"\"");
        }
        // test assertOrThrow handling-behavior
        [
            [
                "aa", "aa"
            ], [
                new Error("aa"), "aa"
            ]
        ].forEach(function ([
            err, msg
        ]) {
            try {
                assertOrThrow(undefined, err);
            } catch (err2) {
                assertJsonEqual(err2.message, msg);
            }
        });
    }());

    (function testCcall() {
// this function will test cCall's handling-behavior
        [
            [-0, "0"],
            [-Infinity, "0"],
            [0, "0"],
            [1 / 0, "0"],
            [Infinity, "0"],
            [false, "0"],
            [null, "0"],
            [true, "1"],
            [undefined, "0"],
            [{}, "0"]
        ].forEach(async function ([
            aa, bb
        ]) {
            let cc;
            cc = String(await cCall("noopAsync", [aa]))[0][0];
            assertOrThrow(bb === cc, [aa, bb, cc]);
            cc = String(cCall("noopSync", [aa]))[0][0];
            assertOrThrow(bb === cc, [aa, bb, cc]);
        });
    }());

    (async function testDbExec() {
// this function will test dbExec's handling-behavior
        let db;
        let promiseList = [];
        let tmp;
        db = await dbOpen({
            filename: ":memory:"
        });
        tmp = async function () {
            let result;
            try {
                result = JSON.stringify(JSON.parse(await dbExec({
                    db,
                    sql: (`
CREATE TABLE tt1 AS
SELECT 101 AS c101, 102 AS c102
UNION ALL
VALUES (201, 202),
       (301, NULL);
CREATE TABLE tt2 AS
SELECT 401 AS c401, 402 AS c402, 403 AS c403
UNION ALL
VALUES (501, 502.0123, 5030123456789),
       (601, '602', '603_\"\x01\x08\x09\x0a\x0b\x0c\x0d\x0e');
SELECT * FROM tt1;
SELECT * FROM tt2;
                    `)
                })));
                assertJsonEqual(result, JSON.stringify([
                    [
                        ["c101", "c102"],
                        [101, 102],
                        [201, 202],
                        [301, null]
                    ],
                    [
                        ["c401", "c402", "c403"],
                        [401, 402, 403],
                        [501, 502.0123, 5030123456789],
                        [601, "602", "603_\"\u0001\b\t\n\u000b\f\r\u000e"]
                    ]
                ]));
            } catch (err) {
                assertOrThrow(
                    err.message.indexOf("table tt1 already exists") >= 0,
                    err
                );
                return;
            }
        };
        promiseList.push(tmp());
        promiseList.push(tmp());
        promiseList.push(tmp());
        promiseList.push(tmp());
        try {
            await dbClose({
                db
            });
        } catch (err) {
            assertOrThrow((
                /db\scannot\sclose\swith\s\d+?\sactions\spending/
            ).test(err.message), err);
        }
        //!! await dbTableInsert({
            //!! db,
            //!! json: [
                //!! [
                    //!! "aa", "bb"
                //!! ]
            //!! ]
        //!! });
        await Promise.all(promiseList);
        dbClose({
            db
        });
    }());

    module.exports = Object.assign(module.exports, {
        SQLITE_MAX_LENGTH,
        SQLITE_OPEN_AUTOPROXY,
        SQLITE_OPEN_CREATE,
        SQLITE_OPEN_DELETEONCLOSE,
        SQLITE_OPEN_EXCLUSIVE,
        SQLITE_OPEN_FULLMUTEX,
        SQLITE_OPEN_MAIN_DB,
        SQLITE_OPEN_MAIN_JOURNAL,
        SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_NOFOLLOW,
        SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_PRIVATECACHE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_SHAREDCACHE,
        SQLITE_OPEN_SUBJOURNAL,
        SQLITE_OPEN_SUPER_JOURNAL,
        SQLITE_OPEN_TEMP_DB,
        SQLITE_OPEN_TEMP_JOURNAL,
        SQLITE_OPEN_TRANSIENT_DB,
        SQLITE_OPEN_URI,
        SQLITE_OPEN_WAL,
        dbClose,
        dbExec,
        dbOpen,
        dbVtmptableCreate
    });

    // jslint-hack
    noop(dbVtmptableCreate);
}());
