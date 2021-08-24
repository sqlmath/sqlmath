// copyright nobody
// LINT_C_FILE

/* sqlmath.h - start */
/* header */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <sqlmath.h>
#ifndef SQLITE_MAX_LENGTH
# define SQLITE_MAX_LENGTH 1000000000
#endif
#define JSSQL_NOMEM -1
#define JSSQL_TOOBIG -2
/* *INDENT-OFF* */
typedef struct Jsctx { char *buf; int alloced; int used; int errcode; } Jsctx;
/* *INDENT-ON* */
/* sqlmath.h - end */







/* sqlmath.c - start */
static bool jssqlAppendRaw(
    Jsctx * ctx,
    const char *zz,
    int nn
) {
/*
** Append <nn> bytes of text from <zz> to <ctx->buf>.
** Increase the size of the memory allocation for <ctx->buf> if necessary.
*/
    // declare var
    char *zTmp;
    int nAlloc;
    // write <zz> to <ctx->buf> if space available
    if (0 <= nn && 0 <= ctx->used && ctx->used + nn <= ctx->alloced) {
        memcpy(ctx->buf + ctx->used, zz, nn);
        ctx->used += nn;
        return true;
    }
    // sanity check
    if (ctx->errcode != SQLITE_OK && ctx->errcode != SQLITE_ROW) {
        return false;
    }
    // check integers >= 0
    if (nn < 0 || nn > SQLITE_MAX_LENGTH) {
        ctx->errcode = JSSQL_TOOBIG;
        return false;
    }
    // grow nalloc exponentially
    nAlloc = ctx->alloced;
    while (nAlloc < ctx->used + nn) {
        nAlloc *= 2;
    }
    // realloc space
    zTmp = realloc(ctx->buf, nAlloc);
    if (zTmp == NULL) {
        ctx->errcode = JSSQL_NOMEM;
        return false;
    }
    ctx->alloced = nn;
    ctx->buf = zTmp;
    // recurse
    return jssqlAppendRaw(ctx, zz, nn);
}

static bool jssqlAppendText(
    Jsctx * ctx,
    const char *zz,
    int nn
) {
/*
** Append <nn> bytes of text from <zz> to <ctx->buf> with json-escaping.
** Increase the size of the memory allocation for <ctx->buf> if necessary.
*/
    // declare var
    const char *zz2 = zz + nn;
    // double-quote open
    jssqlAppendRaw(ctx, "\"", 1);
    while (zz < zz2) {
        switch (*zz) {
        case '\x00':
            jssqlAppendRaw(ctx, "\\u0000", 6);
            break;
        case '\x01':
            jssqlAppendRaw(ctx, "\\u0001", 6);
            break;
        case '\x02':
            jssqlAppendRaw(ctx, "\\u0002", 6);
            break;
        case '\x03':
            jssqlAppendRaw(ctx, "\\u0003", 6);
            break;
        case '\x04':
            jssqlAppendRaw(ctx, "\\u0004", 6);
            break;
        case '\x05':
            jssqlAppendRaw(ctx, "\\u0005", 6);
            break;
        case '\x06':
            jssqlAppendRaw(ctx, "\\u0006", 6);
            break;
        case '\x07':
            jssqlAppendRaw(ctx, "\\u0007", 6);
            break;
        case '\x08':
            jssqlAppendRaw(ctx, "\\b", 2);
            break;
        case '\x09':
            jssqlAppendRaw(ctx, "\\t", 2);
            break;
        case '\x0a':
            jssqlAppendRaw(ctx, "\\n", 2);
            break;
        case '\x0b':
            jssqlAppendRaw(ctx, "\\u000b", 6);
            break;
        case '\x0c':
            jssqlAppendRaw(ctx, "\\f", 2);
            break;
        case '\x0d':
            jssqlAppendRaw(ctx, "\\r", 2);
            break;
        case '\x0e':
            jssqlAppendRaw(ctx, "\\u000e", 6);
            break;
        case '\x0f':
            jssqlAppendRaw(ctx, "\\u000f", 6);
            break;
        case '\x10':
            jssqlAppendRaw(ctx, "\\u0010", 6);
            break;
        case '\x11':
            jssqlAppendRaw(ctx, "\\u0011", 6);
            break;
        case '\x12':
            jssqlAppendRaw(ctx, "\\u0012", 6);
            break;
        case '\x13':
            jssqlAppendRaw(ctx, "\\u0013", 6);
            break;
        case '\x14':
            jssqlAppendRaw(ctx, "\\u0014", 6);
            break;
        case '\x15':
            jssqlAppendRaw(ctx, "\\u0015", 6);
            break;
        case '\x16':
            jssqlAppendRaw(ctx, "\\u0016", 6);
            break;
        case '\x17':
            jssqlAppendRaw(ctx, "\\u0017", 6);
            break;
        case '\x18':
            jssqlAppendRaw(ctx, "\\u0018", 6);
            break;
        case '\x19':
            jssqlAppendRaw(ctx, "\\u0019", 6);
            break;
        case '\x1a':
            jssqlAppendRaw(ctx, "\\u001a", 6);
            break;
        case '\x1b':
            jssqlAppendRaw(ctx, "\\u001b", 6);
            break;
        case '\x1c':
            jssqlAppendRaw(ctx, "\\u001c", 6);
            break;
        case '\x1d':
            jssqlAppendRaw(ctx, "\\u001d", 6);
            break;
        case '\x22':
            jssqlAppendRaw(ctx, "\\\"", 2);
            break;
        case '\x5c':
            jssqlAppendRaw(ctx, "\\\\", 2);
            break;
        default:
            jssqlAppendRaw(ctx, zz, 1);
        }
        zz += 1;
    }
    // double-quote close
    return jssqlAppendRaw(ctx, "\"", 1);
}

bool jssqlExec(
    sqlite3 * db,               /* The database on which the SQL executes */
    const char *zSql,           /* The SQL to be executed */
    char **pzBuf,
    int *pUsed,
    const char **errmsg
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized a json-string in
// <ctx>.
#define JSSQL_APPEND_RAW(str, len) \
    if (!jssqlAppendRaw(&ctx, str, len)) {goto label_error;}
#define JSSQL_APPEND_TEXT(str, len) \
    if (!jssqlAppendText(&ctx, str, len)) {goto label_error;}
    // declare var
    Jsctx ctx = { 0 };
    const char *zTmp = NULL;
    int ii = 0;
    int nCol = 0;
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    // mutext enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // init ctx.buf
    ctx.buf = malloc(256);
    if (ctx.buf == NULL) {
        ctx.errcode = JSSQL_NOMEM;
        goto label_error;
    }
    ctx.alloced = 256;
    // bracket database [
    JSSQL_APPEND_RAW("[", 1);
    // loop over each table
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        pStmt = NULL;
        ctx.errcode = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zTmp);
        if (ctx.errcode != SQLITE_OK || *zSql == '\x00') {
            break;
        }
        zSql = zTmp;
        nCol = -1;
        // loop over each row
        while (1) {
            ctx.errcode = sqlite3_step(pStmt);
            if (ctx.errcode != SQLITE_ROW) {
                ctx.errcode = sqlite3_finalize(pStmt);
                break;
            }
            // insert row of column-names
            if (nCol == -1) {
                if (ctx.used > 1) {
                    JSSQL_APPEND_RAW(",\n\n", 3);
                }
                // bracket table [
                // bracket column [
                JSSQL_APPEND_RAW("[[", 2);
                // loop over each column-name
                nCol = sqlite3_column_count(pStmt);
                ii = 0;
                while (ii < nCol) {
                    if (ii > 0) {
                        JSSQL_APPEND_RAW(",", 1);
                    }
                    zTmp = sqlite3_column_name(pStmt, ii);
                    JSSQL_APPEND_TEXT(zTmp, strlen(zTmp));
                    ii += 1;
                }
                // bracket column ]
                JSSQL_APPEND_RAW("]", 1);
            }
            // bracket row [
            JSSQL_APPEND_RAW(",\n[", 3);
            ii = 0;
            // loop over each column-value
            while (ii < nCol) {
                if (ii > 0) {
                    JSSQL_APPEND_RAW(",", 1);
                }
                switch (sqlite3_column_type(pStmt, ii)) {
                    // encode blob as data-uri application/octet-stream
                    // case SQLITE_BLOB:
                    // break;
                case SQLITE_INTEGER:
                case SQLITE_FLOAT:
                    JSSQL_APPEND_RAW(
                        (const char *) sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                    // append text as json-escaped string
                case SQLITE_TEXT:
                    JSSQL_APPEND_TEXT(
                        (const char *) sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                default:       /* case SQLITE_NULL: */
                    JSSQL_APPEND_RAW("null", 4);
                    break;
                }
                ii += 1;
            }
            // bracket row ]
            JSSQL_APPEND_RAW("]", 1);
            if (ctx.errcode != SQLITE_ROW) {
                break;
            }
        }
        if (nCol != -1) {
            // bracket table ]
            JSSQL_APPEND_RAW("]", 1);
        }
    }
    // bracket database ]
    JSSQL_APPEND_RAW("]\n", 2);
    // shrink ctx.buf to ctx.used
    zTmp = (const char *) realloc(ctx.buf, ctx.used);
    if (zTmp == NULL) {
        ctx.errcode = JSSQL_NOMEM;
    }
  label_error:
    // handle errcode
    if (ctx.errcode != SQLITE_OK) {
        if (ctx.buf != NULL) {
            free(ctx.buf);
        }
        switch (ctx.errcode) {
        case JSSQL_NOMEM:
            *errmsg = sqlite3_errstr(SQLITE_NOMEM);
            break;
        case JSSQL_TOOBIG:
            *errmsg = sqlite3_errstr(SQLITE_TOOBIG);
            break;
        default:
            *errmsg = sqlite3_errmsg(db);
        }
        // mutext leave
        sqlite3_mutex_leave(sqlite3_db_mutex(db));
        return false;
    }
    *pUsed = ctx.used;
    *pzBuf = ctx.buf;
    // mutext leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    return true;
}

int sqlite3_sqlmath_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    // SQLITE_EXTENSION_INIT2(pApi);
    return rc;
}

/* sqlmath.c - end */
