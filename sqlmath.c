// copyright nobody
// LINT_C_FILE


/*
file sqlmath.h
*/
// header
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <sqlite3ext.h>
static const sqlite3_api_routines *sqlite3_api;
// define
#ifndef SQLITE_MAX_LENGTH
#define SQLITE_MAX_LENGTH 1000000000
#endif
#define STR2_NOMEM -1
#define STR2_TOOBIG -2
#define SQLMATH_API
#define UNUSED(x) (void)(x)


/*
file sqlmath_dbExec.c
*/
// dynamically growable string
typedef struct Str2 {
    char *buf;
    int alloced;
    int used;
    int errcode;
} Str2;

static int str2AppendRaw(
    Str2 * str2,
    const char *zz,
    int nn
) {
/*
** Append <nn> bytes of text from <zz> to <str2->buf>.
** Increase the size of the memory allocation for <str2->buf> if necessary.
*/
    // declare var
    char *zTmp;
    int nAlloc;
    // write <zz> to <str2->buf> if space available
    if (0 <= nn && 0 <= str2->used && str2->used + nn <= str2->alloced) {
        memcpy(str2->buf + str2->used, zz, nn);
        str2->used += nn;
        return 0;
    }
    // sanity check
    if (str2->errcode != SQLITE_OK && str2->errcode != SQLITE_ROW) {
        return str2->errcode;
    }
    // check integers >= 0
    if (nn < 0 || nn > SQLITE_MAX_LENGTH) {
        str2->errcode = STR2_TOOBIG;
        return str2->errcode;
    }
    // grow nalloc exponentially
    nAlloc = str2->alloced;
    while (nAlloc < str2->used + nn) {
        nAlloc *= 2;
        if (nAlloc > SQLITE_MAX_LENGTH) {
            str2->errcode = STR2_TOOBIG;
            return str2->errcode;
        }
    }
    zTmp = realloc(str2->buf, nAlloc);
    if (zTmp == NULL) {
        str2->errcode = STR2_NOMEM;
        return str2->errcode;
    }
    str2->alloced = nAlloc;
    str2->buf = zTmp;
    // recurse
    return str2AppendRaw(str2, zz, nn);
}

static int str2AppendText(
    Str2 * str2,
    const char *zz,
    int nn
) {
/*
** Append <nn> bytes of text from <zz> to <str2->buf> with json-escaping.
** Increase the size of the memory allocation for <str2->buf> if necessary.
*/
    // declare var
    const char *zz2 = zz + nn;
    // double-quote open
    str2AppendRaw(str2, "\"", 1);
    while (zz < zz2) {
        switch (*zz) {
        case '\x00':
            str2AppendRaw(str2, "\\u0000", 6);
            break;
        case '\x01':
            str2AppendRaw(str2, "\\u0001", 6);
            break;
        case '\x02':
            str2AppendRaw(str2, "\\u0002", 6);
            break;
        case '\x03':
            str2AppendRaw(str2, "\\u0003", 6);
            break;
        case '\x04':
            str2AppendRaw(str2, "\\u0004", 6);
            break;
        case '\x05':
            str2AppendRaw(str2, "\\u0005", 6);
            break;
        case '\x06':
            str2AppendRaw(str2, "\\u0006", 6);
            break;
        case '\x07':
            str2AppendRaw(str2, "\\u0007", 6);
            break;
        case '\x08':
            str2AppendRaw(str2, "\\b", 2);
            break;
        case '\x09':
            str2AppendRaw(str2, "\\t", 2);
            break;
        case '\x0a':
            str2AppendRaw(str2, "\\n", 2);
            break;
        case '\x0b':
            str2AppendRaw(str2, "\\u000b", 6);
            break;
        case '\x0c':
            str2AppendRaw(str2, "\\f", 2);
            break;
        case '\x0d':
            str2AppendRaw(str2, "\\r", 2);
            break;
        case '\x0e':
            str2AppendRaw(str2, "\\u000e", 6);
            break;
        case '\x0f':
            str2AppendRaw(str2, "\\u000f", 6);
            break;
        case '\x10':
            str2AppendRaw(str2, "\\u0010", 6);
            break;
        case '\x11':
            str2AppendRaw(str2, "\\u0011", 6);
            break;
        case '\x12':
            str2AppendRaw(str2, "\\u0012", 6);
            break;
        case '\x13':
            str2AppendRaw(str2, "\\u0013", 6);
            break;
        case '\x14':
            str2AppendRaw(str2, "\\u0014", 6);
            break;
        case '\x15':
            str2AppendRaw(str2, "\\u0015", 6);
            break;
        case '\x16':
            str2AppendRaw(str2, "\\u0016", 6);
            break;
        case '\x17':
            str2AppendRaw(str2, "\\u0017", 6);
            break;
        case '\x18':
            str2AppendRaw(str2, "\\u0018", 6);
            break;
        case '\x19':
            str2AppendRaw(str2, "\\u0019", 6);
            break;
        case '\x1a':
            str2AppendRaw(str2, "\\u001a", 6);
            break;
        case '\x1b':
            str2AppendRaw(str2, "\\u001b", 6);
            break;
        case '\x1c':
            str2AppendRaw(str2, "\\u001c", 6);
            break;
        case '\x1d':
            str2AppendRaw(str2, "\\u001d", 6);
            break;
        case '\x22':
            str2AppendRaw(str2, "\\\"", 2);
            break;
        case '\x5c':
            str2AppendRaw(str2, "\\\\", 2);
            break;
        default:
            str2AppendRaw(str2, zz, 1);
        }
        zz += 1;
    }
    // double-quote close
    return str2AppendRaw(str2, "\"", 1);
}

SQLMATH_API int dbExec(
    sqlite3 * db,               /* The database on which the SQL executes */
    const char *zSql,           /* The SQL to be executed */
    char **pzBuf,
    int *pAlloced,
    char *zErrmsg
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized a json-string in
// <str2>.
#define STR2_APPEND_RAW(zz, len) \
    if (0 != str2AppendRaw(&str2, zz, len)) {goto label_error;}
#define STR2_APPEND_TEXT(zz, len) \
    if (0 != str2AppendText(&str2, zz, len)) {goto label_error;}
    // declare var
    Str2 str2 = { 0 };
    const char *zTmp = NULL;
    int ii = 0;
    int nCol = 0;
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    // mutext enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // init str2.buf
    str2.buf = malloc(2);
    if (str2.buf == NULL) {
        str2.errcode = STR2_NOMEM;
        goto label_error;
    }
    str2.alloced = 2;
    // bracket database [
    STR2_APPEND_RAW("[", 1);
    // loop over each table
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        pStmt = NULL;
        str2.errcode = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zTmp);
        if (str2.errcode != SQLITE_OK || *zSql == '\x00') {
            break;
        }
        zSql = zTmp;
        nCol = -1;
        // loop over each row
        while (1) {
            str2.errcode = sqlite3_step(pStmt);
            if (str2.errcode != SQLITE_ROW) {
                str2.errcode = sqlite3_finalize(pStmt);
                break;
            }
            // insert row of column-names
            if (nCol == -1) {
                if (str2.used > 1) {
                    STR2_APPEND_RAW(",\n\n", 3);
                }
                // bracket table [
                // bracket column [
                STR2_APPEND_RAW("[[", 2);
                // loop over each column-name
                nCol = sqlite3_column_count(pStmt);
                ii = 0;
                while (ii < nCol) {
                    if (ii > 0) {
                        STR2_APPEND_RAW(",", 1);
                    }
                    zTmp = sqlite3_column_name(pStmt, ii);
                    STR2_APPEND_TEXT(zTmp, strlen(zTmp));
                    ii += 1;
                }
                // bracket column ]
                STR2_APPEND_RAW("]", 1);
            }
            // bracket row [
            STR2_APPEND_RAW(",\n[", 3);
            ii = 0;
            // loop over each column-value
            while (ii < nCol) {
                if (ii > 0) {
                    STR2_APPEND_RAW(",", 1);
                }
                switch (sqlite3_column_type(pStmt, ii)) {
                    // encode blob as data-uri application/octet-stream
                    // case SQLITE_BLOB:
                    // break;
                case SQLITE_INTEGER:
                case SQLITE_FLOAT:
                    STR2_APPEND_RAW(
                        (const char *) sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                    // append text as json-escaped string
                case SQLITE_TEXT:
                    STR2_APPEND_TEXT(
                        (const char *) sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                default:       /* case SQLITE_NULL: */
                    STR2_APPEND_RAW("null", 4);
                    break;
                }
                ii += 1;
            }
            // bracket row ]
            STR2_APPEND_RAW("]", 1);
            if (str2.errcode != SQLITE_ROW) {
                break;
            }
        }
        if (nCol != -1) {
            // bracket table ]
            STR2_APPEND_RAW("]", 1);
        }
    }
    // bracket database ]
    STR2_APPEND_RAW("]\n\x00", 2);
    // shrink str2.buf to str2.used
    zTmp = (const char *) realloc(str2.buf, str2.used);
    if (zTmp == NULL) {
        str2.errcode = STR2_NOMEM;
    } else {
        str2.buf = (char *) zTmp;
        str2.alloced = str2.used;
    }
  label_error:
    // handle errcode
    if (str2.errcode != SQLITE_OK) {
        if (str2.buf != NULL) {
            free(str2.buf);
        }
        switch (str2.errcode) {
        case STR2_NOMEM:
            strncpy(zErrmsg, sqlite3_errstr(SQLITE_NOMEM), 255);
            break;
        case STR2_TOOBIG:
            strncpy(zErrmsg, sqlite3_errstr(SQLITE_TOOBIG), 255);
            break;
        default:
            strncpy(zErrmsg, sqlite3_errmsg(db), 255);
        }
        // mutext leave
        sqlite3_mutex_leave(sqlite3_db_mutex(db));
        return str2.errcode;
    }
    *pAlloced = str2.alloced;
    *pzBuf = str2.buf;
    // mutext leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    return 0;
}

int sqlite3_sqlmath_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED(db);
    UNUSED(pzErrMsg);
    int rc = SQLITE_OK;
    sqlite3_api = pApi;
    return rc;
}
