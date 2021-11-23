// copyright nobody
// LINT_C_FILE


/*
file sqlmath_h - start
*/
#ifndef SQLMATH_H
#define SQLMATH_H
// header
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// header2
#ifdef WIN32
#include <windows.h>
#endif
#include <sqlite3.h>
// define
#define ALLOCC calloc
#define ALLOCF free
#define ALLOCM malloc
inline void *ALLOCR(
    void *ptr,
    size_t size
) {
    // handle null-case
    if (ptr == NULL) {
        return NULL;
    }
    void *ptr2 = realloc(ptr, size);
    // if nomem, cleanup old ptr
    if (ptr2 == NULL) {
        free(ptr);
    }
    return ptr2;
}

#define JS_MAX_SAFE_INTEGER 0x1fffffffffffff
#define JS_MIN_SAFE_INTEGER -0x1fffffffffffff
#define MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
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
#define SQLITE_ERROR_NOMEM2 0x10001
#define SQLITE_ERROR_TOOBIG2 0x10002
#define SQLITE_MAX_LENGTH2 1000000000
#define SQLITE_RESPONSETYPE_LAST_MATRIX_DOUBLE 2
#define SQLITE_RESPONSETYPE_LAST_VALUE 1
#define SQLMATH_API
#define SWAP(aa, bb) tmp = (aa); (aa)=(bb); (bb) = tmp
#define UNUSED(x) (void)(x)
// define2
/*
** A macro to hint to the compiler that a function should not be
** inlined.
*/
#if defined(__GNUC__)
#  define NOINLINE  __attribute__((noinline))
#elif defined(_MSC_VER) && _MSC_VER >= 1310
#  define NOINLINE  __declspec(noinline)
#else
#  define NOINLINE
#endif


/*
file sqlmath_h_struct
*/
#define JSBATON_ARGC 16
typedef struct napi_async_work__ *napi_async_work;
typedef struct napi_deferred__ *napi_deferred;
typedef struct napi_value__ *napi_value;
typedef struct BindElem {
    const char *buf;
    char *key;
    int buflen;
    char datatype;
} BindElem;
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


/*
file sqlmath_h_assert
*/
#define ASSERT_SQLITE_CONTEXT_OK(context, errcode) \
    if (!IS_SQLITE_OK(errcode)) { \
        sqlite3_result_error_code(context, errcode); \
        return; \
    }

// this function will exec <sql> and if <errcode> is not ok,
// throw <baton->errmsg>
#define ASSERT_SQLITE_EXEC(baton, db, sql, errcode) \
    errcode = sqlite3_exec(db, sql, NULL, NULL, NULL); \
    ASSERT_SQLITE_OK(baton, db, errcode);

// this function will if <errcode> is not ok, throw <baton->errmsg>
#define ASSERT_SQLITE_OK(baton, db, errcode) \
    if (!IS_SQLITE_OK(errcode)) { \
        assertSqliteOk(baton, db, errcode, __func__, __FILE__, __LINE__); \
        goto catch_error; \
    }

// this function will return if <errcode> is ok or not
#define IS_SQLITE_OK(errcode) \
    (errcode == 0 || errcode == SQLITE_ROW || errcode == SQLITE_DONE)

static const char *__snprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
) {
// this function will write <errmsg> to <buf> with additional trace-info
    if (snprintf(buf,           // NO_LINT
            SIZEOF_MESSAGE_DEFAULT, "%s%s\n    at %s (%s:%d)", prefix, errmsg,
            func, file, line) < 0) {
        abort();
    }
    return (const char *) buf;
}

static NOINLINE int assertSqliteOk(
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
        return errcode;
    }
    if (baton == NULL || baton->errmsg[0] != '\x00') {
        return errcode;
    }
    switch (errcode) {
    case SQLITE_ERROR_DATATYPE_INVALID:
        __snprintfTrace(baton->errmsg, "sqlite - ", "invalid datatype", func,
            file, line);
        return errcode;
    case SQLITE_ERROR_NOMEM2:
        __snprintfTrace(baton->errmsg, "sqlite - ", "out of memory", func,
            file, line);
        return errcode;
    case SQLITE_ERROR_TOOBIG2:
        __snprintfTrace(baton->errmsg, "sqlite - ", "string or blob too big",
            func, file, line);
        return errcode;
    default:
        __snprintfTrace(baton->errmsg, "sqlite - ",
            db == NULL ? sqlite3_errstr(errcode) : sqlite3_errmsg(db), func,
            file, line);
        return errcode;
    }
}

inline static int doubleCompare(
    const void *aa,
    const void *bb
) {
// this function will compare aa with bb
    const double cc = *(double *) aa - *(double *) bb;
    return cc == 0 ? 0 : cc > 0 ? 1 : -1;
}

inline static double doubleMax(
    double aa,
    double bb
) {
// this function will return max of aa, bb
    return aa < bb ? bb : aa;
}

inline static double doubleMin(
    double aa,
    double bb
) {
// this function will return min of aa, bb
    return aa > bb ? bb : aa;
}

inline static int noop(
) {
// this function will do nothing except return 0
    return 0;
}
#endif                          // SQLMATH_H
/*
file sqlmath_h - end
*/


/*
file sqlmath_c - start
*/
#ifdef SQLMATH_C
// header
#include <sqlite3ext.h>
static const sqlite3_api_routines *sqlite3_api;


/*
file sqlmath_c_base
*/
static char *base64Encode(
    const unsigned char *blob,
    int *nn
) {
// this function will base64-encode <blob> to <text>
#define base64EncodeCleanup ALLOCF
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


/*
file sqlmath_c_str99
*/
// dynamically growable string
#define STR99_APPEND_CHAR(cc) \
    errcode = str99AppendChar(str99, cc, errcode); \
    if (!IS_SQLITE_OK(errcode)) { goto catch_error; }

#define STR99_APPEND_DOUBLE(ff) \
    errcode = str99AppendDouble(str99, ff, errcode); \
    if (!IS_SQLITE_OK(errcode)) { goto catch_error; }

#define STR99_APPEND_JSON(zz, nn) \
    errcode = str99AppendJson(str99, zz, nn, errcode); \
    if (!IS_SQLITE_OK(errcode)) { goto catch_error; }

#define STR99_APPEND_RAW(zz, nn) \
    errcode = str99AppendRaw(str99, zz, nn, errcode); \
    if (!IS_SQLITE_OK(errcode)) { goto catch_error; }

typedef struct Str99 {
    char *zBuf;                 /* Accumulated string */
    int nAlloced;               /* Space allocated for zBuf[] */
    int nUsed;                  /* Number of used-bytes in zBuf[] */
} Str99;

static NOINLINE int str99AppendRawAfterGrow(
    Str99 * str99,
    const char *zz,
    int nn,
    int errcode
);

static int str99AppendChar(
    Str99 * str99,
    const char cc,
    int errcode
) {
/*
** Append <cc> to <str99->zBuf>.
** Increase the size of the memory allocation for <str99->zBuf> if necessary.
*/
    // write <cc> to <str99->zBuf> if space available
    if (str99->nUsed < str99->nAlloced) {
        str99->zBuf[str99->nUsed] = cc;
        str99->nUsed += 1;
        return errcode;
    }
    // else resize and retry
    return str99AppendRawAfterGrow(str99, &cc, 1, errcode);
}

static int str99AppendDouble(
    Str99 * str99,
    double ff,
    int errcode
) {
/*
** Append <ff> to <str99->zBuf>.
** Increase the size of the memory allocation for <str99->zBuf> if necessary.
*/
    // write <ff> to <str99->zBuf> if space available
    if (str99->nUsed < str99->nAlloced) {
        ((double *) (str99->zBuf + str99->nUsed))[0] = ff;
        str99->nUsed += 8;
        return errcode;
    }
    // else resize and retry
    return str99AppendRawAfterGrow(str99, (char *) (&ff), 8, errcode);
}

static int str99AppendRaw(
    Str99 * str99,
    const char *zz,
    int nn,
    int errcode
) {
/*
** Append <nn> bytes of text from <zz> to <str99->zBuf>.
** Increase the size of the memory allocation for <str99->zBuf> if necessary.
*/
    if (nn == 0) {
        return errcode;
    }
    // write <zz> to <str99->zBuf> if space available
    if (0 < nn && str99->nUsed + nn <= str99->nAlloced) {
        memcpy(str99->zBuf + str99->nUsed, zz, nn);
        str99->nUsed += nn;
        return errcode;
    }
    // else resize and retry
    return str99AppendRawAfterGrow(str99, zz, nn, errcode);
}

static NOINLINE int str99AppendRawAfterGrow(
    Str99 * str99,
    const char *zz,
    int nn,
    int errcode
) {
/*
** Increase the size of the memory allocation for <str99->zBuf>.
*/
    // declare var
    int nAlloc;
    // sanity check
    if (!IS_SQLITE_OK(errcode)) {
        return errcode;
    }
    // check SQLITE_MAX_LENGTH2
    if (nn < 0 || str99->nUsed + nn > SQLITE_MAX_LENGTH2) {
        return SQLITE_ERROR_TOOBIG2;
    }
    // grow str99 exponentially
    nAlloc = str99->nAlloced;
    while (nAlloc < str99->nUsed + nn) {
        nAlloc *= 2;
    }
    if (nAlloc > str99->nAlloced) {
        str99->zBuf = ALLOCR(str99->zBuf, nAlloc);
        if (str99->zBuf == NULL) {
            return SQLITE_ERROR_NOMEM2;
        }
        str99->nAlloced = nAlloc;
    }
    if (zz == NULL) {
        return errcode;
    }
    // recurse
    return str99AppendRaw(str99, zz, nn, errcode);
}

static int str99AppendJson(
    Str99 * str99,
    const char *zz,
    int nn,
    int errcode
) {
/*
** Append <nn> bytes of text from <zz> to <str99->zBuf> with json-encoding.
** Increase the size of the memory allocation for <str99->zBuf> if necessary.
*/
    // declare var
    const char *zz2 = zz + nn;
    // double-quote open
    STR99_APPEND_RAW("\"", 1);
    while (zz < zz2) {
        switch (*zz) {
        case '\x00':
            STR99_APPEND_RAW("\\u0000", 6);
            break;
        case '\x01':
            STR99_APPEND_RAW("\\u0001", 6);
            break;
        case '\x02':
            STR99_APPEND_RAW("\\u0002", 6);
            break;
        case '\x03':
            STR99_APPEND_RAW("\\u0003", 6);
            break;
        case '\x04':
            STR99_APPEND_RAW("\\u0004", 6);
            break;
        case '\x05':
            STR99_APPEND_RAW("\\u0005", 6);
            break;
        case '\x06':
            STR99_APPEND_RAW("\\u0006", 6);
            break;
        case '\x07':
            STR99_APPEND_RAW("\\u0007", 6);
            break;
        case '\x08':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('b');
            break;
        case '\x09':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('t');
            break;
        case '\x0a':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('n');
            break;
        case '\x0b':
            STR99_APPEND_RAW("\\u000b", 6);
            break;
        case '\x0c':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('f');
            break;
        case '\x0d':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('r');
            break;
        case '\x0e':
            STR99_APPEND_RAW("\\u000e", 6);
            break;
        case '\x0f':
            STR99_APPEND_RAW("\\u000f", 6);
            break;
        case '\x10':
            STR99_APPEND_RAW("\\u0010", 6);
            break;
        case '\x11':
            STR99_APPEND_RAW("\\u0011", 6);
            break;
        case '\x12':
            STR99_APPEND_RAW("\\u0012", 6);
            break;
        case '\x13':
            STR99_APPEND_RAW("\\u0013", 6);
            break;
        case '\x14':
            STR99_APPEND_RAW("\\u0014", 6);
            break;
        case '\x15':
            STR99_APPEND_RAW("\\u0015", 6);
            break;
        case '\x16':
            STR99_APPEND_RAW("\\u0016", 6);
            break;
        case '\x17':
            STR99_APPEND_RAW("\\u0017", 6);
            break;
        case '\x18':
            STR99_APPEND_RAW("\\u0018", 6);
            break;
        case '\x19':
            STR99_APPEND_RAW("\\u0019", 6);
            break;
        case '\x1a':
            STR99_APPEND_RAW("\\u001a", 6);
            break;
        case '\x1b':
            STR99_APPEND_RAW("\\u001b", 6);
            break;
        case '\x1c':
            STR99_APPEND_RAW("\\u001c", 6);
            break;
        case '\x1d':
            STR99_APPEND_RAW("\\u001d", 6);
            break;
        case '\x22':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('\"');
            break;
        case '\x5c':
            STR99_APPEND_CHAR('\\');
            STR99_APPEND_CHAR('\\');
            break;
        default:
            STR99_APPEND_CHAR(*zz);
        }
        zz += 1;
    }
    // double-quote close
    STR99_APPEND_RAW("\"", 1);
  catch_error:
    return errcode;
}


/*
file sqlmath_c_dbXxx
*/
SQLMATH_API int dbExec(
    sqlite3 * db,               /* The database on which the SQL executes */
    Jsbaton * baton
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized json-string in <str99>.
#define STR99_APPEND_CHAR2(cc) \
    if (!responseType) { \
        errcode = str99AppendChar(str99, cc, errcode); \
        if (!IS_SQLITE_OK(errcode)) { goto catch_error; } \
    }
    // declare var
    BindElem *bindElem;
    BindElem *bindList = NULL;
    Str99 *str99 = NULL;
    Str99 str0 = { 0 };
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
    int nRow = 0;
    int responseType = (int) baton->argint64[5];
    int64_t *pnShared = baton->argint64 + 6;
    int64_t iTmp = 0;
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    static const char bindPrefix[] = "$:@";
    // printf("\n[errcode=%d, sql=%s]\n", errcode, zSql);
    // mutext enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // init bindList
    bindList = ALLOCC(bindListLength, sizeof(BindElem));
    if (bindListLength > 0 && bindList == NULL) {
        errcode = SQLITE_ERROR_NOMEM2;
        ASSERT_SQLITE_OK(baton, db, errcode);
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
            printf("\n[ii=%d, datatype=%d, len=%d]\n", ii, bindElem->datatype,
                bindElem->buflen);
            errcode = SQLITE_ERROR_DATATYPE_INVALID;
            ASSERT_SQLITE_OK(baton, db, errcode);
        }
        bindElem += 1;
        ii += 1;
    }
    // init str99
    str99 = &str0;
    // will be cleaned up by jsbatonBufferFinalize()
    str99->zBuf = ALLOCM(SIZEOF_MESSAGE_DEFAULT);
    if (str99->zBuf == NULL) {
        errcode = SQLITE_ERROR_NOMEM2;
        ASSERT_SQLITE_OK(baton, db, errcode);
    }
    str99->nAlloced = SIZEOF_MESSAGE_DEFAULT;
    // bracket database [
    STR99_APPEND_CHAR2('[');
    // loop over each table
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        // printf("\n[prepare=%s]\n", zSql);
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
                // printf("\n[errcode=%d, ii=%d, idx=%d, key=%s, datatype=%d,"
                //     " len=%d]\n", errcode, ii, bindIdx, bindElem->key,
                //     bindElem->datatype, bindElem->buflen);
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
                        printf("\n[ii=%d, datatype=%d, len=%d]\n", ii,
                            bindElem->datatype, bindElem->buflen);
                        errcode = SQLITE_ERROR_DATATYPE_INVALID;
                    }
                    // ignore bind-range-error
                    if (errcode != SQLITE_RANGE) {
                        ASSERT_SQLITE_OK(baton, db, errcode);
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
            if (errcode != SQLITE_ROW) {
                sqlite3_finalize(pStmt);
                pStmt = NULL;
                if (errcode == SQLITE_DONE || errcode == SQLITE_OK) {
                    break;
                }
                // printf("\n[errcode=%d, line=%d]\n", errcode, __LINE__);
                goto catch_error;
            }
            // insert row of column-names
            if (nCol == -1) {
                nRow = 0;
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
                // SQLITE_RESPONSETYPE_LAST_MATRIX_DOUBLE - save nCol
                if (responseType == SQLITE_RESPONSETYPE_LAST_MATRIX_DOUBLE) {
                    str99->nUsed = 8;
                    STR99_APPEND_DOUBLE((double) nCol);
                } else {
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
            }
            // bracket row [
            STR99_APPEND_CHAR2(',');
            STR99_APPEND_CHAR2('\n');
            STR99_APPEND_CHAR2('[');
            ii = 0;
            nRow += 1;
            // loop over each column-value
            while (ii < nCol) {
                switch (responseType) {
                case SQLITE_RESPONSETYPE_LAST_VALUE:
                    // export last-value as blob
                    str99->nUsed = 0;
                    errcode =
                        str99AppendRaw(str99, sqlite3_column_blob(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii), errcode);
                    if (!IS_SQLITE_OK(errcode)) {
                        // printf("\n[errcode=%d, line=%d]\n", errcode,
                        //     __LINE__);
                        goto catch_error;
                    }
                    ii += 1;
                    continue;
                case SQLITE_RESPONSETYPE_LAST_MATRIX_DOUBLE:
                    // export last-table as matrix of doubles
                    STR99_APPEND_DOUBLE(sqlite3_column_double(pStmt, ii));
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
    // SQLITE_RESPONSETYPE_LAST_MATRIX_DOUBLE - save nRow
    if (responseType == SQLITE_RESPONSETYPE_LAST_MATRIX_DOUBLE) {
        ((double *) str99->zBuf)[0] = nRow;
    }
    // shrink str99
    str99->nAlloced = str99->nUsed;
    str99->zBuf = (char *) ALLOCR(str99->zBuf, MAX(str99->nAlloced, 1));
    if (str99->zBuf == NULL) {
        errcode = SQLITE_ERROR_NOMEM2;
        ASSERT_SQLITE_OK(baton, db, errcode);
    }
  catch_error:
    // cleanup bindList
    ALLOCF(bindList);
    // cleanup pStmt
    sqlite3_finalize(pStmt);
    // handle errcode
    if (errcode != 0) {
        // cleanup str99
        ALLOCF(str99->zBuf);
        assertSqliteOk(baton, db, errcode, __func__, __FILE__, __LINE__);
        // printf("\n[errcode=%d, errmsg=%s]\n", errcode, baton->errmsg);
        // mutext leave
        sqlite3_mutex_leave(sqlite3_db_mutex(db));
        return errcode;
    }
    // save str99
    baton->argint64[0] = (int64_t) str99->nAlloced;
    baton->bufout[0] = str99->zBuf;
    // mutext leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    return 0;
}

SQLMATH_API int dbTableInsert(
    sqlite3 * db,               /* The database on which the SQL executes */
    Jsbaton * baton
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized json-string in <str99>.
    // declare var
    const char *pp = baton->bufin[3];
    const char *zTmp = NULL;
    char datatype = 0;
    int bTxn = 0;
    int errcode = 0;
    int ii = 0;
    int jj = 0;
    int nCol = (int) baton->argint64[4];
    int nLen = 0;
    int nRow = (int) baton->argint64[5];
    sqlite3_stmt *pStmt = NULL;
    // mutext enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // create temp table
    ASSERT_SQLITE_EXEC(baton, db, baton->bufin[1], errcode);
    // begin transaction
    ASSERT_SQLITE_EXEC(baton, db, "BEGIN TRANSACTION;", errcode);
    bTxn = 1;
    // init pStmt
    errcode = sqlite3_prepare_v2(db, baton->bufin[2], -1, &pStmt, &zTmp);
    // printf("\n[errcode=%d, nCol=%d, nRow=%d]\n", errcode, nCol, nRow);
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
                errcode = sqlite3_bind_blob(pStmt, jj, pp + 8, nLen,
                    SQLITE_TRANSIENT);
                ASSERT_SQLITE_OK(baton, db, errcode);
                pp += 8 + nLen;
                break;
            case SQLITE_DATATYPE_FLOAT:
                errcode = sqlite3_bind_double(pStmt, jj, *(double *) pp);
                ASSERT_SQLITE_OK(baton, db, errcode);
                pp += 8;
                break;
            case SQLITE_DATATYPE_INTEGER:
                errcode = sqlite3_bind_int64(pStmt, jj, *(int64_t *) pp);
                ASSERT_SQLITE_OK(baton, db, errcode);
                pp += 8;
                break;
            case SQLITE_DATATYPE_INTEGER_0:
                errcode = sqlite3_bind_int(pStmt, jj, 0);
                ASSERT_SQLITE_OK(baton, db, errcode);
                break;
            case SQLITE_DATATYPE_INTEGER_1:
                errcode = sqlite3_bind_int(pStmt, jj, 1);
                ASSERT_SQLITE_OK(baton, db, errcode);
                break;
            case SQLITE_DATATYPE_NULL:
                errcode = sqlite3_bind_null(pStmt, jj);
                ASSERT_SQLITE_OK(baton, db, errcode);
                break;
            case SQLITE_DATATYPE_TEXT_0:
                errcode = sqlite3_bind_text(pStmt, jj, "", 0, SQLITE_STATIC);
                ASSERT_SQLITE_OK(baton, db, errcode);
                break;
            case SQLITE_DATATYPE_TEXT:
                nLen = *(int *) pp;
                errcode = sqlite3_bind_text(pStmt, jj, pp + 8, nLen,
                    SQLITE_TRANSIENT);
                ASSERT_SQLITE_OK(baton, db, errcode);
                pp += 8 + nLen;
                break;
            default:
                printf("\n[ii=%d, jj=%d, datatype=%d, len=%d]\n",
                    ii, jj, datatype, *(int *) pp);
                errcode = SQLITE_ERROR_DATATYPE_INVALID;
                ASSERT_SQLITE_OK(baton, db, errcode);
            }
            jj += 1;
        }
        errcode = sqlite3_step(pStmt);
        ASSERT_SQLITE_OK(baton, db, errcode);
        errcode = sqlite3_clear_bindings(pStmt);
        ASSERT_SQLITE_OK(baton, db, errcode);
        errcode = sqlite3_reset(pStmt);
        ASSERT_SQLITE_OK(baton, db, errcode);
        ii += 1;
    }
  catch_error:
    // end transaction
    if (bTxn) {
        bTxn = 0;
        ASSERT_SQLITE_EXEC(baton, db, "END TRANSACTION;", errcode);
    }
    // cleanup pStmt
    sqlite3_finalize(pStmt);
    // mutext leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    return errcode;
}


/*
file sqlmath_c_func
*/
static void sql_cot_func(
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

static void sql_coth_func(
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

static void sql_sign_func(
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

static void sql_throwerror_func(
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
    case SQLITE_TOOBIG:
        sqlite3_result_error_toobig(context);
        break;
    default:
        sqlite3_result_error_code(context, errcode);
    }
}

static void sql_tobase64_func(
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
    if (text == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    sqlite3_result_text(context, (const char *) text, nn,
        base64EncodeCleanup);
}

static void sql_tostring_func(
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


/*
file sqlmath_c_init
*/
static int sqlite3_sqlmath_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql_##func##_func, NULL, NULL); \
    ASSERT_SQLITE_OK(NULL, NULL, errcode)
#define SQLITE3_CREATE_FUNCTION2(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        NULL, sql_##func##_step, sql_##func##_final); \
    ASSERT_SQLITE_OK(NULL, NULL, errcode)
    UNUSED(pzErrMsg);
    // declare var
    int errcode = 0;
    // init sqlite3_api
    sqlite3_api = pApi;
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(sign, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION1(tobase64, 1);
    SQLITE3_CREATE_FUNCTION1(tostring, 1);
  catch_error:
    return errcode;
}
#endif                          // SQLMATH_C
/*
file sqlmath_c - end
*/


/*
file sqlmath_napi - start
*/
#ifdef SQLMATH_NAPI
// header
#define NAPI_VERSION 8
#include <node_api.h>
static int dbCount = 0;


/*
file sqlmath_napi_assert
*/
// printf("\n[napi errcode=%d]\n", errcode);

// this function will if <cond> is falsy, throw <msg> in <env>
#define ASSERT_NAPI(env, cond, msg) \
    if (!(cond)) { \
        char buf[SIZEOF_MESSAGE_DEFAULT] = { 0 }; \
        napi_throw_error(env, NULL, \
            __snprintfTrace(buf, "", msg, __func__, __FILE__, __LINE__)); \
        return 0; \
    }

// this function will if <cond> is falsy, terminate process with <msg>
#define ASSERT_NAPI_FATAL(cond, msg) \
    if (!(cond)) { \
        napi_fatal_error(__func__, NAPI_AUTO_LENGTH , msg, NAPI_AUTO_LENGTH); \
    }

// this function will assert <errcode> == napi_ok in <env>
#define ASSERT_NAPI_OK(env, errcode) \
    if (0 != assertNapiOk(env, __func__, __FILE__, __LINE__, errcode)) { \
        return 0; \
    }

static int assertNapiOk(
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
    ASSERT_NAPI_FATAL(errcode == 0, "napi_get_last_error_info");
    errcode = napi_is_exception_pending(env, &is_exception_pending);
    ASSERT_NAPI_FATAL(errcode == 0, "napi_is_exception_pending");
    // A pending exception takes precedence over any internal error status.
    if (is_exception_pending) {
        errcode = napi_get_and_clear_last_exception(env, &val);
        ASSERT_NAPI_FATAL(errcode == 0, "napi_get_and_clear_last_exception");
        napi_throw(env, val);
        return errcode;
    }
    errcode =
        napi_throw_error(env, NULL, __snprintfTrace(buf, "",
            (info->error_message !=
                NULL ? info->error_message : "error in native code"), func,
            file, line));
    ASSERT_NAPI_FATAL(errcode == 0, "napi_throw_error");
    return errcode;
}


/*
file sqlmath_napi_promise
*/
#define JSBATON_CREATE(env, info) \
    jsbatonCreate(env, info); if (baton == NULL) {return NULL;}

#define JSPROMISE_CREATE(func, env, data) \
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
    ASSERT_NAPI_OK(env, errcode);
    // init baton
    errcode = napi_get_element(env, argv, 0, (napi_value *) & baton);
    ASSERT_NAPI_OK(env, errcode);
    errcode =
        napi_get_typedarray_info(env, (napi_value) baton, NULL, NULL,
        (void **) &baton, NULL, NULL);
    ASSERT_NAPI_OK(env, errcode);
    // save argv
    baton->result = argv;
    // init bufin
    ii = 0;
    while (ii < JSBATON_ARGC) {
        errcode = napi_get_element(env, baton->result, ii + 1, &argv);
        ASSERT_NAPI_OK(env, errcode);
        errcode = napi_is_dataview(env, argv, &is_dataview);
        ASSERT_NAPI_OK(env, errcode);
        if (is_dataview) {
            errcode =
                napi_get_dataview_info(env, argv, NULL,
                (void **) &baton->bufin[ii], NULL, NULL);
            ASSERT_NAPI_OK(env, errcode);
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
            ASSERT_NAPI_OK(env, errcode);
        } else {
            // init baton->bufout[ii]
            errcode = napi_create_external_arraybuffer(env,     // napi_env env,
                (void *) baton->bufout[ii],     // void* external_data,
                (size_t) baton->argint64[ii],   // size_t byte_length,
                jsbatonBufferFinalize,  // napi_finalize finalize_cb,
                NULL,           // void* finalize_hint,
                &val);          // napi_value* result
            ASSERT_NAPI_OK(env, errcode);
        }
        errcode = napi_set_element(env, baton->result, ii + 1, val);
        ASSERT_NAPI_OK(env, errcode);
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
    ASSERT_NAPI_OK(env, errcode);
    return result;
}

static int __jspromiseResolve(
    napi_env env,
    napi_status errcode,
    void *data
) {
// This function runs on the main thread after `jspromiseExecute` exits.
    ASSERT_NAPI_FATAL(errcode == 0, __func__);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    napi_ref ref = (napi_ref) baton->result;
    uint32_t refcount = 1;
    // dereference result to allow gc
    errcode = napi_reference_unref(env, ref, &refcount);
    ASSERT_NAPI_FATAL(errcode == 0, "napi_reference_unref");
    ASSERT_NAPI_FATAL(refcount == 0, "memory leak");
    errcode = napi_get_reference_value(env, ref, &baton->result);
    ASSERT_NAPI_FATAL(errcode == 0, "napi_get_reference_value");
    errcode = napi_delete_reference(env, ref);
    ASSERT_NAPI_FATAL(errcode == 0, "napi_delete_reference");
    // Resolve or reject the promise associated with the deferred depending on
    // whether the asynchronous action succeeded.
    if (baton->errmsg[0] == 0) {
        // resolve promise with result
        if (jsbatonExport(env, baton) == NULL) {
            return 0;
        }
        errcode = napi_resolve_deferred(env, baton->deferred, baton->result);
        ASSERT_NAPI_FATAL(errcode == 0, "napi_resolve_deferred");
    } else {
        // declare var
        napi_value err;
        // create error
        errcode =
            napi_create_error(env, NULL, jsstringCreate(env, baton->errmsg),
            &err);
        ASSERT_NAPI_FATAL(errcode == 0, "napi_create_error");
        // reject promise with error
        errcode = napi_reject_deferred(env, baton->deferred, err);
        ASSERT_NAPI_FATAL(errcode == 0, "napi_reject_deferred");
    }
    // Clean up the work item associated with this run.
    errcode = napi_delete_async_work(env, baton->work);
    ASSERT_NAPI_FATAL(errcode == 0, "napi_delete_async_work");
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
    Jsbaton *baton = JSBATON_CREATE(env, info);
    // declare var
    int errcode = 0;
    napi_value promise = 0;
    napi_value async_resource_name =
        jsstringCreate(env, "Node-API Deferred Promise from Async Work Item");
    // reference result to prevent gc
    errcode = napi_create_reference(env,        // napi_env env
        baton->result,          // napi_value value
        1,                      // uint32_t initial_refcount
        (napi_ref *) & baton->result);  // napi_ref* result
    ASSERT_NAPI_OK(env, errcode);
    // Ensure that no work is currently in progress.
    ASSERT_NAPI(env, baton->work == NULL,
        "Only one work item must exist at a time");
    // Create a deferred promise which we will resolve at the completion of the
    // work.
    errcode = napi_create_promise(env, &(baton->deferred), &promise);
    ASSERT_NAPI_OK(env, errcode);
    // Create an async work item, passing in the addon data, which will give the
    // worker thread access to the above-created deferred promise.
    errcode = napi_create_async_work(env,       // napi_env env,
        NULL,                   // napi_value async_resource,
        async_resource_name,    // napi_value async_resource_name,
        jspromiseExecute,       // napi_async_execute_callback execute,
        jspromiseResolve,       // napi_async_complete_callback complete,
        baton,                  // void* data,
        &(baton->work));        // napi_async_work* result);
    ASSERT_NAPI_OK(env, errcode);
    // Queue the work item for execution.
    errcode = napi_queue_async_work(env, baton->work);
    ASSERT_NAPI_OK(env, errcode);
    // This causes created `promise` to be returned to JavaScript.
    return promise;
}


/*
file sqlmath_napi_db
*/
static int JSPROMISE_CREATE(
    __dbCloseAsync,
    env,
    data
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
    ASSERT_SQLITE_OK(baton, db, errcode);
    // save result
    baton->argint64[0] = (int64_t) (intptr_t) db;
  catch_error:
    return 0;
}

static int JSPROMISE_CREATE(
    __dbExecAsync,
    env,
    data
) {
// This function will run <bufin[1]> in <db> and save any result
// (list of tables containing rows from SELECT/pragma/etc) as serialized
// json-string in <pResult>.
/* *INDENT-OFF* */
SQLMATH_API int dbExec(sqlite3 *, Jsbaton *);
/* *INDENT-ON* */
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = dbExec(db, baton);
    ASSERT_SQLITE_OK(baton, db, errcode);
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

static int JSPROMISE_CREATE(
    __dbMemoryLoadOrSave,
    env,
    data
) {
// This function will load/save filename <bufin[0]> to/from <db>
/* *INDENT-OFF* */
SQLMATH_API int dbMemoryLoadOrSave(sqlite3 *, Jsbaton *);
/* *INDENT-ON* */
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = loadOrSaveDb(db,  //
        baton->bufin[1],        // filename
        baton->argint64[2]);    // isSave
    ASSERT_SQLITE_OK(baton, db, errcode);
  catch_error:
    return 0;
}

static int JSPROMISE_CREATE(
    __dbOpenAsync,
    env,
    data
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
    ASSERT_SQLITE_OK(baton, db, errcode);
    // printf("\n[napi db=%zd]\n", (intptr_t) db);
    // save result
    baton->argint64[0] = (int64_t) (intptr_t) db;
  catch_error:
    return 0;
}

static int JSPROMISE_CREATE(
    __dbTableInsertAsync,
    env,
    data
) {
// this function will bulk-insert <bufin[1]> to table <bufin[2]>
/* *INDENT-OFF* */
SQLMATH_API int dbTableInsert(sqlite3 *, Jsbaton *);
/* *INDENT-ON* */
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->argint64[0];
    // call c-function
    errcode = dbTableInsert(db, baton);
    ASSERT_SQLITE_OK(baton, db, errcode);
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
    // printf("\n[napi finalize_data=%zd]\n", db);
    // printf("\n[napi finalize_hint=%s]\n", (const char *) finalize_hint);
    // close db
    errcode = sqlite3_close_v2(db);
    if (db != NULL) {
        dbCount -= 1;
        // fprintf(stderr, "\nsqlite - __dbFinalizer(%d)\n", dbCount);
    }
    ASSERT_NAPI_FATAL(errcode == 0, "sqlite3_close_v2");
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
    ASSERT_NAPI_FATAL(aDb != NULL, "out of memory");
    errcode = napi_create_external_arraybuffer(env,     // napi_env env,
        (void *) aDb,           // void* external_data,
        sizeof(int64_t),        // size_t byte_length,
        __dbFinalizer,          // napi_finalize finalize_cb,
        NULL,                   // void* finalize_hint,
        &val);                  // napi_value* result
    ASSERT_NAPI_OK(env, errcode);
    return val;
}


/*
file sqlmath_napi_noop
*/
static void noopAsyncExecute(
    napi_env env,
    void *data
) {
// This function runs on a worker thread. It has no access to the JavaScript.
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

/*
static int JSPROMISE_CREATE(noopAsync, env, data) {
// This function runs on a worker thread. It has no access to the JavaScript.
    UNUSED(env);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    UNUSED(baton);
}
*/

static napi_value noopSync(
    napi_env env,
    napi_callback_info info
) {
// this function will do nothing
    // init baton
    Jsbaton *baton = JSBATON_CREATE(env, info);
    return jsbatonExport(env, baton);
}


/*
file sqlmath_napi_init
*/
napi_value napi_module_init(
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
    ASSERT_NAPI_OK(env, errcode);
    return exports;
}

// NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_init)
#endif                          // SQLMATH_NAPI
/*
file sqlmath_napi - end
*/
