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


// *INDENT-OFF*
#ifdef SQLMATH_NAPI
    #include <sqlite3.h>
#else // SQLMATH_NAPI
    #include <sqlite3ext.h>
    static const sqlite3_api_routines *sqlite3_api;
#endif // SQLMATH_NAPI
// *INDENT-ON*


#include <assert.h>
#include <float.h>
#include <math.h>
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


#define ALLOCC calloc
#define ALLOCF free
#define ALLOCM malloc
#define JS_MAX_SAFE_INTEGER 0x1fffffffffffff
#define JS_MIN_SAFE_INTEGER -0x1fffffffffffff
#define MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
#define SGN(aa) (((aa) < 0) ? -1 : ((aa) > 0) ? 1 : 0)
#define SIZEOF_MESSAGE_DEFAULT 1024
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_BLOB_0          0x14
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_FLOAT_0         0x12
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       -0x01
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
#define SQLITE_ERROR_DATATYPE_INVALID 0x10003
#define SQLITE_MAX_LENGTH2 1000000000
#define SQLITE_RESPONSETYPE_LAST_VALUE 1
#define SQLMATH_API
#define SQLMATH_FNC
#define SQLMATH_IS_OK_OR_RETURN_RC(rc) if ((rc) != SQLITE_OK) {return (rc);}
#define SWAP(aa, bb) tmp = (aa); (aa) = (bb); (bb) = tmp
#define UNUSED(x) (void)(x)


#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql_##func##_func, NULL, NULL); \
    SQLMATH_IS_OK_OR_RETURN_RC(errcode);

#define SQLITE3_CREATE_FUNCTION2(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        NULL, sql_##func##_step, sql_##func##_final); \
    SQLMATH_IS_OK_OR_RETURN_RC(errcode);


// file sqlmath_h - sqlite3
// *INDENT-OFF*
typedef struct FuncDef FuncDef;
typedef struct JsonString JsonString;
typedef struct Vdbe Vdbe;
typedef struct sqlite3_value Mem;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;

/*
** The "context" argument for an installable function.  A pointer to an
** instance of this structure is the first argument to the routines used
** implement the SQL functions.
**
** There is a typedef for this structure in sqlite.h.  So all routines,
** even the public interface to SQLite, can use a pointer to this structure.
** But this file is the only place where the internal details of this
** structure are known.
**
** This structure is defined inside of vdbeInt.h because it uses substructures
** (Mem) which are only defined there.
*/
struct sqlite3_context {
  Mem *pOut;              /* The return value is stored here */
  FuncDef *pFunc;         /* Pointer to function information */
  Mem *pMem;              /* Memory cell used to store aggregate context */
  Vdbe *pVdbe;            /* The VM that owns this context */
  int iOp;                /* Instruction number of OP_Function */
  int isError;            /* Error code returned by the function. */
  u8 skipFlag;            /* Skip accumulator loading if true */
  u8 argc;                /* Number of arguments */
  sqlite3_value *argv[1]; /* Argument set */
};


// file sqlmath_h - carray
/*
** Structure used to hold the sqlite3_carray_bind() information
*/
typedef struct carray_bind carray_bind;
struct carray_bind {
  void *aData;                /* The data */
  int nData;                  /* Number of elements */
  int mFlags;                 /* Control flags */
  void (*xDel)(void*);        /* Destructor for aData */
};

SQLITE_API int carrayBestIndex(
    sqlite3_vtab * tab,
    sqlite3_index_info * pIdxInfo
);

SQLITE_API int carrayClose(
    sqlite3_vtab_cursor * cur
);

SQLITE_API int carrayColumn(
    sqlite3_vtab_cursor * cur,  /* The cursor */
    sqlite3_context * ctx,      /* First argument to sqlite3_result_...() */
    int i                       /* Which column to return */
);

SQLITE_API int carrayConnect(
    sqlite3 * db,
    void *pAux,
    int argc,
    const char *const *argv,
    sqlite3_vtab ** ppVtab,
    char **pzErr
);

SQLITE_API int carrayDisconnect(
    sqlite3_vtab * pVtab
);

SQLITE_API int carrayEof(
    sqlite3_vtab_cursor * cur
);

SQLITE_API int carrayFilter(
    sqlite3_vtab_cursor * pVtabCursor,
    int idxNum,
    const char *idxStr,
    int argc,
    sqlite3_value ** argv
);

SQLITE_API int carrayNext(
    sqlite3_vtab_cursor * cur
);

SQLITE_API int carrayOpen(
    sqlite3_vtab * p,
    sqlite3_vtab_cursor ** ppCursor
);

SQLITE_API int carrayRowid(
    sqlite3_vtab_cursor * cur,
    sqlite_int64 * pRowid
);

SQLITE_API int sqlite3_carray_bind2(
    sqlite3_stmt * pStmt,
    int idx,
    void *aData,
    int nData,
    int mFlags,
    void (*xDestroy) (void *),  // NOLINT
    carray_bind ** pBind
);


// file sqlmath_h - JsonString
/* An instance of this object represents a JSON string
** under construction.  Really, this is a generic string accumulator
** that can be and is used to create strings other than JSON. */
struct JsonString {
  sqlite3_context *pCtx;   /* Function context - put error messages here */
  char *zBuf;              /* Append JSON content here */
  u64 nAlloc;              /* Bytes of storage available in zBuf[] */
  u64 nUsed;               /* Bytes of zBuf[] currently used */
  u8 bStatic;              /* True if zBuf is static space */
  u8 bErr;                 /* True if an error has been encountered */
  char zSpace[100];        /* Initial static space */
  //
  double kk;
};

/* Enlarge pJson->zBuf so that it can hold at least N more bytes.
** Return zero on success.  Return non-zero on an OOM error */
SQLITE_API int jsonGrow(JsonString *p, u32 N);

/* Append a single character */
SQLITE_API void jsonAppendChar(JsonString *p, char c);

/* Append N bytes from zIn onto the end of the JsonString string. */
SQLITE_API void jsonAppendRaw(JsonString *p, const char *zIn, u32 N);

/* Append a comma separator to the output buffer, if the previous
** character is not '[' or '{'. */
SQLITE_API void jsonAppendSeparator(JsonString *p);

/* Append the N-byte string in zIn to the end of the JsonString string
** under construction.  Enclose the string in "..." and escape
** any double-quotes or backslash characters contained within the
** string. */
SQLITE_API void jsonAppendString(JsonString *p, const char *zIn, u32 N);

/* Initialize the JsonString object */
SQLITE_API void jsonInit(JsonString *p, sqlite3_context *pCtx);

/* Initialize the JsonString object with no context */
SQLITE_API void jsonInitNoContext(JsonString *p);

/* Free all allocated memory and reset the JsonString object back to its
** initial state. */
SQLITE_API void jsonReset(JsonString *p);
// *INDENT-ON*


// file sqlmath_h - Jsbaton
#define JSBATON_ARGC 16

#define JSBATON_ERRCODE_SAVE() \
    jsbatonErrcodeSave(baton, db, errcode, __func__, __FILE__, __LINE__);

typedef struct napi_async_work__ *napi_async_work;
typedef struct napi_deferred__ *napi_deferred;
typedef struct napi_value__ *napi_value;

typedef struct Jsbaton {
    // data
    int64_t argint64[JSBATON_ARGC];
    const char *bufin[JSBATON_ARGC];
    char *bufout[JSBATON_ARGC];
    char errmsg[SIZEOF_MESSAGE_DEFAULT];
    napi_value result;
    // async
    napi_async_work work;
    napi_deferred deferred;
} Jsbaton;

SQLMATH_API SQLITE_NOINLINE void jsbatonErrcodeSave(
    Jsbaton * baton,
    sqlite3 * db,
    int errcode,
    const char *func,
    const char *file,
    int line
);


// file sqlmath_h - misc
// this function will exec <sql> and if <errcode> is not ok,
// throw <baton->errmsg>
#define SQLMATH_ASSERT_NOT_OOM(pp) \
    if (pp == NULL) { errcode = SQLITE_NOMEM; SQLMATH_ASSERT_OK(); }

#define SQLMATH_ASSERT_OK() \
    if (!SQLMATH_IS_OK()) { JSBATON_ERRCODE_SAVE(); goto catch_error; }

#define SQLMATH_IS_OK() \
    (errcode == 0 || errcode == SQLITE_ROW || errcode == SQLITE_DONE)

typedef struct DbExecBindElem DbExecBindElem;

SQLMATH_API int dbExec(
    sqlite3 * db,               /* The database on which the SQL executes */
    Jsbaton * baton
);

SQLMATH_API int dbTableInsert(
    sqlite3 * db,               /* The database on which the SQL executes */
    Jsbaton * baton
);

SQLMATH_API int doubleSign(
    const double aa
);

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
);

SQLMATH_API void jsonVectorDoubleAppend(
    JsonString * pp,
    const double val
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


// file sqlmath_ext - dbExec - start
typedef struct DbExecBindElem {
    const char *buf;
    char *key;
    int buflen;
    char datatype;
} DbExecBindElem;

SQLMATH_API int dbExec(
    sqlite3 * db,               /* The database on which the SQL executes */
    Jsbaton * baton
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized json-string in <str99>.
#define STR99_APPEND_CHAR2(cc) \
    if (!responseType) { jsonAppendChar(str99, cc); STR99_ASSERT_NOT_OOM(); }
#define STR99_APPEND_JSON(zz, nn) \
    jsonAppendString(str99, zz, nn); STR99_ASSERT_NOT_OOM();
#define STR99_APPEND_RAW(zz, nn) \
    jsonAppendRaw(str99, zz, nn); STR99_ASSERT_NOT_OOM();
#define STR99_ASSERT_NOT_OOM() \
    if (str99->bErr != 0) { errcode = SQLITE_NOMEM; goto catch_error; }
    // coverage-hack
    noop();
    // declare var
    DbExecBindElem *bindElem;
    DbExecBindElem *bindList = NULL;
    JsonString *str99 = NULL;
    const char **pzShared = baton->bufin + 6;
    const char *zBind = baton->bufin[3];
    const char *zSql = baton->bufin[1];
    const char *zTmp = NULL;
    double rTmp = 0;
    int bindByKey = (int) baton->argint64[4];
    int bindIdx = 0;
    int bindListLength = (int) baton->argint64[2];
    int errcode = 0;
    int ii = 0;
    int jj = 0;
    int nCol = 0;
    int responseType = (int) baton->argint64[5];
    int64_t *pnShared = baton->argint64 + 6;
    int64_t iTmp = 0;
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    static const char bindPrefix[] = "$:@";
    // mutext enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // init bindList
    bindList =
        (DbExecBindElem *) ALLOCC(bindListLength, sizeof(DbExecBindElem));
    if (bindListLength > 0 && bindList == NULL) {
        SQLMATH_ASSERT_NOT_OOM(str99);
    }
    bindElem = bindList;
    ii = 1;
    while (ii <= bindListLength) {
        // init key and
        if (bindByKey) {
            zBind += 1;
            bindElem->key = (char *) zBind + 8;
            zBind += 8 + *(int *) zBind;
        }
        // init datatype
        bindElem->datatype = zBind[0];
        zBind += 1;
        switch (bindElem->datatype) {
        case SQLITE_DATATYPE_BLOB:
        case SQLITE_DATATYPE_TEXT:
            bindElem->buflen = *(int *) zBind;
            bindElem->buf = zBind + 8;
            zBind += 8 + bindElem->buflen;
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
            bindElem->buflen = *(int *) pnShared;
            bindElem->buf = *pzShared;
            pnShared += 1;
            pzShared += 1;
            break;
        default:
            fprintf(stderr, "\n[ii=%d  datatype=%d  len=%d]\n", ii,
                bindElem->datatype, bindElem->buflen);
            errcode = SQLITE_ERROR_DATATYPE_INVALID;
            SQLMATH_ASSERT_OK();
        }
        bindElem += 1;
        ii += 1;
    }
    // init str99
    str99 = (JsonString *) ALLOCM(sizeof(JsonString));
    SQLMATH_ASSERT_NOT_OOM(str99);
    jsonInitNoContext(str99);
    // bracket database [
    STR99_APPEND_CHAR2('[');
    // loop over each table
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        errcode = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zTmp);
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
                            bindElem->buflen, SQLITE_TRANSIENT);
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
                            bindElem->buflen, SQLITE_TRANSIENT);
                        break;
                    default:
                        fprintf(stderr, "\n[ii=%d  datatype=%d  len=%d]\n",
                            ii, bindElem->datatype, bindElem->buflen);
                        errcode = SQLITE_ERROR_DATATYPE_INVALID;
                    }
                    // ignore bind-range-error
                    if (errcode != SQLITE_RANGE) {
                        SQLMATH_ASSERT_OK();
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
        // loop over each row
        while (1) {
            errcode = sqlite3_step(pStmt);
            // if no next row, then break
            if (errcode != SQLITE_ROW) {
                sqlite3_finalize(pStmt);
                pStmt = NULL;
                if (SQLMATH_IS_OK()) {
                    break;
                }
                goto catch_error;
            }
            // insert row of column-names
            if (nCol == -1) {
                if (str99->nUsed > 1) {
                    STR99_APPEND_CHAR2(',');
                    STR99_APPEND_CHAR2('\n');
                    STR99_APPEND_CHAR2('\n');
                }
                // bracket table [
                STR99_APPEND_CHAR2('[');
                // bracket column [
                STR99_APPEND_CHAR2('[');
                // loop over each column-name
                nCol = sqlite3_column_count(pStmt);
                ii = 0;
                while (ii < nCol) {
                    if (ii > 0) {
                        STR99_APPEND_CHAR2(',');
                    }
                    zTmp = sqlite3_column_name(pStmt, ii);
                    STR99_APPEND_JSON(zTmp, strlen(zTmp));
                    ii += 1;
                }
                // bracket column ]
                STR99_APPEND_CHAR2(']');
            }
            // bracket row [
            STR99_APPEND_CHAR2(',');
            STR99_APPEND_CHAR2('\n');
            STR99_APPEND_CHAR2('[');
            ii = 0;
            // loop over each column-value
            while (ii < nCol) {
                switch (responseType) {
                case SQLITE_RESPONSETYPE_LAST_VALUE:
                    // export last-value as blob
                    str99->nUsed = 0;
                    STR99_APPEND_RAW((const char *) sqlite3_column_blob(pStmt,
                            ii), sqlite3_column_bytes(pStmt, ii));
                    ii += 1;
                    continue;
                }
                if (ii > 0) {
                    STR99_APPEND_CHAR2(',');
                }
                switch (sqlite3_column_type(pStmt, ii)) {
                case SQLITE_INTEGER:
                    iTmp = sqlite3_column_int64(pStmt, ii);
                    if (JS_MIN_SAFE_INTEGER <= iTmp
                        // convert integer to double
                        && iTmp <= JS_MAX_SAFE_INTEGER) {
                        STR99_APPEND_RAW((const char *)
                            sqlite3_column_text(pStmt, ii),
                            sqlite3_column_bytes(pStmt, ii));
                    } else {
                        // convert integer to string
                        STR99_APPEND_JSON((const char *)
                            sqlite3_column_text(pStmt, ii),
                            sqlite3_column_bytes(pStmt, ii));
                    }
                    break;
                case SQLITE_FLOAT:
                    rTmp = sqlite3_column_double(pStmt, ii);
                    if (isnan(rTmp) || rTmp == -INFINITY || rTmp == INFINITY) {
                        STR99_APPEND_RAW("null", 4);
                    } else {
                        STR99_APPEND_RAW((const char *)
                            sqlite3_column_text(pStmt, ii),
                            sqlite3_column_bytes(pStmt, ii));
                    }
                    break;
                case SQLITE_TEXT:
                    // append text as json-escaped string
                    STR99_APPEND_JSON((const char *)
                        sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                    // case SQLITE_BLOB:
                default:       /* case SQLITE_NULL: */
                    STR99_APPEND_RAW("null", 4);
                    break;
                }
                ii += 1;
            }
            // bracket row ]
            STR99_APPEND_CHAR2(']');
            if (errcode != SQLITE_ROW) {
                break;
            }
        }
        if (nCol != -1) {
            // bracket table ]
            STR99_APPEND_CHAR2(']');
        }
    }
    // bracket database ]
    STR99_APPEND_CHAR2(']');
    STR99_APPEND_CHAR2('\n');
    // copy str99 to baton
    // force bStatic -> 0
    jsonGrow(str99, sizeof(str99->zSpace) + 1);
    baton->argint64[0] = (int64_t) str99->nUsed;
    baton->bufout[0] = str99->zBuf;
  catch_error:
    // cleanup pStmt
    sqlite3_finalize(pStmt);
    // cleanup bindList
    ALLOCF(bindList);
    // cleanup str99
    ALLOCF(str99);
    if (!SQLMATH_IS_OK()) {
        // handle errcode
        JSBATON_ERRCODE_SAVE();
    }
    // mutext leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    return errcode;
}

SQLMATH_API int dbTableInsert(
    sqlite3 * db,               /* The database on which the SQL executes */
    Jsbaton * baton
) {
// This function will run <baton>->bufin[1] in <db> and insert rows
// <baton>->bufin[2] to given table.
    // declare var
    char datatype = 0;
    const char *pp = baton->bufin[3];
    const char *zTmp = NULL;
    int bTxn = 0;
    int errcode = 0;
    int ii = 0;
    int jj = 0;
    int nCol = (int) baton->argint64[4];
    int nRow = (int) baton->argint64[5];
    size_t nLen = 0;
    sqlite3_stmt *pStmt = NULL;
    // mutext enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // create temp table
    errcode = sqlite3_exec(db, baton->bufin[1], NULL, NULL, NULL);
    SQLMATH_ASSERT_OK();
    // begin transaction
    errcode = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    SQLMATH_ASSERT_OK();
    bTxn = 1;
    // init pStmt
    errcode = sqlite3_prepare_v2(db, baton->bufin[2], -1, &pStmt, &zTmp);
    SQLMATH_ASSERT_OK();
    while (ii < nRow) {
        jj = 1;
        // type - js
        // 1. bigint
        // 2. boolean
        // 3. null
        // 4. number
        // 5. object
        // 6. string
        // 7. symbol
        // 8. undefined
        while (jj <= nCol) {
            datatype = pp[0];
            pp += 1;
            switch (datatype) {
            case SQLITE_DATATYPE_BLOB:
                nLen = *(int *) pp;
                errcode = sqlite3_bind_blob(pStmt, jj, pp + 8, (int) nLen,
                    SQLITE_TRANSIENT);
                SQLMATH_ASSERT_OK();
                pp += 8 + nLen;
                break;
            case SQLITE_DATATYPE_FLOAT:
                errcode = sqlite3_bind_double(pStmt, jj, *(double *) pp);
                SQLMATH_ASSERT_OK();
                pp += 8;
                break;
            case SQLITE_DATATYPE_INTEGER:
                errcode = sqlite3_bind_int64(pStmt, jj, *(int64_t *) pp);
                SQLMATH_ASSERT_OK();
                pp += 8;
                break;
            case SQLITE_DATATYPE_INTEGER_0:
                errcode = sqlite3_bind_int(pStmt, jj, 0);
                SQLMATH_ASSERT_OK();
                break;
            case SQLITE_DATATYPE_INTEGER_1:
                errcode = sqlite3_bind_int(pStmt, jj, 1);
                SQLMATH_ASSERT_OK();
                break;
            case SQLITE_DATATYPE_NULL:
                errcode = sqlite3_bind_null(pStmt, jj);
                SQLMATH_ASSERT_OK();
                break;
            case SQLITE_DATATYPE_TEXT_0:
                errcode = sqlite3_bind_text(pStmt, jj, "", 0, SQLITE_STATIC);
                SQLMATH_ASSERT_OK();
                break;
            case SQLITE_DATATYPE_TEXT:
                nLen = *(int *) pp;
                errcode = sqlite3_bind_text(pStmt, jj, pp + 8, (int) nLen,
                    SQLITE_TRANSIENT);
                SQLMATH_ASSERT_OK();
                pp += 8 + nLen;
                break;
            default:
                fprintf(stderr, "\n[ii=%d  jj=%d  datatype=%d  len=%d]\n", ii,
                    jj, datatype, *(int *) pp);
                errcode = SQLITE_ERROR_DATATYPE_INVALID;
                SQLMATH_ASSERT_OK();
            }
            jj += 1;
        }
        errcode = sqlite3_step(pStmt);
        SQLMATH_ASSERT_OK();
        errcode = sqlite3_clear_bindings(pStmt);
        SQLMATH_ASSERT_OK();
        errcode = sqlite3_reset(pStmt);
        SQLMATH_ASSERT_OK();
        ii += 1;
    }
  catch_error:
    // end transaction
    if (bTxn) {
        bTxn = 0;
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
    }
    // cleanup pStmt
    sqlite3_finalize(pStmt);
    // mutext leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    return errcode;
}

SQLMATH_API SQLITE_NOINLINE void jsbatonErrcodeSave(
    Jsbaton * baton,
    sqlite3 * db,
    int errcode,
    const char *func,
    const char *file,
    int line
) {
// this function will if errcode != 0, set baton->errmsg
    switch (errcode) {
    case SQLITE_DONE:
    case SQLITE_OK:
    case SQLITE_ROW:
        return;
    }
    if (baton == NULL || baton->errmsg[0] != '\x00') {
        return;
    }
    switch (errcode) {
    case SQLITE_ERROR_DATATYPE_INVALID:
        sqlmathSnprintfTrace(baton->errmsg, "sqlite - ", "invalid datatype",
            func, file, line);
        return;
    case SQLITE_NOMEM:
        sqlmathSnprintfTrace(baton->errmsg, "sqlite - ", "out of memory",
            func, file, line);
        return;
    default:
        sqlmathSnprintfTrace(baton->errmsg, "sqlite - ",
            db == NULL ? sqlite3_errstr(errcode) : sqlite3_errmsg(db), func,
            file, line);
        return;
    }
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

// file sqlmath_ext - dbExec - end


// file sqlmath_ext - jenks - start
#include "sqlmath_jenks.c"
SQLMATH_FNC static void sql_jenks_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will calculate jenks natural breaks
    UNUSED(argc);
    // declare var
    JenksObject *self = NULL;
    const int kk = sqlite3_value_int(argv[2]);
    const int nn = sqlite3_value_int(argv[1]);
    double *input = (double *) sqlite3_value_blob(argv[0]);
    double *output = (double *) sqlite3_value_blob(argv[3]);
    // jenks init
    self = jenksCreate(nn, kk);
    if (self == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    // jenks exec
    jenks(self, nn, kk, input);
    memcpy(output, self, 2 * 8);
    memcpy(output + 2, self->resultBreaks, (size_t) self->kk * 8);
    memcpy(output + 2 + (size_t) self->kk, self->resultCounts,
        (size_t) self->kk * 8);
    // jenks cleanup
    jenksDestroy(self);
    sqlite3_result_null(context);
}

// file sqlmath_ext - jenks - end


// file sqlmath_ext - SQLMATH_FNC
SQLMATH_FNC static void sql_blob_copy_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will copy blob <argv>[0]
    UNUSED(argc);
    const void *buf = sqlite3_value_blob(argv[0]);
    if (buf == NULL) {
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_blob(context, buf, sqlite3_value_bytes(argv[0]),
        SQLITE_TRANSIENT);
}

SQLMATH_FNC static void sql_blob_create_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will create blob of size <argv>[0]
    UNUSED(argc);
    const int nn = sqlite3_value_int(argv[0]);
    if (nn <= 0) {
        sqlite3_result_null(context);
        return;
    }
    const void *buf = sqlite3_malloc(nn);
    if (buf == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    sqlite3_result_blob(context, buf, nn, sqlite3_free);
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

SQLMATH_FNC static void sql_cot_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return cot(argv[0])
    UNUSED(argc);
    assert(argc == 1);
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
    assert(argc == 1);
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
    // pp - init
    JsonString *pp = (JsonString *) sqlite3_aggregate_context(context, 0);
    if (pp == NULL) {
        sqlite3_result_null(context);
        return;
    }
    int nn = pp->nUsed / 8;
    if (nn <= 0) {
        sqlite3_result_null(context);
    } else {
        sqlite3_result_double(context, kthpercentile((double *) pp->zBuf, nn,
                pp->kk));
    }
    jsonReset(pp);
}

SQLMATH_FNC static void sql_kthpercentile_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate kth-percentile element
    UNUSED(argc);
    // pp - init
    JsonString *pp =
        (JsonString *) sqlite3_aggregate_context(context, sizeof(*pp));
    if (pp == NULL) {
        return;
    }
    // pp - jsonInit
    if (pp->zBuf == 0) {
        jsonInit(pp, context);
        pp->kk = sqlite3_value_double(argv[1]);
    }
    // pp - handle null-case
    if (sqlite3_value_numeric_type(argv[0]) == SQLITE_NULL) {
        return;
    }
    // pp - append double
    jsonVectorDoubleAppend(pp, sqlite3_value_double(argv[0]));
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
    // pp - init
    JsonString *pp = (JsonString *) sqlite3_aggregate_context(context, 0);
    if (pp == NULL) {
        sqlite3_result_null(context);
        return;
    }
    if (pp->nUsed <= 0) {
        sqlite3_result_null(context);
    } else {
        double *arr = (double *) pp->zBuf;
        arr[0] = (0.125 * ((double) pp->nUsed) - 2) / arr[1];
        sqlite3_result_blob(context, arr, pp->nUsed,
            pp->bStatic ? SQLITE_TRANSIENT : sqlite3_free);
    }
    // jsonReset(pp);
}

SQLMATH_FNC static void sql_matrix2d_concat_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    UNUSED(argc);
    // pp - init
    JsonString *pp =
        (JsonString *) sqlite3_aggregate_context(context, sizeof(*pp));
    if (pp == NULL) {
        return;
    }
    // pp - jsonInit
    if (pp->zBuf == 0) {
        jsonInit(pp, context);
        jsonVectorDoubleAppend(pp, 0);
        jsonVectorDoubleAppend(pp, (double) argc);
    }
    // pp - append double
    for (int ii = 0; ii < argc; ii += 1) {
        jsonVectorDoubleAppend(pp, sqlite3_value_double(argv[ii]));
    }
}

// SQLMATH_FNC sql_matrix2d_concat_func - end

// SQLMATH_FNC sql_blob_each_func - start
//!! /*
//!! ** This method is called to "rewind" the carray_cursor object back
//!! ** to the first row of output.
//!! */
//!! SQLITE_API int blobEachFilter(
    //!! sqlite3_vtab_cursor * pVtabCursor,
    //!! int idxNum,
    //!! const char *idxStr,
    //!! int argc,
    //!! sqlite3_value ** argv
//!! ) {
    //!! UNUSED(argv);
    //!! UNUSED(idxNum);
    //!! UNUSED(argc);
    //!! UNUSED(idxStr);
    //!! carray_cursor *pCur = (carray_cursor *) pVtabCursor;
    //!! pCur->pPtr = 0;
    //!! pCur->iCnt = 0;
    //!! switch (idxNum) {
    //!! case 1:
        //!! carray_bind * pBind = sqlite3_value_pointer(argv[0],
        //!! "carray-bind");
        //!! if (pBind == 0)
            //!! break;
        //!! pCur->pPtr = pBind->aData;
        //!! pCur->iCnt = pBind->nData;
        //!! pCur->eType = pBind->mFlags & 0x03;
        //!! break;
    //!! case 2:
    //!! case 3:
        //!! pCur->pPtr = sqlite3_value_pointer(argv[0], "carray");
        //!! pCur->iCnt = pCur->pPtr ? sqlite3_value_int64(argv[1]) : 0;
        //!! if (idxNum < 3) {
            //!! pCur->eType = CARRAY_INT32;
        //!! } else {
            //!! unsigned char i;
            //!! const char *zType = (const char *) sqlite3_value_text(argv[2]);
            //!! for (i = 0; i < sizeof(azType) / sizeof(azType[0]); i++) {
                //!! if (sqlite3_stricmp(zType, azType[i]) == 0)
                    //!! break;
            //!! }
            //!! if (i >= sizeof(azType) / sizeof(azType[0])) {
                //!! pVtabCursor->pVtab->zErrMsg =
                    //!! sqlite3_mprintf("unknown datatype: %Q", zType);
                //!! return SQLITE_ERROR;
            //!! } else {
                //!! pCur->eType = i;
            //!! }
        //!! }
        //!! break;
    //!! }
    //!! pCur->iRowid = 1;
    //!! return SQLITE_OK;
//!! }

/*
** This following structure defines all the methods for the
** carray virtual table.
*/
static sqlite3_module blobEachModule = {
    0,                          /* iVersion */
    0,                          /* xCreate */
    carrayConnect,              /* xConnect */
    carrayBestIndex,            /* xBestIndex */
    carrayDisconnect,           /* xDisconnect */
    0,                          /* xDestroy */
    carrayOpen,                 /* xOpen - open a cursor */
    carrayClose,                /* xClose - close a cursor */
    carrayFilter,               /* xFilter - configure scan constraints */
    carrayNext,                 /* xNext - advance a cursor */
    carrayEof,                  /* xEof - check for end of scan */
    carrayColumn,               /* xColumn - read data */
    carrayRowid,                /* xRowid - read data */
    0,                          /* xUpdate */
    0,                          /* xBegin */
    0,                          /* xSync */
    0,                          /* xCommit */
    0,                          /* xRollback */
    0,                          /* xFindMethod */
    0,                          /* xRename */
};

// SQLMATH_FNC sql_blob_each_func - end

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
        if (zBuf == 0) {
            sqlite3_result_error_nomem(context);
            return;
        }
        rr = strtod(zBuf, NULL);
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
    assert(argc == 1);
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

SQLMATH_FNC static void sql_throwerror_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return sqlite3_result_error_code(argv[0])
    UNUSED(argc);
    assert(argc == 1);
    // declare var
    int errcode = sqlite3_value_int(argv[0]);
    switch (errcode) {
    case SQLITE_DONE:
    case SQLITE_ROW:
        sqlite3_result_error_code(context, 2);
        break;
    case SQLITE_NOMEM:
        sqlite3_result_error_nomem(context);
        break;
    default:
        sqlite3_result_error_code(context, errcode);
    }
}


// SQLMATH_FNC  sql_tobase64_func - start
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
    text = ALLOCM(MAX(bb, 4));
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

SQLMATH_FNC static void sql_tobase64_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to base64-encoded-text
    UNUSED(argc);
    assert(argc == 1);
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
    sqlite3_result_text(context, (const char *) text, nn, ALLOCF);
}

// SQLMATH_FNC  sql_tobase64_func - end

SQLMATH_FNC static void sql_tostring_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to text
    UNUSED(argc);
    assert(argc == 1);
    sqlite3_result_text(context, (const char *) sqlite3_value_text(argv[0]),
        -1, SQLITE_TRANSIENT);
}


// file sqlmath_ext - misc
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

SQLMATH_API void jsonVectorDoubleAppend(
    JsonString * pp,
    double val
) {
// this function will append double <val> to <pp>
    static const uint64_t NN = 8;
    if ((NN + pp->nUsed >= pp->nAlloc) && jsonGrow(pp, NN) != 0) {
        return;
    }
    // pp - append double
    double *arr = (double *) &pp->zBuf[pp->nUsed];
    *arr = val;
    pp->nUsed += NN;
}

SQLMATH_API int noop(
) {
// this function will do nothing except return 0
    return 0;
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
    SQLITE3_CREATE_FUNCTION1(blob_copy, 1);
    SQLITE3_CREATE_FUNCTION1(blob_create, 1);
    SQLITE3_CREATE_FUNCTION1(castrealorzero, 1);
    SQLITE3_CREATE_FUNCTION1(casttextorempty, 1);
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(jenks, 4);
    SQLITE3_CREATE_FUNCTION1(marginoferror95, 2);
    SQLITE3_CREATE_FUNCTION1(roundorzero, 2);
    SQLITE3_CREATE_FUNCTION1(sign, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION1(tobase64, 1);
    SQLITE3_CREATE_FUNCTION1(tostring, 1);
    SQLITE3_CREATE_FUNCTION2(kthpercentile, 2);
    SQLITE3_CREATE_FUNCTION2(matrix2d_concat, -1);
    errcode =
        sqlite3_create_function(db, "random1", 0,
        SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, sql_random1_func, NULL, NULL);
    SQLMATH_IS_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_create_module(db, "blob_each", &blobEachModule, 0);
    SQLMATH_IS_OK_OR_RETURN_RC(errcode);
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

#define NAPI_VERSION 8

static int dbCount = 0;


// file sqlmath_napi - assert
// this function will if <cond> is falsy, terminate process with <msg>
#define NAPI_ASSERT_FATAL(cond, msg) \
    if (!(cond)) { \
        napi_fatal_error(__func__, NAPI_AUTO_LENGTH , msg, NAPI_AUTO_LENGTH); \
    }

// this function will assert <errcode> == napi_ok in <env>
#define NAPI_ASSERT_OK() \
    if (0 != napiAssertOk(env, __func__, __FILE__, __LINE__, errcode)) { \
        return 0; \
    }

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
#define NAPI_JSBATON_CREATE() \
    jsbatonCreate(env, info); if (baton == NULL) {return NULL;}

#define NAPI_JSPROMISE_CREATE(func) \
    __##func(void *data); \
    static void _##func(napi_env env, void *data) { \
        UNUSED(env); \
        __##func(data); \
    } \
    static napi_value func(napi_env env, napi_callback_info info) { \
        return jspromiseCreate(env, info, _##func); \
    } \
    static int __##func(void *data)

static void jsbatonBufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED(env);
    UNUSED(finalize_hint);
    // cleanup baton->bufout[ii]
    ALLOCF(finalize_data);
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
    size_t ii = 1;
    // init argv
    errcode = napi_get_cb_info(env, info, &ii, &argv, NULL, NULL);
    NAPI_ASSERT_OK();
    // init baton
    errcode = napi_get_element(env, argv, 0, (napi_value *) & baton);
    NAPI_ASSERT_OK();
    errcode =
        napi_get_typedarray_info(env, (napi_value) baton, NULL, NULL,
        (void **) &baton, NULL, NULL);
    NAPI_ASSERT_OK();
    // save argv
    baton->result = argv;
    // init bufin
    ii = 0;
    while (ii < JSBATON_ARGC) {
        errcode = napi_get_element(env, baton->result, ii + 1, &argv);
        NAPI_ASSERT_OK();
        errcode = napi_is_dataview(env, argv, &is_dataview);
        NAPI_ASSERT_OK();
        if (is_dataview) {
            errcode =
                napi_get_dataview_info(env, argv, NULL,
                (void **) &baton->bufin[ii], NULL, NULL);
            NAPI_ASSERT_OK();
        }
        ii += 1;
    }
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
    // export baton->argint64 and baton->bufout to baton->result
    while (ii < JSBATON_ARGC) {
        if (baton->bufout[ii] == NULL) {
            errcode =
                napi_create_double(env, (double) baton->argint64[ii], &val);
            NAPI_ASSERT_OK();
        } else {
            // init baton->bufout[ii]
            errcode = napi_create_external_arraybuffer(env,     // napi_env env,
                (void *) baton->bufout[ii],     // void* external_data,
                (size_t) baton->argint64[ii],   // size_t byte_length,
                jsbatonBufferFinalize,  // napi_finalize finalize_cb,
                NULL,           // void* finalize_hint,
                &val);          // napi_value* result
            NAPI_ASSERT_OK();
        }
        errcode = napi_set_element(env, baton->result, ii + 1, val);
        NAPI_ASSERT_OK();
        ii += 1;
    }
    return baton->result;
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

static int __jspromiseResolve(
    napi_env env,
    napi_status errcode,
    void *data
) {
// This function runs on the main thread after `jspromiseExecute` exits.
    NAPI_ASSERT_FATAL(errcode == 0, __func__);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    napi_ref ref = (napi_ref) baton->result;
    uint32_t refcount = 1;
    // dereference result to allow gc
    errcode = napi_reference_unref(env, ref, &refcount);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_reference_unref");
    NAPI_ASSERT_FATAL(refcount == 0, "memory leak");
    errcode = napi_get_reference_value(env, ref, &baton->result);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_get_reference_value");
    errcode = napi_delete_reference(env, ref);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_reference");
    // Resolve or reject the promise associated with the deferred depending on
    // whether the asynchronous action succeeded.
    if (baton->errmsg[0] == 0) {
        // resolve promise with result
        if (jsbatonExport(env, baton) == NULL) {
            return 0;
        }
        errcode = napi_resolve_deferred(env, baton->deferred, baton->result);
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
        errcode = napi_reject_deferred(env, baton->deferred, err);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_reject_deferred");
    }
    // Clean up the work item associated with this run.
    errcode = napi_delete_async_work(env, baton->work);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_async_work");
    // Set both values to NULL so JavaScript can order a new run of the thread.
    baton->work = NULL;
    baton->deferred = NULL;
    return 0;
}

static void jspromiseResolve(
    napi_env env,
    napi_status errcode,
    void *data
) {
// This function runs on the main thread after `jspromiseExecute` exits.
    __jspromiseResolve(env, errcode, data);
}

static napi_value jspromiseCreate(
    napi_env env,
    napi_callback_info info,
    napi_async_execute_callback jspromiseExecute
) {
// Create a deferred promise and an async queue work item.
    // init baton
    Jsbaton *baton = NAPI_JSBATON_CREATE();
    // declare var
    char buf[SIZEOF_MESSAGE_DEFAULT] = { 0 };
    int errcode = 0;
    napi_value promise = 0;
    napi_value async_resource_name =
        jsstringCreate(env, "Node-API Deferred Promise from Async Work Item");
    // reference result to prevent gc
    errcode = napi_create_reference(env,        // napi_env env
        baton->result,          // napi_value value
        1,                      // uint32_t initial_refcount
        (napi_ref *) & baton->result);  // napi_ref* result
    NAPI_ASSERT_OK();
    // Ensure that no work is currently in progress.
    if (baton->work != NULL) {
        napi_throw_error(env, NULL, sqlmathSnprintfTrace(buf, "",
                "Only one work item must exist at a time", __func__, __FILE__,
                __LINE__));
        return NULL;
    }
    // Create a deferred promise which we will resolve at the completion of the
    // work.
    errcode = napi_create_promise(env, &(baton->deferred), &promise);
    NAPI_ASSERT_OK();
    // Create an async work item, passing in the addon data, which will give the
    // worker thread access to the above-created deferred promise.
    errcode = napi_create_async_work(env,       // napi_env env,
        NULL,                   // napi_value async_resource,
        async_resource_name,    // napi_value async_resource_name,
        jspromiseExecute,       // napi_async_execute_callback execute,
        jspromiseResolve,       // napi_async_complete_callback complete,
        baton,                  // void* data,
        &(baton->work));        // napi_async_work* result);
    NAPI_ASSERT_OK();
    // Queue the work item for execution.
    errcode = napi_queue_async_work(env, baton->work);
    NAPI_ASSERT_OK();
    // This causes created `promise` to be returned to JavaScript.
    return promise;
}


// file sqlmath_napi - noop
static void noopAsyncExecute(
    napi_env env,
    void *data
) {
// This function runs on a worker thread. It has no access to the JavaScript.
    // coverage-hack
    noop();
    UNUSED(env);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    UNUSED(baton);
}

static napi_value noopAsync(
    napi_env env,
    napi_callback_info info
) {
// Create a deferred promise and an async queue work item.
    return jspromiseCreate(env, info, noopAsyncExecute);
}

static napi_value noopSync(
    napi_env env,
    napi_callback_info info
) {
// this function will do nothing
    // init baton
    Jsbaton *baton = NAPI_JSBATON_CREATE();
    return jsbatonExport(env, baton);
}


// file sqlmath_napi - dbXxx
static int NAPI_JSPROMISE_CREATE(
    __dbCloseAsync
) {
// int sqlite3_close_v2(sqlite3*);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = sqlite3_close_v2(db);
    if (db != NULL) {
        dbCount -= 1;
        // fprintf(stderr, "\nsqlite - __dbCloseAsync(%d)\n", dbCount);
    }
    SQLMATH_ASSERT_OK();
    // save result
    baton->argint64[0] = (int64_t) (intptr_t) db;
  catch_error:
    return 0;
}

static int NAPI_JSPROMISE_CREATE(
    __dbExecAsync
) {
// This function will run <bufin[1]> in <db> and save any result
// (list of tables containing rows from SELECT/pragma/etc) as serialized
// json-string in <pResult>.
// *INDENT-OFF*
SQLMATH_API int dbExec(sqlite3 *, Jsbaton *);
// *INDENT-ON*
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = dbExec(db, baton);
    SQLMATH_ASSERT_OK();
  catch_error:
    return 0;
}

static int loadOrSaveDb(
    sqlite3 * pInMemory,
    const char *zFilename,
    int isSave
) {
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

static int NAPI_JSPROMISE_CREATE(
    __dbMemoryLoadOrSave
) {
// This function will load/save filename <bufin[0]> to/from <db>
// *INDENT-OFF*
SQLMATH_API int dbMemoryLoadOrSave(sqlite3 *, Jsbaton *);
// *INDENT-ON*
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = loadOrSaveDb(db,  //
        baton->bufin[1],        // filename
        baton->argint64[2]);    // isSave
    SQLMATH_ASSERT_OK();
  catch_error:
    return 0;
}

static int NAPI_JSPROMISE_CREATE(
    __dbOpenAsync
) {
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = NULL;
    // call c-function
    errcode = sqlite3_open_v2(baton->bufin[0],  // filename
        &db,                    // db
        (int) baton->argint64[2],       // flags // NOLINT
        NULL);
    dbCount += 1;
    // fprintf(stderr, "\nsqlite - __dbOpenAsync(%d)\n", dbCount);
    SQLMATH_ASSERT_OK();
    // save result
    baton->argint64[0] = (int64_t) (intptr_t) db;
  catch_error:
    return 0;
}

static int NAPI_JSPROMISE_CREATE(
    __dbTableInsertAsync
) {
// this function will bulk-insert <bufin[1]> to table <bufin[2]>
// *INDENT-OFF*
SQLMATH_API int dbTableInsert(sqlite3 *, Jsbaton *);
// *INDENT-ON*
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = dbTableInsert(db, baton);
    SQLMATH_ASSERT_OK();
  catch_error:
    return 0;
}

static void __dbFinalizer(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED(env);
    UNUSED(finalize_hint);
    // declare var
    int errcode = 0;
    sqlite3 *db = *(sqlite3 **) finalize_data;
    // fprintf(stderr, "\n[napi autoclose db=%p]\n", db);
    // close db
    errcode = sqlite3_close_v2(db);
    if (db != NULL) {
        dbCount -= 1;
        // fprintf(stderr, "\nsqlite - __dbFinalizer(%d)\n", dbCount);
    }
    NAPI_ASSERT_FATAL(errcode == 0, "sqlite3_close_v2");
    // cleanup aDb
    ALLOCF(finalize_data);
}

static napi_value __dbFinalizerCreate(
    napi_env env,
    napi_callback_info info
) {
// this function will create empty finalizer for db
    UNUSED(info);
    // declare var
    int errcode = 0;
    napi_value val = NULL;
    // init aDb
    int64_t *aDb = (int64_t *) ALLOCM(sizeof(int64_t));
    NAPI_ASSERT_FATAL(aDb != NULL, "out of memory");
    errcode = napi_create_external_arraybuffer(env,     // napi_env env,
        (void *) aDb,           // void* external_data,
        sizeof(int64_t),        // size_t byte_length,
        __dbFinalizer,          // napi_finalize finalize_cb,
        NULL,                   // void* finalize_hint,
        &val);                  // napi_value* result
    NAPI_ASSERT_OK();
    return val;
}


// file sqlmath_napi - init
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
#define NAPI_EXPORT_MEMBER(name) \
    {#name, NULL, name, NULL, NULL, NULL, napi_default, NULL}
    // coverage-hack
    noop();
    // declare var
    int errcode = 0;
    const napi_property_descriptor propList[] = {
        NAPI_EXPORT_MEMBER(__dbCloseAsync),
        NAPI_EXPORT_MEMBER(__dbExecAsync),
        NAPI_EXPORT_MEMBER(__dbFinalizerCreate),
        NAPI_EXPORT_MEMBER(__dbMemoryLoadOrSave),
        NAPI_EXPORT_MEMBER(__dbOpenAsync),
        NAPI_EXPORT_MEMBER(__dbTableInsertAsync),
        NAPI_EXPORT_MEMBER(noopAsync),
        NAPI_EXPORT_MEMBER(noopSync),
    };
    errcode = napi_define_properties(env, exports,
        sizeof(propList) / sizeof(napi_property_descriptor), propList);
    NAPI_ASSERT_OK();
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_sqlmath_init)
#endif                          // SQLMATH_NAPI
/*
file sqlmath_napi - end
*/
