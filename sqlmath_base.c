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


#if !defined(SRC_SQLMATH_H2)
#define SRC_SQLMATH_H2


/*
file sqlmath_h - start
*/


#define NAPI_VERSION 6
#if defined(SRC_SQLITE_BASE_C2)
#   undef SRC_SQLITE_BASE_C2
#endif                          // SRC_SQLITE_BASE_C2
#if defined(_WIN32)
#   include <windows.h>
#else
#   include <dlfcn.h>
#   include <unistd.h>
#endif                          // _WIN32


#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlmath_external_sqlite.c"
#include "./sqlmath_base.h"


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
#endif                          // defined(__GNUC__)


#define JSBATON_ARGC            8
#define JSBATON_OFFSET_ALL      256
#define JSBATON_OFFSET_ARGV     128
#define JSBATON_OFFSET_BUFV     192
#define JSBATON_OFFSET_ERRMSG   48
#define JSBATON_OFFSET_FUNCNAME 8
#define JS_MAX_SAFE_INTEGER     0x1fffffffffffffLL
#define JS_MIN_SAFE_INTEGER     -0x1fffffffffffffLL
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
#define SQLITE_RESPONSETYPE_LASTVALUE   2


#define LGBM_EVAL_BUF_LEN_MAX 16
#define LGBM_EVAL_OUT_LEN_MAX 8
#define LGBM_MODE_TRAIN 1
#define LGBM_PARAM_BUF_LEN_MAX 1024
#define MATH_MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MATH_MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
#define MATH_PI 3.141592653589793238463
#define MATH_SIGN(aa) (((aa) < 0) ? -1 : ((aa) > 0) ? 1 : 0)
#define SQLITE_COLUMNTYPE_INTEGER_BIG   11
#define SQLITE_COLUMNTYPE_TEXT_BIG      13
#define SQLITE_ERROR_DATATYPE_INVALID   0x10003
#define SQLITE_ERROR_JSON_ARRAY_INVALID 0x71
#define SQLITE_ERROR_ZSQL_NULL          0x10004
#define SQLMATH_API
#define SQLMATH_FUNC
#define UNUSED_PARAMETER(x) ((void)(x))


#if !defined(SQLITE_MAX_FUNCTION_ARG)
#   define SQLITE_MAX_FUNCTION_ARG 127
#endif                          // SQLITE_MAX_FUNCTION_ARG


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

#define LGBM_ASSERT_OK() \
    if (errcode) { \
        sqlite3_result_error(context, LGBM_GetLastError(), -1); \
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

#define SQLITE_OK_OR_RETURN_RC(rc) \
    if (rc != SQLITE_OK) { \
        return rc; \
    }

#define SQL_CREATE_FUNC1(func, argc, flag) \
    errcode = sqlite3_create_function(db, #func, argc, \
        flag | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql1_##func##_func, NULL, NULL); \
    if (errcode) { \
        return errcode; \
    }

#define SQL_CREATE_FUNC2(func, argc, flag) \
    errcode = sqlite3_create_function(db, #func, argc, \
        flag | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        NULL, sql2_##func##_step, sql2_##func##_final); \
    if (errcode) { \
        return errcode; \
    }

#define SQL_CREATE_FUNC3(func, argc, flag) \
    errcode = sqlite3_create_window_function(db, #func, argc, \
        flag | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql3_##func##_step, sql3_##func##_final, \
        sql3_##func##_value, sql3_##func##_inverse, NULL); \
    if (errcode) { \
        return errcode; \
    }

#define STR99_ALLOCA(str99) \
    sqlite3_str __##str99 = { 0 }; \
    sqlite3_str *str99 = &__##str99; \
    str99->mxAlloc = SIZEOF_BLOB_MAX; \

#define STR99_RESULT_ERROR(str99) \
    errcode = sqlite3_str_errcode(str99); \
    if (errcode == SQLITE_ERROR_JSON_ARRAY_INVALID) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error(context, \
            "str99ArrayAppendJsonarray - invalid JSON array", -1); \
        goto catch_error; \
    } \
    if (errcode) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error_code(context, errcode); \
        goto catch_error; \
    } \
    if (sqlite3_str_length(str99) <= 0) { \
        sqlite3_str_reset(str99); \
        goto catch_error; \
    }

// file sqlmath_h - sqlite3


// *INDENT-OFF*
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
SQLMATH_API int dbDlopen(
    sqlite3_context * context,
    const char *filename,
    void **library
);
SQLMATH_API void dbExec(
    Jsbaton * baton
);
SQLMATH_API void dbFileLoad(
    Jsbaton * baton
);
SQLMATH_API int dbFileLoadOrSave(
    sqlite3 * pInMemory,
    const char *zFilename,
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
    double wnn;                 // window-mode
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
    const double xx
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


// file sqlmath_h - idate
#define SQLITE_FUNC_CONSTANT 0x0800 /* Constant inputs give a constant output */
#define SQLITE_FUNC_SLOCHNG  0x2000 // "Slow Change". Value constant during a
                                    // single query - might change over time
#define SQLITE_FUNC_IDATE \
    (SQLITE_FUNC_SLOCHNG | SQLITE_UTF8 | SQLITE_FUNC_CONSTANT)
#define SQL1_IDATE_FUNC(func, typeFrom, typeTo) \
    sql1_##func##_func( \
        sqlite3_context * context, \
        int argc, \
        sqlite3_value ** argv \
    ) { \
        sql1_idatefromto_func0(context, argc, argv, typeFrom, typeTo); \
    }
/*
** A structure for holding a single date and time.
*/
typedef struct DateTime {
    sqlite3_int64 iJD;  /* The julian day number times 86400000 */
    int Y, M, D;        /* Year, month, and day */
    int h, m;           /* Hour and minutes */
    int tz;             /* Timezone offset in minutes */
    double s;           /* Seconds */
    char validJD;       /* True (1) if iJD is valid */
    char validYMD;      /* True (1) if Y,M,D are valid */
    char validHMS;      /* True (1) if h,m,s are valid */
    char nFloor;            /* Days to implement "floor" */
    unsigned rawS      : 1; /* Raw numeric value stored in s */
    unsigned isError   : 1; /* An overflow has occurred */
    unsigned useSubsec : 1; /* Display subsecond precision */
    unsigned isUtc     : 1; /* Time is known to be UTC */
    unsigned isLocal   : 1; /* Time is known to be localtime */
} DateTime;
SQLITE_API int sqlite3_isDate2(
    sqlite3_context * context,
    const int argc,
    sqlite3_value ** argv,
    DateTime * dt,
    const int typeFrom
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

SQLMATH_API void sqlite3_result_error2(
    sqlite3_context * context,
    const char *template,
    ...                         // NOLINT
);

SQLMATH_API double sqlite3_value_double_or_nan(
    sqlite3_value * arg
);

SQLMATH_API double sqlite3_value_double_or_prev(
    sqlite3_value * arg,
    double *previous
);
/*
file sqlmath_h - end
*/


/*
file sqlmath_base - start
*/
#if defined(SRC_SQLMATH_BASE_C2)


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

SQLMATH_API int dbDlopen(
    sqlite3_context * context,
    const char *filename,
    void **library
) {
// This function will set <library> = dlopen(<filename>).
    static const intptr_t isBusy = 1;
    for (int ii = 0; *library == (void *) isBusy && ii < 100; ii += 1) {
#if defined(_WIN32)
        Sleep(100);
#else
        sleep(100);
#endif                          // _WIN32
    }
    if (*library == (void *) isBusy) {
        *library = (void *) 0;
        sqlite3_result_error2(context, "dbDlopen - dlopen(\"%s\") timeout",
            filename);
        return SQLITE_BUSY;
    }
    if (*library > (void *) isBusy) {
        return 0;
    }
    *library = (void *) isBusy;
#if defined(_WIN32)
    *library = (void *) LoadLibrary(filename);
    if (*library == NULL) {
        sqlite3_result_error2(context,
            "dbDlopen - LoadLibrary(\"%s\") failed with code=%lu", filename,
            GetLastError());
        return SQLITE_ERROR;
    }
#else
    *library = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
    if (*library == NULL) {
        // fprintf(stderr, "%s", dlerror());
        sqlite3_result_error2(context, "dbDlopen - dlopen(\"%s\") - %s",
            filename, dlerror());
        return SQLITE_ERROR;
    }
#endif                          // _WIN32
    return 0;
}

// SQLMATH_API dbExec - start
typedef struct DbExecBindElem {
    const char *buf;
    char *key;
    int buflen;
    char datatype;
} DbExecBindElem;

static inline void dbExecStr99AppendValue(
    sqlite3_str * str99,
    sqlite3_stmt * pStmt,
    int ii
) {
// This function will append json-value to <str99> from <pStmt> at column <ii>.
    double valDouble = 0;
    int64_t valInt64 = 0;
    switch (sqlite3_column_type(pStmt, ii)) {
    case SQLITE_INTEGER:
        valInt64 = sqlite3_column_int64(pStmt, ii);
        if (JS_MIN_SAFE_INTEGER <= valInt64
            // convert integer to double
            && valInt64 <= JS_MAX_SAFE_INTEGER) {
            sqlite3_str_append(str99,
                (char *) sqlite3_column_text(pStmt, ii),
                sqlite3_column_bytes(pStmt, ii));
        } else {
            // convert integer to string
            str99JsonAppendText(str99,
                (char *) sqlite3_column_text(pStmt, ii),
                sqlite3_column_bytes(pStmt, ii));
        }
        break;
    case SQLITE_FLOAT:
        valDouble = sqlite3_column_double(pStmt, ii);
        if (!isfinite(valDouble)) {
            sqlite3_str_append(str99, "null", 4);
        } else {
            sqlite3_str_append(str99,
                (char *) sqlite3_column_text(pStmt, ii),
                sqlite3_column_bytes(pStmt, ii));
        }
        break;
    case SQLITE_TEXT:
        // append text as json-escaped string
        str99JsonAppendText(str99,
            (char *) sqlite3_column_text(pStmt, ii),
            sqlite3_column_bytes(pStmt, ii));
        break;
        // case SQLITE_BLOB:
    default:                   /* case SQLITE_NULL: */
        sqlite3_str_append(str99, "null", 4);
        break;
    }
}

SQLMATH_API void dbExec(
    Jsbaton * baton
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized json-string in <str99>.
    // declare var
    DbExecBindElem *bindElem = NULL;
    DbExecBindElem *bindList = NULL;
    const char *zBind = (char *) baton + JSBATON_OFFSET_ALL;
    const char *zSql = jsbatonGetString(baton, 1);
    const char *zTmp = NULL;
    int bindByKey = (int) baton->argv[3];
    int bindIdx = 0;
    int bindListLength = (int) baton->argv[2];
    int errcode = 0;
    int ii = 0;
    int jj = 0;
    int nCol = 0;
    int responseType = (int) baton->argv[4];
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    static const char bindPrefix[] = "$:@";
    // str99 - init
    STR99_ALLOCA(str99);
    // fprintf(stderr, "\nsqlmath._dbExec(db=%p blen=%d sql=%s)\n",
    //     db, bindListLength, zSql);
#if !defined(__EMSCRIPTEN__)
    // mutex enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
#endif                          // __EMSCRIPTEN__
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
            bindElem->buflen = (int) baton->bufv[*(int32_t *) zBind].len;
            bindElem->buf = (char *) baton->bufv[*(int32_t *) zBind].buf;
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
    switch (responseType) {
    case SQLITE_RESPONSETYPE_LASTBLOB:
    case SQLITE_RESPONSETYPE_LASTVALUE:
        break;
    default:
        sqlite3_str_appendchar(str99, 1, '[');
    }
    // loop over each table - start
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        errcode = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zTmp);
        JSBATON_ASSERT_OK();
        if (errcode || *zSql == '\x00') {
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
                    (char *) sqlite3_column_blob(pStmt, nCol - 1),
                    sqlite3_column_bytes(pStmt, nCol - 1));
                break;
            case SQLITE_RESPONSETYPE_LASTVALUE:
                // export last-value as json-value
                if (nCol == -1) {
                    nCol = sqlite3_column_count(pStmt);
                }
                sqlite3_str_reset(str99);
                dbExecStr99AppendValue(str99, pStmt, nCol - 1);
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
                    dbExecStr99AppendValue(str99, pStmt, ii);
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
        case SQLITE_RESPONSETYPE_LASTVALUE:
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
    case SQLITE_RESPONSETYPE_LASTVALUE:
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
    if (errcode && str99->zText) {
        sqlite3_free(str99->zText);
    }
#if !defined(__EMSCRIPTEN__)
    // mutex leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
#endif                          // __EMSCRIPTEN__
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
        (int) jsbatonGetInt64(baton, 2));       // const int isSave
    JSBATON_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_API int dbFileLoadOrSave(
    sqlite3 * pInMemory,
    const char *zFilename,
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
    const double xx
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
        (int) (sizeof(Doublewin) / sizeof(double) + dblwin->nhead +
        dblwin->nbody);
    uint32_t alloc = (uint32_t) dblwin->alloc;
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
    sqlite3_result_blob(context, (char *) dblwin, (int) dblwin->alloc,
        // destructor
        sqlite3_free);
}

// SQLMATH_API doublewin - end

// SQLMATH_API idate - start
// SQLMATH_API idate - end

// SQLMATH_API str99 - start
SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double xx
) {
// This function will append double <xx> to <str99>.
    sqlite3_str_append(str99, (char *) &xx, 8);
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
    sqlite3_result_blob(context, (char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}

SQLMATH_API void str99ResultText(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// This function will return <str99> as result-text in given <context>.
    sqlite3_result_text(context, (char *) sqlite3_str_value(str99),
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

SQLMATH_API void sqlite3_result_error2(
    sqlite3_context * context,
    const char *format,
    ...                         // NOLINT
) {
// This function will return formatted error-message as sqlite-result.
    static char errbuf[256] = { 0 };
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(errbuf, sizeof(errbuf), format, argptr);
    sqlite3_result_error(context, errbuf, -1);
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
    sqlite3_result_text(context, (char *) sqlite3_value_text(argv[0]),
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
    sqlite3_result_blob(context, (char *) arr, argc * sizeof(double),
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
        const double xx = ((double *) sqlite3_value_blob(argv[0]))[ii];
        if (isfinite(xx)) {
            sqlite3_result_double(context, xx);
            return;
        }
    }
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
        (char *) sqlite3_value_blob(argv[0]),   // json
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
        (double *) sqlite3_value_blob(argv[0]),
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

SQLMATH_FUNC static void sql1_idatefromto_func0(
/*
**    datetime( TIMESTRING, MOD, MOD, ...)
**
** Return int64 YYYYMMDDHHMMSS
*/
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv,
    int typeFrom,
    int typeTo
) {
    // shift typeFrom from argv
    if (typeFrom == -1 || typeTo == -1) {
        if (argc < 1) {
            sqlite3_result_error2(context,      //
                "%s - number of arguments must be greater than 0",      //
                typeFrom == -1 ? "idatefrom" : "idateto",       //
                argc);
            return;
        }
        const char *zType = (char *) sqlite3_value_text(argv[0]);
        argc -= 1;
        argv += 1;
        if (typeFrom == -1) {
            if (sqlite3_stricmp(zType, "IDATE") == 0) {
                typeFrom = IDATE_TYPE_IDATE;
            } else if (sqlite3_stricmp(zType, "IEPOCH") == 0) {
                typeFrom = IDATE_TYPE_IEPOCH;
            } else if (sqlite3_stricmp(zType, "IJULIAN") == 0) {
                typeFrom = IDATE_TYPE_IJULIAN;
            } else if (sqlite3_stricmp(zType, "ITEXT") == 0) {
                typeFrom = IDATE_TYPE_ITEXT;
            }
        }
        if (typeTo == -1) {
            if (sqlite3_stricmp(zType, "IDATE") == 0) {
                typeTo = IDATE_TYPE_IDATE;
            } else if (sqlite3_stricmp(zType, "IEPOCH") == 0) {
                typeTo = IDATE_TYPE_IEPOCH;
            } else if (sqlite3_stricmp(zType, "IJULIAN") == 0) {
                typeTo = IDATE_TYPE_IJULIAN;
            } else if (sqlite3_stricmp(zType, "ITEXT") == 0) {
                typeTo = IDATE_TYPE_ITEXT;
            } else if (sqlite3_stricmp(zType, "ITEXTYMD") == 0) {
                typeTo = IDATE_TYPE_ITEXTYMD;
            } else if (sqlite3_stricmp(zType, "IY") == 0) {
                typeTo = IDATE_TYPE_IY;
            } else if (sqlite3_stricmp(zType, "IYM") == 0) {
                typeTo = IDATE_TYPE_IYM;
            } else if (sqlite3_stricmp(zType, "IYMD") == 0) {
                typeTo = IDATE_TYPE_IYMD;
            } else if (sqlite3_stricmp(zType, "IYMDH") == 0) {
                typeTo = IDATE_TYPE_IYMDH;
            } else if (sqlite3_stricmp(zType, "IYMDHM") == 0) {
                typeTo = IDATE_TYPE_IYMDHM;
            } else if (sqlite3_stricmp(zType, "IYMDHMS") == 0) {
                typeTo = IDATE_TYPE_IYMDHMS;
            }
        }
        if (typeFrom == -1 || typeTo == -1) {
            sqlite3_result_error2(context,      //
                "%s - invalid 1st argument %s='%s'",    //
                typeFrom == -1 ? "idatefrom" : "idateto",       //
                typeFrom == -1 ? "typeFrom" : "typeTo", //
                zType);
            return;
        }
    }
    // parse argv[0]
    DateTime __dt = { 0 };
    DateTime *dt = &__dt;
    int modeYmd = 0;
    if (typeFrom == IDATE_TYPE_IDATE) {
        const sqlite3_int64 idate64 = sqlite3_value_int64(argv[0]);
        modeYmd = 10000101 <= idate64 && idate64 <= 99991231;
        // parse yyyymmdd
        {
            const int yyyymmdd =
                (int) (modeYmd ? idate64 : idate64 / 1000000);
            dt->validYMD = 1;
            dt->Y = yyyymmdd / 10000;
            dt->M = (yyyymmdd / 100) % 100;
            dt->D = yyyymmdd % 100;
        }
        // parse hhmmss
        if (!modeYmd) {
            const int hhmmss = idate64 % 1000000;
            dt->validHMS = 1;
            dt->h = hhmmss / 10000;
            dt->m = (hhmmss / 100) % 100;
            dt->rawS = 0;
            dt->s = hhmmss % 100;
        }
        // validate dt
        if (!((1000 <= dt->Y && dt->Y <= 9999)
                && (1 <= dt->M && dt->M <= 12)
                && (1 <= dt->D && dt->D <= 31)
                && (0 <= dt->h && dt->h < 24)
                && (0 <= dt->m && dt->m < 60)
                && (0 <= dt->s && dt->s < 60))) {
            return;
        }
    }
    if (sqlite3_isDate2(context, argc, argv, dt, typeFrom)) {
        return;
    }
    // serialize result
    switch (typeTo) {
    case IDATE_TYPE_IEPOCH:
        sqlite3_result_int64(context,
            dt->iJD / 1000 - 21086676 * (int64_t) 10000);
        return;
    case IDATE_TYPE_IJULIAN:
        sqlite3_result_double(context, dt->iJD / 86400000.0);
        return;
    case IDATE_TYPE_ITEXT:
        {
            char zBuf[10 + 1 + 8 + 1] = { 0 };
            sqlite3_snprintf(sizeof(zBuf), zBuf,        //
                "%04d-%02d-%02d %02d:%02d:%02d",        //
                dt->Y, dt->M, dt->D, dt->h, dt->m, (int) dt->s);
            sqlite3_result_text(context, zBuf, sizeof(zBuf) - 1,
                SQLITE_TRANSIENT);
        }
        return;
    case IDATE_TYPE_ITEXTYMD:
        {
            char zBuf[10 + 1] = { 0 };
            sqlite3_snprintf(sizeof(zBuf), zBuf,        //
                "%04d-%02d-%02d",       //
                dt->Y, dt->M, dt->D);
            sqlite3_result_text(context, zBuf, sizeof(zBuf) - 1,
                SQLITE_TRANSIENT);
        }
        return;
    case IDATE_TYPE_IY:
        dt->M = 0;
        dt->D = 0;
        modeYmd = 1;
        break;
    case IDATE_TYPE_IYM:
        dt->D = 0;
        modeYmd = 1;
        break;
    case IDATE_TYPE_IYMD:
        modeYmd = 1;
        break;
    case IDATE_TYPE_IYMDH:
        dt->m = 0;
        dt->s = 0;
        modeYmd = 0;
        break;
    case IDATE_TYPE_IYMDHM:
        dt->s = 0;
        modeYmd = 0;
        break;
    case IDATE_TYPE_IYMDHMS:
        modeYmd = 0;
        break;
        // case IDATE_TYPE_IDATE:
    }
    if (modeYmd) {
        sqlite3_result_int(context, dt->Y * 10000 + dt->M * 100 + dt->D);
        return;
    }
    sqlite3_result_int64(context,       //
        (int64_t) (dt->Y * 10000 + dt->M * 100 + dt->D) * 1000000       //
        + (int64_t) (dt->h * 10000 + dt->m * 100 + (int) dt->s));
}

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idateadd,
    IDATE_TYPE_IDATE,
    IDATE_TYPE_IDATE
);

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idatefrom,
    IDATE_TYPE_IDEFAULT,
    IDATE_TYPE_IYMDHMS
);

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idatefromepoch,
    IDATE_TYPE_IEPOCH,
    IDATE_TYPE_IYMDHMS
);

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idateto,
    IDATE_TYPE_IDATE,
    -1
);

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idatetoepoch,
    IDATE_TYPE_IDATE,
    IDATE_TYPE_IEPOCH
);

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idateymdfrom,
    IDATE_TYPE_IDEFAULT,
    IDATE_TYPE_IYMD
);

SQLMATH_FUNC static void SQL1_IDATE_FUNC(
    idateymdfromepoch,
    IDATE_TYPE_IEPOCH,
    IDATE_TYPE_IYMD
);

// SQLMATH_FUNC sql1_lgbm_xxx_func - start
// https://lightgbm.readthedocs.io/en/latest/C-API.html
SQLMATH_FUNC static void sql1_lgbm_dlopen_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will init lgbm.
    UNUSED_PARAMETER(argc);
    const char *filename = (char *) sqlite3_value_text(argv[0]);
    int errcode = 0;
    errcode = dbDlopen(context, filename, (void **) &lgbm_library);
    if (errcode) {
        return;
    }
    LGBM_dlsym();
}

SQLMATH_FUNC static void sql1_lgbm_datasetcreatefromfile_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will load dataset <out> from file.
// (like LightGBM CLI version does).
    UNUSED_PARAMETER(argc);
    DatasetHandle out = NULL;
    int errcode = 0;
    errcode = LGBM_DatasetCreateFromFile(       //
        (char *) sqlite3_value_text(argv[0]),   // const char *filename,
        (char *) sqlite3_value_text(argv[1]),   // const char *param_data,
        // const DatasetHandle reference,
        (DatasetHandle) sqlite3_value_int64(argv[2]),   //
        &out);                  // DatasetHandle * out
    LGBM_ASSERT_OK();
    sqlite3_result_int64(context, (int64_t) out);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_datasetcreatefrommat_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will create dataset <out> from dense matrix <data>.
    const void *data = sqlite3_value_blob(argv[0]);
    if (data == NULL) {
        sqlite3_result_error(context,
            "lgbm_datasetcreatefrommat - data cannot be NULL", -1);
        return;
    }
    UNUSED_PARAMETER(argc);
    DatasetHandle out = NULL;
    int errcode = 0;
    errcode = LGBM_DatasetCreateFromMat(        //
        data,                   // const void *data,
        sqlite3_value_int(argv[1]),     // int data_type,
        (int32_t) sqlite3_value_int(argv[2]),   // int32_t nrow,
        (int32_t) sqlite3_value_int(argv[3]),   // int32_t ncol,
        sqlite3_value_int(argv[4]),     // int is_row_major,
        (char *) sqlite3_value_text(argv[5]),   // const char *param_data,
        // const DatasetHandle reference,
        (DatasetHandle) sqlite3_value_int64(argv[6]),   //
        &out);                  // DatasetHandle *out
    LGBM_ASSERT_OK();
    sqlite3_result_int64(context, (int64_t) out);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_datasetdumptext_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will save dataset <handle> to text file <filename>,
// intended for debugging use only.
    UNUSED_PARAMETER(argc);
    int errcode = 0;
    errcode = LGBM_DatasetDumpText(     //
        (DatasetHandle) sqlite3_value_int64(argv[0]),
        (char *) sqlite3_value_text(argv[1]));
    LGBM_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_datasetfree_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will free space for dataset <handle>.
    UNUSED_PARAMETER(argc);
    int errcode = 0;
    errcode = LGBM_DatasetFree( //
        (DatasetHandle) sqlite3_value_int64(argv[0]));
    LGBM_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_datasetgetnumdata_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will get number of datapoints from dataset <handle>.
    UNUSED_PARAMETER(argc);
    int errcode = 0;
    int result = 0;
    errcode = LGBM_DatasetGetNumData(   //
        (DatasetHandle) sqlite3_value_int64(argv[0]), &result);
    LGBM_ASSERT_OK();
    sqlite3_result_int(context, result);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_datasetgetnumfeature_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will get number of features from dataset <handle>.
    UNUSED_PARAMETER(argc);
    int errcode = 0;
    int result = 0;
    errcode = LGBM_DatasetGetNumFeature(        //
        (DatasetHandle) sqlite3_value_int64(argv[0]), &result);
    LGBM_ASSERT_OK();
    sqlite3_result_int(context, result);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_datasetsavebinary_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will save dataset <handle> to binary file <filename>.
    UNUSED_PARAMETER(argc);
    int errcode = 0;
    errcode = LGBM_DatasetSaveBinary(   //
        (DatasetHandle) sqlite3_value_int64(argv[0]),
        (char *) sqlite3_value_text(argv[1]));
    LGBM_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql1_lgbm_extract_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will extract <val> from lgbm-pred-object, with given <key>.
    UNUSED_PARAMETER(argc);
    const int nn = sqlite3_value_bytes(argv[0]) / sizeof(double);
    // validate argv
    if (nn == 0) {
        sqlite3_result_error(context,
            "lgbm_extract - 1st argument must be double-array with length >= 0",
            -1);
        return;
    }
    const double *arr = (double *) sqlite3_value_blob(argv[0]);
    const char *key = (char *) sqlite3_value_text(argv[1]);
    int argmax = 0;
    double probability = arr[0];
    for (int ii = 1; ii < nn; ii += 1) {
        if (arr[ii] > probability) {
            argmax = ii;
            probability = arr[ii];
        }
    }
    // predicted-class using argmax
    if (strcmp(key, "argmax") == 0) {
        sqlite3_result_int(context, argmax);
        return;
    }
    // predicted-probability
    if (strcmp(key, "probability") == 0) {
        sqlite3_result_double_or_null(context, probability);
        return;
    }
    sqlite3_result_error(context,
        "lgbm_extract - 2nd argument is invalid key", -1);
}

SQLMATH_FUNC static void sql1_lgbm_predictforfile_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will make prediction for file from <model_str>.
    const char *model_str = (char *) sqlite3_value_text(argv[0]);
    if (model_str == NULL) {
        sqlite3_result_error(context,
            "lgbm_predictforfile - model_str cannot be NULL", -1);
        return;
    }
    UNUSED_PARAMETER(argc);
    int errcode = 0;
    // booster - init
    BoosterHandle booster = NULL;
    int out_num_iteration = 0;
    errcode = LGBM_BoosterLoadModelFromString(  //
        model_str,              // const char *model_str,
        &out_num_iteration,     // int *out_num_iteration,
        &booster);              // BoosterHandle *out
    LGBM_ASSERT_OK();
    // booster - predict
    errcode = LGBM_BoosterPredictForFile(       //
        booster,                // BoosterHandle handle,
        // argv - data_filename
        (char *) sqlite3_value_text(argv[5]),   // const char *data_filename,
        sqlite3_value_int(argv[6]),     // int data_has_header,
        // argv - predict
        sqlite3_value_int(argv[1]),     // int predict_type,
        sqlite3_value_int(argv[2]),     // int start_iteration,
        sqlite3_value_int(argv[3]),     // int num_iteration,
        (char *) sqlite3_value_text(argv[4]),   // const char *param_pred,
        // argv - result_filename
        (char *) sqlite3_value_text(argv[7]));  // const char *result_filename
    LGBM_ASSERT_OK();
  catch_error:
    LGBM_BoosterFree(booster);
}

SQLMATH_FUNC static void sql1_lgbm_trainfromdataset_func0(
    sqlite3_context * context,
    // argv - train
    const char *param_train,
    const int num_iteration,
    const int eval_step,
    // argv - data
    const DatasetHandle train_data,
    const DatasetHandle test_data
) {
// This function will create-and-train <model_str> from training data.
    if (train_data == NULL) {
        sqlite3_result_error(context,
            "lgbm_trainfromdataset - train_data cannot be NULL", -1);
        return;
    }
    // booster - init
    BoosterHandle booster = NULL;
    char *model_str = NULL;
    int errcode = 0;
    errcode = LGBM_BoosterCreate(       //
        train_data,             // const DatasetHandle train_data,
        param_train,            // const char *param_train,
        &booster);              // BoosterHandle *out
    LGBM_ASSERT_OK();
    if (test_data != NULL) {
        LGBM_BoosterAddValidData(booster, test_data);
        LGBM_ASSERT_OK();
    }
    // booster - train
    char *eval_names[LGBM_EVAL_OUT_LEN_MAX] = { 0 };
    char eval_buf[LGBM_EVAL_OUT_LEN_MAX * LGBM_EVAL_BUF_LEN_MAX] = { 0 };
    double eval_results[LGBM_EVAL_OUT_LEN_MAX] = { 0 };
    int eval_len = 0;
    int is_finished = 0;
    size_t eval_name_len = 0;
    for (int ii = 0; ii < LGBM_EVAL_OUT_LEN_MAX; ii += 1) {
        eval_names[ii] = &eval_buf[ii * LGBM_EVAL_BUF_LEN_MAX];
    }
    errcode = LGBM_BoosterGetEvalCounts(booster, &eval_len);
    LGBM_ASSERT_OK();
    if (eval_len > LGBM_EVAL_OUT_LEN_MAX) {
        sqlite3_result_error2(context,
            "lgbm_trainfromdataset - number-of-metrics must be <= %d",
            LGBM_EVAL_OUT_LEN_MAX);
        goto catch_error;
    }
    errcode = LGBM_BoosterGetEvalNames( //
        booster,                // BoosterHandle handle,
        LGBM_EVAL_OUT_LEN_MAX,  // const int len,
        &eval_len,              // int *out_len,
        LGBM_EVAL_BUF_LEN_MAX,  // const size_t buffer_len,
        &eval_name_len,         // size_t *out_buffer_len,
        (char **) eval_names);  // char **out_strs
    LGBM_ASSERT_OK();
    for (int ii = 0; ii < num_iteration; ii += 1) {
        errcode = LGBM_BoosterUpdateOneIter(booster, &is_finished);
        LGBM_ASSERT_OK();
        errcode = LGBM_BoosterGetEval(  //
            booster,            // BoosterHandle handle,
            0,                  // int data_idx,
            &eval_len,          // int *out_len,
            eval_results);      // double *out_results
        LGBM_ASSERT_OK();
        if (eval_step > 0 && (ii + 1) % eval_step == 0) {
            fprintf(stderr, "    lgbm_trainfromdataset - round %d -", ii);
            for (int jj = 0; jj < eval_len; jj += 1) {
                fprintf(stderr, " %s=%.6f", eval_names[jj], eval_results[jj]);
            }
            fprintf(stderr, "\n");
        }
    }
    // booster - save
    int64_t model_len = 0;
    errcode = LGBM_BoosterSaveModelToString(    //
        booster,                // BoosterHandle handle,
        0,                      // int start_iteration,
        -1,                     // int num_iteration,
        C_API_FEATURE_IMPORTANCE_SPLIT, // int feature_importance_type,
        // C_API_FEATURE_IMPORTANCE_GAIN,  // int feature_importance_type,
        0,                      // int64_t buffer_len,
        &model_len,             // int64_t *out_len,
        model_str);             // char *out_str
    LGBM_ASSERT_OK();
    model_str = sqlite3_malloc((int) model_len);
    if (model_str == NULL) {
        sqlite3_result_error_nomem(context);
        goto catch_error;
    }
    errcode = LGBM_BoosterSaveModelToString(    //
        booster,                // BoosterHandle handle,
        0,                      // int start_iteration,
        -1,                     // int num_iteration,
        C_API_FEATURE_IMPORTANCE_SPLIT, // int feature_importance_type,
        // C_API_FEATURE_IMPORTANCE_GAIN,  // int feature_importance_type,
        model_len,              // int64_t buffer_len,
        &model_len,             // int64_t *out_len,
        model_str);             // char *out_str
    LGBM_ASSERT_OK();
    sqlite3_result_blob(context, model_str, (int) model_len, sqlite3_free);
  catch_error:
    LGBM_BoosterFree(booster);
    if (errcode) {
        sqlite3_free(model_str);
    }
}

SQLMATH_FUNC static void sql1_lgbm_trainfromdataset_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will create-and-train <model_str> from training data.
    UNUSED_PARAMETER(argc);
    sql1_lgbm_trainfromdataset_func0(context,   //
        // argv - train
        (char *) sqlite3_value_text(argv[0]),   // param_train
        sqlite3_value_int(argv[1]),     // num_iteration
        sqlite3_value_int(argv[2]),     // eval_step
        // argv - data
        (DatasetHandle) sqlite3_value_int64(argv[3]),   // train_data
        (DatasetHandle) sqlite3_value_int64(argv[4]));  // test_data
}

SQLMATH_FUNC static void sql1_lgbm_trainfromfile_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will create-and-train <model_str> from training data.
    UNUSED_PARAMETER(argc);
    DatasetHandle test_data = NULL;
    int errcode = 0;
    // train_data - init
    DatasetHandle train_data = NULL;
    const char *file_train = (char *) sqlite3_value_text(argv[3]);
    const char *param_data = (char *) sqlite3_value_text(argv[4]);
    errcode = LGBM_DatasetCreateFromFile(       //
        file_train,             // const char *filename,
        param_data,             // const char *param_data,
        NULL,                   // const DatasetHandle reference,
        &train_data);           // DatasetHandle * out
    LGBM_ASSERT_OK();
    // train_test - init
    const char *file_test = (char *) sqlite3_value_text(argv[5]);
    if (file_test != NULL) {
        errcode = LGBM_DatasetCreateFromFile(   //
            file_test,          // const char *filename,
            param_data,         // const char *param_data,
            train_data,         // const DatasetHandle reference,
            &test_data);        // DatasetHandle * out
        LGBM_ASSERT_OK();
    }
    // train_data - train
    sql1_lgbm_trainfromdataset_func0(context,   //
        // argv - train
        (char *) sqlite3_value_text(argv[0]),   // param_train
        sqlite3_value_int(argv[1]),     // num_iteration
        sqlite3_value_int(argv[2]),     // eval_step
        // argv - data
        train_data,             // train_data
        test_data);             // test_data
  catch_error:
    LGBM_DatasetFree(test_data);
    LGBM_DatasetFree(train_data);
}

// SQLMATH_FUNC sql1_lgbm_xxx_func - end

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
    uint64_t ii = 0;
    sqlite3_randomness(sizeof(ii), &ii);
    sqlite3_result_double(context,
        ((double) ii) / ((double) UINT64_MAX) * (1.0 - DBL_EPSILON));
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

// SQLMATH_FUNC sql1_sha256_func - start
static void sql1_sha256_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** The sha256(X) function computes the SHA3 hash of the input X, or NULL if
** X is NULL.  If inputs X is text, the UTF-8 rendering of that text is
** used to compute the hash.  If X is a BLOB, then the binary data of the
** blob is used to compute the hash.  If X is an integer or real number,
** then that number if converted into UTF-8 text and the hash is computed
** over the text.
*/
    UNUSED_PARAMETER(argc);
    int eType = sqlite3_value_type(argv[0]);
    size_t nByte = sqlite3_value_bytes(argv[0]);
    if (eType == SQLITE_NULL) {
        return;
    }
    const unsigned char *message;
    if (eType == SQLITE_BLOB) {
        message = sqlite3_value_blob(argv[0]);
    } else {
        message = sqlite3_value_text(argv[0]);
    }
    unsigned char hash[32] = {
        0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
        0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
        0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
        0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
    };
    if (nByte == 0) {
        sqlite3_result_blob(context, hash, sizeof(hash), SQLITE_TRANSIENT);
        return;
    }
    struct tc_sha256_state_struct sha_state;
    if (tc_sha256_init(&sha_state) != TC_CRYPTO_SUCCESS) {
        goto catch_error;
    }
    if (tc_sha256_update(&sha_state, message, nByte) != TC_CRYPTO_SUCCESS) {
        goto catch_error;
    }
    if (tc_sha256_final(hash, &sha_state) != TC_CRYPTO_SUCCESS) {
        goto catch_error;
    }
    sqlite3_result_blob(context, hash, sizeof(hash), SQLITE_TRANSIENT);
    return;
  catch_error:
    sqlite3_result_error(context, "sha256 - could not hash", -1);
}

// SQLMATH_FUNC sql1_sha256_func - end

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

SQLMATH_FUNC static void sql1_strtoll_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return strtoll(str, base).
    UNUSED_PARAMETER(argc);
    const char *str = (char *) sqlite3_value_text(argv[0]);
    const int base = sqlite3_value_int(argv[1]);
    char *endptr = NULL;
    int64_t value = strtoll(str, &endptr, base);
    if (endptr == str) {
        return;
    }
    sqlite3_result_int64(context, (int64_t) value);
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

// SQLMATH_FUNC sql1_zlib_xxx_func - start
typedef unsigned long z_ulong;  // NOLINT

SQLMATH_FUNC static void sql1_zlib_compress_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will compress <argv[0]> using zlib's compress() function,
// with a 4-byte header storing <original_size>.
    UNUSED_PARAMETER(argc);
    // init original_data
    const void *original_data = sqlite3_value_blob(argv[0]);
    if (original_data == NULL) {
        sqlite3_result_error(context, "zlib_compress - cannot compress NULL",
            -1);
        return;
    }
    // init original_size
    int original_size = sqlite3_value_bytes(argv[0]);
    // init compress_size
    uLongf compress_size = compressBound(original_size);
    // init compress_data
    unsigned char *compress_data =
        (unsigned char *) sqlite3_malloc(4 + compress_size);
    if (compress_data == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    // zlib_compress
    if (compress(compress_data + 4, &compress_size, original_data,
            original_size) != Z_OK) {
        sqlite3_free(compress_data);
        sqlite3_result_error(context, "zlib_compress - failed compress", -1);
        return;
    }
    // set 4-byte header storing <original_size> (big-endian)
    compress_data[0] = (original_size >> 0x18) & 0xff;
    compress_data[1] = (original_size >> 0x10) & 0xff;
    compress_data[2] = (original_size >> 0x08) & 0xff;
    compress_data[3] = (original_size >> 0x00) & 0xff;
    sqlite3_result_blob(context, compress_data, 4 + compress_size,
        sqlite3_free);
}

SQLMATH_FUNC static void sql1_zlib_uncompress_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will compress <argv[0]> using zlib's compress() function,
// with a 4-byte header storing <original_size>.
    UNUSED_PARAMETER(argc);
    // init compress_data
    const unsigned char *compress_data = sqlite3_value_blob(argv[0]);
    if (compress_data == NULL) {
        sqlite3_result_error(context,
            "zlib_uncompress - cannot uncompress NULL", -1);
        return;
    }
    // init compress_size
    int compress_size = sqlite3_value_bytes(argv[0]) - 4;
    if (compress_size < 0) {
        sqlite3_result_error(context, "zlib_uncompress - invalid header", -1);
        return;
    }
    // init original_size
    uLongf original_size = 0    //
        | (compress_data[0] << 0x18)    //
        | (compress_data[1] << 0x10)    //
        | (compress_data[2] << 0x08)    //
        | (compress_data[3] << 0x00);
    if (original_size <= 0 || original_size > SIZEOF_BLOB_MAX) {
        sqlite3_result_error(context,
            "zlib_uncompress - invalid original_size", -1);
        return;
    }
    // init original_data
    unsigned char *original_data =
        (unsigned char *) sqlite3_malloc(original_size);
    if (original_data == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    // zlib_uncompress
    if (uncompress(original_data, &original_size, compress_data + 4,
            compress_size) != Z_OK) {
        sqlite3_free(original_data);
        sqlite3_result_error(context, "zlib_uncompress - failed uncompress",
            -1);
        return;
    }
    sqlite3_result_blob(context, original_data, original_size, sqlite3_free);
}

// SQLMATH_FUNC sql1_zlib_xxx_func - end

// SQLMATH_FUNC sql2_columntype_func - start
typedef struct AggColumntype {
    int columntype;
} AggColumntype;

SQLMATH_FUNC static void sql2_columntype_final(
    sqlite3_context * context
) {
// This function will return columntype for given <column>.
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggColumntype);
    // agg - null-case
    if (agg->columntype == 0) {
        agg->columntype = SQLITE_INTEGER;
    }
    sqlite3_result_int(context, agg->columntype);
}

SQLMATH_FUNC static void sql2_columntype_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return columntype for given <column>.
    UNUSED_PARAMETER(argc);
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggColumntype);
    // agg - null-case
    if (agg->columntype == 0) {
        agg->columntype = SQLITE_INTEGER;
    }
    switch (sqlite3_value_type(argv[0])) {
    case SQLITE_BLOB:
        agg->columntype = SQLITE_BLOB;
        break;
    case SQLITE_FLOAT:
        if (agg->columntype == SQLITE_INTEGER
            || agg->columntype == SQLITE_COLUMNTYPE_INTEGER_BIG) {
            agg->columntype = SQLITE_FLOAT;
        }
        break;
    case SQLITE_INTEGER:
        {
            const int64_t val = sqlite3_value_int64(argv[0]);
            if (!(-0x80000000LL <= val && val <= 0x7fffffffLL)) {
                agg->columntype = SQLITE_COLUMNTYPE_INTEGER_BIG;
            }
        }
        break;
        // case SQLITE_NULL:
    case SQLITE_TEXT:
        if (agg->columntype == SQLITE_INTEGER
            || agg->columntype == SQLITE_COLUMNTYPE_INTEGER_BIG
            || agg->columntype == SQLITE_FLOAT) {
            agg->columntype = SQLITE_TEXT;
        }
        if (agg->columntype == SQLITE_TEXT
            && sqlite3_value_bytes(argv[0]) > 255) {
            agg->columntype = SQLITE_COLUMNTYPE_TEXT_BIG;
        }
        break;
    }
}

// SQLMATH_FUNC sql2_columntype_func - end

// SQLMATH_FUNC sql2_lgbm_datasetcreatefromtable_func - start
typedef struct AggLgbmDataset {
    char param_data[LGBM_PARAM_BUF_LEN_MAX];
    DatasetHandle ref;
    //
    char param_train[LGBM_PARAM_BUF_LEN_MAX];
    int num_iteration;
    int eval_step;
} AggLgbmDataset;

SQLMATH_FUNC static void sql2_lgbm_datasetcreatefromtable_final0(
    sqlite3_context * context,
    const int modeTrain
) {
// This function will aggregate sql-table into lgbm-dataset.
    float *label = NULL;
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - null-case
    if (dblwin->nbody == 0) {
        goto catch_error;
    }
    AggLgbmDataset *agg = (AggLgbmDataset *) dblwin_head;
    // label - init
    DatasetHandle out = NULL;
    const int ncol = (int) dblwin->ncol;
    const int nrow = (int) (dblwin->nbody / dblwin->ncol);
    int errcode = 0;
    label = sqlite3_malloc(nrow * sizeof(float));
    if (label == NULL) {
        sqlite3_result_error_nomem(context);
        goto catch_error;
    }
    int kk = 0;
    int ll = 0;
    for (int ii = 0; ii < nrow; ii += 1) {
        label[ii] = (float) dblwin_body[kk];
        kk += 1;
        for (int jj = 1; jj < ncol; jj += 1) {
            dblwin_body[ll] = dblwin_body[kk];
            kk += 1;
            ll += 1;
        }
    }
    // dataset - init
    errcode = LGBM_DatasetCreateFromMat(        //
        dblwin_body,            // const void *data,
        C_API_DTYPE_FLOAT64,    // int data_type,
        (int32_t) nrow,         // int32_t nrow,
        (int32_t) ncol - 1,     // int32_t ncol,
        1,                      // int is_row_major,
        agg->param_data,        // const char *param_data,
        agg->ref,               // const DatasetHandle reference,
        &out);                  // DatasetHandle *out
    LGBM_ASSERT_OK();
    errcode = LGBM_DatasetSetField(     //
        out,                    // DatasetHandle handle,
        "label\x00",            // const char *field_name,
        label,                  // const void *field_data,
        nrow,                   // int num_element,
        C_API_DTYPE_FLOAT32);   // int type
    LGBM_ASSERT_OK();
    // agg - train
    if (modeTrain) {
        sql1_lgbm_trainfromdataset_func0(context,       //
            // argv - train
            agg->param_train,   // param_train
            agg->num_iteration, // num_iteration
            agg->eval_step,     // eval_step
            // argv - data
            out,                // train_data
            NULL);              // test_data
        LGBM_DatasetFree(out);
        goto catch_error;
    }
    sqlite3_result_int64(context, (int64_t) out);
  catch_error:
    doublewinAggfree(dblwinAgg);
    sqlite3_free(label);
}

SQLMATH_FUNC static void sql2_lgbm_datasetcreatefromtable_final(
    sqlite3_context * context
) {
// This function will aggregate sql-table into lgbm-dataset.
    sql2_lgbm_datasetcreatefromtable_final0(context, 0);
}

SQLMATH_FUNC static void sql2_lgbm_datasetcreatefromtable_step0(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv,
    const int modeTrain
) {
// This function will aggregate sql-table into lgbm-dataset.
    const int argc_train = modeTrain ? 3 : 0;
    const int argc0 = argc_train + 2;
    const int ncol = argc - argc0;
    if (ncol < 1) {
        sqlite3_result_error(context,
            "lgbm_datasetcreatefromtable - wrong number of arguments", -1);
        return;
    }
    DOUBLEWIN_AGGREGATE_CONTEXT(sizeof(AggLgbmDataset) / sizeof(double) + 1);
    if (dblwin->nbody == 0) {
        // dblwin - init ncol
        dblwin->ncol = ncol;
        // agg - init
        AggLgbmDataset *agg = (AggLgbmDataset *) dblwin_head;
        int param_bytes = sqlite3_value_bytes(argv[argc_train + 0]);
        if (param_bytes + 1 >= LGBM_PARAM_BUF_LEN_MAX) {
            sqlite3_result_error2(context,
                "lgbm_datasetcreatefromtable"
                " - param_data must be < %d bytes",
                LGBM_PARAM_BUF_LEN_MAX - 1);
            return;
        }
        if (param_bytes > 0) {
            memcpy(agg->param_data, sqlite3_value_text(argv[argc_train + 0]),
                param_bytes);
            agg->param_data[param_bytes] = 0;
        }
        agg->ref = (DatasetHandle) sqlite3_value_int64(argv[argc_train + 1]);
        if (modeTrain) {
            param_bytes = sqlite3_value_bytes(argv[0]);
            if (param_bytes + 1 >= LGBM_PARAM_BUF_LEN_MAX) {
                sqlite3_result_error2(context,
                    "lgbm_datasetcreatefromtable"
                    " - param_train must be < %d bytes",
                    LGBM_PARAM_BUF_LEN_MAX - 1);
                return;
            }
            if (param_bytes > 0) {
                memcpy(agg->param_train, sqlite3_value_text(argv[0]),
                    param_bytes);
                agg->param_train[param_bytes] = 0;
            }
            agg->num_iteration = sqlite3_value_int(argv[1]);
            agg->eval_step = sqlite3_value_int(argv[2]);
        }
    }
    // dblwin - push xx
    for (int ii = 0; ii < ncol; ii += 1) {
        DOUBLEWIN_AGGREGATE_PUSH(sqlite3_value_double_or_nan(argv[argc0 +
                    ii]));
    }
}

SQLMATH_FUNC static void sql2_lgbm_datasetcreatefromtable_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate sql-table into lgbm-dataset.
    sql2_lgbm_datasetcreatefromtable_step0(context, argc, argv, 0);
}

// SQLMATH_FUNC sql2_lgbm_datasetcreatefromtable_func - end

// SQLMATH_FUNC sql2_lgbm_trainfromtable_func - start
SQLMATH_FUNC static void sql2_lgbm_trainfromtable_final(
    sqlite3_context * context
) {
// This function will create-and-train <model_str> from sql-table.
    sql2_lgbm_datasetcreatefromtable_final0(context, LGBM_MODE_TRAIN);
}

static void sql2_lgbm_trainfromtable_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will create-and-train <model_str> from sql-table.
    sql2_lgbm_datasetcreatefromtable_step0(context, argc, argv,
        LGBM_MODE_TRAIN);
}

// SQLMATH_FUNC sql2_lgbm_trainfromtable_func - end

// SQLMATH_FUNC sql2_quantile_func - start
static double quickselect(
    double *arr,
    const int nn,
    const int kk
) {
// This function will find <qq>-th-quantile element in <arr>,
// using median-of-three quickselect-algorithm.
// derived from https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
#define QUICKSELECT_SWAP(aa, bb) \
    do { \
        double tmp = (aa); \
        (aa) = (bb); \
        (bb) = tmp; \
    } while (0)
    if (nn <= 0 || kk < 0 || kk >= nn) {
        return NAN;             // Invalid input
    }
    int low = 0;
    int high = nn - 1;
    while (low <= high) {
        if (low == high) {
            return arr[low];
        }
        int mid = low + (high - low) / 2;
        // Median-of-Three Pivot Selection
        if (arr[low] > arr[mid]) {
            QUICKSELECT_SWAP(arr[low], arr[mid]);
        }
        if (arr[mid] > arr[high]) {
            QUICKSELECT_SWAP(arr[mid], arr[high]);
        }
        if (arr[low] > arr[mid]) {
            QUICKSELECT_SWAP(arr[low], arr[mid]);
        }
        // Move pivot to end
        QUICKSELECT_SWAP(arr[mid], arr[high]);
        double pivot_val = arr[high];
        int ii = low;
        int jj = high - 1;
        // Hoare’s Partitioning
        while (1) {
            while (ii < high && arr[ii] < pivot_val) {
                ii++;
            }
            while (jj > low && arr[jj] > pivot_val) {
                jj--;
            }
            if (ii >= jj) {
                break;
            }
            QUICKSELECT_SWAP(arr[ii], arr[jj]);
            ii++;
            jj--;
        }
        // Move pivot to its correct position
        QUICKSELECT_SWAP(arr[ii], arr[high]);
        // Quickselect logic
        if (ii == kk) {
            return arr[ii];
        } else if (ii > kk) {
            high = ii - 1;
        } else {
            low = ii + 1;
        }
    }
    return NAN;                 // Should not reach here
}

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double qq
) {
// This function will find <qq>-th-quantile element in <arr>,
// using median-of-three quickselect-algorithm.
// derived from https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn < 1 || isnan(qq)) {
        // if (nn < 1 || isnan(qq) || qq < 0 || qq > 1) {
        return NAN;             // Invalid input
    }
    if (qq == 0) {
        return quickselect(arr, nn, 0);
    }
    if (qq == 1) {
        return quickselect(arr, nn, nn - 1);
    }
    double kmod = fmax(0, fmin(1, qq)) * (nn - 1);
    int kk = (int) kmod;
    kmod -= kk;
    double val1 = quickselect(arr, nn, kk);
    double val2 = (kk + 1 < nn) ? quickselect(arr, nn, kk + 1) : val1;
    return (kmod == 0) ? val1 : (1 - kmod) * val1 + kmod * val2;
}

SQLMATH_FUNC static void sql2_quantile_final(
    sqlite3_context * context
) {
// This function will aggregate kth-quantile element.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - null-case
    if (dblwin->nbody == 0) {
        goto catch_error;
    }
    sqlite3_result_double(context, quantile(dblwin_body, (int) dblwin->nbody,
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

SQLMATH_FUNC static void sql2_median2_final(
    sqlite3_context * context
) {
    sql2_quantile_final(context);
}

SQLMATH_FUNC static void sql2_median2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will aggregate kth-quantile element.
    sql2_quantile_step0(context, argc, argv, 0.5);
}

// SQLMATH_FUNC sql2_quantile_func - end

// SQLMATH_FUNC sql3_win_avg1_func - start
SQLMATH_FUNC static void sql3_win_avg1_value(
    sqlite3_context * context
) {
// This function will calculate running-avg.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    sqlite3_result_double(context,
        dblwin_head[(int) dblwin->ncol] / dblwin->nbody);
}

SQLMATH_FUNC static void sql3_win_avg1_final(
    sqlite3_context * context
) {
// This function will calculate running-avg.
    // dblwin - value
    sql3_win_avg1_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_avg1_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-avg.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->wnn) {
        dblwin->wnn = 1;
    }
}

SQLMATH_FUNC static void sql3_win_avg1_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-avg.
    if (argc < 1) {
        sqlite3_result_error(context,
            "win_avg2 - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = argc;
    DOUBLEWIN_AGGREGATE_CONTEXT(3 * ncol);
    if (dblwin->nbody == 0) {
        // dblwin - init ncol
        dblwin->ncol = ncol;
    }
    // dblwin - calculate avg
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[ii], &dblwin_head[ii]);
        dblwin_head[ncol + ii] += dblwin_head[ii];
        // fprintf(stderr, "sqlmath.win_avg2 - stp %d - xx=%f avg=%f\n", ii,
        //     dblwin_head[ii], dblwin_head[ncol + ii]);
        if (dblwin->wnn) {
            dblwin_head[ncol + ii] -= dblwin_body[(int) dblwin->waa + ii];
            // fprintf(stderr, "                   inv %d - xx=%f avg=%f\n",
            //     ii,
            //     dblwin_body[(int) dblwin->waa + ii], dblwin_head[ncol + ii]);
        }
    }
    // dblwin - push xx
    for (int ii = 0; ii < ncol; ii += 1) {
        DOUBLEWIN_AGGREGATE_PUSH(dblwin_head[ii]);
    }
}

// SQLMATH_FUNC sql3_win_avg1_func - end

// SQLMATH_FUNC sql3_win_avg2_func - start
SQLMATH_FUNC static void sql3_win_avg2_value(
    sqlite3_context * context
) {
// This function will calculate running-avg.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    const int ncol = (int) dblwin->ncol;
    if (!ncol) {
        return;
    }
    // dblwin - result
    const double inv = dblwin->ncol / dblwin->nbody;
    for (int ii = 0; ii < ncol; ii += 1) {
        dblwin_head[2 * ncol + ii] = dblwin_head[ncol + ii] * inv;
    }
    doublearrayResult(context, dblwin_head + 2 * ncol, ncol,
        SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_avg2_final(
    sqlite3_context * context
) {
// This function will calculate running-avg.
    // dblwin - value
    sql3_win_avg2_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_avg2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-avg.
    sql3_win_avg1_inverse(context, argc, argv);
}

SQLMATH_FUNC static void sql3_win_avg2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-avg.
    sql3_win_avg1_step(context, argc, argv);
}

// SQLMATH_FUNC sql3_win_avg2_func - end

// SQLMATH_FUNC sql3_lgbm_predictfortable_func - start
typedef struct AggLgbmPredict {
    BoosterHandle booster;      // booster
    int num_iteration;          // [out] Number of iterations of this booster
    int ncol;                   // Number of columns
    //
    FastConfigHandle fastConfig;        // [out] FastConfig object
    int64_t nnn;                // number of elements
    double result[256];         // [out] Pointer to array with predictions
} AggLgbmPredict;

SQLMATH_FUNC static void sql3_lgbm_predictfortable_value(
    sqlite3_context * context
) {
// This function will make prediction for sql-table from <model_str>.
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggLgbmPredict);
    // agg - null-case
    if (agg->nnn <= 0) {
        return;
    }
    sqlite3_result_blob(context, (char *) agg->result,
        (int) agg->nnn * sizeof(double), SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_lgbm_predictfortable_final(
    sqlite3_context * context
) {
// This function will make prediction for sql-table from <model_str>.
    int errcode = 0;
    // agg - value
    sql3_lgbm_predictfortable_value(context);
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggLgbmPredict);
    // agg - cleanup
    errcode = LGBM_BoosterFree(agg->booster);
    LGBM_ASSERT_OK();
    errcode = LGBM_FastConfigFree(agg->fastConfig);
    LGBM_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql3_lgbm_predictfortable_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will make prediction for sql-table from <model_str>.
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    UNUSED_PARAMETER(context);
}

static void sql3_lgbm_predictfortable_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will make prediction for sql-table from <model_str>.
    static int argc0 = 5;
    const int ncol = argc - argc0;
    int errcode = 0;
    if (ncol < 1) {
        sqlite3_result_error(context,
            "lgbm_predictfortable - wrong number of arguments", -1);
        return;
    }
    // agg - init
    SQLITE3_AGGREGATE_CONTEXT(AggLgbmPredict);
    if (agg->nnn == 0) {
        const char *model_str = (char *) sqlite3_value_text(argv[0]);
        if (model_str == NULL) {
            sqlite3_result_error(context,
                "lgbm_predictfortable - model_str cannot be NULL", -1);
            return;
        }
        errcode = LGBM_BoosterLoadModelFromString(      //
            // const char *model_str,
            (char *) sqlite3_value_text(argv[0]),       //
            &agg->num_iteration,        // int *out_num_iteration,
            &agg->booster);     // BoosterHandle *out
        LGBM_ASSERT_OK();
        errcode = LGBM_BoosterPredictForMatSingleRowFastInit(   //
            agg->booster,       // BoosterHandle handle,
            sqlite3_value_int(argv[1]), // const int predict_type,
            sqlite3_value_int(argv[2]), // const int start_iteration,
            sqlite3_value_int(argv[3]), // const int num_iteration,
            C_API_DTYPE_FLOAT64,        // const int data_type,
            ncol,               // const int32_t ncol,
            // const char *parameter,
            (char *) sqlite3_value_text(argv[4]),       //
            &agg->fastConfig);  // FastConfigHandle *out_fastConfig
        LGBM_ASSERT_OK();
    }
    double data[SQLITE_MAX_FUNCTION_ARG] = { 0 };
    for (int ii = 0; ii < ncol; ii += 1) {
        data[ii] = sqlite3_value_double_or_nan(argv[argc0 + ii]);
    }
    errcode = LGBM_BoosterPredictForMatSingleRowFast(   //
        agg->fastConfig,        // FastConfigHandle fastConfig_handle,
        data,                   // const void *data,
        &agg->nnn,              // int64_t *out_len,
        agg->result);           // double *out_result
    LGBM_ASSERT_OK();
    // fprintf(stderr, "    lgbm_predictfortable - out_len=%I64d\n", agg->nnn);
  catch_error:
    (void) 0;
}

// SQLMATH_FUNC sql3_lgbm_predictfortable_func - end

// SQLMATH_FUNC sql3_stdev_func - start
typedef struct AggStdev {
    double mxx;                 // x-average
    double nnn;                 // number of elements
    double vxx;                 // x-variance.p
    double wnn;                 // window-mode
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
        agg->wnn = 1;
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
                (xx * xx - xx0 * xx0) - dx * (dx * invn0 + 2 * agg->mxx);
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
    doublearrayResult(context, dblwin_head, (int) dblwin->nhead,
        SQLITE_TRANSIENT);
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
            "win_coinflip2 - wrong number of arguments", -1);
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
            "coinflip_extract - 2nd argument must be integer column >= 0",
            -1);
        return;
    }
    if (bytes <= 0 || SIZEOF_BLOB_MAX < bytes   //
        || bytes < (icol + 1) * WIN_COINFLIP_N * sizeof(double)) {
        sqlite3_result_error(context,
            "coinflip_extract"
            " - 1st argument as coinflip-object does not have enough columns",
            -1);
        return;
    }
    const WinCoinflip *agg =
        (WinCoinflip *) sqlite3_value_blob(argv[0]) + icol;
    const char *key = (char *) sqlite3_value_text(argv[2]);
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
        "coinflip_extract - 3rd argument is invalid key", -1);
}

// SQLMATH_FUNC sql3_win_coinflip2_func - end

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
        dblwin->wnn = 1;
    }
}

SQLMATH_FUNC static void sql3_win_ema1_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running exponential-moving-average.
    if (argc < 2) {
        sqlite3_result_error(context, "win_ema2 - wrong number of arguments",
            -1);
        return;
    }
    // dblwin - init
    const int ncol = argc - 1;
    double arg_alpha = NAN;
    DOUBLEWIN_AGGREGATE_CONTEXT(argc);
    if (dblwin->nbody == 0) {
        // dblwin - init ncol
        dblwin->ncol = ncol;
        // arg_alpha
        arg_alpha = sqlite3_value_double_or_nan(argv[0]);
        if (isnan(arg_alpha)) {
            sqlite3_result_error(context,
                "win_emax - invalid argument 'alpha'", -1);
            return;
        }
        dblwin_head[ncol + 0] = arg_alpha;
    }
    // dblwin - calculate ema
    arg_alpha = dblwin_head[ncol + 0];
    const int nrow = (int) dblwin->nbody / ncol;
    argv += 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        sqlite3_value_double_or_prev(argv[0], &dblwin_head[ii]);
        double *row = dblwin_body + ii;
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
        return;
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
    UNUSED_PARAMETER(argv);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->wnn) {
        dblwin->wnn = 1;
    }
    // dblwin - inverse
    const int ncol = argc / 2;
    const int nstep = ncol * 2;
    const int nn = (int) dblwin->nbody - nstep;
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
            "win_quantile2 - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = argc / 2;
    DOUBLEWIN_AGGREGATE_CONTEXT(2 * ncol);
    if (dblwin->nbody == 0) {
        // dblwin - init ncol
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
    const int nn = (int) dblwin->nbody / nstep;
    double *arr = dblwin_body + 1;
    for (int ii = 0; ii < ncol; ii += 1) {
        // init argQuantile
        double argQuantile = sqlite3_value_double_or_nan(argv[2 * ii + 0]);
        if (!(0 <= argQuantile && argQuantile <= 1)) {
            sqlite3_result_error(context,
                "win_quantilex"
                " - argument 'quantile' must be between 0 and 1 inclusive",
                -1);
            return;
        }
        argQuantile *= (nn - 1);
        const int kk1 = (int) floor(argQuantile) * nstep;
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
        return;
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
    double see;                 // sine y-stdev
    double spp;                 // sine phase
    double sww;                 // sine angular-frequency
    //
    double vxx;                 // x-variance.p
    double vxy;                 // xy-covariance.p
    double vyy;                 // y-variance.p
    //
    double wbb;                 // window-position-right
    double wnn;                 // window-mode
    //
    double xx0;                 // x-trailing
    double xx1;                 // x-current
    double xx2;                 // x-refit
    double yy0;                 // y-trailing
    double yy1;                 // y-current
} WinSinefit;
static const int WIN_SINEFIT_N = sizeof(WinSinefit) / sizeof(double);
static const int WIN_SINEFIT_STEP = 3 + 0;
// static const int WIN_SINEFIT_STEP = 3 + 2;

static void winSinefitDft(
    WinSinefit * wsf,
    double *xxyy,
    const int wbb,
    const int nbody,
    const int ncol
) {
// This function will calculate running sliding-discrete-fourier-transform as:
//     DFTn(tt+1) = (DFTn(tt) - dft(tt) + dft(tt+nnn)) * e
    UNUSED_PARAMETER(wsf);
    UNUSED_PARAMETER(xxyy);
    UNUSED_PARAMETER(wbb);
    UNUSED_PARAMETER(nbody);
    UNUSED_PARAMETER(ncol);
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
        vxx += (xx * xx - xx0 * xx0) - dx * (dx * invn0 + 2 * mxx);
        vyy += (yy * yy - yy0 * yy0) - dy * (dy * invn0 + 2 * myy);
        vxy += (xx * yy - xx0 * yy0) - dx * myy - dy * (dx * invn0 + mxx);
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

static void winSinefitSnr(
    WinSinefit * wsf,
    double *xxyy,
    const int wbb,
    const int nbody,
    const int ncol
) {
// This function will calculate running sine-regression as:
//     yy = saa*sin(sww*xx + spp)
    UNUSED_PARAMETER(wbb);
    // declare var0
    const double nnn = nbody / (ncol * WIN_SINEFIT_STEP);
    const double invn0 = 1.0 / nnn;
    double saa = 0;
    double spp = 0;
    double sww = 0;
    double tmp = 0;
    // guess snr - saa
    if (1) {
        saa = sqrt(2 * wsf->vyy * invn0 //
            * (1 - wsf->vxy * wsf->vxy / (wsf->vxx * wsf->vyy)));
    }
    if (saa <= 0 || !isnormal(saa)) {
        return;
    }
    const double inva = 1.0 / saa;
    // guess snr - sww - using x-variance.p
    if (1) {
        sww = 2 * MATH_PI / sqrt(4.0 * wsf->vxx * invn0);       // window-period
    }
    // guess snr - spp - using multivariate-linear-regression
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
            // Use de-trended residual.
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
    for (int jj = 8; jj > 0; jj -= 1) {
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
        wsf->see = sqrt(vrr1 * invn0);
    }
    // save wsf
    if (spp < 0) {
        spp += 2 * MATH_PI;
    }
    wsf->saa = saa;
    wsf->spp = spp;
    wsf->sww = sww;
}

SQLMATH_FUNC static void sql3_win_sinefit2_value(
    sqlite3_context * context
) {
// This function will calculate running simple-linear-regression
// and sine-regression as:
//     yy = laa + lbb*xx + saa*sin(sww*xx + spp)
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    const WinSinefit *wsf = (WinSinefit *) dblwin_head;
    // dblwin - result
    doublearrayResult(context, dblwin_head,     //
        // If x-current == x-refit, then include extra data needed for refit.
        // This data is normally not included, due to memory performance.
        (int) (wsf->xx2 == wsf->xx1     //
            ? dblwin->nhead + dblwin->nbody     //
            : dblwin->nhead), SQLITE_TRANSIENT);
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
        dblwin->wnn = 1;
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
            "win_sinefit2 - wrong number of arguments", -1);
        return;
    }
    // dblwin - init
    const int ncol = (argc - argc0) / 2;
    DOUBLEWIN_AGGREGATE_CONTEXT(ncol * WIN_SINEFIT_N);
    if (dblwin->nbody == 0) {
        // dblwin - init ncol
        dblwin->ncol = ncol;
    }
    // dblwin - init argv
    const double xx2 = sqlite3_value_double_or_nan(argv[1]);
    const int modeSnr = sqlite3_value_int(argv[0]);
    argv += argc0;
    WinSinefit *wsf = NULL;
    const int waa = (int) dblwin->waa;
    const int wbb = (int) (dblwin->wnn ? dblwin->waa : dblwin->nbody);
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
            winSinefitDft(wsf, xxyy, wbb, (int) dblwin->nbody,
                (int) dblwin->ncol);
            winSinefitSnr(wsf, xxyy, wbb, (int) dblwin->nbody,
                (int) dblwin->ncol);
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
            "sinefit_extract - 2nd argument must be integer column >= 0", -1);
        return;
    }
    if (bytes <= 0 || SIZEOF_BLOB_MAX < bytes   //
        || bytes < (icol + 1) * WIN_SINEFIT_N * sizeof(double)) {
        sqlite3_result_error(context,
            "sinefit_extract"
            " - 1st argument as sinefit-object does not have enough columns",
            -1);
        return;
    }
    const WinSinefit *wsf = (WinSinefit *) sqlite3_value_blob(argv[0]) + icol;
    const char *key = (char *) sqlite3_value_text(argv[2]);
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
                / wsf->nnn));
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
        sqlite3_result_double_or_null(context, 0        //
            + wsf->yy1 - wsf->rr1       //
            + wsf->saa * sin(fmod(wsf->sww * wsf->xx1,
                    2 * MATH_PI) + wsf->spp));
        return;
    }
    sqlite3_result_error(context,
        "sinefit_extract - 3rd argument is invalid key", -1);
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
            "sinefit_refitlast"
            " - 1st argument as sinefit-object does not have enough columns",
            -1);
        return;
    }
    const WinSinefit *blob0 = sqlite3_value_blob(argv[0]);
    const int nbody = (int) blob0->nnn * ncol * WIN_SINEFIT_STEP;
    if (blob0->nnn <= 0
        || bytes != (ncol * WIN_SINEFIT_N + nbody) * sizeof(double)) {
        sqlite3_result_error(context,
            "sinefit_refitlast"
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
    const int wbb = (int) wsf->wbb;
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
        winSinefitDft(wsf, xxyy, wbb, nbody, ncol);
        winSinefitSnr(wsf, xxyy, wbb, nbody, ncol);
        // increment counter
        argv += 2;
        wsf += 1;
        xxyy += WIN_SINEFIT_STEP;
    }
    // dblwin - result
    doublearrayResult(context, (double *) wsf0, bytes / sizeof(double),
        sqlite3_free);
    return;
  catch_error:
    sqlite3_result_error(context, "sinefit_refitlast - invalid arguments",
        -1);
}

// SQLMATH_FUNC sql3_win_sinefit2_func - end

// SQLMATH_FUNC sql3_win_sum1_func - start
SQLMATH_FUNC static void sql3_win_sum1_value(
    sqlite3_context * context
) {
// This function will calculate running-sum.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    sqlite3_result_double(context, dblwin_head[(int) dblwin->ncol]);
}

SQLMATH_FUNC static void sql3_win_sum1_final(
    sqlite3_context * context
) {
// This function will calculate running-sum.
    // dblwin - value
    sql3_win_sum1_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_sum1_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-sum.
    sql3_win_avg1_inverse(context, argc, argv);
}

SQLMATH_FUNC static void sql3_win_sum1_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-sum.
    sql3_win_avg1_step(context, argc, argv);
}

// SQLMATH_FUNC sql3_win_sum1_func - end

// SQLMATH_FUNC sql3_win_sum2_func - start
SQLMATH_FUNC static void sql3_win_sum2_value(
    sqlite3_context * context
) {
// This function will calculate running-sum.
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    if (!dblwin->ncol) {
        return;
    }
    // dblwin - result
    doublearrayResult(context, dblwin_head + (int) dblwin->ncol,
        (int) dblwin->ncol, SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql3_win_sum2_final(
    sqlite3_context * context
) {
// This function will calculate running-sum.
    // dblwin - value
    sql3_win_sum2_value(context);
    // dblwin - init
    DOUBLEWIN_AGGREGATE_CONTEXT(0);
    // dblwin - cleanup
    doublewinAggfree(dblwinAgg);
}

SQLMATH_FUNC static void sql3_win_sum2_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-sum.
    sql3_win_avg1_inverse(context, argc, argv);
}

SQLMATH_FUNC static void sql3_win_sum2_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate running-sum.
    sql3_win_avg1_step(context, argc, argv);
}

// SQLMATH_FUNC sql3_win_sum2_func - end

// file sqlmath_base - init
int sqlite3_compress_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
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
    errcode = sqlite3_sqlmath_custom_init(db, pzErrMsg, pApi);
    SQLITE_OK_OR_RETURN_RC(errcode);
    //
    SQL_CREATE_FUNC1(castrealornull, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(castrealorzero, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(casttextorempty, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(coinflip_extract, 3, 0);
    SQL_CREATE_FUNC1(copyblob, 1, 0);
    SQL_CREATE_FUNC1(cot, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(coth, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(doublearray_array, -1, 0);
    SQL_CREATE_FUNC1(doublearray_extract, 2, 0);
    SQL_CREATE_FUNC1(doublearray_jsonfrom, 1, 0);
    SQL_CREATE_FUNC1(doublearray_jsonto, 1, 0);
    SQL_CREATE_FUNC1(fmod, 2, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(idateadd, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(idatefrom, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(idatefromepoch, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(idateto, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(idatetoepoch, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(idateymdfrom, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(idateymdfromepoch, -1, SQLITE_FUNC_IDATE);
    SQL_CREATE_FUNC1(lgbm_datasetcreatefromfile, 3, 0);
    SQL_CREATE_FUNC1(lgbm_datasetcreatefrommat, 7, 0);
    SQL_CREATE_FUNC1(lgbm_datasetdumptext, 2, 0);
    SQL_CREATE_FUNC1(lgbm_datasetfree, 1, 0);
    SQL_CREATE_FUNC1(lgbm_datasetgetnumdata, 1, 0);
    SQL_CREATE_FUNC1(lgbm_datasetgetnumfeature, 1, 0);
    SQL_CREATE_FUNC1(lgbm_datasetsavebinary, 1, 0);
    SQL_CREATE_FUNC1(lgbm_dlopen, 1, 0);
    SQL_CREATE_FUNC1(lgbm_extract, 2, 0);
    SQL_CREATE_FUNC1(lgbm_predictforfile, 8, 0);
    SQL_CREATE_FUNC1(lgbm_trainfromdataset, 5, 0);
    SQL_CREATE_FUNC1(lgbm_trainfromfile, 6, 0);
    SQL_CREATE_FUNC1(marginoferror95, 2, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(normalizewithsqrt, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(normalizewithsquared, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(random1, 0, 0);
    SQL_CREATE_FUNC1(roundorzero, 2, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(sha256, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(sinefit_extract, 4, 0);
    SQL_CREATE_FUNC1(sinefit_refitlast, -1, 0);
    SQL_CREATE_FUNC1(sqrtwithsign, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(squared, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(squaredwithsign, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(strtoll, 2, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(throwerror, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(zlib_compress, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC1(zlib_uncompress, 1, SQLITE_DETERMINISTIC);
    SQL_CREATE_FUNC2(columntype, 1, 0);
    SQL_CREATE_FUNC2(lgbm_datasetcreatefromtable, -1, 0);
    SQL_CREATE_FUNC2(lgbm_trainfromtable, -1, 0);
    SQL_CREATE_FUNC2(median2, 1, 0);
    SQL_CREATE_FUNC2(quantile, 2, 0);
    SQL_CREATE_FUNC3(lgbm_predictfortable, -1, 0);
    SQL_CREATE_FUNC3(stdev, 1, 0);
    SQL_CREATE_FUNC3(win_avg1, 1, 0);
    SQL_CREATE_FUNC3(win_avg2, -1, 0);
    SQL_CREATE_FUNC3(win_coinflip2, -1, 0);
    SQL_CREATE_FUNC3(win_ema1, 2, 0);
    SQL_CREATE_FUNC3(win_ema2, -1, 0);
    SQL_CREATE_FUNC3(win_quantile1, 2, 0);
    SQL_CREATE_FUNC3(win_quantile2, -1, 0);
    SQL_CREATE_FUNC3(win_sinefit2, -1, 0);
    SQL_CREATE_FUNC3(win_sum1, 1, 0);
    SQL_CREATE_FUNC3(win_sum2, -1, 0);
    return 0;
}
#endif                          // SRC_SQLMATH_BASE_C2
/*
file sqlmath_base - end
*/


/*
file sqlmath_nodejs - start
*/
#if defined(SRC_SQLMATH_NODEJS_C2)


#if defined(UNDEFINED)          // cpplint-hack
#   include <cstdio>
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
    errcode = napi_get_last_error_info(env, &info);
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
            (char *) sqlite_buf->buf,   // const char* str
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
#endif                          // SQLMATH_NODEJS_C2
/*
file sqlmath_nodejs - end
*/


/*
file sqlmath_python - start
*/
#if defined(SRC_SQLMATH_PYTHON_C2)


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
            (char *) sqlite_buf->buf,   //
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
#endif                          // SRC_SQLMATH_PYTHON_C2
/*
file sqlmath_python - end
*/
#endif                          // SRC_SQLMATH_H2
