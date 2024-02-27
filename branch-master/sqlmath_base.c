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
#include "sqlmath_external_sqlite.c"


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


#define JSBATON_ARGC            8
#define JSBATON_OFFSET_ALL      256
#define JSBATON_OFFSET_ARGV     128
#define JSBATON_OFFSET_BUFV     192
#define JSBATON_OFFSET_ERRMSG   48
#define JSBATON_OFFSET_FUNCNAME 8
#define JS_MAX_SAFE_INTEGER     0x1fffffffffffff
#define JS_MIN_SAFE_INTEGER     -0x1fffffffffffff
#define SIZEOF_BLOB_MAX         1000000000
#define SIZEOF_ERRMSG           80
#define SIZEOF_FUNCNAME         16
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_EXTERNALBUFFER          0x71
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x00
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
#define SQLITE_RESPONSETYPE_LASTBLOB    1


#define SQLITE_ERROR_DATATYPE_INVALID   0x10003
#define SQLITE_ERROR_JSON_ARRAY_INVALID         0x71
#define SQLITE_ERROR_ZSQL_NULL          0x10004
#define MATH_MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MATH_MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
#define MATH_PI 3.141592653589793238463
#define MATH_SIGN(aa) (((aa) < 0) ? -1 : ((aa) > 0) ? 1 : 0)
#define MATH_SWAP(aa, bb, tmp) tmp = (aa); (aa) = (bb); (bb) = tmp
#define SQLMATH_API
#define SQLMATH_FUNC
#define UNUSED_PARAMETER(x) ((void)(x))


#define DOUBLEWIN_AGGREGATE_CONTEXT(nhead) \
    Doublewin **dblwinAgg = (Doublewin **) \
        sqlite3_aggregate_context(context, sizeof(*dblwinAgg)); \
    if (doublewinAggmalloc(dblwinAgg, nhead) != SQLITE_OK) { \
        sqlite3_result_error_nomem(context); \
        return; \
    } \
    Doublewin *dblwin = *dblwinAgg; \
    double *dblwin_body = doublewinBody(dblwin); \
    double *dblwin_head = doublewinHead(dblwin); \
    UNUSED_PARAMETER(dblwin_body); \
    UNUSED_PARAMETER(dblwin_head);

#define DOUBLEWIN_AGGREGATE_PUSH(xx) \
    if (doublewinAggpush(dblwinAgg, xx) != SQLITE_OK) { \
        sqlite3_result_error_nomem(context); \
        return; \
    } \
    dblwin = *dblwinAgg; \
    dblwin_body = doublewinBody(dblwin); \
    dblwin_head = doublewinHead(dblwin);

// This function will exec <sql> and if <errcode> is not ok,
// throw <baton>->errmsg with given sqlite-<errcode>.
#define JSBATON_ASSERT_OK() \
    if (jsbatonAssertOk(baton, db, errcode)) { \
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

#define NAPI_PARSE_ARGV(funcname, argc0) \
    Jsbaton *baton = NULL; \
    int errcode = 0; \
    napi_value argv[argc0] = { 0 }; \
    size_t argc = argc0; \
    errcode = napi_get_cb_info(env, info, &argc, argv, NULL, NULL); \
    NAPI_ASSERT_OK(); \
    if (argc != argc0) { \
        napi_throw_error(env, NULL, \
            "sqlmath._" #funcname " - expected " #argc0 " arguments"); \
        return NULL; \
    } \
    errcode = \
        napi_get_arraybuffer_info(env, argv[0], (void **) &baton, &argc); \
    NAPI_ASSERT_OK();

#define NAPI_CREATE_FUNCTION(func) \
    {"_" #func, NULL, func, NULL, NULL, NULL, napi_default, NULL}

#define PY_PARSE_ARGV(bufc, ...) \
    Jsbaton *baton = NULL; \
    Py_buffer pybuf[bufc] = { 0 }; \
    if (!PyArg_ParseTuple(__VA_ARGS__)) { \
        return NULL; \
    } \
    baton = pybuf[0].buf; \
    PyBuffer_Release(&(pybuf[0]));

#define SQLITE3_AGGREGATE_CONTEXT(type) \
    type *agg = (type *) sqlite3_aggregate_context(context, sizeof(*agg)); \
    if (agg == NULL) { \
        sqlite3_result_error_nomem(context); \
        return; \
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

#define SQLITE_OK_OR_RETURN_RC(rc) \
    if (rc != SQLITE_OK) { return rc; }

#define STR99_ALLOCA(str99) \
    sqlite3_str __##str99 = { 0 }; \
    sqlite3_str *str99 = &__##str99; \
    str99->mxAlloc = SIZEOF_BLOB_MAX; \

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

// file sqlmath_h - sqlite3
// *INDENT-OFF*
SQLITE_API const sqlite3_api_routines *sqlite3ApiGet();
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;


// file sqlmath_h - db
typedef struct Jsbuffer {
    int64_t buf;
    int64_t len;
} Jsbuffer;
typedef struct Jsbaton {
    int32_t nallc;              // offset 000-004
    int32_t nused;              // offset 004-008
    char funcname[SIZEOF_FUNCNAME];     // offset 008-024
    int64_t napi_argv;          // offset 024-032
    int64_t napi_deferred;      // offset 032-040
    int64_t napi_work;          // offset 040-048
    char errmsg[SIZEOF_ERRMSG]; // offset 048-128
    int64_t argv[JSBATON_ARGC]; // offset 128-192
    Jsbuffer bufv[JSBATON_ARGC];        // offset 192-256
} Jsbaton;
SQLMATH_API void dbCall(
    Jsbaton * baton
);
SQLMATH_API void dbClose(
    Jsbaton * baton
);
SQLMATH_API void dbExec(
    Jsbaton * baton
);
SQLMATH_API void dbFileLoad(
    Jsbaton * baton
);
SQLMATH_API int dbFileLoadOrSave(
    sqlite3 * pInMemory,
    char *zFilename,
    const int isSave
);
SQLMATH_API void dbNoop(
    Jsbaton * baton
);
SQLMATH_API void dbOpen(
    Jsbaton * baton
);



// file sqlmath_h - doublewin
typedef struct Doublewin {
    double alloc;               // allocated size in bytes
    double nbody;               // number of body elements
    double nhead;               // number of head elements
    //
    double ncol;                // number of columns
    double waa;                 // window-position-left
    double wnn;                 // number of window elements
} Doublewin;
SQLMATH_API void doublewinAggfree(
    Doublewin ** dblwinAgg
);
SQLMATH_API int doublewinAggmalloc(
    Doublewin ** dblwinAgg,
    const int nhead
);
SQLMATH_API int doublewinAggpush(
    Doublewin ** dblwinAgg,
    double xx
);
SQLMATH_API double *doublewinBody(
    const Doublewin * dblwin
);
SQLMATH_API double *doublewinHead(
    const Doublewin * dblwin
);
SQLMATH_API void doublewinResultBlob(
    Doublewin * dblwin,
    sqlite3_context * context
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

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
);

SQLMATH_API void doublearrayResult(
    sqlite3_context * context,
    const double *arr,
    const int nn,
    void (*xDel) (void *)       // NOLINT
);

SQLMATH_API int jsbatonAssertOk(
    Jsbaton * baton,
    sqlite3 * db,
    int errcode
);

SQLMATH_API char *jsbatonGetErrmsg(
    Jsbaton * baton
);

SQLMATH_API int64_t jsbatonGetInt64(
    Jsbaton * baton,
    int argi
);

SQLMATH_API char *jsbatonGetString(
    Jsbaton * baton,
    int argi
);

SQLMATH_API void jsbatonSetErrmsg(
    Jsbaton * baton,
    const char *format,
    ...
);

SQLMATH_API void jsonResultDoublearray(
    sqlite3_context * context,
    const double *arr,
    int nn
);

SQLMATH_API int noop(
);

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double pp
);

SQLMATH_API void sqlite3_result_double_or_null(
    sqlite3_context * context,
    const double xx
);

SQLMATH_API double sqlite3_value_double_or_nan(
    sqlite3_value * arg
);

SQLMATH_API double sqlite3_value_double_or_prev(
    sqlite3_value * arg,
    double *previous
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
SQLMATH_API void dbCall(
    Jsbaton * baton
) {
// This function will call c-function dbXxx() with given <funcname>.
    char *funcname = (char *) baton + JSBATON_OFFSET_FUNCNAME;
    if (strcmp(funcname, "_dbClose") == 0) {
        dbClose(baton);
    } else if (strcmp(funcname, "_dbExec") == 0) {
        dbExec(baton);
    } else if (strcmp(funcname, "_dbFileLoad") == 0) {
        dbFileLoad(baton);
    } else if (strcmp(funcname, "_dbNoop") == 0) {
        dbNoop(baton);
    } else if (strcmp(funcname, "_dbOpen") == 0) {
        dbOpen(baton);
    } else {
        jsbatonSetErrmsg(baton, "sqlmath._dbCall - invalid funcname '%s'",
            funcname);
    }
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
    const char *zBind = (const char *) baton + JSBATON_OFFSET_ALL;
    const char *zSql = jsbatonGetString(baton, 1);
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
    // fprintf(stderr, "\nsqlmath._dbExec(db=%p blen=%d sql=%s)\n",
    //     db, bindListLength, zSql);
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
        case SQLITE_DATATYPE_EXTERNALBUFFER:
            bindElem->buflen = baton->bufv[*(int32_t *) zBind].len;
            bindElem->buf =
                (const char *) baton->bufv[*(int32_t *) zBind].buf;
            zBind += 4;
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
        default:
            fprintf(stderr, "\nsqlmath._dbExec(ii=%d datatype=%d len=%d)\n",
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
                    case SQLITE_DATATYPE_EXTERNALBUFFER:
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
                            "\nsqlmath._dbExec(ii=%d  datatype=%d  len=%d)\n",
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
    baton->bufv[0].buf = (int64_t) str99->zText;
    baton->bufv[0].len = sqlite3_str_length(str99);
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

SQLMATH_API void dbFileLoad(
    Jsbaton * baton
) {
// This function will load/save <zFilename> to/from <db>.
    sqlite3 *db = NULL;
    int errcode = dbFileLoadOrSave(     //
        (sqlite3 *) jsbatonGetInt64(baton, 0),  // sqlite3 * pInMemory
        jsbatonGetString(baton, 1),     // char *zFilename
        (const int) jsbatonGetInt64(baton, 2)); // const int isSave
    JSBATON_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_API int dbFileLoadOrSave(
    sqlite3 * pInMemory,
    char *zFilename,
    const int isSave
) {
    // fprintf(stderr, "\nsqlmath._dbFileLoadOrSave(pp=%p ff=%s ss=%d)\n",
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
    const char *filename = jsbatonGetString(baton, 0);
    const int flags = (int) baton->argv[2];
    // call c-function
    errcode = sqlite3_open_v2(  //
        filename,               // filename
        &db,                    // db
        flags,                  // flags
        NULL);
    JSBATON_ASSERT_OK();
    dbCount += 1;
    // save db
    baton->argv[0] = (int64_t) db;
  catch_error:
    (void) 0;
}

// SQLMATH_API db - end

// SQLMATH_API doublewin - start
SQLMATH_API void doublewinAggfree(
    Doublewin ** dblwinAgg
) {
    if (dblwinAgg != NULL) {
        sqlite3_free(*dblwinAgg);
        *dblwinAgg = NULL;
    }
}

SQLMATH_API int doublewinAggmalloc(
    Doublewin ** dblwinAgg,
    const int nhead
) {
    if (dblwinAgg && *dblwinAgg) {
        return SQLITE_OK;
    }
    const int alloc = sizeof(Doublewin) + nhead * sizeof(double) + 256;
    if (dblwinAgg == NULL       //
        || nhead < 0            //
        || alloc <= 0 || SIZEOF_BLOB_MAX < alloc) {
        doublewinAggfree(dblwinAgg);
        return SQLITE_NOMEM;
    }
    Doublewin *dblwin = sqlite3_malloc(alloc);
    if (dblwin == NULL) {
        doublewinAggfree(dblwinAgg);
        return SQLITE_NOMEM;
    }
    // Zero dblwin after malloc.
    memset(dblwin, 0, alloc);
    dblwin->alloc = alloc;
    dblwin->nhead = doubleMax(0, nhead);
    *dblwinAgg = dblwin;
    return SQLITE_OK;
}

SQLMATH_API int doublewinAggpush(
    Doublewin ** dblwinAgg,
    double xx
) {
    if (dblwinAgg == NULL || *dblwinAgg == NULL) {
        return SQLITE_NOMEM;
    }
    Doublewin *dblwin = *dblwinAgg;
    // rotate wnn
    if (dblwin->wnn) {
        doublewinBody(dblwin)[(int) dblwin->waa] = xx;
        dblwin->waa += 1;
        if (dblwin->waa >= dblwin->nbody) {
            dblwin->waa = 0;
        }
        return SQLITE_OK;
    }
    const int nn =
        sizeof(Doublewin) / sizeof(double) + dblwin->nhead + dblwin->nbody;
    uint32_t alloc = dblwin->alloc;
    if (nn * sizeof(double) >= alloc) {
        // error - toobig
        if (alloc <= 0 || SIZEOF_BLOB_MAX <= alloc) {
            doublewinAggfree(dblwinAgg);
            return SQLITE_NOMEM;
        }
        alloc = MATH_MIN(SIZEOF_BLOB_MAX, 2 * alloc);
        dblwin = sqlite3_realloc(dblwin, alloc);
        // error - nomem
        if (dblwin == NULL) {
            doublewinAggfree(dblwinAgg);
            return SQLITE_NOMEM;
        }
        // Zero dblwin after realloc.
        memset(                 //
            (char *) dblwin + (int) dblwin->alloc,      //
            0,                  //
            alloc - (int) dblwin->alloc);
        dblwin->alloc = alloc;
        *dblwinAgg = dblwin;
    }
    ((double *) dblwin)[nn] = xx;
    dblwin->nbody += 1;
    return SQLITE_OK;
}

SQLMATH_API double *doublewinBody(
    const Doublewin * dblwin
) {
    return (double *) ((char *) dblwin + sizeof(Doublewin)) +
        (int) dblwin->nhead;
}

SQLMATH_API double *doublewinHead(
    const Doublewin * dblwin
) {
    return (double *) ((char *) dblwin + sizeof(Doublewin));
}

SQLMATH_API void doublewinResultBlob(
    Doublewin * dblwin,
    sqlite3_context * context
) {
// This function will return <dblwin> as result-blob in given <context>.
    dblwin->alloc =
        sizeof(Doublewin) + (dblwin->nhead + dblwin->nbody) * sizeof(double);
    sqlite3_result_blob(context, (const char *) dblwin, dblwin->alloc,
        // destructor
        sqlite3_free);
}

// SQLMATH_API doublewin - end

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

SQLMATH_API int jsbatonAssertOk(
    Jsbaton * baton,
    sqlite3 * db,
    int errcode
) {
// This function will assert <errcode> == SQLITE_OK.
    if (errcode == 0) {
        return 0;
    }
    jsbatonSetErrmsg(baton, (   //
            errcode == SQLITE_ERROR_DATATYPE_INVALID    //
            ? "sqlmath._dbExec - invalid datatype"      //
            : errcode == SQLITE_ERROR_ZSQL_NULL //
            ? "sqlmath._dbExec - cannot execute null sql-string"        //
            : db == NULL || sqlite3_errcode(db) == SQLITE_OK    //
            ? sqlite3_errstr(errcode)   //
            : sqlite3_errmsg(db)));
    return errcode;
}

SQLMATH_API char *jsbatonGetErrmsg(
    Jsbaton * baton
) {
// This function will return <baton>->errmsg.
    return baton->errmsg;
}

SQLMATH_API int64_t jsbatonGetInt64(
    Jsbaton * baton,
    int argi
) {
// This function will return int64-value from <baton> at <argi>.
    return baton->argv[argi];
}

SQLMATH_API char *jsbatonGetString(
    Jsbaton * baton,
    int argi
) {
// This function will return string-value from <baton> at <argi>.
    if (baton->argv[argi] == 0) {
        return NULL;
    }
    return (char *) baton + ((size_t) baton->argv[argi] + 1 + 4);
}

SQLMATH_API void jsbatonSetErrmsg(
    Jsbaton * baton,
    const char *format,
    ...
) {
// This function will set <baton>->errmsg.
    if (baton == NULL) {
        return;
    }
    char *buf = jsbatonGetErrmsg(baton);
    if (buf[0] != '\x00') {
        return;
    }
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, SIZEOF_ERRMSG, format, argptr);
    va_end(argptr);
    buf[SIZEOF_ERRMSG - 1] = '\x00';
    if (buf[0] == '\x00') {
        buf[0] = '?';
    }
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

SQLMATH_API void sqlite3_result_double_or_null(
    sqlite3_context * context,
    const double xx
) {
// This function will return NULL if <xx> is not finite else <xx>.
    sqlite3_result_double(context, isfinite(xx) ? xx : NAN);
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

SQLMATH_FUNC static void sql1_fmod_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return fmod(dividend, divisor).
    UNUSED_PARAMETER(argc);
    sqlite3_result_double_or_null(context, fmod(        //
            sqlite3_value_double_or_nan(argv[0]),       //
            sqlite3_value_double_or_nan(argv[1])));
}

SQLMATH_FUNC static void sql1_marginoferror95_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate margin-of-error sqrt(pp*(1-pp)/nn).
    UNUSED_PARAMETER(argc);
    const double pp = sqlite3_value_double_or_nan(argv[0]);
    const double nn = sqlite3_value_double_or_nan(argv[1]);
    sqlite3_result_double(context,
        1.9599639845400543 * sqrt(pp * (1 - pp) / nn));
}

SQLMATH_FUNC static void sql1_normalizewithsqrt_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will sqrt() and normalize <xx> to +/-1.
    UNUSED_PARAMETER(argc);
    double xx = sqlite3_value_double_or_nan(argv[0]);
    sqlite3_result_double(context,      //
        doubleMax(-1, doubleMin(1, xx < 0 ? -sqrt(-xx) : sqrt(xx))));
}

SQLMATH_FUNC static void sql1_normalizewithsquared_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will squared() and normalize <xx> to +/-1.
    UNUSED_PARAMETER(argc);
    double xx = sqlite3_value_double_or_nan(argv[0]);
    sqlite3_result_double(context,      //
        doubleMax(-1, doubleMin(1, xx < 0 ? -xx * xx : xx * xx)));
}

SQLMATH_FUNC static void sql1_random1_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will generate high-quality random-float between 0 <= xx < 1.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    uint32_t xx[1];
    sqlite3_randomness(4, (void *) xx);
    sqlite3_result_double(context, ((double) *xx) / 0x100000000);
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
    nn = MATH_MIN(nn, 30);
    nn = MATH_MAX(nn, 0);
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

SQLMATH_FUNC static void sql1_sqrtwithsign_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return sqrt() of magnitude of value, with sign preserved.
    UNUSED_PARAMETER(argc);
    const double xx = sqlite3_value_double_or_nan(argv[0]);
    sqlite3_result_double(context, xx < 0 ? -sqrt(-xx) : sqrt(xx));
}

SQLMATH_FUNC static void sql1_squaredwithsign_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return squared() of magnitude of value,
// with sign preserved.
    UNUSED_PARAMETER(argc);
    const double xx = sqlite3_value_double_or_nan(argv[0]);
    sqlite3_result_double(context, xx < 0 ? -xx * xx : xx * xx);
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
                MATH_SWAP(arr[ll], arr[ir], tmp);
            }
            return arr[kk];
        } else {
            mid = (ll + ir) >> 1;
            MATH_SWAP(arr[mid], arr[ll + 1], tmp);
            if (arr[ll] > arr[ir]) {
                MATH_SWAP(arr[ll], arr[ir], tmp);
            }
            if (arr[ll + 1] > arr[ir]) {
                MATH_SWAP(arr[ll + 1], arr[ir], tmp);
            }
            if (arr[ll] > arr[ll + 1]) {
                MATH_SWAP(arr[ll], arr[ll + 1], tmp);
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
                MATH_SWAP(arr[ii], arr[jj], tmp);
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
    double kmod = MATH_MAX(0, MATH_MIN(1, qq)) * (nn - 1);
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
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - null-case
    if (dblwin->nbody == 0) {
        sqlite3_result_null(context);
        goto catch_error;
    }
    sqlite3_result_double(context, quantile(dblwin_body, dblwin->nbody,
            dblwin_head[0]));
  catch_error:
    doublewinAggfree(dblwinAgg);
}

static void sql2_quantile_step0(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv,
    const double qq
) {
// This function will aggregate kth-quantile element.
    UNUSED_PARAMETER(argc);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(1);
    if (dblwin->nbody == 0) {
        dblwin_head[0] = qq;    // kth-quantile
    }
    // dblwin - append isfinite
    const double xx = sqlite3_value_double_or_nan(argv[0]);
    if (!isnan(xx)) {
        DOUBLEWIN_AGGREGATE_PUSH(xx);
    }
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

// SQLMATH_FUNC sql3_stdev_func - start
typedef struct AggStdev {
    double mxx;                 // x-average
    double nnn;                 // number of elements
    double vxx;                 // x-variance.p
    double wnn;                 // number of window elements
    double xx0;                 // x-trailing
} AggStdev;

SQLMATH_FUNC static void sql3_stdev_value(
    sqlite3_context * context
) {
// This function will aggregate elements and calculate sample-stdev.
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggStdev);
    // agg - null-case
    if (agg->nnn <= 0) {
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_double(context,
        agg->nnn == 1 ? 0 : sqrt(agg->vxx / (agg->nnn - 1)));
}

SQLMATH_FUNC static void sql3_stdev_final(
    sqlite3_context * context
) {
// This function will aggregate elements and calculate sample-stdev.
    // agg - value
    sql3_stdev_value(context);
}

SQLMATH_FUNC static void sql3_stdev_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    UNUSED_PARAMETER(argc);
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggStdev);
    // agg - welford - increment agg->vxx
    if (sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL) {
        agg->wnn = agg->nnn;
        agg->xx0 = sqlite3_value_double(argv[0]);
    }
}

static void sql3_stdev_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate elements and calculate sample-stdev.
    UNUSED_PARAMETER(argc);
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggStdev);
    // agg - welford - increment agg->vxx
    if (sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL) {
        const double xx = sqlite3_value_double(argv[0]);
        if (agg->wnn) {
            // calculate vxx - window
            const double invn0 = 1.0 / agg->nnn;
            const double xx0 = agg->xx0;
            const double dx = xx - xx0;
            agg->vxx +=
                (xx * xx - xx0 * xx0) - dx * (invn0 * dx + 2 * agg->mxx);
            agg->mxx += dx * invn0;
        } else {
            // calculate vxx - welford
            const double dx = xx - agg->mxx;
            agg->nnn += 1;
            agg->mxx += dx / agg->nnn;
            agg->vxx += dx * (xx - agg->mxx);
        }
    }
}

// SQLMATH_FUNC sql3_stdev_func - end

// SQLMATH_FUNC sql3_win_coinflip2_func - start
typedef struct WinCoinflip {
    double nflip;               // number of coin-flips
    double nhead;               // number of coin-flips landing on head
    double nstreak;             // current head/tail win-streak
    double ntail;               // number of coin-flips landing on tail
    double outcome;             // current-outcome - head=+1, tail=-1, tie=0
} WinCoinflip;
static const int WIN_COINFLIP_N = sizeof(WinCoinflip) / sizeof(double);

SQLMATH_FUNC static void sql3_win_coinflip2_value(
    sqlite3_context * context
) {
// This function will aggregate bias of given coin-flips landing on head,
// ranging from -0.5 (never head) to 0.0 (no bias) to 0.5 (always head).
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - result
    doublearrayResult(context, dblwin_head, dblwin->nhead, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_coinflip2_final(
    sqlite3_context * context
) {
// This function will aggregate bias of given coin-flips landing on head,
// ranging from -0.5 (never head) to 0.0 (no bias) to 0.5 (always head).
    // dblwin - value
    sql3_win_coinflip2_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_coinflip2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate bias of given coin-flips landing on head,
// ranging from -0.5 (never head) to 0.0 (no bias) to 0.5 (always head).
    UNUSED_PARAMETER(argv);
    // dblwin - init
    const int ncol = argc;
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - inverse
    WinCoinflip *agg = (WinCoinflip *) dblwin_head;
    for (int ii = 0; ii < ncol; ii += 1) {
        const int outcome = doubleSign(sqlite3_value_double(argv[0]));
        const int sstreak = doubleSign(agg->nstreak);
        agg->nflip -= 1;
        agg->nhead -= outcome == +1;
        agg->nstreak =
            sstreak * doubleMin(sstreak * agg->nstreak, agg->nflip);
        agg->ntail -= outcome == -1;
        // increment counter
        agg += 1;
        argv += 1;
    }
}

SQLMATH_FUNC static void sql3_win_coinflip2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate bias of given coin-flips landing on head,
// ranging from -0.5 (never head) to 0.0 (no bias) to 0.5 (always head).
    if (argc < 1) {
        sqlite3_result_error(context,
            "win_coinflip2() - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = argc;
    DOUBLEWIN_AGGREGATE_CONTEXT(ncol * WIN_COINFLIP_N);
    // dblwin - calculate coin-flip-bias
    WinCoinflip *agg = (WinCoinflip *) dblwin_head;
    for (int ii = 0; ii < ncol; ii += 1) {
        const int outcome = doubleSign(sqlite3_value_double(argv[0]));
        const int sstreak = doubleSign(agg->nstreak);
        agg->nflip += 1;
        agg->nhead += outcome == +1;
        agg->nstreak = outcome == sstreak ? agg->nstreak + outcome : outcome;
        agg->ntail += outcome == -1;
        agg->outcome = outcome;
        // increment counter
        agg += 1;
        argv += 1;
    }
}

SQLMATH_FUNC static void sql1_coinflip_extract_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will extract <val> from WinCoinflip-object, with given <key>.
    UNUSED_PARAMETER(argc);
    // validate argv
    const int icol = sqlite3_value_int(argv[1]);
    const uint32_t bytes = (uint32_t) sqlite3_value_bytes(argv[0]);
    if (icol < 0) {
        sqlite3_result_error(context,
            "coinflip_extract() - 2nd argument must be integer column >= 0",
            -1);
        return;
    }
    if (bytes <= 0 || SIZEOF_BLOB_MAX < bytes   //
        || bytes < (icol + 1) * WIN_COINFLIP_N * sizeof(double)) {
        sqlite3_result_error(context,
            "coinflip_extract()"
            " - 1st argument as coinflip-object does not have enough columns",
            -1);
        return;
    }
    const WinCoinflip *agg =
        (WinCoinflip *) sqlite3_value_blob(argv[0]) + icol;
    const char *key = (const char *) sqlite3_value_text(argv[2]);
    const char *keyList[] = {
        "nflip",
        "nhead",
        "nstreak",
        "ntail",
        "outcome"
    };
    for (int ii = 0; ii < WIN_COINFLIP_N; ii += 1) {
        if (strcmp(key, keyList[ii]) == 0) {
            sqlite3_result_int(context, (int) (((double *) agg)[ii]));
            return;
        }
    }
    // bias for head
    if (strcmp(key, "biashead") == 0) {
        sqlite3_result_double_or_null(context, agg->nhead / agg->nflip - 0.5);
        return;
    }
    // bias for head or tail, whichever is greater in magnitude
    if (strcmp(key, "biasmax") == 0) {
        sqlite3_result_double_or_null(context, (        //
                doubleAbs(agg->ntail) > doubleAbs(agg->nhead)   //
                ? -agg->ntail / agg->nflip + 0.5        //
                : +agg->nhead / agg->nflip - 0.5));     //
        return;
    }
    // bias for tail
    if (strcmp(key, "biastail") == 0) {
        sqlite3_result_double_or_null(context, agg->ntail / agg->nflip - 0.5);
        return;
    }
    // fair-coin-flip margin-of-error within 95th percentile
    if (strcmp(key, "moe95") == 0) {
        sqlite3_result_double_or_null(context,
            1.9599639845400543 * 0.5 / sqrt(agg->nflip));
        return;
    }
    // fair-coin-flip stdev.s
    if (strcmp(key, "stdev") == 0) {
        sqlite3_result_double_or_null(context, 0.5 / sqrt(agg->nflip));
        return;
    }
    sqlite3_result_error(context,
        "coinflip_extract() - 3rd argument is invalid key", -1);
}

// SQLMATH_FUNC sql3_win_coinflip2_func - end

// SQLMATH_FUNC sql3_win_sinefit2_func - start
typedef struct WinSinefit {
    double laa;                 // linest y-intercept
    double lbb;                 // linest slope
    //
    double mxx;                 // x-average
    double myy;                 // y-average
    double nnn;                 // number of elements
    //
    double rr0;                 // r-trailing
    double rr1;                 // r-current
    //
    double saa;                 // sine amplitude
    double see;                 // sine y-stdev.s5
    double spp;                 // sine phase
    double sww;                 // sine angular-frequency
    //
    double vxx;                 // y-variance.p
    double vxy;                 // xy-covariance.p
    double vyy;                 // y-variance.p
    //
    double wbb;                 // window-position-right
    double wnn;                 // number of window elements
    //
    double xx0;                 // x-trailing
    double xx1;                 // x-current
    double xx2;                 // x-refit
    double yy0;                 // y-trailing
    double yy1;                 // y-current
} WinSinefit;
static const int WIN_SINEFIT_N = sizeof(WinSinefit) / sizeof(double);
static const int WIN_SINEFIT_STEP = 3 + 2;

static void winSinefitSnr(
    WinSinefit * wsf,
    double *xxyy,
    const int wbb,
    const int nbody,
    const int ncol
) {
// This function will calculate running sine-regression as:
//     yy = saa*sin(sww*xx + spp)
    // declare var0
    const double nnn = nbody / (ncol * WIN_SINEFIT_STEP);
    const double invn0 = 1.0 / nnn;
    const double wtt0 = sqrt(12.0 * invn0 * wsf->vxx);  // window-period
    double saa = 0;
    double spp = 0;
    double sww = 0;
    double tmp = 0;
    // calculate snr - saa
    saa = sqrt(2 * wsf->vyy * invn0     //
        * (1 - wsf->vxy * wsf->vxy / (wsf->vxx * wsf->vyy)));
    // calculate snr - sww - using incremental-discrete-fourier-transform
    {
        const double ibb = 2 * MATH_PI * (wbb / (ncol * WIN_SINEFIT_STEP));
        const double rr0 = isfinite(wsf->rr0) ? wsf->rr0 : 0;
        const double rr1 = isfinite(wsf->rr1) ? wsf->rr1 : 0;
        const double rr2 = wsf->wnn ? rr1 - rr0 : rr1;
        double cfkmax = 0;
        double tmp = 0;
        int kk = 0;
        for (int ii = 0; ii < nbody; ii += ncol * WIN_SINEFIT_STEP) {
            // rr   = yy - (laa + lbb*tt)
            // dfkr = cos(2*pi/nnn*kk*ibb)*(rr1 - rr0)
            // dfki = sin(2*pi/nnn*kk*ibb)*(rr1 - rr0)
            // sfk = sfk + sum(dfkr)^2 + sum(dfki)^2
            xxyy[ii + 3] += cos(kk * ibb) * rr2;
            xxyy[ii + 4] += sin(kk * ibb) * rr2;
            tmp = pow(xxyy[ii + 3], 2) + pow(xxyy[ii + 4], 2);
            if (                //
                1 <= kk         //
                && kk <= 0.5 * nnn      //
                && kk <= doubleMax(0.03125 * nnn, 4.0 / nnn)    //
                && cfkmax < tmp) {
                cfkmax = tmp;
                sww = kk;
            }
            kk += 1;
        }
        sww *= 2 * MATH_PI / wtt0;
    }
    const double inva = 1.0 / saa;
    if (inva <= 0 || !isfinite(inva) || !isnormal(sww)) {
        return;
    }
    // calculate snr - spp - using multivariate-linear-regression
    if (1) {
        double sumxx = 0;
        double sumxy = 0;
        double sumxz = 0;
        double sumyy = 0;
        double sumyz = 0;
        // szz  ~ sin(sww*tt + spp)
        // szz  ~ sbb*sxx + scc*syy
        // sbb  = sin(spp)
        // scc  = cos(spp)
        // sxx  = cos(sww*tt)
        // syy  = sin(sww*tt)
        // rr   = szz - sbb*sxx - scc*syy
        // gb   = d/db[z-b*x-c*y]^2 = -2*x*(z - b*x - c*y)
        // gc   = d/dc[z-b*x-c*y]^2 = -2*y*(z - b*x - c*y)
        // invp = 1/(sum(xx)*sum(yy) - sum(xy)^2)
        // sbb  = invp*(sum(yy)*sum(xz) - sum(xy)*sum(yz))
        // scc  = invp*(sum(xx)*sum(yz) - sum(xy)*sum(xz))
        // spp  = asin(sbb) = acos(scc)
        // spp  = atan(sbb/scc)
        for (int ii = 0; ii < nbody; ii += ncol * WIN_SINEFIT_STEP) {
            tmp = sww * xxyy[ii + 0];
            const double sxx = cos(tmp);
            const double syy = sin(tmp);
            const double szz = inva * xxyy[ii + 2];
            sumxx += sxx * sxx;
            sumxy += sxx * syy;
            sumxz += sxx * szz;
            sumyy += syy * syy;
            sumyz += syy * szz;
        }
        const double sbb = sumyy * sumxz - sumxy * sumyz;
        const double scc = sumxx * sumyz - sumxy * sumxz;
        spp = atan(sbb / scc);
        if (!isfinite(spp)) {
            spp = 0;
        }
    }
    // Offset spp by pi if root of derivative is maxima instead of minima.
    if (1) {
        const double spp2 = fmod(spp + MATH_PI, 2 * MATH_PI);
        double dr = 0;
        double mrr1 = 0;        // r-average
        double mrr2 = 0;        // r-average
        double rr = 0;
        double vrr1 = 0;        // r-variance.p
        double vrr2 = 0;        // r-variance.p
        for (int ii = 0; ii < nbody; ii += ncol * WIN_SINEFIT_STEP) {
            tmp = fmod(sww * xxyy[ii + 0], 2 * MATH_PI);
            // welford - increment vrr1
            rr = xxyy[ii + 2] - saa * sin(tmp + spp);
            dr = rr - mrr1;
            mrr1 += dr * invn0;
            vrr1 += dr * (rr - mrr1);
            // welford - increment vrr2
            rr = xxyy[ii + 2] - saa * sin(tmp + spp2);
            dr = rr - mrr2;
            mrr2 += dr * invn0;
            vrr2 += dr * (rr - mrr2);
        }
        if (vrr2 < vrr1) {
            spp = spp2;
        }
    }
    // calculate snr - spp, sww - using gauss-newton-method
    for (int jj = 4; jj > 0; jj -= 1) {
        // for (int jj = sqrt(nnn); jj > 1; jj -= 1) {
        double gp = 0;          // gradient-phase
        double gw = 0;          // gradient-frequency
        double hpp = 0;         // hessian ddr/dpdp
        double hpw = 0;         // hessian ddr/dpdw
        double hww = 0;         // hessian ddr/dwdw
        double sxx = 0;
        double sxy = 0;
        // yy   ~ saa*sin(sww*tt + spp)
        // cost = cos(sww*tt + spp)
        // sint = sin(sww*tt + spp)
        // sxx  = sint*sint
        // sxy  = sint*yy
        // saa  = sxy/sxx
        //
        // yy   ~ saa*sin(sww*tt + spp)
        // cost = cos(sww*tt + spp)
        // sint = sin(sww*tt + spp)
        // rr   = yy/saa - sint
        // gp   =     d/dp[y-sin(w*t+p)]^2 = 2*(        0 - cost*rr)
        // gw   =     d/dw[y-sin(w*t+p)]^2 = 2*(        0 - cost*rr)*tt
        // hpp  = d^2/dpdp[y-sin(w*t+p)]^2 = 2*(cost*cost + sint*rr)
        // hpw  = d^2/dpdw[y-sin(w*t+p)]^2 = 2*(cost*cost + sint*rr)*tt
        // hww  = d^2/dwdw[y-sin(w*t+p)]^2 = 2*(cost*cost + sint*rr)*tt*tt
        for (int ii = 0; ii < nbody; ii += ncol * WIN_SINEFIT_STEP) {
            const double tt = xxyy[ii + 0];
            tmp = fmod(sww * tt, 2 * MATH_PI) + spp;
            const double cost = cos(tmp);
            const double sint = sin(tmp);
            // solve saa
            sxx += sint * sint;
            sxy += sint * xxyy[ii + 2];
            // solve spp, sww
            const double rr = inva * xxyy[ii + 2] - sint;
            tmp = -cost * rr;
            gp += tmp;
            gw += tmp * tt;
            tmp = cost * cost;
            // tmp = cost * cost + sint * rr;
            hpp += tmp;
            hpw += tmp * tt;
            hww += tmp * tt * tt;
        }
        // [hpp hpw][dp] = [gp]
        // [hpw hww][dw] = [gw]
        // [dp] = 1 /    [ hww -hpw][gp]
        // [dw] =  / det [-hpw  hpp][gw]
        // det  = hpp*hww - hpw*hpw
        // dp   = 1/det*( hww*gp - hpw*gw)
        // dw   = 1/det*(-hpw*gp + hpp*gw)
        // spp  = spp - dp
        // sww  = sww - dw
        const double invd = 1.0 / (hpp * hww - hpw * hpw);
        if (!isfinite(invd)) {
            return;
        }
        saa = sxy / sxx;
        spp -= invd * (+hww * gp - hpw * gw);
        sww -= invd * (-hpw * gp + hpp * gw);
        spp = fmod(spp, 2 * MATH_PI);
    }
    // calculate spp - shift phase to left for better prediction
    // spp += 0.0625 * MATH_PI;
    // calculate snr - see
    if (1) {
        const double spp2 = fmod(spp + MATH_PI, 2 * MATH_PI);
        double dr = 0;
        double mrr1 = 0;        // r-average
        double mrr2 = 0;        // r-average
        double rr = 0;
        double vrr1 = 0;        // r-variance.p
        double vrr2 = 0;        // r-variance.p
        for (int ii = 0; ii < nbody; ii += ncol * WIN_SINEFIT_STEP) {
            tmp = fmod(sww * xxyy[ii + 0], 2 * MATH_PI);
            // welford - increment vrr1
            rr = xxyy[ii + 2] - saa * sin(tmp + spp);
            dr = rr - mrr1;
            mrr1 += dr * invn0;
            vrr1 += dr * (rr - mrr1);
            // welford - increment vrr2
            rr = xxyy[ii + 2] - saa * sin(tmp + spp2);
            dr = rr - mrr2;
            mrr2 += dr * invn0;
            vrr2 += dr * (rr - mrr2);
        }
        // Offset phase by pi if root of derivative is maxima instead of minima.
        if (vrr2 < vrr1) {
            spp = spp2;
            vrr1 = vrr2;
        }
        wsf->see = sqrt(vrr1 / (nnn - 5));
    }
    // save wsf
    if (spp < 0) {
        spp += 2 * MATH_PI;
    }
    wsf->saa = saa;
    wsf->spp = spp;
    wsf->sww = sww;
}

static void winSinefitLnr(
    WinSinefit * wsf,
    double *xxyy,
    const int wbb
) {
// This function will calculate running simple-linear-regression as:
//     yy = laa + lbb*xx
    const double invn0 = 1.0 / wsf->nnn;
    const double xx = wsf->xx1;
    const double yy = wsf->yy1;
    double mxx = wsf->mxx;
    double myy = wsf->myy;
    double vxx = wsf->vxx;
    double vxy = wsf->vxy;
    double vyy = wsf->vyy;
    if (wsf->wnn) {
        // calculate running lnr - window
        const double xx0 = wsf->xx0;
        const double yy0 = wsf->yy0;
        const double dx = xx - xx0;
        const double dy = yy - yy0;
        vxx += (xx * xx - xx0 * xx0) - dx * (invn0 * dx + 2 * mxx);
        vyy += (yy * yy - yy0 * yy0) - dy * (invn0 * dy + 2 * myy);
        vxy += (xx * yy - xx0 * yy0) - dx * myy - dy * (invn0 * dx + mxx);
        mxx += dx * invn0;
        myy += dy * invn0;
    } else {
        // calculate running lnr - welford
        const double dx = xx - mxx;
        const double dy = yy - myy;
        // welford - increment vxx
        mxx += dx * invn0;
        vxx += dx * (xx - mxx);
        // welford - increment vyy
        myy += dy * invn0;
        vyy += dy * (yy - myy);
        // welford - increment vxy
        vxy += dy * (xx - mxx);
    }
    // calculate lnr - laa, lbb
    const double lbb = vxy / vxx;
    const double laa = myy - lbb * mxx;
    const double rr = yy - (laa + lbb * xx);
    // wsf - save
    wsf->laa = laa;
    wsf->lbb = lbb;
    wsf->mxx = mxx;
    wsf->myy = myy;
    wsf->rr1 = rr;
    wsf->vxx = vxx;
    wsf->vxy = vxy;
    wsf->vyy = vyy;
    // save rr1 in window
    xxyy[wbb + 2] = isfinite(rr) ? rr : 0;
}

SQLMATH_FUNC static void sql3_win_sinefit2_value(
    sqlite3_context * context
) {
// This function will calculate running simple-linear-regression
// and sine-regression as:
//     yy = laa + lbb*xx + saa*sin(sww*xx + spp)
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    const WinSinefit *wsf = (const WinSinefit *) dblwin_head;
    // dblwin - result
    doublearrayResult(context, dblwin_head,     //
        // If x-current == x-refit, then include extra data needed for refit.
        // This data is normally not included, due to memory performance.
        wsf->xx2 == wsf->xx1 ? dblwin->nhead + dblwin->nbody : dblwin->nhead,
        SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_sinefit2_final(
    sqlite3_context * context
) {
// This function will calculate running simple-linear-regression
// and sine-regression as:
//     yy = laa + lbb*xx + saa*sin(sww*xx + spp)
    // dblwin - value
    sql3_win_sinefit2_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_sinefit2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running simple-linear-regression
// and sine-regression as:
//     yy = laa + lbb*xx + saa*sin(sww*xx + spp)
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->wnn) {
        dblwin->wnn = dblwin->nbody;
    }
}

static void sql3_win_sinefit2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running simple-linear-regression
// and sine-regression as:
//     yy = laa + lbb*xx + saa*sin(sww*xx + spp)
    static const int argc0 = 2;
    if (argc < argc0 + 2 || (argc - argc0) % 2) {
        sqlite3_result_error(context,
            "win_sinefit2() - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = (argc - argc0) / 2;
    DOUBLEWIN_AGGREGATE_CONTEXT(ncol * WIN_SINEFIT_N);
    if (dblwin->nbody == 0) {
        // dbwin - init ncol
        dblwin->ncol = ncol;
    }
    // dblwin - init argv
    const double xx2 = sqlite3_value_double_or_nan(argv[1]);
    const int modeSnr = sqlite3_value_int(argv[0]);
    argv += argc0;
    WinSinefit *wsf = NULL;
    const int waa = dblwin->waa;
    const int wbb = dblwin->wnn ? dblwin->waa : dblwin->nbody;
    double *xxyy = NULL;
    for (int ii = 0; ii < ncol; ii += 1) {
        // dblwin - init xx, yy, rr
        wsf = (WinSinefit *) dblwin_head + ii;
        sqlite3_value_double_or_prev(argv[0], &wsf->xx1);
        sqlite3_value_double_or_prev(argv[1], &wsf->yy1);
        // bugfix - Fix buffer-overlow, reading pointer past dblwin->nbody.
        if (dblwin->nbody) {
            xxyy = dblwin_body + ii * WIN_SINEFIT_STEP;
            wsf->rr0 = xxyy[waa + 2];
            wsf->xx0 = xxyy[waa + 0];
            wsf->yy0 = xxyy[waa + 1];
        }
        wsf->wbb = wbb;
        wsf->xx2 = xx2;
        const double xx = wsf->xx1;
        const double yy = wsf->yy1;
        // dblwin - push xx, yy, rr, sff
        DOUBLEWIN_AGGREGATE_PUSH(xx);
        DOUBLEWIN_AGGREGATE_PUSH(yy);
        for (int jj = 2; jj < WIN_SINEFIT_STEP; jj += 1) {
            DOUBLEWIN_AGGREGATE_PUSH(0);
        }
        // increment counter
        argv += 2;
    }
    // dblwin - calculate lnr, snr
    wsf = (WinSinefit *) dblwin_head;
    xxyy = dblwin_body;
    for (int ii = 0; ii < ncol; ii += 1) {
        wsf->nnn = dblwin->nbody / (ncol * WIN_SINEFIT_STEP);
        wsf->wnn = dblwin->wnn;
        // dblwin - calculate lnr
        winSinefitLnr(wsf, xxyy, wbb);
        // dblwin - calculate snr
        if (modeSnr) {
            winSinefitSnr(wsf, xxyy, wbb, dblwin->nbody, dblwin->ncol);
        }
        // increment counter
        wsf += 1;
        xxyy += WIN_SINEFIT_STEP;
    }
}

SQLMATH_FUNC static void sql1_sinefit_extract_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will extract <val> from WinSinefit-object, with given <key>.
    UNUSED_PARAMETER(argc);
    // validate argv
    const int icol = sqlite3_value_int(argv[1]);
    const uint32_t bytes = (uint32_t) sqlite3_value_bytes(argv[0]);
    if (icol < 0) {
        sqlite3_result_error(context,
            "sinefit_extract() - 2nd argument must be integer column >= 0",
            -1);
        return;
    }
    if (bytes <= 0 || SIZEOF_BLOB_MAX < bytes   //
        || bytes < (icol + 1) * WIN_SINEFIT_N * sizeof(double)) {
        sqlite3_result_error(context,
            "sinefit_extract()"
            " - 1st argument as sinefit-object does not have enough columns",
            -1);
        return;
    }
    const WinSinefit *wsf = (WinSinefit *) sqlite3_value_blob(argv[0]) + icol;
    const char *key = (const char *) sqlite3_value_text(argv[2]);
    const char *keyList[] = {
        "laa",
        "lbb",
        //
        "mxx",
        "myy",
        "nnn",
        //
        "rr0",
        "rr1",
        //
        "saa",
        "see",
        "spp",
        "sww",
        //
        "vxx",
        "vxy",
        "vyy",
        //
        "wbb",
        "wnn",
        //
        "xx0",
        "xx1",
        "xx2",
        "yy0",
        "yy1"
    };
    for (int ii = 0; ii < WIN_SINEFIT_N; ii += 1) {
        if (strcmp(key, keyList[ii]) == 0) {
            sqlite3_result_double_or_null(context, ((double *) wsf)[ii]);
            return;
        }
    }
    // gaussian-normalized y-value
    if (strcmp(key, "gyy") == 0) {
        sqlite3_result_double_or_null(context,  //
            (wsf->yy1 - wsf->myy) * sqrt((wsf->nnn - 1) / wsf->vyy));
        return;
    }
    // linest y-stdev.s2
    if (strcmp(key, "lee") == 0) {
        sqlite3_result_double_or_null(context, sqrt(    //
                wsf->vyy        //
                * (1 - wsf->vxy * wsf->vxy / (wsf->vxx * wsf->vyy))     //
                / (wsf->nnn - 2)));
        return;
    }
    // linest pearson-correlation xy
    if (strcmp(key, "lxy") == 0) {
        sqlite3_result_double_or_null(context,
            wsf->vxy / sqrt(wsf->vxx * wsf->vyy));
        return;
    }
    // linest y-estimate
    if (strcmp(key, "lyy") == 0) {
        sqlite3_result_double_or_null(context, wsf->yy1 - wsf->rr1);
        return;
    }
    // y-stdev.s1
    if (strcmp(key, "mee") == 0) {
        sqlite3_result_double_or_null(context,
            sqrt(wsf->vyy / (wsf->nnn - 1)));
        return;
    }
    // x-stdev.s1
    if (strcmp(key, "mxe") == 0) {
        sqlite3_result_double_or_null(context,
            sqrt(wsf->vxx / (wsf->nnn - 1)));
        return;
    }
    // predict simple-linear-regression + sine-wave
    if (strcmp(key, "predict_all") == 0) {
        const double xx = sqlite3_value_double(argv[3]);
        sqlite3_result_double_or_null(context, 0        //
            + wsf->laa + wsf->lbb * xx  //
            + wsf->saa * sin(fmod(wsf->sww * xx, 2 * MATH_PI) + wsf->spp));
        return;
    }
    // predict cosine-wave
    if (strcmp(key, "predict_cnr") == 0) {
        const double xx = sqlite3_value_double(argv[3]);
        sqlite3_result_double_or_null(context, 0        //
            + wsf->saa * cos(fmod(wsf->sww * xx, 2 * MATH_PI) + wsf->spp));
        return;
    }
    // predict cosine-wave normalized between -1, 1
    if (strcmp(key, "predict_cos") == 0) {
        const double xx = sqlite3_value_double(argv[3]);
        sqlite3_result_double_or_null(context, 0        //
            + cos(fmod(wsf->sww * xx, 2 * MATH_PI) + wsf->spp));
        return;
    }
    // predict simple-linear-regression
    if (strcmp(key, "predict_lnr") == 0) {
        const double xx = sqlite3_value_double(argv[3]);
        sqlite3_result_double_or_null(context, 0        //
            + wsf->laa + wsf->lbb * xx);
        return;
    }
    // predict sine-wave normalized between -1, 1
    if (strcmp(key, "predict_sin") == 0) {
        const double xx = sqlite3_value_double(argv[3]);
        sqlite3_result_double_or_null(context, 0        //
            + sin(fmod(wsf->sww * xx, 2 * MATH_PI) + wsf->spp));
        return;
    }
    // predict sine-wave
    if (strcmp(key, "predict_snr") == 0) {
        const double xx = sqlite3_value_double(argv[3]);
        sqlite3_result_double_or_null(context, 0        //
            + wsf->saa * sin(fmod(wsf->sww * xx, 2 * MATH_PI) + wsf->spp));
        return;
    }
    // sine period
    if (strcmp(key, "stt") == 0) {
        sqlite3_result_double_or_null(context, 2 * MATH_PI / wsf->sww);
        return;
    }
    // sine y-estimate
    if (strcmp(key, "syy") == 0) {
        sqlite3_result_double_or_null(context,
            wsf->yy1 - wsf->rr1 +
            wsf->saa * sin(fmod(wsf->sww * wsf->xx1,
                    2 * MATH_PI) + wsf->spp));
        return;
    }
    sqlite3_result_error(context,
        "sinefit_extract() - 3rd argument is invalid key", -1);
}

SQLMATH_FUNC static void sql1_sinefit_refitlast_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will refit last datapoint.
    static const int argc0 = 1;
    // validate argv
    const int ncol = (argc - argc0) / 2;
    const uint32_t bytes = (uint32_t) sqlite3_value_bytes(argv[0]);
    if (argc < argc0 + 2 || argc != argc0 + ncol * 2) {
        goto catch_error;
    }
    if (bytes <= 0 || SIZEOF_BLOB_MAX < bytes   //
        || bytes < ncol * WIN_SINEFIT_N * sizeof(double)) {
        sqlite3_result_error(context,
            "sinefit_refitlast()"
            " - 1st argument as sinefit-object does not have enough columns",
            -1);
        return;
    }
    const WinSinefit *blob0 = sqlite3_value_blob(argv[0]);
    const int nbody = blob0->nnn * ncol * WIN_SINEFIT_STEP;
    if (blob0->nnn <= 0
        || bytes != (ncol * WIN_SINEFIT_N + nbody) * sizeof(double)) {
        sqlite3_result_error(context,
            "sinefit_refitlast()"
            " - 1st argument as sinefit-object does not have enough columns",
            -1);
        return;
    }
    // init wsf0
    WinSinefit *wsf0 = sqlite3_malloc(bytes);
    if (wsf0 == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    memcpy(wsf0, blob0, bytes);
    // dblwin - calculate lnr, snr
    WinSinefit *wsf = wsf0;
    argv += argc0;
    double *xxyy = (double *) (wsf0 + ncol);
    const int wbb = wsf->wbb;
    if (!(0 <= wbb && wbb + ncol * WIN_SINEFIT_STEP <= nbody)) {
        goto catch_error;
    }
    for (int ii = 0; ii < ncol; ii += 1) {
        wsf->wnn = 1;
        wsf->rr0 = wsf->rr1;
        wsf->xx0 = wsf->xx1;
        wsf->yy0 = wsf->yy1;
        sqlite3_value_double_or_prev(argv[0], &wsf->xx1);
        sqlite3_value_double_or_prev(argv[1], &wsf->yy1);
        xxyy[wbb + 0] = wsf->xx1;
        xxyy[wbb + 1] = wsf->yy1;
        // dblwin - calculate lnr
        winSinefitLnr(wsf, xxyy, wbb);
        // dblwin - calculate snr
        winSinefitSnr(wsf, xxyy, wbb, nbody, ncol);
        // increment counter
        argv += 2;
        wsf += 1;
        xxyy += WIN_SINEFIT_STEP;
    }
    // dblwin - result
    doublearrayResult(context, (const double *) wsf0, bytes / sizeof(double),
        sqlite3_free);
    return;
  catch_error:
    sqlite3_result_error(context,
        "sinefit_refitlast() - invalid arguments", -1);
}

// SQLMATH_FUNC sql3_win_sinefit2_func - end

// SQLMATH_FUNC sql3_win_ema1_func - start
SQLMATH_FUNC static void sql3_win_ema1_value(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    sqlite3_result_double(context, dblwin_body[(int) dblwin->waa]);
}

SQLMATH_FUNC static void sql3_win_ema1_final(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // dblwin - value
    sql3_win_ema1_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_ema1_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->wnn) {
        dblwin->wnn = dblwin->nbody;
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
            "win_ema2() - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = argc - 1;
    double arg_alpha = NAN;
    DOUBLEWIN_AGGREGATE_CONTEXT(argc);
    if (dblwin->nbody == 0) {
        // dbwin - init ncol
        dblwin->ncol = ncol;
        // arg_alpha
        arg_alpha = sqlite3_value_double_or_nan(argv[0]);
        if (isnan(arg_alpha)) {
            sqlite3_result_error(context,
                "win_emax() - invalid argument 'alpha'", -1);
            return;
        }
        dblwin_head[ncol + 0] = arg_alpha;
    }
    // dblwin - calculate ema
    arg_alpha = dblwin_head[ncol + 0];
    const int nrow = dblwin->nbody / ncol;
    argv += 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[0], &dblwin_head[ii]);
        double *row = dblwin_body + ii;
        // debug
        // fprintf(stderr,         //
        //     "win_ema2 - nbody=%.0f xx0=%f xx=%f arg_alpha=%f\n",        //
        //     dblwin->nbody, *row, dblwin_head[0], arg_alpha);
        for (int jj = 0; jj < nrow; jj += 1) {
            *row = arg_alpha * dblwin_head[ii] + (1 - arg_alpha) * *row;
            row += ncol;
        }
        argv += 1;
    }
    // dblwin - push xx
    for (int ii = 0; ii < ncol; ii += 1) {
        DOUBLEWIN_AGGREGATE_PUSH(dblwin_head[ii]);
    }
}

// SQLMATH_FUNC sql3_win_ema1_func - end

// SQLMATH_FUNC sql3_win_ema2_func - start
SQLMATH_FUNC static void sql3_win_ema2_value(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->ncol) {
        sqlite3_result_null(context);
    }
    // dblwin - result
    doublearrayResult(context, dblwin_body + (int) dblwin->waa,
        (int) dblwin->ncol, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_ema2_final(
    sqlite3_context * context
) {
// This function will calculate running exponential-moving-average.
    // dblwin - value
    sql3_win_ema2_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
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
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    sqlite3_result_double(context, dblwin_head[(int) dblwin->ncol]);
}

SQLMATH_FUNC static void sql3_win_quantile1_final(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // dblwin - value
    sql3_win_quantile1_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_quantile1_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running quantile.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->wnn) {
        dblwin->wnn = dblwin->nbody;
    }
    // dblwin - invert
    const int ncol = argc / 2;
    const int nstep = ncol * 2;
    const int nn = dblwin->nbody - nstep;
    double *arr = dblwin_body + 1;
    double *xx0 = dblwin_body + 0 + (int) dblwin->waa;
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
    if (argc < 2 || argc % 2) {
        sqlite3_result_error(context,
            "win_quantile2() - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = argc / 2;
    DOUBLEWIN_AGGREGATE_CONTEXT(2 * ncol);
    if (dblwin->nbody == 0) {
        // dbwin - init ncol
        dblwin->ncol = ncol;
    }
    // dblwin - push xx
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[ii * 2 + 1], &dblwin_head[ii]);
        DOUBLEWIN_AGGREGATE_PUSH(dblwin_head[ii]);
        DOUBLEWIN_AGGREGATE_PUSH(       //
            dblwin->wnn ? dblwin_body[(int) dblwin->waa] : INFINITY);
    }
    // dblwin - calculate quantile
    const int nstep = ncol * 2;
    const int nn = dblwin->nbody / nstep;
    double *arr = dblwin_body + 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        // init argQuantile
        double argQuantile = sqlite3_value_double_or_nan(argv[2 * ii + 0]);
        if (!(0 <= argQuantile && argQuantile <= 1)) {
            sqlite3_result_error(context,
                "win_quantilex()"
                " - argument 'quantile' must be between 0 and 1 inclusive",
                -1);
            return;
        }
        argQuantile *= (nn - 1);
        const int kk1 = floor(argQuantile) * nstep;
        const int kk2 = kk1 + nstep;
        argQuantile = fmod(argQuantile, 1);
        // calculate quantile
        const double xx = dblwin_head[ii];
        int jj = (nn - 2) * nstep;
        for (; jj >= 0 && arr[jj] > xx; jj -= nstep) {
            arr[jj + nstep] = arr[jj];
        }
        arr[jj + nstep] = xx;
        dblwin_head[ncol + ii] = argQuantile == 0       //
            ? arr[kk1]          //
            : (1 - argQuantile) * arr[kk1] + argQuantile * arr[kk2];
        arr += 2;
    }
}

// SQLMATH_FUNC sql3_win_quantile1_func - end

// SQLMATH_FUNC sql3_win_quantile2_func - start
SQLMATH_FUNC static void sql3_win_quantile2_value(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->ncol) {
        sqlite3_result_null(context);
    }
    // dblwin - result
    doublearrayResult(context, dblwin_head + (int) dblwin->ncol,
        (int) dblwin->ncol, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_quantile2_final(
    sqlite3_context * context
) {
// This function will calculate running quantile.
    // dblwin - value
    sql3_win_quantile2_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
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
int sqlite3_compress_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
);

int regexp_init(
    sqlite3 * db
);

int sqlite3_sqlmath_custom_init(
    sqlite3 *,
    char **,
    const sqlite3_api_routines *
);

int sqlite3_sqlmath_base_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED_PARAMETER(pApi);
    UNUSED_PARAMETER(pzErrMsg);
    int errcode = 0;
    //
    errcode = sqlite3_compress_init(db, pzErrMsg, pApi);
    SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = regexp_init(db);
    SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_sqlmath_custom_init(db, pzErrMsg, pApi);
    SQLITE_OK_OR_RETURN_RC(errcode);
    //
    SQLITE3_CREATE_FUNCTION1(castrealornull, 1);
    SQLITE3_CREATE_FUNCTION1(castrealorzero, 1);
    SQLITE3_CREATE_FUNCTION1(casttextorempty, 1);
    SQLITE3_CREATE_FUNCTION1(coinflip_extract, 3);
    SQLITE3_CREATE_FUNCTION1(copyblob, 1);
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(doublearray_array, -1);
    SQLITE3_CREATE_FUNCTION1(doublearray_extract, 2);
    SQLITE3_CREATE_FUNCTION1(doublearray_jsonfrom, 1);
    SQLITE3_CREATE_FUNCTION1(doublearray_jsonto, 1);
    SQLITE3_CREATE_FUNCTION1(fmod, 2);
    SQLITE3_CREATE_FUNCTION1(marginoferror95, 2);
    SQLITE3_CREATE_FUNCTION1(normalizewithsqrt, 1);
    SQLITE3_CREATE_FUNCTION1(normalizewithsquared, 1);
    SQLITE3_CREATE_FUNCTION1(roundorzero, 2);
    SQLITE3_CREATE_FUNCTION1(sinefit_extract, 4);
    SQLITE3_CREATE_FUNCTION1(sinefit_refitlast, -1);
    SQLITE3_CREATE_FUNCTION1(sqrtwithsign, 1);
    SQLITE3_CREATE_FUNCTION1(squaredwithsign, 1);
    SQLITE3_CREATE_FUNCTION1(squared, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION2(median, 1);
    SQLITE3_CREATE_FUNCTION2(quantile, 2);
    SQLITE3_CREATE_FUNCTION3(stdev, 1);
    SQLITE3_CREATE_FUNCTION3(win_coinflip2, -1);
    SQLITE3_CREATE_FUNCTION3(win_ema1, 2);
    SQLITE3_CREATE_FUNCTION3(win_ema2, -1);
    SQLITE3_CREATE_FUNCTION3(win_quantile1, 2);
    SQLITE3_CREATE_FUNCTION3(win_quantile2, -1);
    SQLITE3_CREATE_FUNCTION3(win_sinefit2, -1);
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
// This function will assert <errcode> == napi_ok.
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
    static char errbuf[256] = { 0 };
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
    NAPI_ASSERT_FATAL(errcode == 0, "napi_get_last_error_info");
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    errcode = napi_is_exception_pending(env, &is_exception_pending);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_is_exception_pending");
    // A pending exception takes precedence over any internal error status.
    if (is_exception_pending) {
        errcode = napi_get_and_clear_last_exception(env, &val);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_get_and_clear_last_exception");
        napi_throw(env, val);
        return errcode;
    }
    snprintf(errbuf, sizeof(errbuf),    //
        "%s\n    at %s (%s:%d)",        //
        (info->error_message != NULL    //
            ? info->error_message       //
            : "error in native code"),  //
        func, file, line);
    errcode = napi_throw_error(env, NULL, errbuf);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_throw_error");
    return errcode;
}


// file sqlmath_nodejs - promise
static void cbufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// This function will sqlite3_free memory from sqlite3_malloc.
    UNUSED_PARAMETER(env);
    UNUSED_PARAMETER(finalize_hint);
    sqlite3_free(finalize_data);
}

static napi_value jsbatonSetArraybuffer(
    napi_env env,
    napi_callback_info info
) {
// This function will set arraybuffer to <baton> at <bufi>.
    // declare var
    Jsbuffer *buf = NULL;
    int32_t bufi = 0;
    // init argv, baton
    NAPI_PARSE_ARGV(jsbatonSetArraybuffer, 3);
    // init bufi
    napi_get_value_int32(env, argv[1], &bufi);
    NAPI_ASSERT_OK();
    // set arraybuffer
    buf = &(baton->bufv[(size_t) bufi]);
    errcode =
        napi_get_arraybuffer_info(env, argv[2], (void **) &(buf->buf),
        (size_t *) & (buf->len));
    NAPI_ASSERT_OK();
    return argv[0];
}

static napi_value jsbatonStealCbuffer(
    napi_env env,
    napi_callback_info info
) {
// This function will reference-steal c-buffer from <baton> at <bufi>,
// and assume cleanup responsibility.
    napi_value result = NULL;
    // init argv, baton
    NAPI_PARSE_ARGV(jsbatonStealCbuffer, 3);
    // init bufi, modestr
    int32_t bufi = 0;
    int32_t modestr = 0;
    errcode = napi_get_value_int32(env, argv[1], &bufi);
    NAPI_ASSERT_OK();
    errcode = napi_get_value_int32(env, argv[2], &modestr);
    NAPI_ASSERT_OK();
    // init sqlite-buffer
    Jsbuffer *sqlite_buf = &(baton->bufv[(size_t) bufi]);
    // reference-steal sqlite-buffer to nodejs-string
    if (modestr) {
        errcode = napi_create_string_utf8(env,  // napi_env env
            (const char *) sqlite_buf->buf,     // const char* str
            (size_t) sqlite_buf->len,   // size_t length
            &result);           // napi_value* result
        sqlite3_free((void *) sqlite_buf->buf);
        sqlite_buf->buf = 0;
        sqlite_buf->len = 0;
        NAPI_ASSERT_OK();
        return result;
    }
    // reference-steal sqlite-buffer to nodejs-arraybuffer
    errcode = napi_create_external_arraybuffer( //
        env,                    // napi_env env
        (void *) sqlite_buf->buf,       // void* external_data
        (size_t) sqlite_buf->len,       // size_t byte_length
        cbufferFinalize,        // napi_finalize finalize_cb
        NULL,                   // void* finalize_hint
        &result);               // napi_value* result
    NAPI_ASSERT_OK();
    return result;
}

static void jspromiseExecute(
    napi_env env,
    void *data
) {
// This function will execute dbCall() in a thread.
    UNUSED_PARAMETER(env);
    dbCall((Jsbaton *) data);
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
    napi_ref ref = (void *) baton->napi_argv;
    napi_value val = NULL;
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
    if (jsbatonGetErrmsg(baton)[0] != '\x00') {
        // create error
        errcode =
            napi_create_string_utf8(env, jsbatonGetErrmsg(baton),
            NAPI_AUTO_LENGTH, &val);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_create_string_utf8");
        errcode = napi_create_error(env, NULL, val, &val);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_create_error");
        // reject promise with error
        errcode =
            napi_reject_deferred(env, (void *) baton->napi_deferred, val);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_reject_deferred");
    } else {
        errcode =
            napi_resolve_deferred(env, (void *) baton->napi_deferred,
            (void *) baton->napi_argv);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_resolve_deferred");
    }
    // Clean up the work item associated with this run.
    errcode = napi_delete_async_work(env, (void *) baton->napi_work);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_async_work");
    // Set both values to NULL so JavaScript can order a new run of the thread.
    baton->napi_argv = 0;
    baton->napi_deferred = 0;
    baton->napi_work = 0;
}

static napi_value jspromiseCreate(
    napi_env env,
    napi_callback_info info
) {
// Create a deferred promise and an async queue work item.
    //
    // Create baton for passing data between nodejs <-> c.
    // declare var
    napi_value promise = NULL;
    // init argv, baton
    NAPI_PARSE_ARGV(jspromiseCreate, 3);
    // init napi_argv
    baton->napi_argv = (int64_t) argv[1];
    //
    // Create nodejs-promise.
    // reference result to prevent gc
    errcode = napi_create_reference(env,        // napi_env env
        (void *) baton->napi_argv,      // napi_value value
        1,                      // uint32_t initial_refcount
        (napi_ref *) & baton->napi_argv);       // napi_ref* result
    NAPI_ASSERT_OK();
    // Ensure that no work is currently in progress.
    if (baton->napi_work) {
        napi_throw_error(env, NULL,
            "sqlmath._jspromiseCreate"
            " - Only one work item must exist at a time");
        return NULL;
    }
    // Create a deferred promise which we will resolve at the completion of the
    // work.
    errcode =
        napi_create_promise(env, (napi_deferred *) & (baton->napi_deferred),
        &promise);
    NAPI_ASSERT_OK();
    //
    // Execute dbCall() inside nodejs-promise.
    // Create an async work item, passing in the addon data, which will give the
    // worker thread access to the above-created deferred promise.
    errcode = napi_create_async_work(env,       // napi_env env,
        NULL,                   // napi_value async_resource,
        argv[2],                // napi_value async_resource_name,
        jspromiseExecute,       // napi_async_execute_callback execute,
        jspromiseResolve,       // napi_async_complete_callback complete,
        baton,                  // void* data,
        // napi_async_work* result);
        (napi_async_work *) & (baton->napi_work));
    NAPI_ASSERT_OK();
    // Queue the work item for execution.
    errcode = napi_queue_async_work(env, (void *) baton->napi_work);
    NAPI_ASSERT_OK();
    // This causes created `promise` to be returned to JavaScript.
    return promise;
}


// file sqlmath_nodejs - init
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
        NAPI_CREATE_FUNCTION(jsbatonSetArraybuffer),
        NAPI_CREATE_FUNCTION(jsbatonStealCbuffer),
        NAPI_CREATE_FUNCTION(jspromiseCreate),
    };
    errcode = napi_define_properties(env, exports,
        sizeof(propList) / sizeof(napi_property_descriptor), propList);
    NAPI_ASSERT_OK();
    // init sqlite
    errcode = sqlite3_initialize();
    NAPI_ASSERT_OK();
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_sqlmath_init);
#endif                          // SQLMATH_NODEJS_C3
/*
file sqlmath_nodejs - end
*/


/*
file sqlmath_python - start
*/
#if defined(SQLMATH_PYTHON_C2) && !defined(SQLMATH_PYTHON_C3)
#define SQLMATH_PYTHON_C3


#include <Python.h>


static PyObject *pybatonSetMemoryview(
    PyObject * self,
    PyObject * args
) {
// This function will set memoryview to <baton> at <bufi>.
    UNUSED_PARAMETER(self);
    // init baton, bufi
    int bufi = 0;
    PY_PARSE_ARGV(2, args, "y*ly*", &(pybuf[0]), &bufi, &(pybuf[1]));
    // set memoryview
    Jsbuffer *buf = &(baton->bufv[(size_t) bufi]);
    buf->buf = (int64_t) pybuf[1].buf;
    buf->len = pybuf[1].len;
    PyBuffer_Release(&(pybuf[1]));
    Py_RETURN_NONE;
}


// static PyObject *pybatonStealCbuffer - end
typedef struct {
    PyObject_HEAD char *buf;
    Py_ssize_t len;
} PycbufStruct;

static void Pycbuf_dealloc(
    PycbufStruct * self
) {
    sqlite3_free(self->buf);
    self->buf = NULL;
    self->len = 0;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int Pycbuf_getbuf(
    PycbufStruct * self,
    Py_buffer * view,
    int flags
) {
    UNUSED_PARAMETER(flags);
    view->obj = Py_XNewRef((PyObject *) self);
    view->buf = self->buf;
    view->len = self->len;
    view->readonly = 1;
    view->itemsize = 1;
    view->format = "B";
    view->ndim = 1;
    view->shape = NULL;
    view->strides = NULL;
    view->suboffsets = NULL;
    view->internal = NULL;
    return 0;
}

static PyBufferProcs Pycbuf_as_buffer = {
    .bf_getbuffer = (getbufferproc) Pycbuf_getbuf,
};

static PyTypeObject PycbufType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "PycbufType",
    .tp_basicsize = sizeof(PycbufStruct),
    //
    .tp_dealloc = (destructor) Pycbuf_dealloc,
    .tp_as_buffer = &Pycbuf_as_buffer,
    .tp_flags = Py_TPFLAGS_DEFAULT,
};

static PyObject *pybatonStealCbuffer(
    PyObject * self,
    PyObject * args
) {
// This function will reference-steal sqlite-buffer from <baton> at <bufi>,
// and assume cleanup responsibility.
    UNUSED_PARAMETER(self);
    // init baton, bufi, modestr
    int bufi = 0;
    int modestr = 0;
    PY_PARSE_ARGV(1, args, "y*ii", &(pybuf[0]), &bufi, &modestr);
    // init sqlite-buffer
    Jsbuffer *sqlite_buf = &(baton->bufv[(size_t) bufi]);
    // reference-steal sqlite-buffer to python-str
    if (modestr) {
        PyObject *python_str = PyUnicode_Decode(        //
            (const char *) sqlite_buf->buf,     //
            sqlite_buf->len,    //
            "utf-8",            //
            "surrogatepass");
        sqlite3_free((void *) sqlite_buf->buf);
        sqlite_buf->buf = 0;
        sqlite_buf->len = 0;
        return python_str;
    }
    // reference-steal sqlite-buffer to python-buffer
    PycbufStruct *python_buf =
        (PycbufStruct *) (&PycbufType)->tp_alloc(&PycbufType, 0);
    if (python_buf == NULL) {
        sqlite3_free((void *) sqlite_buf->buf);
        sqlite_buf->buf = 0;
        sqlite_buf->len = 0;
        return NULL;
    }
    python_buf->buf = (char *) sqlite_buf->buf;
    python_buf->len = sqlite_buf->len;
    return (PyObject *) python_buf;
}

// static PyObject *pybatonStealCbuffer - end


static PyObject *pydbCall(
    PyObject * self,
    PyObject * args
) {
    UNUSED_PARAMETER(self);
    //
    // Create baton for passing data between nodejs <-> c.
    // init baton
    PY_PARSE_ARGV(1, args, "y*", &(pybuf[0]));
    //
    // Execute dbCall().
    dbCall(baton);
    if (jsbatonGetErrmsg(baton)[0] != '\x00') {
        PyErr_SetString(PyExc_RuntimeError, jsbatonGetErrmsg(baton));
        return NULL;
    }
    Py_RETURN_NONE;
}


// file sqlmath_python - init
static PyMethodDef SqlmathMethods[] = {
    {"_pybatonSetMemoryview", pybatonSetMemoryview, METH_VARARGS, NULL},
    {"_pybatonStealCbuffer", pybatonStealCbuffer, METH_VARARGS, NULL},
    {"_pydbCall", pydbCall, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}       // sentinel
};

static struct PyModuleDef _sqlmathmodule = {
    PyModuleDef_HEAD_INIT,
    // name of module
    "_sqlmath",
    // module documentation, may be NULL
    NULL,
    // size of per-interpreter state of the module,
    // or -1 if the module keeps state in global variables.
    -1,
    SqlmathMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit__sqlmath(
    void
) {
    if (PyType_Ready(&PycbufType) < 0) {
        return NULL;
    }
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        PyErr_SetString(PyExc_ImportError, sqlite3_errstr(rc));
        return NULL;
    }
    return PyModule_Create(&_sqlmathmodule);
}
#endif                          // SQLMATH_PYTHON_C3
/*
file sqlmath_python - end
*/
