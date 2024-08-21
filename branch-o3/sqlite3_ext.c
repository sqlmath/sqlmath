// copyright nobody
// LINT_C_FILE
#define IS_SQLITE_OK_OR_RETURN_RC(rc) if ((rc) != SQLITE_OK) {return (rc);}
#define UNUSED(x) (void)(x)


/*
file zlib.h
*/
#ifdef EMSCRIPTEN
#include "stdio.h"
#define Z_OK 0
int compress(
    unsigned char *,
    unsigned long *,
    const unsigned char *,
    unsigned long
);
int uncompress(
    unsigned char *,
    unsigned long *,
    const unsigned char *,
    unsigned long
);
#else                           // EMSCRIPTEN
#include <zlib.h>
#endif                          // EMSCRIPTEN
/* *INDENT-OFF* */


/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/LICENSE.md"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/compress.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/csv.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/noop.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/regexp.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/series.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/shathree.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/tree/version-3.38.0/contrib/download/extension-functions.c/download/extension-functions.c",
            "url2": "https://www.sqlite.org/contrib/download/extension-functions.c/download/extension-functions.c?get=25"
        }
    ],
    "replaceList": [
        {
            "aa": "SQLITE_EXTENSION_INIT1",
            "bb": "// $&",
            "flags": "g"
        },
        {
            "aa": "SQLITE_EXTENSION_INIT2",
            "bb": "UNUSED",
            "flags": "g"
        },
        {
            "aa": "\\* (p|s)\\)\\{",
            "bb": "$&\nUNUSED($1);",
            "flags": "g"
        },
        {
            "aa": "\\*(idxStr|pAux|pzErrMsg|tab)[\n,][^\\)]*\\n?\\)\\{",
            "bb": "$&\nUNUSED($1);",
            "flags": "g"
        },
        {
            "aa": "\\n\\*\\* This is a macro that facilitates writting[\\S\\s]*?(\\n\\*\\* Given a string)",
            "bb": "$1",
            "flags": ""
        },
        {
            "aa": "^(?:const )?(?:char|int|void) \\w+?\\([^)]*?\\)",
            "bb": "static $&",
            "flags": "igm"
        },
        {
            "aa": "^__declspec\\(dllexport\\)",
            "bb": "// $&",
            "flags": "gm"
        },
        {
            "aa": "int argc,[^\\)]*\\n?\\)\\{",
            "bb": "$&\nUNUSED(argc);",
            "flags": "g"
        },
        {
            "aa": "int idxNum,[^\\)]*\\n?\\)\\{",
            "bb": "$&\nUNUSED(idxNum);",
            "flags": "g"
        },
        {
            "aa": "unused\\(idxnum\\);",
            "bb": "UNUSED(argv);\n$&",
            "flags": "ig"
        }
    ]
}
-    /\\* math.h *\\/
-    { "acos",               1, 0, SQLITE_UTF8,    0, acosFunc  },
-    { "asin",               1, 0, SQLITE_UTF8,    0, asinFunc  },
-    { "atan",               1, 0, SQLITE_UTF8,    0, atanFunc  },
-    { "atn2",               2, 0, SQLITE_UTF8,    0, atn2Func  },
-    /\\* XXX alias *\\/
-    { "atan2",              2, 0, SQLITE_UTF8,    0, atn2Func  },
-    { "acosh",              1, 0, SQLITE_UTF8,    0, acoshFunc  },
-    { "asinh",              1, 0, SQLITE_UTF8,    0, asinhFunc  },
-    { "atanh",              1, 0, SQLITE_UTF8,    0, atanhFunc  },
-
-    { "difference",         2, 0, SQLITE_UTF8,    0, differenceFunc},
-    { "degrees",            1, 0, SQLITE_UTF8,    0, rad2degFunc  },
-    { "radians",            1, 0, SQLITE_UTF8,    0, deg2radFunc  },
-
-    { "cos",                1, 0, SQLITE_UTF8,    0, cosFunc  },
-    { "sin",                1, 0, SQLITE_UTF8,    0, sinFunc },
-    { "tan",                1, 0, SQLITE_UTF8,    0, tanFunc },
-    { "cot",                1, 0, SQLITE_UTF8,    0, cotFunc },
-    { "cosh",               1, 0, SQLITE_UTF8,    0, coshFunc  },
-    { "sinh",               1, 0, SQLITE_UTF8,    0, sinhFunc },
-    { "tanh",               1, 0, SQLITE_UTF8,    0, tanhFunc },
-    { "coth",               1, 0, SQLITE_UTF8,    0, cothFunc },
-
-    { "exp",                1, 0, SQLITE_UTF8,    0, expFunc  },
-    { "log",                1, 0, SQLITE_UTF8,    0, logFunc  },
-    { "log10",              1, 0, SQLITE_UTF8,    0, log10Func  },
-    { "power",              2, 0, SQLITE_UTF8,    0, powerFunc  },
-    { "sign",               1, 0, SQLITE_UTF8,    0, signFunc },
-    { "sqrt",               1, 0, SQLITE_UTF8,    0, sqrtFunc },
-    { "square",             1, 0, SQLITE_UTF8,    0, squareFunc },
-
-    { "ceil",               1, 0, SQLITE_UTF8,    0, ceilFunc },
-    { "floor",              1, 0, SQLITE_UTF8,    0, floorFunc },
-
-    { "pi",                 0, 0, SQLITE_UTF8,    1, piFunc },
+    /\\* math.h *\\/
+// hack-sqlite - deduplicate math-functions
+    { "difference",         2, 0, SQLITE_UTF8,    0, differenceFunc},

-  pIn = sqlite3_value_blob(argv[0]);
-  nIn = sqlite3_value_bytes(argv[0]);
+  pIn = sqlite3_value_blob(argv[0]);
+// hack-sqlite - handle null-case compress
+  if (pIn == NULL) { sqlite3_result_error(context, "Cannot compress() NULL blob", -1); return; }
+  nIn = sqlite3_value_bytes(argv[0]);

-  pIn = sqlite3_value_blob(argv[0]);
-  nIn = sqlite3_value_bytes(argv[0]);
+  pIn = sqlite3_value_blob(argv[0]);
+// hack-sqlite - handle null-case uncompress
+  if (pIn == NULL) { sqlite3_result_error(context, "Cannot uncompress() NULL blob", -1); return; }
+  nIn = sqlite3_value_bytes(argv[0]);

-  unsigned int nIn;
+// hack-sqlite - fix warning
+  int nIn;

-  unsigned int nIn;
+// hack-sqlite - fix warning
+  int nIn;

-#include "sqlite3ext.h"
+#include "sqlite3ext.h"
+// hack-sqlite - init sqlite3_api
+static const sqlite3_api_routines *sqlite3_api;

-#include <zlib.h>
+// hack-sqlite - inline zlib.h
+// #include <zlib.h>

-static void print_elem(void *e, int64_t c, void* p){
-UNUSED(p);
-  int ee = *(int*)(e);
-  printf("%d => %lld\n", ee,c);
-}
+// hack-sqlite - fix warning
*/


/*
repo https://github.com/sqlite/sqlite/tree/version-3.38.0
committed 2022-02-22T18:58:40Z
*/


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/LICENSE.md
*/
/*
The author disclaims copyright to this source code.  In place of
a legal notice, here is a blessing:

  *   May you do good and not evil.
  *   May you find forgiveness for yourself and forgive others.
  *   May you share freely, never taking more than you give.
*/


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/compress.c
*/
/*
** 2014-06-13
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
** This SQLite extension implements SQL compression functions
** compress() and uncompress() using ZLIB.
*/
#include "sqlite3ext.h"
// hack-sqlite - init sqlite3_api
static const sqlite3_api_routines *sqlite3_api;
// SQLITE_EXTENSION_INIT1
// hack-sqlite - inline zlib.h
// #include <zlib.h>

/*
** Implementation of the "compress(X)" SQL function.  The input X is
** compressed using zLib and the output is returned.
**
** The output is a BLOB that begins with a variable-length integer that
** is the input size in bytes (the size of X before compression).  The
** variable-length integer is implemented as 1 to 5 bytes.  There are
** seven bits per integer stored in the lower seven bits of each byte.
** More significant bits occur first.  The most significant bit (0x80)
** is a flag to indicate the end of the integer.
**
** This function, SQLAR, and ZIP all use the same "deflate" compression
** algorithm, but each is subtly different:
**
**   *  ZIP uses raw deflate.
**
**   *  SQLAR uses the "zlib format" which is raw deflate with a two-byte
**      algorithm-identification header and a four-byte checksum at the end.
**
**   *  This utility uses the "zlib format" like SQLAR, but adds the variable-
**      length integer uncompressed size value at the beginning.
**
** This function might be extended in the future to support compression
** formats other than deflate, by providing a different algorithm-id
** mark following the variable-length integer size parameter.
*/
static void compressFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
UNUSED(argc);
  const unsigned char *pIn;
  unsigned char *pOut;
// hack-sqlite - fix warning
  int nIn;
  unsigned long int nOut;
  unsigned char x[8];
  int rc;
  int i, j;

  pIn = sqlite3_value_blob(argv[0]);
// hack-sqlite - handle null-case compress
  if (pIn == NULL) { sqlite3_result_error(context, "Cannot compress() NULL blob", -1); return; }
  nIn = sqlite3_value_bytes(argv[0]);
  nOut = 13 + nIn + (nIn+999)/1000;
  pOut = sqlite3_malloc( nOut+5 );
  for(i=4; i>=0; i--){
    x[i] = (nIn >> (7*(4-i)))&0x7f;
  }
  for(i=0; i<4 && x[i]==0; i++){}
  for(j=0; i<=4; i++, j++) pOut[j] = x[i];
  pOut[j-1] |= 0x80;
  rc = compress(&pOut[j], &nOut, pIn, nIn);
  if( rc==Z_OK ){
    sqlite3_result_blob(context, pOut, nOut+j, sqlite3_free);
  }else{
    sqlite3_free(pOut);
  }
}

/*
** Implementation of the "uncompress(X)" SQL function.  The argument X
** is a blob which was obtained from compress(Y).  The output will be
** the value Y.
*/
static void uncompressFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
UNUSED(argc);
  const unsigned char *pIn;
  unsigned char *pOut;
// hack-sqlite - fix warning
  int nIn;
  unsigned long int nOut;
  int rc;
  int i;

  pIn = sqlite3_value_blob(argv[0]);
// hack-sqlite - handle null-case uncompress
  if (pIn == NULL) { sqlite3_result_error(context, "Cannot uncompress() NULL blob", -1); return; }
  nIn = sqlite3_value_bytes(argv[0]);
  nOut = 0;
  for(i=0; i<nIn && i<5; i++){
    nOut = (nOut<<7) | (pIn[i]&0x7f);
    if( (pIn[i]&0x80)!=0 ){ i++; break; }
  }
  pOut = sqlite3_malloc( nOut+1 );
  rc = uncompress(pOut, &nOut, &pIn[i], nIn-i);
  if( rc==Z_OK ){
    sqlite3_result_blob(context, pOut, nOut, sqlite3_free);
  }else{
    sqlite3_free(pOut);
  }
}


#ifdef _WIN32
// __declspec(dllexport)
#endif
static int sqlite3_compress_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
UNUSED(pzErrMsg);
  int rc = SQLITE_OK;
  UNUSED(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  rc = sqlite3_create_function(db, "compress", 1,
                    SQLITE_UTF8 | SQLITE_INNOCUOUS,
                    0, compressFunc, 0, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "uncompress", 1,
                    SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
                    0, uncompressFunc, 0, 0);
  }
  return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/csv.c
*/
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
**    CREATE VIRTUAL TABLE temp.csv USING csv(filename=FILENAME);
**    SELECT * FROM csv;
**
** The columns are named "c1", "c2", "c3", ... by default.  Or the
** application can define its own CREATE TABLE statement using the
** schema= parameter, like this:
**
**    CREATE VIRTUAL TABLE temp.csv2 USING csv(
**       filename = "../http.log",
**       schema = "CREATE TABLE x(date,ipaddr,url,referrer,userAgent)"
**    );
**
** Instead of specifying a file, the text of the CSV can be loaded using
** the data= parameter.
**
** If the columns=N parameter is supplied, then the CSV file is assumed to have
** N columns.  If both the columns= and schema= parameters are omitted, then
** the number and names of the columns is determined by the first line of
** the CSV input.
**
** Some extra debugging features (used for testing virtual tables) are available
** if this module is compiled with -DSQLITE_TEST.
*/
#include <sqlite3ext.h>
// SQLITE_EXTENSION_INIT1
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>

#ifndef SQLITE_OMIT_VIRTUALTABLE

/*
** A macro to hint to the compiler that a function should not be
** inlined.
*/
#if defined(__GNUC__)
#  define CSV_NOINLINE  __attribute__((noinline))
#elif defined(_MSC_VER) && _MSC_VER>=1310
#  define CSV_NOINLINE  __declspec(noinline)
#else
#  define CSV_NOINLINE
#endif


/* Max size of the error message in a CsvReader */
#define CSV_MXERR 200

/* Size of the CsvReader input buffer */
#define CSV_INBUFSZ 1024

/* A context object used when read a CSV file. */
typedef struct CsvReader CsvReader;
struct CsvReader {
  FILE *in;              /* Read the CSV text from this input stream */
  char *z;               /* Accumulated text for a field */
  int n;                 /* Number of bytes in z */
  int nAlloc;            /* Space allocated for z[] */
  int nLine;             /* Current line number */
  int bNotFirst;         /* True if prior text has been seen */
  int cTerm;             /* Character that terminated the most recent field */
  size_t iIn;            /* Next unread character in the input buffer */
  size_t nIn;            /* Number of characters in the input buffer */
  char *zIn;             /* The input buffer */
  char zErr[CSV_MXERR];  /* Error message */
};

/* Initialize a CsvReader object */
static void csv_reader_init(CsvReader *p){
  p->in = 0;
  p->z = 0;
  p->n = 0;
  p->nAlloc = 0;
  p->nLine = 0;
  p->bNotFirst = 0;
  p->nIn = 0;
  p->zIn = 0;
  p->zErr[0] = 0;
}

/* Close and reset a CsvReader object */
static void csv_reader_reset(CsvReader *p){
  if( p->in ){
    fclose(p->in);
    sqlite3_free(p->zIn);
  }
  sqlite3_free(p->z);
  csv_reader_init(p);
}

/* Report an error on a CsvReader */
static void csv_errmsg(CsvReader *p, const char *zFormat, ...){
  va_list ap;
  va_start(ap, zFormat);
  sqlite3_vsnprintf(CSV_MXERR, p->zErr, zFormat, ap);
  va_end(ap);
}

/* Open the file associated with a CsvReader
** Return the number of errors.
*/
static int csv_reader_open(
  CsvReader *p,               /* The reader to open */
  const char *zFilename,      /* Read from this filename */
  const char *zData           /*  ... or use this data */
){
  if( zFilename ){
    p->zIn = sqlite3_malloc( CSV_INBUFSZ );
    if( p->zIn==0 ){
      csv_errmsg(p, "out of memory");
      return 1;
    }
    p->in = fopen(zFilename, "rb");
    if( p->in==0 ){
      sqlite3_free(p->zIn);
      csv_reader_reset(p);
      csv_errmsg(p, "cannot open '%s' for reading", zFilename);
      return 1;
    }
  }else{
    assert( p->in==0 );
    p->zIn = (char*)zData;
    p->nIn = strlen(zData);
  }
  return 0;
}

/* The input buffer has overflowed.  Refill the input buffer, then
** return the next character
*/
static CSV_NOINLINE int csv_getc_refill(CsvReader *p){
  size_t got;

  assert( p->iIn>=p->nIn );  /* Only called on an empty input buffer */
  assert( p->in!=0 );        /* Only called if reading froma file */

  got = fread(p->zIn, 1, CSV_INBUFSZ, p->in);
  if( got==0 ) return EOF;
  p->nIn = got;
  p->iIn = 1;
  return p->zIn[0];
}

/* Return the next character of input.  Return EOF at end of input. */
static int csv_getc(CsvReader *p){
  if( p->iIn >= p->nIn ){
    if( p->in!=0 ) return csv_getc_refill(p);
    return EOF;
  }
  return ((unsigned char*)p->zIn)[p->iIn++];
}

/* Increase the size of p->z and append character c to the end.
** Return 0 on success and non-zero if there is an OOM error */
static CSV_NOINLINE int csv_resize_and_append(CsvReader *p, char c){
  char *zNew;
  int nNew = p->nAlloc*2 + 100;
  zNew = sqlite3_realloc64(p->z, nNew);
  if( zNew ){
    p->z = zNew;
    p->nAlloc = nNew;
    p->z[p->n++] = c;
    return 0;
  }else{
    csv_errmsg(p, "out of memory");
    return 1;
  }
}

/* Append a single character to the CsvReader.z[] array.
** Return 0 on success and non-zero if there is an OOM error */
static int csv_append(CsvReader *p, char c){
  if( p->n>=p->nAlloc-1 ) return csv_resize_and_append(p, c);
  p->z[p->n++] = c;
  return 0;
}

/* Read a single field of CSV text.  Compatible with rfc4180 and extended
** with the option of having a separator other than ",".
**
**   +  Input comes from p->in.
**   +  Store results in p->z of length p->n.  Space to hold p->z comes
**      from sqlite3_malloc64().
**   +  Keep track of the line number in p->nLine.
**   +  Store the character that terminates the field in p->cTerm.  Store
**      EOF on end-of-file.
**
** Return 0 at EOF or on OOM.  On EOF, the p->cTerm character will have
** been set to EOF.
*/
static char *csv_read_one_field(CsvReader *p){
  int c;
  p->n = 0;
  c = csv_getc(p);
  if( c==EOF ){
    p->cTerm = EOF;
    return 0;
  }
  if( c=='"' ){
    int pc, ppc;
    int startLine = p->nLine;
    pc = ppc = 0;
    while( 1 ){
      c = csv_getc(p);
      if( c<='"' || pc=='"' ){
        if( c=='\n' ) p->nLine++;
        if( c=='"' ){
          if( pc=='"' ){
            pc = 0;
            continue;
          }
        }
        if( (c==',' && pc=='"')
         || (c=='\n' && pc=='"')
         || (c=='\n' && pc=='\r' && ppc=='"')
         || (c==EOF && pc=='"')
        ){
          do{ p->n--; }while( p->z[p->n]!='"' );
          p->cTerm = (char)c;
          break;
        }
        if( pc=='"' && c!='\r' ){
          csv_errmsg(p, "line %d: unescaped %c character", p->nLine, '"');
          break;
        }
        if( c==EOF ){
          csv_errmsg(p, "line %d: unterminated %c-quoted field\n",
                     startLine, '"');
          p->cTerm = (char)c;
          break;
        }
      }
      if( csv_append(p, (char)c) ) return 0;
      ppc = pc;
      pc = c;
    }
  }else{
    /* If this is the first field being parsed and it begins with the
    ** UTF-8 BOM  (0xEF BB BF) then skip the BOM */
    if( (c&0xff)==0xef && p->bNotFirst==0 ){
      csv_append(p, (char)c);
      c = csv_getc(p);
      if( (c&0xff)==0xbb ){
        csv_append(p, (char)c);
        c = csv_getc(p);
        if( (c&0xff)==0xbf ){
          p->bNotFirst = 1;
          p->n = 0;
          return csv_read_one_field(p);
        }
      }
    }
    while( c>',' || (c!=EOF && c!=',' && c!='\n') ){
      if( csv_append(p, (char)c) ) return 0;
      c = csv_getc(p);
    }
    if( c=='\n' ){
      p->nLine++;
      if( p->n>0 && p->z[p->n-1]=='\r' ) p->n--;
    }
    p->cTerm = (char)c;
  }
  if( p->z ) p->z[p->n] = 0;
  p->bNotFirst = 1;
  return p->z;
}


/* Forward references to the various virtual table methods implemented
** in this file. */
static int csvtabCreate(sqlite3*, void*, int, const char*const*,
                           sqlite3_vtab**,char**);
static int csvtabConnect(sqlite3*, void*, int, const char*const*,
                           sqlite3_vtab**,char**);
static int csvtabBestIndex(sqlite3_vtab*,sqlite3_index_info*);
static int csvtabDisconnect(sqlite3_vtab*);
static int csvtabOpen(sqlite3_vtab*, sqlite3_vtab_cursor**);
static int csvtabClose(sqlite3_vtab_cursor*);
static int csvtabFilter(sqlite3_vtab_cursor*, int idxNum, const char *idxStr,
                          int argc, sqlite3_value **argv);
static int csvtabNext(sqlite3_vtab_cursor*);
static int csvtabEof(sqlite3_vtab_cursor*);
static int csvtabColumn(sqlite3_vtab_cursor*,sqlite3_context*,int);
static int csvtabRowid(sqlite3_vtab_cursor*,sqlite3_int64*);

/* An instance of the CSV virtual table */
typedef struct CsvTable {
  sqlite3_vtab base;              /* Base class.  Must be first */
  char *zFilename;                /* Name of the CSV file */
  char *zData;                    /* Raw CSV data in lieu of zFilename */
  long iStart;                    /* Offset to start of data in zFilename */
  int nCol;                       /* Number of columns in the CSV file */
  unsigned int tstFlags;          /* Bit values used for testing */
} CsvTable;

/* Allowed values for tstFlags */
#define CSVTEST_FIDX  0x0001      /* Pretend that constrained searchs cost less*/

/* A cursor for the CSV virtual table */
typedef struct CsvCursor {
  sqlite3_vtab_cursor base;       /* Base class.  Must be first */
  CsvReader rdr;                  /* The CsvReader object */
  char **azVal;                   /* Value of the current row */
  int *aLen;                      /* Length of each entry */
  sqlite3_int64 iRowid;           /* The current rowid.  Negative for EOF */
} CsvCursor;

/* Transfer error message text from a reader into a CsvTable */
static void csv_xfer_error(CsvTable *pTab, CsvReader *pRdr){
  sqlite3_free(pTab->base.zErrMsg);
  pTab->base.zErrMsg = sqlite3_mprintf("%s", pRdr->zErr);
}

/*
** This method is the destructor fo a CsvTable object.
*/
static int csvtabDisconnect(sqlite3_vtab *pVtab){
  CsvTable *p = (CsvTable*)pVtab;
  sqlite3_free(p->zFilename);
  sqlite3_free(p->zData);
  sqlite3_free(p);
  return SQLITE_OK;
}

/* Skip leading whitespace.  Return a pointer to the first non-whitespace
** character, or to the zero terminator if the string has only whitespace */
static const char *csv_skip_whitespace(const char *z){
  while( isspace((unsigned char)z[0]) ) z++;
  return z;
}

/* Remove trailing whitespace from the end of string z[] */
static void csv_trim_whitespace(char *z){
  size_t n = strlen(z);
  while( n>0 && isspace((unsigned char)z[n]) ) n--;
  z[n] = 0;
}

/* Dequote the string */
static void csv_dequote(char *z){
  int j;
  char cQuote = z[0];
  size_t i, n;

  if( cQuote!='\'' && cQuote!='"' ) return;
  n = strlen(z);
  if( n<2 || z[n-1]!=z[0] ) return;
  for(i=1, j=0; i<n-1; i++){
    if( z[i]==cQuote && z[i+1]==cQuote ) i++;
    z[j++] = z[i];
  }
  z[j] = 0;
}

/* Check to see if the string is of the form:  "TAG = VALUE" with optional
** whitespace before and around tokens.  If it is, return a pointer to the
** first character of VALUE.  If it is not, return NULL.
*/
static const char *csv_parameter(const char *zTag, int nTag, const char *z){
  z = csv_skip_whitespace(z);
  if( strncmp(zTag, z, nTag)!=0 ) return 0;
  z = csv_skip_whitespace(z+nTag);
  if( z[0]!='=' ) return 0;
  return csv_skip_whitespace(z+1);
}

/* Decode a parameter that requires a dequoted string.
**
** Return 1 if the parameter is seen, or 0 if not.  1 is returned
** even if there is an error.  If an error occurs, then an error message
** is left in p->zErr.  If there are no errors, p->zErr[0]==0.
*/
static int csv_string_parameter(
  CsvReader *p,            /* Leave the error message here, if there is one */
  const char *zParam,      /* Parameter we are checking for */
  const char *zArg,        /* Raw text of the virtual table argment */
  char **pzVal             /* Write the dequoted string value here */
){
  const char *zValue;
  zValue = csv_parameter(zParam,(int)strlen(zParam),zArg);
  if( zValue==0 ) return 0;
  p->zErr[0] = 0;
  if( *pzVal ){
    csv_errmsg(p, "more than one '%s' parameter", zParam);
    return 1;
  }
  *pzVal = sqlite3_mprintf("%s", zValue);
  if( *pzVal==0 ){
    csv_errmsg(p, "out of memory");
    return 1;
  }
  csv_trim_whitespace(*pzVal);
  csv_dequote(*pzVal);
  return 1;
}


/* Return 0 if the argument is false and 1 if it is true.  Return -1 if
** we cannot really tell.
*/
static int csv_boolean(const char *z){
  if( sqlite3_stricmp("yes",z)==0
   || sqlite3_stricmp("on",z)==0
   || sqlite3_stricmp("true",z)==0
   || (z[0]=='1' && z[1]==0)
  ){
    return 1;
  }
  if( sqlite3_stricmp("no",z)==0
   || sqlite3_stricmp("off",z)==0
   || sqlite3_stricmp("false",z)==0
   || (z[0]=='0' && z[1]==0)
  ){
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
  const char *zTag,       /* Tag we are looking for */
  int nTag,               /* Size of the tag in bytes */
  const char *z,          /* Input parameter */
  int *pValue             /* Write boolean value here */
){
  int b;
  z = csv_skip_whitespace(z);
  if( strncmp(zTag, z, nTag)!=0 ) return 0;
  z = csv_skip_whitespace(z + nTag);
  if( z[0]==0 ){
    *pValue = 1;
    return 1;
  }
  if( z[0]!='=' ) return 0;
  z = csv_skip_whitespace(z+1);
  b = csv_boolean(z);
  if( b>=0 ){
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
**    header=YES|NO              First row of CSV defines the names of
**                               columns if "yes".  Default "no".
**    columns=N                  Assume the CSV file contains N columns.
**
** Only available if compiled with SQLITE_TEST:
**
**    testflags=N                Bitmask of test flags.  Optional
**
** If schema= is omitted, then the columns are named "c0", "c1", "c2",
** and so forth.  If columns=N is omitted, then the file is opened and
** the number of columns in the first row is counted to determine the
** column count.  If header=YES, then the first row is skipped.
*/
static int csvtabConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
UNUSED(argc);
UNUSED(pAux);
  CsvTable *pNew = 0;        /* The CsvTable object to construct */
  int bHeader = -1;          /* header= flags.  -1 means not seen yet */
  int rc = SQLITE_OK;        /* Result code from this routine */
  int i, j;                  /* Loop counters */
#ifdef SQLITE_TEST
  int tstFlags = 0;          /* Value for testflags=N parameter */
#endif
  int b;                     /* Value of a boolean parameter */
  int nCol = -99;            /* Value of the columns= parameter */
  CsvReader sRdr;            /* A CSV file reader used to store an error
                             ** message and/or to count the number of columns */
  static const char *azParam[] = {
     "filename", "data", "schema",
  };
  char *azPValue[3];         /* Parameter values */
# define CSV_FILENAME (azPValue[0])
# define CSV_DATA     (azPValue[1])
# define CSV_SCHEMA   (azPValue[2])


  assert( sizeof(azPValue)==sizeof(azParam) );
  memset(&sRdr, 0, sizeof(sRdr));
  memset(azPValue, 0, sizeof(azPValue));
  for(i=3; i<argc; i++){
    const char *z = argv[i];
    const char *zValue;
    for(j=0; j<sizeof(azParam)/sizeof(azParam[0]); j++){
      if( csv_string_parameter(&sRdr, azParam[j], z, &azPValue[j]) ) break;
    }
    if( j<sizeof(azParam)/sizeof(azParam[0]) ){
      if( sRdr.zErr[0] ) goto csvtab_connect_error;
    }else
    if( csv_boolean_parameter("header",6,z,&b) ){
      if( bHeader>=0 ){
        csv_errmsg(&sRdr, "more than one 'header' parameter");
        goto csvtab_connect_error;
      }
      bHeader = b;
    }else
#ifdef SQLITE_TEST
    if( (zValue = csv_parameter("testflags",9,z))!=0 ){
      tstFlags = (unsigned int)atoi(zValue);
    }else
#endif
    if( (zValue = csv_parameter("columns",7,z))!=0 ){
      if( nCol>0 ){
        csv_errmsg(&sRdr, "more than one 'columns' parameter");
        goto csvtab_connect_error;
      }
      nCol = atoi(zValue);
      if( nCol<=0 ){
        csv_errmsg(&sRdr, "column= value must be positive");
        goto csvtab_connect_error;
      }
    }else
    {
      csv_errmsg(&sRdr, "bad parameter: '%s'", z);
      goto csvtab_connect_error;
    }
  }
  if( (CSV_FILENAME==0)==(CSV_DATA==0) ){
    csv_errmsg(&sRdr, "must specify either filename= or data= but not both");
    goto csvtab_connect_error;
  }

  if( (nCol<=0 || bHeader==1)
   && csv_reader_open(&sRdr, CSV_FILENAME, CSV_DATA)
  ){
    goto csvtab_connect_error;
  }
  pNew = sqlite3_malloc( sizeof(*pNew) );
  *ppVtab = (sqlite3_vtab*)pNew;
  if( pNew==0 ) goto csvtab_connect_oom;
  memset(pNew, 0, sizeof(*pNew));
  if( CSV_SCHEMA==0 ){
    sqlite3_str *pStr = sqlite3_str_new(0);
    char *zSep = "";
    int iCol = 0;
    sqlite3_str_appendf(pStr, "CREATE TABLE x(");
    if( nCol<0 && bHeader<1 ){
      nCol = 0;
      do{
        csv_read_one_field(&sRdr);
        nCol++;
      }while( sRdr.cTerm==',' );
    }
    if( nCol>0 && bHeader<1 ){
      for(iCol=0; iCol<nCol; iCol++){
        sqlite3_str_appendf(pStr, "%sc%d TEXT", zSep, iCol);
        zSep = ",";
      }
    }else{
      do{
        char *z = csv_read_one_field(&sRdr);
        if( (nCol>0 && iCol<nCol) || (nCol<0 && bHeader) ){
          sqlite3_str_appendf(pStr,"%s\"%w\" TEXT", zSep, z);
          zSep = ",";
          iCol++;
        }
      }while( sRdr.cTerm==',' );
      if( nCol<0 ){
        nCol = iCol;
      }else{
        while( iCol<nCol ){
          sqlite3_str_appendf(pStr,"%sc%d TEXT", zSep, ++iCol);
          zSep = ",";
        }
      }
    }
    pNew->nCol = nCol;
    sqlite3_str_appendf(pStr, ")");
    CSV_SCHEMA = sqlite3_str_finish(pStr);
    if( CSV_SCHEMA==0 ) goto csvtab_connect_oom;
  }else if( nCol<0 ){
    do{
      csv_read_one_field(&sRdr);
      pNew->nCol++;
    }while( sRdr.cTerm==',' );
  }else{
    pNew->nCol = nCol;
  }
  pNew->zFilename = CSV_FILENAME;  CSV_FILENAME = 0;
  pNew->zData = CSV_DATA;          CSV_DATA = 0;
#ifdef SQLITE_TEST
  pNew->tstFlags = tstFlags;
#endif
  if( bHeader!=1 ){
    pNew->iStart = 0;
  }else if( pNew->zData ){
    pNew->iStart = (int)sRdr.iIn;
  }else{
    pNew->iStart = (int)(ftell(sRdr.in) - sRdr.nIn + sRdr.iIn);
  }
  csv_reader_reset(&sRdr);
  rc = sqlite3_declare_vtab(db, CSV_SCHEMA);
  if( rc ){
    csv_errmsg(&sRdr, "bad schema: '%s' - %s", CSV_SCHEMA, sqlite3_errmsg(db));
    goto csvtab_connect_error;
  }
  for(i=0; i<sizeof(azPValue)/sizeof(azPValue[0]); i++){
    sqlite3_free(azPValue[i]);
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
  if( pNew ) csvtabDisconnect(&pNew->base);
  for(i=0; i<sizeof(azPValue)/sizeof(azPValue[0]); i++){
    sqlite3_free(azPValue[i]);
  }
  if( sRdr.zErr[0] ){
    sqlite3_free(*pzErr);
    *pzErr = sqlite3_mprintf("%s", sRdr.zErr);
  }
  csv_reader_reset(&sRdr);
  if( rc==SQLITE_OK ) rc = SQLITE_ERROR;
  return rc;
}

/*
** Reset the current row content held by a CsvCursor.
*/
static void csvtabCursorRowReset(CsvCursor *pCur){
  CsvTable *pTab = (CsvTable*)pCur->base.pVtab;
  int i;
  for(i=0; i<pTab->nCol; i++){
    sqlite3_free(pCur->azVal[i]);
    pCur->azVal[i] = 0;
    pCur->aLen[i] = 0;
  }
}

/*
** The xConnect and xCreate methods do the same thing, but they must be
** different so that the virtual table is not an eponymous virtual table.
*/
static int csvtabCreate(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
UNUSED(argc);
UNUSED(pAux);
 return csvtabConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

/*
** Destructor for a CsvCursor.
*/
static int csvtabClose(sqlite3_vtab_cursor *cur){
  CsvCursor *pCur = (CsvCursor*)cur;
  csvtabCursorRowReset(pCur);
  csv_reader_reset(&pCur->rdr);
  sqlite3_free(cur);
  return SQLITE_OK;
}

/*
** Constructor for a new CsvTable cursor object.
*/
static int csvtabOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  CsvTable *pTab = (CsvTable*)p;
  CsvCursor *pCur;
  size_t nByte;
  nByte = sizeof(*pCur) + (sizeof(char*)+sizeof(int))*pTab->nCol;
  pCur = sqlite3_malloc64( nByte );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, nByte);
  pCur->azVal = (char**)&pCur[1];
  pCur->aLen = (int*)&pCur->azVal[pTab->nCol];
  *ppCursor = &pCur->base;
  if( csv_reader_open(&pCur->rdr, pTab->zFilename, pTab->zData) ){
    csv_xfer_error(pTab, &pCur->rdr);
    return SQLITE_ERROR;
  }
  return SQLITE_OK;
}


/*
** Advance a CsvCursor to its next row of input.
** Set the EOF marker if we reach the end of input.
*/
static int csvtabNext(sqlite3_vtab_cursor *cur){
  CsvCursor *pCur = (CsvCursor*)cur;
  CsvTable *pTab = (CsvTable*)cur->pVtab;
  int i = 0;
  char *z;
  do{
    z = csv_read_one_field(&pCur->rdr);
    if( z==0 ){
      break;
    }
    if( i<pTab->nCol ){
      if( pCur->aLen[i] < pCur->rdr.n+1 ){
        char *zNew = sqlite3_realloc64(pCur->azVal[i], pCur->rdr.n+1);
        if( zNew==0 ){
          csv_errmsg(&pCur->rdr, "out of memory");
          csv_xfer_error(pTab, &pCur->rdr);
          break;
        }
        pCur->azVal[i] = zNew;
        pCur->aLen[i] = pCur->rdr.n+1;
      }
      memcpy(pCur->azVal[i], z, pCur->rdr.n+1);
      i++;
    }
  }while( pCur->rdr.cTerm==',' );
  if( z==0 || (pCur->rdr.cTerm==EOF && i<pTab->nCol) ){
    pCur->iRowid = -1;
  }else{
    pCur->iRowid++;
    while( i<pTab->nCol ){
      sqlite3_free(pCur->azVal[i]);
      pCur->azVal[i] = 0;
      pCur->aLen[i] = 0;
      i++;
    }
  }
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the CsvCursor
** is currently pointing.
*/
static int csvtabColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  CsvCursor *pCur = (CsvCursor*)cur;
  CsvTable *pTab = (CsvTable*)cur->pVtab;
  if( i>=0 && i<pTab->nCol && pCur->azVal[i]!=0 ){
    sqlite3_result_text(ctx, pCur->azVal[i], -1, SQLITE_TRANSIENT);
  }
  return SQLITE_OK;
}

/*
** Return the rowid for the current row.
*/
static int csvtabRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  CsvCursor *pCur = (CsvCursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
static int csvtabEof(sqlite3_vtab_cursor *cur){
  CsvCursor *pCur = (CsvCursor*)cur;
  return pCur->iRowid<0;
}

/*
** Only a full table scan is supported.  So xFilter simply rewinds to
** the beginning.
*/
static int csvtabFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
UNUSED(argv);
UNUSED(idxNum);
UNUSED(argc);
UNUSED(idxStr);
  CsvCursor *pCur = (CsvCursor*)pVtabCursor;
  CsvTable *pTab = (CsvTable*)pVtabCursor->pVtab;
  pCur->iRowid = 0;
  if( pCur->rdr.in==0 ){
    assert( pCur->rdr.zIn==pTab->zData );
    assert( pTab->iStart>=0 );
    assert( (size_t)pTab->iStart<=pCur->rdr.nIn );
    pCur->rdr.iIn = pTab->iStart;
  }else{
    fseek(pCur->rdr.in, pTab->iStart, SEEK_SET);
    pCur->rdr.iIn = 0;
    pCur->rdr.nIn = 0;
  }
  return csvtabNext(pVtabCursor);
}

/*
** Only a forward full table scan is supported.  xBestIndex is mostly
** a no-op.  If CSVTEST_FIDX is set, then the presence of equality
** constraints lowers the estimated cost, which is fiction, but is useful
** for testing certain kinds of virtual table behavior.
*/
static int csvtabBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
UNUSED(tab);
  pIdxInfo->estimatedCost = 1000000;
#ifdef SQLITE_TEST
  if( (((CsvTable*)tab)->tstFlags & CSVTEST_FIDX)!=0 ){
    /* The usual (and sensible) case is to always do a full table scan.
    ** The code in this branch only runs when testflags=1.  This code
    ** generates an artifical and unrealistic plan which is useful
    ** for testing virtual table logic but is not helpful to real applications.
    **
    ** Any ==, LIKE, or GLOB constraint is marked as usable by the virtual
    ** table (even though it is not) and the cost of running the virtual table
    ** is reduced from 1 million to just 10.  The constraints are *not* marked
    ** as omittable, however, so the query planner should still generate a
    ** plan that gives a correct answer, even if they plan is not optimal.
    */
    int i;
    int nConst = 0;
    for(i=0; i<pIdxInfo->nConstraint; i++){
      unsigned char op;
      if( pIdxInfo->aConstraint[i].usable==0 ) continue;
      op = pIdxInfo->aConstraint[i].op;
      if( op==SQLITE_INDEX_CONSTRAINT_EQ
       || op==SQLITE_INDEX_CONSTRAINT_LIKE
       || op==SQLITE_INDEX_CONSTRAINT_GLOB
      ){
        pIdxInfo->estimatedCost = 10;
        pIdxInfo->aConstraintUsage[nConst].argvIndex = nConst+1;
        nConst++;
      }
    }
  }
#endif
  return SQLITE_OK;
}


static sqlite3_module CsvModule = {
  0,                       /* iVersion */
  csvtabCreate,            /* xCreate */
  csvtabConnect,           /* xConnect */
  csvtabBestIndex,         /* xBestIndex */
  csvtabDisconnect,        /* xDisconnect */
  csvtabDisconnect,        /* xDestroy */
  csvtabOpen,              /* xOpen - open a cursor */
  csvtabClose,             /* xClose - close a cursor */
  csvtabFilter,            /* xFilter - configure scan constraints */
  csvtabNext,              /* xNext - advance a cursor */
  csvtabEof,               /* xEof - check for end of scan */
  csvtabColumn,            /* xColumn - read data */
  csvtabRowid,             /* xRowid - read data */
  0,                       /* xUpdate */
  0,                       /* xBegin */
  0,                       /* xSync */
  0,                       /* xCommit */
  0,                       /* xRollback */
  0,                       /* xFindMethod */
  0,                       /* xRename */
};

#ifdef SQLITE_TEST
/*
** For virtual table testing, make a version of the CSV virtual table
** available that has an xUpdate function.  But the xUpdate always returns
** SQLITE_READONLY since the CSV file is not really writable.
*/
static int csvtabUpdate(sqlite3_vtab *p,int n,sqlite3_value**v,sqlite3_int64*x){
  return SQLITE_READONLY;
}
static sqlite3_module CsvModuleFauxWrite = {
  0,                       /* iVersion */
  csvtabCreate,            /* xCreate */
  csvtabConnect,           /* xConnect */
  csvtabBestIndex,         /* xBestIndex */
  csvtabDisconnect,        /* xDisconnect */
  csvtabDisconnect,        /* xDestroy */
  csvtabOpen,              /* xOpen - open a cursor */
  csvtabClose,             /* xClose - close a cursor */
  csvtabFilter,            /* xFilter - configure scan constraints */
  csvtabNext,              /* xNext - advance a cursor */
  csvtabEof,               /* xEof - check for end of scan */
  csvtabColumn,            /* xColumn - read data */
  csvtabRowid,             /* xRowid - read data */
  csvtabUpdate,            /* xUpdate */
  0,                       /* xBegin */
  0,                       /* xSync */
  0,                       /* xCommit */
  0,                       /* xRollback */
  0,                       /* xFindMethod */
  0,                       /* xRename */
};
#endif /* SQLITE_TEST */

#endif /* !defined(SQLITE_OMIT_VIRTUALTABLE) */


#ifdef _WIN32
// __declspec(dllexport)
#endif
/*
** This routine is called when the extension is loaded.  The new
** CSV virtual table module is registered with the calling database
** connection.
*/
static int sqlite3_csv_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
UNUSED(pzErrMsg);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  int rc;
  UNUSED(pApi);
  rc = sqlite3_create_module(db, "csv", &CsvModule, 0);
#ifdef SQLITE_TEST
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_module(db, "csv_wr", &CsvModuleFauxWrite, 0);
  }
#endif
  return rc;
#else
  return SQLITE_OK;
#endif
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/noop.c
*/
/*
** 2020-01-08
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
** This SQLite extension implements a noop() function used for testing.
**
** Variants:
**
**    noop(X)           The default.  Deterministic.
**    noop_i(X)         Deterministic and innocuous.
**    noop_do(X)        Deterministic and direct-only.
**    noop_nd(X)        Non-deterministic.
*/
#include "sqlite3ext.h"
// SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>

/*
** Implementation of the noop() function.
**
** The function returns its argument, unchanged.
*/
static void noopfunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
UNUSED(argc);
  assert( argc==1 );
  sqlite3_result_value(context, argv[0]);
}

#ifdef _WIN32
// __declspec(dllexport)
#endif
static int sqlite3_noop_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
UNUSED(pzErrMsg);
  int rc = SQLITE_OK;
  UNUSED(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  rc = sqlite3_create_function(db, "noop", 1,
                     SQLITE_UTF8 | SQLITE_DETERMINISTIC,
                     0, noopfunc, 0, 0);
  if( rc ) return rc;
  rc = sqlite3_create_function(db, "noop_i", 1,
                     SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
                     0, noopfunc, 0, 0);
  if( rc ) return rc;
  rc = sqlite3_create_function(db, "noop_do", 1,
                     SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY,
                     0, noopfunc, 0, 0);
  if( rc ) return rc;
  rc = sqlite3_create_function(db, "noop_nd", 1,
                     SQLITE_UTF8,
                     0, noopfunc, 0, 0);
  return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/regexp.c
*/
/*
** 2012-11-13
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
** The code in this file implements a compact but reasonably
** efficient regular-expression matcher for posix extended regular
** expressions against UTF8 text.
**
** This file is an SQLite extension.  It registers a single function
** named "regexp(A,B)" where A is the regular expression and B is the
** string to be matched.  By registering this function, SQLite will also
** then implement the "B regexp A" operator.  Note that with the function
** the regular expression comes first, but with the operator it comes
** second.
**
**  The following regular expression syntax is supported:
**
**     X*      zero or more occurrences of X
**     X+      one or more occurrences of X
**     X?      zero or one occurrences of X
**     X{p,q}  between p and q occurrences of X
**     (X)     match X
**     X|Y     X or Y
**     ^X      X occurring at the beginning of the string
**     X$      X occurring at the end of the string
**     .       Match any single character
**     \c      Character c where c is one of \{}()[]|*+?.
**     \c      C-language escapes for c in afnrtv.  ex: \t or \n
**     \uXXXX  Where XXXX is exactly 4 hex digits, unicode value XXXX
**     \xXX    Where XX is exactly 2 hex digits, unicode value XX
**     [abc]   Any single character from the set abc
**     [^abc]  Any single character not in the set abc
**     [a-z]   Any single character in the range a-z
**     [^a-z]  Any single character not in the range a-z
**     \b      Word boundary
**     \w      Word character.  [A-Za-z0-9_]
**     \W      Non-word character
**     \d      Digit
**     \D      Non-digit
**     \s      Whitespace character
**     \S      Non-whitespace character
**
** A nondeterministic finite automaton (NFA) is used for matching, so the
** performance is bounded by O(N*M) where N is the size of the regular
** expression and M is the size of the input string.  The matcher never
** exhibits exponential behavior.  Note that the X{p,q} operator expands
** to p copies of X following by q-p copies of X? and that the size of the
** regular expression in the O(N*M) performance bound is computed after
** this expansion.
*/
#include <string.h>
#include <stdlib.h>
#include "sqlite3ext.h"
// SQLITE_EXTENSION_INIT1

/*
** The following #defines change the names of some functions implemented in
** this file to prevent name collisions with C-library functions of the
** same name.
*/
#define re_match   sqlite3re_match
#define re_compile sqlite3re_compile
#define re_free    sqlite3re_free

/* The end-of-input character */
#define RE_EOF            0    /* End of input */

/* The NFA is implemented as sequence of opcodes taken from the following
** set.  Each opcode has a single integer argument.
*/
#define RE_OP_MATCH       1    /* Match the one character in the argument */
#define RE_OP_ANY         2    /* Match any one character.  (Implements ".") */
#define RE_OP_ANYSTAR     3    /* Special optimized version of .* */
#define RE_OP_FORK        4    /* Continue to both next and opcode at iArg */
#define RE_OP_GOTO        5    /* Jump to opcode at iArg */
#define RE_OP_ACCEPT      6    /* Halt and indicate a successful match */
#define RE_OP_CC_INC      7    /* Beginning of a [...] character class */
#define RE_OP_CC_EXC      8    /* Beginning of a [^...] character class */
#define RE_OP_CC_VALUE    9    /* Single value in a character class */
#define RE_OP_CC_RANGE   10    /* Range of values in a character class */
#define RE_OP_WORD       11    /* Perl word character [A-Za-z0-9_] */
#define RE_OP_NOTWORD    12    /* Not a perl word character */
#define RE_OP_DIGIT      13    /* digit:  [0-9] */
#define RE_OP_NOTDIGIT   14    /* Not a digit */
#define RE_OP_SPACE      15    /* space:  [ \t\n\r\v\f] */
#define RE_OP_NOTSPACE   16    /* Not a digit */
#define RE_OP_BOUNDARY   17    /* Boundary between word and non-word */

/* Each opcode is a "state" in the NFA */
typedef unsigned short ReStateNumber;

/* Because this is an NFA and not a DFA, multiple states can be active at
** once.  An instance of the following object records all active states in
** the NFA.  The implementation is optimized for the common case where the
** number of actives states is small.
*/
typedef struct ReStateSet {
  unsigned nState;            /* Number of current states */
  ReStateNumber *aState;      /* Current states */
} ReStateSet;

/* An input string read one character at a time.
*/
typedef struct ReInput ReInput;
struct ReInput {
  const unsigned char *z;  /* All text */
  int i;                   /* Next byte to read */
  int mx;                  /* EOF when i>=mx */
};

/* A compiled NFA (or an NFA that is in the process of being compiled) is
** an instance of the following object.
*/
typedef struct ReCompiled ReCompiled;
struct ReCompiled {
  ReInput sIn;                /* Regular expression text */
  const char *zErr;           /* Error message to return */
  char *aOp;                  /* Operators for the virtual machine */
  int *aArg;                  /* Arguments to each operator */
  unsigned (*xNextChar)(ReInput*);  /* Next character function */
  unsigned char zInit[12];    /* Initial text to match */
  int nInit;                  /* Number of characters in zInit */
  unsigned nState;            /* Number of entries in aOp[] and aArg[] */
  unsigned nAlloc;            /* Slots allocated for aOp[] and aArg[] */
};

/* Add a state to the given state set if it is not already there */
static void re_add_state(ReStateSet *pSet, int newState){
  unsigned i;
  for(i=0; i<pSet->nState; i++) if( pSet->aState[i]==newState ) return;
  pSet->aState[pSet->nState++] = (ReStateNumber)newState;
}

/* Extract the next unicode character from *pzIn and return it.  Advance
** *pzIn to the first byte past the end of the character returned.  To
** be clear:  this routine converts utf8 to unicode.  This routine is
** optimized for the common case where the next character is a single byte.
*/
static unsigned re_next_char(ReInput *p){
  unsigned c;
  if( p->i>=p->mx ) return 0;
  c = p->z[p->i++];
  if( c>=0x80 ){
    if( (c&0xe0)==0xc0 && p->i<p->mx && (p->z[p->i]&0xc0)==0x80 ){
      c = (c&0x1f)<<6 | (p->z[p->i++]&0x3f);
      if( c<0x80 ) c = 0xfffd;
    }else if( (c&0xf0)==0xe0 && p->i+1<p->mx && (p->z[p->i]&0xc0)==0x80
           && (p->z[p->i+1]&0xc0)==0x80 ){
      c = (c&0x0f)<<12 | ((p->z[p->i]&0x3f)<<6) | (p->z[p->i+1]&0x3f);
      p->i += 2;
      if( c<=0x7ff || (c>=0xd800 && c<=0xdfff) ) c = 0xfffd;
    }else if( (c&0xf8)==0xf0 && p->i+3<p->mx && (p->z[p->i]&0xc0)==0x80
           && (p->z[p->i+1]&0xc0)==0x80 && (p->z[p->i+2]&0xc0)==0x80 ){
      c = (c&0x07)<<18 | ((p->z[p->i]&0x3f)<<12) | ((p->z[p->i+1]&0x3f)<<6)
                       | (p->z[p->i+2]&0x3f);
      p->i += 3;
      if( c<=0xffff || c>0x10ffff ) c = 0xfffd;
    }else{
      c = 0xfffd;
    }
  }
  return c;
}
static unsigned re_next_char_nocase(ReInput *p){
  unsigned c = re_next_char(p);
  if( c>='A' && c<='Z' ) c += 'a' - 'A';
  return c;
}

/* Return true if c is a perl "word" character:  [A-Za-z0-9_] */
static int re_word_char(int c){
  return (c>='0' && c<='9') || (c>='a' && c<='z')
      || (c>='A' && c<='Z') || c=='_';
}

/* Return true if c is a "digit" character:  [0-9] */
static int re_digit_char(int c){
  return (c>='0' && c<='9');
}

/* Return true if c is a perl "space" character:  [ \t\r\n\v\f] */
static int re_space_char(int c){
  return c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\v' || c=='\f';
}

/* Run a compiled regular expression on the zero-terminated input
** string zIn[].  Return true on a match and false if there is no match.
*/
static int re_match(ReCompiled *pRe, const unsigned char *zIn, int nIn){
  ReStateSet aStateSet[2], *pThis, *pNext;
  ReStateNumber aSpace[100];
  ReStateNumber *pToFree;
  unsigned int i = 0;
  unsigned int iSwap = 0;
  int c = RE_EOF+1;
  int cPrev = 0;
  int rc = 0;
  ReInput in;

  in.z = zIn;
  in.i = 0;
  in.mx = nIn>=0 ? nIn : (int)strlen((char const*)zIn);

  /* Look for the initial prefix match, if there is one. */
  if( pRe->nInit ){
    unsigned char x = pRe->zInit[0];
    while( in.i+pRe->nInit<=in.mx
     && (zIn[in.i]!=x ||
         strncmp((const char*)zIn+in.i, (const char*)pRe->zInit, pRe->nInit)!=0)
    ){
      in.i++;
    }
    if( in.i+pRe->nInit>in.mx ) return 0;
  }

  if( pRe->nState<=(sizeof(aSpace)/(sizeof(aSpace[0])*2)) ){
    pToFree = 0;
    aStateSet[0].aState = aSpace;
  }else{
    pToFree = sqlite3_malloc64( sizeof(ReStateNumber)*2*pRe->nState );
    if( pToFree==0 ) return -1;
    aStateSet[0].aState = pToFree;
  }
  aStateSet[1].aState = &aStateSet[0].aState[pRe->nState];
  pNext = &aStateSet[1];
  pNext->nState = 0;
  re_add_state(pNext, 0);
  while( c!=RE_EOF && pNext->nState>0 ){
    cPrev = c;
    c = pRe->xNextChar(&in);
    pThis = pNext;
    pNext = &aStateSet[iSwap];
    iSwap = 1 - iSwap;
    pNext->nState = 0;
    for(i=0; i<pThis->nState; i++){
      int x = pThis->aState[i];
      switch( pRe->aOp[x] ){
        case RE_OP_MATCH: {
          if( pRe->aArg[x]==c ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_ANY: {
          if( c!=0 ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_WORD: {
          if( re_word_char(c) ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_NOTWORD: {
          if( !re_word_char(c) && c!=0 ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_DIGIT: {
          if( re_digit_char(c) ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_NOTDIGIT: {
          if( !re_digit_char(c) && c!=0 ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_SPACE: {
          if( re_space_char(c) ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_NOTSPACE: {
          if( !re_space_char(c) && c!=0 ) re_add_state(pNext, x+1);
          break;
        }
        case RE_OP_BOUNDARY: {
          if( re_word_char(c)!=re_word_char(cPrev) ) re_add_state(pThis, x+1);
          break;
        }
        case RE_OP_ANYSTAR: {
          re_add_state(pNext, x);
          re_add_state(pThis, x+1);
          break;
        }
        case RE_OP_FORK: {
          re_add_state(pThis, x+pRe->aArg[x]);
          re_add_state(pThis, x+1);
          break;
        }
        case RE_OP_GOTO: {
          re_add_state(pThis, x+pRe->aArg[x]);
          break;
        }
        case RE_OP_ACCEPT: {
          rc = 1;
          goto re_match_end;
        }
        case RE_OP_CC_EXC: {
          if( c==0 ) break;
          /* fall-through */ goto re_op_cc_inc;
        }
        case RE_OP_CC_INC: re_op_cc_inc: {
          int j = 1;
          int n = pRe->aArg[x];
          int hit = 0;
          for(j=1; j>0 && j<n; j++){
            if( pRe->aOp[x+j]==RE_OP_CC_VALUE ){
              if( pRe->aArg[x+j]==c ){
                hit = 1;
                j = -1;
              }
            }else{
              if( pRe->aArg[x+j]<=c && pRe->aArg[x+j+1]>=c ){
                hit = 1;
                j = -1;
              }else{
                j++;
              }
            }
          }
          if( pRe->aOp[x]==RE_OP_CC_EXC ) hit = !hit;
          if( hit ) re_add_state(pNext, x+n);
          break;
        }
      }
    }
  }
  for(i=0; i<pNext->nState; i++){
    if( pRe->aOp[pNext->aState[i]]==RE_OP_ACCEPT ){ rc = 1; break; }
  }
re_match_end:
  sqlite3_free(pToFree);
  return rc;
}

/* Resize the opcode and argument arrays for an RE under construction.
*/
static int re_resize(ReCompiled *p, int N){
  char *aOp;
  int *aArg;
  aOp = sqlite3_realloc64(p->aOp, N*sizeof(p->aOp[0]));
  if( aOp==0 ) return 1;
  p->aOp = aOp;
  aArg = sqlite3_realloc64(p->aArg, N*sizeof(p->aArg[0]));
  if( aArg==0 ) return 1;
  p->aArg = aArg;
  p->nAlloc = N;
  return 0;
}

/* Insert a new opcode and argument into an RE under construction.  The
** insertion point is just prior to existing opcode iBefore.
*/
static int re_insert(ReCompiled *p, int iBefore, int op, int arg){
  int i;
  if( p->nAlloc<=p->nState && re_resize(p, p->nAlloc*2) ) return 0;
  for(i=p->nState; i>iBefore; i--){
    p->aOp[i] = p->aOp[i-1];
    p->aArg[i] = p->aArg[i-1];
  }
  p->nState++;
  p->aOp[iBefore] = (char)op;
  p->aArg[iBefore] = arg;
  return iBefore;
}

/* Append a new opcode and argument to the end of the RE under construction.
*/
static int re_append(ReCompiled *p, int op, int arg){
  return re_insert(p, p->nState, op, arg);
}

/* Make a copy of N opcodes starting at iStart onto the end of the RE
** under construction.
*/
static void re_copy(ReCompiled *p, int iStart, int N){
  if( p->nState+N>=p->nAlloc && re_resize(p, p->nAlloc*2+N) ) return;
  memcpy(&p->aOp[p->nState], &p->aOp[iStart], N*sizeof(p->aOp[0]));
  memcpy(&p->aArg[p->nState], &p->aArg[iStart], N*sizeof(p->aArg[0]));
  p->nState += N;
}

/* Return true if c is a hexadecimal digit character:  [0-9a-fA-F]
** If c is a hex digit, also set *pV = (*pV)*16 + valueof(c).  If
** c is not a hex digit *pV is unchanged.
*/
static int re_hex(int c, int *pV){
  if( c>='0' && c<='9' ){
    c -= '0';
  }else if( c>='a' && c<='f' ){
    c -= 'a' - 10;
  }else if( c>='A' && c<='F' ){
    c -= 'A' - 10;
  }else{
    return 0;
  }
  *pV = (*pV)*16 + (c & 0xff);
  return 1;
}

/* A backslash character has been seen, read the next character and
** return its interpretation.
*/
static unsigned re_esc_char(ReCompiled *p){
  static const char zEsc[] = "afnrtv\\()*.+?[$^{|}]";
  static const char zTrans[] = "\a\f\n\r\t\v";
  int i, v = 0;
  char c;
  if( p->sIn.i>=p->sIn.mx ) return 0;
  c = p->sIn.z[p->sIn.i];
  if( c=='u' && p->sIn.i+4<p->sIn.mx ){
    const unsigned char *zIn = p->sIn.z + p->sIn.i;
    if( re_hex(zIn[1],&v)
     && re_hex(zIn[2],&v)
     && re_hex(zIn[3],&v)
     && re_hex(zIn[4],&v)
    ){
      p->sIn.i += 5;
      return v;
    }
  }
  if( c=='x' && p->sIn.i+2<p->sIn.mx ){
    const unsigned char *zIn = p->sIn.z + p->sIn.i;
    if( re_hex(zIn[1],&v)
     && re_hex(zIn[2],&v)
    ){
      p->sIn.i += 3;
      return v;
    }
  }
  for(i=0; zEsc[i] && zEsc[i]!=c; i++){}
  if( zEsc[i] ){
    if( i<6 ) c = zTrans[i];
    p->sIn.i++;
  }else{
    p->zErr = "unknown \\ escape";
  }
  return c;
}

/* Forward declaration */
static const char *re_subcompile_string(ReCompiled*);

/* Peek at the next byte of input */
static unsigned char rePeek(ReCompiled *p){
  return p->sIn.i<p->sIn.mx ? p->sIn.z[p->sIn.i] : 0;
}

/* Compile RE text into a sequence of opcodes.  Continue up to the
** first unmatched ")" character, then return.  If an error is found,
** return a pointer to the error message string.
*/
static const char *re_subcompile_re(ReCompiled *p){
  const char *zErr;
  int iStart, iEnd, iGoto;
  iStart = p->nState;
  zErr = re_subcompile_string(p);
  if( zErr ) return zErr;
  while( rePeek(p)=='|' ){
    iEnd = p->nState;
    re_insert(p, iStart, RE_OP_FORK, iEnd + 2 - iStart);
    iGoto = re_append(p, RE_OP_GOTO, 0);
    p->sIn.i++;
    zErr = re_subcompile_string(p);
    if( zErr ) return zErr;
    p->aArg[iGoto] = p->nState - iGoto;
  }
  return 0;
}

/* Compile an element of regular expression text (anything that can be
** an operand to the "|" operator).  Return NULL on success or a pointer
** to the error message if there is a problem.
*/
static const char *re_subcompile_string(ReCompiled *p){
  int iPrev = -1;
  int iStart;
  unsigned c;
  const char *zErr;
  while( (c = p->xNextChar(&p->sIn))!=0 ){
    iStart = p->nState;
    switch( c ){
      case '|':
      case '$':
      case ')': {
        p->sIn.i--;
        return 0;
      }
      case '(': {
        zErr = re_subcompile_re(p);
        if( zErr ) return zErr;
        if( rePeek(p)!=')' ) return "unmatched '('";
        p->sIn.i++;
        break;
      }
      case '.': {
        if( rePeek(p)=='*' ){
          re_append(p, RE_OP_ANYSTAR, 0);
          p->sIn.i++;
        }else{
          re_append(p, RE_OP_ANY, 0);
        }
        break;
      }
      case '*': {
        if( iPrev<0 ) return "'*' without operand";
        re_insert(p, iPrev, RE_OP_GOTO, p->nState - iPrev + 1);
        re_append(p, RE_OP_FORK, iPrev - p->nState + 1);
        break;
      }
      case '+': {
        if( iPrev<0 ) return "'+' without operand";
        re_append(p, RE_OP_FORK, iPrev - p->nState);
        break;
      }
      case '?': {
        if( iPrev<0 ) return "'?' without operand";
        re_insert(p, iPrev, RE_OP_FORK, p->nState - iPrev+1);
        break;
      }
      case '{': {
        int m = 0, n = 0;
        int sz, j;
        if( iPrev<0 ) return "'{m,n}' without operand";
        while( (c=rePeek(p))>='0' && c<='9' ){ m = m*10 + c - '0'; p->sIn.i++; }
        n = m;
        if( c==',' ){
          p->sIn.i++;
          n = 0;
          while( (c=rePeek(p))>='0' && c<='9' ){ n = n*10 + c-'0'; p->sIn.i++; }
        }
        if( c!='}' ) return "unmatched '{'";
        if( n>0 && n<m ) return "n less than m in '{m,n}'";
        p->sIn.i++;
        sz = p->nState - iPrev;
        if( m==0 ){
          if( n==0 ) return "both m and n are zero in '{m,n}'";
          re_insert(p, iPrev, RE_OP_FORK, sz+1);
          n--;
        }else{
          for(j=1; j<m; j++) re_copy(p, iPrev, sz);
        }
        for(j=m; j<n; j++){
          re_append(p, RE_OP_FORK, sz+1);
          re_copy(p, iPrev, sz);
        }
        if( n==0 && m>0 ){
          re_append(p, RE_OP_FORK, -sz);
        }
        break;
      }
      case '[': {
        int iFirst = p->nState;
        if( rePeek(p)=='^' ){
          re_append(p, RE_OP_CC_EXC, 0);
          p->sIn.i++;
        }else{
          re_append(p, RE_OP_CC_INC, 0);
        }
        while( (c = p->xNextChar(&p->sIn))!=0 ){
          if( c=='[' && rePeek(p)==':' ){
            return "POSIX character classes not supported";
          }
          if( c=='\\' ) c = re_esc_char(p);
          if( rePeek(p)=='-' ){
            re_append(p, RE_OP_CC_RANGE, c);
            p->sIn.i++;
            c = p->xNextChar(&p->sIn);
            if( c=='\\' ) c = re_esc_char(p);
            re_append(p, RE_OP_CC_RANGE, c);
          }else{
            re_append(p, RE_OP_CC_VALUE, c);
          }
          if( rePeek(p)==']' ){ p->sIn.i++; break; }
        }
        if( c==0 ) return "unclosed '['";
        p->aArg[iFirst] = p->nState - iFirst;
        break;
      }
      case '\\': {
        int specialOp = 0;
        switch( rePeek(p) ){
          case 'b': specialOp = RE_OP_BOUNDARY;   break;
          case 'd': specialOp = RE_OP_DIGIT;      break;
          case 'D': specialOp = RE_OP_NOTDIGIT;   break;
          case 's': specialOp = RE_OP_SPACE;      break;
          case 'S': specialOp = RE_OP_NOTSPACE;   break;
          case 'w': specialOp = RE_OP_WORD;       break;
          case 'W': specialOp = RE_OP_NOTWORD;    break;
        }
        if( specialOp ){
          p->sIn.i++;
          re_append(p, specialOp, 0);
        }else{
          c = re_esc_char(p);
          re_append(p, RE_OP_MATCH, c);
        }
        break;
      }
      default: {
        re_append(p, RE_OP_MATCH, c);
        break;
      }
    }
    iPrev = iStart;
  }
  return 0;
}

/* Free and reclaim all the memory used by a previously compiled
** regular expression.  Applications should invoke this routine once
** for every call to re_compile() to avoid memory leaks.
*/
static void re_free(ReCompiled *pRe){
  if( pRe ){
    sqlite3_free(pRe->aOp);
    sqlite3_free(pRe->aArg);
    sqlite3_free(pRe);
  }
}

/*
** Compile a textual regular expression in zIn[] into a compiled regular
** expression suitable for us by re_match() and return a pointer to the
** compiled regular expression in *ppRe.  Return NULL on success or an
** error message if something goes wrong.
*/
static const char *re_compile(ReCompiled **ppRe, const char *zIn, int noCase){
  ReCompiled *pRe;
  const char *zErr;
  int i, j;

  *ppRe = 0;
  pRe = sqlite3_malloc( sizeof(*pRe) );
  if( pRe==0 ){
    return "out of memory";
  }
  memset(pRe, 0, sizeof(*pRe));
  pRe->xNextChar = noCase ? re_next_char_nocase : re_next_char;
  if( re_resize(pRe, 30) ){
    re_free(pRe);
    return "out of memory";
  }
  if( zIn[0]=='^' ){
    zIn++;
  }else{
    re_append(pRe, RE_OP_ANYSTAR, 0);
  }
  pRe->sIn.z = (unsigned char*)zIn;
  pRe->sIn.i = 0;
  pRe->sIn.mx = (int)strlen(zIn);
  zErr = re_subcompile_re(pRe);
  if( zErr ){
    re_free(pRe);
    return zErr;
  }
  if( rePeek(pRe)=='$' && pRe->sIn.i+1>=pRe->sIn.mx ){
    re_append(pRe, RE_OP_MATCH, RE_EOF);
    re_append(pRe, RE_OP_ACCEPT, 0);
    *ppRe = pRe;
  }else if( pRe->sIn.i>=pRe->sIn.mx ){
    re_append(pRe, RE_OP_ACCEPT, 0);
    *ppRe = pRe;
  }else{
    re_free(pRe);
    return "unrecognized character";
  }

  /* The following is a performance optimization.  If the regex begins with
  ** ".*" (if the input regex lacks an initial "^") and afterwards there are
  ** one or more matching characters, enter those matching characters into
  ** zInit[].  The re_match() routine can then search ahead in the input
  ** string looking for the initial match without having to run the whole
  ** regex engine over the string.  Do not worry able trying to match
  ** unicode characters beyond plane 0 - those are very rare and this is
  ** just an optimization. */
  if( pRe->aOp[0]==RE_OP_ANYSTAR && !noCase ){
    for(j=0, i=1; j<(int)sizeof(pRe->zInit)-2 && pRe->aOp[i]==RE_OP_MATCH; i++){
      unsigned x = pRe->aArg[i];
      if( x<=127 ){
        pRe->zInit[j++] = (unsigned char)x;
      }else if( x<=0xfff ){
        pRe->zInit[j++] = (unsigned char)(0xc0 | (x>>6));
        pRe->zInit[j++] = 0x80 | (x&0x3f);
      }else if( x<=0xffff ){
        pRe->zInit[j++] = (unsigned char)(0xd0 | (x>>12));
        pRe->zInit[j++] = 0x80 | ((x>>6)&0x3f);
        pRe->zInit[j++] = 0x80 | (x&0x3f);
      }else{
        break;
      }
    }
    if( j>0 && pRe->zInit[j-1]==0 ) j--;
    pRe->nInit = j;
  }
  return pRe->zErr;
}

/*
** Implementation of the regexp() SQL function.  This function implements
** the build-in REGEXP operator.  The first argument to the function is the
** pattern and the second argument is the string.  So, the SQL statements:
**
**       A REGEXP B
**
** is implemented as regexp(B,A).
*/
static void re_sql_func(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
UNUSED(argc);
  ReCompiled *pRe;          /* Compiled regular expression */
  const char *zPattern;     /* The regular expression */
  const unsigned char *zStr;/* String being searched */
  const char *zErr;         /* Compile error message */
  int setAux = 0;           /* True to invoke sqlite3_set_auxdata() */

  (void)argc;  /* Unused */
  pRe = sqlite3_get_auxdata(context, 0);
  if( pRe==0 ){
    zPattern = (const char*)sqlite3_value_text(argv[0]);
    if( zPattern==0 ) return;
    zErr = re_compile(&pRe, zPattern, sqlite3_user_data(context)!=0);
    if( zErr ){
      re_free(pRe);
      sqlite3_result_error(context, zErr, -1);
      return;
    }
    if( pRe==0 ){
      sqlite3_result_error_nomem(context);
      return;
    }
    setAux = 1;
  }
  zStr = (const unsigned char*)sqlite3_value_text(argv[1]);
  if( zStr!=0 ){
    sqlite3_result_int(context, re_match(pRe, zStr, -1));
  }
  if( setAux ){
    sqlite3_set_auxdata(context, 0, pRe, (void(*)(void*))re_free);
  }
}

/*
** Invoke this routine to register the regexp() function with the
** SQLite database connection.
*/
#ifdef _WIN32
// __declspec(dllexport)
#endif
static int sqlite3_regexp_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
UNUSED(pzErrMsg);
  int rc = SQLITE_OK;
  UNUSED(pApi);
  (void)pzErrMsg;  /* Unused */
  rc = sqlite3_create_function(db, "regexp", 2,
                            SQLITE_UTF8|SQLITE_INNOCUOUS|SQLITE_DETERMINISTIC,
                            0, re_sql_func, 0, 0);
  if( rc==SQLITE_OK ){
    /* The regexpi(PATTERN,STRING) function is a case-insensitive version
    ** of regexp(PATTERN,STRING). */
    rc = sqlite3_create_function(db, "regexpi", 2,
                            SQLITE_UTF8|SQLITE_INNOCUOUS|SQLITE_DETERMINISTIC,
                            (void*)db, re_sql_func, 0, 0);
  }
  return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/series.c
*/
/*
** 2015-08-18
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file demonstrates how to create a table-valued-function using
** a virtual table.  This demo implements the generate_series() function
** which gives similar results to the eponymous function in PostgreSQL.
** Examples:
**
**      SELECT * FROM generate_series(0,100,5);
**
** The query above returns integers from 0 through 100 counting by steps
** of 5.
**
**      SELECT * FROM generate_series(0,100);
**
** Integers from 0 through 100 with a step size of 1.
**
**      SELECT * FROM generate_series(20) LIMIT 10;
**
** Integers 20 through 29.
**
** HOW IT WORKS
**
** The generate_series "function" is really a virtual table with the
** following schema:
**
**     CREATE TABLE generate_series(
**       value,
**       start HIDDEN,
**       stop HIDDEN,
**       step HIDDEN
**     );
**
** Function arguments in queries against this virtual table are translated
** into equality constraints against successive hidden columns.  In other
** words, the following pairs of queries are equivalent to each other:
**
**    SELECT * FROM generate_series(0,100,5);
**    SELECT * FROM generate_series WHERE start=0 AND stop=100 AND step=5;
**
**    SELECT * FROM generate_series(0,100);
**    SELECT * FROM generate_series WHERE start=0 AND stop=100;
**
**    SELECT * FROM generate_series(20) LIMIT 10;
**    SELECT * FROM generate_series WHERE start=20 LIMIT 10;
**
** The generate_series virtual table implementation leaves the xCreate method
** set to NULL.  This means that it is not possible to do a CREATE VIRTUAL
** TABLE command with "generate_series" as the USING argument.  Instead, there
** is a single generate_series virtual table that is always available without
** having to be created first.
**
** The xBestIndex method looks for equality constraints against the hidden
** start, stop, and step columns, and if present, it uses those constraints
** to bound the sequence of generated values.  If the equality constraints
** are missing, it uses 0 for start, 4294967295 for stop, and 1 for step.
** xBestIndex returns a small cost when both start and stop are available,
** and a very large cost if either start or stop are unavailable.  This
** encourages the query planner to order joins such that the bounds of the
** series are well-defined.
*/
#include "sqlite3ext.h"
// SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>

#ifndef SQLITE_OMIT_VIRTUALTABLE


/* series_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct series_cursor series_cursor;
struct series_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  int isDesc;                /* True to count down rather than up */
  sqlite3_int64 iRowid;      /* The rowid */
  sqlite3_int64 iValue;      /* Current value ("value") */
  sqlite3_int64 mnValue;     /* Mimimum value ("start") */
  sqlite3_int64 mxValue;     /* Maximum value ("stop") */
  sqlite3_int64 iStep;       /* Increment ("step") */
};

/*
** The seriesConnect() method is invoked to create a new
** series_vtab that describes the generate_series virtual table.
**
** Think of this routine as the constructor for series_vtab objects.
**
** All this routine needs to do is:
**
**    (1) Allocate the series_vtab object and initialize all fields.
**
**    (2) Tell SQLite (via the sqlite3_declare_vtab() interface) what the
**        result set of queries against generate_series will look like.
*/
static int seriesConnect(
  sqlite3 *db,
  void *pUnused,
  int argcUnused, const char *const*argvUnused,
  sqlite3_vtab **ppVtab,
  char **pzErrUnused
){
  sqlite3_vtab *pNew;
  int rc;

/* Column numbers */
#define SERIES_COLUMN_VALUE 0
#define SERIES_COLUMN_START 1
#define SERIES_COLUMN_STOP  2
#define SERIES_COLUMN_STEP  3

  (void)pUnused;
  (void)argcUnused;
  (void)argvUnused;
  (void)pzErrUnused;
  rc = sqlite3_declare_vtab(db,
     "CREATE TABLE x(value,start hidden,stop hidden,step hidden)");
  if( rc==SQLITE_OK ){
    pNew = *ppVtab = sqlite3_malloc( sizeof(*pNew) );
    if( pNew==0 ) return SQLITE_NOMEM;
    memset(pNew, 0, sizeof(*pNew));
    sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS);
  }
  return rc;
}

/*
** This method is the destructor for series_cursor objects.
*/
static int seriesDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/*
** Constructor for a new series_cursor object.
*/
static int seriesOpen(sqlite3_vtab *pUnused, sqlite3_vtab_cursor **ppCursor){
  series_cursor *pCur;
  (void)pUnused;
  pCur = sqlite3_malloc( sizeof(*pCur) );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(*pCur));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/*
** Destructor for a series_cursor.
*/
static int seriesClose(sqlite3_vtab_cursor *cur){
  sqlite3_free(cur);
  return SQLITE_OK;
}


/*
** Advance a series_cursor to its next row of output.
*/
static int seriesNext(sqlite3_vtab_cursor *cur){
  series_cursor *pCur = (series_cursor*)cur;
  if( pCur->isDesc ){
    pCur->iValue -= pCur->iStep;
  }else{
    pCur->iValue += pCur->iStep;
  }
  pCur->iRowid++;
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the series_cursor
** is currently pointing.
*/
static int seriesColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  series_cursor *pCur = (series_cursor*)cur;
  sqlite3_int64 x = 0;
  switch( i ){
    case SERIES_COLUMN_START:  x = pCur->mnValue; break;
    case SERIES_COLUMN_STOP:   x = pCur->mxValue; break;
    case SERIES_COLUMN_STEP:   x = pCur->iStep;   break;
    default:                   x = pCur->iValue;  break;
  }
  sqlite3_result_int64(ctx, x);
  return SQLITE_OK;
}

/*
** Return the rowid for the current row. In this implementation, the
** first row returned is assigned rowid value 1, and each subsequent
** row a value 1 more than that of the previous.
*/
static int seriesRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  series_cursor *pCur = (series_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
static int seriesEof(sqlite3_vtab_cursor *cur){
  series_cursor *pCur = (series_cursor*)cur;
  if( pCur->isDesc ){
    return pCur->iValue < pCur->mnValue;
  }else{
    return pCur->iValue > pCur->mxValue;
  }
}

/* True to cause run-time checking of the start=, stop=, and/or step=
** parameters.  The only reason to do this is for testing the
** constraint checking logic for virtual tables in the SQLite core.
*/
#ifndef SQLITE_SERIES_CONSTRAINT_VERIFY
# define SQLITE_SERIES_CONSTRAINT_VERIFY 0
#endif

/*
** This method is called to "rewind" the series_cursor object back
** to the first row of output.  This method is always called at least
** once prior to any call to seriesColumn() or seriesRowid() or
** seriesEof().
**
** The query plan selected by seriesBestIndex is passed in the idxNum
** parameter.  (idxStr is not used in this implementation.)  idxNum
** is a bitmask showing which constraints are available:
**
**    1:    start=VALUE
**    2:    stop=VALUE
**    4:    step=VALUE
**
** Also, if bit 8 is set, that means that the series should be output
** in descending order rather than in ascending order.  If bit 16 is
** set, then output must appear in ascending order.
**
** This routine should initialize the cursor and position it so that it
** is pointing at the first row, or pointing off the end of the table
** (so that seriesEof() will return true) if the table is empty.
*/
static int seriesFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStrUnused,
  int argc, sqlite3_value **argv
){
UNUSED(argv);
UNUSED(idxNum);
UNUSED(argc);
  series_cursor *pCur = (series_cursor *)pVtabCursor;
  int i = 0;
  (void)idxStrUnused;
  if( idxNum & 1 ){
    pCur->mnValue = sqlite3_value_int64(argv[i++]);
  }else{
    pCur->mnValue = 0;
  }
  if( idxNum & 2 ){
    pCur->mxValue = sqlite3_value_int64(argv[i++]);
  }else{
    pCur->mxValue = 0xffffffff;
  }
  if( idxNum & 4 ){
    pCur->iStep = sqlite3_value_int64(argv[i++]);
    if( pCur->iStep==0 ){
      pCur->iStep = 1;
    }else if( pCur->iStep<0 ){
      pCur->iStep = -pCur->iStep;
      if( (idxNum & 16)==0 ) idxNum |= 8;
    }
  }else{
    pCur->iStep = 1;
  }
  for(i=0; i<argc; i++){
    if( sqlite3_value_type(argv[i])==SQLITE_NULL ){
      /* If any of the constraints have a NULL value, then return no rows.
      ** See ticket https://www.sqlite.org/src/info/fac496b61722daf2 */
      pCur->mnValue = 1;
      pCur->mxValue = 0;
      break;
    }
  }
  if( idxNum & 8 ){
    pCur->isDesc = 1;
    pCur->iValue = pCur->mxValue;
    if( pCur->iStep>0 ){
      pCur->iValue -= (pCur->mxValue - pCur->mnValue)%pCur->iStep;
    }
  }else{
    pCur->isDesc = 0;
    pCur->iValue = pCur->mnValue;
  }
  pCur->iRowid = 1;
  return SQLITE_OK;
}

/*
** SQLite will invoke this method one or more times while planning a query
** that uses the generate_series virtual table.  This routine needs to create
** a query plan for each invocation and compute an estimated cost for that
** plan.
**
** In this implementation idxNum is used to represent the
** query plan.  idxStr is unused.
**
** The query plan is represented by bits in idxNum:
**
**  (1)  start = $value  -- constraint exists
**  (2)  stop = $value   -- constraint exists
**  (4)  step = $value   -- constraint exists
**  (8)  output in descending order
*/
static int seriesBestIndex(
  sqlite3_vtab *pVTab,
  sqlite3_index_info *pIdxInfo
){
  int i, j;              /* Loop over constraints */
  int idxNum = 0;        /* The query plan bitmask */
  int bStartSeen = 0;    /* EQ constraint seen on the START column */
  int unusableMask = 0;  /* Mask of unusable constraints */
  int nArg = 0;          /* Number of arguments that seriesFilter() expects */
  int aIdx[3];           /* Constraints on start, stop, and step */
  const struct sqlite3_index_constraint *pConstraint;

  /* This implementation assumes that the start, stop, and step columns
  ** are the last three columns in the virtual table. */
  assert( SERIES_COLUMN_STOP == SERIES_COLUMN_START+1 );
  assert( SERIES_COLUMN_STEP == SERIES_COLUMN_START+2 );

  aIdx[0] = aIdx[1] = aIdx[2] = -1;
  pConstraint = pIdxInfo->aConstraint;
  for(i=0; i<pIdxInfo->nConstraint; i++, pConstraint++){
    int iCol;    /* 0 for start, 1 for stop, 2 for step */
    int iMask;   /* bitmask for those column */
    if( pConstraint->iColumn<SERIES_COLUMN_START ) continue;
    iCol = pConstraint->iColumn - SERIES_COLUMN_START;
    assert( iCol>=0 && iCol<=2 );
    iMask = 1 << iCol;
    if( iCol==0 ) bStartSeen = 1;
    if( pConstraint->usable==0 ){
      unusableMask |=  iMask;
      continue;
    }else if( pConstraint->op==SQLITE_INDEX_CONSTRAINT_EQ ){
      idxNum |= iMask;
      aIdx[iCol] = i;
    }
  }
  for(i=0; i<3; i++){
    if( (j = aIdx[i])>=0 ){
      pIdxInfo->aConstraintUsage[j].argvIndex = ++nArg;
      pIdxInfo->aConstraintUsage[j].omit = !SQLITE_SERIES_CONSTRAINT_VERIFY;
    }
  }
  /* The current generate_column() implementation requires at least one
  ** argument (the START value).  Legacy versions assumed START=0 if the
  ** first argument was omitted.  Compile with -DZERO_ARGUMENT_GENERATE_SERIES
  ** to obtain the legacy behavior */
#ifndef ZERO_ARGUMENT_GENERATE_SERIES
  if( !bStartSeen ){
    sqlite3_free(pVTab->zErrMsg);
    pVTab->zErrMsg = sqlite3_mprintf(
        "first argument to \"generate_series()\" missing or unusable");
    return SQLITE_ERROR;
  }
#endif
  if( (unusableMask & ~idxNum)!=0 ){
    /* The start, stop, and step columns are inputs.  Therefore if there
    ** are unusable constraints on any of start, stop, or step then
    ** this plan is unusable */
    return SQLITE_CONSTRAINT;
  }
  if( (idxNum & 3)==3 ){
    /* Both start= and stop= boundaries are available.  This is the
    ** the preferred case */
    pIdxInfo->estimatedCost = (double)(2 - ((idxNum&4)!=0));
    pIdxInfo->estimatedRows = 1000;
    if( pIdxInfo->nOrderBy>=1 && pIdxInfo->aOrderBy[0].iColumn==0 ){
      if( pIdxInfo->aOrderBy[0].desc ){
        idxNum |= 8;
      }else{
        idxNum |= 16;
      }
      pIdxInfo->orderByConsumed = 1;
    }
  }else{
    /* If either boundary is missing, we have to generate a huge span
    ** of numbers.  Make this case very expensive so that the query
    ** planner will work hard to avoid it. */
    pIdxInfo->estimatedRows = 2147483647;
  }
  pIdxInfo->idxNum = idxNum;
  return SQLITE_OK;
}

/*
** This following structure defines all the methods for the
** generate_series virtual table.
*/
static sqlite3_module seriesModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  seriesConnect,             /* xConnect */
  seriesBestIndex,           /* xBestIndex */
  seriesDisconnect,          /* xDisconnect */
  0,                         /* xDestroy */
  seriesOpen,                /* xOpen - open a cursor */
  seriesClose,               /* xClose - close a cursor */
  seriesFilter,              /* xFilter - configure scan constraints */
  seriesNext,                /* xNext - advance a cursor */
  seriesEof,                 /* xEof - check for end of scan */
  seriesColumn,              /* xColumn - read data */
  seriesRowid,               /* xRowid - read data */
  0,                         /* xUpdate */
  0,                         /* xBegin */
  0,                         /* xSync */
  0,                         /* xCommit */
  0,                         /* xRollback */
  0,                         /* xFindMethod */
  0,                         /* xRename */
  0,                         /* xSavepoint */
  0,                         /* xRelease */
  0,                         /* xRollbackTo */
  0                          /* xShadowName */
};

#endif /* SQLITE_OMIT_VIRTUALTABLE */

#ifdef _WIN32
// __declspec(dllexport)
#endif
static int sqlite3_series_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
UNUSED(pzErrMsg);
  int rc = SQLITE_OK;
  UNUSED(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  if( sqlite3_libversion_number()<3008012 && pzErrMsg!=0 ){
    *pzErrMsg = sqlite3_mprintf(
        "generate_series() requires SQLite 3.8.12 or later");
    return SQLITE_ERROR;
  }
  rc = sqlite3_create_module(db, "generate_series", &seriesModule, 0);
#endif
  return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.38.0/ext/misc/shathree.c
*/
/*
** 2017-03-08
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
** This SQLite extension implements functions that compute SHA3 hashes.
** Two SQL functions are implemented:
**
**     sha3(X,SIZE)
**     sha3_query(Y,SIZE)
**
** The sha3(X) function computes the SHA3 hash of the input X, or NULL if
** X is NULL.
**
** The sha3_query(Y) function evalutes all queries in the SQL statements of Y
** and returns a hash of their results.
**
** The SIZE argument is optional.  If omitted, the SHA3-256 hash algorithm
** is used.  If SIZE is included it must be one of the integers 224, 256,
** 384, or 512, to determine SHA3 hash variant that is computed.
*/
#include "sqlite3ext.h"
// SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#ifndef SQLITE_AMALGAMATION
typedef sqlite3_uint64 u64;
#endif /* SQLITE_AMALGAMATION */

/******************************************************************************
** The Hash Engine
*/
/*
** Macros to determine whether the machine is big or little endian,
** and whether or not that determination is run-time or compile-time.
**
** For best performance, an attempt is made to guess at the byte-order
** using C-preprocessor macros.  If that is unsuccessful, or if
** -DSHA3_BYTEORDER=0 is set, then byte-order is determined
** at run-time.
*/
#ifndef SHA3_BYTEORDER
# if defined(i386)     || defined(__i386__)   || defined(_M_IX86) ||    \
     defined(__x86_64) || defined(__x86_64__) || defined(_M_X64)  ||    \
     defined(_M_AMD64) || defined(_M_ARM)     || defined(__x86)   ||    \
     defined(__arm__)
#   define SHA3_BYTEORDER    1234
# elif defined(sparc)    || defined(__ppc__)
#   define SHA3_BYTEORDER    4321
# else
#   define SHA3_BYTEORDER 0
# endif
#endif


/*
** State structure for a SHA3 hash in progress
*/
typedef struct SHA3Context SHA3Context;
struct SHA3Context {
  union {
    u64 s[25];                /* Keccak state. 5x5 lines of 64 bits each */
    unsigned char x[1600];    /* ... or 1600 bytes */
  } u;
  unsigned nRate;        /* Bytes of input accepted per Keccak iteration */
  unsigned nLoaded;      /* Input bytes loaded into u.x[] so far this cycle */
  unsigned ixMask;       /* Insert next input into u.x[nLoaded^ixMask]. */
};

/*
** A single step of the Keccak mixing function for a 1600-bit state
*/
static void KeccakF1600Step(SHA3Context *p){
  int i;
  u64 b0, b1, b2, b3, b4;
  u64 c0, c1, c2, c3, c4;
  u64 d0, d1, d2, d3, d4;
  static const u64 RC[] = {
    0x0000000000000001ULL,  0x0000000000008082ULL,
    0x800000000000808aULL,  0x8000000080008000ULL,
    0x000000000000808bULL,  0x0000000080000001ULL,
    0x8000000080008081ULL,  0x8000000000008009ULL,
    0x000000000000008aULL,  0x0000000000000088ULL,
    0x0000000080008009ULL,  0x000000008000000aULL,
    0x000000008000808bULL,  0x800000000000008bULL,
    0x8000000000008089ULL,  0x8000000000008003ULL,
    0x8000000000008002ULL,  0x8000000000000080ULL,
    0x000000000000800aULL,  0x800000008000000aULL,
    0x8000000080008081ULL,  0x8000000000008080ULL,
    0x0000000080000001ULL,  0x8000000080008008ULL
  };
# define a00 (p->u.s[0])
# define a01 (p->u.s[1])
# define a02 (p->u.s[2])
# define a03 (p->u.s[3])
# define a04 (p->u.s[4])
# define a10 (p->u.s[5])
# define a11 (p->u.s[6])
# define a12 (p->u.s[7])
# define a13 (p->u.s[8])
# define a14 (p->u.s[9])
# define a20 (p->u.s[10])
# define a21 (p->u.s[11])
# define a22 (p->u.s[12])
# define a23 (p->u.s[13])
# define a24 (p->u.s[14])
# define a30 (p->u.s[15])
# define a31 (p->u.s[16])
# define a32 (p->u.s[17])
# define a33 (p->u.s[18])
# define a34 (p->u.s[19])
# define a40 (p->u.s[20])
# define a41 (p->u.s[21])
# define a42 (p->u.s[22])
# define a43 (p->u.s[23])
# define a44 (p->u.s[24])
# define ROL64(a,x) ((a<<x)|(a>>(64-x)))

  for(i=0; i<24; i+=4){
    c0 = a00^a10^a20^a30^a40;
    c1 = a01^a11^a21^a31^a41;
    c2 = a02^a12^a22^a32^a42;
    c3 = a03^a13^a23^a33^a43;
    c4 = a04^a14^a24^a34^a44;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a11^d1), 44);
    b2 = ROL64((a22^d2), 43);
    b3 = ROL64((a33^d3), 21);
    b4 = ROL64((a44^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i];
    a11 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a20^d0), 3);
    b3 = ROL64((a31^d1), 45);
    b4 = ROL64((a42^d2), 61);
    b0 = ROL64((a03^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a20 =   b0 ^((~b1)&  b2 );
    a31 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a40^d0), 18);
    b0 = ROL64((a01^d1), 1);
    b1 = ROL64((a12^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a34^d4), 8);
    a40 =   b0 ^((~b1)&  b2 );
    a01 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a10^d0), 36);
    b2 = ROL64((a21^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a43^d3), 56);
    b0 = ROL64((a04^d4), 27);
    a10 =   b0 ^((~b1)&  b2 );
    a21 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a30^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a02^d2), 62);
    b1 = ROL64((a13^d3), 55);
    b2 = ROL64((a24^d4), 39);
    a30 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    c0 = a00^a20^a40^a10^a30;
    c1 = a11^a31^a01^a21^a41;
    c2 = a22^a42^a12^a32^a02;
    c3 = a33^a03^a23^a43^a13;
    c4 = a44^a14^a34^a04^a24;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a31^d1), 44);
    b2 = ROL64((a12^d2), 43);
    b3 = ROL64((a43^d3), 21);
    b4 = ROL64((a24^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i+1];
    a31 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a40^d0), 3);
    b3 = ROL64((a21^d1), 45);
    b4 = ROL64((a02^d2), 61);
    b0 = ROL64((a33^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a40 =   b0 ^((~b1)&  b2 );
    a21 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a30^d0), 18);
    b0 = ROL64((a11^d1), 1);
    b1 = ROL64((a42^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a04^d4), 8);
    a30 =   b0 ^((~b1)&  b2 );
    a11 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a20^d0), 36);
    b2 = ROL64((a01^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a13^d3), 56);
    b0 = ROL64((a44^d4), 27);
    a20 =   b0 ^((~b1)&  b2 );
    a01 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a10^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a22^d2), 62);
    b1 = ROL64((a03^d3), 55);
    b2 = ROL64((a34^d4), 39);
    a10 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    c0 = a00^a40^a30^a20^a10;
    c1 = a31^a21^a11^a01^a41;
    c2 = a12^a02^a42^a32^a22;
    c3 = a43^a33^a23^a13^a03;
    c4 = a24^a14^a04^a44^a34;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a21^d1), 44);
    b2 = ROL64((a42^d2), 43);
    b3 = ROL64((a13^d3), 21);
    b4 = ROL64((a34^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i+2];
    a21 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a30^d0), 3);
    b3 = ROL64((a01^d1), 45);
    b4 = ROL64((a22^d2), 61);
    b0 = ROL64((a43^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a30 =   b0 ^((~b1)&  b2 );
    a01 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a10^d0), 18);
    b0 = ROL64((a31^d1), 1);
    b1 = ROL64((a02^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a44^d4), 8);
    a10 =   b0 ^((~b1)&  b2 );
    a31 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a40^d0), 36);
    b2 = ROL64((a11^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a03^d3), 56);
    b0 = ROL64((a24^d4), 27);
    a40 =   b0 ^((~b1)&  b2 );
    a11 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a20^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a12^d2), 62);
    b1 = ROL64((a33^d3), 55);
    b2 = ROL64((a04^d4), 39);
    a20 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    c0 = a00^a30^a10^a40^a20;
    c1 = a21^a01^a31^a11^a41;
    c2 = a42^a22^a02^a32^a12;
    c3 = a13^a43^a23^a03^a33;
    c4 = a34^a14^a44^a24^a04;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a01^d1), 44);
    b2 = ROL64((a02^d2), 43);
    b3 = ROL64((a03^d3), 21);
    b4 = ROL64((a04^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i+3];
    a01 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a10^d0), 3);
    b3 = ROL64((a11^d1), 45);
    b4 = ROL64((a12^d2), 61);
    b0 = ROL64((a13^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a10 =   b0 ^((~b1)&  b2 );
    a11 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a20^d0), 18);
    b0 = ROL64((a21^d1), 1);
    b1 = ROL64((a22^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a24^d4), 8);
    a20 =   b0 ^((~b1)&  b2 );
    a21 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a30^d0), 36);
    b2 = ROL64((a31^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a33^d3), 56);
    b0 = ROL64((a34^d4), 27);
    a30 =   b0 ^((~b1)&  b2 );
    a31 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a40^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a42^d2), 62);
    b1 = ROL64((a43^d3), 55);
    b2 = ROL64((a44^d4), 39);
    a40 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );
  }
}

/*
** Initialize a new hash.  iSize determines the size of the hash
** in bits and should be one of 224, 256, 384, or 512.  Or iSize
** can be zero to use the default hash size of 256 bits.
*/
static void SHA3Init(SHA3Context *p, int iSize){
  memset(p, 0, sizeof(*p));
  if( iSize>=128 && iSize<=512 ){
    p->nRate = (1600 - ((iSize + 31)&~31)*2)/8;
  }else{
    p->nRate = (1600 - 2*256)/8;
  }
#if SHA3_BYTEORDER==1234
  /* Known to be little-endian at compile-time. No-op */
#elif SHA3_BYTEORDER==4321
  p->ixMask = 7;  /* Big-endian */
#else
  {
    static unsigned int one = 1;
    if( 1==*(unsigned char*)&one ){
      /* Little endian.  No byte swapping. */
      p->ixMask = 0;
    }else{
      /* Big endian.  Byte swap. */
      p->ixMask = 7;
    }
  }
#endif
}

/*
** Make consecutive calls to the SHA3Update function to add new content
** to the hash
*/
static void SHA3Update(
  SHA3Context *p,
  const unsigned char *aData,
  unsigned int nData
){
  unsigned int i = 0;
  if( aData==0 ) return;
#if SHA3_BYTEORDER==1234
  if( (p->nLoaded % 8)==0 && ((aData - (const unsigned char*)0)&7)==0 ){
    for(; i+7<nData; i+=8){
      p->u.s[p->nLoaded/8] ^= *(u64*)&aData[i];
      p->nLoaded += 8;
      if( p->nLoaded>=p->nRate ){
        KeccakF1600Step(p);
        p->nLoaded = 0;
      }
    }
  }
#endif
  for(; i<nData; i++){
#if SHA3_BYTEORDER==1234
    p->u.x[p->nLoaded] ^= aData[i];
#elif SHA3_BYTEORDER==4321
    p->u.x[p->nLoaded^0x07] ^= aData[i];
#else
    p->u.x[p->nLoaded^p->ixMask] ^= aData[i];
#endif
    p->nLoaded++;
    if( p->nLoaded==p->nRate ){
      KeccakF1600Step(p);
      p->nLoaded = 0;
    }
  }
}

/*
** After all content has been added, invoke SHA3Final() to compute
** the final hash.  The function returns a pointer to the binary
** hash value.
*/
static unsigned char *SHA3Final(SHA3Context *p){
  unsigned int i;
  if( p->nLoaded==p->nRate-1 ){
    const unsigned char c1 = 0x86;
    SHA3Update(p, &c1, 1);
  }else{
    const unsigned char c2 = 0x06;
    const unsigned char c3 = 0x80;
    SHA3Update(p, &c2, 1);
    p->nLoaded = p->nRate - 1;
    SHA3Update(p, &c3, 1);
  }
  for(i=0; i<p->nRate; i++){
    p->u.x[i+p->nRate] = p->u.x[i^p->ixMask];
  }
  return &p->u.x[p->nRate];
}
/* End of the hashing logic
*****************************************************************************/

/*
** Implementation of the sha3(X,SIZE) function.
**
** Return a BLOB which is the SIZE-bit SHA3 hash of X.  The default
** size is 256.  If X is a BLOB, it is hashed as is.
** For all other non-NULL types of input, X is converted into a UTF-8 string
** and the string is hashed without the trailing 0x00 terminator.  The hash
** of a NULL value is NULL.
*/
static void sha3Func(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
UNUSED(argc);
  SHA3Context cx;
  int eType = sqlite3_value_type(argv[0]);
  int nByte = sqlite3_value_bytes(argv[0]);
  int iSize;
  if( argc==1 ){
    iSize = 256;
  }else{
    iSize = sqlite3_value_int(argv[1]);
    if( iSize!=224 && iSize!=256 && iSize!=384 && iSize!=512 ){
      sqlite3_result_error(context, "SHA3 size should be one of: 224 256 "
                                    "384 512", -1);
      return;
    }
  }
  if( eType==SQLITE_NULL ) return;
  SHA3Init(&cx, iSize);
  if( eType==SQLITE_BLOB ){
    SHA3Update(&cx, sqlite3_value_blob(argv[0]), nByte);
  }else{
    SHA3Update(&cx, sqlite3_value_text(argv[0]), nByte);
  }
  sqlite3_result_blob(context, SHA3Final(&cx), iSize/8, SQLITE_TRANSIENT);
}

/* Compute a string using sqlite3_vsnprintf() with a maximum length
** of 50 bytes and add it to the hash.
*/
static void hash_step_vformat(
  SHA3Context *p,                 /* Add content to this context */
  const char *zFormat,
  ...
){
  va_list ap;
  int n;
  char zBuf[50];
  va_start(ap, zFormat);
  sqlite3_vsnprintf(sizeof(zBuf),zBuf,zFormat,ap);
  va_end(ap);
  n = (int)strlen(zBuf);
  SHA3Update(p, (unsigned char*)zBuf, n);
}

/*
** Implementation of the sha3_query(SQL,SIZE) function.
**
** This function compiles and runs the SQL statement(s) given in the
** argument. The results are hashed using a SIZE-bit SHA3.  The default
** size is 256.
**
** The format of the byte stream that is hashed is summarized as follows:
**
**       S<n>:<sql>
**       R
**       N
**       I<int>
**       F<ieee-float>
**       B<size>:<bytes>
**       T<size>:<text>
**
** <sql> is the original SQL text for each statement run and <n> is
** the size of that text.  The SQL text is UTF-8.  A single R character
** occurs before the start of each row.  N means a NULL value.
** I mean an 8-byte little-endian integer <int>.  F is a floating point
** number with an 8-byte little-endian IEEE floating point value <ieee-float>.
** B means blobs of <size> bytes.  T means text rendered as <size>
** bytes of UTF-8.  The <n> and <size> values are expressed as an ASCII
** text integers.
**
** For each SQL statement in the X input, there is one S segment.  Each
** S segment is followed by zero or more R segments, one for each row in the
** result set.  After each R, there are one or more N, I, F, B, or T segments,
** one for each column in the result set.  Segments are concatentated directly
** with no delimiters of any kind.
*/
static void sha3QueryFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
UNUSED(argc);
  sqlite3 *db = sqlite3_context_db_handle(context);
  const char *zSql = (const char*)sqlite3_value_text(argv[0]);
  sqlite3_stmt *pStmt = 0;
  int nCol;                   /* Number of columns in the result set */
  int i;                      /* Loop counter */
  int rc;
  int n;
  const char *z;
  SHA3Context cx;
  int iSize;

  if( argc==1 ){
    iSize = 256;
  }else{
    iSize = sqlite3_value_int(argv[1]);
    if( iSize!=224 && iSize!=256 && iSize!=384 && iSize!=512 ){
      sqlite3_result_error(context, "SHA3 size should be one of: 224 256 "
                                    "384 512", -1);
      return;
    }
  }
  if( zSql==0 ) return;
  SHA3Init(&cx, iSize);
  while( zSql[0] ){
    rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zSql);
    if( rc ){
      char *zMsg = sqlite3_mprintf("error SQL statement [%s]: %s",
                                   zSql, sqlite3_errmsg(db));
      sqlite3_finalize(pStmt);
      sqlite3_result_error(context, zMsg, -1);
      sqlite3_free(zMsg);
      return;
    }
    if( !sqlite3_stmt_readonly(pStmt) ){
      char *zMsg = sqlite3_mprintf("non-query: [%s]", sqlite3_sql(pStmt));
      sqlite3_finalize(pStmt);
      sqlite3_result_error(context, zMsg, -1);
      sqlite3_free(zMsg);
      return;
    }
    nCol = sqlite3_column_count(pStmt);
    z = sqlite3_sql(pStmt);
    if( z ){
      n = (int)strlen(z);
      hash_step_vformat(&cx,"S%d:",n);
      SHA3Update(&cx,(unsigned char*)z,n);
    }

    /* Compute a hash over the result of the query */
    while( SQLITE_ROW==sqlite3_step(pStmt) ){
      SHA3Update(&cx,(const unsigned char*)"R",1);
      for(i=0; i<nCol; i++){
        switch( sqlite3_column_type(pStmt,i) ){
          case SQLITE_NULL: {
            SHA3Update(&cx, (const unsigned char*)"N",1);
            break;
          }
          case SQLITE_INTEGER: {
            sqlite3_uint64 u;
            int j;
            unsigned char x[9];
            sqlite3_int64 v = sqlite3_column_int64(pStmt,i);
            memcpy(&u, &v, 8);
            for(j=8; j>=1; j--){
              x[j] = u & 0xff;
              u >>= 8;
            }
            x[0] = 'I';
            SHA3Update(&cx, x, 9);
            break;
          }
          case SQLITE_FLOAT: {
            sqlite3_uint64 u;
            int j;
            unsigned char x[9];
            double r = sqlite3_column_double(pStmt,i);
            memcpy(&u, &r, 8);
            for(j=8; j>=1; j--){
              x[j] = u & 0xff;
              u >>= 8;
            }
            x[0] = 'F';
            SHA3Update(&cx,x,9);
            break;
          }
          case SQLITE_TEXT: {
            int n2 = sqlite3_column_bytes(pStmt, i);
            const unsigned char *z2 = sqlite3_column_text(pStmt, i);
            hash_step_vformat(&cx,"T%d:",n2);
            SHA3Update(&cx, z2, n2);
            break;
          }
          case SQLITE_BLOB: {
            int n2 = sqlite3_column_bytes(pStmt, i);
            const unsigned char *z2 = sqlite3_column_blob(pStmt, i);
            hash_step_vformat(&cx,"B%d:",n2);
            SHA3Update(&cx, z2, n2);
            break;
          }
        }
      }
    }
    sqlite3_finalize(pStmt);
  }
  sqlite3_result_blob(context, SHA3Final(&cx), iSize/8, SQLITE_TRANSIENT);
}


#ifdef _WIN32
// __declspec(dllexport)
#endif
static int sqlite3_shathree_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
UNUSED(pzErrMsg);
  int rc = SQLITE_OK;
  UNUSED(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  rc = sqlite3_create_function(db, "sha3", 1,
                      SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
                      0, sha3Func, 0, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sha3", 2,
                      SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
                      0, sha3Func, 0, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sha3_query", 1,
                      SQLITE_UTF8 | SQLITE_DIRECTONLY,
                      0, sha3QueryFunc, 0, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sha3_query", 2,
                      SQLITE_UTF8 | SQLITE_DIRECTONLY,
                      0, sha3QueryFunc, 0, 0);
  }
  return rc;
}


/*
repo https://github.com/sqlite/sqlite/tree/version-3.38.0
committed 2022-02-22T18:58:40Z
*/


/*
file https://github.com/sqlite/sqlite/tree/version-3.38.0/contrib/download/extension-functions.c/download/extension-functions.c
*/
/*
This library will provide common mathematical and string functions in
SQL queries using the operating system libraries or provided
definitions.  It includes the following functions:

Math: acos, asin, atan, atn2, atan2, acosh, asinh, atanh, difference,
degrees, radians, cos, sin, tan, cot, cosh, sinh, tanh, coth, exp,
log, log10, power, sign, sqrt, square, ceil, floor, pi.

String: replicate, charindex, leftstr, rightstr, ltrim, rtrim, trim,
replace, reverse, proper, padl, padr, padc, strfilter.

Aggregate: stdev, variance, mode, median, lower_quartile,
upper_quartile.

The string functions ltrim, rtrim, trim, replace are included in
recent versions of SQLite and so by default do not build.

Compilation instructions:
 Compile this C source file into a dynamic library as follows:
 * Linux:
   gcc -fPIC -lm -shared extension-functions.c -o libsqlitefunctions.so
 * Mac OS X:
   gcc -fno-common -dynamiclib extension-functions.c -o libsqlitefunctions.dylib
 (You may need to add flags
  -I /opt/local/include/ -L/opt/local/lib -lsqlite3
  if your sqlite3 is installed from Mac ports, or
  -I /sw/include/ -L/sw/lib -lsqlite3
  if installed with Fink.)
 * Windows:
  1. Install MinGW (http://www.mingw.org/) and you will get the gcc
  (gnu compiler collection)
  2. add the path to your path variable (isn't done during the
   installation!)
  3. compile:
   gcc -shared -I "path" -o libsqlitefunctions.so extension-functions.c
   (path = path of sqlite3ext.h; i.e. C:\programs\sqlite)

Usage instructions for applications calling the sqlite3 API functions:
  In your application, call sqlite3_enable_load_extension(db,1) to
  allow loading external libraries.  Then load the library libsqlitefunctions
  using sqlite3_load_extension; the third argument should be 0.
  See http://www.sqlite.org/cvstrac/wiki?p=LoadableExtensions.
  Select statements may now use these functions, as in
  SELECT cos(radians(inclination)) FROM satsum WHERE satnum = 25544;

Usage instructions for the sqlite3 program:
  If the program is built so that loading extensions is permitted,
  the following will work:
   sqlite> SELECT load_extension('./libsqlitefunctions.so');
   sqlite> select cos(radians(45));
   0.707106781186548
  Note: Loading extensions is by default prohibited as a
  security measure; see "Security Considerations" in
  http://www.sqlite.org/cvstrac/wiki?p=LoadableExtensions.
  If the sqlite3 program and library are built this
  way, you cannot use these functions from the program, you
  must write your own program using the sqlite3 API, and call
  sqlite3_enable_load_extension as described above, or else
  rebuilt the sqlite3 program to allow loadable extensions.

Alterations:
The instructions are for Linux, Mac OS X, and Windows; users of other
OSes may need to modify this procedure.  In particular, if your math
library lacks one or more of the needed trig or log functions, comment
out the appropriate HAVE_ #define at the top of file.  If you do not
wish to make a loadable module, comment out the define for
COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE.  If you are using a
version of SQLite without the trim functions and replace, comment out
the HAVE_TRIM #define.

Liam Healy

History:
2010-01-06 Correct check for argc in squareFunc, and add Windows
compilation instructions.
2009-06-24 Correct check for argc in properFunc.
2008-09-14 Add check that memory was actually allocated after
sqlite3_malloc or sqlite3StrDup, call sqlite3_result_error_nomem if
not.  Thanks to Robert Simpson.
2008-06-13 Change to instructions to indicate use of the math library
and that program might work.
2007-10-01 Minor clarification to instructions.
2007-09-29 Compilation as loadable module is optional with
COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE.
2007-09-28 Use sqlite3_extension_init and macros
// SQLITE_EXTENSION_INIT1, UNUSED, so that it works with
sqlite3_load_extension.  Thanks to Eric Higashino and Joe Wilson.
New instructions for Mac compilation.
2007-09-17 With help from Joe Wilson and Nuno Luca, made use of
external interfaces so that compilation is no longer dependent on
SQLite source code.  Merged source, header, and README into a single
file.  Added casts so that Mac will compile without warnings (unsigned
and signed char).
2007-09-05 Included some definitions from sqlite 3.3.13 so that this
will continue to work in newer versions of sqlite.  Completed
description of functions available.
2007-03-27 Revised description.
2007-03-23 Small cleanup and a bug fix on the code.  This was mainly
letting errno flag errors encountered in the math library and checking
the result, rather than pre-checking.  This fixes a bug in power that
would cause an error if any non-positive number was raised to any
power.
2007-02-07 posted by Mikey C to sqlite mailing list.
Original code 2006 June 05 by relicoder.

*/

//#include "config.h"

#define COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE 1
#define HAVE_ACOSH 1
#define HAVE_ASINH 1
#define HAVE_ATANH 1
#define HAVE_SINH 1
#define HAVE_COSH 1
#define HAVE_TANH 1
#define HAVE_LOG10 1
#define HAVE_ISBLANK 1
#define SQLITE_SOUNDEX 1
#define HAVE_TRIM 1		/* LMH 2007-03-25 if sqlite has trim functions */

#ifdef COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE
#include "sqlite3ext.h"
// SQLITE_EXTENSION_INIT1
#else
#include "sqlite3.h"
#endif

#include <ctype.h>
/* relicoder */
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>		/* LMH 2007-03-25 */

#include <stdlib.h>
#include <assert.h>

#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>

/*
** Simple binary tree implementation to use in median, mode and quartile calculations
** Tree is not necessarily balanced. That would require something like red&black trees of AVL
*/

typedef int(*cmp_func)(const void *, const void *);
typedef void(*map_iterator)(void*, int64_t, void*);

typedef struct node{
  struct node *l;
  struct node *r;
  void* data;
  int64_t count;
} node;

typedef struct map{
  node *base;
  cmp_func cmp;
  short free;
} map;

/*
** creates a map given a comparison function
*/
map map_make(cmp_func cmp);

/*
** inserts the element e into map m
*/
static void map_insert(map *m, void *e);

/*
** executes function iter over all elements in the map, in key increasing order
*/
static void map_iterate(map *m, map_iterator iter, void* p);

/*
** frees all memory used by a map
*/
static void map_destroy(map *m);

/*
** compares 2 integers
** to use with map_make
*/
static int int_cmp(const void *a, const void *b);

/*
** compares 2 doubles
** to use with map_make
*/
static int double_cmp(const void *a, const void *b);

#endif /* _MAP_H_ */

typedef uint8_t         u8;
typedef uint16_t        u16;
typedef int64_t         i64;

static char *sqlite3StrDup( const char *z ) {
    char *res = sqlite3_malloc( strlen(z)+1 );
    return strcpy( res, z );
}

/*
** These are copied verbatim from fun.c so as to not have the names exported
*/

/* LMH from sqlite3 3.3.13 */
/*
** This table maps from the first byte of a UTF-8 character to the number
** of trailing bytes expected. A value '4' indicates that the table key
** is not a legal first byte for a UTF-8 character.
*/
static const u8 xtra_utf8_bytes[256]  = {
/* 0xxxxxxx */
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,

/* 10wwwwww */
4, 4, 4, 4, 4, 4, 4, 4,     4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4,     4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4,     4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4,     4, 4, 4, 4, 4, 4, 4, 4,

/* 110yyyyy */
1, 1, 1, 1, 1, 1, 1, 1,     1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,     1, 1, 1, 1, 1, 1, 1, 1,

/* 1110zzzz */
2, 2, 2, 2, 2, 2, 2, 2,     2, 2, 2, 2, 2, 2, 2, 2,

/* 11110yyy */
3, 3, 3, 3, 3, 3, 3, 3,     4, 4, 4, 4, 4, 4, 4, 4,
};


/*
** This table maps from the number of trailing bytes in a UTF-8 character
** to an integer constant that is effectively calculated for each character
** read by a naive implementation of a UTF-8 character reader. The code
** in the READ_UTF8 macro explains things best.
*/
static const int xtra_utf8_bits[] =  {
  0,
  12416,          /* (0xC0 << 6) + (0x80) */
  925824,         /* (0xE0 << 12) + (0x80 << 6) + (0x80) */
  63447168        /* (0xF0 << 18) + (0x80 << 12) + (0x80 << 6) + 0x80 */
};

/*
** If a UTF-8 character contains N bytes extra bytes (N bytes follow
** the initial byte so that the total character length is N+1) then
** masking the character with utf8_mask[N] must produce a non-zero
** result.  Otherwise, we have an (illegal) overlong encoding.
*/
static const int utf_mask[] = {
  0x00000000,
  0xffffff80,
  0xfffff800,
  0xffff0000,
};

/* LMH salvaged from sqlite3 3.3.13 source code src/utf.c */
#define READ_UTF8(zIn, c) { \
  int xtra;                                            \
  c = *(zIn)++;                                        \
  xtra = xtra_utf8_bytes[c];                           \
  switch( xtra ){                                      \
    case 4: c = (int)0xFFFD; break;                    \
    case 3: c = (c<<6) + *(zIn)++;                     \
    case 2: c = (c<<6) + *(zIn)++;                     \
    case 1: c = (c<<6) + *(zIn)++;                     \
    c -= xtra_utf8_bits[xtra];                         \
    if( (utf_mask[xtra]&c)==0                          \
        || (c&0xFFFFF800)==0xD800                      \
        || (c&0xFFFFFFFE)==0xFFFE ){  c = 0xFFFD; }    \
  }                                                    \
}

static int sqlite3ReadUtf8(const unsigned char *z){
  int c;
  READ_UTF8(z, c);
  return c;
}

#define SKIP_UTF8(zIn) {                               \
  zIn += (xtra_utf8_bytes[*(u8 *)zIn] + 1);            \
}

/*
** pZ is a UTF-8 encoded unicode string. If nByte is less than zero,
** return the number of unicode characters in pZ up to (but not including)
** the first 0x00 byte. If nByte is not less than zero, return the
** number of unicode characters in the first nByte of pZ (or up to
** the first 0x00, whichever comes first).
*/
static int sqlite3Utf8CharLen(const char *z, int nByte){
  int r = 0;
  const char *zTerm;
  if( nByte>=0 ){
    zTerm = &z[nByte];
  }else{
    zTerm = (const char *)(-1);
  }
  assert( z<=zTerm );
  while( *z!=0 && z<zTerm ){
    SKIP_UTF8(z);
    r++;
  }
  return r;
}

/*
** X is a pointer to the first byte of a UTF-8 character.  Increment
** X so that it points to the next character.  This only works right
** if X points to a well-formed UTF-8 string.
*/
#define sqliteNextChar(X)  while( (0xc0&*++(X))==0x80 ){}
#define sqliteCharVal(X)   sqlite3ReadUtf8(X)

/*
** Given a string (s) in the first argument and an integer (n) in the second returns the
** string that constains s contatenated n times
*/
static void replicateFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  unsigned char *z;        /* input string */
  unsigned char *zo;       /* result string */
  i64 iCount;              /* times to repeat */
  i64 nLen;                /* length of the input string (no multibyte considerations) */
  i64 nTLen;               /* length of the result string (no multibyte considerations) */
  i64 i=0;

  if( argc!=2 || SQLITE_NULL==sqlite3_value_type(argv[0]) )
    return;

  iCount = sqlite3_value_int64(argv[1]);

  if( iCount<0 ){
    sqlite3_result_error(context, "domain error", -1);
  }else{

    nLen  = sqlite3_value_bytes(argv[0]);
    nTLen = nLen*iCount;
    z=sqlite3_malloc(nTLen+1);
    zo=sqlite3_malloc(nLen+1);
    if (!z || !zo){
      sqlite3_result_error_nomem(context);
      if (z) sqlite3_free(z);
      if (zo) sqlite3_free(zo);
      return;
    }
    strcpy((char*)zo, (char*)sqlite3_value_text(argv[0]));

    for(i=0; i<iCount; ++i){
      strcpy((char*)(z+i*nLen), (char*)zo);
    }

    sqlite3_result_text(context, (char*)z, -1, SQLITE_TRANSIENT);
    sqlite3_free(z);
    sqlite3_free(zo);
  }
}

/*
** Some systems (win32 among others) don't have an isblank function, this will emulate it.
** This function is not UFT-8 safe since it only analyses a byte character.
*/
#ifndef HAVE_ISBLANK
static int isblank(char c){
  return( ' '==c || '\t'==c );
}
#endif

static void properFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const unsigned char *z;     /* input string */
  unsigned char *zo;          /* output string */
  unsigned char *zt;          /* iterator */
  char r;
  int c=1;

  assert( argc==1);
  if( SQLITE_NULL==sqlite3_value_type(argv[0]) ){
    sqlite3_result_null(context);
    return;
  }

  z = sqlite3_value_text(argv[0]);
  zo = (unsigned char *)sqlite3StrDup((char *) z);
  if (!zo) {
    sqlite3_result_error_nomem(context);
    return;
  }
  zt = zo;

  while( (r = *(z++))!=0 ){
    if( isblank(r) ){
      c=1;
    }else{
      if( c==1 ){
        r = toupper(r);
      }else{
        r = tolower(r);
      }
      c=0;
    }
    *(zt++) = r;
  }
  *zt = '\0';

  sqlite3_result_text(context, (char*)zo, -1, SQLITE_TRANSIENT);
  sqlite3_free(zo);
}

/*
** given an input string (s) and an integer (n) adds spaces at the begining of  s
** until it has a length of n characters.
** When s has a length >=n it's a NOP
** padl(NULL) = NULL
*/
static void padlFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  i64 ilen;          /* length to pad to */
  i64 zl;            /* length of the input string (UTF-8 chars) */
  int i = 0;
  const char *zi;    /* input string */
  char *zo;          /* output string */
  char *zt;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0]) == SQLITE_NULL ){
    sqlite3_result_null(context);
  }else{
    zi = (char *)sqlite3_value_text(argv[0]);
    ilen = sqlite3_value_int64(argv[1]);
    /* check domain */
    if(ilen<0){
      sqlite3_result_error(context, "domain error", -1);
      return;
    }
    zl = sqlite3Utf8CharLen(zi, -1);
    if( zl>=ilen ){
      /* string is longer than the requested pad length, return the same string (dup it) */
      zo = sqlite3StrDup(zi);
      if (!zo){
        sqlite3_result_error_nomem(context);
        return;
      }
      sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    }else{
      zo = sqlite3_malloc(strlen(zi)+ilen-zl+1);
      if (!zo){
        sqlite3_result_error_nomem(context);
        return;
      }
      zt = zo;
      for(i=1; i+zl<=ilen; ++i){
        *(zt++)=' ';
      }
      /* no need to take UTF-8 into consideration here */
      strcpy(zt,zi);
    }
    sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    sqlite3_free(zo);
  }
}

/*
** given an input string (s) and an integer (n) appends spaces at the end of  s
** until it has a length of n characters.
** When s has a length >=n it's a NOP
** padl(NULL) = NULL
*/
static void padrFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  i64 ilen;          /* length to pad to */
  i64 zl;            /* length of the input string (UTF-8 chars) */
  i64 zll;           /* length of the input string (bytes) */
  int i = 0;
  const char *zi;    /* input string */
  char *zo;          /* output string */
  char *zt;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0]) == SQLITE_NULL ){
    sqlite3_result_null(context);
  }else{
    zi = (char *)sqlite3_value_text(argv[0]);
    ilen = sqlite3_value_int64(argv[1]);
    /* check domain */
    if(ilen<0){
      sqlite3_result_error(context, "domain error", -1);
      return;
    }
    zl = sqlite3Utf8CharLen(zi, -1);
    if( zl>=ilen ){
      /* string is longer than the requested pad length, return the same string (dup it) */
      zo = sqlite3StrDup(zi);
      if (!zo){
        sqlite3_result_error_nomem(context);
        return;
      }
      sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    }else{
      zll = strlen(zi);
      zo = sqlite3_malloc(zll+ilen-zl+1);
      if (!zo){
        sqlite3_result_error_nomem(context);
        return;
      }
      zt = strcpy(zo,zi)+zll;
      for(i=1; i+zl<=ilen; ++i){
        *(zt++) = ' ';
      }
      *zt = '\0';
    }
    sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    sqlite3_free(zo);
  }
}

/*
** given an input string (s) and an integer (n) appends spaces at the end of  s
** and adds spaces at the begining of s until it has a length of n characters.
** Tries to add has many characters at the left as at the right.
** When s has a length >=n it's a NOP
** padl(NULL) = NULL
*/
static void padcFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  i64 ilen;           /* length to pad to */
  i64 zl;             /* length of the input string (UTF-8 chars) */
  i64 zll;            /* length of the input string (bytes) */
  int i = 0;
  const char *zi;     /* input string */
  char *zo;           /* output string */
  char *zt;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0]) == SQLITE_NULL ){
    sqlite3_result_null(context);
  }else{
    zi = (char *)sqlite3_value_text(argv[0]);
    ilen = sqlite3_value_int64(argv[1]);
    /* check domain */
    if(ilen<0){
      sqlite3_result_error(context, "domain error", -1);
      return;
    }
    zl = sqlite3Utf8CharLen(zi, -1);
    if( zl>=ilen ){
      /* string is longer than the requested pad length, return the same string (dup it) */
      zo = sqlite3StrDup(zi);
      if (!zo){
        sqlite3_result_error_nomem(context);
        return;
      }
      sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    }else{
      zll = strlen(zi);
      zo = sqlite3_malloc(zll+ilen-zl+1);
      if (!zo){
        sqlite3_result_error_nomem(context);
        return;
      }
      zt = zo;
      for(i=1; 2*i+zl<=ilen; ++i){
        *(zt++) = ' ';
      }
      strcpy(zt, zi);
      zt+=zll;
      for(; i+zl<=ilen; ++i){
        *(zt++) = ' ';
      }
      *zt = '\0';
    }
    sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    sqlite3_free(zo);
  }
}

/*
** given 2 string (s1,s2) returns the string s1 with the characters NOT in s2 removed
** assumes strings are UTF-8 encoded
*/
static void strfilterFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const char *zi1;        /* first parameter string (searched string) */
  const char *zi2;        /* second parameter string (vcontains valid characters) */
  const char *z1;
  const char *z21;
  const char *z22;
  char *zo;               /* output string */
  char *zot;
  int c1 = 0;
  int c2 = 0;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0]) == SQLITE_NULL || sqlite3_value_type(argv[1]) == SQLITE_NULL ){
    sqlite3_result_null(context);
  }else{
    zi1 = (char *)sqlite3_value_text(argv[0]);
    zi2 = (char *)sqlite3_value_text(argv[1]);
    /*
    ** maybe I could allocate less, but that would imply 2 passes, rather waste
    ** (possibly) some memory
    */
    zo = sqlite3_malloc(strlen(zi1)+1);
    if (!zo){
      sqlite3_result_error_nomem(context);
      return;
    }
    zot = zo;
    z1 = zi1;
    while( (c1=sqliteCharVal((unsigned char *)z1))!=0 ){
      z21=zi2;
      while( (c2=sqliteCharVal((unsigned char *)z21))!=0 && c2!=c1 ){
        sqliteNextChar(z21);
      }
      if( c2!=0){
        z22=z21;
        sqliteNextChar(z22);
        strncpy(zot, z21, z22-z21);
        zot+=z22-z21;
      }
      sqliteNextChar(z1);
    }
    *zot = '\0';

    sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
    sqlite3_free(zo);
  }
}

/*
** Given a string z1, retutns the (0 based) index of it's first occurence
** in z2 after the first s characters.
** Returns -1 when there isn't a match.
** updates p to point to the character where the match occured.
** This is an auxiliary function.
*/
static int _substr(const char* z1, const char* z2, int s, const char** p){
UNUSED(p);
  int c = 0;
  int rVal=-1;
  const char* zt1;
  const char* zt2;
  int c1,c2;

  if( '\0'==*z1 ){
    return -1;
  }

  while( (sqliteCharVal((unsigned char *)z2) != 0) && (c++)<s){
    sqliteNextChar(z2);
  }

  c = 0;
  while( (sqliteCharVal((unsigned char *)z2)) != 0 ){
    zt1 = z1;
    zt2 = z2;

    do{
      c1 = sqliteCharVal((unsigned char *)zt1);
      c2 = sqliteCharVal((unsigned char *)zt2);
      sqliteNextChar(zt1);
      sqliteNextChar(zt2);
    }while( c1 == c2 && c1 != 0 && c2 != 0 );

    if( c1 == 0 ){
      rVal = c;
      break;
    }

    sqliteNextChar(z2);
    ++c;
  }
  if(p){
    *p=z2;
  }
  return rVal >=0 ? rVal+s : rVal;
}

/*
** given 2 input strings (s1,s2) and an integer (n) searches from the nth character
** for the string s1. Returns the position where the match occured.
** Characters are counted from 1.
** 0 is returned when no match occurs.
*/

static void charindexFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const u8 *z1;          /* s1 string */
  u8 *z2;                /* s2 string */
  int s=0;
  int rVal=0;

  assert( argc==3 ||argc==2);

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) || SQLITE_NULL==sqlite3_value_type(argv[1])){
    sqlite3_result_null(context);
    return;
  }

  z1 = sqlite3_value_text(argv[0]);
  if( z1==0 ) return;
  z2 = (u8*) sqlite3_value_text(argv[1]);
  if(argc==3){
    s = sqlite3_value_int(argv[2])-1;
    if(s<0){
      s=0;
    }
  }else{
    s = 0;
  }

  rVal = _substr((char *)z1,(char *)z2,s,NULL);
  sqlite3_result_int(context, rVal+1);
}

/*
** given a string (s) and an integer (n) returns the n leftmost (UTF-8) characters
** if the string has a length<=n or is NULL this function is NOP
*/
static void leftFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  int c=0;
  int cc=0;
  int l=0;
  const unsigned char *z;       /* input string */
  const unsigned char *zt;
  unsigned char *rz;            /* output string */

  assert( argc==2);

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) || SQLITE_NULL==sqlite3_value_type(argv[1])){
    sqlite3_result_null(context);
    return;
  }

  z  = sqlite3_value_text(argv[0]);
  l  = sqlite3_value_int(argv[1]);
  zt = z;

  while( sqliteCharVal(zt) && c++<l)
    sqliteNextChar(zt);

  cc=zt-z;

  rz = sqlite3_malloc(zt-z+1);
  if (!rz){
    sqlite3_result_error_nomem(context);
    return;
  }
  strncpy((char*) rz, (char*) z, zt-z);
  *(rz+cc) = '\0';
  sqlite3_result_text(context, (char*)rz, -1, SQLITE_TRANSIENT);
  sqlite3_free(rz);
}

/*
** given a string (s) and an integer (n) returns the n rightmost (UTF-8) characters
** if the string has a length<=n or is NULL this function is NOP
*/
static void rightFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  int l=0;
  int c=0;
  int cc=0;
  const char *z;
  const char *zt;
  const char *ze;
  char *rz;

  assert( argc==2);

  if( SQLITE_NULL == sqlite3_value_type(argv[0]) || SQLITE_NULL == sqlite3_value_type(argv[1])){
    sqlite3_result_null(context);
    return;
  }

  z  = (char *)sqlite3_value_text(argv[0]);
  l  = sqlite3_value_int(argv[1]);
  zt = z;

  while( sqliteCharVal((unsigned char *)zt)!=0){
    sqliteNextChar(zt);
    ++c;
  }

  ze = zt;
  zt = z;

  cc=c-l;
  if(cc<0)
    cc=0;

  while( cc-- > 0 ){
    sqliteNextChar(zt);
  }

  rz = sqlite3_malloc(ze-zt+1);
  if (!rz){
    sqlite3_result_error_nomem(context);
    return;
  }
  strcpy((char*) rz, (char*) (zt));
  sqlite3_result_text(context, (char*)rz, -1, SQLITE_TRANSIENT);
  sqlite3_free(rz);
}

#ifndef HAVE_TRIM
/*
** removes the whitespaces at the begining of a string.
*/
const char* ltrim(const char* s){
UNUSED(s);
  while( *s==' ' )
    ++s;
  return s;
}

/*
** removes the whitespaces at the end of a string.
** !mutates the input string!
*/
static void rtrim(char* s){
UNUSED(s);
  char* ss = s+strlen(s)-1;
  while( ss>=s && *ss==' ' )
    --ss;
  *(ss+1)='\0';
}

/*
**  Removes the whitespace at the begining of a string
*/
static void ltrimFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const char *z;

  assert( argc==1);

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) ){
    sqlite3_result_null(context);
    return;
  }
  z = sqlite3_value_text(argv[0]);
  sqlite3_result_text(context, ltrim(z), -1, SQLITE_TRANSIENT);
}

/*
**  Removes the whitespace at the end of a string
*/
static void rtrimFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const char *z;
  char *rz;
  /* try not to change data in argv */

  assert( argc==1);

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) ){
    sqlite3_result_null(context);
    return;
  }
  z = sqlite3_value_text(argv[0]);
  rz = sqlite3StrDup(z);
  rtrim(rz);
  sqlite3_result_text(context, rz, -1, SQLITE_TRANSIENT);
  sqlite3_free(rz);
}

/*
**  Removes the whitespace at the begining and end of a string
*/
static void trimFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const char *z;
  char *rz;
  /* try not to change data in argv */

  assert( argc==1);

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) ){
    sqlite3_result_null(context);
    return;
  }
  z = sqlite3_value_text(argv[0]);
  rz = sqlite3StrDup(z);
  rtrim(rz);
  sqlite3_result_text(context, ltrim(rz), -1, SQLITE_TRANSIENT);
  sqlite3_free(rz);
}
#endif

/*
** given a pointer to a string s1, the length of that string (l1), a new string (s2)
** and it's length (l2) appends s2 to s1.
** All lengths in bytes.
** This is just an auxiliary function
*/
// static void _append(char **s1, int l1, const char *s2, int l2){
//   *s1 = realloc(*s1, (l1+l2+1)*sizeof(char));
//   strncpy((*s1)+l1, s2, l2);
//   *(*(s1)+l1+l2) = '\0';
// }

#ifndef HAVE_TRIM

/*
** given strings s, s1 and s2 replaces occurrences of s1 in s by s2
*/
static void replaceFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const char *z1;     /* string s (first parameter) */
  const char *z2;     /* string s1 (second parameter) string to look for */
  const char *z3;     /* string s2 (third parameter) string to replace occurrences of s1 with */
  int lz1;
  int lz2;
  int lz3;
  int lzo=0;
  char *zo=0;
  int ret=0;
  const char *zt1;
  const char *zt2;

  assert( 3==argc );

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) ){
    sqlite3_result_null(context);
    return;
  }

  z1 = sqlite3_value_text(argv[0]);
  z2 = sqlite3_value_text(argv[1]);
  z3 = sqlite3_value_text(argv[2]);
  /* handle possible null values */
  if( 0==z2 ){
    z2="";
  }
  if( 0==z3 ){
    z3="";
  }

  lz1 = strlen(z1);
  lz2 = strlen(z2);
  lz3 = strlen(z3);

#if 0
  /* special case when z2 is empty (or null) nothing will be changed */
  if( 0==lz2 ){
    sqlite3_result_text(context, z1, -1, SQLITE_TRANSIENT);
    return;
  }
#endif

  zt1=z1;
  zt2=z1;

  while(1){
    ret=_substr(z2,zt1 , 0, &zt2);

    if( ret<0 )
      break;

    _append(&zo, lzo, zt1, zt2-zt1);
    lzo+=zt2-zt1;
    _append(&zo, lzo, z3, lz3);
    lzo+=lz3;

    zt1=zt2+lz2;
  }
  _append(&zo, lzo, zt1, lz1-(zt1-z1));
  sqlite3_result_text(context, zo, -1, SQLITE_TRANSIENT);
  sqlite3_free(zo);
}
#endif

/*
** given a string returns the same string but with the characters in reverse order
*/
static void reverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  const char *z;
  const char *zt;
  char *rz;
  char *rzt;
  int l = 0;
  int i = 0;

  assert( 1==argc );

  if( SQLITE_NULL==sqlite3_value_type(argv[0]) ){
    sqlite3_result_null(context);
    return;
  }
  z = (char *)sqlite3_value_text(argv[0]);
  l = strlen(z);
  rz = sqlite3_malloc(l+1);
  if (!rz){
    sqlite3_result_error_nomem(context);
    return;
  }
  rzt = rz+l;
  *(rzt--) = '\0';

  zt=z;
  while( sqliteCharVal((unsigned char *)zt)!=0 ){
    z=zt;
    sqliteNextChar(zt);
    for(i=1; zt-i>=z; ++i){
      *(rzt--)=*(zt-i);
    }
  }

  sqlite3_result_text(context, rz, -1, SQLITE_TRANSIENT);
  sqlite3_free(rz);
}

/*
** An instance of the following structure holds the context of a
** stdev() or variance() aggregate computation.
** implementaion of http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Algorithm_II
** less prone to rounding errors
*/
typedef struct StdevCtx StdevCtx;
struct StdevCtx {
  double rM;
  double rS;
  i64 cnt;          /* number of elements */
};

/*
** An instance of the following structure holds the context of a
** mode() or median() aggregate computation.
** Depends on structures defined in map.c (see map & map)
** These aggregate functions only work for integers and floats although
** they could be made to work for strings. This is usually considered meaningless.
** Only usuall order (for median), no use of collation functions (would this even make sense?)
*/
typedef struct ModeCtx ModeCtx;
struct ModeCtx {
  i64 riM;            /* integer value found so far */
  double rdM;         /* double value found so far */
  i64 cnt;            /* number of elements so far */
  double pcnt;        /* number of elements smaller than a percentile */
  i64 mcnt;           /* maximum number of occurrences (for mode) */
  i64 mn;             /* number of occurrences (for mode and percentiles) */
  i64 is_double;      /* whether the computation is being done for doubles (>0) or integers (=0) */
  map* m;             /* map structure used for the computation */
  int done;           /* whether the answer has been found */
};

/*
** called for each value received during a calculation of stdev or variance
*/
static void varianceStep(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  StdevCtx *p;

  double delta;
  double x;

  assert( argc==1 );
  p = sqlite3_aggregate_context(context, sizeof(*p));
  /* only consider non-null values */
  if( SQLITE_NULL != sqlite3_value_numeric_type(argv[0]) ){
    p->cnt++;
    x = sqlite3_value_double(argv[0]);
    delta = (x-p->rM);
    p->rM += delta/p->cnt;
    p->rS += delta*(x-p->rM);
  }
}

/*
** called for each value received during a calculation of mode of median
*/
static void modeStep(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  ModeCtx *p;
  i64 xi=0;
  double xd=0.0;
  i64 *iptr;
  double *dptr;
  int type;

  assert( argc==1 );
  type = sqlite3_value_numeric_type(argv[0]);

  if( type == SQLITE_NULL)
    return;

  p = sqlite3_aggregate_context(context, sizeof(*p));

  if( 0==(p->m) ){
    p->m = calloc(1, sizeof(map));
    if( type==SQLITE_INTEGER ){
      /* map will be used for integers */
      *(p->m) = map_make(int_cmp);
      p->is_double = 0;
    }else{
      p->is_double = 1;
      /* map will be used for doubles */
      *(p->m) = map_make(double_cmp);
    }
  }

  ++(p->cnt);

  if( 0==p->is_double ){
    xi = sqlite3_value_int64(argv[0]);
    iptr = (i64*)calloc(1,sizeof(i64));
    *iptr = xi;
    map_insert(p->m, iptr);
  }else{
    xd = sqlite3_value_double(argv[0]);
    dptr = (double*)calloc(1,sizeof(double));
    *dptr = xd;
    map_insert(p->m, dptr);
  }
}

/*
**  Auxiliary function that iterates all elements in a map and finds the mode
**  (most frequent value)
*/
static void modeIterate(void* e, i64 c, void* pp){
  i64 ei;
  double ed;
  ModeCtx *p = (ModeCtx*)pp;

  if( 0==p->is_double ){
    ei = *(int*)(e);

	if( p->mcnt==c ){
      ++p->mn;
    }else if( p->mcnt<c ){
      p->riM = ei;
      p->mcnt = c;
	  p->mn=1;
    }
  }else{
    ed = *(double*)(e);

	if( p->mcnt==c ){
      ++p->mn;
    }else if(p->mcnt<c){
      p->rdM = ed;
      p->mcnt = c;
	  p->mn=1;
    }
  }
}

/*
**  Auxiliary function that iterates all elements in a map and finds the median
**  (the value such that the number of elements smaller is equal the the number of
**  elements larger)
*/
static void medianIterate(void* e, i64 c, void* pp){
  i64 ei;
  double ed;
  double iL;
  double iR;
  int il;
  int ir;
  ModeCtx *p = (ModeCtx*)pp;

  if(p->done>0)
    return;

  iL = p->pcnt;
  iR = p->cnt - p->pcnt;
  il = p->mcnt + c;
  ir = p->cnt - p->mcnt;

  if( il >= iL ){
    if( ir >= iR ){
    ++p->mn;
      if( 0==p->is_double ){
        ei = *(int*)(e);
        p->riM += ei;
      }else{
        ed = *(double*)(e);
        p->rdM += ed;
      }
    }else{
      p->done=1;
    }
  }
  p->mcnt+=c;
}

/*
** Returns the mode value
*/
static void modeFinalize(sqlite3_context *context){
  ModeCtx *p;
  p = sqlite3_aggregate_context(context, 0);
  if( p && p->m ){
    map_iterate(p->m, modeIterate, p);
    map_destroy(p->m);
    free(p->m);

    if( 1==p->mn ){
      if( 0==p->is_double )
        sqlite3_result_int64(context, p->riM);
      else
        sqlite3_result_double(context, p->rdM);
    }
  }
}

/*
** auxiliary function for percentiles
*/
static void _medianFinalize(sqlite3_context *context){
  ModeCtx *p;
  p = (ModeCtx*) sqlite3_aggregate_context(context, 0);
  if( p && p->m ){
    p->done=0;
    map_iterate(p->m, medianIterate, p);
    map_destroy(p->m);
    free(p->m);

    if( 0==p->is_double )
      if( 1==p->mn )
      	sqlite3_result_int64(context, p->riM);
      else
      	sqlite3_result_double(context, p->riM*1.0/p->mn);
    else
      sqlite3_result_double(context, p->rdM/p->mn);
  }
}

/*
** Returns the median value
*/
static void medianFinalize(sqlite3_context *context){
  ModeCtx *p;
  p = (ModeCtx*) sqlite3_aggregate_context(context, 0);
  if( p!=0 ){
    p->pcnt = (p->cnt)/2.0;
    _medianFinalize(context);
  }
}

/*
** Returns the lower_quartile value
*/
static void lower_quartileFinalize(sqlite3_context *context){
  ModeCtx *p;
  p = (ModeCtx*) sqlite3_aggregate_context(context, 0);
  if( p!=0 ){
    p->pcnt = (p->cnt)/4.0;
    _medianFinalize(context);
  }
}

/*
** Returns the upper_quartile value
*/
static void upper_quartileFinalize(sqlite3_context *context){
  ModeCtx *p;
  p = (ModeCtx*) sqlite3_aggregate_context(context, 0);
  if( p!=0 ){
    p->pcnt = (p->cnt)*3/4.0;
    _medianFinalize(context);
  }
}

/*
** Returns the stdev value
*/
static void stdevFinalize(sqlite3_context *context){
  StdevCtx *p;
  p = sqlite3_aggregate_context(context, 0);
  if( p && p->cnt>1 ){
    sqlite3_result_double(context, sqrt(p->rS/(p->cnt-1)));
  }else{
    sqlite3_result_double(context, 0.0);
  }
}

/*
** Returns the variance value
*/
static void varianceFinalize(sqlite3_context *context){
  StdevCtx *p;
  p = sqlite3_aggregate_context(context, 0);
  if( p && p->cnt>1 ){
    sqlite3_result_double(context, p->rS/(p->cnt-1));
  }else{
    sqlite3_result_double(context, 0.0);
  }
}

#ifdef SQLITE_SOUNDEX

/* relicoder factored code */
/*
** Calculates the soundex value of a string
*/

static void soundex(const u8 *zIn, char *zResult){
  int i, j;
  static const unsigned char iCode[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0,
    1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0,
    1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2, 0, 0, 0, 0, 0,
  };

  for(i=0; zIn[i] && !isalpha(zIn[i]); i++){}
  if( zIn[i] ){
    zResult[0] = toupper(zIn[i]);
    for(j=1; j<4 && zIn[i]; i++){
      int code = iCode[zIn[i]&0x7f];
      if( code>0 ){
        zResult[j++] = code + '0';
      }
    }
    while( j<4 ){
      zResult[j++] = '0';
    }
    zResult[j] = 0;
  }else{
    strcpy(zResult, "?000");
  }
}

/*
** computes the number of different characters between the soundex value fo 2 strings
*/
static void differenceFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
UNUSED(argc);
  char zResult1[8];
  char zResult2[8];
  char *zR1 = zResult1;
  char *zR2 = zResult2;
  int rVal = 0;
  int i = 0;
  const u8 *zIn1;
  const u8 *zIn2;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0])==SQLITE_NULL || sqlite3_value_type(argv[1])==SQLITE_NULL ){
    sqlite3_result_null(context);
    return;
  }

  zIn1 = (u8*)sqlite3_value_text(argv[0]);
  zIn2 = (u8*)sqlite3_value_text(argv[1]);

  soundex(zIn1, zR1);
  soundex(zIn2, zR2);

  for(i=0; i<4; ++i){
    if( sqliteCharVal((unsigned char *)zR1)==sqliteCharVal((unsigned char *)zR2) )
      ++rVal;
    sqliteNextChar(zR1);
    sqliteNextChar(zR2);
  }
  sqlite3_result_int(context, rVal);
}
#endif

/*
** This function registered all of the above C functions as SQL
** functions.  This should be the only routine in this file with
** external linkage.
*/
static int RegisterExtensionFunctions(sqlite3 *db){
  static const struct FuncDef {
     char *zName;
     signed char nArg;
     u8 argType;           /* 0: none.  1: db  2: (-1) */
     u8 eTextRep;          /* 1: UTF-16.  0: UTF-8 */
     u8 needCollSeq;
     void (*xFunc)(sqlite3_context*,int,sqlite3_value **);
  } aFuncs[] = {
    /* math.h */
// hack-sqlite - deduplicate math-functions
    { "difference",         2, 0, SQLITE_UTF8,    0, differenceFunc},


    /* string */
    { "replicate",          2, 0, SQLITE_UTF8,    0, replicateFunc },
    { "charindex",          2, 0, SQLITE_UTF8,    0, charindexFunc },
    { "charindex",          3, 0, SQLITE_UTF8,    0, charindexFunc },
    { "leftstr",            2, 0, SQLITE_UTF8,    0, leftFunc },
    { "rightstr",           2, 0, SQLITE_UTF8,    0, rightFunc },
#ifndef HAVE_TRIM
    { "ltrim",              1, 0, SQLITE_UTF8,    0, ltrimFunc },
    { "rtrim",              1, 0, SQLITE_UTF8,    0, rtrimFunc },
    { "trim",               1, 0, SQLITE_UTF8,    0, trimFunc },
    { "replace",            3, 0, SQLITE_UTF8,    0, replaceFunc },
#endif
    { "reverse",            1, 0, SQLITE_UTF8,    0, reverseFunc },
    { "proper",             1, 0, SQLITE_UTF8,    0, properFunc },
    { "padl",               2, 0, SQLITE_UTF8,    0, padlFunc },
    { "padr",               2, 0, SQLITE_UTF8,    0, padrFunc },
    { "padc",               2, 0, SQLITE_UTF8,    0, padcFunc },
    { "strfilter",          2, 0, SQLITE_UTF8,    0, strfilterFunc },
  };
  /* Aggregate functions */
  static const struct FuncDefAgg {
    char *zName;
    signed char nArg;
    u8 argType;
    u8 needCollSeq;
    void (*xStep)(sqlite3_context*,int,sqlite3_value**);
    void (*xFinalize)(sqlite3_context*);
  } aAggs[] = {
    { "stdev",            1, 0, 0, varianceStep, stdevFinalize  },
    { "variance",         1, 0, 0, varianceStep, varianceFinalize  },
    { "mode",             1, 0, 0, modeStep,     modeFinalize  },
    { "median",           1, 0, 0, modeStep,     medianFinalize  },
    { "lower_quartile",   1, 0, 0, modeStep,     lower_quartileFinalize  },
    { "upper_quartile",   1, 0, 0, modeStep,     upper_quartileFinalize  },
  };
  int i;

  for(i=0; i<sizeof(aFuncs)/sizeof(aFuncs[0]); i++){
    void *pArg = 0;
    switch( aFuncs[i].argType ){
      case 1: pArg = db; break;
      case 2: pArg = (void *)(-1); break;
    }
    //sqlite3CreateFunc
    /* LMH no error checking */
    sqlite3_create_function(db, aFuncs[i].zName, aFuncs[i].nArg,
        aFuncs[i].eTextRep, pArg, aFuncs[i].xFunc, 0, 0);
#if 0
    if( aFuncs[i].needCollSeq ){
      struct FuncDef *pFunc = sqlite3FindFunction(db, aFuncs[i].zName,
          strlen(aFuncs[i].zName), aFuncs[i].nArg, aFuncs[i].eTextRep, 0);
      if( pFunc && aFuncs[i].needCollSeq ){
        pFunc->needCollSeq = 1;
      }
    }
#endif
  }

  for(i=0; i<sizeof(aAggs)/sizeof(aAggs[0]); i++){
    void *pArg = 0;
    switch( aAggs[i].argType ){
      case 1: pArg = db; break;
      case 2: pArg = (void *)(-1); break;
    }
    //sqlite3CreateFunc
    /* LMH no error checking */
    sqlite3_create_function(db, aAggs[i].zName, aAggs[i].nArg, SQLITE_UTF8,
        pArg, 0, aAggs[i].xStep, aAggs[i].xFinalize);
#if 0
    if( aAggs[i].needCollSeq ){
      struct FuncDefAgg *pFunc = sqlite3FindFunction( db, aAggs[i].zName,
          strlen(aAggs[i].zName), aAggs[i].nArg, SQLITE_UTF8, 0);
      if( pFunc && aAggs[i].needCollSeq ){
        pFunc->needCollSeq = 1;
      }
    }
#endif
  }
  return 0;
}

#ifdef COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE
static int sqlite3_extension_init(
    sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi){
UNUSED(pzErrMsg);
  UNUSED(pApi);
  RegisterExtensionFunctions(db);
  return 0;
}
#endif /* COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE */

map map_make(cmp_func cmp){
  map r;
  r.cmp=cmp;
  r.base = 0;

  return r;
}

void* xcalloc(size_t nmemb, size_t size, char* s){
UNUSED(s);
  void* ret = calloc(nmemb, size);
  return ret;
}

static void xfree(void* p){
UNUSED(p);
  free(p);
}

static void node_insert(node** n, cmp_func cmp, void *e){
  int c;
  node* nn;
  if(*n==0){
    nn = (node*)xcalloc(1,sizeof(node), "for node");
    nn->data = e;
    nn->count = 1;
    *n=nn;
  }else{
    c=cmp((*n)->data,e);
    if(0==c){
      ++((*n)->count);
      xfree(e);
    }else if(c>0){
      /* put it right here */
      node_insert(&((*n)->l), cmp, e);
    }else{
      node_insert(&((*n)->r), cmp, e);
    }
  }
}

static void map_insert(map *m, void *e){
  node_insert(&(m->base), m->cmp, e);
}

static void node_iterate(node *n, map_iterator iter, void* p){
UNUSED(p);
  if(n){
    if(n->l)
      node_iterate(n->l, iter, p);
    iter(n->data, n->count, p);
    if(n->r)
      node_iterate(n->r, iter, p);
  }
}

static void map_iterate(map *m, map_iterator iter, void* p){
UNUSED(p);
  node_iterate(m->base, iter, p);
}

static void node_destroy(node *n){
  if(0!=n){
    xfree(n->data);
    if(n->l)
      node_destroy(n->l);
    if(n->r)
      node_destroy(n->r);

    xfree(n);
  }
}

static void map_destroy(map *m){
  node_destroy(m->base);
}

static int int_cmp(const void *a, const void *b){
  int64_t aa = *(int64_t *)(a);
  int64_t bb = *(int64_t *)(b);
  /* printf("cmp %d <=> %d\n",aa,bb); */
  if(aa==bb)
    return 0;
  else if(aa<bb)
    return -1;
  else
    return 1;
}

static int double_cmp(const void *a, const void *b){
  double aa = *(double *)(a);
  double bb = *(double *)(b);
  /* printf("cmp %d <=> %d\n",aa,bb); */
  if(aa==bb)
    return 0;
  else if(aa<bb)
    return -1;
  else
    return 1;
}

// hack-sqlite - fix warning


/*
file none
*/
/*jslint-enable*/


/* *INDENT-ON* */
int sqlite3_ext_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int errcode = 0;
    sqlite3_api = pApi;
    errcode = sqlite3_compress_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_csv_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_extension_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_noop_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_regexp_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_series_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    errcode = sqlite3_shathree_init(db, pzErrMsg, pApi);
    IS_SQLITE_OK_OR_RETURN_RC(errcode);
    return 0;
}
