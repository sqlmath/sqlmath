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
#define SQLITE3EXT_H2
#ifdef SQLITE3_C2
#   undef SQLITE3_C2
#   define SQLMATH_C2
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
#include "sqlite3_rollup.c"


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
#define SWAP(aa, bb) tmp = (aa); (aa) = (bb); (bb) = tmp
#define UNUSED_PARAMETER(x) ((void)(x))


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
        UNUSED_PARAMETER(env); \
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

#define SQLITE3_CREATE_FUNCTION3(func, argc) \
    errcode = sqlite3_create_window_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql_##func##_step, sql_##func##_final, \
        sql_##func##_value, sql_##func##_inverse, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_RESULT_ERROR_CODE(errcode) \
    if (errcode) { \
        sqlite3_result_error_code(context, errcode); \
        goto catch_error; \
    }

#define SQLITE3_RESULT_ERROR_MALLOC(pp) \
    if (pp == NULL) { \
        sqlite3_result_error_nomem(context); \
        goto catch_error; \
    }

#define STR99_ALLOCA(str99) \
    sqlite3_str __##str99 = { 0 }; \
    sqlite3_str *str99 = &__##str99; \
    str99->mxAlloc = SQLITE_MAX_LENGTH2; \

#define STR99_RESULT_ERROR(str99) \
    errcode = sqlite3_str_errcode(str99); \
    if (errcode == SQLITE_ERROR_JSON_ARRAY_INVALID) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error(context, \
            "str99ArrayAppendJsonarray() - invalid JSON array", \
            SQLITE_ERROR_JSON_ARRAY_INVALID); \
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


// file sqlmath_h - sqlite3
// *INDENT-OFF*
SQLITE_API const sqlite3_api_routines *sqlite3ApiGet();
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
// *INDENT-ON*
SQLITE_API void str99JsonAppendText(
    sqlite3_str * str99,
    const char *zIn,
    u32 nn
);
SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double val
);
SQLMATH_API void str99ArrayAppendJsonarray(
    sqlite3_str * str99,
    const char *json,
    int nn
);
SQLMATH_API void str99JsonAppendFloat64array(
    sqlite3_str * str99,
    const double *arr,
    int nn
);
SQLMATH_API void str99JsonAppendJenks(
    sqlite3_str * str99,
    int kk,
    const double *arr,
    int nn
);
SQLMATH_API void str99JsonAppendText(
    sqlite3_str * str99,
    const char *zIn,
    u32 nn
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
typedef struct vector99 {
    double *buf;
    int alloc;
    int size;
    double arg0;
    double arg1;
} vector99;
SQLMATH_API void vector99_buf_free(
    vector99 * vec99
);
SQLMATH_API int vector99_buf_malloc(
    vector99 * vec99
);
SQLMATH_API int vector99_nomem(
    const vector99 * vec99
);
SQLMATH_API double vector99_pop_front(
    vector99 * vec99
);
SQLMATH_API int vector99_push_back(
    vector99 * vec99,
    double val
);


// file sqlmath_h - Jsbaton
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

SQLMATH_API int noop(
);

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double pp
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


// file sqlmath_base - SQLMATH_API
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

SQLMATH_API void vector99_buf_free(
    vector99 * vec99
) {
// this function will free <vec99>->buf and reset everything to zero
    if (vec99 == NULL) {
        return;
    }
    sqlite3_free(vec99->buf);
    memset(vec99, 0, sizeof(*vec99));
}

SQLMATH_API int vector99_buf_malloc(
    vector99 * vec99
) {
// this function will malloc <vec99>->buf, or return SQLITE_NOMEM
    static const int alloc0 = 256;
    if (vec99 == NULL) {
        return SQLITE_NOMEM;
    }
    vec99->buf = sqlite3_malloc(alloc0 * sizeof(double));
    if (vector99_nomem(vec99)) {
        return SQLITE_NOMEM;
    }
    vec99->alloc = alloc0;
    vec99->size = 0;
    vec99->arg0 = NAN;
    vec99->arg1 = NAN;
    return SQLITE_OK;
}

SQLMATH_API int vector99_nomem(
    const vector99 * vec99
) {
// this function will check if <vec99>->buf is null
    return vec99 == NULL || vec99->buf == NULL;
}

SQLMATH_API double vector99_pop_front(
    vector99 * vec99
) {
// this function will pop_front <vec99>, or return NAN if empty
    if (vector99_nomem(vec99) || vec99->size <= 0) {
        return NAN;
    }
    double val = vec99->buf[0];
    vec99->size -= 1;
    memmove(vec99->buf, vec99->buf + 1, vec99->size * sizeof(double));
    return val;
}

SQLMATH_API int vector99_push_back(
    vector99 * vec99,
    double val
) {
// this function will push_back <val> to <vec99>,
// and dynamically grow <vec99> if necessary
    // error - nomem
    if (vector99_nomem(vec99)) {
        return SQLITE_NOMEM;
    }
    if (vec99->size >= vec99->alloc) {
        vec99->alloc *= 2;
        // error - toobig
        if (vec99->alloc > SQLITE_MAX_LENGTH2) {
            vector99_buf_free(vec99);
            return SQLITE_TOOBIG;
        }
        double *buf_old = vec99->buf;
        vec99->buf =
            sqlite3_realloc(vec99->buf, vec99->alloc * sizeof(double));
        // error - nomem
        if (vector99_nomem(vec99)) {
            // dont forget to free buf_old
            vec99->buf = buf_old;
            vector99_buf_free(vec99);
            return SQLITE_NOMEM;
        }
    }
    vec99->buf[vec99->size] = val;
    vec99->size += 1;
    return SQLITE_OK;
}

// SQLMATH_API vector99 - end

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

SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double val
) {
// this function will append double <val> to <str99>
    sqlite3_str_append(str99, (const char *) &val, 8);
}

SQLMATH_API void str99ArrayAppendJsonarray(
    sqlite3_str * str99,
    const char *json,
    int nn
) {
// this function will append binary-Float64Array from json-encoded-flat-array
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

SQLMATH_API void str99JsonAppendFloat64array(
    sqlite3_str * str99,
    const double *arr,
    int nn
) {
// this function will append json-encoded-flat-array from binary-Float64Array
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
}

SQLMATH_API void str99JsonAppendJenks(
    sqlite3_str * str99,
    int kk,
    const double *arr,
    int nn
) {
// this function will append json-encoded-flat-array from jenks-result
    // jenks - null-case
    if (kk <= 0 || nn <= 0) {
        sqlite3_str_appendchar(str99, 1, '[');
        sqlite3_str_appendchar(str99, 1, ']');
        return;
    }
    // jenks - classify
    double *result = jenksCreate(kk, arr, nn);
    if (result == NULL) {
        str99->accError = SQLITE_NOMEM;
        return;
    }
    // str99 - to-json
    str99JsonAppendFloat64array(str99, result, 1 + ((int) result[0]) * 2);
    sqlite3_free(result);
}

SQLMATH_API void str99ResultBlob(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// this function will return <str99> as result-blob in given <context>
    sqlite3_result_blob(context, (const char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}

SQLMATH_API void str99ResultText(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// this function will return <str99> as result-text in given <context>
    sqlite3_result_text(context, (const char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}


// file sqlmath_base - SQLMATH_FUNC
// SQLMATH_FUNC sql_avg_ema_func - start
SQLMATH_FUNC static void sql_avg_ema_value(
    sqlite3_context * context
) {
// this function will aggregate exponential-moving-average
    // vec99 - init
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    double result = vec99->buf[0];
    sqlite3_result_double(context, result);
    return;
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_avg_ema_final(
    sqlite3_context * context
) {
// this function will aggregate exponential-moving-average
    // vec99 - value
    sql_avg_ema_value(context);
    // vec99 - cleanup
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_avg_ema_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate exponential-moving-average
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    vector99_pop_front(vec99);
    return;
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_avg_ema_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate exponential-moving-average
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // vec99 - init
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    if (vec99->size <= 0) {
        errcode = vector99_buf_malloc(vec99);
        SQLITE3_RESULT_ERROR_CODE(errcode);
        vec99->arg0 = sqlite3_value_double(argv[1]);    // alpha
    }
    // vec99 - calculate ema
    const double alpha = vec99->arg0;
    const double elem = sqlite3_value_double(argv[0]);
    for (int ii = 0; ii < vec99->size; ii += 1) {
        vec99->buf[ii] = alpha * elem + (1 - alpha) * vec99->buf[ii];
    }
    // vec99 - push_back elem
    errcode = vector99_push_back(vec99, elem);
    SQLITE3_RESULT_ERROR_CODE(errcode);
    return;
  catch_error:
    vector99_buf_free(vec99);
}

// SQLMATH_FUNC sql_avg_ema_func - end

// SQLMATH_FUNC sql_btobase64_func - start
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
    bb = 4 * (int) ceil((double) *nn / 3);
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

SQLMATH_FUNC static void sql_btobase64_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to base64-encoded-text
    UNUSED_PARAMETER(argc);
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

// SQLMATH_FUNC sql_btobase64_func - end

SQLMATH_FUNC static void sql_btotext_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to text
    UNUSED_PARAMETER(argc);
    sqlite3_result_text(context, (const char *) sqlite3_value_text(argv[0]),
        -1, SQLITE_TRANSIENT);
}


SQLMATH_FUNC static void sql_castrealorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will cast <argv>[0] to double or zero
    UNUSED_PARAMETER(argc);
    double num = sqlite3_value_double(argv[0]);
    sqlite3_result_double(context, isfinite(num) ? num : 0);
}

SQLMATH_FUNC static void sql_casttextorempty_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will cast <argv>[0] to text or empty-string
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

SQLMATH_FUNC static void sql_copyblob_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will copy blob/text/value <argv>[0]
    UNUSED_PARAMETER(argc);
    sqlite3_result_value(context, argv[0]);
}

SQLMATH_FUNC static void sql_cot_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return cot(argv[0])
    UNUSED_PARAMETER(argc);
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

SQLMATH_FUNC static void sql_coth_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return coth(argv[0])
    UNUSED_PARAMETER(argc);
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

SQLMATH_FUNC static void sql_jenks_blob_func(
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
    UNUSED_PARAMETER(argc);
    // declare var
    const double *arr = (double *) sqlite3_value_blob(argv[1]);
    const int kk = sqlite3_value_int(argv[0]);
    const int nn = sqlite3_value_bytes(argv[1]) / sizeof(double);
    // jenks - null-case
    if (kk <= 0 || nn <= 0) {
        sqlite3_result_null(context);
        return;
    }
    // jenks - classify
    double *result = jenksCreate(kk, arr, nn);
    if (result == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    // jenks - result
    sqlite3_result_blob(context, (void *) result,
        (1 + ((int) result[0]) * 2) * 8, sqlite3_free);
}

// SQLMATH_FUNC sql_jenks_concat_func - start
static void str99arrCleanup(
    const int argc,
    sqlite3_str ** str99arr
) {
    for (int ii = 0; ii < argc; ii += 1) {
        sqlite3_str *str99 = str99arr[ii];
        if (str99 != NULL) {
            if (ii > 0) {
                sqlite3_str_reset(str99);
            }
            sqlite3_free(str99);
            str99arr[ii] = NULL;
        }
    }
}

SQLMATH_FUNC static void sql_jenks_concat_final(
    sqlite3_context * context
) {
// this function will calculate <kk> jenks-natrual-breaks in each column <ii>,
// and return a json array
    // str99arr - init
    sqlite3_str **str99arr =
        (sqlite3_str **) sqlite3_aggregate_context(context, 0);
    if (str99arr == NULL) {
        sqlite3_result_null(context);
        return;
    }
    // declare var
    const int argc = ((int *) (str99arr[0]))[0];
    const int kk = ((int *) (str99arr[0]))[1];
    int errcode = 0;
    // str99arr - cleanup
    str99arrCleanup(1, str99arr);
    // str99json - init
    STR99_ALLOCA(str99json);
    sqlite3_str_appendchar(str99json, 1, '[');
    for (int ii = 1; ii < argc; ii += 1) {
        sqlite3_str *str99 = str99arr[ii];
        STR99_RESULT_ERROR(str99);
        // jenks - classify
        if (ii > 1) {
            sqlite3_str_appendchar(str99json, 1, ',');
        }
        str99JsonAppendJenks(   //
            str99json,          // json
            kk,                 // kk
            (double *) str99->zText,    // array
            sqlite3_str_length(str99) / 8);     // nn
        STR99_RESULT_ERROR(str99json);
    }
    sqlite3_str_appendchar(str99json, 1, ']');
    STR99_RESULT_ERROR(str99json);
    // str99json - result
    str99ResultText(str99json, context);
  catch_error:
    // str99arr - cleanup
    str99arrCleanup(argc, str99arr);
}

SQLMATH_FUNC static void sql_jenks_concat_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will calculate <kk> jenks-natrual-breaks in each column <ii>,
// and return a json array
    // init kk
    if (argc < 2) {
        return;
    }
    const int kk = sqlite3_value_int(argv[0]);
    if (kk <= 0) {
        return;
    }
    // str99arr - init
    sqlite3_str **str99arr =
        (sqlite3_str **) sqlite3_aggregate_context(context,
        argc * sizeof(sqlite3_str *));
    if (str99arr == NULL) {
        return;
    }
    if (str99arr[0] == NULL) {
        for (int ii = 0; ii < argc; ii += 1) {
            str99arr[ii] = sqlite3_malloc(sizeof(sqlite3_str));
            sqlite3_str *str99 = str99arr[ii];
            if (str99 == NULL) {
                // str99arr - cleanup
                str99arrCleanup(argc, str99arr);
                sqlite3_result_error_nomem(context);
                return;
            }
            memset(str99, 0, sizeof(sqlite3_str));
            str99->mxAlloc = SQLITE_MAX_LENGTH2;
        }
        ((int *) (str99arr[0]))[0] = argc;
        ((int *) (str99arr[0]))[1] = kk;
    }
    // str99 - append double
    for (int ii = 1; ii < argc; ii += 1) {
        sqlite3_str *str99 = str99arr[ii];
        if (sqlite3_value_numeric_type(argv[ii]) != SQLITE_NULL) {
            str99ArrayAppendDouble(str99, sqlite3_value_double(argv[ii]));
        }
    }
}

// SQLMATH_FUNC sql_jenks_concat_func - end

SQLMATH_FUNC static void sql_jenks_json_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate <kk> jenks-natrual-breaks in given <values>,
// and return a json array with length (1 + kk * 2) of form:
// [
// (double) kk,
// (double) break_1, (double) count_1,
// (double) break_2, (double) count_2,
// ...,
// (double) break_k, (double) count_k
// ]
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - to-array
    STR99_ALLOCA(arr);
    str99ArrayAppendJsonarray(  //
        arr,                    // array
        (char *) sqlite3_value_blob(argv[1]),   // json
        sqlite3_value_bytes(argv[1]));  // nn
    STR99_RESULT_ERROR(arr);
    // jenks - classify
    STR99_ALLOCA(str99);
    str99JsonAppendJenks(       //
        str99,                  // json
        sqlite3_value_int(argv[0]),     // kk
        (double *) arr->zText,  // array
        sqlite3_str_length(arr) / 8);   // nn
    sqlite3_str_reset(arr);
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultText(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql_jsonfromfloat64array_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will create json-encoded-flat-array from binary-Float64Array
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - init
    STR99_ALLOCA(str99);
    // str99 - jsonfrom
    str99JsonAppendFloat64array(        //
        str99,                  //
        (double *) sqlite3_value_blob(argv[0]), //
        sqlite3_value_bytes(argv[0]) / 8);
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultText(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql_jsontofloat64array_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will create binary-Float64Array from json-encoded-flat-array
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - to-array
    STR99_ALLOCA(str99);
    str99ArrayAppendJsonarray(  //
        str99,                  // array
        (char *) sqlite3_value_blob(argv[0]),   // json
        sqlite3_value_bytes(argv[0]));  // nn
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultBlob(str99, context);
  catch_error:
    (void) 0;
}

// SQLMATH_FUNC sql_marginoferror95_func - start
SQLMATH_API double marginoferror95(
    double nn,
    double pp
) {
// this function will calculate margin-of-error sqrt(pp*(1-pp)/nn)
    return 1.9599639845400542 * sqrt(pp * (1 - pp) / nn);
}

SQLMATH_FUNC static void sql_marginoferror95_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will calculate margin-of-error sqrt(pp*(1-pp)/nn)
    UNUSED_PARAMETER(argc);
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

// SQLMATH_FUNC sql_marginoferror95_func - end

// SQLMATH_FUNC sql_matrix2d_concat_func - start
SQLMATH_FUNC static void sql_matrix2d_concat_final(
    sqlite3_context * context
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    // declare var
    int errcode = 0;
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, 0);
    if (str99 == NULL) {
        sqlite3_result_null(context);
        return;
    }
    STR99_RESULT_ERROR(str99);
    // str99 - null-case
    if (sqlite3_str_length(str99) <= 2 * 8) {
        sqlite3_str_reset(str99);
        sqlite3_result_null(context);
        return;
    }
    double *arr = (double *) str99->zText;
    arr[0] = (0.125 * ((double) sqlite3_str_length(str99)) - 2) / arr[1];
    // str99 - result
    str99ResultBlob(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql_matrix2d_concat_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, sizeof(*str99));
    if (str99 == NULL) {
        return;
    }
    // str99 - init zText
    if (sqlite3_str_length(str99) == 0) {
        str99->mxAlloc = SQLITE_MAX_LENGTH2;
        str99ArrayAppendDouble(str99, 0);
        str99ArrayAppendDouble(str99, (double) argc);
    }
    // str99 - append double
    for (int ii = 0; ii < argc; ii += 1) {
        str99ArrayAppendDouble(str99, sqlite3_value_double(argv[ii]));
    }
}

// SQLMATH_FUNC sql_matrix2d_concat_func - end

// SQLMATH_FUNC sql_quantile_func - start
static double quickselect(
    double *arr,
    const int nn,
    const int kk
) {
// this function will find <kk>-th element in <arr> using quickselect-algorithm
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn <= 0) {
        return 0;
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
                SWAP(arr[ll], arr[ir]);
            }
            return arr[kk];
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
    const double pp
) {
// this function will find <pp>-th-quantile element in <arr>
// using quickselect-algorithm
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn <= 0) {
        return 0;
    }
    const int kk = MAX(0, MIN(nn - 1, (const int) (pp * nn)));
    // handle even-case
    if ((0 < kk && kk + 1 <= nn) && (double) kk == (pp * nn)) {
        return 0.5 * (quickselect(arr, nn, kk) + quickselect(arr, nn,
                kk - 1));
    }
    // handle odd-case
    return quickselect(arr, nn, kk);
}

SQLMATH_FUNC static void sql_quantile_final(
    sqlite3_context * context
) {
// this function will aggregate kth-quantile element
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    // vec99 - null-case
    if (vec99->size <= 0) {
        sqlite3_result_null(context);
        goto catch_error;
    }
    sqlite3_result_double(context, quantile(vec99->buf, vec99->size,
            vec99->arg0));
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_quantile_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate kth-quantile element
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // vec99 - init
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    if (vec99->size <= 0) {
        errcode = vector99_buf_malloc(vec99);
        SQLITE3_RESULT_ERROR_CODE(errcode);
        vec99->arg0 = sqlite3_value_double(argv[1]);    // kth-quantile
    }
    // vec99 - append isfinite
    const double elem = sqlite3_value_double(argv[0]);
    if (sqlite3_value_type(argv[0]) != SQLITE_NULL && isfinite(elem)) {
        errcode = vector99_push_back(vec99, elem);
        SQLITE3_RESULT_ERROR_CODE(errcode);
    }
    return;
  catch_error:
    vector99_buf_free(vec99);
}

// SQLMATH_FUNC sql_quantile_func - end


SQLMATH_FUNC static void sql_roundorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will round <argv>[0] to decimal <argv>[1]
    UNUSED_PARAMETER(argc);
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

SQLMATH_FUNC static void sql_random1_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will generate high-quality random-float between 0 <= xx < 1
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    static const double inv = 1.0 / 0x100000000;
    uint32_t xx[1];
    sqlite3_randomness(4, (void *) xx);
    sqlite3_result_double(context, ((double) *xx) * inv);
}

SQLMATH_FUNC static void sql_sign_func(
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

SQLMATH_FUNC static void sql_squared_func(
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

SQLMATH_FUNC static void sql_throwerror_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return sqlite3_result_error_code(argv[0])
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = sqlite3_value_int(argv[0]);
    if (0 <= errcode && errcode <= 28) {
        sqlite3_result_error_code(context, errcode);
        return;
    }
    sqlite3_result_error_code(context, SQLITE_INTERNAL);
}


// file sqlmath_base - init
int sqlite3_sqlmath_base_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED_PARAMETER(pzErrMsg);
    // declare var
    int errcode = 0;
    // init sqlite3_api
    sqlite3_api = pApi;
    SQLITE3_CREATE_FUNCTION1(btobase64, 1);
    SQLITE3_CREATE_FUNCTION1(btotext, 1);
    SQLITE3_CREATE_FUNCTION1(castrealorzero, 1);
    SQLITE3_CREATE_FUNCTION1(casttextorempty, 1);
    SQLITE3_CREATE_FUNCTION1(copyblob, 1);
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(jenks_blob, 2);
    SQLITE3_CREATE_FUNCTION1(jenks_json, 2);
    SQLITE3_CREATE_FUNCTION1(jsonfromfloat64array, 1);
    SQLITE3_CREATE_FUNCTION1(jsontofloat64array, 1);
    SQLITE3_CREATE_FUNCTION1(marginoferror95, 2);
    SQLITE3_CREATE_FUNCTION1(roundorzero, 2);
    SQLITE3_CREATE_FUNCTION1(sign, 1);
    SQLITE3_CREATE_FUNCTION1(squared, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION2(jenks_concat, -1);
    SQLITE3_CREATE_FUNCTION2(matrix2d_concat, -1);
    SQLITE3_CREATE_FUNCTION2(quantile, 2);
    SQLITE3_CREATE_FUNCTION3(avg_ema, 2);
    errcode =
        sqlite3_create_function(db, "random1", 0,
        SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, sql_random1_func, NULL, NULL);
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
#ifdef SQLMATH_NODEJS_C2
//!! #if defined(SQLMATH_NODEJS_C2) && !defined(SQLMATH_NODEJS_C3)
//!! #define SQLMATH_NODEJS_C3


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


// file sqlmath_nodejs - promise
static void jsbatonBufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED_PARAMETER(env);
    UNUSED_PARAMETER(finalize_hint);
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
