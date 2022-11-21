// MIT License
//
// Copyright (c) 2021 Kai Zhu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// LINT_C_FILE


/*
file sqlmath_h - start
*/
// *INDENT-OFF*
#if defined(__cplusplus)
extern "C" {
#endif
// *INDENT-ON*


#ifndef SQLMATH_H
#define SQLMATH_H


#include <assert.h>
#include <float.h>
#include <math.h>
#include <sqlite3ext.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
** A macro to hint to the compiler that a function should not be
** inlined.
*/
#if defined(__GNUC__)
#  define SQLITE_NOINLINE  __attribute__((noinline))
#elif defined(_MSC_VER) && _MSC_VER >= 1310
#  define SQLITE_NOINLINE  __declspec(noinline)
#else
#  define SQLITE_NOINLINE
#endif


#define JSBATON_ARGC 16
#define JS_MAX_SAFE_INTEGER 0x1fffffffffffff
#define JS_MIN_SAFE_INTEGER -0x1fffffffffffff
#define MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
#define NAPI_VERSION 8
#define SGN(aa) (((aa) < 0) ? -1 : ((aa) > 0) ? 1 : 0)
#define SIZEOF_MESSAGE_DEFAULT 256
#define SQLITE_DATATYPE_BLOB            0x04
// #define SQLITE_DATATYPE_BLOB_0          0x14
#define SQLITE_DATATYPE_FLOAT           0x02
// #define SQLITE_DATATYPE_FLOAT_0         0x12
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_OFFSET          768
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       -0x01
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
#define SQLITE_ERROR_DATATYPE_INVALID 0x10003
#define SQLITE_ERROR_ZSQL_NULL        0x10004
#define SQLITE_MAX_LENGTH2 1000000000
#define SQLITE_RESPONSETYPE_LASTBLOB 1
#define SQLMATH_API
#define SQLMATH_FNC
#define SWAP(aa, bb) tmp = (aa); (aa) = (bb); (bb) = tmp
#define UNUSED(x) ((void)(x))


// this function will exec <sql> and if <errcode> is not ok,
// throw <baton>->errmsg with given sqlite-<errcode>
#define JSBATON_ASSERT_OK() \
    if (errcode != SQLITE_OK) { \
        if (baton != NULL && baton->errmsg[0] == '\x00') { \
            sqlmathSnprintfTrace(baton->errmsg, "sqlite - ", ( \
                errcode == SQLITE_ERROR_DATATYPE_INVALID ? "invalid datatype" \
                : errcode == SQLITE_ERROR_ZSQL_NULL \
                    ? "sqlite - cannot execute null sql-string" \
                : db == NULL || sqlite3_errcode(db) == SQLITE_OK \
                    ? sqlite3_errstr(errcode) \
                : sqlite3_errmsg(db)), __func__, __FILE__, __LINE__); \
        } \
        goto catch_error; \
    }

// this function will if <cond> is falsy, terminate process with <msg>
#define NAPI_ASSERT_FATAL(cond, msg) \
    if (!(cond)) { \
        napi_fatal_error(__func__, NAPI_AUTO_LENGTH , msg, NAPI_AUTO_LENGTH); \
    }

// this function will assert <errcode> == napi_ok in <env>
#define NAPI_ASSERT_OK() \
    if (0 != napiAssertOk(env, __func__, __FILE__, __LINE__, errcode)) { \
        return NULL; \
    }

#define NAPI_EXPORT_MEMBER(name) \
    {#name, NULL, name, NULL, NULL, NULL, napi_default, NULL}

#define NAPI_JSPROMISE_CREATE(func) \
    static void __##napi##_##func(napi_env env, void *data) { \
        UNUSED(env); \
        func((Jsbaton *) data); \
    } \
    static napi_value _##func(napi_env env, napi_callback_info info) { \
        return jspromiseCreate(env, info, __##napi##_##func); \
    } \

#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql_##func##_func, NULL, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_CREATE_FUNCTION2(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        NULL, sql_##func##_step, sql_##func##_final); \
    if (errcode != SQLITE_OK) { return errcode; }

#define STR99_AGGREGATE_CLEANUP() \
    if (str99->zText) { sqlite3_free(str99->zText); str99->zText = NULL; }

#define STR99_AGGREGATE_RESULT_ERROR() \
    if (str99->accError != 0) { \
        sqlite3_result_error_code(context, str99->accError); \
        STR99_AGGREGATE_CLEANUP(); \
        return; \
    }


// file sqlmath_h - sqlite3
// *INDENT-OFF*
SQLITE_API const sqlite3_api_routines *sqlite3ApiGet();
typedef struct sqlite3_str sqlite3_str;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;


// file sqlmath_h - str99
/*
** An objected used to accumulate the text of a string where we
** do not necessarily know how big the string will be in the end.
*/
struct sqlite3_str {
  sqlite3 *db;         /* Optional database for lookaside.  Can be NULL */
  char *zText;         /* The string collected so far */
  u32  nAlloc;         /* Amount of space allocated in zText */
  u32  mxAlloc;        /* Maximum allowed allocation.  0 for no malloc usage */
  u32  nChar;          /* Length of the string so far */
  u8   accError;       /* SQLITE_NOMEM or SQLITE_TOOBIG */
  u8   printfFlags;    /* SQLITE_PRINTF flags below */
};
SQLITE_API int sqlite3_str_enlarge(sqlite3_str *p, int nn);
SQLMATH_API void str99JsonAppend(sqlite3_str *str99, const char *zIn, u32 nn);
SQLMATH_API void str99VectorDoubleAppend(sqlite3_str *str99, double val);
// *INDENT-ON*


// file sqlmath_h - Jsbaton
typedef struct napi_async_work__ *napi_async_work;
typedef struct napi_deferred__ *napi_deferred;
typedef struct napi_value__ *napi_value;

typedef struct Jsbaton {
    int32_t nallc;              // offset - 0-4
    int32_t nused;              // offset - 4-8
    int64_t argv[JSBATON_ARGC]; // offset - 8-136
    int64_t bufv[JSBATON_ARGC]; // offset - 136-264
    int64_t cfuncname;          // offset - 264-272
    char errmsg[SIZEOF_MESSAGE_DEFAULT];        // offset 272-528
    napi_value napi_argv;
    napi_async_work napi_work;
    napi_deferred napi_deferred;
} Jsbaton;


// file sqlmath_h - SQLMATH_API
SQLMATH_API int __dbFileImportOrExport(
    sqlite3 * pInMemory,
    char *zFilename,
    const int isSave
);

SQLMATH_API void dbClose(
    Jsbaton * baton
);

SQLMATH_API void dbExec(
    Jsbaton * baton
);

SQLMATH_API void dbFileImportOrExport(
    Jsbaton * baton
);

SQLMATH_API void dbNoop(
    Jsbaton * baton
);

SQLMATH_API void dbOpen(
    Jsbaton * baton
);

SQLMATH_API int doubleSign(
    const double aa
);

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
);

SQLMATH_API const char *jsbatonValueErrmsg(
    Jsbaton * baton
);

SQLMATH_API const char *jsbatonValueStringArgi(
    Jsbaton * baton,
    int argi
);

SQLMATH_API double kthpercentile(
    double *arr,
    const int nn,
    const double kk
);

SQLMATH_API int noop(
);

SQLMATH_API const char *sqlmathSnprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
);


// *INDENT-OFF*
#if defined(__cplusplus)
}
#endif
// *INDENT-ON*
/*
file sqlmath_h - end
*/
#endif                          // SQLMATH_H


/*
file sqlmath_ext - start
*/
#ifdef SQLITE3_EXT_C2


static const sqlite3_api_routines *sqlite3_api = NULL;
#include "sqlmath_jenks.c"
typedef struct DbExecBindElem {
    const char *buf;
    char *key;
    int buflen;
    char datatype;
} DbExecBindElem;
// track how many sqlite-db open
static int dbCount = 0;


// file sqlmath_ext - SQLMATH_API
SQLMATH_API int __dbFileImportOrExport(
    sqlite3 * pInMemory,
    char *zFilename,
    const int isSave
) {
    // fprintf(stderr, "\nsqlmath.dbFileImportOrExport(pp=%p ff=%s ss=%d)\n",
    //     pInMemory, zFilename, isSave);
/*
** https://www.sqlite.org/backup.html
** This function is used to load the contents of a database file on disk
** into the "main" database of open database connection pInMemory, or
** to save the current contents of the database opened by pInMemory into
** a database file on disk. pInMemory is probably an in-memory database,
** but this function will also work fine if it is not.
**
** Parameter zFilename points to a nul-terminated string containing the
** name of the database file on disk to load from or save to. If parameter
** isSave is non-zero, then the contents of the file zFilename are
** overwritten with the contents of the database opened by pInMemory. If
** parameter isSave is zero, then the contents of the database opened by
** pInMemory are replaced by data loaded from the file zFilename.
**
** If the operation is successful, SQLITE_OK is returned. Otherwise, if
** an error occurs, an SQLite error code is returned.
*/
    int rc;                     /* Function return code */
    sqlite3 *pFile;             /* Database connection opened on zFilename */
    sqlite3_backup *pBackup;    /* Backup object used to copy data */
    sqlite3 *pTo;               /* Database to copy to (pFile or pInMemory) */
    sqlite3 *pFrom;             /* Database to copy from (pFile or pInMemory) */
    /* Open the database file identified by zFilename. Exit early if this fails
     ** for any reason. */
    rc = sqlite3_open(zFilename, &pFile);
    if (rc == SQLITE_OK) {
        /* If this is a 'load' operation (isSave==0), then data is copied
         ** from the database file just opened to database pInMemory.
         ** Otherwise, if this is a 'save' operation (isSave==1), then data
         ** is copied from pInMemory to pFile.  Set the variables pFrom and
         ** pTo accordingly. */
        pFrom = (isSave ? pInMemory : pFile);
        pTo = (isSave ? pFile : pInMemory);
        /* Set up the backup procedure to copy from the "main" database of
         ** connection pFile to the main database of connection pInMemory.
         ** If something goes wrong, pBackup will be set to NULL and an error
         ** code and message left in connection pTo.
         **
         ** If the backup object is successfully created, call backup_step()
         ** to copy data from pFile to pInMemory. Then call backup_finish()
         ** to release resources associated with the pBackup object.  If an
         ** error occurred, then an error code and message will be left in
         ** connection pTo. If no error occurred, then the error code belonging
         ** to pTo is set to SQLITE_OK.
         */
        pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
        if (pBackup) {
            (void) sqlite3_backup_step(pBackup, -1);
            (void) sqlite3_backup_finish(pBackup);
        }
        rc = sqlite3_errcode(pTo);
    }
    /* Close the database connection opened on database file zFilename
     ** and return the result of this function. */
    (void) sqlite3_close(pFile);
    return rc;
}

SQLMATH_API void dbClose(
    Jsbaton * baton
) {
// int sqlite3_close_v2(sqlite3*);
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    // call c-function
    errcode = sqlite3_close_v2(db);
    JSBATON_ASSERT_OK();
    if (db != NULL) {
        dbCount -= 1;
        // fprintf(stderr, "\nsqlmath.dbClose(argv0=%lld db=%lld dbCount=%d)\n",
        //     baton->argv[0], (int64_t) db, dbCount);
    }
  catch_error:
    (void) 0;
}

// SQLMATH_API dbexec - start
SQLMATH_API void dbExec(
    Jsbaton * baton
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized json-string in <str99>.
    // declare var
    DbExecBindElem *bindElem = NULL;
    DbExecBindElem *bindList = NULL;
    const char **pzShared = ((const char **) baton->argv) + 8;
    const char *zBind = (const char *) baton + SQLITE_DATATYPE_OFFSET;
    const char *zSql = jsbatonValueStringArgi(baton, 1);
    const char *zTmp = NULL;
    double rTmp = 0;
    int bindByKey = (int) baton->argv[3];
    int bindIdx = 0;
    int bindListLength = (int) baton->argv[2];
    int errcode = 0;
    int ii = 0;
    int jj = 0;
    int nCol = 0;
    int responseType = (int) baton->argv[4];
    int64_t iTmp = 0;
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    static const char bindPrefix[] = "$:@";
    // str99 - init
    sqlite3_str __str99 = { 0 };
    sqlite3_str *str99 = &__str99;
    str99->mxAlloc = SQLITE_MAX_LENGTH2;
    // fprintf(stderr, "\nsqlmath.dbExec(db=%lld blen=%d sql=%s)\n",
    //     (int64_t) db, bindListLength, zSql);
#ifndef EMSCRIPTEN
    // mutex enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
#endif                          // EMSCRIPTEN
    if (zSql == NULL) {
        errcode = SQLITE_ERROR_ZSQL_NULL;
        JSBATON_ASSERT_OK();
    }
    // init bindList
    bindList =
        (DbExecBindElem *) sqlite3_malloc(bindListLength *
        sizeof(DbExecBindElem));
    memset(bindList, 0, bindListLength * sizeof(DbExecBindElem));
    if (bindListLength > 0 && bindList == NULL) {
        errcode = SQLITE_NOMEM;
        JSBATON_ASSERT_OK()
    }
    bindElem = bindList;
    ii = 1;
    while (ii <= bindListLength) {
        // init key and
        if (bindByKey) {
            zBind += 1;
            bindElem->key = (char *) zBind + 4;
            zBind += 4 + *(int32_t *) zBind;
        }
        // init datatype
        bindElem->datatype = zBind[0];
        zBind += 1;
        switch (bindElem->datatype) {
        case SQLITE_DATATYPE_BLOB:
        case SQLITE_DATATYPE_TEXT:
            bindElem->buflen = *(int32_t *) zBind;
            bindElem->buf = zBind + 4;
            zBind += 4 + bindElem->buflen;
            break;
        case SQLITE_DATATYPE_FLOAT:
        case SQLITE_DATATYPE_INTEGER:
            bindElem->buf = zBind;
            zBind += 8;
            break;
        case SQLITE_DATATYPE_INTEGER_0:
        case SQLITE_DATATYPE_INTEGER_1:
        case SQLITE_DATATYPE_NULL:
        case SQLITE_DATATYPE_TEXT_0:
            break;
        case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
            bindElem->buflen = *(int32_t *) zBind;
            bindElem->buf = *pzShared;
            pzShared += 1;
            zBind += 4;
            break;
        default:
            fprintf(stderr, "\nsqlmath.dbExec(ii=%d datatype=%d len=%d)\n",
                ii, bindElem->datatype, bindElem->buflen);
            errcode = SQLITE_ERROR_DATATYPE_INVALID;
            JSBATON_ASSERT_OK();
        }
        bindElem += 1;
        ii += 1;
    }
    // bracket database [
    sqlite3_str_appendchar(str99, 1, '[');
    // loop over each table - start
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        errcode = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zTmp);
        JSBATON_ASSERT_OK();
        if (errcode != 0 || *zSql == '\x00') {
            break;
        }
        zSql = zTmp;
        // bind bindList to pStmt
        bindElem = bindList;
        ii = 1;
        while (ii <= bindListLength) {
            bindIdx = ii;
            jj = 0;
            while (jj < 3) {
                // if bindByKey, then get idx for given key
                if (bindByKey) {
                    bindElem->key[0] = bindPrefix[jj];
                    bindIdx =
                        sqlite3_bind_parameter_index(pStmt, bindElem->key);
                }
                if (bindIdx > 0) {
                    switch (bindElem->datatype) {
                    case SQLITE_DATATYPE_BLOB:
                        errcode =
                            sqlite3_bind_blob(pStmt, bindIdx, bindElem->buf,
                            bindElem->buflen, SQLITE_STATIC);
                        break;
                    case SQLITE_DATATYPE_FLOAT:
                        errcode =
                            sqlite3_bind_double(pStmt, bindIdx,
                            *(double *) bindElem->buf);
                        break;
                    case SQLITE_DATATYPE_INTEGER:
                        errcode =
                            sqlite3_bind_int64(pStmt, bindIdx,
                            *(int64_t *) bindElem->buf);
                        break;
                    case SQLITE_DATATYPE_INTEGER_0:
                        errcode = sqlite3_bind_int(pStmt, bindIdx, 0);
                        break;
                    case SQLITE_DATATYPE_INTEGER_1:
                        errcode = sqlite3_bind_int(pStmt, bindIdx, 1);
                        break;
                    case SQLITE_DATATYPE_NULL:
                        errcode = sqlite3_bind_null(pStmt, bindIdx);
                        break;
                    case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
                        errcode =
                            sqlite3_bind_blob(pStmt, bindIdx, bindElem->buf,
                            bindElem->buflen, SQLITE_STATIC);
                        break;
                    case SQLITE_DATATYPE_TEXT_0:
                        errcode =
                            sqlite3_bind_text(pStmt, bindIdx, "", 0,
                            SQLITE_STATIC);
                        break;
                    case SQLITE_DATATYPE_TEXT:
                        errcode =
                            sqlite3_bind_text(pStmt, bindIdx, bindElem->buf,
                            bindElem->buflen, SQLITE_STATIC);
                        break;
                    default:
                        fprintf(stderr,
                            "\nsqlmath.dbExec(ii=%d  datatype=%d  len=%d)\n",
                            ii, bindElem->datatype, bindElem->buflen);
                        errcode = SQLITE_ERROR_DATATYPE_INVALID;
                    }
                    // ignore bind-range-error
                    if (errcode != SQLITE_RANGE) {
                        JSBATON_ASSERT_OK();
                    }
                }
                if (!bindByKey) {
                    break;
                }
                jj += 1;
            }
            bindElem += 1;
            ii += 1;
        }
        nCol = -1;
        // loop over each row - start
        while (1) {
            errcode = sqlite3_step(pStmt);
            if (errcode == SQLITE_DONE) {
                errcode = SQLITE_OK;
            }
            // if no next row, then break
            if (errcode != SQLITE_ROW) {
                // cleanup pStmt
                sqlite3_finalize(pStmt);
                pStmt = NULL;
                JSBATON_ASSERT_OK();
                break;
            }
            // switch (responseType) - start
            switch (responseType) {
            case SQLITE_RESPONSETYPE_LASTBLOB:
                // export last-value as blob
                if (nCol == -1) {
                    nCol = sqlite3_column_count(pStmt);
                }
                str99->nChar = 0;
                sqlite3_str_append(str99,
                    (const char *) sqlite3_column_blob(pStmt, nCol - 1),
                    sqlite3_column_bytes(pStmt, nCol - 1));
                break;
            default:
                // insert row of column-names
                if (nCol == -1) {
                    nCol = sqlite3_column_count(pStmt);
                    if (str99->nChar > 1) {
                        sqlite3_str_appendchar(str99, 1, ',');
                        sqlite3_str_appendchar(str99, 1, '\n');
                        sqlite3_str_appendchar(str99, 1, '\n');
                    }
                    // bracket table [
                    sqlite3_str_appendchar(str99, 1, '[');
                    // bracket column [
                    sqlite3_str_appendchar(str99, 1, '[');
                    // loop over each column-name
                    ii = 0;
                    while (ii < nCol) {
                        if (ii > 0) {
                            sqlite3_str_appendchar(str99, 1, ',');
                        }
                        zTmp = sqlite3_column_name(pStmt, ii);
                        str99JsonAppend(str99, zTmp, strlen(zTmp));
                        ii += 1;
                    }
                    // bracket column ]
                    sqlite3_str_appendchar(str99, 1, ']');
                }
                // bracket row [
                sqlite3_str_appendchar(str99, 1, ',');
                sqlite3_str_appendchar(str99, 1, '\n');
                sqlite3_str_appendchar(str99, 1, '[');
                // loop over each column-value
                ii = 0;
                while (ii < nCol) {
                    if (ii > 0) {
                        sqlite3_str_appendchar(str99, 1, ',');
                    }
                    switch (sqlite3_column_type(pStmt, ii)) {
                    case SQLITE_INTEGER:
                        iTmp = sqlite3_column_int64(pStmt, ii);
                        if (JS_MIN_SAFE_INTEGER <= iTmp
                            // convert integer to double
                            && iTmp <= JS_MAX_SAFE_INTEGER) {
                            sqlite3_str_append(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        } else {
                            // convert integer to string
                            str99JsonAppend(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        }
                        break;
                    case SQLITE_FLOAT:
                        rTmp = sqlite3_column_double(pStmt, ii);
                        if (isnan(rTmp) || rTmp == -INFINITY
                            || rTmp == INFINITY) {
                            sqlite3_str_append(str99, "null", 4);
                        } else {
                            sqlite3_str_append(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        }
                        break;
                    case SQLITE_TEXT:
                        // append text as json-escaped string
                        str99JsonAppend(str99,
                            (const char *) sqlite3_column_text(pStmt, ii),
                            sqlite3_column_bytes(pStmt, ii));
                        break;
                        // case SQLITE_BLOB:
                    default:   /* case SQLITE_NULL: */
                        sqlite3_str_append(str99, "null", 4);
                        break;
                    }
                    ii += 1;
                }
                // bracket row ]
                sqlite3_str_appendchar(str99, 1, ']');
            }
            // switch (responseType) - end
        }
        // while (1)
        // loop over each row - end
        switch (responseType) {
        case SQLITE_RESPONSETYPE_LASTBLOB:
            break;
        default:
            if (nCol != -1) {
                // bracket table ]
                sqlite3_str_appendchar(str99, 1, ']');
            }
        }
    }
    // while (1)
    // loop over each table - end
    switch (responseType) {
    case SQLITE_RESPONSETYPE_LASTBLOB:
        break;
    default:
        // bracket database ]
        sqlite3_str_appendchar(str99, 1, ']');
        sqlite3_str_appendchar(str99, 1, '\n');
    }
    errcode = str99->accError;
    JSBATON_ASSERT_OK();
    // copy str99 to baton
    baton->argv[7] = str99->nChar;
    baton->bufv[7] = (int64_t) str99->zText;
  catch_error:
    // cleanup pStmt
    sqlite3_finalize(pStmt);
    // cleanup bindList
    sqlite3_free(bindList);
    // cleanup str99
    if (errcode != SQLITE_OK && str99->zText) {
        sqlite3_free(str99->zText);
    }
#ifndef EMSCRIPTEN
    // mutex leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
#endif                          // EMSCRIPTEN
}

// SQLMATH_API dbexec - end

SQLMATH_API void dbFileImportOrExport(
    Jsbaton * baton
) {
// This function will load/save <zFilename> to/from <db>
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    // call __dbFileImportOrExport()
    errcode = __dbFileImportOrExport(   //
        db,                     //
        (char *) jsbatonValueStringArgi(baton, 1),      //
        (const int) baton->argv[2]);
    JSBATON_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_API void dbNoop(
    Jsbaton * baton
) {
// this function will do nothing
    UNUSED(baton);
}

SQLMATH_API void dbOpen(
    Jsbaton * baton
) {
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
    // declare var
    int errcode = 0;
    sqlite3 *db;
    const char *filename = jsbatonValueStringArgi(baton, 0);
    const int flags = (int) baton->argv[2];
    // call c-function
    errcode = sqlite3ApiGet()->open_v2( //
        filename,               // filename
        &db,                    // db
        flags,                  // flags
        NULL);
    JSBATON_ASSERT_OK();
    dbCount += 1;
    // fprintf(stderr, "\nsqlmath.dbOpen(dbCount=%d ff=%s)\n", dbCount,
    //     filename);
    // save db
    baton->argv[0] = (int64_t) db;
  catch_error:
    (void) 0;
}

SQLMATH_API int doubleSign(
    const double aa
) {
// this function will return sign of <aa>
    return aa < 0 ? -1 : aa > 0 ? 1 : 0;
}

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
) {
// this function will compare <aa> with <bb>
    const double cc = *(double *) aa - *(double *) bb;
    return cc < 0 ? -1 : cc > 0 ? 1 : 0;
}

SQLMATH_API const char *jsbatonValueErrmsg(
    Jsbaton * baton
) {
// this function will return <baton>->errmsg
    return (const char *) baton->errmsg;
}

SQLMATH_API const char *jsbatonValueStringArgi(
    Jsbaton * baton,
    int argi
) {
// this function will return string-value from <baton> at given <argi>
    if (baton->argv[argi] == 0) {
        return NULL;
    }
    return ((const char *) baton) + ((size_t) baton->argv[argi] + 1 + 4);
}

SQLMATH_API int noop(
) {
// this function will do nothing except return 0
    return 0;
}

SQLMATH_API const char *sqlmathSnprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
) {
// this function will write <errmsg> to <buf> with additional trace-info
    if (snprintf(buf, SIZEOF_MESSAGE_DEFAULT, "%s%s\n    at %s (%s:%d)",
            prefix, errmsg, func, file, line) < 0) {
        abort();
    }
    return (const char *) buf;
}

SQLITE_API void str99JsonAppend(
    sqlite3_str * str99,
    const char *zIn,
    u32 nn
) {
/* Append the nn-byte string in zIn to the end of the JsonString string
** under construction.  Enclose the string in "..." and escape
** any double-quotes or backslash characters contained within the
** string.
*/
    u32 i;
    if (zIn == 0 || ((nn + str99->nChar + 2 >= str99->nAlloc)
            && sqlite3_str_enlarge(str99, nn + 2) == 0))
        return;
    str99->zText[str99->nChar++] = '"';
    for (i = 0; i < nn; i++) {
        unsigned char c = ((unsigned const char *) zIn)[i];
        if (c == '"' || c == '\\') {
          json_simple_escape:
            if ((str99->nChar + nn + 3 - i > str99->nAlloc)
                && sqlite3_str_enlarge(str99, nn + 3 - i) == 0)
                return;
            str99->zText[str99->nChar++] = '\\';
        } else if (c <= 0x1f) {
            static const char aSpecial[] = {
                0, 0, 0, 0, 0, 0, 0, 0, 'b', 't', 'n', 0, 'f', 'r', 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            assert(sizeof(aSpecial) == 32);
            assert(aSpecial['\b'] == 'b');
            assert(aSpecial['\f'] == 'f');
            assert(aSpecial['\n'] == 'n');
            assert(aSpecial['\r'] == 'r');
            assert(aSpecial['\t'] == 't');
            if (aSpecial[c]) {
                c = aSpecial[c];
                goto json_simple_escape;
            }
            if ((str99->nChar + nn + 7 + i > str99->nAlloc)
                && sqlite3_str_enlarge(str99, nn + 7 - i) == 0)
                return;
            str99->zText[str99->nChar++] = '\\';
            str99->zText[str99->nChar++] = 'u';
            str99->zText[str99->nChar++] = '0';
            str99->zText[str99->nChar++] = '0';
            str99->zText[str99->nChar++] = '0' + (c >> 4);
            c = "0123456789abcdef"[c & 0xf];
        }
        str99->zText[str99->nChar++] = c;
    }
    str99->zText[str99->nChar++] = '"';
    assert(str99->nChar < str99->nAlloc);
}

SQLMATH_API void str99VectorDoubleAppend(
    sqlite3_str * str99,
    double val
) {
// this function will append double <val> to vector <str99>
    sqlite3_str_append(str99, (const char *) &val, 8);
}


// file sqlmath_ext - SQLMATH_FNC
// SQLMATH_FNC sql_btobase64_func - start
static char *base64Encode(
    const unsigned char *blob,
    int *nn
) {
// this function will base64-encode <blob> to <text>
    if (blob == NULL || *nn < 0) {
        *nn = 0;
    }
    // declare var
    char *text = NULL;
    int aa = *nn - 3;
    int bb = 0;
    int ii = 0;
    int triplet = 0;
    static const char BASE64_ENCODE_TABLE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    // init bb
    bb = 4 * ceil((double) *nn / 3);
    // init text
    text = sqlite3_malloc(MAX(bb, 4));
    // handle nomem
    if (text == NULL) {
        return NULL;
    }
    // handle null-case
    if (bb < 4) {
        text[0] = '\x00';
    }
    // base64-encode loop
    while (ii < aa) {
        triplet = blob[0];
        triplet = triplet << 8 | blob[1];
        triplet = triplet << 8 | blob[2];
        blob += 3;
        text[0] = BASE64_ENCODE_TABLE[(triplet >> 18) & 0x3f];
        text[1] = BASE64_ENCODE_TABLE[(triplet >> 12) & 0x3f];
        text[2] = BASE64_ENCODE_TABLE[(triplet >> 6) & 0x3f];
        text[3] = BASE64_ENCODE_TABLE[triplet & 0x3f];
        text += 4;
        ii += 3;
    }
    // base64-encode '='
    if (bb >= 4 && blob != NULL) {
        aa += 3;
        triplet = blob[0];
        triplet = ii + 1 < aa ? triplet << 8 | blob[1] : triplet << 8;
        triplet = ii + 2 < aa ? triplet << 8 | blob[2] : triplet << 8;
        blob += 3;
        text[0] = BASE64_ENCODE_TABLE[(triplet >> 18) & 0x3f];
        text[1] = BASE64_ENCODE_TABLE[(triplet >> 12) & 0x3f];
        text[2] =
            (ii + 1 < aa) ? BASE64_ENCODE_TABLE[(triplet >> 6) & 0x3f] : '=';
        text[3] = (ii + 2 < aa) ? BASE64_ENCODE_TABLE[triplet & 0x3f] : '=';
        text += 4;
        ii += 3;
    }
    // save bb
    *nn = bb;
    // return text
    return text - bb;
}

SQLMATH_FNC static void sql_btobase64_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to base64-encoded-text
    UNUSED(argc);
    // declare var
    char *text = NULL;
    int nn = sqlite3_value_bytes(argv[0]);
    // base64-encode blob to text
    text =
        base64Encode((const unsigned char *) sqlite3_value_blob(argv[0]),
        &nn);
    // handle nomem
    if (text == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    sqlite3_result_text(context, (const char *) text, nn,
        // cleanup base64Encode()
        sqlite3_free);
}

// SQLMATH_FNC sql_btobase64_func - end

SQLMATH_FNC static void sql_btofloat64array_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to json-encoded Float64Array
    UNUSED(argc);
    // declare var
    double *arr = (double *) sqlite3_value_blob(argv[0]);
    int nn = sqlite3_value_int(argv[1]);
    if (nn < 0) {
        nn = sqlite3_value_bytes(argv[0]) / 8;
    }
    // str99 - init
    sqlite3_str __str99 = { 0 };
    sqlite3_str *str99 = &__str99;
    str99->mxAlloc = SQLITE_MAX_LENGTH2;
    // str99 - append double
    sqlite3_str_appendchar(str99, 1, '[');
    for (int ii = 0; ii < nn; ii += 1) {
        sqlite3_str_appendf(str99, ii + 1 == nn ? "%!.15g" : "%!.15g,",
            arr[ii]);
    }
    sqlite3_str_appendchar(str99, 1, ']');
    sqlite3_str_appendchar(str99, 1, '\x00');
    if (str99->accError != 0) {
        sqlite3_result_error_code(context, str99->accError);
        return;
    }
    sqlite3_result_text(context, (const char *) str99->zText, str99->nChar,
        // destructor
        sqlite3_free);
}

SQLMATH_FNC static void sql_btotext_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to text
    UNUSED(argc);
    sqlite3_result_text(context, (const char *) sqlite3_value_text(argv[0]),
        -1, SQLITE_TRANSIENT);
}


SQLMATH_FNC static void sql_castrealorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will cast <argv>[0] to double or zero
    UNUSED(argc);
    double num = sqlite3_value_double(argv[0]);
    sqlite3_result_double(context, isnormal(num) ? num : 0);
}

SQLMATH_FNC static void sql_casttextorempty_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will cast <argv>[0] to text or empty-string
    UNUSED(argc);
    switch (sqlite3_value_type(argv[0])) {
        // case SQLITE_BLOB:
        // case SQLITE_FLOAT:
        // case SQLITE_INTEGER:
    case SQLITE_NULL:
        sqlite3_result_text(context, "", 0, SQLITE_STATIC);
        return;
    case SQLITE_TEXT:
        sqlite3_result_value(context, argv[0]);
        return;
    }
    sqlite3_result_text(context, (const char *) sqlite3_value_text(argv[0]),
// ^If the 3rd parameter to the sqlite3_result_text* interfaces
// is negative, then SQLite takes result text from the 2nd parameter
// through the first zero character.
//
// ^If the 3rd parameter to the sqlite3_result_text* interfaces
// is non-negative, then as many bytes (not characters) of the text
// pointed to by the 2nd parameter are taken as the application-defined
// function result.
//
// If the 3rd parameter is non-negative, then it
// must be the byte offset into the string where the NUL terminator would
// appear if the string where NUL terminated.  If any NUL characters occur
// in the string at a byte offset that is less than the value of the 3rd
// parameter, then the resulting string will contain embedded NULs and the
// result of expressions operating on strings with embedded NULs is undefined.
        -1,
// If you insist on round-tripping through sqlite3_value_text,
// then you must pass SQLITE_TRANSIENT for the last parameter
// - the pointer returned by sqlite3_value_text is only guaranteed
// to be valid until the custom function returns.
        SQLITE_TRANSIENT);
}

SQLMATH_FNC static void sql_copyblob_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will copy blob/text/value <argv>[0]
    UNUSED(argc);
    sqlite3_result_value(context, argv[0]);
}

SQLMATH_FNC static void sql_cot_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return cot(argv[0])
    UNUSED(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        if (argv[0] == 0) {
            sqlite3_result_double(context, INFINITY);
            return;
        }
        sqlite3_result_double(context,
            1.0 / tan(sqlite3_value_double(argv[0])));
    }
}

SQLMATH_FNC static void sql_coth_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return coth(argv[0])
    UNUSED(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        if (argv[0] == 0) {
            sqlite3_result_double(context, INFINITY);
            return;
        }
        sqlite3_result_double(context,
            1.0 / tanh(sqlite3_value_double(argv[0])));
    }
}

SQLMATH_FNC static void sql_jenks_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate <kk> jenks-natrual-breaks in given <values>,
// and return a mallocd (double *) array with length (1 + kk * 2) of form:
// [
// (double) kk,
// (double) break_1, (double) count_1,
// (double) break_2, (double) count_2,
// ...,
// (double) break_k, (double) count_k
// ]
    UNUSED(argc);
    // declare var
    const double *values = (double *) sqlite3_value_blob(argv[1]);
    const int kk = sqlite3_value_int(argv[0]);
    const int nn = sqlite3_value_bytes(argv[1]) / sizeof(double);
    // jenks exec
    double *result = jenksCreate(kk, values, nn);
    if (result == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    sqlite3_result_blob(context, (void *) result,
        (1 + (int) result[0] * 2) * 8, sqlite3_free);
}

// SQLMATH_FNC sql_kthpercentile_func - start
SQLMATH_API double kthpercentile(
    double *arr,
    const int nn,
    const double kk
) {
// this function will find <kk>-th-percentile element in <arr>
// using quickselect-algorithm
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn <= 0) {
        return 0;
    }
    double aa = *arr;
    double tmp;
    int kk2 = kk * nn - 1;
    // handle kk <= 0
    if (kk2 <= 0) {
        kk2 = nn;
        while (kk2 > 0) {
            kk2 -= 1;
            aa = MIN(aa, arr[kk2]);
        }
        return aa;
    }
    // handle kk >= nn - 1
    if (kk2 + 1 >= nn) {
        kk2 = nn;
        while (kk2 > 0) {
            kk2 -= 1;
            aa = MAX(aa, arr[kk2]);
        }
        return aa;
    }
    int ii;
    int ir;
    int jj;
    int ll;
    int mid;
    ll = 0;
    ir = nn - 1;
    while (1) {
        if (ir <= ll + 1) {
            if (ir == ll + 1 && arr[ir] < arr[ll]) {
                SWAP(arr[ll], arr[ir]);
            }
            return arr[kk2];
        } else {
            mid = (ll + ir) >> 1;
            SWAP(arr[mid], arr[ll + 1]);
            if (arr[ll] > arr[ir]) {
                SWAP(arr[ll], arr[ir]);
            }
            if (arr[ll + 1] > arr[ir]) {
                SWAP(arr[ll + 1], arr[ir]);
            }
            if (arr[ll] > arr[ll + 1]) {
                SWAP(arr[ll], arr[ll + 1]);
            }
            ii = ll + 1;
            jj = ir;
            aa = arr[ll + 1];
            while (1) {
                while (1) {
                    ii += 1;
                    if (arr[ii] >= aa) {
                        break;
                    }
                }
                while (1) {
                    jj -= 1;
                    if (arr[jj] <= aa) {
                        break;
                    }
                }
                if (jj < ii) {
                    break;
                }
                SWAP(arr[ii], arr[jj]);
            }
            arr[ll + 1] = arr[jj];
            arr[jj] = aa;
            if (jj >= kk2) {
                ir = jj - 1;
            }
            if (jj <= kk2) {
                ll = ii;
            }
        }
    }
}

SQLMATH_FNC static void sql_kthpercentile_final(
    sqlite3_context * context
) {
// this function will aggregate kth-percentile element
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, 0);
    if (str99 == NULL) {
        sqlite3_result_null(context);
        return;
    }
    // str99 - result
    STR99_AGGREGATE_RESULT_ERROR();
    int nn = str99->nChar / 8 - 1;
    if (nn <= 0) {
        STR99_AGGREGATE_CLEANUP();
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_double(context,
        kthpercentile(((double *) str99->zText) + 1, nn,
            ((double *) str99->zText)[0]));
}

SQLMATH_FNC static void sql_kthpercentile_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate kth-percentile element
    UNUSED(argc);
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, sizeof(*str99));
    if (str99 == NULL) {
        return;
    }
    // str99 - init zText
    if (str99->zText == NULL) {
        str99->mxAlloc = SQLITE_MAX_LENGTH2;
        str99VectorDoubleAppend(str99, sqlite3_value_double(argv[1]));
        STR99_AGGREGATE_RESULT_ERROR();
    }
    // str99 - handle null-case
    if (sqlite3_value_numeric_type(argv[0]) == SQLITE_NULL) {
        return;
    }
    // str99 - append double
    str99VectorDoubleAppend(str99, sqlite3_value_double(argv[0]));
    STR99_AGGREGATE_RESULT_ERROR();
}

// SQLMATH_FNC sql_kthpercentile_func - end

// SQLMATH_FNC sql_marginoferror95_func - start
SQLMATH_API double marginoferror95(
    double nn,
    double pp
) {
// this function will calculate margin-of-error sqrt(pp*(1-pp)/nn)
    return 1.9599639845400542 * sqrt(pp * (1 - pp) / nn);
}

SQLMATH_FNC static void sql_marginoferror95_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will calculate margin-of-error sqrt(pp*(1-pp)/nn)
    UNUSED(argc);
    // declare var
    double nn = sqlite3_value_double(argv[0]);
    double pp = sqlite3_value_double(argv[1]);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL
        || sqlite3_value_type(argv[1]) == SQLITE_NULL || nn == 0) {
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_double(context, marginoferror95(nn, pp));
}

// SQLMATH_FNC sql_marginoferror95_func - end

// SQLMATH_FNC sql_matrix2d_concat_func - start
SQLMATH_FNC static void sql_matrix2d_concat_final(
    sqlite3_context * context
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, 0);
    if (str99 == NULL) {
        sqlite3_result_null(context);
        return;
    }
    // str99 - result
    STR99_AGGREGATE_RESULT_ERROR();
    if (str99->nChar <= 2 * 8) {
        STR99_AGGREGATE_CLEANUP();
        sqlite3_result_null(context);
        return;
    }
    double *arr = (double *) str99->zText;
    arr[0] = (0.125 * ((double) str99->nChar) - 2) / arr[1];
    sqlite3_result_blob(context, arr, str99->nChar,
        // destructor
        sqlite3_free);
}

SQLMATH_FNC static void sql_matrix2d_concat_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    UNUSED(argc);
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, sizeof(*str99));
    if (str99 == NULL) {
        return;
    }
    // str99 - init zText
    if (str99->zText == NULL) {
        str99->mxAlloc = SQLITE_MAX_LENGTH2;
        str99VectorDoubleAppend(str99, 0);
        str99VectorDoubleAppend(str99, (double) argc);
        STR99_AGGREGATE_RESULT_ERROR();
    }
    // str99 - append double
    for (int ii = 0; ii < argc; ii += 1) {
        str99VectorDoubleAppend(str99, sqlite3_value_double(argv[ii]));
    }
    STR99_AGGREGATE_RESULT_ERROR();
}

// SQLMATH_FNC sql_matrix2d_concat_func - end

SQLMATH_FNC static void sql_roundorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will round <argv>[0] to decimal <argv>[1]
    UNUSED(argc);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        sqlite3_result_double(context, 0);
        return;
    }
    // declare var
    char *zBuf = NULL;
    double rr = sqlite3_value_double(argv[0]);
    int nn = sqlite3_value_int(argv[1]);
    nn = MIN(nn, 30);
    nn = MAX(nn, 0);
    // If YY==0 and XX will fit in a 64-bit int,
    // handle the rounding directly,
    // otherwise use printf.
    if (rr < -4503599627370496.0 || rr > +4503599627370496.0) {
    } else if (nn == 0) {
        // The value has no fractional part so there is nothing to round
        rr = (double) ((sqlite_int64) (rr + (rr < 0 ? -0.5 : +0.5)));
    } else {
        zBuf = sqlite3_mprintf("%.*f", nn, rr);
        if (zBuf == NULL) {
            sqlite3_result_error_nomem(context);
            return;
        }
        rr = strtod(zBuf, NULL);
        // cleanup sqlite3_mprintf()
        sqlite3_free(zBuf);
    }
    sqlite3_result_double(context, rr);
}

SQLMATH_FNC static void sql_random1_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will generate high-quality random-float between 0 <= xx < 1
    UNUSED(argc);
    UNUSED(argv);
    static const double inv = 1.0 / 0x100000000;
    uint32_t xx[1];
    sqlite3_randomness(4, (void *) xx);
    sqlite3_result_double(context, ((double) *xx) * inv);
}

SQLMATH_FNC static void sql_sign_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** Implementation of the sign() function
** return one of 3 possibilities +1,0 or -1 when the argument is respectively
** positive, 0 or negative.
** When the argument is NULL the result is also NULL (completly conventional)
*/
    UNUSED(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        {
            double rVal = sqlite3_value_double(argv[0]);
            sqlite3_result_double(context,
                (rVal > 0) ? 1 : (rVal < 0) ? -1 : 0);
        }
        return;
    }
}

SQLMATH_FNC static void sql_squared_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** Implementation of the squared() function
** the argument is an integer.
** Since SQLite isn't strongly typed (almost untyped actually) this is a bit pedantic
*/
    UNUSED(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        {
            double rVal = sqlite3_value_double(argv[0]);
            sqlite3_result_double(context, rVal * rVal);
        }
        return;
    }
}

SQLMATH_FNC static void sql_throwerror_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return sqlite3_result_error_code(argv[0])
    UNUSED(argc);
    // declare var
    int errcode = sqlite3_value_int(argv[0]);
    if (0 <= errcode && errcode <= 28) {
        sqlite3_result_error_code(context, errcode);
        return;
    }
    sqlite3_result_error_code(context, SQLITE_INTERNAL);
}


// file sqlmath_ext - init
int sqlite3_sqlmath_ext_base_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED(pzErrMsg);
    // declare var
    int errcode = 0;
    // init sqlite3_api
    sqlite3_api = pApi;
    SQLITE3_CREATE_FUNCTION1(btobase64, 1);
    SQLITE3_CREATE_FUNCTION1(btofloat64array, 2);
    SQLITE3_CREATE_FUNCTION1(btotext, 1);
    SQLITE3_CREATE_FUNCTION1(castrealorzero, 1);
    SQLITE3_CREATE_FUNCTION1(casttextorempty, 1);
    SQLITE3_CREATE_FUNCTION1(copyblob, 1);
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(jenks, 2);
    SQLITE3_CREATE_FUNCTION1(marginoferror95, 2);
    SQLITE3_CREATE_FUNCTION1(roundorzero, 2);
    SQLITE3_CREATE_FUNCTION1(sign, 1);
    SQLITE3_CREATE_FUNCTION1(squared, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION2(kthpercentile, 2);
    SQLITE3_CREATE_FUNCTION2(matrix2d_concat, -1);
    errcode =
        sqlite3_create_function(db, "random1", 0,
        SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, sql_random1_func, NULL, NULL);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    return 0;
}
#endif                          // SQLITE3_EXT_C2
/*
file sqlmath_ext - end
*/


/*
file sqlmath_napi - start
*/
#ifdef SQLMATH_NAPI


#ifdef WIN32
#include <windows.h>
#endif
#include <node_api.h>


// file sqlmath_napi - assert
static int napiAssertOk(
    napi_env env,
    const char *func,
    const char *file,
    int line,
    int errcode
) {
// this function will throw error if <errcode> != napi_ok
// derived from https://github.com/nodejs/node-addon-api/blob/3.2.1/napi-inl.h
// typedef struct {
//   const char* error_message;
//   void* engine_reserved;
//   uint32_t engine_error_code;
//   napi_status error_code;
// } napi_extended_error_info;
    if (errcode == napi_ok) {
        return errcode;
    }
    // declare var
    char buf[SIZEOF_MESSAGE_DEFAULT] = { 0 };
    bool is_exception_pending;
    const napi_extended_error_info *info;
    napi_value val = NULL;
    // We must retrieve the last error info before doing anything else, because
    // doing anything else will replace the last error info.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26812)
#endif
    errcode = napi_get_last_error_info(env, &info);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    NAPI_ASSERT_FATAL(errcode == 0, "napi_get_last_error_info");
    errcode = napi_is_exception_pending(env, &is_exception_pending);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_is_exception_pending");
    // A pending exception takes precedence over any internal error status.
    if (is_exception_pending) {
        errcode = napi_get_and_clear_last_exception(env, &val);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_get_and_clear_last_exception");
        napi_throw(env, val);
        return errcode;
    }
    errcode =
        napi_throw_error(env, NULL, sqlmathSnprintfTrace(buf, "",
            (info->error_message !=
                NULL ? info->error_message : "error in native code"), func,
            file, line));
    NAPI_ASSERT_FATAL(errcode == 0, "napi_throw_error");
    return errcode;
}


// file sqlmath_napi - promise
static void jsbatonBufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED(env);
    UNUSED(finalize_hint);
    // cleanup baton->bufv[ii]
    sqlite3ApiGet()->free(finalize_data);
}

static Jsbaton *jsbatonCreate(
    napi_env env,
    napi_callback_info info
) {
// this function will create a baton for passing data between nodejs <-> c
    // declare var
    Jsbaton *baton = NULL;
    bool is_dataview;
    int errcode = 0;
    napi_value argv;
    size_t ii = 0;
    // init argv
    ii = 1;
    errcode = napi_get_cb_info(env, info, &ii, &argv, NULL, NULL);
    NAPI_ASSERT_OK();
    // init baton
    errcode = napi_get_element(env, argv, 0, (napi_value *) & baton);
    NAPI_ASSERT_OK();
    errcode =
        napi_get_dataview_info(env, (napi_value) baton, NULL,
        (void **) &baton, NULL, NULL);
    NAPI_ASSERT_OK();
    // save argv
    baton->napi_argv = argv;
    // init argv - external dataview
    for (ii = 0; ii < JSBATON_ARGC; ii += 1) {
        errcode = napi_get_element(env, baton->napi_argv, ii + 2, &argv);
        NAPI_ASSERT_OK();
        errcode = napi_is_dataview(env, argv, &is_dataview);
        NAPI_ASSERT_OK();
        if (is_dataview) {
            errcode =
                napi_get_dataview_info(env, argv, NULL,
                (void **) baton->argv + ii, NULL, NULL);
            NAPI_ASSERT_OK();
        }
    }
    // fprintf(stderr, "\nsqlmath.jsbatonCreate(cfuncname=%s)\n",
    //     jsbatonValueStringArgi(baton, 2 * JSBATON_ARGC));
    return baton;
}

static napi_value jsbatonExport(
    napi_env env,
    Jsbaton * baton
) {
// this function will export c-data to js-data in <jsbaton>
    // declare var
    int errcode = 0;
    napi_value val;
    size_t ii = 0;
    // export baton->argv and baton->bufv to baton->napi_argv
    while (ii < JSBATON_ARGC) {
        if (baton->bufv[ii] == 0) {
            // init argList[ii] = argv[ii]
            errcode = napi_create_bigint_int64(env, baton->argv[ii], &val);
            NAPI_ASSERT_OK();
        } else {
            // init argList[ii] = bufv[ii]
            errcode = napi_create_external_arraybuffer(env,     // napi_env env,
                (void *) baton->bufv[ii],       // void* external_data,
                (size_t) baton->argv[ii],       // size_t byte_length,
                jsbatonBufferFinalize,  // napi_finalize finalize_cb,
                NULL,           // void* finalize_hint,
                &val);          // napi_value* result
            NAPI_ASSERT_OK();
        }
        errcode = napi_set_element(env, baton->napi_argv, ii + 2, val);
        NAPI_ASSERT_OK();
        ii += 1;
    }
    return baton->napi_argv;
}

static napi_value jsstringCreate(
    napi_env env,
    const char *ss
) {
// This API creates a JavaScript string value from a UTF8-encoded C string.
// The native string is copied.
    // declare var
    int errcode = 0;
    napi_value result = NULL;
    // return result
    errcode = napi_create_string_utf8(env, ss, NAPI_AUTO_LENGTH, &result);
    NAPI_ASSERT_OK();
    return result;
}

static void jspromiseResolve(
    napi_env env,
    napi_status errcode,
    void *data
) {
// This function runs on the main thread after `jspromiseExecute` exits.
    NAPI_ASSERT_FATAL(errcode == 0, __func__);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    napi_ref ref = (napi_ref) baton->napi_argv;
    uint32_t refcount = 1;
    // dereference result to allow gc
    errcode = napi_reference_unref(env, ref, &refcount);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_reference_unref");
    NAPI_ASSERT_FATAL(refcount == 0, "memory leak");
    errcode = napi_get_reference_value(env, ref, &baton->napi_argv);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_get_reference_value");
    errcode = napi_delete_reference(env, ref);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_reference");
    // Resolve or reject the promise associated with the deferred depending on
    // whether the asynchronous action succeeded.
    if (baton->errmsg[0] == '\x00') {
        // resolve promise with result
        if (jsbatonExport(env, baton) == NULL) {
            return;
        }
        errcode =
            napi_resolve_deferred(env, baton->napi_deferred,
            baton->napi_argv);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_resolve_deferred");
    } else {
        // declare var
        napi_value err;
        // create error
        errcode =
            napi_create_error(env, NULL, jsstringCreate(env, baton->errmsg),
            &err);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_create_error");
        // reject promise with error
        errcode = napi_reject_deferred(env, baton->napi_deferred, err);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_reject_deferred");
    }
    // Clean up the work item associated with this run.
    errcode = napi_delete_async_work(env, baton->napi_work);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_async_work");
    // Set both values to NULL so JavaScript can order a new run of the thread.
    baton->napi_work = NULL;
    baton->napi_deferred = NULL;
}

static napi_value jspromiseCreate(
    napi_env env,
    napi_callback_info info,
    napi_async_execute_callback jspromiseExecute
) {
// Create a deferred promise and an async queue work item.
    // init baton
    Jsbaton *baton = jsbatonCreate(env, info);
    if (baton == NULL) {
        return NULL;
    }
    // declare var
    char buf[SIZEOF_MESSAGE_DEFAULT] = { 0 };
    int errcode = 0;
    napi_value promise = 0;
    napi_value async_resource_name;
    // reference result to prevent gc
    errcode = napi_create_reference(env,        // napi_env env
        baton->napi_argv,       // napi_value value
        1,                      // uint32_t initial_refcount
        (napi_ref *) & baton->napi_argv);       // napi_ref* result
    NAPI_ASSERT_OK();
    // Ensure that no work is currently in progress.
    if (baton->napi_work != NULL) {
        napi_throw_error(env, NULL, sqlmathSnprintfTrace(buf, "",
                "sqlmath.jspromiseCreate()"
                " - Only one work item must exist at a time", __func__,
                __FILE__, __LINE__));
        return NULL;
    }
    // Create a deferred promise which we will resolve at the completion of the
    // work.
    errcode = napi_create_promise(env, &(baton->napi_deferred), &promise);
    NAPI_ASSERT_OK();
    // init async_resource_name
    errcode =
        napi_create_string_utf8(env,
        "sqlmath.jspromiseCreate() - napi_create_async_work()",
        NAPI_AUTO_LENGTH, &async_resource_name);
    // Create an async work item, passing in the addon data, which will give the
    // worker thread access to the above-created deferred promise.
    errcode = napi_create_async_work(env,       // napi_env env,
        NULL,                   // napi_value async_resource,
        async_resource_name,    // napi_value async_resource_name,
        jspromiseExecute,       // napi_async_execute_callback execute,
        jspromiseResolve,       // napi_async_complete_callback complete,
        baton,                  // void* data,
        &(baton->napi_work));   // napi_async_work* result);
    NAPI_ASSERT_OK();
    // Queue the work item for execution.
    errcode = napi_queue_async_work(env, baton->napi_work);
    NAPI_ASSERT_OK();
    // This causes created `promise` to be returned to JavaScript.
    return promise;
}


// file sqlmath_napi - init
NAPI_JSPROMISE_CREATE(dbClose);
NAPI_JSPROMISE_CREATE(dbExec);
NAPI_JSPROMISE_CREATE(dbFileImportOrExport);
NAPI_JSPROMISE_CREATE(dbNoop);
NAPI_JSPROMISE_CREATE(dbOpen);

napi_value napi_module_sqlmath_init(
    napi_env env,
    napi_value exports
) {
// typedef struct {
//   // One of utf8name or name should be NULL.
//   const char* utf8name;
//   napi_value name;
//
//   napi_callback method;
//   napi_callback getter;
//   napi_callback setter;
//   napi_value value;
//
//   napi_property_attributes attributes;
//   void* data;
// } napi_property_descriptor;
    // coverage-hack
    noop();
    // declare var
    int errcode = 0;
    const napi_property_descriptor propList[] = {
        NAPI_EXPORT_MEMBER(_dbClose),
        NAPI_EXPORT_MEMBER(_dbExec),
        NAPI_EXPORT_MEMBER(_dbFileImportOrExport),
        NAPI_EXPORT_MEMBER(_dbNoop),
        NAPI_EXPORT_MEMBER(_dbOpen),
    };
    errcode = napi_define_properties(env, exports,
        sizeof(propList) / sizeof(napi_property_descriptor), propList);
    NAPI_ASSERT_OK();
    // init sqlite
    errcode = sqlite3_initialize();
    NAPI_ASSERT_OK();
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_sqlmath_init)
#endif                          // SQLMATH_NAPI
/*
file sqlmath_napi - end
*/
