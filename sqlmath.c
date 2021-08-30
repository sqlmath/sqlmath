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
//!! #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <sqlite3ext.h>
static const sqlite3_api_routines *sqlite3_api;
// define
#define CFREE free
#define CMALLOC malloc
#define CREALLOC realloc
#define EOF -1
#define SIZEOF_BUFFER_DEFAULT 1024
#define SQLITE_MAX_LENGTH 1000000000
#define SQLMATH_API
#define STR2_NOMEM -1
#define STR2_TOOBIG -2
#define UNUSED(x) (void)(x)
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
file sqlmath_dbExec.c
*/
// dynamically growable string
typedef struct Str2 {
    char *buf;
    int alloced;
    int used;
    int errcode;
} Str2;

static int NOINLINE str2Resize(
    Str2 * str2,
    const char *zz,
    int nn
);

static int str2AppendRaw(
    Str2 * str2,
    const char *zz,
    int nn
) {
/*
** Append <nn> bytes of text from <zz> to <str2->buf>.
** Increase the size of the memory allocation for <str2->buf> if necessary.
*/
    // write <zz> to <str2->buf> if space available
    if (0 <= nn && 0 <= str2->used && str2->used + nn <= str2->alloced) {
        memcpy(str2->buf + str2->used, zz, nn);
        str2->used += nn;
        return 0;
    }
    return str2Resize(str2, zz, nn);
}

static int NOINLINE str2Resize(
    Str2 * str2,
    const char *zz,
    int nn
) {
/*
** Increase the size of the memory allocation for <str2->buf>.
*/
    // declare var
    char *zTmp;
    int nAlloc;
    // sanity check
    if (str2->errcode != SQLITE_OK && str2->errcode != SQLITE_ROW) {
        return str2->errcode;
    }
    // check SQLITE_MAX_LENGTH
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
    zTmp = CREALLOC(str2->buf, nAlloc);
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
    str2.buf = CMALLOC(SIZEOF_BUFFER_DEFAULT);
    if (str2.buf == NULL) {
        str2.errcode = STR2_NOMEM;
        goto label_error;
    }
    str2.alloced = SIZEOF_BUFFER_DEFAULT;
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
    zTmp = (const char *) CREALLOC(str2.buf, str2.used);
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
            CFREE(str2.buf);
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


/*
file sqlmath_blobtable.c
*/
// https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/csv.c
/*
** 2016-05-28
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains the implementation of an SQLite virtual table for
** reading CSV files.
**
** Usage:
**
**    .load ./csv
**    CREATE VIRTUAL TABLE temp.csv USING csv(data=DATA);
**    SELECT * FROM csv;
**
** The columns are named "c1", "c2", "c3", ... by default.  Or the
** application can define its own CREATE TABLE statement using the
** schema= parameter, like this:
**
**    CREATE VIRTUAL TABLE temp.csv2 USING csv(
**       data = "c1,c2\n1,2",
**       schema = "CREATE TABLE x(date,ipaddr,url,referrer,userAgent)"
**    );
**
** Instead of specifying a file, the text of the CSV can be loaded using
** the data= parameter.
**
** If the columns=NN parameter is supplied, then the CSV file is assumed to have
** NN columns.  If both the columns= and schema= parameters are omitted, then
** the number and names of the columns is determined by the first line of
** the CSV input.
*/
#include <assert.h>

/* Max size of the error message in a CsvReader */
#define CSV_MXERR 200

/* An instance of the CSV virtual table */
typedef struct CsvTable {
    sqlite3_vtab base;          /* Base class.  Must be first */
    char *zData;                /* Raw CSV data in lieu of zFilename */
    int iStart;                 /* Offset to start of data in zFilename */
    int nCol;                   /* Number of columns in the CSV file */
} CsvTable;

/* A context object used when read a CSV file. */
typedef struct CsvReader CsvReader;
struct CsvReader {
    CsvTable base;              /* Base class.  Must be first */
    char *zz;                   /* Accumulated text for a field */
    int nn;                     /* Number of bytes in zz */
    int nAlloc;                 /* Space allocated for zz[] */
    int nLine;                  /* Current line number */
    int bNotFirst;              /* True if prior text has been seen */
    int cTerm;                  /* Char that terminated most recent field */
    size_t iIn;                 /* Next unread character in the input buffer */
    size_t nIn;                 /* Number of characters in the input buffer */
    char *zIn;                  /* The input buffer */
    char zErr[CSV_MXERR];       /* Error message */
};

/* A cursor for the CSV virtual table */
typedef struct CsvCursor {
    sqlite3_vtab_cursor base;   /* Base class.  Must be first */
    CsvReader rdr;              /* The CsvReader object */
    char **azVal;               /* Value of the current row */
    int *aLen;                  /* Length of each entry */
    sqlite3_int64 iRowid;       /* The current rowid.  Negative for EOF */
} CsvCursor;

static void csv_reader_init(
    CsvReader * pp
) {
/* Initialize a CsvReader object */
    pp->zz = 0;
    pp->nn = 0;
    pp->nAlloc = 0;
    pp->nLine = 0;
    pp->bNotFirst = 0;
    pp->nIn = 0;
    pp->zIn = 0;
    pp->zErr[0] = 0;
}

static void csv_reader_reset(
    CsvReader * pp
) {
/* Close and reset a CsvReader object */
    sqlite3_free(pp->zz);
    csv_reader_init(pp);
}

static void csv_errmsg(
    CsvReader * pp,
    const char *zFormat,
    ...
) {
/* Report an error on a CsvReader */
    va_list ap;
    va_start(ap, zFormat);
    sqlite3_vsnprintf(CSV_MXERR, pp->zErr, zFormat, ap);
    va_end(ap);
}

static int csv_getc(
/* Return the next character of input.  Return EOF at end of input. */
    CsvReader * pp
) {
    if (pp->iIn >= pp->nIn) {
        return EOF;
    }
    return ((unsigned char *) pp->zIn)[pp->iIn++];
}

static int NOINLINE csv_resize_and_append(
/* Increase the size of pp->zz and append character c to the end.
** Return 0 on success and non-zero if there is an OOM error */
    CsvReader * pp,
    char c
) {
    char *zNew;
    int nNew = pp->nAlloc * 2 + 100;
    zNew = sqlite3_realloc64(pp->zz, nNew);
    if (zNew) {
        pp->zz = zNew;
        pp->nAlloc = nNew;
        pp->zz[pp->nn++] = c;
        return 0;
    } else {
        csv_errmsg(pp, "out of memory");
        return 1;
    }
}

/* Append a single character to the CsvReader.zz[] array.
** Return 0 on success and non-zero if there is an OOM error */
static int csv_append(
    CsvReader * pp,
    char c
) {
    if (pp->nn >= pp->nAlloc - 1)
        return csv_resize_and_append(pp, c);
    pp->zz[pp->nn++] = c;
    return 0;
}

/* Read a single field of CSV text.  Compatible with rfc4180 and extended
** with the option of having a separator other than ",".
**
**   +  Input comes from pp->in.
**   +  Store results in pp->zz of length pp->nn.  Space to hold pp->zz comes
**      from sqlite3_malloc64().
**   +  Keep track of the line number in pp->nLine.
**   +  Store the character that terminates the field in pp->cTerm.  Store
**      EOF on end-of-file.
**
** Return 0 at EOF or on OOM.  On EOF, the pp->cTerm character will have
** been set to EOF.
*/
static char *csv_read_one_field(
    CsvReader * pp
) {
    int c;
    pp->nn = 0;
    c = csv_getc(pp);
    if (c == EOF) {
        pp->cTerm = EOF;
        return 0;
    }
    if (c == '"') {
        int pc;
        int ppc;
        int startLine = pp->nLine;
        pc = ppc = 0;
        while (1) {
            c = csv_getc(pp);
            if (c <= '"' || pc == '"') {
                if (c == '\n')
                    pp->nLine++;
                if (c == '"') {
                    if (pc == '"') {
                        pc = 0;
                        continue;
                    }
                }
                if ((c == ',' && pc == '"')
                    || (c == '\n' && pc == '"')
                    || (c == '\n' && pc == '\r' && ppc == '"')
                    || (c == EOF && pc == '"')
                    ) {
                    while (1) {
                        pp->nn--;
                        if (pp->zz[pp->nn] == '"') {
                            break;
                        }
                    }
                    pp->cTerm = (char) c;
                    break;
                }
                if (pc == '"' && c != '\r') {
                    csv_errmsg(pp, "line %d: unescaped %c character",
                        pp->nLine, '"');
                    break;
                }
                if (c == EOF) {
                    csv_errmsg(pp, "line %d: unterminated %c-quoted field\n",
                        startLine, '"');
                    pp->cTerm = (char) c;
                    break;
                }
            }
            if (csv_append(pp, (char) c))
                return 0;
            ppc = pc;
            pc = c;
        }
    } else {
        /* If this is the first field being parsed and it begins with the
         ** UTF-8 BOM  (0xEF BB BF) then skip the BOM */
        if ((c & 0xff) == 0xef && pp->bNotFirst == 0) {
            csv_append(pp, (char) c);
            c = csv_getc(pp);
            if ((c & 0xff) == 0xbb) {
                csv_append(pp, (char) c);
                c = csv_getc(pp);
                if ((c & 0xff) == 0xbf) {
                    pp->bNotFirst = 1;
                    pp->nn = 0;
                    // recurse
                    return csv_read_one_field(pp);
                }
            }
        }
        while (c > ',' || (c != EOF && c != ',' && c != '\n')) {
            if (csv_append(pp, (char) c))
                return 0;
            c = csv_getc(pp);
        }
        if (c == '\n') {
            pp->nLine++;
            if (pp->nn > 0 && pp->zz[pp->nn - 1] == '\r')
                pp->nn--;
        }
        pp->cTerm = (char) c;
    }
    if (pp->zz)
        pp->zz[pp->nn] = 0;
    pp->bNotFirst = 1;
    return pp->zz;
}

/*
** This method is the destructor fo a CsvTable object.
*/
static int csvtabDisconnect(
    sqlite3_vtab * pVtab
) {
    CsvTable *pp = (CsvTable *) pVtab;
    sqlite3_free(pp->zData);
    sqlite3_free(pp);
    return SQLITE_OK;
}

/* Skip leading whitespace.  Return a pointer to the first non-whitespace
** character, or to the zero terminator if the string has only whitespace */
static const char *csv_skip_whitespace(
    const char *zz
) {
    while (isspace((unsigned char) zz[0]))
        zz++;
    return zz;
}

/* Remove trailing whitespace from the end of string zz[] */
static void csv_trim_whitespace(
    char *zz
) {
    size_t n = strlen(zz);
    while (n > 0 && isspace((unsigned char) zz[n]))
        n--;
    zz[n] = 0;
}

/* Dequote the string */
static void csv_dequote(
    char *zz
) {
    int jj;
    char cQuote = zz[0];
    size_t ii,
     n;

    if (cQuote != '\'' && cQuote != '"')
        return;
    n = strlen(zz);
    if (n < 2 || zz[n - 1] != zz[0])
        return;
    for (ii = 1, jj = 0; ii < n - 1; ii++) {
        if (zz[ii] == cQuote && zz[ii + 1] == cQuote)
            ii++;
        zz[jj++] = zz[ii];
    }
    zz[jj] = 0;
}

/* Check to see if the string is of the form:  "TAG = VALUE" with optional
** whitespace before and around tokens.  If it is, return a pointer to the
** first character of VALUE.  If it is not, return NULL.
*/
static const char *csv_parameter(
    const char *zTag,
    int nTag,
    const char *zz
) {
    zz = csv_skip_whitespace(zz);
    if (strncmp(zTag, zz, nTag) != 0)
        return 0;
    zz = csv_skip_whitespace(zz + nTag);
    if (zz[0] != '=')
        return 0;
    return csv_skip_whitespace(zz + 1);
}

/* Decode a parameter that requires a dequoted string.
**
** Return 1 if the parameter is seen, or 0 if not.  1 is returned
** even if there is an error.  If an error occurs, then an error message
** is left in pp->zErr.  If there are no errors, pp->zErr[0]==0.
*/
static int csv_string_parameter(
    CsvReader * pp,             /* Leave error message here, if there is one */
    const char *zParam,         /* Parameter we are checking for */
    const char *zArg,           /* Raw text of the virtual table argment */
    char **pzVal                /* Write the dequoted string value here */
) {
    const char *zValue;
    zValue = csv_parameter(zParam, (int) strlen(zParam), zArg);
    if (zValue == 0)
        return 0;
    pp->zErr[0] = 0;
    if (*pzVal) {
        csv_errmsg(pp, "more than one '%s' parameter", zParam);
        return 1;
    }
    *pzVal = sqlite3_mprintf("%s", zValue);
    if (*pzVal == 0) {
        csv_errmsg(pp, "out of memory");
        return 1;
    }
    csv_trim_whitespace(*pzVal);
    csv_dequote(*pzVal);
    return 1;
}

/* Return 0 if the argument is false and 1 if it is true.  Return -1 if
** we cannot really tell.
*/
static int csv_boolean(
    const char *zz
) {
    if (sqlite3_stricmp("yes", zz) == 0
        || sqlite3_stricmp("on", zz) == 0
        || sqlite3_stricmp("true", zz) == 0 || (zz[0] == '1' && zz[1] == 0)
        ) {
        return 1;
    }
    if (sqlite3_stricmp("no", zz) == 0
        || sqlite3_stricmp("off", zz) == 0
        || sqlite3_stricmp("false", zz) == 0 || (zz[0] == '0' && zz[1] == 0)
        ) {
        return 0;
    }
    return -1;
}

/* Check to see if the string is of the form:  "TAG = BOOLEAN" or just "TAG".
** If it is, set *pValue to be the value of the boolean ("true" if there is
** not "= BOOLEAN" component) and return non-zero.  If the input string
** does not begin with TAG, return zero.
*/
static int csv_boolean_parameter(
    const char *zTag,           /* Tag we are looking for */
    int nTag,                   /* Size of the tag in bytes */
    const char *zz,             /* Input parameter */
    int *pValue                 /* Write boolean value here */
) {
    int b;
    zz = csv_skip_whitespace(zz);
    if (strncmp(zTag, zz, nTag) != 0)
        return 0;
    zz = csv_skip_whitespace(zz + nTag);
    if (zz[0] == 0) {
        *pValue = 1;
        return 1;
    }
    if (zz[0] != '=')
        return 0;
    zz = csv_skip_whitespace(zz + 1);
    b = csv_boolean(zz);
    if (b >= 0) {
        *pValue = b;
        return 1;
    }
    return 0;
}

/*
** Parameters:
**    filename=FILENAME          Name of file containing CSV content
**    data=TEXT                  Direct CSV content.
**    schema=SCHEMA              Alternative CSV schema.
**    columns=N                  Assume the CSV file contains N columns.
**
** If schema= is omitted, then the columns are named "c0", "c1", "c2",
** and so forth.  If columns=N is omitted, then the file is opened and
** the number of columns in the first row is counted to determine the
** column count. first row is skipped as header.
*/
static int csvtabConnect(
    sqlite3 * db,
    void *pAux,
    int argc,
    const char *const *argv,
    sqlite3_vtab ** ppVtab,
    char **pzErr
) {
    UNUSED(pAux);
    // declare var
    CsvTable *pNew = 0;         /* The CsvTable object to construct */
    int rc = SQLITE_OK;         /* Result code from this routine */
    size_t ii,
     jj;                        /* Loop counters */
    int b;                      /* Value of a boolean parameter */
    int nCol = -99;             /* Value of the columns= parameter */
/* A CSV file reader used to store an error
** message and/or to count the number of columns */
    CsvReader sRdr;
    static const char *azParam[] = {
        "filename", "data", "schema",
    };
    char *azPValue[3];          /* Parameter values */
# define CSV_FILENAME (azPValue[0])
# define CSV_DATA     (azPValue[1])
# define CSV_SCHEMA   (azPValue[2])
    assert(sizeof(azPValue) == sizeof(azParam));
    memset(&sRdr, 0, sizeof(sRdr));
    memset(azPValue, 0, sizeof(azPValue));
    for (ii = 3; ii < argc; ii++) {
        const char *zz = argv[ii];
        const char *zValue;
        for (jj = 0; jj < sizeof(azParam) / sizeof(azParam[0]); jj++) {
            if (csv_string_parameter(&sRdr, azParam[jj], zz, &azPValue[jj]))
                break;
        }
        if (jj < sizeof(azParam) / sizeof(azParam[0])) {
            if (sRdr.zErr[0])
                goto csvtab_connect_error;
        } else if ((zValue = csv_parameter("columns", 7, zz)) != 0) {
            if (nCol > 0) {
                csv_errmsg(&sRdr, "more than one 'columns' parameter");
                goto csvtab_connect_error;
            }
            nCol = atoi(zValue);
            if (nCol <= 0) {
                csv_errmsg(&sRdr, "column= value must be positive");
                goto csvtab_connect_error;
            }
        } else {
            csv_errmsg(&sRdr, "bad parameter: '%s'", zz);
            goto csvtab_connect_error;
        }
    }
    if ((CSV_FILENAME == 0) == (CSV_DATA == 0)) {
        csv_errmsg(&sRdr,
            "must specify either filename= or data= but not both");
        goto csvtab_connect_error;
    }
    //!! csv_reader_open(&sRdr, CSV_DATA);
    sRdr.zIn = (char *) CSV_DATA;
    sRdr.nIn = strlen(CSV_DATA);

    pNew = sqlite3_malloc(sizeof(*pNew));
    *ppVtab = (sqlite3_vtab *) pNew;
    if (pNew == 0)
        goto csvtab_connect_oom;
    memset(pNew, 0, sizeof(*pNew));

    sqlite3_str *pStr = sqlite3_str_new(0);
    char *zSep = "";
    int iCol = 0;
    sqlite3_str_appendf(pStr, "CREATE TABLE x(");
    // read columns from first row
    while (1) {
        char *zz = csv_read_one_field(&sRdr);
        if ((nCol > 0 && iCol < nCol) || nCol < 0) {
            sqlite3_str_appendf(pStr, "%s\"%w\" TEXT", zSep, zz);
            zSep = ",";
            iCol++;
        }
        if (sRdr.cTerm != ',') {
            break;
        }
    }
    if (nCol < 0) {
        nCol = iCol;
    } else {
        while (iCol < nCol) {
            sqlite3_str_appendf(pStr, "%sc%d TEXT", zSep, ++iCol);
            zSep = ",";
        }
    }

    pNew->nCol = nCol;
    sqlite3_str_appendf(pStr, ")");
    CSV_SCHEMA = sqlite3_str_finish(pStr);
    if (CSV_SCHEMA == 0)
        goto csvtab_connect_oom;

    pNew->zData = CSV_DATA;
    CSV_DATA = 0;
    // skip first row of headers
    pNew->iStart = (int) sRdr.iIn;
    csv_reader_reset(&sRdr);
    rc = sqlite3_declare_vtab(db, CSV_SCHEMA);
    if (rc) {
        csv_errmsg(&sRdr, "bad schema: '%s' - %s", CSV_SCHEMA,
            sqlite3_errmsg(db));
        goto csvtab_connect_error;
    }
    for (ii = 0; ii < sizeof(azPValue) / sizeof(azPValue[0]); ii++) {
        sqlite3_free(azPValue[ii]);
    }
    /* Rationale for DIRECTONLY:
     ** An attacker who controls a database schema could use this vtab
     ** to exfiltrate sensitive data from other files in the filesystem.
     ** And, recommended practice is to put all CSV virtual tables in the
     ** TEMP namespace, so they should still be usable from within TEMP
     ** views, so there shouldn't be a serious loss of functionality by
     ** prohibiting the use of this vtab from persistent triggers and views.
     */
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY);
    return SQLITE_OK;

  csvtab_connect_oom:
    rc = SQLITE_NOMEM;
    csv_errmsg(&sRdr, "out of memory");

  csvtab_connect_error:
    if (pNew)
        csvtabDisconnect(&pNew->base);
    for (ii = 0; ii < sizeof(azPValue) / sizeof(azPValue[0]); ii++) {
        sqlite3_free(azPValue[ii]);
    }
    if (sRdr.zErr[0]) {
        sqlite3_free(*pzErr);
        *pzErr = sqlite3_mprintf("%s", sRdr.zErr);
    }
    csv_reader_reset(&sRdr);
    if (rc == SQLITE_OK)
        rc = SQLITE_ERROR;
    return rc;
}

/*
** Reset the current row content held by a CsvCursor.
*/
static void csvtabCursorRowReset(
    CsvCursor * pCur
) {
    CsvTable *pTab = (CsvTable *) pCur->base.pVtab;
    int ii;
    for (ii = 0; ii < pTab->nCol; ii++) {
        sqlite3_free(pCur->azVal[ii]);
        pCur->azVal[ii] = 0;
        pCur->aLen[ii] = 0;
    }
}

/*
** The xConnect and xCreate methods do the same thing, but they must be
** different so that the virtual table is not an eponymous virtual table.
*/
static int csvtabCreate(
    sqlite3 * db,
    void *pAux,
    int argc,
    const char *const *argv,
    sqlite3_vtab ** ppVtab,
    char **pzErr
) {
    return csvtabConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

/*
** Destructor for a CsvCursor.
*/
static int csvtabClose(
    sqlite3_vtab_cursor * cur
) {
    CsvCursor *pCur = (CsvCursor *) cur;
    csvtabCursorRowReset(pCur);
    csv_reader_reset(&pCur->rdr);
    sqlite3_free(cur);
    return SQLITE_OK;
}

/*
** Constructor for a new CsvTable cursor object.
*/
static int csvtabOpen(
    sqlite3_vtab * pp,
    sqlite3_vtab_cursor ** ppCursor
) {
    CsvTable *pTab = (CsvTable *) pp;
    CsvCursor *pCur;
    size_t nByte;
    nByte = sizeof(*pCur) + (sizeof(char *) + sizeof(int)) * pTab->nCol;
    pCur = sqlite3_malloc64(nByte);
    if (pCur == 0)
        return SQLITE_NOMEM;
    memset(pCur, 0, nByte);
    pCur->azVal = (char **) &pCur[1];
    pCur->aLen = (int *) &pCur->azVal[pTab->nCol];
    *ppCursor = &pCur->base;

    //!! csv_reader_open(&pCur->rdr, pTab->zData);
    pCur->rdr.zIn = (char *) pTab->zData;
    pCur->rdr.nIn = strlen(pTab->zData);

    return SQLITE_OK;
}

/*
** Advance a CsvCursor to its next row of input.
** Set the EOF marker if we reach the end of input.
*/
static int csvtabNext(
    sqlite3_vtab_cursor * cur
) {
    CsvCursor *pCur = (CsvCursor *) cur;
    CsvTable *pTab = (CsvTable *) cur->pVtab;
    int ii = 0;
    char *zz;
    while (1) {
        zz = csv_read_one_field(&pCur->rdr);
        if (zz == 0) {
            break;
        }
        if (ii < pTab->nCol) {
            if (pCur->aLen[ii] < pCur->rdr.nn + 1) {
                char *zNew =
                    sqlite3_realloc64(pCur->azVal[ii], pCur->rdr.nn + 1);
/* Transfer error message text from a reader into a CsvTable */
                if (zNew == 0) {
                    csv_errmsg(&pCur->rdr, "out of memory");
                    sqlite3_free(pTab->base.zErrMsg);
                    pTab->base.zErrMsg =
                        sqlite3_mprintf("%s", pCur->rdr.zErr);
                    break;
                }
                pCur->azVal[ii] = zNew;
                pCur->aLen[ii] = pCur->rdr.nn + 1;
            }
            memcpy(pCur->azVal[ii], zz, pCur->rdr.nn + 1);
            ii++;
        }
        if (pCur->rdr.cTerm != ',') {
            break;
        }
    }
    if (zz == 0 || (pCur->rdr.cTerm == EOF && ii < pTab->nCol)) {
        pCur->iRowid = -1;
    } else {
        pCur->iRowid++;
        while (ii < pTab->nCol) {
            sqlite3_free(pCur->azVal[ii]);
            pCur->azVal[ii] = 0;
            pCur->aLen[ii] = 0;
            ii++;
        }
    }
    return SQLITE_OK;
}

/*
** Return values of columns for the row at which the CsvCursor
** is currently pointing.
*/
static int csvtabColumn(
    sqlite3_vtab_cursor * cur,  /* The cursor */
    sqlite3_context * ctx,      /* First argument to sqlite3_result_...() */
    int ii                      /* Which column to return */
) {
    CsvCursor *pCur = (CsvCursor *) cur;
    CsvTable *pTab = (CsvTable *) cur->pVtab;
    if (ii >= 0 && ii < pTab->nCol && pCur->azVal[ii] != 0) {
        sqlite3_result_text(ctx, pCur->azVal[ii], -1, SQLITE_TRANSIENT);
    }
    return SQLITE_OK;
}

/*
** Return the rowid for the current row.
*/
static int csvtabRowid(
    sqlite3_vtab_cursor * cur,
    sqlite_int64 * pRowid
) {
    CsvCursor *pCur = (CsvCursor *) cur;
    *pRowid = pCur->iRowid;
    return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
static int csvtabEof(
    sqlite3_vtab_cursor * cur
) {
    CsvCursor *pCur = (CsvCursor *) cur;
    return pCur->iRowid < 0;
}

/*
** Only a full table scan is supported.  So xFilter simply rewinds to
** the beginning.
*/
static int csvtabFilter(
    sqlite3_vtab_cursor * pVtabCursor,
    int idxNum,
    const char *idxStr,
    int argc,
    sqlite3_value ** argv
) {
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(idxNum);
    UNUSED(idxStr);
    // declare var
    CsvCursor *pCur = (CsvCursor *) pVtabCursor;
    CsvTable *pTab = (CsvTable *) pVtabCursor->pVtab;
    pCur->iRowid = 0;
    pCur->rdr.iIn = pTab->iStart;
    return csvtabNext(pVtabCursor);
}

/*
** Only a forward full table scan is supported.  xBestIndex is mostly
** a no-op.
*/
static int csvtabBestIndex(
    sqlite3_vtab * tab,
    sqlite3_index_info * pIdxInfo
) {
    UNUSED(tab);
    pIdxInfo->estimatedCost = 1000000;
    return SQLITE_OK;
}

static sqlite3_module CsvModule = {
    0,                          /* iVersion */
    csvtabCreate,               /* xCreate */
    csvtabConnect,              /* xConnect */
    csvtabBestIndex,            /* xBestIndex */
    csvtabDisconnect,           /* xDisconnect */
    csvtabDisconnect,           /* xDestroy */
    csvtabOpen,                 /* xOpen - open a cursor */
    csvtabClose,                /* xClose - close a cursor */
    csvtabFilter,               /* xFilter - configure scan constraints */
    csvtabNext,                 /* xNext - advance a cursor */
    csvtabEof,                  /* xEof - check for end of scan */
    csvtabColumn,               /* xColumn - read data */
    csvtabRowid,                /* xRowid - read data */
    0,                          /* xUpdate */
    0,                          /* xBegin */
    0,                          /* xSync */
    0,                          /* xCommit */
    0,                          /* xRollback */
    0,                          /* xFindMethod */
    0,                          /* xRename */
    0,                          /* xSavepoint  */
    0,                          /* xRelease    */
    0,                          /* xRollbackTo */
    0                           /* xShadowName */
};


/*
file sqlite3_sqlmath_init.c
*/
int sqlite3_sqlmath_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
#define ASSERT_SQLITE_OK(errcode) if (errcode != SQLITE_OK) { return errcode; }
    UNUSED(pzErrMsg);
    // declare var
    int errcode = SQLITE_OK;
    // init sqlite3_api
    sqlite3_api = pApi;
    errcode = sqlite3_create_module(db, "csv", &CsvModule, 0);
    ASSERT_SQLITE_OK(errcode);
    return errcode;
}
