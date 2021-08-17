// LINT_C_FILE
/*
 * 2018-04-19
 *
 * The author disclaims copyright to this source code.  In place of
 * a legal notice, here is a blessing:
 *
 *    May you do good and not evil.
 *    May you find forgiveness for yourself and forgive others.
 *    May you share freely, never taking more than you give.
 *
 ************************************************************************
 *
 * This file implements a template virtual-table.
 * Developers can make a copy of this file as a baseline for writing
 * new virtual tables and/or table-valued functions.
 *
 * Steps for writing a new virtual table implementation:
 *
 *     (1)  Make a copy of this file.  Perhaps call it "mynewvtab.c"
 *
 *     (2)  Replace this header comment with something appropriate for
 *          the new virtual table
 *
 *     (3)  Change every occurrence of "sqlmatrix" to some other string
 *          appropriate for the new virtual table.  Ideally, the new string
 *          should be the basename of the source file: "mynewvtab".  Also
 *          globally change "SQLMATRIX" to "MYNEWVTAB".
 *
 *     (4)  Run a test compilation to make sure the unmodified virtual
 *          table works.
 *
 *     (5)  Begin making incremental changes, testing as you go, to evolve
 *          the new virtual table to do what you want it to do.
 *
 * This template is minimal, in the sense that it uses only the required
 * methods on the sqlite3_module object.  As a result, sqlmatrix is
 * a read-only and eponymous-only table.  Those limitation can be removed
 * by adding new methods.
 *
 * This template implements an eponymous-only virtual table with a rowid and
 * two columns named "a" and "b".  The table as 10 rows with fixed integer
 * values. Usage example:
 *
 *     SELECT rowid, a, b FROM sqlmatrix;
 */
//!! amatch.c:  amatchDisconnect,       /* xDestroy */
//!! closure.c:  closureDisconnect,      /* xDestroy */
//!! csv.c:  csvtabDisconnect,        /* xDestroy */
//!! spellfix.c:  spellfix1Destroy,        /* xDestroy - handle DROP TABLE */
//!! vtablog.c:  vtablogDestroy,            /* xDestroy */
//!! zipfile.c:    zipfileDisconnect,         /* xDestroy */
//!! 14390 Aug  9 08:36 vtablog.c
//!! 28997 Aug  9 08:36 csv.c
//!! 30026 Aug  9 08:36 closure.c
//!! 45869 Aug  9 08:36 amatch.c
//!! 64873 Aug  9 08:36 zipfile.c
//!! 103310 Aug  9 08:36 spellfix.c


/* sqlmath.h - start */
/* header */
#include <assert.h>
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
#include <sqlmath.h>


/* typedef */
typedef struct FuncDef FuncDef;
typedef struct sqlite3_stmt Vdbe;
typedef struct sqlite3_value Mem;
typedef struct sqlmatrix_vtab sqlmatrix_vtab;
typedef struct sqlite3_context {
    Mem *pOut;                  /* The return value is stored here */
    FuncDef *pFunc;             /* Pointer to function information */
    Mem *pMem;                  /* Memory cell used to store aggregate ctx */
    Vdbe *pVdbe;                /* The VM that owns this context */
    int iOp;                    /* Instruction number of OP_Function */
    int isError;                /* Error code returned by the function. */
    u8 skipFlag;                /* Skip accumulator loading if true */
    u8 argc;                    /* Number of arguments */
    sqlite3_value *argv[1];     /* Argument set */
} sqlite3_context;
typedef struct sqlite3_value {
/*
** Internally, the vdbe manipulates nearly all SQL values as Mem
** structures. Each Mem struct may cache multiple representations (string,
** integer etc.) of the same value.
*/
    union MemValue {
        double r;               /* Real val used when MEM_Real set in flags */
        i64 i;                  /* Integer val used when MEM_Int set in flags */
        int nZero;              /* Zero padding for MEM_Zero and MEM_Blob */
        const char *zPType;     /* Pointer type MEM_Term|MEM_Subtype|MEM_Null */
        FuncDef *pDef;          /* Used only when flags==MEM_Agg */
    } u;
    u16 flags;                  /* Combo of MEM_Null, MEM_Str, MEM_Dyn, etc. */
    u8 enc;                     /* SQLITE_UTF8,SQLITE_UTF16BE,SQLITE_UTF16LE */
    u8 eSubtype;                /* Subtype for this value */
    int n;                      /* Number of char in string, excluding '\0' */
    char *z;                    /* String or BLOB value */
    /* ShallowCopy only needs to copy the information above */
    char *zMalloc;              /* MEM_Str or MEM_Blob if szMalloc>0 */
    int szMalloc;               /* Size of the zMalloc allocation */
    u32 uTemp;                  /* Trans store serial_type in OP_MakeRecord */
    sqlite3 *db;                /* The associated database connection */
/* *INDENT-OFF* */
    void (*xDel)(void *);      /* Mem.z destructor - only valid if MEM_Dyn */
/* *INDENT-ON* */
#ifdef SQLITE_DEBUG
    Mem *pScopyFrom;            /* This Mem is a shallow copy of pScopyFrom */
    u16 mScopyFlags;            /* flags value immediately after shallow copy */
#endif
} sqlite3_value;
typedef struct sqlmatrix_cursor {
/* sqlmatrix_cursor is a subclass of sqlite3_vtab_cursor which will
 * serve as the underlying representation of a cursor that scans
 * over rows of the result
 */
    sqlite3_vtab_cursor base;   /* Base class - must be first */
    /* Insert new fields here.  For this sqlmatrix we only keep track
     ** of the rowid */
    sqlite3_int64 iRowid;       /* The rowid */
    sqlmatrix_vtab *pXtab;      /* Pointer to vtab */
} sqlmatrix_cursor;
typedef struct sqlmatrix_vtab {
/* sqlmatrix_vtab is a subclass of sqlite3_vtab which is
 * underlying representation of the virtual table
 */
    sqlite3_vtab base;          /* Base class - must be first */
    /* Add new fields here, as necessary */
    double *pMatrix;            /* nRow x nCol matrix */
    int nByte;                  /* number of bytes allocated for matrix */
    int nCol;                   /* number of columns */
    int nRow;                   /* number of rows */
} sqlmatrix_vtab;


/* function */
JsonString *sqlite3_exec_get_tables_json(
    sqlite3 * db,               /* The database on which the SQL executes */
    const char *zSql,           /* The SQL to be executed */
    sqlite3_context * pCtx,
    int *pErrCode
);
int sqlmatrixDisconnect(
    sqlite3_vtab * pVtab
);
// static const int64_t JS_MAX_SAFE_INTEGER = 0x1fffffffffffffll;
// static const int64_t JS_MIN_SAFE_INTEGER = -0x1fffffffffffffll;
/* sqlmath.h - end */







/* sqlmath.c - start */
static unsigned char base64_decode_table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, '>', 0, 0, 0, '?',
    '4', '5', '6', '7', '8', '9', ':', ';',
    '<', '=', 0, 0, 0, 0, 0, 0,
    0, '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06',
    '\x07', '\b', '\t', '\n', '\x0b', '\f', '\r', '\x0e',
    '\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16',
    '\x17', '\x18', '\x19', 0, 0, 0, 0, 0,
    0, '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', ' ',
    '!', '"', '#', '$', '%', '&', '\'', '(',
    ')', '*', '+', ',', '-', '.', '/', '0',
    '1', '2', '3', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char base64_encode_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

static int base64_mod_table[] = { 0, 2, 1 };

unsigned char *base64_decode(
    const unsigned char *input_text,
    int input_length,
    int *output_length
) {
    int ii;
    int jj;
    int nn;
    uint32_t sextet1;
    uint32_t sextet2;
    uint32_t sextet3;
    uint32_t sextet4;
    uint32_t triplet;
    unsigned char *output_blob;
    // Init output_length.
    *output_length = 0;
    // Find input_length with '=' stripped.
    while (1) {
        input_length -= 1;
        if (input_length < 0) {
            return NULL;
        }
        if (input_text[input_length] != '=') {
            break;
        }
    }
    input_length += 1;
    // Find output_length as multiple of 4.
    nn = input_length;
    if (nn % 4 != 0) {
        nn += 4 - (nn % 4);
    }
    // Decrease output_length for each '=' up to 2.
    switch (nn - input_length) {
    case 1:
        nn = nn / 4 * 3 - 1;
        break;
    case 2:
        nn = nn / 4 * 3 - 2;
        break;
    default:
        nn = nn / 4 * 3;
    }
    // Init output_blob.
    output_blob = (unsigned char *) sqlite3_malloc(nn);
    if (output_blob == NULL) {
        return NULL;
    }
    *output_length = nn;
    ii = 0;
    jj = 0;
    while (ii < input_length) {
        sextet1 =
            ii < input_length ? base64_decode_table[input_text[ii]] : 0;
        ii += 1;
        sextet2 =
            ii < input_length ? base64_decode_table[input_text[ii]] : 0;
        ii += 1;
        sextet3 =
            ii < input_length ? base64_decode_table[input_text[ii]] : 0;
        ii += 1;
        sextet4 =
            ii < input_length ? base64_decode_table[input_text[ii]] : 0;
        ii += 1;
        triplet = (sextet1 << 3 * 6)
            + (sextet2 << 2 * 6)
            + (sextet3 << 1 * 6)
            + (sextet4 << 0 * 6);
        if (jj < nn) {
            output_blob[jj] = (triplet >> 2 * 8) & 0xFF;
            jj += 1;
        }
        if (jj < nn) {
            output_blob[jj] = (triplet >> 1 * 8) & 0xFF;
            jj += 1;
        }
        if (jj < nn) {
            output_blob[jj] = (triplet >> 0 * 8) & 0xFF;
            jj += 1;
        }
    }
    return output_blob;
}

unsigned char *base64_encode(
    const unsigned char *input_blob,
    int input_length,
    int *output_length
) {
    int ii;
    int jj;
    int nn;
    uint32_t octet1;
    uint32_t octet2;
    uint32_t octet3;
    uint32_t triplet;
    unsigned char *output_text;
    *output_length = 0;
    nn = 4 * ((input_length + 2) / 3);
    output_text = sqlite3_malloc(nn);
    if (output_text == NULL) {
        return NULL;
    }
    *output_length = nn;
    ii = 0;
    jj = 0;
    while (ii < input_length) {
        octet1 = ii < input_length ? input_blob[ii] : 0;
        ii += 1;
        octet2 = ii < input_length ? input_blob[ii] : 0;
        ii += 1;
        octet3 = ii < input_length ? input_blob[ii] : 0;
        ii += 1;
        triplet = (octet1 << 0x10) + (octet2 << 0x08) + octet3;
        output_text[jj] = base64_encode_table[(triplet >> 3 * 6) & 0x3F];
        jj += 1;
        output_text[jj] = base64_encode_table[(triplet >> 2 * 6) & 0x3F];
        jj += 1;
        output_text[jj] = base64_encode_table[(triplet >> 1 * 6) & 0x3F];
        jj += 1;
        output_text[jj] = base64_encode_table[(triplet >> 0 * 6) & 0x3F];
        jj += 1;
    }
    ii = 0;
    while (ii < base64_mod_table[input_length % 3]) {
        output_text[nn - 1 - ii] = '=';
        ii += 1;
    }
    return output_text;
}

int example_sqlite3_exec_get_tables_json(
    sqlite3 * db
) {
    JsonString *pJson;
    pJson = sqlite3_exec_get_tables_json(db, (" CREATE TABLE tt1 AS"
            " SELECT 101 AS c102, 102 AS c102"
            " UNION ALL"
            " VALUES (201, 202),"
            "        (301, NULL);"
            ""
            " CREATE TABLE tt2 AS"
            " SELECT 401 AS c402, 402 AS c402, 403 AS c403"
            " UNION ALL"
            " VALUES (501, 502.0123, 5030123456789),"
            "        (601, NULL, 603),"
            "        (701, b64decode('0123456789'), b64decode('8J+YgQ'));"
            "" " SELECT * FROM tt1;" " SELECT * FROM tt2;"), NULL, NULL);
    printf("%s\n", pJson->zBuf);
    sqlite3_free(pJson);
    return 0;
}

int example_sqlite3_get_table(
    sqlite3 * db
) {
    char **dbResult;            // It is char ** type, two * signs
    char *errmsg = NULL;
    int ii;
    int jj;
    int nColumn;
    int nRow;
    int rc;
//     sqlite3 *db;
//     rc = sqlite3_open(":memory:", &db);
//     if (rc != SQLITE_OK) {
//         return rc;
//     }
// Database operation code
// Assume that the MyTable_1 table has been created earlier
// Start the query, the incoming dbResult is already char **, there is an & to
// get the address character, and the one passed in becomes char ***
    rc = sqlite3_get_table(db, (" CREATE TABLE tt1 AS"
            " SELECT 101 AS c102, 102 AS c102"
            " UNION ALL"
            " VALUES (201, 202),"
            "        (301, NULL);"
            ""
            " CREATE TABLE tt2 AS"
            " SELECT 401 AS c402, 402 AS c402"
            " UNION ALL"
            " VALUES (501, 502),"
            "        (601, NULL);"
            ""
            " SELECT * FROM tt1;"
            " SELECT * FROM tt2;"), &dbResult, &nRow, &nColumn, &errmsg);
    if (SQLITE_OK == rc) {
// search successful
// As mentioned earlier, the first line of data in the previous dbResult is the
// field name, and the real data starts from the nColumn index
        printf("% d records found\n", nRow);
        for (ii = 0; ii < nRow; ii += 1) {
            printf("The %d record\n", ii);
            for (jj = 0; jj < nColumn; jj += 1) {
                printf("%i %i Field name: %s?> Field value: %s\n", ii, jj,
                    dbResult[jj], dbResult[ii * nColumn + jj]);
// The field values ??of dbResult are continuous, from the 0th index to the
// nColumn-1 index are the field names, starting from the nColumn index,
// followed by the field values, it puts a two-dimensional table (traditional
// rows and columns Notation) expressed in a flat form
            }
            printf("-------\n");
        }
    }
// At this point, regardless of whether the database query is successful, the
// char ** query results are released, using the function provided by sqlite to
// release
    sqlite3_free_table(dbResult);
//     sqlite3_close(db);
    return rc;
}

int example_sqlite3_step(
    sqlite3 * db
) {
    int rc;
    sqlite3_stmt *pStmt;
//     sqlite3 *db;
//     rc = sqlite3_open(":memory:", &db);
//     if (rc != SQLITE_OK) {
//         return rc;
//     }
    rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }
    rc = sqlite3_step(pStmt);
    if (rc == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(pStmt, 0));
    }
    sqlite3_finalize(pStmt);
//     sqlite3_close(db);
    return rc;
}

void base64DecodeFunc(
    sqlite3_context * ctx,
    int argc,
    sqlite3_value ** argv
) {
    const unsigned char *zTmp;
    int nLen;
    assert(argc == 1);
    //!! zTmp = base64_decode(
    //!! (const char *) sqlite3_value_text(argv[0]),
    //!! sqlite3_value_bytes(argv[0]));
    zTmp = sqlite3_value_text(argv[0]);
    nLen = sqlite3_value_bytes(argv[0]);
    zTmp = base64_decode(zTmp, sqlite3_value_bytes(argv[0]), &nLen);
    sqlite3_result_blob64(ctx, (const void *) zTmp, nLen, sqlite3_free);
//!! SQLITE_API int sqlite3_value_bytes(sqlite3_value *pVal){
    //!! return sqlite3ValueBytes(pVal, SQLITE_UTF8);
//!! }
//!! SQLITE_API void sqlite3_result_blob(
    //!! sqlite3_context *pCtx,
    //!! const void *z,
    //!! int n,
    //!! void (*xDel)(void *)
//!! ){
    //!! assert( n>=0 );
    //!! assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
    //!! setResultStrOrError(pCtx, z, n, 0, xDel);
//!! }
}

static void noopfunc2(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** Implementation of the noop() function.
**
** The function returns its argument, unchanged.
*/
    assert(argc == 1);
    sqlite3_result_value(context, argv[0]);
}

JsonString *sqlite3_exec_get_tables_json(
    sqlite3 * db,               /* The database on which the SQL executes */
    const char *zSql,           /* The SQL to be executed */
    sqlite3_context * pCtx,
    int *pErrCode
) {
    JsonString *pJson;
    const char *zSql2;
    const char *zTmp;
    int ii;
    int nCol;
    int nLen;
    int rc = SQLITE_OK;         /* Return code */
    sqlite3_context ctx = { 0 };
    sqlite3_stmt *pStmt;        /* The current SQL statement */
    sqlite3_value ctxPout = { 0 };
    if (pCtx == NULL) {
        ctxPout.db = db;
        ctx.pOut = (Mem *) & ctxPout;
        pCtx = &ctx;
    }
    pJson = (JsonString *) sqlite3_malloc(sizeof(struct JsonString));
    jsonInit(pJson, pCtx);
    // bracket database [
    jsonAppendChar(pJson, '[');
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
    // Loop over each table.
    while (rc == SQLITE_OK && zSql[0]) {
        pStmt = NULL;
        rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zSql2);
        assert(rc == SQLITE_OK || pStmt == NULL);
        if (rc != SQLITE_OK) {
            continue;
        }
        if (pStmt == NULL) {
            /* this happens for a comment or white-space */
            zSql = zSql2;
            continue;
        }
        nCol = -1;
        // Loop over each row.
        while (1) {
            rc = sqlite3_step(pStmt);
            if (rc != SQLITE_ROW) {
                rc = sqlite3_finalize(pStmt);
                pStmt = NULL;
                zSql = zSql2;
                //!! while( sqlite3Isspace(zSql[0]) ) zSql++;
                break;
            }
            // Insert row of column-names.
            if (nCol == -1) {
                if (pJson->nUsed > 1) {
                    jsonAppendChar(pJson, ',');
                    jsonAppendChar(pJson, '\n');
                    jsonAppendChar(pJson, '\n');
                }
                // bracket table [
                jsonAppendChar(pJson, '[');
                nCol = sqlite3_column_count(pStmt);
                ii = 0;
                // bracket column [
                jsonAppendChar(pJson, '[');
                // Loop over each column-name.
                while (ii < nCol) {
                    if (ii > 0) {
                        jsonAppendChar(pJson, ',');
                    }
                    zTmp = sqlite3_column_name(pStmt, ii);
                    jsonAppendString(pJson, zTmp, strlen(zTmp));
                    ii += 1;
                }
                // bracket column ]
                jsonAppendChar(pJson, ']');
            }
            jsonAppendChar(pJson, ',');
            jsonAppendChar(pJson, '\n');
            // bracket row [
            jsonAppendChar(pJson, '[');
            ii = 0;
            // Loop over each column-value.
            while (ii < nCol) {
                if (ii > 0) {
                    jsonAppendChar(pJson, ',');
                }
                switch (sqlite3_column_type(pStmt, ii)) {
                    // encode blob as data-uri application/octet-stream
                case SQLITE_BLOB:
                    jsonAppendRaw(pJson,
                        "\"data:application/octet-stream;base64,", 38);
                    zTmp = (const char *) base64_encode(
                        (const unsigned char *) sqlite3_column_text(pStmt,
                            ii), sqlite3_column_bytes(pStmt, ii), &nLen);
                    jsonAppendRaw(pJson, zTmp, nLen);
                    sqlite3_free((void *) zTmp);
                    jsonAppendChar(pJson, '"');
                    break;
                case SQLITE_INTEGER:
                case SQLITE_FLOAT:
                    jsonAppendRaw(pJson,
                        (const char *) sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                case SQLITE_TEXT:
                    zTmp = (const char *) sqlite3_column_text(pStmt, ii);
                    jsonAppendString(pJson,
                        (const char *) sqlite3_column_text(pStmt, ii),
                        sqlite3_column_bytes(pStmt, ii));
                    break;
                default:       /* case SQLITE_NULL: */
                    jsonAppendRaw(pJson, "null", 4);
                    break;
                }
                ii += 1;
            }
            // bracket row ]
            jsonAppendChar(pJson, ']');
            if (pJson->bErr) {
                goto exec_out;
            }
        }
        if (nCol != -1) {
            // bracket table ]
            jsonAppendChar(pJson, ']');
        }
        if (pJson->bErr) {
            goto exec_out;
        }
    }
    // bracket database ]
    jsonAppendChar(pJson, ']');
    jsonAppendChar(pJson, 0);
  exec_out:
    // sync isError = pErrCode = rc
    rc = sqlite3_finalize(pStmt) || rc;
    rc = pJson->pCtx->isError || rc;
    pJson->pCtx->isError = rc;
    if (pErrCode != NULL) {
        *pErrCode = rc;
    }
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
    // cleanup stack-allocated ctx
    if (pJson->pCtx == &ctx) {
        pJson->pCtx = NULL;
    }
    return pJson;
}

SQLITE_API void sqlite3_str_append_jsonescaped(
    sqlite3_str * p,
    const char *z,
    int N
) {
/*
** Append N bytes of text from z to the StrAccum object with json-escaping.
** Increase the size of the memory allocation for StrAccum if necessary.
*/
    char *c = (char *) z;
    char *n = c + N;
    sqlite3_str_appendchar(p, 1, '"');
    while (c < n) {
// https://stackoverflow.com/questions/7724448/simple-json-string-escape-for-c
        switch (*c) {
        case '\x00':
            sqlite3_str_append(p, "\\u0000", 6);
            break;
        case '\x01':
            sqlite3_str_append(p, "\\u0001", 6);
            break;
        case '\x02':
            sqlite3_str_append(p, "\\u0002", 6);
            break;
        case '\x03':
            sqlite3_str_append(p, "\\u0003", 6);
            break;
        case '\x04':
            sqlite3_str_append(p, "\\u0004", 6);
            break;
        case '\x05':
            sqlite3_str_append(p, "\\u0005", 6);
            break;
        case '\x06':
            sqlite3_str_append(p, "\\u0006", 6);
            break;
        case '\x07':
            sqlite3_str_append(p, "\\u0007", 6);
            break;
        case '\x08':
            sqlite3_str_append(p, "\\b", 2);
            break;
        case '\x09':
            sqlite3_str_append(p, "\\t", 2);
            break;
        case '\x0a':
            sqlite3_str_append(p, "\\n", 2);
            break;
        case '\x0b':
            sqlite3_str_append(p, "\\u000b", 6);
            break;
        case '\x0c':
            sqlite3_str_append(p, "\\f", 2);
            break;
        case '\x0d':
            sqlite3_str_append(p, "\\r", 2);
            break;
        case '\x0e':
            sqlite3_str_append(p, "\\u000e", 6);
            break;
        case '\x0f':
            sqlite3_str_append(p, "\\u000f", 6);
            break;
        case '\x10':
            sqlite3_str_append(p, "\\u0010", 6);
            break;
        case '\x11':
            sqlite3_str_append(p, "\\u0011", 6);
            break;
        case '\x12':
            sqlite3_str_append(p, "\\u0012", 6);
            break;
        case '\x13':
            sqlite3_str_append(p, "\\u0013", 6);
            break;
        case '\x14':
            sqlite3_str_append(p, "\\u0014", 6);
            break;
        case '\x15':
            sqlite3_str_append(p, "\\u0015", 6);
            break;
        case '\x16':
            sqlite3_str_append(p, "\\u0016", 6);
            break;
        case '\x17':
            sqlite3_str_append(p, "\\u0017", 6);
            break;
        case '\x18':
            sqlite3_str_append(p, "\\u0018", 6);
            break;
        case '\x19':
            sqlite3_str_append(p, "\\u0019", 6);
            break;
        case '\x1a':
            sqlite3_str_append(p, "\\u001a", 6);
            break;
        case '\x1b':
            sqlite3_str_append(p, "\\u001b", 6);
            break;
        case '\x1c':
            sqlite3_str_append(p, "\\u001c", 6);
            break;
        case '\x1d':
            sqlite3_str_append(p, "\\u001d", 6);
            break;
        case '\x22':
            sqlite3_str_append(p, "\\\"", 2);
            break;
        case '\x5c':
            sqlite3_str_append(p, "\\\\", 2);
            break;
        default:
            sqlite3_str_appendchar(p, 1, *c);
        }
        c += 1;
    }
    sqlite3_str_appendchar(p, 1, '"');
}

int sqlmatrixBestIndex(
/*
 * xBestIndex
 * SQLite will invoke this method one or more times while planning a query
 * that uses the virtual table.  This routine needs to create
 * a query plan for each invocation and compute an estimated cost for that
 * plan.
 */
    sqlite3_vtab * tab,
    sqlite3_index_info * pIdxInfo
) {
    pIdxInfo->estimatedCost = (double) 10;
    pIdxInfo->estimatedRows = 10;
    return SQLITE_OK;
}

int sqlmatrixClose(
    sqlite3_vtab_cursor * cur
) {
/*
 * xClose
 * Destructor for a sqlmatrix_cursor.
 */
    sqlmatrix_cursor *pCur = (sqlmatrix_cursor *) cur;
    sqlite3_free(pCur);
    return SQLITE_OK;
}

int sqlmatrixColumn(
/*
 * xColumn
 * Return values of columns for the row at which the sqlmatrix_cursor
 * is currently pointing.
 */
    sqlite3_vtab_cursor * cur,  /* The cursor */
    sqlite3_context * ctx,      /* First argument to sqlite3_result_...() */
    int i                       /* Which column to return */
) {
    sqlmatrix_cursor *pCur = (sqlmatrix_cursor *) cur;
    sqlmatrix_vtab *p = pCur->pXtab;
    switch (i - p->nCol) {
    case 0:
        sqlite3_result_pointer(ctx, p, "sqlmatrix", 0);
        break;
    case 1:
        sqlite3_result_double(ctx, p->nCol);
        break;
    case 2:
        sqlite3_result_double(ctx, p->nRow);
        break;
    default:
        sqlite3_result_double(ctx,
            p->pMatrix[(pCur->iRowid - 1) * p->nCol + i]);
    }
    return SQLITE_OK;
}

int sqlmatrixConnect(
/*
 * xConnect
 * The sqlmatrixConnect() method is invoked to create a new
 * template virtual table.
 *
 * Think of this routine as the constructor for sqlmatrix_vtab objects.
 *
 * All this routine needs to do is:
 *
 *    (1) Allocate the sqlmatrix_vtab object and initialize all fields.
 *
 *    (2) Tell SQLite (via the sqlite3_declare_vtab() interface) what the
 *        result set of queries against the virtual table will look like.
 */
    sqlite3 * db,
    void *pAux,
    int argc,
    const char *const *argv,
    sqlite3_vtab ** ppVtab,
    char **pzErr
) {
    char zCreateTable[15000] = { 0 };
    int ii;
    int jj;
    int nCol;
    int rc;
    sqlmatrix_vtab *pNew;
/*
 *   argv[0]    -> module name  ("approximate_match")
 *   argv[1]    -> database name
 *   argv[2]    -> table name
 *   argv[3...] -> arguments
 */
    // CREATE VIRTUAL TABLE aa USING SQLMATRIX(2);
    nCol = atoi(argv[3]);
    if (!(0 < nCol && nCol <= 2000)) {
        *pzErr =
            sqlite3_mprintf
            ("CREATE TABLE sqlmatrix() is not allowed with column size %i",
            nCol);
        return SQLITE_ERROR;
    }
    ii = 0;
    jj = 0;
    while (ii <= nCol) {
        jj += (ii == 0
            ? snprintf(zCreateTable + jj, 15000ll - jj,
                "CREATE TABLE %s(", argv[2])
            : ii < nCol ? snprintf(&zCreateTable[jj], 15000ll - jj,
                "c%i,", ii)
            : snprintf(&zCreateTable[jj], 15000ll - jj,
                "c%i," "ptr INTEGER HIDDEN,"
                "cols INTEGER HIDDEN," "rows INTEGER HIDDEN)", ii));
        if (!(0 <= jj && jj < 15000)) {
            break;
        }
        ii += 1;
    }
    if (0 < jj && jj < 15000) {
        zCreateTable[jj - 1] = ')';
        zCreateTable[jj] = 0;
    }
    rc = sqlite3_declare_vtab(db, zCreateTable);
    /* For convenience, define symbolic names for the index to each column. */
    if (rc != SQLITE_OK) {
        *pzErr = sqlite3_mprintf(zCreateTable);
        return rc;
    }
    pNew = sqlite3_malloc(sizeof(*pNew));
    if (pNew == NULL) {
        return SQLITE_NOMEM;
    }
    memset(pNew, 0, sizeof(*pNew));
    *ppVtab = (sqlite3_vtab *) pNew;
    pNew->nCol = nCol;
    pNew->nRow = 0;
    pNew->nByte = 256 * nCol * sizeof(double);
    pNew->pMatrix = sqlite3_malloc(pNew->nByte);
    if (pNew->pMatrix == NULL) {
        return SQLITE_NOMEM;
    }
    return rc;
}

int sqlmatrixCreate(
/*
 * xCreate
 * The xConnect and xCreate methods do the same thing, but they must be
 * different so that the virtual table is not an eponymous virtual table.
 */
    sqlite3 * db,
    void *pAux,
    int argc,
    const char *const *argv,
    sqlite3_vtab ** ppVtab,
    char **pzErr
) {
    return sqlmatrixConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

int sqlmatrixDisconnect(
    sqlite3_vtab * pVtab
) {
/*
 * xDisconnect
 * This method is the destructor for sqlmatrix_vtab objects.
 */
    sqlmatrix_vtab *p = (sqlmatrix_vtab *) pVtab;
    if (p == NULL) {
        return SQLITE_OK;
    }
    sqlite3_free(p->pMatrix);
    memset(p, 0, sizeof(*p));
    sqlite3_free(p);
    return SQLITE_OK;
}

int sqlmatrixEof(
    sqlite3_vtab_cursor * cur
) {
/*
 * xEof
 * Return TRUE if the cursor has been moved off of the last
 * row of output.
 */
    sqlmatrix_cursor *pCur = (sqlmatrix_cursor *) cur;
    return pCur->iRowid > pCur->pXtab->nRow;
}

int sqlmatrixFilter(
/*
 * xFilter
 * This method is called to "rewind" the sqlmatrix_cursor object back
 * to the first row of output.  This method is always called at least
 * once prior to any call to sqlmatrixColumn() or sqlmatrixRowid() or
 * sqlmatrixEof().
 */
    sqlite3_vtab_cursor * pVtabCursor,
    int idxNum,
    const char *idxStr,
    int argc,
    sqlite3_value ** argv
) {
    sqlmatrix_cursor *pCur = (sqlmatrix_cursor *) pVtabCursor;
    pCur->iRowid = 1;
    pCur->pXtab = (sqlmatrix_vtab *) pVtabCursor->pVtab;
    return SQLITE_OK;
}

int sqlmatrixNext(
    sqlite3_vtab_cursor * cur
) {
/*
 * xNext
 * Advance a sqlmatrix_cursor to its next row of output.
 */
    sqlmatrix_cursor *pCur = (sqlmatrix_cursor *) cur;
    pCur->iRowid++;
    return SQLITE_OK;
}

int sqlmatrixOpen(
    sqlite3_vtab * p,
    sqlite3_vtab_cursor ** ppCursor
) {
/*
 * xOpen
 * Constructor for a new sqlmatrix_cursor object.
 */
    sqlmatrix_cursor *pCur;
    pCur = sqlite3_malloc(sizeof(*pCur));
    if (pCur == NULL) {
        return SQLITE_NOMEM;
    }
    memset(pCur, 0, sizeof(*pCur));
    *ppCursor = &pCur->base;
    return SQLITE_OK;
}

int sqlmatrixRowid(
    sqlite3_vtab_cursor * cur,
    sqlite_int64 * pRowid
) {
/*
 * xRowid
 * Return the rowid for the current row.  In this implementation, the
 * rowid is the same as the output value.
 */
    sqlmatrix_cursor *pCur = (sqlmatrix_cursor *) cur;
    *pRowid = pCur->iRowid;
    return SQLITE_OK;
}

int sqlmatrixUpdate(
/*
 * xUpdate
 * This implementation disallows DELETE and UPDATE.  The only thing
 * allowed is INSERT.
 */
    sqlite3_vtab * pVtab,
    int argc,
    sqlite3_value ** argv,
    sqlite_int64 * pRowid
) {
    double *pRow;
    int ii;
    sqlmatrix_vtab *p = (sqlmatrix_vtab *) pVtab;
    // https://www.sqlite.org/vtab.html#xupdate
    /* DELETE row */
    if (argc == 1) {
        pVtab->zErrMsg =
            sqlite3_mprintf("DELETE row from sqlmatrix() is not allowed");
        return SQLITE_ERROR;
    }
    /* INSERT row */
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        p->nRow += 1;
        *pRowid = p->nRow;
        while (p->nRow * p->nCol >= p->nByte) {
            p->nByte *= 2;
            p->pMatrix = sqlite3_realloc(p->pMatrix, p->nByte);
            if (p->pMatrix == NULL) {
                sqlmatrixDisconnect((sqlite3_vtab *) p);
                return SQLITE_NOMEM;
            }
        }
        ii = 2;
        pRow = p->pMatrix + (*pRowid - 1) * p->nCol - ii;
        while (ii < argc) {
            pRow[ii] = sqlite3_value_double(argv[ii]);
            ii += 1;
        }
        return SQLITE_OK;
    }
    /* UPDATE row at rowid argv[0] */
    if (argv[0] == argv[1]) {
        pVtab->zErrMsg =
            sqlite3_mprintf("UPDATE row in sqlmatrix() is not allowed");
        return SQLITE_ERROR;
    }
    /* UPDATE row at rowid argv[0] and change rowid to argv[1] */
    pVtab->zErrMsg =
        sqlite3_mprintf
        ("UPDATE row with rowid change in sqlmatrix() is not allowed");
    return SQLITE_ERROR;
}

sqlite3_module sqlmatrixModule = {
/*
 * This following structure defines all the methods for the
 * virtual table.
 */
    0,                          /* iVersion    */
    sqlmatrixCreate,            /* xCreate     */
    sqlmatrixConnect,           /* xConnect    */
    sqlmatrixBestIndex,         /* xBestIndex  */
    sqlmatrixDisconnect,        /* xDisconnect */
    sqlmatrixDisconnect,        /* xDestroy    */
    sqlmatrixOpen,              /* xOpen       */
    sqlmatrixClose,             /* xClose      */
    sqlmatrixFilter,            /* xFilter     */
    sqlmatrixNext,              /* xNext       */
    sqlmatrixEof,               /* xEof        */
    sqlmatrixColumn,            /* xColumn     */
    sqlmatrixRowid,             /* xRowid      */
    sqlmatrixUpdate,            /* xUpdate     */
    0,                          /* xBegin      */
    0,                          /* xSync       */
    0,                          /* xCommit     */
    0,                          /* xRollback   */
    0,                          /* xFindMethod */
    0,                          /* xRename     */
    0,                          /* xSavepoint  */
    0,                          /* xRelease    */
    0,                          /* xRollbackTo */
    0                           /* xShadowName */
};

int sqlite3_sqlmath_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    // SQLITE_EXTENSION_INIT2(pApi);
    rc = rc || sqlite3_create_module(db, "sqlmatrix", &sqlmatrixModule, 0);
    rc = rc || sqlite3_create_function(db,
        "b64decode",
        1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0, base64DecodeFunc, 0, 0);
    rc = rc || sqlite3_create_function(db, "noop2", 1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0, noopfunc2, 0, 0);
    //!! example_sqlite3_get_table(db);
    //!! example_sqlite3_step(db);
    example_sqlite3_exec_get_tables_json(db);
    return rc;
}

/* sqlmath.c - end */
