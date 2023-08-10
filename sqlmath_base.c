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
#define SQLMATH_H2
#if defined(SQLMATH_H2) && !defined(SQLMATH_H3)
#define SQLMATH_H3


#define NAPI_VERSION 6
#ifdef SRC_SQLITE_BASE_C2
#   undef SRC_SQLITE_BASE_C2
#   ifndef SQLMATH_BASE_C2
#       define SQLMATH_BASE_C2
#   endif
#endif


#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite_rollup.c"


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
#define MATH_PI 3.141592653589793238463
#define MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
#define SGN(aa) (((aa) < 0) ? -1 : ((aa) > 0) ? 1 : 0)
#define SIZEOF_MESSAGE_DEFAULT 256
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_OFFSET          768
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       0x71
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
#define SQLITE_ERROR_DATATYPE_INVALID   0x10003
#define SQLITE_ERROR_JSON_ARRAY_INVALID         0x71
#define SQLITE_ERROR_ZSQL_NULL          0x10004
#define SQLITE_MAX_LENGTH2 1000000000
#define SQLITE_RESPONSETYPE_LASTBLOB 1
#define SQLMATH_API
#define SQLMATH_FUNC
#define SWAP(aa, bb, tmp) tmp = (aa); (aa) = (bb); (bb) = tmp
#define UNUSED_PARAMETER(x) ((void)(x))


// This function will exec <sql> and if <errcode> is not ok,
// throw <baton>->errmsg with given sqlite-<errcode>.
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

// This function will if <cond> is falsy, terminate process with <msg>.
#define NAPI_ASSERT_FATAL(cond, msg) \
    if (!(cond)) { \
        napi_fatal_error(__func__, NAPI_AUTO_LENGTH , msg, NAPI_AUTO_LENGTH); \
    }

// This function will assert <errcode> == napi_ok in <env>.
#define NAPI_ASSERT_OK() \
    if (0 != napiAssertOk(env, __func__, __FILE__, __LINE__, errcode)) { \
        return NULL; \
    }

#define NAPI_EXPORT_MEMBER(name) \
    {#name, NULL, name, NULL, NULL, NULL, napi_default, NULL}

#define NAPI_JSPROMISE_CREATE(func) \
    static void __##napi##_##func(napi_env env, void *data) { \
        UNUSED_PARAMETER(env); \
        func((Jsbaton *) data); \
    } \
    static napi_value _##func(napi_env env, napi_callback_info info) { \
        return jspromiseCreate(env, info, __##napi##_##func); \
    } \

#define SQLITE3_AGGREGATE_CONTEXT(type) \
    type *agg = (type *) sqlite3_aggregate_context(context, sizeof(*agg)); \
    if (agg == NULL) { \
        sqlite3_result_error_nomem(context); \
        goto catch_error; \
    }

#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql1_##func##_func, NULL, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_CREATE_FUNCTION2(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        NULL, sql2_##func##_step, sql2_##func##_final); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_CREATE_FUNCTION3(func, argc) \
    errcode = sqlite3_create_window_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql3_##func##_step, sql3_##func##_final, \
        sql3_##func##_value, sql3_##func##_inverse, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }

#define STR99_ALLOCA(str99) \
    sqlite3_str __##str99 = { 0 }; \
    sqlite3_str *str99 = &__##str99; \
    str99->mxAlloc = SQLITE_MAX_LENGTH2; \

#define STR99_RESULT_ERROR(str99) \
    errcode = sqlite3_str_errcode(str99); \
    if (errcode == SQLITE_ERROR_JSON_ARRAY_INVALID) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error(context, \
            "str99ArrayAppendJsonarray() - invalid JSON array", -1); \
        goto catch_error; \
    } \
    if (errcode) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error_code(context, errcode); \
        goto catch_error; \
    } \
    if (sqlite3_str_length(str99) <= 0) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_null(context); \
        goto catch_error; \
    }

#define VECTOR99_AGGREGATE_CONTEXT(nhead) \
    Vector99 **vec99_agg = \
        (Vector99 **) sqlite3_aggregate_context(context, sizeof(*vec99_agg)); \
    if (vec99_agg == NULL) { \
        sqlite3_result_error_nomem(context); \
        return; \
    } \
    Vector99 *vec99 = *vec99_agg; \
    if (vec99 == NULL) { \
        vec99 = vector99_malloc(nhead); \
        if (vec99 == NULL) { \
            sqlite3_result_error_nomem(context); \
            return; \
        } \
        *vec99_agg = vec99; \
    } \
    double *vec99_body = vector99_body(vec99); \
    double *vec99_head = vector99_head(vec99); \
    int errcode = 0; \
    UNUSED_PARAMETER(errcode); \
    UNUSED_PARAMETER(vec99_body); \
    UNUSED_PARAMETER(vec99_head);

#define VECTOR99_AGGREGATE_PUSH(xx) \
    errcode = vector99_agg_push(vec99_agg, xx); \
    if (errcode) { \
        sqlite3_result_error_code(context, errcode); \
        goto catch_error; \
    } \
    vec99 = *vec99_agg; \
    vec99_body = vector99_body(vec99); \
    vec99_head = vector99_head(vec99);

// file sqlmath_h - sqlite3
// *INDENT-OFF*
SQLITE_API const sqlite3_api_routines *sqlite3ApiGet();
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;


// file sqlmath_h - db
typedef struct Jsbaton {
    int32_t nallc;              // offset - 0-4
    int32_t nused;              // offset - 4-8
    int64_t argv[JSBATON_ARGC]; // offset - 8-136
    int64_t bufv[JSBATON_ARGC]; // offset - 136-264
    int64_t cfuncname;          // offset - 264-272
    char errmsg[SIZEOF_MESSAGE_DEFAULT];        // offset 272-528
    void *napi_argv;
    void *napi_work;
    void *napi_deferred;
} Jsbaton;
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
// *INDENT-ON*
SQLITE_API void str99JsonAppendText(
    sqlite3_str * str99,
    const char *zIn,
    u32 nn
);
SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double xx
);
SQLMATH_API void str99ArrayAppendJsonarray(
    sqlite3_str * str99,
    const char *json,
    int nn
);
SQLMATH_API void str99ResultBlob(
    sqlite3_str * str99,
    sqlite3_context * context
);
SQLMATH_API void str99ResultText(
    sqlite3_str * str99,
    sqlite3_context * context
);


// file sqlmath_h - vector99
typedef struct Vector99 {
    double alloc;               // allocated size in bytes
    double nbody;               // number of body elements
    double nhead;               // number of head elements
    //
    double ncol;                // number of columns
    double wii;                 // counter of window elements
    double wnn;                 // number of window elements
} Vector99;
SQLMATH_API void vector99_agg_free(
    Vector99 ** vec99_agg
);
SQLMATH_API int vector99_agg_push(
    Vector99 ** vec99_agg,
    double xx
);
SQLMATH_API double *vector99_body(
    const Vector99 * vec99
);
SQLMATH_API Vector99 *vector99_from_json(
    const char *json,
    int nn
);
SQLMATH_API double *vector99_head(
    const Vector99 * vec99
);
SQLMATH_API Vector99 *vector99_malloc(
    const int nhead
);
SQLMATH_API void vector99_result_blob(
    Vector99 * vec99,
    sqlite3_context * context
);
SQLMATH_API int vector99_valid(
    const Vector99 * vec99
);


// file sqlmath_h - SQLMATH_API
SQLMATH_API double doubleAbs(
    const double aa
);

SQLMATH_API double doubleMax(
    const double aa,
    const double bb
);

SQLMATH_API double doubleMin(
    const double aa,
    const double bb
);

SQLMATH_API int doubleSign(
    const double aa
);

SQLMATH_API void doublearrayResult(
    sqlite3_context * context,
    const double *arr,
    const int nn,
    void (*xDel) (void *)       // NOLINT
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

SQLMATH_API void jsonResultDoublearray(
    sqlite3_context * context,
    const double *arr,
    int nn
);

SQLMATH_API double marginoferror95(
    double nn,
    double pp
);

SQLMATH_API int noop(
);

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double pp
);

SQLMATH_API double sqlite3_value_double_or_nan(
    sqlite3_value * arg
);

SQLMATH_API double sqlite3_value_double_or_prev(
    sqlite3_value * arg,
    double *previous
);

SQLMATH_API const char *sqlmathSnprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
);


#endif                          // SQLMATH_H3
/*
file sqlmath_h - end
*/


/*
file sqlmath_base - start
*/
#if defined(SQLMATH_BASE_C2) && !defined(SQLMATH_BASE_C3)
#define SQLMATH_BASE_C3


// track how many sqlite-db open
static int dbCount = 0;


// file sqlmath_base - SQLMATH_API

// SQLMATH_API db - start
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

// SQLMATH_API dbExec - start
typedef struct DbExecBindElem {
    const char *buf;
    char *key;
    int buflen;
    char datatype;
} DbExecBindElem;

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
    STR99_ALLOCA(str99);
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
                sqlite3_str_reset(str99);
                sqlite3_str_append(str99,
                    (const char *) sqlite3_column_blob(pStmt, nCol - 1),
                    sqlite3_column_bytes(pStmt, nCol - 1));
                break;
            default:
                // insert row of column-names
                if (nCol == -1) {
                    nCol = sqlite3_column_count(pStmt);
                    if (sqlite3_str_length(str99) > 1) {
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
                        str99JsonAppendText(str99, zTmp,
                            (uint32_t) strlen(zTmp));
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
                            str99JsonAppendText(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        }
                        break;
                    case SQLITE_FLOAT:
                        rTmp = sqlite3_column_double(pStmt, ii);
                        if (!isfinite(rTmp)) {
                            sqlite3_str_append(str99, "null", 4);
                        } else {
                            sqlite3_str_append(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        }
                        break;
                    case SQLITE_TEXT:
                        // append text as json-escaped string
                        str99JsonAppendText(str99,
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
    errcode = sqlite3_str_errcode(str99);
    JSBATON_ASSERT_OK();
    // copy str99 to baton
    baton->argv[7] = sqlite3_str_length(str99);
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

// SQLMATH_API dbExec - end

SQLMATH_API void dbFileImportOrExport(
    Jsbaton * baton
) {
// This function will load/save <zFilename> to/from <db>.
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
// This function will do nothing.
    UNUSED_PARAMETER(baton);
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
    errcode = sqlite3_open_v2(  //
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

// SQLMATH_API db - end

// SQLMATH_API str99 - start
SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double xx
) {
// This function will append double <xx> to <str99>.
    sqlite3_str_append(str99, (const char *) &xx, 8);
}

SQLMATH_API void str99ArrayAppendJsonarray(
    sqlite3_str * str99,
    const char *json,
    int nn
) {
// This function will append float-values from json-encoded-flat-array.
    // declare var
    int ii = 0;
    int jj = 0;
    // validate json
    for (; ii < nn; ii += 1) {
        if (json[ii] == '[') {
            break;
        }
    }
    for (; nn > ii; nn -= 1) {
        if (json[nn - 1] == ']') {
            break;
        }
    }
    if (nn <= ii) {
        goto catch_error_json;
    }
    jj = ii;
    // str99 - append double
    for (; ii < nn; ii += 1) {
        // skip whitespace
        switch (json[jj]) {
        case '[':
        case '\x09':
        case '\x0a':
        case '\x0d':
        case '\x20':
            jj = ii;
            break;
        default:
            switch (json[ii]) {
            case ',':
            case ']':
                str99ArrayAppendDouble(str99, atof(json + jj));
                jj = ii + 1;
                break;
            }
            break;
        }
        switch (json[jj]) {
        case ',':
            goto catch_error_json;
        case ']':
            if (sqlite3_str_length(str99) > 0) {
                goto catch_error_json;
            }
            break;
        }
    }
    return;
  catch_error_json:
    sqlite3_str_reset(str99);
    str99->accError = SQLITE_ERROR_JSON_ARRAY_INVALID;
}

SQLMATH_API void str99ResultBlob(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// This function will return <str99> as result-blob in given <context>.
    sqlite3_result_blob(context, (const char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}

SQLMATH_API void str99ResultText(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// This function will return <str99> as result-text in given <context>.
    sqlite3_result_text(context, (const char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}

// SQLMATH_API str99 - end

// SQLMATH_API vector99 - start
SQLMATH_API void vector99_agg_free(
    Vector99 ** vec99_agg
) {
    if (vec99_agg != NULL) {
        sqlite3_free(*vec99_agg);
        *vec99_agg = NULL;
    }
}

SQLMATH_API int vector99_agg_push(
    Vector99 ** vec99_agg,
    double xx
) {
    if (vec99_agg == NULL || *vec99_agg == NULL) {
        return SQLITE_NOMEM;
    }
    Vector99 *vec99 = *vec99_agg;
    // rotate wnn
    if (vec99->wnn) {
        vector99_body(vec99)[(int) vec99->wii] = xx;
        vec99->wii += 1;
        if (vec99->wii >= vec99->wnn) {
            vec99->wii -= vec99->wnn;
        }
        return SQLITE_OK;
    }
    const int nn =
        sizeof(Vector99) / sizeof(double) + vec99->nhead + vec99->nbody;
    uint32_t alloc = vec99->alloc;
    if (nn * sizeof(double) >= alloc) {
        // error - toobig
        if (alloc >= SQLITE_MAX_LENGTH2) {
            goto catch_error;
        }
        alloc = MIN(SQLITE_MAX_LENGTH2, alloc * 2);
        vec99 = sqlite3_realloc(vec99, alloc);
        // error - nomem
        if (vec99 == NULL) {
            goto catch_error;
        }
        vec99->alloc = alloc;
        *vec99_agg = vec99;
    }
    ((double *) vec99)[nn] = xx;
    vec99->nbody += 1;
    return SQLITE_OK;
  catch_error:
    vector99_agg_free(vec99_agg);
    return SQLITE_NOMEM;
}

SQLMATH_API double *vector99_body(
    const Vector99 * vec99
) {
    return (double *) ((char *) vec99 + sizeof(Vector99)) +
        (int) vec99->nhead;
}

SQLMATH_API Vector99 *vector99_from_json(
    const char *json,
    int nn
) {
    // declare var
    int ii = 0;
    int jj = 0;
    // validate json
    for (; ii < nn; ii += 1) {
        if (json[ii] == '[') {
            break;
        }
    }
    for (; nn > ii; nn -= 1) {
        if (json[nn - 1] == ']') {
            break;
        }
    }
    if (nn <= ii) {
        return NULL;
    }
    jj = ii;
    // init vec99_agg
    Vector99 *vec99_agg[1] = { 0 };
    *vec99_agg = vector99_malloc(0);
    if (*vec99_agg == NULL) {
        return NULL;
    }
    // str99 - append double
    for (; ii < nn; ii += 1) {
        // skip whitespace
        switch (json[jj]) {
        case '[':
        case '\x09':
        case '\x0a':
        case '\x0d':
        case '\x20':
            jj = ii;
            break;
        default:
            switch (json[ii]) {
            case ',':
            case ']':
                vector99_agg_push(vec99_agg, atof(json + jj));
                jj = ii + 1;
                break;
            }
            break;
        }
        switch (json[jj]) {
        case ',':
            goto catch_error_json;
        case ']':
            if ((*vec99_agg)->nbody > 0) {
                goto catch_error_json;
            }
            break;
        }
    }
    return *vec99_agg;
  catch_error_json:
    vector99_agg_free(vec99_agg);
    return NULL;
}

SQLMATH_API double *vector99_head(
    const Vector99 * vec99
) {
    return (double *) ((char *) vec99 + sizeof(Vector99));
}

SQLMATH_API Vector99 *vector99_malloc(
    const int nhead
) {
    const int alloc = sizeof(Vector99) + nhead * sizeof(double) + 256;
    if (alloc > SQLITE_MAX_LENGTH2) {
        return NULL;
    }
    Vector99 *vec99 = sqlite3_malloc(alloc);
    if (vec99 == NULL) {
        return NULL;
    }
    // zero vec99
    memset(vec99, 0, alloc);
    vec99->alloc = alloc;
    vec99->nhead = doubleMax(0, nhead);
    return vec99;
}

SQLMATH_API void vector99_result_blob(
    Vector99 * vec99,
    sqlite3_context * context
) {
// This function will return <vec99> as result-blob in given <context>.
    vec99->alloc =
        sizeof(Vector99) + (vec99->nhead + vec99->nbody) * sizeof(double);
    sqlite3_result_blob(context, (const char *) vec99, vec99->alloc,
        // destructor
        sqlite3_free);
}

SQLMATH_API int vector99_valid(
    const Vector99 * vec99
) {
// This function will validate <vec99>.
    if (vec99 == NULL) {
        return 0;
    }
    const double alloc = vec99->alloc;
    const double nbody = vec99->nbody;
    return (alloc >= sizeof(*vec99) + nbody * sizeof(double)
        && alloc <= SQLITE_MAX_LENGTH2 && nbody >= 0
        && nbody <= SQLITE_MAX_LENGTH2);
}

// SQLMATH_API vector99 - end

SQLMATH_API double doubleAbs(
    const double aa
) {
// This function will return abs of <aa>.
    return aa < 0 ? -aa : aa;
}

SQLMATH_API double doubleMax(
    const double aa,
    const double bb
) {
// This function will return max of <aa>, <bb>.
    return aa > bb ? aa : bb;
}

SQLMATH_API double doubleMin(
    const double aa,
    const double bb
) {
// This function will return min of <aa>, <bb>.
    return aa < bb ? aa : bb;
}

SQLMATH_API int doubleSign(
    const double aa
) {
// This function will return sign of <aa>.
    return aa < 0 ? -1 : aa > 0 ? 1 : 0;
}

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
) {
// This function will compare <aa> with <bb>.
    const double cc = *(double *) aa - *(double *) bb;
    return cc < 0 ? -1 : cc > 0 ? 1 : 0;
}

SQLMATH_API void doublearrayResult(
    sqlite3_context * context,
    const double *arr,
    const int nn,
    void (*xDel) (void *)       // NOLINT
) {
// This function will return double *<arr> as binary-double-array
// in given <context>.
    sqlite3_result_blob(context, arr, nn * sizeof(double), xDel);
}

SQLMATH_API const char *jsbatonValueErrmsg(
    Jsbaton * baton
) {
// This function will return <baton>->errmsg.
    return (const char *) baton->errmsg;
}

SQLMATH_API const char *jsbatonValueStringArgi(
    Jsbaton * baton,
    int argi
) {
// This function will return string-value from <baton> at given <argi>.
    if (baton->argv[argi] == 0) {
        return NULL;
    }
    return ((const char *) baton) + ((size_t) baton->argv[argi] + 1 + 4);
}

SQLMATH_API void jsonResultDoublearray(
    sqlite3_context * context,
    const double *arr,
    int nn
) {
// This function will return double *<arr> as json-result-text
// in given <context>.
    STR99_ALLOCA(str99);
    sqlite3_str_appendchar(str99, 1, '[');
    while (1) {
        nn -= 1;
        if (nn <= 0) {
            break;
        }
        // append with comma
        sqlite3_str_appendf(str99, isfinite(*arr) ? "%!.15g," : "null,",
            *arr);
        arr += 1;
    }
    if (nn == 0) {
        // append with no comma
        sqlite3_str_appendf(str99, isfinite(*arr) ? "%!.15g" : "null", *arr);
    }
    sqlite3_str_appendchar(str99, 1, ']');
    const int errcode = sqlite3_str_errcode(str99);
    if (errcode) {
        sqlite3_str_reset(str99);
        sqlite3_result_error_code(context, errcode);
        return;
    }
    if (sqlite3_str_length(str99) <= 0) {
        sqlite3_str_reset(str99);
        sqlite3_result_null(context);
        return;
    }
    str99ResultText(str99, context);
}

SQLMATH_API int noop(
) {
// This function will do nothing except return 0.
    return 0;
}

SQLMATH_API double sqlite3_value_double_or_nan(
    sqlite3_value * arg
) {
// This function will return NAN if NULL else double.
    return sqlite3_value_type(arg) ==
        SQLITE_NULL ? NAN : sqlite3_value_double(arg);
}

SQLMATH_API double sqlite3_value_double_or_prev(
    sqlite3_value * arg,
    double *previous
) {
// This function will save <arg> as <previous> if not NULL.
    if (sqlite3_value_type(arg) != SQLITE_NULL) {
        *previous = sqlite3_value_double(arg);
    }
    return *previous;
}

SQLMATH_API const char *sqlmathSnprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
) {
// This function will write <errmsg> to <buf> with additional trace-info.
    if (snprintf(buf, SIZEOF_MESSAGE_DEFAULT, "%s%s\n    at %s (%s:%d)",
            prefix, errmsg, func, file, line) < 0) {
        abort();
    }
    return (const char *) buf;
}


// file sqlmath_base - SQLMATH_FUNC
SQLMATH_FUNC static void sql1_castrealornull_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will cast <argv>[0] to double or zero.
    UNUSED_PARAMETER(argc);
    sqlite3_result_double(context, sqlite3_value_double_or_nan(argv[0]));
}

SQLMATH_FUNC static void sql1_castrealorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will cast <argv>[0] to double or zero.
    UNUSED_PARAMETER(argc);
    const double xx = sqlite3_value_double(argv[0]);
    sqlite3_result_double(context, isfinite(xx) ? xx : 0);
}

SQLMATH_FUNC static void sql1_casttextorempty_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will cast <argv>[0] to text or empty-string.
    UNUSED_PARAMETER(argc);
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

SQLMATH_FUNC static void sql1_copyblob_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will copy blob/text/value <argv>[0].
    UNUSED_PARAMETER(argc);
    sqlite3_result_value(context, argv[0]);
}

SQLMATH_FUNC static void sql1_cot_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return cot(argv[0]).
    UNUSED_PARAMETER(argc);
    sqlite3_result_double(context,
        1.0 / tan(sqlite3_value_double_or_nan(argv[0])));
}

SQLMATH_FUNC static void sql1_coth_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return coth(argv[0]).
    UNUSED_PARAMETER(argc);
    sqlite3_result_double(context,
        1.0 / tanh(sqlite3_value_double_or_nan(argv[0])));
}

SQLMATH_FUNC static void sql1_doublearray_array_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return binary-double-array from <argv>.
    if (argc <= 0) {
        sqlite3_result_null(context);
        return;
    }
    double *arr = sqlite3_malloc(argc * sizeof(double));
    if (arr == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    for (int ii = 0; ii < argc; ii += 1) {
        arr[ii] = sqlite3_value_double_or_nan(argv[ii]);
    }
    sqlite3_result_blob(context, (const char *) arr, argc * sizeof(double),
        // destructor
        sqlite3_free);
}

SQLMATH_FUNC static void sql1_doublearray_extract_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will extract double-value <xx> from <argv>[0]
// at position <argv>[ii]. If <xx> is not finite, then return NULL.
    UNUSED_PARAMETER(argc);
    const int ii = sqlite3_value_int(argv[1]);
    const int nn = sqlite3_value_bytes(argv[0]) / sizeof(double);
    if (0 <= ii && ii < nn) {
        const double xx = ((const double *) sqlite3_value_blob(argv[0]))[ii];
        if (isfinite(xx)) {
            sqlite3_result_double(context, xx);
            return;
        }
    }
    sqlite3_result_null(context);
}

SQLMATH_FUNC static void sql1_doublearray_jsonfrom_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return binary-double-array from json-encoded-flat-array.
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - to-array
    STR99_ALLOCA(str99);
    str99ArrayAppendJsonarray(  //
        str99,                  // array
        (const char *) sqlite3_value_blob(argv[0]),     // json
        sqlite3_value_bytes(argv[0]));  // nn
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultBlob(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_doublearray_jsonto_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return json-encoded-flat-array from binary-double-array.
    UNUSED_PARAMETER(argc);
    jsonResultDoublearray(context,
        (const double *) sqlite3_value_blob(argv[0]),
        sqlite3_value_bytes(argv[0]) / sizeof(double));
}

// SQLMATH_FUNC sql1_marginoferror95_func - start
SQLMATH_API double marginoferror95(
    double nn,
    double pp
) {
// This function will calculate margin-of-error sqrt(pp*(1-pp)/nn).
    return 1.9599639845400542 * sqrt(pp * (1 - pp) / nn);
}

SQLMATH_FUNC static void sql1_marginoferror95_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate margin-of-error sqrt(pp*(1-pp)/nn).
    UNUSED_PARAMETER(argc);
    sqlite3_result_double(context,
        marginoferror95(sqlite3_value_double_or_nan(argv[0]),
            sqlite3_value_double_or_nan(argv[1])));
}

// SQLMATH_FUNC sql1_marginoferror95_func - end

SQLMATH_FUNC static void sql1_random1_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will generate high-quality random-float between 0 <= xx < 1.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    static const double inv = 1.0 / 0x100000000;
    uint32_t xx[1];
    sqlite3_randomness(4, (void *) xx);
    sqlite3_result_double(context, ((double) *xx) * inv);
}

SQLMATH_FUNC static void sql1_roundorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will round <argv>[0] to decimal <argv>[1].
    UNUSED_PARAMETER(argc);
    if (sqlite3_value_numeric_type(argv[0]) == SQLITE_NULL) {
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

SQLMATH_FUNC static void sql1_sign_func(
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
    UNUSED_PARAMETER(argc);
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

SQLMATH_FUNC static void sql1_squared_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** Implementation of the squared() function
** the argument is an integer.
** Since SQLite isn't strongly typed (almost untyped actually) this is a bit pedantic
*/
    UNUSED_PARAMETER(argc);
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

SQLMATH_FUNC static void sql1_throwerror_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return sqlite3_result_error_code(argv[0]).
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = sqlite3_value_int(argv[0]);
    if (0 <= errcode && errcode <= 28) {
        sqlite3_result_error_code(context, errcode);
        return;
    }
    sqlite3_result_error_code(context, SQLITE_INTERNAL);
}

// SQLMATH_FUNC sql2_quantile_func - start
static double quickselect(
    double *arr,
    const int nn,
    const int kk
) {
// This function will find <kk>-th element in <arr> using quickselect-algorithm.
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn <= 0) {
        return NAN;
    }
    double aa = *arr;
    double tmp = 0;
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
                SWAP(arr[ll], arr[ir], tmp);
            }
            return arr[kk];
        } else {
            mid = (ll + ir) >> 1;
            SWAP(arr[mid], arr[ll + 1], tmp);
            if (arr[ll] > arr[ir]) {
                SWAP(arr[ll], arr[ir], tmp);
            }
            if (arr[ll + 1] > arr[ir]) {
                SWAP(arr[ll + 1], arr[ir], tmp);
            }
            if (arr[ll] > arr[ll + 1]) {
                SWAP(arr[ll], arr[ll + 1], tmp);
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
                SWAP(arr[ii], arr[jj], tmp);
            }
            arr[ll + 1] = arr[jj];
            arr[jj] = aa;
            if (jj >= kk) {
                ir = jj - 1;
            }
            if (jj <= kk) {
                ll = ii;
            }
        }
    }
}

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double qq
) {
// This function will find <qq>-th-quantile element in <arr>
// using quickselect-algorithm.
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (!(nn >= 1)) {
        return NAN;
    }
    double kmod = MAX(0, MIN(1, qq)) * (nn - 1);
    const int kk = kmod;
    kmod = fmod(kmod, 1);
    return kmod == 0            //
        ? quickselect(arr, nn, kk)      //
        : (1 - kmod) * quickselect(arr, nn, kk) + kmod * quickselect(arr, nn,
        kk + 1);
}

SQLMATH_FUNC static void sql2_quantile_final(
    sqlite3_context * context
) {
// This function will aggregate kth-quantile element.
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - null-case
    if (vec99->nbody == 0) {
        sqlite3_result_null(context);
        goto catch_error;
    }
    sqlite3_result_double(context, quantile(vec99_body, vec99->nbody,
            vec99_head[0]));
  catch_error:
    vector99_agg_free(vec99_agg);
}

static void sql2_quantile_step0(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv,
    const double qq
) {
// This function will aggregate kth-quantile element.
    UNUSED_PARAMETER(argc);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(1);
    if (vec99->nbody == 0) {
        vec99_head[0] = qq;     // kth-quantile
    }
    // vec99 - append isfinite
    const double xx = sqlite3_value_double_or_nan(argv[0]);
    if (!isnan(xx)) {
        VECTOR99_AGGREGATE_PUSH(xx);
    }
    return;
  catch_error:
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql2_quantile_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate kth-quantile element.
    sql2_quantile_step0(context, argc, argv, sqlite3_value_double(argv[1]));
}

SQLMATH_FUNC static void sql2_median_final(
    sqlite3_context * context
) {
    sql2_quantile_final(context);
}

SQLMATH_FUNC static void sql2_median_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate kth-quantile element.
    sql2_quantile_step0(context, argc, argv, 0.5);
}

// SQLMATH_FUNC sql2_quantile_func - end

// SQLMATH_FUNC sql2_stdev_func - start
typedef struct AggStdev {
    double mxx;                 // x-average
    double nnn;                 // number of elements
    double sxx;                 // xx-variance.p
} AggStdev;

SQLMATH_FUNC static void sql2_stdev_final(
    sqlite3_context * context
) {
// This function will aggregate elements and calculate sample stdev.
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggStdev);
    // agg - null-case
    if (agg->nnn <= 0) {
        sqlite3_result_null(context);
        goto catch_error;
    }
    sqlite3_result_double(context,
        agg->nnn == 1 ? 0 : sqrt(agg->sxx / (agg->nnn - 1)));
  catch_error:
    (void) 0;
}

static void sql2_stdev_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate elements and calculate stdev.
    UNUSED_PARAMETER(argc);
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggStdev);
    // agg - welford - increment agg->sxx
    if (sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL) {
        const double xx = sqlite3_value_double(argv[0]);
        const double dxx0 = xx - agg->mxx;
        agg->nnn += 1;
        agg->mxx += dxx0 / agg->nnn;
        agg->sxx += dxx0 * (xx - agg->mxx);
    }
  catch_error:
    (void) 0;
}

// SQLMATH_FUNC sql2_stdev_func - end

// SQLMATH_FUNC sql3_win_cosfit2_func - start
typedef struct WinCosfit {
    double nnn;                 // 00 number of elements
    double xx1;                 // 01 x-actual
    double yy1;                 // 02 y-actual
    //
    double mee;                 // 03 y-stdev.s1
    double myy;                 // 04 y-average
    double mxx;                 // 05 x-average
    double mxe;                 // 06 x-stdev.s1
    //
    double lee;                 // 07 y-stdev.s2 linest
    double lyy;                 // 08 y-estimate linest
    double laa;                 // 09 linest y-intercept
    double lbb;                 // 10 linest slope
    double lxy;                 // 11 linest pearson-correlation xy
    //
    double cee;                 // 12 y-stdev.s5 cosine
    double cyy;                 // 13 y-estimate cosine
    double caa;                 // 14 cosine amplitude
    double cww;                 // 15 cosine angular-frequency
    double cpp;                 // 16 cosine phase
    double ctt;                 // 17 cosine period
    double ctp;                 // 18 cosine period-phase
    //
    double inv0;                // 1.0 / (nnn - 0)
    double inv1;                // 1.0 / (nnn - 1)
    double inv2;                // 1.0 / (nnn - 2)
    double vxx;                 // y-variance.p
    double vxy;                 // xy-covariance.p
    double vyy;                 // y-variance.p
    double xx0;                 // trailing-window-x
    double yy0;                 // trailing-window-y
} WinCosfit;
static const int WinCosfitN = sizeof(WinCosfit) / sizeof(double);

static void winCosfitCsr(
    WinCosfit * wcf,
    const int nbody,
    const int ncol,
    const int icol
) {
// This function will calculate running cosine-regression as:
//     yy = caa*cos(cww*xx + cpp)
    // declare var
    const double laa = wcf->laa;        // linest y-intercept
    const double lbb = wcf->lbb;        // linest slope
    double dr = 0;
    double mrr = 0;             // r-average
    double nnn = 0;             // number of elements
    double rr = 0;              // r-residual
    double tt = 0;
    double vrr = 0;             // r-variance.p
    static const int dof = 5;
    // calculate csr - caa
    double *ttyy = ((double *) (wcf + ncol - icol)) + icol * 3;
    for (int ii = 0; ii < nbody; ii += ncol * 3) {
        rr = ttyy[ii + 1] - (laa + lbb * ttyy[ii + 0]);
        ttyy[ii + 2] = rr;
        nnn += 1;
        // welford - increment vrr
        dr = rr - mrr;
        mrr += dr / nnn;
        vrr += dr * (rr - mrr);
    }
    const double caa = sqrt(vrr / nnn);
    const double inva = 1 / caa;
    if (!isfinite(inva) || !isfinite(1 / wcf->mxe)) {
        return;
    }
    wcf->caa = caa;
    // calculate csr - cpp, cww - using gauss-newton-method
    // yy   ~ caa*cos(cww*tt + cpp)
    // cost = cos(cww*tt + cpp)
    // sint = sin(cww*tt + cpp)
    // rr   = yy - cos
    // gp   =     d/dp[y-cos(w*t+p)]^2 = 2*(rr*sint            )
    // gw   =     d/dw[y-cos(w*t+p)]^2 = 2*(rr*sint            )*tt
    // hpp  = d^2/dpdp[y-cos(w*t+p)]^2 = 2*(rr*cost + sint*sint)
    // hpw  = d^2/dpdw[y-cos(w*t+p)]^2 = 2*(rr*cost + sint*sint)*tt
    // hww  = d^2/dwdw[y-cos(w*t+p)]^2 = 2*(rr*cost + sint*sint)*tt*tt
    // [hpp hpw][dp] = [gp]
    // [hpw hww][dw] = [gw]
    // [dp] = 1 /    [ hww -hpw][gp]
    // [dw] =  / det [-hpw  hpp][gw]
    // det  = hpp*hww - hpw*hpw
    // dp   = 1/det*( hww*gp - hpw*gw)
    // dw   = 1/det*(-hpw*gp + hpp*gw)
    // cpp  = cpp - dp
    // cww  = cww - dw
    const double ctt0 = 2 * wcf->mxe;
    const double cww0 = 2 * MATH_PI / ctt0;
    const double cww_max = 0.2500 * cww0 * sqrt(nnn);
    double cpp = wcf->cpp;      // angular-phase
    double cww = wcf->cww == 0 ? cww0 : wcf->cww;       // angular-frequency
    for (int jj = 0; jj < 1; jj += 1) {
        double dw = 0;
        double dw_sign = 0;
        double gp = 0;          // gradient-phase
        double gw = 0;          // gradient-frequency
        double hpp = 0;         // hessian ddr/dpdp
        double hpw = 0;         // hessian ddr/dpdw
        double hww = 0;         // hessian ddr/dwdw
        for (int ii = 0; ii < nbody; ii += ncol * 3) {
            tt = ttyy[ii + 0];
            // tt = fmod(ttyy[ii + 0], 2 * MATH_PI / cww);
            // const double cost = cos(cpp + cww * tt);
            // const double sint = sin(cpp + cww * tt);
            const double cost = cos(cpp + fmod(cww * tt, 2 * MATH_PI));
            const double sint = sin(cpp + fmod(cww * tt, 2 * MATH_PI));
            rr = sint * (ttyy[ii + 2] * inva - cost);
            gp += rr;
            gw += rr * tt;
            rr = sint * sint;
            // rr = sint * sint + cost * (ttyy[ii + 2] * inva - cost);
            hpp += rr;
            hpw += rr * tt;
            hww += rr * tt * tt;
        }
        const double invd = 1 / (hpp * hww - hpw * hpw);
        if (!isfinite(invd)) {
            return;
        }
        // increment cpp
        cpp = fmod(cpp - invd * (+hww * gp - hpw * gw), 2 * MATH_PI);
        // increment cww
        dw = invd * (-hpw * gp + hpp * gw);
        dw_sign = doubleSign(dw);
        dw = doubleAbs(dw);
        dw = doubleMin(dw, 0.2500 * cww_max);
        cww -= dw_sign * dw;
        cww = doubleMax(cww, MATH_PI / (2 * wcf->mxe));
        cww = doubleMin(cww, cww_max);
    }
    if (cpp < 0) {
        cpp += 2 * MATH_PI;
    }
    wcf->cpp = cpp;
    wcf->cww = cww;
    // calculate csr - cyy, ctt, ctp
    const int xx = wcf->xx1;
    wcf->cyy = wcf->lyy + caa * cos(fmod(cww * xx, 2 * MATH_PI) + cpp);
    wcf->ctt = 2 * MATH_PI / cww;
    wcf->ctp = fmod((fmod(cww * xx, 2 * MATH_PI) + cpp) / (2 * MATH_PI), 1);
    if (wcf->ctp < 0) {
        wcf->ctp += 1;
    }
    // calculate csr - cee
    mrr = 0;                    // r-average
    vrr = 0;                    // r-variance.p
    for (int ii = 0; ii < nbody; ii += ncol * 3) {
        tt = ttyy[ii + 0];
        const double cyy =
            laa + lbb * tt + caa * cos(fmod(cww * tt, 2 * MATH_PI) + cpp);
        rr = ttyy[ii + 1] - cyy;
        // welford - increment vrr
        dr = rr - mrr;
        mrr += dr / nnn;
        vrr += dr * (rr - mrr);
    }
    wcf->cee = sqrt(vrr / (nnn - dof));
}

static void winCosfitLnr(
    WinCosfit * wcf,
    const int modeWelford
) {
// This function will calculate running simple-linear-regression as:
//     yy = laa + lbb*xx
    const double xx = wcf->xx1;
    const double xx0 = wcf->xx0;
    const double yy = wcf->yy1;
    const double yy0 = wcf->yy0;
    double dx = 0;
    double dy = 0;
    double inv0 = wcf->inv0;
    double mxx = wcf->mxx;
    double myy = wcf->myy;
    double vxx = wcf->vxx;
    double vxy = wcf->vxy;
    double vyy = wcf->vyy;
    if (modeWelford) {
        // calculate running lnr - welford
        wcf->nnn += 1;
        inv0 = 1.0 / (wcf->nnn - 0);
        wcf->inv1 = 1.0 / (wcf->nnn - 1);
        wcf->inv2 = 1.0 / (wcf->nnn - 2);
        // welford - increment vxx
        dx = xx - mxx;
        mxx += dx * inv0;
        vxx += dx * (xx - mxx);
        // welford - increment vyy
        dy = yy - myy;
        myy += dy * inv0;
        vyy += dy * (yy - myy);
        // welford - increment vxy
        vxy += dy * (xx - mxx);
    } else {
        // calculate running lnr - window
        dx = xx - xx0;
        dy = yy - yy0;
        vxx += (xx * xx - xx0 * xx0) - inv0 * dx * dx - 2 * dx * mxx;
        vxy += (xx * yy - xx0 * yy0) - inv0 * dx * dy - mxx * dy - dx * myy;
        vyy += (yy * yy - yy0 * yy0) - inv0 * dy * dy - 2 * dy * myy;
        mxx += dx * inv0;
        myy += dy * inv0;
    }
    // calculate lnr - lxy, lbb, laa
    const double lxy = vxy / sqrt(vxx * vyy);
    const double lbb = vxy / vxx;
    const double laa = myy - lbb * mxx;
    // save wcf
    wcf->inv0 = inv0;
    wcf->laa = laa;
    wcf->lbb = lbb;
    wcf->lee = sqrt(vyy * (1 - lxy * lxy) * wcf->inv2);
    wcf->lxy = lxy;
    wcf->lyy = laa + lbb * xx;
    wcf->mee = sqrt(vyy * wcf->inv1);
    wcf->mxe = sqrt(vxx * wcf->inv1);
    wcf->mxx = mxx;
    wcf->myy = myy;
    wcf->vxx = vxx;
    wcf->vxy = vxy;
    wcf->vyy = vyy;
    wcf->xx0 = xx;
    wcf->yy0 = yy;
}

SQLMATH_FUNC static void sql3_win_cosfit2_value(
    sqlite3_context * context
) {
// This function will calculate running simple-linear-regression
// and cosine-regression as:
//     yy = laa + lbb*xx + caa*cos(cww*xx + cpp)
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - result
    doublearrayResult(context, vec99_head, vec99->nhead, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_cosfit2_final(
    sqlite3_context * context
) {
// This function will calculate running simple-linear-regression
// and cosine-regression as:
//     yy = laa + lbb*xx + caa*cos(cww*xx + cpp)
    // vec99 - value
    sql3_win_cosfit2_value(context);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - cleanup
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql3_win_cosfit2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running simple-linear-regression
// and cosine-regression as:
//     yy = laa + lbb*xx + caa*cos(cww*xx + cpp)
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    if (!vec99->wnn) {
        vec99->wnn = vec99->nbody;
    }
}

static void sql3_win_cosfit2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running simple-linear-regression
// and cosine-regression as:
//     yy = laa + lbb*xx + caa*cos(cww*xx + cpp)
    if (argc < 1 + 2 || argc % 2 != 1) {
        sqlite3_result_error(context,
            "wrong number of arguments to function win_cosfit2()", -1);
        return;
    }
    // vec99 - init
    const int ncol = (argc - 1) / 2;
    VECTOR99_AGGREGATE_CONTEXT(ncol * WinCosfitN);
    if (vec99->nbody == 0) {
        // ncol
        vec99->ncol = ncol;
    }
    // vec99 - init argv - xx, yy, xx0, yy0
    const int modeNocsr = sqlite3_value_int(argv[0]);
    const int wii0 = vec99->wii;
    argv += 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        WinCosfit *wcf = (WinCosfit *) vec99_head + ii;
        wcf->xx0 = vec99_body[wii0 + ii * 3 + 0];
        wcf->yy0 = vec99_body[wii0 + ii * 3 + 1];
        sqlite3_value_double_or_prev(argv[0], &wcf->xx1);
        sqlite3_value_double_or_prev(argv[1], &wcf->yy1);
        // vec99 - push xx, yy, rr
        VECTOR99_AGGREGATE_PUSH(wcf->xx1);
        VECTOR99_AGGREGATE_PUSH(wcf->yy1);
        VECTOR99_AGGREGATE_PUSH(0);
        argv += 2;
    }
    // vec99 - calculate lnr, csr
    WinCosfit *wcf = (WinCosfit *) vec99_head;
    for (int ii = 0; ii < ncol; ii += 1) {
        // vec99 - calculate lnr
        winCosfitLnr(wcf, vec99->wnn == 0);
        // vec99 - calculate csr
        if (!modeNocsr) {
            winCosfitCsr(wcf, vec99->nbody, vec99->ncol, ii);
        }
        // increment counter
        wcf += 1;
    }
    return;
  catch_error:
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql1_win_cosfit2_predict_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will predict next cosfit
    UNUSED_PARAMETER(argc);
    // validate argv
    const int bytes = sqlite3_value_bytes(argv[0]);
    if (bytes <= 0) {
        goto catch_error;
    }
    const int icol = sqlite3_value_int(argv[1]);
    if (icol < 0 || (size_t) bytes < (icol + 1) * WinCosfitN * sizeof(double)) {
        goto catch_error;
    }
    const WinCosfit *wcf = (WinCosfit *) sqlite3_value_blob(argv[0]) + icol;
    const double xx = sqlite3_value_double(argv[2]);
    const int mode = sqlite3_value_int(argv[3]);
    const double yy =           //
        mode == 1               //
        ? wcf->laa + wcf->lbb * xx      //
        : mode == 2             //
        ? wcf->caa * cos(fmod(wcf->cww * xx, 2 * MATH_PI) + wcf->cpp)
        : 0                     //
        + wcf->laa + wcf->lbb * xx      //
        + wcf->caa * cos(fmod(wcf->cww * xx, 2 * MATH_PI) + wcf->cpp);
    if (!isfinite(yy)) {
        goto catch_error;
    }
    sqlite3_result_double(context, yy);
    return;
  catch_error:
    sqlite3_result_null(context);
}

SQLMATH_FUNC static void sql1_win_cosfit2_step_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will step simple-linear-regression.
    const int ncol = (argc - 2) / 2;
    // validate argv
    if (argc < 4 || argc != 2 + ncol * 2) {
        goto catch_error;
    }
    const int bytes = sqlite3_value_bytes(argv[0]);
    if (ncol <= 0 || (size_t) bytes != ncol * WinCosfitN * sizeof(double)) {
        goto catch_error;
    }
    // init wcf0
    const WinCosfit *blob0 = sqlite3_value_blob(argv[0]);
    WinCosfit *wcf0 = sqlite3_malloc(bytes);
    if (wcf0 == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    memcpy(wcf0, blob0, bytes);
    // vec99 - calculate lnr, csr
    WinCosfit *wcf = wcf0;
    argv += 2;
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[0], &wcf->xx1);
        sqlite3_value_double_or_prev(argv[1], &wcf->yy1);
        // vec99 - calculate lnr
        winCosfitLnr(wcf, 0);
        // vec99 - calculate csr
        // winCosfitCsr(wcf, nbody, ncol, ii);
        argv += 2;
        wcf += 1;
    }
    // vec99 - result
    doublearrayResult(context, (const double *) wcf0, bytes / sizeof(double),
        sqlite3_free);
    return;
  catch_error:
    sqlite3_result_error(context,
        "invalid arguments to function win_cosfit2_step()", -1);
}

// SQLMATH_FUNC sql3_win_cosfit2_func - end

// SQLMATH_FUNC sql3_win_ema1_func - start
SQLMATH_FUNC static void sql3_win_ema1_value(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    sqlite3_result_double(context, vec99_body[(int) vec99->wii]);
}

SQLMATH_FUNC static void sql3_win_ema1_final(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // vec99 - value
    sql3_win_ema1_value(context);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - cleanup
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql3_win_ema1_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    if (!vec99->wnn) {
        vec99->wnn = vec99->nbody;
    }
}

SQLMATH_FUNC static void sql3_win_ema1_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    if (argc < 2) {
        sqlite3_result_error(context,
            "wrong number of arguments to function win_ema2()", -1);
        return;
    }
    // vec99 - init
    const int ncol = argc - 1;
    double arg_alpha = NAN;
    VECTOR99_AGGREGATE_CONTEXT(argc);
    if (vec99->nbody == 0) {
        // ncol
        vec99->ncol = ncol;
        // arg_alpha
        arg_alpha = sqlite3_value_double_or_nan(argv[0]);
        if (isnan(arg_alpha)) {
            sqlite3_result_error(context,
                "invalid argument 'alpha' to function win_emax()", -1);
            return;
        }
        vec99_head[ncol + 0] = arg_alpha;
    }
    // vec99 - calculate ema
    arg_alpha = vec99_head[ncol + 0];
    const int nrow = vec99->nbody / ncol;
    argv += 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[0], &vec99_head[ii]);
        double *row = vec99_body + ii;
        // debug
        // fprintf(stderr,         //
        //     "win_ema2 - nbody=%.0f xx0=%f xx=%f arg_alpha=%f\n",        //
        //     vec99->nbody, *row, vec99_head[0], arg_alpha);
        for (int jj = 0; jj < nrow; jj += 1) {
            *row = arg_alpha * vec99_head[ii] + (1 - arg_alpha) * *row;
            row += ncol;
        }
        argv += 1;
    }
    // vec99 - push xx
    for (int ii = 0; ii < ncol; ii += 1) {
        VECTOR99_AGGREGATE_PUSH(vec99_head[ii]);
    }
    return;
  catch_error:
    vector99_agg_free(vec99_agg);
}

// SQLMATH_FUNC sql3_win_ema1_func - end

// SQLMATH_FUNC sql3_win_ema2_func - start
SQLMATH_FUNC static void sql3_win_ema2_value(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    if (!vec99->ncol) {
        sqlite3_result_null(context);
    }
    // vec99 - result
    doublearrayResult(context, vec99_body + (int) vec99->wii,
        (int) vec99->ncol, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_ema2_final(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // vec99 - value
    sql3_win_ema2_value(context);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - cleanup
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql3_win_ema2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    sql3_win_ema1_inverse(context, argc, argv);
}

SQLMATH_FUNC static void sql3_win_ema2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    sql3_win_ema1_step(context, argc, argv);
}

// SQLMATH_FUNC sql3_win_ema2_func - end

// SQLMATH_FUNC sql3_win_quantile1_func - start
SQLMATH_FUNC static void sql3_win_quantile1_value(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    sqlite3_result_double(context, vec99_head[(int) vec99->ncol + 1]);
}

SQLMATH_FUNC static void sql3_win_quantile1_final(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // vec99 - value
    sql3_win_quantile1_value(context);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - cleanup
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql3_win_quantile1_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running quantile.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    if (!vec99->wnn) {
        vec99->wnn = vec99->nbody;
    }
    // vec99 - invert
    const int ncol = argc - 1;
    const int nstep = ncol * 2;
    const int nn = vec99->nbody - nstep;
    double *arr = vec99_body + 1;
    double *xx0 = vec99_body + 0 + (int) vec99->wii;
    for (int ii = 0; ii < ncol; ii += 1) {
        const double xx = *xx0;
        int jj = 0;
        for (; jj < nn && arr[jj] < xx; jj += nstep) {
        }
        for (; jj < nn; jj += nstep) {
            arr[jj] = arr[jj + nstep];
        }
        arr[jj] = INFINITY;
        arr += 2;
        xx0 += 2;
    }
}

SQLMATH_FUNC static void sql3_win_quantile1_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running quantile.
    if (argc < 2) {
        sqlite3_result_error(context,
            "wrong number of arguments to function win_quantile2()", -1);
        return;
    }
    // vec99 - init
    const int ncol = argc - 1;
    double arg_quantile = NAN;
    VECTOR99_AGGREGATE_CONTEXT(argc + ncol);
    if (vec99->nbody == 0) {
        // ncol
        vec99->ncol = ncol;
        // arg_quantile
        arg_quantile = sqlite3_value_double_or_nan(argv[0]);
        if (!(0 <= arg_quantile && arg_quantile <= 1)) {
            sqlite3_result_error(context,
                "invalid argument 'quantile' to function win_quantilex()",
                -1);
            return;
        }
        vec99_head[ncol + 0] = arg_quantile;
    }
    // vec99 - push xx
    argv += 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[0], &vec99_head[ii]);
        VECTOR99_AGGREGATE_PUSH(vec99_head[ii]);
        VECTOR99_AGGREGATE_PUSH(        //
            vec99->wnn ? vec99_body[(int) vec99->wii] : INFINITY);
        argv += 1;
    }
    // vec99 - calculate quantile
    const int nstep = ncol * 2;
    const int nn = vec99->nbody / nstep;
    double *arr = vec99_body + 1;
    //
    arg_quantile = vec99_head[ncol + 0] * (nn - 1);
    const int kk1 = floor(arg_quantile) * nstep;
    const int kk2 = kk1 + nstep;
    arg_quantile = fmod(arg_quantile, 1);
    for (int ii = 0; ii < ncol; ii += 1) {
        const double xx = vec99_head[ii];
        int jj = (nn - 2) * nstep;
        for (; jj >= 0 && arr[jj] > xx; jj -= nstep) {
            arr[jj + nstep] = arr[jj];
        }
        arr[jj + nstep] = xx;
        vec99_head[ncol + 1 + ii] = arg_quantile == 0   //
            ? arr[kk1]          //
            : (1 - arg_quantile) * arr[kk1] + arg_quantile * arr[kk2];
        arr += 2;
    }
    return;
  catch_error:
    vector99_agg_free(vec99_agg);
}

// SQLMATH_FUNC sql3_win_quantile1_func - end

// SQLMATH_FUNC sql3_win_quantile2_func - start
SQLMATH_FUNC static void sql3_win_quantile2_value(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    if (!vec99->ncol) {
        sqlite3_result_null(context);
    }
    // vec99 - result
    doublearrayResult(context, vec99_head + (int) vec99->ncol + 1,
        (int) vec99->ncol, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_quantile2_final(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // vec99 - value
    sql3_win_quantile2_value(context);
    // vec99 - init
    VECTOR99_AGGREGATE_CONTEXT(0);
    // vec99 - cleanup
    vector99_agg_free(vec99_agg);
}

SQLMATH_FUNC static void sql3_win_quantile2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running quantile.
    sql3_win_quantile1_inverse(context, argc, argv);
}

SQLMATH_FUNC static void sql3_win_quantile2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running quantile.
    sql3_win_quantile1_step(context, argc, argv);
}

// SQLMATH_FUNC sql3_win_quantile2_func - end

// file sqlmath_base - init
int sqlite3_sqlmath_base_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED_PARAMETER(pApi);
    UNUSED_PARAMETER(pzErrMsg);
    int errcode = 0;
    SQLITE3_CREATE_FUNCTION1(castrealornull, 1);
    SQLITE3_CREATE_FUNCTION1(castrealorzero, 1);
    SQLITE3_CREATE_FUNCTION1(casttextorempty, 1);
    SQLITE3_CREATE_FUNCTION1(copyblob, 1);
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(doublearray_array, -1);
    SQLITE3_CREATE_FUNCTION1(doublearray_extract, 2);
    SQLITE3_CREATE_FUNCTION1(doublearray_jsonfrom, 1);
    SQLITE3_CREATE_FUNCTION1(doublearray_jsonto, 1);
    SQLITE3_CREATE_FUNCTION1(marginoferror95, 2);
    SQLITE3_CREATE_FUNCTION1(roundorzero, 2);
    SQLITE3_CREATE_FUNCTION1(sign, 1);
    SQLITE3_CREATE_FUNCTION1(squared, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION1(win_cosfit2_predict, 4);
    SQLITE3_CREATE_FUNCTION1(win_cosfit2_step, -1);
    SQLITE3_CREATE_FUNCTION2(median, 1);
    SQLITE3_CREATE_FUNCTION2(quantile, 2);
    SQLITE3_CREATE_FUNCTION2(stdev, 1);
    SQLITE3_CREATE_FUNCTION3(win_cosfit2, -1);
    SQLITE3_CREATE_FUNCTION3(win_ema1, 2);
    SQLITE3_CREATE_FUNCTION3(win_ema2, -1);
    SQLITE3_CREATE_FUNCTION3(win_quantile1, 2);
    SQLITE3_CREATE_FUNCTION3(win_quantile2, -1);
    errcode =
        sqlite3_create_function(db, "random1", 0,
        SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, sql1_random1_func, NULL, NULL);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    return 0;
}
#endif                          // SQLMATH_BASE_C3
/*
file sqlmath_base - end
*/


/*
file sqlmath_nodejs - start
*/
#if defined(SQLMATH_NODEJS_C2) && !defined(SQLMATH_NODEJS_C3)
#define SQLMATH_NODEJS_C3


#ifdef WIN32
#include <windows.h>
#endif


// file sqlmath_nodejs - assert
static int napiAssertOk(
    napi_env env,
    const char *func,
    const char *file,
    int line,
    int errcode
) {
// This function will throw error if <errcode> != napi_ok.
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


// file sqlmath_nodejs - promise
static void jsbatonBufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// This function will finalize <finalize_data>.
    UNUSED_PARAMETER(env);
    UNUSED_PARAMETER(finalize_hint);
    // cleanup baton->bufv[ii]
    sqlite3_free(finalize_data);
}

static Jsbaton *jsbatonCreate(
    napi_env env,
    napi_callback_info info
) {
// This function will create a baton for passing data between nodejs <-> c.
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
// This function will export c-data to js-data in <jsbaton>.
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
    errcode =
        napi_get_reference_value(env, ref, (napi_value *) & baton->napi_argv);
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
    errcode =
        napi_create_promise(env, (napi_deferred *) & (baton->napi_deferred),
        &promise);
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
        // napi_async_work* result);
        (napi_async_work *) & (baton->napi_work));
    NAPI_ASSERT_OK();
    // Queue the work item for execution.
    errcode = napi_queue_async_work(env, baton->napi_work);
    NAPI_ASSERT_OK();
    // This causes created `promise` to be returned to JavaScript.
    return promise;
}


// file sqlmath_nodejs - init
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
#endif                          // SQLMATH_NODEJS_C3
/*
file sqlmath_nodejs - end
*/
