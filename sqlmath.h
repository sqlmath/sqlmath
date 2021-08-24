/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/LICENSE.md"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/carray.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/compress.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/csv.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/json1.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/noop.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/regexp.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/templatevtab.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/tree/version-3.36.0/contrib/download/extension-functions.c/download/extension-functions.c",
            "url2": "https://www.sqlite.org/contrib/download/extension-functions.c/download/extension-functions.c?get=25"
        }
    ],
    "replaceList": [
        {
            "aa": "SQLITE_EXTENSION_INIT",
            "bb": "// $&",
            "flags": "g"
        },
        {
            "aa": "(0, *?\\/\\* xRename *?\\*\\/)(\\n\\};)",
            "bb": "$1\n0, 0, 0, 0$2",
            "flags": "g"
        }
    ]
}
-        case RE_OP_CC_INC: {
+// hack-sqlite
+        // fall through
+        case RE_OP_CC_INC: {

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
+// hack-sqlite
+    { "atn2",               2, 0, SQLITE_UTF8,    0, atn2Func  },
+    { "cot",                1, 0, SQLITE_UTF8,    0, cotFunc },
+    { "coth",               1, 0, SQLITE_UTF8,    0, cothFunc },
+    { "difference",         2, 0, SQLITE_UTF8,    0, differenceFunc},
+    { "sign",               1, 0, SQLITE_UTF8,    0, signFunc },
+    { "square",             1, 0, SQLITE_UTF8,    0, squareFunc },

-  READ_UTF8(z, c);
+// hack-sqlite
+/\\* LMH salvaged from sqlite3 3.3.13 source code src/utf.c *\\/
+  int xtra;
+  c = *(z)++;
+  xtra = xtra_utf8_bytes[c];
+  switch( xtra ){
+    case 4: c = (int)0xFFFD; break;
+    case 3: c = (c<<6) + *(z)++;
+    // fall through
+    case 2: c = (c<<6) + *(z)++;
+    // fall through
+    case 1: c = (c<<6) + *(z)++;
+    c -= xtra_utf8_bits[xtra];
+    if( (utf_mask[xtra]&c)==0
+        || (c&0xFFFFF800)==0xD800
+        || (c&0xFFFFFFFE)==0xFFFE ){  c = 0xFFFD; }
+  }

-  for(i=3; i<argc; i++){
+// hack-sqlite
+  for(i=3; i<(unsigned int)argc; i++){

-  if( !p->bStatic ) sqlite3_free(p->zBuf);
+// hack-sqlite
+  if (p == NULL) {return;}
+  if( !p->bStatic ) sqlite3_free(p->zBuf);

-  int i, j;                  /\\* Loop counters *\\/
+// hack-sqlite
+  unsigned int i, j;                  /\\* Loop counters *\\/

-  int i;
-
-  for(i=0; i<sizeof(aFuncs)/sizeof(aFuncs[0]); i++){
+// hack-sqlite
+  unsigned int i;
+
+  for(i=0; i<sizeof(aFuncs)/sizeof(aFuncs[0]); i++){

-  int i;
-
-  pIn = sqlite3_value_blob(argv[0]);
+// hack-sqlite
+  unsigned int i;
+
+  pIn = sqlite3_value_blob(argv[0]);

-#define GEN_MATH_WRAP_DOUBLE_1(name, function) \
+// hack-sqlite
+#define GEN_MATH_WRAP_DOUBLE_1(name, function)
+#define GEN_MATH_WRAP_DOUBLE_2(name, function) \

-#ifndef deliberate_fall_through
-# define deliberate_fall_through
-#endif
+#ifndef deliberate_fall_through
+// hack-sqlite
+#ifdef __GNUC__
+# define deliberate_fall_through __attribute__((fallthrough));
+#else
+# define deliberate_fall_through
+#endif
+#endif

-// SQLITE_EXTENSION_INIT1
+// SQLITE_EXTENSION_INIT1
+// hack-sqlite
+const sqlite3_api_routines *sqlite3_api=0;

-GEN_MATH_WRAP_DOUBLE_1(cotFunc, cot)
+// hack-sqlite
+GEN_MATH_WRAP_DOUBLE_2(cotFunc, cot)

-GEN_MATH_WRAP_DOUBLE_1(cothFunc, coth)
+// hack-sqlite
+GEN_MATH_WRAP_DOUBLE_2(cothFunc, coth)

-int sqlite3_extension_init(
-    sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi){
-  // SQLITE_EXTENSION_INIT2(pApi);
+// hack-sqlite
+int sqlite3_sqlmath_init(sqlite3*, char**, const sqlite3_api_routines*);
+int sqlite3_extension_functions_init(
+    sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi){
+  // SQLITE_EXTENSION_INIT2(pApi);
+  sqlite3_api=pApi;
+  sqlite3_auto_extension((void(*)(void))sqlite3_carray_init);
+  sqlite3_auto_extension((void(*)(void))sqlite3_compress_init);
+  sqlite3_auto_extension((void(*)(void))sqlite3_csv_init);
+  sqlite3_auto_extension((void(*)(void))sqlite3_noop_init);
+  sqlite3_auto_extension((void(*)(void))sqlite3_regexp_init);
+  sqlite3_auto_extension((void(*)(void))sqlite3_sqlmath_init);
+  sqlite3_auto_extension((void(*)(void))sqlite3_templatevtab_init);

-static double deg2rad(double x){
+// hack-sqlite
+#if 0
+static double deg2rad(double x){

-static void atn2Func(sqlite3_context *context, int argc, sqlite3_value **argv){
+// hack-sqlite
+#endif
+static void atn2Func(sqlite3_context *context, int argc, sqlite3_value **argv){

-static void ceilFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
+// hack-sqlite
+#if 0
+static void ceilFunc(sqlite3_context *context, int argc, sqlite3_value **argv){

-static void powerFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
+// hack-sqlite
+#if 0
+static void powerFunc(sqlite3_context *context, int argc, sqlite3_value **argv){

-static void replicateFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
+// hack-sqlite
+#endif
+static void replicateFunc(sqlite3_context *context, int argc, sqlite3_value **argv){

-static void squareFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
+// hack-sqlite
+#endif
+static void squareFunc(sqlite3_context *context, int argc, sqlite3_value **argv){

-void print_elem(void *e, int64_t c, void* p){
-  int ee = *(int*)(e);
-  printf("%d => %lld\n", ee,c);
-}
+// hack-sqlite
+#if 0
+void print_elem(void *e, int64_t c, void* p){
+  int ee = *(int*)(e);
+  printf("%d => %lld\n", ee,c);
+}
+#endif
*/


/*
repo https://github.com/sqlite/sqlite/tree/version-3.36.0
committed 2021-06-18T18:36:39Z
*/


/*
file https://github.com/sqlite/sqlite/blob/version-3.36.0/LICENSE.md
*/
/*
The author disclaims copyright to this source code.  In place of
a legal notice, here is a blessing:

  *   May you do good and not evil.
  *   May you find forgiveness for yourself and forgive others.
  *   May you share freely, never taking more than you give.
*/


/*
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/carray.c
*/
/*
** 2016-06-29
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
** This file demonstrates how to create a table-valued-function that
** returns the values in a C-language array.
** Examples:
**
**      SELECT * FROM carray($ptr,5)
**
** The query above returns 5 integers contained in a C-language array
** at the address $ptr.  $ptr is a pointer to the array of integers.
** The pointer value must be assigned to $ptr using the
** sqlite3_bind_pointer() interface with a pointer type of "carray".
** For example:
**
**    static int aX[] = { 53, 9, 17, 2231, 4, 99 };
**    int i = sqlite3_bind_parameter_index(pStmt, "$ptr");
**    sqlite3_bind_pointer(pStmt, i, aX, "carray", 0);
**
** There is an optional third parameter to determine the datatype of
** the C-language array.  Allowed values of the third parameter are
** 'int32', 'int64', 'double', 'char*'.  Example:
**
**      SELECT * FROM carray($ptr,10,'char*');
**
** The default value of the third parameter is 'int32'.
**
** HOW IT WORKS
**
** The carray "function" is really a virtual table with the
** following schema:
**
**     CREATE TABLE carray(
**       value,
**       pointer HIDDEN,
**       count HIDDEN,
**       ctype TEXT HIDDEN
**     );
**
** If the hidden columns "pointer" and "count" are unconstrained, then
** the virtual table has no rows.  Otherwise, the virtual table interprets
** the integer value of "pointer" as a pointer to the array and "count"
** as the number of elements in the array.  The virtual table steps through
** the array, element by element.
*/
#include "sqlite3ext.h"
// SQLITE_EXTENSION_INIT1
// hack-sqlite
const sqlite3_api_routines *sqlite3_api=0;
#include <assert.h>
#include <string.h>

/* Allowed values for the mFlags parameter to sqlite3_carray_bind().
** Must exactly match the definitions in carray.h.
*/
#define CARRAY_INT32     0      /* Data is 32-bit signed integers */
#define CARRAY_INT64     1      /* Data is 64-bit signed integers */
#define CARRAY_DOUBLE    2      /* Data is doubles */
#define CARRAY_TEXT      3      /* Data is char* */

#ifndef SQLITE_OMIT_VIRTUALTABLE

/*
** Names of allowed datatypes
*/
static const char *azType[] = { "int32", "int64", "double", "char*" };

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


/* carray_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct carray_cursor carray_cursor;
struct carray_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  sqlite3_int64 iRowid;      /* The rowid */
  void *pPtr;                /* Pointer to the array of values */
  sqlite3_int64 iCnt;        /* Number of integers in the array */
  unsigned char eType;       /* One of the CARRAY_type values */
};

/*
** The carrayConnect() method is invoked to create a new
** carray_vtab that describes the carray virtual table.
**
** Think of this routine as the constructor for carray_vtab objects.
**
** All this routine needs to do is:
**
**    (1) Allocate the carray_vtab object and initialize all fields.
**
**    (2) Tell SQLite (via the sqlite3_declare_vtab() interface) what the
**        result set of queries against carray will look like.
*/
static int carrayConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  sqlite3_vtab *pNew;
  int rc;

/* Column numbers */
#define CARRAY_COLUMN_VALUE   0
#define CARRAY_COLUMN_POINTER 1
#define CARRAY_COLUMN_COUNT   2
#define CARRAY_COLUMN_CTYPE   3

  rc = sqlite3_declare_vtab(db,
     "CREATE TABLE x(value,pointer hidden,count hidden,ctype hidden)");
  if( rc==SQLITE_OK ){
    pNew = *ppVtab = sqlite3_malloc( sizeof(*pNew) );
    if( pNew==0 ) return SQLITE_NOMEM;
    memset(pNew, 0, sizeof(*pNew));
  }
  return rc;
}

/*
** This method is the destructor for carray_cursor objects.
*/
static int carrayDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/*
** Constructor for a new carray_cursor object.
*/
static int carrayOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  carray_cursor *pCur;
  pCur = sqlite3_malloc( sizeof(*pCur) );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(*pCur));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/*
** Destructor for a carray_cursor.
*/
static int carrayClose(sqlite3_vtab_cursor *cur){
  sqlite3_free(cur);
  return SQLITE_OK;
}


/*
** Advance a carray_cursor to its next row of output.
*/
static int carrayNext(sqlite3_vtab_cursor *cur){
  carray_cursor *pCur = (carray_cursor*)cur;
  pCur->iRowid++;
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the carray_cursor
** is currently pointing.
*/
static int carrayColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  carray_cursor *pCur = (carray_cursor*)cur;
  sqlite3_int64 x = 0;
  switch( i ){
    case CARRAY_COLUMN_POINTER:   return SQLITE_OK;
    case CARRAY_COLUMN_COUNT:     x = pCur->iCnt;   break;
    case CARRAY_COLUMN_CTYPE: {
      sqlite3_result_text(ctx, azType[pCur->eType], -1, SQLITE_STATIC);
      return SQLITE_OK;
    }
    default: {
      switch( pCur->eType ){
        case CARRAY_INT32: {
          int *p = (int*)pCur->pPtr;
          sqlite3_result_int(ctx, p[pCur->iRowid-1]);
          return SQLITE_OK;
        }
        case CARRAY_INT64: {
          sqlite3_int64 *p = (sqlite3_int64*)pCur->pPtr;
          sqlite3_result_int64(ctx, p[pCur->iRowid-1]);
          return SQLITE_OK;
        }
        case CARRAY_DOUBLE: {
          double *p = (double*)pCur->pPtr;
          sqlite3_result_double(ctx, p[pCur->iRowid-1]);
          return SQLITE_OK;
        }
        case CARRAY_TEXT: {
          const char **p = (const char**)pCur->pPtr;
          sqlite3_result_text(ctx, p[pCur->iRowid-1], -1, SQLITE_TRANSIENT);
          return SQLITE_OK;
        }
      }
    }
  }
  sqlite3_result_int64(ctx, x);
  return SQLITE_OK;
}

/*
** Return the rowid for the current row.  In this implementation, the
** rowid is the same as the output value.
*/
static int carrayRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  carray_cursor *pCur = (carray_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
static int carrayEof(sqlite3_vtab_cursor *cur){
  carray_cursor *pCur = (carray_cursor*)cur;
  return pCur->iRowid>pCur->iCnt;
}

/*
** This method is called to "rewind" the carray_cursor object back
** to the first row of output.
*/
static int carrayFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  carray_cursor *pCur = (carray_cursor *)pVtabCursor;
  pCur->pPtr = 0;
  pCur->iCnt = 0;
  switch( idxNum ){
    case 1: {
      carray_bind *pBind = sqlite3_value_pointer(argv[0], "carray-bind");
      if( pBind==0 ) break;
      pCur->pPtr = pBind->aData;
      pCur->iCnt = pBind->nData;
      pCur->eType = pBind->mFlags & 0x03;
      break;
    }
    case 2:
    case 3: {
      pCur->pPtr = sqlite3_value_pointer(argv[0], "carray");
      pCur->iCnt = pCur->pPtr ? sqlite3_value_int64(argv[1]) : 0;
      if( idxNum<3 ){
        pCur->eType = CARRAY_INT32;
      }else{
        unsigned char i;
        const char *zType = (const char*)sqlite3_value_text(argv[2]);
        for(i=0; i<sizeof(azType)/sizeof(azType[0]); i++){
          if( sqlite3_stricmp(zType, azType[i])==0 ) break;
        }
        if( i>=sizeof(azType)/sizeof(azType[0]) ){
          pVtabCursor->pVtab->zErrMsg = sqlite3_mprintf(
            "unknown datatype: %Q", zType);
          return SQLITE_ERROR;
        }else{
          pCur->eType = i;
        }
      }
      break;
    }
  }
  pCur->iRowid = 1;
  return SQLITE_OK;
}

/*
** SQLite will invoke this method one or more times while planning a query
** that uses the carray virtual table.  This routine needs to create
** a query plan for each invocation and compute an estimated cost for that
** plan.
**
** In this implementation idxNum is used to represent the
** query plan.  idxStr is unused.
**
** idxNum is:
**
**    1    If only the pointer= constraint exists.  In this case, the
**         parameter must be bound using sqlite3_carray_bind().
**
**    2    if the pointer= and count= constraints exist.
**
**    3    if the ctype= constraint also exists.
**
** idxNum is 0 otherwise and carray becomes an empty table.
*/
static int carrayBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
  int i;                 /* Loop over constraints */
  int ptrIdx = -1;       /* Index of the pointer= constraint, or -1 if none */
  int cntIdx = -1;       /* Index of the count= constraint, or -1 if none */
  int ctypeIdx = -1;     /* Index of the ctype= constraint, or -1 if none */

  const struct sqlite3_index_constraint *pConstraint;
  pConstraint = pIdxInfo->aConstraint;
  for(i=0; i<pIdxInfo->nConstraint; i++, pConstraint++){
    if( pConstraint->usable==0 ) continue;
    if( pConstraint->op!=SQLITE_INDEX_CONSTRAINT_EQ ) continue;
    switch( pConstraint->iColumn ){
      case CARRAY_COLUMN_POINTER:
        ptrIdx = i;
        break;
      case CARRAY_COLUMN_COUNT:
        cntIdx = i;
        break;
      case CARRAY_COLUMN_CTYPE:
        ctypeIdx = i;
        break;
    }
  }
  if( ptrIdx>=0 ){
    pIdxInfo->aConstraintUsage[ptrIdx].argvIndex = 1;
    pIdxInfo->aConstraintUsage[ptrIdx].omit = 1;
    pIdxInfo->estimatedCost = (double)1;
    pIdxInfo->estimatedRows = 100;
    pIdxInfo->idxNum = 1;
    if( cntIdx>=0 ){
      pIdxInfo->aConstraintUsage[cntIdx].argvIndex = 2;
      pIdxInfo->aConstraintUsage[cntIdx].omit = 1;
      pIdxInfo->idxNum = 2;
      if( ctypeIdx>=0 ){
        pIdxInfo->aConstraintUsage[ctypeIdx].argvIndex = 3;
        pIdxInfo->aConstraintUsage[ctypeIdx].omit = 1;
        pIdxInfo->idxNum = 3;
      }
    }
  }else{
    pIdxInfo->estimatedCost = (double)2147483647;
    pIdxInfo->estimatedRows = 2147483647;
    pIdxInfo->idxNum = 0;
  }
  return SQLITE_OK;
}

/*
** This following structure defines all the methods for the
** carray virtual table.
*/
static sqlite3_module carrayModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  carrayConnect,             /* xConnect */
  carrayBestIndex,           /* xBestIndex */
  carrayDisconnect,          /* xDisconnect */
  0,                         /* xDestroy */
  carrayOpen,                /* xOpen - open a cursor */
  carrayClose,               /* xClose - close a cursor */
  carrayFilter,              /* xFilter - configure scan constraints */
  carrayNext,                /* xNext - advance a cursor */
  carrayEof,                 /* xEof - check for end of scan */
  carrayColumn,              /* xColumn - read data */
  carrayRowid,               /* xRowid - read data */
  0,                         /* xUpdate */
  0,                         /* xBegin */
  0,                         /* xSync */
  0,                         /* xCommit */
  0,                         /* xRollback */
  0,                         /* xFindMethod */
  0,                         /* xRename */
0, 0, 0, 0
};

/*
** Destructor for the carray_bind object
*/
static void carrayBindDel(void *pPtr){
  carray_bind *p = (carray_bind*)pPtr;
  if( p->xDel!=SQLITE_STATIC ){
     p->xDel(p->aData);
  }
  sqlite3_free(p);
}

/*
** Invoke this interface in order to bind to the single-argument
** version of CARRAY().
*/
#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_carray_bind(
  sqlite3_stmt *pStmt,
  int idx,
  void *aData,
  int nData,
  int mFlags,
  void (*xDestroy)(void*)
){
  carray_bind *pNew;
  int i;
  pNew = sqlite3_malloc64(sizeof(*pNew));
  if( pNew==0 ){
    if( xDestroy!=SQLITE_STATIC && xDestroy!=SQLITE_TRANSIENT ){
      xDestroy(aData);
    }
    return SQLITE_NOMEM;
  }
  pNew->nData = nData;
  pNew->mFlags = mFlags;
  if( xDestroy==SQLITE_TRANSIENT ){
    sqlite3_int64 sz = nData;
    switch( mFlags & 0x03 ){
      case CARRAY_INT32:   sz *= 4;              break;
      case CARRAY_INT64:   sz *= 8;              break;
      case CARRAY_DOUBLE:  sz *= 8;              break;
      case CARRAY_TEXT:    sz *= sizeof(char*);  break;
    }
    if( (mFlags & 0x03)==CARRAY_TEXT ){
      for(i=0; i<nData; i++){
        const char *z = ((char**)aData)[i];
        if( z ) sz += strlen(z) + 1;
      }
    }
    pNew->aData = sqlite3_malloc64( sz );
    if( pNew->aData==0 ){
      sqlite3_free(pNew);
      return SQLITE_NOMEM;
    }
    if( (mFlags & 0x03)==CARRAY_TEXT ){
      char **az = (char**)pNew->aData;
      char *z = (char*)&az[nData];
      for(i=0; i<nData; i++){
        const char *zData = ((char**)aData)[i];
        sqlite3_int64 n;
        if( zData==0 ){
          az[i] = 0;
          continue;
        }
        az[i] = z;
        n = strlen(zData);
        memcpy(z, zData, n+1);
        z += n+1;
      }
    }else{
      memcpy(pNew->aData, aData, sz*nData);
    }
    pNew->xDel = sqlite3_free;
  }else{
    pNew->aData = aData;
    pNew->xDel = xDestroy;
  }
  return sqlite3_bind_pointer(pStmt, idx, pNew, "carray-bind", carrayBindDel);
}


/*
** For testing purpose in the TCL test harness, we need a method for
** setting the pointer value.  The inttoptr(X) SQL function accomplishes
** this.  Tcl script will bind an integer to X and the inttoptr() SQL
** function will use sqlite3_result_pointer() to convert that integer into
** a pointer.
**
** This is for testing on TCL only.
*/
#ifdef SQLITE_TEST
static void inttoptrFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  void *p;
  sqlite3_int64 i64;
  i64 = sqlite3_value_int64(argv[0]);
  if( sizeof(i64)==sizeof(p) ){
    memcpy(&p, &i64, sizeof(p));
  }else{
    int i32 = i64 & 0xffffffff;
    memcpy(&p, &i32, sizeof(p));
  }
  sqlite3_result_pointer(context, p, "carray", 0);
}
#endif /* SQLITE_TEST */

#endif /* SQLITE_OMIT_VIRTUALTABLE */

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_carray_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  // SQLITE_EXTENSION_INIT2(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3_create_module(db, "carray", &carrayModule, 0);
#ifdef SQLITE_TEST
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "inttoptr", 1, SQLITE_UTF8, 0,
                                 inttoptrFunc, 0, 0);
  }
#endif /* SQLITE_TEST */
#endif /* SQLITE_OMIT_VIRTUALTABLE */
  return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/compress.c
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
// SQLITE_EXTENSION_INIT1
#include <zlib.h>

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
  const unsigned char *pIn;
  unsigned char *pOut;
  unsigned int nIn;
  unsigned long int nOut;
  unsigned char x[8];
  int rc;
  int i, j;

  pIn = sqlite3_value_blob(argv[0]);
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
  const unsigned char *pIn;
  unsigned char *pOut;
  unsigned int nIn;
  unsigned long int nOut;
  int rc;
// hack-sqlite
  unsigned int i;

  pIn = sqlite3_value_blob(argv[0]);
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
__declspec(dllexport)
#endif
int sqlite3_compress_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  // SQLITE_EXTENSION_INIT2(pApi);
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
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/csv.c
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
  CsvTable *pNew = 0;        /* The CsvTable object to construct */
  int bHeader = -1;          /* header= flags.  -1 means not seen yet */
  int rc = SQLITE_OK;        /* Result code from this routine */
// hack-sqlite
  unsigned int i, j;                  /* Loop counters */
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
// hack-sqlite
  for(i=3; i<(unsigned int)argc; i++){
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
0, 0, 0, 0
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
0, 0, 0, 0
};
#endif /* SQLITE_TEST */

#endif /* !defined(SQLITE_OMIT_VIRTUALTABLE) */


#ifdef _WIN32
__declspec(dllexport)
#endif
/*
** This routine is called when the extension is loaded.  The new
** CSV virtual table module is registered with the calling database
** connection.
*/
int sqlite3_csv_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
#ifndef SQLITE_OMIT_VIRTUALTABLE
  int rc;
  // SQLITE_EXTENSION_INIT2(pApi);
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
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/json1.c
*/
/*
** 2015-08-12
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
** This SQLite extension implements JSON functions.  The interface is
** modeled after MySQL JSON functions:
**
**     https://dev.mysql.com/doc/refman/5.7/en/json.html
**
** For the time being, all JSON is stored as pure text.  (We might add
** a JSONB type in the future which stores a binary encoding of JSON in
** a BLOB, but there is no support for JSONB in the current implementation.
** This implementation parses JSON text at 250 MB/s, so it is hard to see
** how JSONB might improve on that.)
*/
#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_JSON1)
#if !defined(SQLITEINT_H)
#include "sqlite3ext.h"
#endif
// SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* Mark a function parameter as unused, to suppress nuisance compiler
** warnings. */
#ifndef UNUSED_PARAM
# define UNUSED_PARAM(X)  (void)(X)
#endif

#ifndef LARGEST_INT64
# define LARGEST_INT64  (0xffffffff|(((sqlite3_int64)0x7fffffff)<<32))
# define SMALLEST_INT64 (((sqlite3_int64)-1) - LARGEST_INT64)
#endif

#ifndef deliberate_fall_through
// hack-sqlite
#ifdef __GNUC__
# define deliberate_fall_through __attribute__((fallthrough));
#else
# define deliberate_fall_through
#endif
#endif

/*
** Versions of isspace(), isalnum() and isdigit() to which it is safe
** to pass signed char values.
*/
#ifdef sqlite3Isdigit
   /* Use the SQLite core versions if this routine is part of the
   ** SQLite amalgamation */
#  define safe_isdigit(x)  sqlite3Isdigit(x)
#  define safe_isalnum(x)  sqlite3Isalnum(x)
#  define safe_isxdigit(x) sqlite3Isxdigit(x)
#else
   /* Use the standard library for separate compilation */
#include <ctype.h>  /* amalgamator: keep */
#  define safe_isdigit(x)  isdigit((unsigned char)(x))
#  define safe_isalnum(x)  isalnum((unsigned char)(x))
#  define safe_isxdigit(x) isxdigit((unsigned char)(x))
#endif

/*
** Growing our own isspace() routine this way is twice as fast as
** the library isspace() function, resulting in a 7% overall performance
** increase for the parser.  (Ubuntu14.10 gcc 4.8.4 x64 with -Os).
*/
static const char jsonIsSpace[] = {
  0, 0, 0, 0, 0, 0, 0, 0,     0, 1, 1, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
};
#define safe_isspace(x) (jsonIsSpace[(unsigned char)x])

#ifndef SQLITE_AMALGAMATION
  /* Unsigned integer types.  These are already defined in the sqliteInt.h,
  ** but the definitions need to be repeated for separate compilation. */
  typedef sqlite3_uint64 u64;
  typedef unsigned int u32;
  typedef unsigned short int u16;
  typedef unsigned char u8;
#endif

/* Objects */
typedef struct JsonString JsonString;
typedef struct JsonNode JsonNode;
typedef struct JsonParse JsonParse;

/* An instance of this object represents a JSON string
** under construction.  Really, this is a generic string accumulator
** that can be and is used to create strings other than JSON.
*/
struct JsonString {
  sqlite3_context *pCtx;   /* Function context - put error messages here */
  char *zBuf;              /* Append JSON content here */
  u64 nAlloc;              /* Bytes of storage available in zBuf[] */
  u64 nUsed;               /* Bytes of zBuf[] currently used */
  u8 bStatic;              /* True if zBuf is static space */
  u8 bErr;                 /* True if an error has been encountered */
  char zSpace[100];        /* Initial static space */
};

/* JSON type values
*/
#define JSON_NULL     0
#define JSON_TRUE     1
#define JSON_FALSE    2
#define JSON_INT      3
#define JSON_REAL     4
#define JSON_STRING   5
#define JSON_ARRAY    6
#define JSON_OBJECT   7

/* The "subtype" set for JSON values */
#define JSON_SUBTYPE  74    /* Ascii for "J" */

/*
** Names of the various JSON types:
*/
static const char * const jsonType[] = {
  "null", "true", "false", "integer", "real", "text", "array", "object"
};

/* Bit values for the JsonNode.jnFlag field
*/
#define JNODE_RAW     0x01         /* Content is raw, not JSON encoded */
#define JNODE_ESCAPE  0x02         /* Content is text with \ escapes */
#define JNODE_REMOVE  0x04         /* Do not output */
#define JNODE_REPLACE 0x08         /* Replace with JsonNode.u.iReplace */
#define JNODE_PATCH   0x10         /* Patch with JsonNode.u.pPatch */
#define JNODE_APPEND  0x20         /* More ARRAY/OBJECT entries at u.iAppend */
#define JNODE_LABEL   0x40         /* Is a label of an object */


/* A single node of parsed JSON
*/
struct JsonNode {
  u8 eType;              /* One of the JSON_ type values */
  u8 jnFlags;            /* JNODE flags */
  u32 n;                 /* Bytes of content, or number of sub-nodes */
  union {
    const char *zJContent; /* Content for INT, REAL, and STRING */
    u32 iAppend;           /* More terms for ARRAY and OBJECT */
    u32 iKey;              /* Key for ARRAY objects in json_tree() */
    u32 iReplace;          /* Replacement content for JNODE_REPLACE */
    JsonNode *pPatch;      /* Node chain of patch for JNODE_PATCH */
  } u;
};

/* A completely parsed JSON string
*/
struct JsonParse {
  u32 nNode;         /* Number of slots of aNode[] used */
  u32 nAlloc;        /* Number of slots of aNode[] allocated */
  JsonNode *aNode;   /* Array of nodes containing the parse */
  const char *zJson; /* Original JSON string */
  u32 *aUp;          /* Index of parent of each node */
  u8 oom;            /* Set to true if out of memory */
  u8 nErr;           /* Number of errors seen */
  u16 iDepth;        /* Nesting depth */
  int nJson;         /* Length of the zJson string in bytes */
  u32 iHold;         /* Replace cache line with the lowest iHold value */
};

/*
** Maximum nesting depth of JSON for this implementation.
**
** This limit is needed to avoid a stack overflow in the recursive
** descent parser.  A depth of 2000 is far deeper than any sane JSON
** should go.
*/
#define JSON_MAX_DEPTH  2000

/**************************************************************************
** Utility routines for dealing with JsonString objects
**************************************************************************/

/* Set the JsonString object to an empty string
*/
static void jsonZero(JsonString *p){
  p->zBuf = p->zSpace;
  p->nAlloc = sizeof(p->zSpace);
  p->nUsed = 0;
  p->bStatic = 1;
}

/* Initialize the JsonString object
*/
static void jsonInit(JsonString *p, sqlite3_context *pCtx){
  p->pCtx = pCtx;
  p->bErr = 0;
  jsonZero(p);
}


/* Free all allocated memory and reset the JsonString object back to its
** initial state.
*/
static void jsonReset(JsonString *p){
// hack-sqlite
  if (p == NULL) {return;}
  if( !p->bStatic ) sqlite3_free(p->zBuf);
  jsonZero(p);
}


/* Report an out-of-memory (OOM) condition
*/
static void jsonOom(JsonString *p){
  p->bErr = 1;
  sqlite3_result_error_nomem(p->pCtx);
  jsonReset(p);
}

/* Enlarge pJson->zBuf so that it can hold at least N more bytes.
** Return zero on success.  Return non-zero on an OOM error
*/
static int jsonGrow(JsonString *p, u32 N){
  u64 nTotal = N<p->nAlloc ? p->nAlloc*2 : p->nAlloc+N+10;
  char *zNew;
  if( p->bStatic ){
    if( p->bErr ) return 1;
    zNew = sqlite3_malloc64(nTotal);
    if( zNew==0 ){
      jsonOom(p);
      return SQLITE_NOMEM;
    }
    memcpy(zNew, p->zBuf, (size_t)p->nUsed);
    p->zBuf = zNew;
    p->bStatic = 0;
  }else{
    zNew = sqlite3_realloc64(p->zBuf, nTotal);
    if( zNew==0 ){
      jsonOom(p);
      return SQLITE_NOMEM;
    }
    p->zBuf = zNew;
  }
  p->nAlloc = nTotal;
  return SQLITE_OK;
}

/* Append N bytes from zIn onto the end of the JsonString string.
*/
static void jsonAppendRaw(JsonString *p, const char *zIn, u32 N){
  if( N==0 ) return;
  if( (N+p->nUsed >= p->nAlloc) && jsonGrow(p,N)!=0 ) return;
  memcpy(p->zBuf+p->nUsed, zIn, N);
  p->nUsed += N;
}

/* Append formatted text (not to exceed N bytes) to the JsonString.
*/
static void jsonPrintf(int N, JsonString *p, const char *zFormat, ...){
  va_list ap;
  if( (p->nUsed + N >= p->nAlloc) && jsonGrow(p, N) ) return;
  va_start(ap, zFormat);
  sqlite3_vsnprintf(N, p->zBuf+p->nUsed, zFormat, ap);
  va_end(ap);
  p->nUsed += (int)strlen(p->zBuf+p->nUsed);
}

/* Append a single character
*/
static void jsonAppendChar(JsonString *p, char c){
  if( p->nUsed>=p->nAlloc && jsonGrow(p,1)!=0 ) return;
  p->zBuf[p->nUsed++] = c;
}

/* Append a comma separator to the output buffer, if the previous
** character is not '[' or '{'.
*/
static void jsonAppendSeparator(JsonString *p){
  char c;
  if( p->nUsed==0 ) return;
  c = p->zBuf[p->nUsed-1];
  if( c!='[' && c!='{' ) jsonAppendChar(p, ',');
}

/* Append the N-byte string in zIn to the end of the JsonString string
** under construction.  Enclose the string in "..." and escape
** any double-quotes or backslash characters contained within the
** string.
*/
static void jsonAppendString(JsonString *p, const char *zIn, u32 N){
  u32 i;
  if( zIn==0 || ((N+p->nUsed+2 >= p->nAlloc) && jsonGrow(p,N+2)!=0) ) return;
  p->zBuf[p->nUsed++] = '"';
  for(i=0; i<N; i++){
    unsigned char c = ((unsigned const char*)zIn)[i];
    if( c=='"' || c=='\\' ){
      json_simple_escape:
      if( (p->nUsed+N+3-i > p->nAlloc) && jsonGrow(p,N+3-i)!=0 ) return;
      p->zBuf[p->nUsed++] = '\\';
    }else if( c<=0x1f ){
      static const char aSpecial[] = {
         0, 0, 0, 0, 0, 0, 0, 0, 'b', 't', 'n', 0, 'f', 'r', 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0,   0,   0, 0, 0
      };
      assert( sizeof(aSpecial)==32 );
      assert( aSpecial['\b']=='b' );
      assert( aSpecial['\f']=='f' );
      assert( aSpecial['\n']=='n' );
      assert( aSpecial['\r']=='r' );
      assert( aSpecial['\t']=='t' );
      if( aSpecial[c] ){
        c = aSpecial[c];
        goto json_simple_escape;
      }
      if( (p->nUsed+N+7+i > p->nAlloc) && jsonGrow(p,N+7-i)!=0 ) return;
      p->zBuf[p->nUsed++] = '\\';
      p->zBuf[p->nUsed++] = 'u';
      p->zBuf[p->nUsed++] = '0';
      p->zBuf[p->nUsed++] = '0';
      p->zBuf[p->nUsed++] = '0' + (c>>4);
      c = "0123456789abcdef"[c&0xf];
    }
    p->zBuf[p->nUsed++] = c;
  }
  p->zBuf[p->nUsed++] = '"';
  assert( p->nUsed<p->nAlloc );
}

/*
** Append a function parameter value to the JSON string under
** construction.
*/
static void jsonAppendValue(
  JsonString *p,                 /* Append to this JSON string */
  sqlite3_value *pValue          /* Value to append */
){
  switch( sqlite3_value_type(pValue) ){
    case SQLITE_NULL: {
      jsonAppendRaw(p, "null", 4);
      break;
    }
    case SQLITE_INTEGER:
    case SQLITE_FLOAT: {
      const char *z = (const char*)sqlite3_value_text(pValue);
      u32 n = (u32)sqlite3_value_bytes(pValue);
      jsonAppendRaw(p, z, n);
      break;
    }
    case SQLITE_TEXT: {
      const char *z = (const char*)sqlite3_value_text(pValue);
      u32 n = (u32)sqlite3_value_bytes(pValue);
      if( sqlite3_value_subtype(pValue)==JSON_SUBTYPE ){
        jsonAppendRaw(p, z, n);
      }else{
        jsonAppendString(p, z, n);
      }
      break;
    }
    default: {
      if( p->bErr==0 ){
        sqlite3_result_error(p->pCtx, "JSON cannot hold BLOB values", -1);
        p->bErr = 2;
        jsonReset(p);
      }
      break;
    }
  }
}


/* Make the JSON in p the result of the SQL function.
*/
static void jsonResult(JsonString *p){
  if( p->bErr==0 ){
    sqlite3_result_text64(p->pCtx, p->zBuf, p->nUsed,
                          p->bStatic ? SQLITE_TRANSIENT : sqlite3_free,
                          SQLITE_UTF8);
    jsonZero(p);
  }
  assert( p->bStatic );
}

/**************************************************************************
** Utility routines for dealing with JsonNode and JsonParse objects
**************************************************************************/

/*
** Return the number of consecutive JsonNode slots need to represent
** the parsed JSON at pNode.  The minimum answer is 1.  For ARRAY and
** OBJECT types, the number might be larger.
**
** Appended elements are not counted.  The value returned is the number
** by which the JsonNode counter should increment in order to go to the
** next peer value.
*/
static u32 jsonNodeSize(JsonNode *pNode){
  return pNode->eType>=JSON_ARRAY ? pNode->n+1 : 1;
}

/*
** Reclaim all memory allocated by a JsonParse object.  But do not
** delete the JsonParse object itself.
*/
static void jsonParseReset(JsonParse *pParse){
  sqlite3_free(pParse->aNode);
  pParse->aNode = 0;
  pParse->nNode = 0;
  pParse->nAlloc = 0;
  sqlite3_free(pParse->aUp);
  pParse->aUp = 0;
}

/*
** Free a JsonParse object that was obtained from sqlite3_malloc().
*/
static void jsonParseFree(JsonParse *pParse){
  jsonParseReset(pParse);
  sqlite3_free(pParse);
}

/*
** Convert the JsonNode pNode into a pure JSON string and
** append to pOut.  Subsubstructure is also included.  Return
** the number of JsonNode objects that are encoded.
*/
static void jsonRenderNode(
  JsonNode *pNode,               /* The node to render */
  JsonString *pOut,              /* Write JSON here */
  sqlite3_value **aReplace       /* Replacement values */
){
  if( pNode->jnFlags & (JNODE_REPLACE|JNODE_PATCH) ){
    if( pNode->jnFlags & JNODE_REPLACE ){
      jsonAppendValue(pOut, aReplace[pNode->u.iReplace]);
      return;
    }
    pNode = pNode->u.pPatch;
  }
  switch( pNode->eType ){
    default: {
      assert( pNode->eType==JSON_NULL );
      jsonAppendRaw(pOut, "null", 4);
      break;
    }
    case JSON_TRUE: {
      jsonAppendRaw(pOut, "true", 4);
      break;
    }
    case JSON_FALSE: {
      jsonAppendRaw(pOut, "false", 5);
      break;
    }
    case JSON_STRING: {
      if( pNode->jnFlags & JNODE_RAW ){
        jsonAppendString(pOut, pNode->u.zJContent, pNode->n);
        break;
      }
      /* no break */ deliberate_fall_through
    }
    case JSON_REAL:
    case JSON_INT: {
      jsonAppendRaw(pOut, pNode->u.zJContent, pNode->n);
      break;
    }
    case JSON_ARRAY: {
      u32 j = 1;
      jsonAppendChar(pOut, '[');
      for(;;){
        while( j<=pNode->n ){
          if( (pNode[j].jnFlags & JNODE_REMOVE)==0 ){
            jsonAppendSeparator(pOut);
            jsonRenderNode(&pNode[j], pOut, aReplace);
          }
          j += jsonNodeSize(&pNode[j]);
        }
        if( (pNode->jnFlags & JNODE_APPEND)==0 ) break;
        pNode = &pNode[pNode->u.iAppend];
        j = 1;
      }
      jsonAppendChar(pOut, ']');
      break;
    }
    case JSON_OBJECT: {
      u32 j = 1;
      jsonAppendChar(pOut, '{');
      for(;;){
        while( j<=pNode->n ){
          if( (pNode[j+1].jnFlags & JNODE_REMOVE)==0 ){
            jsonAppendSeparator(pOut);
            jsonRenderNode(&pNode[j], pOut, aReplace);
            jsonAppendChar(pOut, ':');
            jsonRenderNode(&pNode[j+1], pOut, aReplace);
          }
          j += 1 + jsonNodeSize(&pNode[j+1]);
        }
        if( (pNode->jnFlags & JNODE_APPEND)==0 ) break;
        pNode = &pNode[pNode->u.iAppend];
        j = 1;
      }
      jsonAppendChar(pOut, '}');
      break;
    }
  }
}

/*
** Return a JsonNode and all its descendents as a JSON string.
*/
static void jsonReturnJson(
  JsonNode *pNode,            /* Node to return */
  sqlite3_context *pCtx,      /* Return value for this function */
  sqlite3_value **aReplace    /* Array of replacement values */
){
  JsonString s;
  jsonInit(&s, pCtx);
  jsonRenderNode(pNode, &s, aReplace);
  jsonResult(&s);
  sqlite3_result_subtype(pCtx, JSON_SUBTYPE);
}

/*
** Translate a single byte of Hex into an integer.
** This routine only works if h really is a valid hexadecimal
** character:  0..9a..fA..F
*/
static u8 jsonHexToInt(int h){
  assert( (h>='0' && h<='9') ||  (h>='a' && h<='f') ||  (h>='A' && h<='F') );
#ifdef SQLITE_EBCDIC
  h += 9*(1&~(h>>4));
#else
  h += 9*(1&(h>>6));
#endif
  return (u8)(h & 0xf);
}

/*
** Convert a 4-byte hex string into an integer
*/
static u32 jsonHexToInt4(const char *z){
  u32 v;
  assert( safe_isxdigit(z[0]) );
  assert( safe_isxdigit(z[1]) );
  assert( safe_isxdigit(z[2]) );
  assert( safe_isxdigit(z[3]) );
  v = (jsonHexToInt(z[0])<<12)
    + (jsonHexToInt(z[1])<<8)
    + (jsonHexToInt(z[2])<<4)
    + jsonHexToInt(z[3]);
  return v;
}

/*
** Make the JsonNode the return value of the function.
*/
static void jsonReturn(
  JsonNode *pNode,            /* Node to return */
  sqlite3_context *pCtx,      /* Return value for this function */
  sqlite3_value **aReplace    /* Array of replacement values */
){
  switch( pNode->eType ){
    default: {
      assert( pNode->eType==JSON_NULL );
      sqlite3_result_null(pCtx);
      break;
    }
    case JSON_TRUE: {
      sqlite3_result_int(pCtx, 1);
      break;
    }
    case JSON_FALSE: {
      sqlite3_result_int(pCtx, 0);
      break;
    }
    case JSON_INT: {
      sqlite3_int64 i = 0;
      const char *z = pNode->u.zJContent;
      if( z[0]=='-' ){ z++; }
      while( z[0]>='0' && z[0]<='9' ){
        unsigned v = *(z++) - '0';
        if( i>=LARGEST_INT64/10 ){
          if( i>LARGEST_INT64/10 ) goto int_as_real;
          if( z[0]>='0' && z[0]<='9' ) goto int_as_real;
          if( v==9 ) goto int_as_real;
          if( v==8 ){
            if( pNode->u.zJContent[0]=='-' ){
              sqlite3_result_int64(pCtx, SMALLEST_INT64);
              goto int_done;
            }else{
              goto int_as_real;
            }
          }
        }
        i = i*10 + v;
      }
      if( pNode->u.zJContent[0]=='-' ){ i = -i; }
      sqlite3_result_int64(pCtx, i);
      int_done:
      break;
      int_as_real: i=0; /* no break */ deliberate_fall_through
    }
    case JSON_REAL: {
      double r;
#ifdef SQLITE_AMALGAMATION
      const char *z = pNode->u.zJContent;
      sqlite3AtoF(z, &r, sqlite3Strlen30(z), SQLITE_UTF8);
#else
      r = strtod(pNode->u.zJContent, 0);
#endif
      sqlite3_result_double(pCtx, r);
      break;
    }
    case JSON_STRING: {
#if 0 /* Never happens because JNODE_RAW is only set by json_set(),
      ** json_insert() and json_replace() and those routines do not
      ** call jsonReturn() */
      if( pNode->jnFlags & JNODE_RAW ){
        sqlite3_result_text(pCtx, pNode->u.zJContent, pNode->n,
                            SQLITE_TRANSIENT);
      }else
#endif
      assert( (pNode->jnFlags & JNODE_RAW)==0 );
      if( (pNode->jnFlags & JNODE_ESCAPE)==0 ){
        /* JSON formatted without any backslash-escapes */
        sqlite3_result_text(pCtx, pNode->u.zJContent+1, pNode->n-2,
                            SQLITE_TRANSIENT);
      }else{
        /* Translate JSON formatted string into raw text */
        u32 i;
        u32 n = pNode->n;
        const char *z = pNode->u.zJContent;
        char *zOut;
        u32 j;
        zOut = sqlite3_malloc( n+1 );
        if( zOut==0 ){
          sqlite3_result_error_nomem(pCtx);
          break;
        }
        for(i=1, j=0; i<n-1; i++){
          char c = z[i];
          if( c!='\\' ){
            zOut[j++] = c;
          }else{
            c = z[++i];
            if( c=='u' ){
              u32 v = jsonHexToInt4(z+i+1);
              i += 4;
              if( v==0 ) break;
              if( v<=0x7f ){
                zOut[j++] = (char)v;
              }else if( v<=0x7ff ){
                zOut[j++] = (char)(0xc0 | (v>>6));
                zOut[j++] = 0x80 | (v&0x3f);
              }else{
                u32 vlo;
                if( (v&0xfc00)==0xd800
                  && i<n-6
                  && z[i+1]=='\\'
                  && z[i+2]=='u'
                  && ((vlo = jsonHexToInt4(z+i+3))&0xfc00)==0xdc00
                ){
                  /* We have a surrogate pair */
                  v = ((v&0x3ff)<<10) + (vlo&0x3ff) + 0x10000;
                  i += 6;
                  zOut[j++] = 0xf0 | (v>>18);
                  zOut[j++] = 0x80 | ((v>>12)&0x3f);
                  zOut[j++] = 0x80 | ((v>>6)&0x3f);
                  zOut[j++] = 0x80 | (v&0x3f);
                }else{
                  zOut[j++] = 0xe0 | (v>>12);
                  zOut[j++] = 0x80 | ((v>>6)&0x3f);
                  zOut[j++] = 0x80 | (v&0x3f);
                }
              }
            }else{
              if( c=='b' ){
                c = '\b';
              }else if( c=='f' ){
                c = '\f';
              }else if( c=='n' ){
                c = '\n';
              }else if( c=='r' ){
                c = '\r';
              }else if( c=='t' ){
                c = '\t';
              }
              zOut[j++] = c;
            }
          }
        }
        zOut[j] = 0;
        sqlite3_result_text(pCtx, zOut, j, sqlite3_free);
      }
      break;
    }
    case JSON_ARRAY:
    case JSON_OBJECT: {
      jsonReturnJson(pNode, pCtx, aReplace);
      break;
    }
  }
}

/* Forward reference */
static int jsonParseAddNode(JsonParse*,u32,u32,const char*);

/*
** A macro to hint to the compiler that a function should not be
** inlined.
*/
#if defined(__GNUC__)
#  define JSON_NOINLINE  __attribute__((noinline))
#elif defined(_MSC_VER) && _MSC_VER>=1310
#  define JSON_NOINLINE  __declspec(noinline)
#else
#  define JSON_NOINLINE
#endif


static JSON_NOINLINE int jsonParseAddNodeExpand(
  JsonParse *pParse,        /* Append the node to this object */
  u32 eType,                /* Node type */
  u32 n,                    /* Content size or sub-node count */
  const char *zContent      /* Content */
){
  u32 nNew;
  JsonNode *pNew;
  assert( pParse->nNode>=pParse->nAlloc );
  if( pParse->oom ) return -1;
  nNew = pParse->nAlloc*2 + 10;
  pNew = sqlite3_realloc64(pParse->aNode, sizeof(JsonNode)*nNew);
  if( pNew==0 ){
    pParse->oom = 1;
    return -1;
  }
  pParse->nAlloc = nNew;
  pParse->aNode = pNew;
  assert( pParse->nNode<pParse->nAlloc );
  return jsonParseAddNode(pParse, eType, n, zContent);
}

/*
** Create a new JsonNode instance based on the arguments and append that
** instance to the JsonParse.  Return the index in pParse->aNode[] of the
** new node, or -1 if a memory allocation fails.
*/
static int jsonParseAddNode(
  JsonParse *pParse,        /* Append the node to this object */
  u32 eType,                /* Node type */
  u32 n,                    /* Content size or sub-node count */
  const char *zContent      /* Content */
){
  JsonNode *p;
  if( pParse->nNode>=pParse->nAlloc ){
    return jsonParseAddNodeExpand(pParse, eType, n, zContent);
  }
  p = &pParse->aNode[pParse->nNode];
  p->eType = (u8)eType;
  p->jnFlags = 0;
  p->n = n;
  p->u.zJContent = zContent;
  return pParse->nNode++;
}

/*
** Return true if z[] begins with 4 (or more) hexadecimal digits
*/
static int jsonIs4Hex(const char *z){
  int i;
  for(i=0; i<4; i++) if( !safe_isxdigit(z[i]) ) return 0;
  return 1;
}

/*
** Parse a single JSON value which begins at pParse->zJson[i].  Return the
** index of the first character past the end of the value parsed.
**
** Return negative for a syntax error.  Special cases:  return -2 if the
** first non-whitespace character is '}' and return -3 if the first
** non-whitespace character is ']'.
*/
static int jsonParseValue(JsonParse *pParse, u32 i){
  char c;
  u32 j;
  int iThis;
  int x;
  JsonNode *pNode;
  const char *z = pParse->zJson;
  while( safe_isspace(z[i]) ){ i++; }
  if( (c = z[i])=='{' ){
    /* Parse object */
    iThis = jsonParseAddNode(pParse, JSON_OBJECT, 0, 0);
    if( iThis<0 ) return -1;
    for(j=i+1;;j++){
      while( safe_isspace(z[j]) ){ j++; }
      if( ++pParse->iDepth > JSON_MAX_DEPTH ) return -1;
      x = jsonParseValue(pParse, j);
      if( x<0 ){
        pParse->iDepth--;
        if( x==(-2) && pParse->nNode==(u32)iThis+1 ) return j+1;
        return -1;
      }
      if( pParse->oom ) return -1;
      pNode = &pParse->aNode[pParse->nNode-1];
      if( pNode->eType!=JSON_STRING ) return -1;
      pNode->jnFlags |= JNODE_LABEL;
      j = x;
      while( safe_isspace(z[j]) ){ j++; }
      if( z[j]!=':' ) return -1;
      j++;
      x = jsonParseValue(pParse, j);
      pParse->iDepth--;
      if( x<0 ) return -1;
      j = x;
      while( safe_isspace(z[j]) ){ j++; }
      c = z[j];
      if( c==',' ) continue;
      if( c!='}' ) return -1;
      break;
    }
    pParse->aNode[iThis].n = pParse->nNode - (u32)iThis - 1;
    return j+1;
  }else if( c=='[' ){
    /* Parse array */
    iThis = jsonParseAddNode(pParse, JSON_ARRAY, 0, 0);
    if( iThis<0 ) return -1;
    for(j=i+1;;j++){
      while( safe_isspace(z[j]) ){ j++; }
      if( ++pParse->iDepth > JSON_MAX_DEPTH ) return -1;
      x = jsonParseValue(pParse, j);
      pParse->iDepth--;
      if( x<0 ){
        if( x==(-3) && pParse->nNode==(u32)iThis+1 ) return j+1;
        return -1;
      }
      j = x;
      while( safe_isspace(z[j]) ){ j++; }
      c = z[j];
      if( c==',' ) continue;
      if( c!=']' ) return -1;
      break;
    }
    pParse->aNode[iThis].n = pParse->nNode - (u32)iThis - 1;
    return j+1;
  }else if( c=='"' ){
    /* Parse string */
    u8 jnFlags = 0;
    j = i+1;
    for(;;){
      c = z[j];
      if( (c & ~0x1f)==0 ){
        /* Control characters are not allowed in strings */
        return -1;
      }
      if( c=='\\' ){
        c = z[++j];
        if( c=='"' || c=='\\' || c=='/' || c=='b' || c=='f'
           || c=='n' || c=='r' || c=='t'
           || (c=='u' && jsonIs4Hex(z+j+1)) ){
          jnFlags = JNODE_ESCAPE;
        }else{
          return -1;
        }
      }else if( c=='"' ){
        break;
      }
      j++;
    }
    jsonParseAddNode(pParse, JSON_STRING, j+1-i, &z[i]);
    if( !pParse->oom ) pParse->aNode[pParse->nNode-1].jnFlags = jnFlags;
    return j+1;
  }else if( c=='n'
         && strncmp(z+i,"null",4)==0
         && !safe_isalnum(z[i+4]) ){
    jsonParseAddNode(pParse, JSON_NULL, 0, 0);
    return i+4;
  }else if( c=='t'
         && strncmp(z+i,"true",4)==0
         && !safe_isalnum(z[i+4]) ){
    jsonParseAddNode(pParse, JSON_TRUE, 0, 0);
    return i+4;
  }else if( c=='f'
         && strncmp(z+i,"false",5)==0
         && !safe_isalnum(z[i+5]) ){
    jsonParseAddNode(pParse, JSON_FALSE, 0, 0);
    return i+5;
  }else if( c=='-' || (c>='0' && c<='9') ){
    /* Parse number */
    u8 seenDP = 0;
    u8 seenE = 0;
    assert( '-' < '0' );
    if( c<='0' ){
      j = c=='-' ? i+1 : i;
      if( z[j]=='0' && z[j+1]>='0' && z[j+1]<='9' ) return -1;
    }
    j = i+1;
    for(;; j++){
      c = z[j];
      if( c>='0' && c<='9' ) continue;
      if( c=='.' ){
        if( z[j-1]=='-' ) return -1;
        if( seenDP ) return -1;
        seenDP = 1;
        continue;
      }
      if( c=='e' || c=='E' ){
        if( z[j-1]<'0' ) return -1;
        if( seenE ) return -1;
        seenDP = seenE = 1;
        c = z[j+1];
        if( c=='+' || c=='-' ){
          j++;
          c = z[j+1];
        }
        if( c<'0' || c>'9' ) return -1;
        continue;
      }
      break;
    }
    if( z[j-1]<'0' ) return -1;
    jsonParseAddNode(pParse, seenDP ? JSON_REAL : JSON_INT,
                        j - i, &z[i]);
    return j;
  }else if( c=='}' ){
    return -2;  /* End of {...} */
  }else if( c==']' ){
    return -3;  /* End of [...] */
  }else if( c==0 ){
    return 0;   /* End of file */
  }else{
    return -1;  /* Syntax error */
  }
}

/*
** Parse a complete JSON string.  Return 0 on success or non-zero if there
** are any errors.  If an error occurs, free all memory associated with
** pParse.
**
** pParse is uninitialized when this routine is called.
*/
static int jsonParse(
  JsonParse *pParse,           /* Initialize and fill this JsonParse object */
  sqlite3_context *pCtx,       /* Report errors here */
  const char *zJson            /* Input JSON text to be parsed */
){
  int i;
  memset(pParse, 0, sizeof(*pParse));
  if( zJson==0 ) return 1;
  pParse->zJson = zJson;
  i = jsonParseValue(pParse, 0);
  if( pParse->oom ) i = -1;
  if( i>0 ){
    assert( pParse->iDepth==0 );
    while( safe_isspace(zJson[i]) ) i++;
    if( zJson[i] ) i = -1;
  }
  if( i<=0 ){
    if( pCtx!=0 ){
      if( pParse->oom ){
        sqlite3_result_error_nomem(pCtx);
      }else{
        sqlite3_result_error(pCtx, "malformed JSON", -1);
      }
    }
    jsonParseReset(pParse);
    return 1;
  }
  return 0;
}

/* Mark node i of pParse as being a child of iParent.  Call recursively
** to fill in all the descendants of node i.
*/
static void jsonParseFillInParentage(JsonParse *pParse, u32 i, u32 iParent){
  JsonNode *pNode = &pParse->aNode[i];
  u32 j;
  pParse->aUp[i] = iParent;
  switch( pNode->eType ){
    case JSON_ARRAY: {
      for(j=1; j<=pNode->n; j += jsonNodeSize(pNode+j)){
        jsonParseFillInParentage(pParse, i+j, i);
      }
      break;
    }
    case JSON_OBJECT: {
      for(j=1; j<=pNode->n; j += jsonNodeSize(pNode+j+1)+1){
        pParse->aUp[i+j] = i;
        jsonParseFillInParentage(pParse, i+j+1, i);
      }
      break;
    }
    default: {
      break;
    }
  }
}

/*
** Compute the parentage of all nodes in a completed parse.
*/
static int jsonParseFindParents(JsonParse *pParse){
  u32 *aUp;
  assert( pParse->aUp==0 );
  aUp = pParse->aUp = sqlite3_malloc64( sizeof(u32)*pParse->nNode );
  if( aUp==0 ){
    pParse->oom = 1;
    return SQLITE_NOMEM;
  }
  jsonParseFillInParentage(pParse, 0, 0);
  return SQLITE_OK;
}

/*
** Magic number used for the JSON parse cache in sqlite3_get_auxdata()
*/
#define JSON_CACHE_ID  (-429938)  /* First cache entry */
#define JSON_CACHE_SZ  4          /* Max number of cache entries */

/*
** Obtain a complete parse of the JSON found in the first argument
** of the argv array.  Use the sqlite3_get_auxdata() cache for this
** parse if it is available.  If the cache is not available or if it
** is no longer valid, parse the JSON again and return the new parse,
** and also register the new parse so that it will be available for
** future sqlite3_get_auxdata() calls.
*/
static JsonParse *jsonParseCached(
  sqlite3_context *pCtx,
  sqlite3_value **argv,
  sqlite3_context *pErrCtx
){
  const char *zJson = (const char*)sqlite3_value_text(argv[0]);
  int nJson = sqlite3_value_bytes(argv[0]);
  JsonParse *p;
  JsonParse *pMatch = 0;
  int iKey;
  int iMinKey = 0;
  u32 iMinHold = 0xffffffff;
  u32 iMaxHold = 0;
  if( zJson==0 ) return 0;
  for(iKey=0; iKey<JSON_CACHE_SZ; iKey++){
    p = (JsonParse*)sqlite3_get_auxdata(pCtx, JSON_CACHE_ID+iKey);
    if( p==0 ){
      iMinKey = iKey;
      break;
    }
    if( pMatch==0
     && p->nJson==nJson
     && memcmp(p->zJson,zJson,nJson)==0
    ){
      p->nErr = 0;
      pMatch = p;
    }else if( p->iHold<iMinHold ){
      iMinHold = p->iHold;
      iMinKey = iKey;
    }
    if( p->iHold>iMaxHold ){
      iMaxHold = p->iHold;
    }
  }
  if( pMatch ){
    pMatch->nErr = 0;
    pMatch->iHold = iMaxHold+1;
    return pMatch;
  }
  p = sqlite3_malloc64( sizeof(*p) + nJson + 1 );
  if( p==0 ){
    sqlite3_result_error_nomem(pCtx);
    return 0;
  }
  memset(p, 0, sizeof(*p));
  p->zJson = (char*)&p[1];
  memcpy((char*)p->zJson, zJson, nJson+1);
  if( jsonParse(p, pErrCtx, p->zJson) ){
    sqlite3_free(p);
    return 0;
  }
  p->nJson = nJson;
  p->iHold = iMaxHold+1;
  sqlite3_set_auxdata(pCtx, JSON_CACHE_ID+iMinKey, p,
                      (void(*)(void*))jsonParseFree);
  return (JsonParse*)sqlite3_get_auxdata(pCtx, JSON_CACHE_ID+iMinKey);
}

/*
** Compare the OBJECT label at pNode against zKey,nKey.  Return true on
** a match.
*/
static int jsonLabelCompare(JsonNode *pNode, const char *zKey, u32 nKey){
  if( pNode->jnFlags & JNODE_RAW ){
    if( pNode->n!=nKey ) return 0;
    return strncmp(pNode->u.zJContent, zKey, nKey)==0;
  }else{
    if( pNode->n!=nKey+2 ) return 0;
    return strncmp(pNode->u.zJContent+1, zKey, nKey)==0;
  }
}

/* forward declaration */
static JsonNode *jsonLookupAppend(JsonParse*,const char*,int*,const char**);

/*
** Search along zPath to find the node specified.  Return a pointer
** to that node, or NULL if zPath is malformed or if there is no such
** node.
**
** If pApnd!=0, then try to append new nodes to complete zPath if it is
** possible to do so and if no existing node corresponds to zPath.  If
** new nodes are appended *pApnd is set to 1.
*/
static JsonNode *jsonLookupStep(
  JsonParse *pParse,      /* The JSON to search */
  u32 iRoot,              /* Begin the search at this node */
  const char *zPath,      /* The path to search */
  int *pApnd,             /* Append nodes to complete path if not NULL */
  const char **pzErr      /* Make *pzErr point to any syntax error in zPath */
){
  u32 i, j, nKey;
  const char *zKey;
  JsonNode *pRoot = &pParse->aNode[iRoot];
  if( zPath[0]==0 ) return pRoot;
  if( pRoot->jnFlags & JNODE_REPLACE ) return 0;
  if( zPath[0]=='.' ){
    if( pRoot->eType!=JSON_OBJECT ) return 0;
    zPath++;
    if( zPath[0]=='"' ){
      zKey = zPath + 1;
      for(i=1; zPath[i] && zPath[i]!='"'; i++){}
      nKey = i-1;
      if( zPath[i] ){
        i++;
      }else{
        *pzErr = zPath;
        return 0;
      }
    }else{
      zKey = zPath;
      for(i=0; zPath[i] && zPath[i]!='.' && zPath[i]!='['; i++){}
      nKey = i;
    }
    if( nKey==0 ){
      *pzErr = zPath;
      return 0;
    }
    j = 1;
    for(;;){
      while( j<=pRoot->n ){
        if( jsonLabelCompare(pRoot+j, zKey, nKey) ){
          return jsonLookupStep(pParse, iRoot+j+1, &zPath[i], pApnd, pzErr);
        }
        j++;
        j += jsonNodeSize(&pRoot[j]);
      }
      if( (pRoot->jnFlags & JNODE_APPEND)==0 ) break;
      iRoot += pRoot->u.iAppend;
      pRoot = &pParse->aNode[iRoot];
      j = 1;
    }
    if( pApnd ){
      u32 iStart, iLabel;
      JsonNode *pNode;
      iStart = jsonParseAddNode(pParse, JSON_OBJECT, 2, 0);
      iLabel = jsonParseAddNode(pParse, JSON_STRING, nKey, zKey);
      zPath += i;
      pNode = jsonLookupAppend(pParse, zPath, pApnd, pzErr);
      if( pParse->oom ) return 0;
      if( pNode ){
        pRoot = &pParse->aNode[iRoot];
        pRoot->u.iAppend = iStart - iRoot;
        pRoot->jnFlags |= JNODE_APPEND;
        pParse->aNode[iLabel].jnFlags |= JNODE_RAW;
      }
      return pNode;
    }
  }else if( zPath[0]=='[' ){
    i = 0;
    j = 1;
    while( safe_isdigit(zPath[j]) ){
      i = i*10 + zPath[j] - '0';
      j++;
    }
    if( j<2 || zPath[j]!=']' ){
      if( zPath[1]=='#' ){
        JsonNode *pBase = pRoot;
        int iBase = iRoot;
        if( pRoot->eType!=JSON_ARRAY ) return 0;
        for(;;){
          while( j<=pBase->n ){
            if( (pBase[j].jnFlags & JNODE_REMOVE)==0 ) i++;
            j += jsonNodeSize(&pBase[j]);
          }
          if( (pBase->jnFlags & JNODE_APPEND)==0 ) break;
          iBase += pBase->u.iAppend;
          pBase = &pParse->aNode[iBase];
          j = 1;
        }
        j = 2;
        if( zPath[2]=='-' && safe_isdigit(zPath[3]) ){
          unsigned int x = 0;
          j = 3;
          do{
            x = x*10 + zPath[j] - '0';
            j++;
          }while( safe_isdigit(zPath[j]) );
          if( x>i ) return 0;
          i -= x;
        }
        if( zPath[j]!=']' ){
          *pzErr = zPath;
          return 0;
        }
      }else{
        *pzErr = zPath;
        return 0;
      }
    }
    if( pRoot->eType!=JSON_ARRAY ) return 0;
    zPath += j + 1;
    j = 1;
    for(;;){
      while( j<=pRoot->n && (i>0 || (pRoot[j].jnFlags & JNODE_REMOVE)!=0) ){
        if( (pRoot[j].jnFlags & JNODE_REMOVE)==0 ) i--;
        j += jsonNodeSize(&pRoot[j]);
      }
      if( (pRoot->jnFlags & JNODE_APPEND)==0 ) break;
      iRoot += pRoot->u.iAppend;
      pRoot = &pParse->aNode[iRoot];
      j = 1;
    }
    if( j<=pRoot->n ){
      return jsonLookupStep(pParse, iRoot+j, zPath, pApnd, pzErr);
    }
    if( i==0 && pApnd ){
      u32 iStart;
      JsonNode *pNode;
      iStart = jsonParseAddNode(pParse, JSON_ARRAY, 1, 0);
      pNode = jsonLookupAppend(pParse, zPath, pApnd, pzErr);
      if( pParse->oom ) return 0;
      if( pNode ){
        pRoot = &pParse->aNode[iRoot];
        pRoot->u.iAppend = iStart - iRoot;
        pRoot->jnFlags |= JNODE_APPEND;
      }
      return pNode;
    }
  }else{
    *pzErr = zPath;
  }
  return 0;
}

/*
** Append content to pParse that will complete zPath.  Return a pointer
** to the inserted node, or return NULL if the append fails.
*/
static JsonNode *jsonLookupAppend(
  JsonParse *pParse,     /* Append content to the JSON parse */
  const char *zPath,     /* Description of content to append */
  int *pApnd,            /* Set this flag to 1 */
  const char **pzErr     /* Make this point to any syntax error */
){
  *pApnd = 1;
  if( zPath[0]==0 ){
    jsonParseAddNode(pParse, JSON_NULL, 0, 0);
    return pParse->oom ? 0 : &pParse->aNode[pParse->nNode-1];
  }
  if( zPath[0]=='.' ){
    jsonParseAddNode(pParse, JSON_OBJECT, 0, 0);
  }else if( strncmp(zPath,"[0]",3)==0 ){
    jsonParseAddNode(pParse, JSON_ARRAY, 0, 0);
  }else{
    return 0;
  }
  if( pParse->oom ) return 0;
  return jsonLookupStep(pParse, pParse->nNode-1, zPath, pApnd, pzErr);
}

/*
** Return the text of a syntax error message on a JSON path.  Space is
** obtained from sqlite3_malloc().
*/
static char *jsonPathSyntaxError(const char *zErr){
  return sqlite3_mprintf("JSON path error near '%q'", zErr);
}

/*
** Do a node lookup using zPath.  Return a pointer to the node on success.
** Return NULL if not found or if there is an error.
**
** On an error, write an error message into pCtx and increment the
** pParse->nErr counter.
**
** If pApnd!=NULL then try to append missing nodes and set *pApnd = 1 if
** nodes are appended.
*/
static JsonNode *jsonLookup(
  JsonParse *pParse,      /* The JSON to search */
  const char *zPath,      /* The path to search */
  int *pApnd,             /* Append nodes to complete path if not NULL */
  sqlite3_context *pCtx   /* Report errors here, if not NULL */
){
  const char *zErr = 0;
  JsonNode *pNode = 0;
  char *zMsg;

  if( zPath==0 ) return 0;
  if( zPath[0]!='$' ){
    zErr = zPath;
    goto lookup_err;
  }
  zPath++;
  pNode = jsonLookupStep(pParse, 0, zPath, pApnd, &zErr);
  if( zErr==0 ) return pNode;

lookup_err:
  pParse->nErr++;
  assert( zErr!=0 && pCtx!=0 );
  zMsg = jsonPathSyntaxError(zErr);
  if( zMsg ){
    sqlite3_result_error(pCtx, zMsg, -1);
    sqlite3_free(zMsg);
  }else{
    sqlite3_result_error_nomem(pCtx);
  }
  return 0;
}


/*
** Report the wrong number of arguments for json_insert(), json_replace()
** or json_set().
*/
static void jsonWrongNumArgs(
  sqlite3_context *pCtx,
  const char *zFuncName
){
  char *zMsg = sqlite3_mprintf("json_%s() needs an odd number of arguments",
                               zFuncName);
  sqlite3_result_error(pCtx, zMsg, -1);
  sqlite3_free(zMsg);
}

/*
** Mark all NULL entries in the Object passed in as JNODE_REMOVE.
*/
static void jsonRemoveAllNulls(JsonNode *pNode){
  int i, n;
  assert( pNode->eType==JSON_OBJECT );
  n = pNode->n;
  for(i=2; i<=n; i += jsonNodeSize(&pNode[i])+1){
    switch( pNode[i].eType ){
      case JSON_NULL:
        pNode[i].jnFlags |= JNODE_REMOVE;
        break;
      case JSON_OBJECT:
        jsonRemoveAllNulls(&pNode[i]);
        break;
    }
  }
}


/****************************************************************************
** SQL functions used for testing and debugging
****************************************************************************/

#ifdef SQLITE_DEBUG
/*
** The json_parse(JSON) function returns a string which describes
** a parse of the JSON provided.  Or it returns NULL if JSON is not
** well-formed.
*/
static void jsonParseFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonString s;       /* Output string - not real JSON */
  JsonParse x;        /* The parse */
  u32 i;

  assert( argc==1 );
  if( jsonParse(&x, ctx, (const char*)sqlite3_value_text(argv[0])) ) return;
  jsonParseFindParents(&x);
  jsonInit(&s, ctx);
  for(i=0; i<x.nNode; i++){
    const char *zType;
    if( x.aNode[i].jnFlags & JNODE_LABEL ){
      assert( x.aNode[i].eType==JSON_STRING );
      zType = "label";
    }else{
      zType = jsonType[x.aNode[i].eType];
    }
    jsonPrintf(100, &s,"node %3u: %7s n=%-4d up=%-4d",
               i, zType, x.aNode[i].n, x.aUp[i]);
    if( x.aNode[i].u.zJContent!=0 ){
      jsonAppendRaw(&s, " ", 1);
      jsonAppendRaw(&s, x.aNode[i].u.zJContent, x.aNode[i].n);
    }
    jsonAppendRaw(&s, "\n", 1);
  }
  jsonParseReset(&x);
  jsonResult(&s);
}

/*
** The json_test1(JSON) function return true (1) if the input is JSON
** text generated by another json function.  It returns (0) if the input
** is not known to be JSON.
*/
static void jsonTest1Func(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  UNUSED_PARAM(argc);
  sqlite3_result_int(ctx, sqlite3_value_subtype(argv[0])==JSON_SUBTYPE);
}
#endif /* SQLITE_DEBUG */

/****************************************************************************
** Scalar SQL function implementations
****************************************************************************/

/*
** Implementation of the json_QUOTE(VALUE) function.  Return a JSON value
** corresponding to the SQL value input.  Mostly this means putting
** double-quotes around strings and returning the unquoted string "null"
** when given a NULL input.
*/
static void jsonQuoteFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonString jx;
  UNUSED_PARAM(argc);

  jsonInit(&jx, ctx);
  jsonAppendValue(&jx, argv[0]);
  jsonResult(&jx);
  sqlite3_result_subtype(ctx, JSON_SUBTYPE);
}

/*
** Implementation of the json_array(VALUE,...) function.  Return a JSON
** array that contains all values given in arguments.  Or if any argument
** is a BLOB, throw an error.
*/
static void jsonArrayFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  int i;
  JsonString jx;

  jsonInit(&jx, ctx);
  jsonAppendChar(&jx, '[');
  for(i=0; i<argc; i++){
    jsonAppendSeparator(&jx);
    jsonAppendValue(&jx, argv[i]);
  }
  jsonAppendChar(&jx, ']');
  jsonResult(&jx);
  sqlite3_result_subtype(ctx, JSON_SUBTYPE);
}


/*
** json_array_length(JSON)
** json_array_length(JSON, PATH)
**
** Return the number of elements in the top-level JSON array.
** Return 0 if the input is not a well-formed JSON array.
*/
static void jsonArrayLengthFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse *p;          /* The parse */
  sqlite3_int64 n = 0;
  u32 i;
  JsonNode *pNode;

  p = jsonParseCached(ctx, argv, ctx);
  if( p==0 ) return;
  assert( p->nNode );
  if( argc==2 ){
    const char *zPath = (const char*)sqlite3_value_text(argv[1]);
    pNode = jsonLookup(p, zPath, 0, ctx);
  }else{
    pNode = p->aNode;
  }
  if( pNode==0 ){
    return;
  }
  if( pNode->eType==JSON_ARRAY ){
    assert( (pNode->jnFlags & JNODE_APPEND)==0 );
    for(i=1; i<=pNode->n; n++){
      i += jsonNodeSize(&pNode[i]);
    }
  }
  sqlite3_result_int64(ctx, n);
}

/*
** json_extract(JSON, PATH, ...)
**
** Return the element described by PATH.  Return NULL if there is no
** PATH element.  If there are multiple PATHs, then return a JSON array
** with the result from each path.  Throw an error if the JSON or any PATH
** is malformed.
*/
static void jsonExtractFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse *p;          /* The parse */
  JsonNode *pNode;
  const char *zPath;
  JsonString jx;
  int i;

  if( argc<2 ) return;
  p = jsonParseCached(ctx, argv, ctx);
  if( p==0 ) return;
  jsonInit(&jx, ctx);
  jsonAppendChar(&jx, '[');
  for(i=1; i<argc; i++){
    zPath = (const char*)sqlite3_value_text(argv[i]);
    pNode = jsonLookup(p, zPath, 0, ctx);
    if( p->nErr ) break;
    if( argc>2 ){
      jsonAppendSeparator(&jx);
      if( pNode ){
        jsonRenderNode(pNode, &jx, 0);
      }else{
        jsonAppendRaw(&jx, "null", 4);
      }
    }else if( pNode ){
      jsonReturn(pNode, ctx, 0);
    }
  }
  if( argc>2 && i==argc ){
    jsonAppendChar(&jx, ']');
    jsonResult(&jx);
    sqlite3_result_subtype(ctx, JSON_SUBTYPE);
  }
  jsonReset(&jx);
}

/* This is the RFC 7396 MergePatch algorithm.
*/
static JsonNode *jsonMergePatch(
  JsonParse *pParse,   /* The JSON parser that contains the TARGET */
  u32 iTarget,         /* Node of the TARGET in pParse */
  JsonNode *pPatch     /* The PATCH */
){
  u32 i, j;
  u32 iRoot;
  JsonNode *pTarget;
  if( pPatch->eType!=JSON_OBJECT ){
    return pPatch;
  }
  assert( iTarget>=0 && iTarget<pParse->nNode );
  pTarget = &pParse->aNode[iTarget];
  assert( (pPatch->jnFlags & JNODE_APPEND)==0 );
  if( pTarget->eType!=JSON_OBJECT ){
    jsonRemoveAllNulls(pPatch);
    return pPatch;
  }
  iRoot = iTarget;
  for(i=1; i<pPatch->n; i += jsonNodeSize(&pPatch[i+1])+1){
    u32 nKey;
    const char *zKey;
    assert( pPatch[i].eType==JSON_STRING );
    assert( pPatch[i].jnFlags & JNODE_LABEL );
    nKey = pPatch[i].n;
    zKey = pPatch[i].u.zJContent;
    assert( (pPatch[i].jnFlags & JNODE_RAW)==0 );
    for(j=1; j<pTarget->n; j += jsonNodeSize(&pTarget[j+1])+1 ){
      assert( pTarget[j].eType==JSON_STRING );
      assert( pTarget[j].jnFlags & JNODE_LABEL );
      assert( (pPatch[i].jnFlags & JNODE_RAW)==0 );
      if( pTarget[j].n==nKey && strncmp(pTarget[j].u.zJContent,zKey,nKey)==0 ){
        if( pTarget[j+1].jnFlags & (JNODE_REMOVE|JNODE_PATCH) ) break;
        if( pPatch[i+1].eType==JSON_NULL ){
          pTarget[j+1].jnFlags |= JNODE_REMOVE;
        }else{
          JsonNode *pNew = jsonMergePatch(pParse, iTarget+j+1, &pPatch[i+1]);
          if( pNew==0 ) return 0;
          pTarget = &pParse->aNode[iTarget];
          if( pNew!=&pTarget[j+1] ){
            pTarget[j+1].u.pPatch = pNew;
            pTarget[j+1].jnFlags |= JNODE_PATCH;
          }
        }
        break;
      }
    }
    if( j>=pTarget->n && pPatch[i+1].eType!=JSON_NULL ){
      int iStart, iPatch;
      iStart = jsonParseAddNode(pParse, JSON_OBJECT, 2, 0);
      jsonParseAddNode(pParse, JSON_STRING, nKey, zKey);
      iPatch = jsonParseAddNode(pParse, JSON_TRUE, 0, 0);
      if( pParse->oom ) return 0;
      jsonRemoveAllNulls(pPatch);
      pTarget = &pParse->aNode[iTarget];
      pParse->aNode[iRoot].jnFlags |= JNODE_APPEND;
      pParse->aNode[iRoot].u.iAppend = iStart - iRoot;
      iRoot = iStart;
      pParse->aNode[iPatch].jnFlags |= JNODE_PATCH;
      pParse->aNode[iPatch].u.pPatch = &pPatch[i+1];
    }
  }
  return pTarget;
}

/*
** Implementation of the json_mergepatch(JSON1,JSON2) function.  Return a JSON
** object that is the result of running the RFC 7396 MergePatch() algorithm
** on the two arguments.
*/
static void jsonPatchFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse x;     /* The JSON that is being patched */
  JsonParse y;     /* The patch */
  JsonNode *pResult;   /* The result of the merge */

  UNUSED_PARAM(argc);
  if( jsonParse(&x, ctx, (const char*)sqlite3_value_text(argv[0])) ) return;
  if( jsonParse(&y, ctx, (const char*)sqlite3_value_text(argv[1])) ){
    jsonParseReset(&x);
    return;
  }
  pResult = jsonMergePatch(&x, 0, y.aNode);
  assert( pResult!=0 || x.oom );
  if( pResult ){
    jsonReturnJson(pResult, ctx, 0);
  }else{
    sqlite3_result_error_nomem(ctx);
  }
  jsonParseReset(&x);
  jsonParseReset(&y);
}


/*
** Implementation of the json_object(NAME,VALUE,...) function.  Return a JSON
** object that contains all name/value given in arguments.  Or if any name
** is not a string or if any value is a BLOB, throw an error.
*/
static void jsonObjectFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  int i;
  JsonString jx;
  const char *z;
  u32 n;

  if( argc&1 ){
    sqlite3_result_error(ctx, "json_object() requires an even number "
                                  "of arguments", -1);
    return;
  }
  jsonInit(&jx, ctx);
  jsonAppendChar(&jx, '{');
  for(i=0; i<argc; i+=2){
    if( sqlite3_value_type(argv[i])!=SQLITE_TEXT ){
      sqlite3_result_error(ctx, "json_object() labels must be TEXT", -1);
      jsonReset(&jx);
      return;
    }
    jsonAppendSeparator(&jx);
    z = (const char*)sqlite3_value_text(argv[i]);
    n = (u32)sqlite3_value_bytes(argv[i]);
    jsonAppendString(&jx, z, n);
    jsonAppendChar(&jx, ':');
    jsonAppendValue(&jx, argv[i+1]);
  }
  jsonAppendChar(&jx, '}');
  jsonResult(&jx);
  sqlite3_result_subtype(ctx, JSON_SUBTYPE);
}


/*
** json_remove(JSON, PATH, ...)
**
** Remove the named elements from JSON and return the result.  malformed
** JSON or PATH arguments result in an error.
*/
static void jsonRemoveFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse x;          /* The parse */
  JsonNode *pNode;
  const char *zPath;
  u32 i;

  if( argc<1 ) return;
  if( jsonParse(&x, ctx, (const char*)sqlite3_value_text(argv[0])) ) return;
  assert( x.nNode );
  for(i=1; i<(u32)argc; i++){
    zPath = (const char*)sqlite3_value_text(argv[i]);
    if( zPath==0 ) goto remove_done;
    pNode = jsonLookup(&x, zPath, 0, ctx);
    if( x.nErr ) goto remove_done;
    if( pNode ) pNode->jnFlags |= JNODE_REMOVE;
  }
  if( (x.aNode[0].jnFlags & JNODE_REMOVE)==0 ){
    jsonReturnJson(x.aNode, ctx, 0);
  }
remove_done:
  jsonParseReset(&x);
}

/*
** json_replace(JSON, PATH, VALUE, ...)
**
** Replace the value at PATH with VALUE.  If PATH does not already exist,
** this routine is a no-op.  If JSON or PATH is malformed, throw an error.
*/
static void jsonReplaceFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse x;          /* The parse */
  JsonNode *pNode;
  const char *zPath;
  u32 i;

  if( argc<1 ) return;
  if( (argc&1)==0 ) {
    jsonWrongNumArgs(ctx, "replace");
    return;
  }
  if( jsonParse(&x, ctx, (const char*)sqlite3_value_text(argv[0])) ) return;
  assert( x.nNode );
  for(i=1; i<(u32)argc; i+=2){
    zPath = (const char*)sqlite3_value_text(argv[i]);
    pNode = jsonLookup(&x, zPath, 0, ctx);
    if( x.nErr ) goto replace_err;
    if( pNode ){
      pNode->jnFlags |= (u8)JNODE_REPLACE;
      pNode->u.iReplace = i + 1;
    }
  }
  if( x.aNode[0].jnFlags & JNODE_REPLACE ){
    sqlite3_result_value(ctx, argv[x.aNode[0].u.iReplace]);
  }else{
    jsonReturnJson(x.aNode, ctx, argv);
  }
replace_err:
  jsonParseReset(&x);
}

/*
** json_set(JSON, PATH, VALUE, ...)
**
** Set the value at PATH to VALUE.  Create the PATH if it does not already
** exist.  Overwrite existing values that do exist.
** If JSON or PATH is malformed, throw an error.
**
** json_insert(JSON, PATH, VALUE, ...)
**
** Create PATH and initialize it to VALUE.  If PATH already exists, this
** routine is a no-op.  If JSON or PATH is malformed, throw an error.
*/
static void jsonSetFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse x;          /* The parse */
  JsonNode *pNode;
  const char *zPath;
  u32 i;
  int bApnd;
  int bIsSet = *(int*)sqlite3_user_data(ctx);

  if( argc<1 ) return;
  if( (argc&1)==0 ) {
    jsonWrongNumArgs(ctx, bIsSet ? "set" : "insert");
    return;
  }
  if( jsonParse(&x, ctx, (const char*)sqlite3_value_text(argv[0])) ) return;
  assert( x.nNode );
  for(i=1; i<(u32)argc; i+=2){
    zPath = (const char*)sqlite3_value_text(argv[i]);
    bApnd = 0;
    pNode = jsonLookup(&x, zPath, &bApnd, ctx);
    if( x.oom ){
      sqlite3_result_error_nomem(ctx);
      goto jsonSetDone;
    }else if( x.nErr ){
      goto jsonSetDone;
    }else if( pNode && (bApnd || bIsSet) ){
      pNode->jnFlags |= (u8)JNODE_REPLACE;
      pNode->u.iReplace = i + 1;
    }
  }
  if( x.aNode[0].jnFlags & JNODE_REPLACE ){
    sqlite3_result_value(ctx, argv[x.aNode[0].u.iReplace]);
  }else{
    jsonReturnJson(x.aNode, ctx, argv);
  }
jsonSetDone:
  jsonParseReset(&x);
}

/*
** json_type(JSON)
** json_type(JSON, PATH)
**
** Return the top-level "type" of a JSON string.  Throw an error if
** either the JSON or PATH inputs are not well-formed.
*/
static void jsonTypeFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse *p;          /* The parse */
  const char *zPath;
  JsonNode *pNode;

  p = jsonParseCached(ctx, argv, ctx);
  if( p==0 ) return;
  if( argc==2 ){
    zPath = (const char*)sqlite3_value_text(argv[1]);
    pNode = jsonLookup(p, zPath, 0, ctx);
  }else{
    pNode = p->aNode;
  }
  if( pNode ){
    sqlite3_result_text(ctx, jsonType[pNode->eType], -1, SQLITE_STATIC);
  }
}

/*
** json_valid(JSON)
**
** Return 1 if JSON is a well-formed JSON string according to RFC-7159.
** Return 0 otherwise.
*/
static void jsonValidFunc(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonParse *p;          /* The parse */
  UNUSED_PARAM(argc);
  p = jsonParseCached(ctx, argv, 0);
  sqlite3_result_int(ctx, p!=0);
}


/****************************************************************************
** Aggregate SQL function implementations
****************************************************************************/
/*
** json_group_array(VALUE)
**
** Return a JSON array composed of all values in the aggregate.
*/
static void jsonArrayStep(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonString *pStr;
  UNUSED_PARAM(argc);
  pStr = (JsonString*)sqlite3_aggregate_context(ctx, sizeof(*pStr));
  if( pStr ){
    if( pStr->zBuf==0 ){
      jsonInit(pStr, ctx);
      jsonAppendChar(pStr, '[');
    }else if( pStr->nUsed>1 ){
      jsonAppendChar(pStr, ',');
    }
    pStr->pCtx = ctx;
    jsonAppendValue(pStr, argv[0]);
  }
}
static void jsonArrayCompute(sqlite3_context *ctx, int isFinal){
  JsonString *pStr;
  pStr = (JsonString*)sqlite3_aggregate_context(ctx, 0);
  if( pStr ){
    pStr->pCtx = ctx;
    jsonAppendChar(pStr, ']');
    if( pStr->bErr ){
      if( pStr->bErr==1 ) sqlite3_result_error_nomem(ctx);
      assert( pStr->bStatic );
    }else if( isFinal ){
      sqlite3_result_text(ctx, pStr->zBuf, (int)pStr->nUsed,
                          pStr->bStatic ? SQLITE_TRANSIENT : sqlite3_free);
      pStr->bStatic = 1;
    }else{
      sqlite3_result_text(ctx, pStr->zBuf, (int)pStr->nUsed, SQLITE_TRANSIENT);
      pStr->nUsed--;
    }
  }else{
    sqlite3_result_text(ctx, "[]", 2, SQLITE_STATIC);
  }
  sqlite3_result_subtype(ctx, JSON_SUBTYPE);
}
static void jsonArrayValue(sqlite3_context *ctx){
  jsonArrayCompute(ctx, 0);
}
static void jsonArrayFinal(sqlite3_context *ctx){
  jsonArrayCompute(ctx, 1);
}

#ifndef SQLITE_OMIT_WINDOWFUNC
/*
** This method works for both json_group_array() and json_group_object().
** It works by removing the first element of the group by searching forward
** to the first comma (",") that is not within a string and deleting all
** text through that comma.
*/
static void jsonGroupInverse(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  unsigned int i;
  int inStr = 0;
  int nNest = 0;
  char *z;
  char c;
  JsonString *pStr;
  UNUSED_PARAM(argc);
  UNUSED_PARAM(argv);
  pStr = (JsonString*)sqlite3_aggregate_context(ctx, 0);
#ifdef NEVER
  /* pStr is always non-NULL since jsonArrayStep() or jsonObjectStep() will
  ** always have been called to initalize it */
  if( NEVER(!pStr) ) return;
#endif
  z = pStr->zBuf;
  for(i=1; i<pStr->nUsed && ((c = z[i])!=',' || inStr || nNest); i++){
    if( c=='"' ){
      inStr = !inStr;
    }else if( c=='\\' ){
      i++;
    }else if( !inStr ){
      if( c=='{' || c=='[' ) nNest++;
      if( c=='}' || c==']' ) nNest--;
    }
  }
  if( i<pStr->nUsed ){
    pStr->nUsed -= i;
    memmove(&z[1], &z[i+1], (size_t)pStr->nUsed-1);
    z[pStr->nUsed] = 0;
  }else{
    pStr->nUsed = 1;
  }
}
#else
# define jsonGroupInverse 0
#endif


/*
** json_group_obj(NAME,VALUE)
**
** Return a JSON object composed of all names and values in the aggregate.
*/
static void jsonObjectStep(
  sqlite3_context *ctx,
  int argc,
  sqlite3_value **argv
){
  JsonString *pStr;
  const char *z;
  u32 n;
  UNUSED_PARAM(argc);
  pStr = (JsonString*)sqlite3_aggregate_context(ctx, sizeof(*pStr));
  if( pStr ){
    if( pStr->zBuf==0 ){
      jsonInit(pStr, ctx);
      jsonAppendChar(pStr, '{');
    }else if( pStr->nUsed>1 ){
      jsonAppendChar(pStr, ',');
    }
    pStr->pCtx = ctx;
    z = (const char*)sqlite3_value_text(argv[0]);
    n = (u32)sqlite3_value_bytes(argv[0]);
    jsonAppendString(pStr, z, n);
    jsonAppendChar(pStr, ':');
    jsonAppendValue(pStr, argv[1]);
  }
}
static void jsonObjectCompute(sqlite3_context *ctx, int isFinal){
  JsonString *pStr;
  pStr = (JsonString*)sqlite3_aggregate_context(ctx, 0);
  if( pStr ){
    jsonAppendChar(pStr, '}');
    if( pStr->bErr ){
      if( pStr->bErr==1 ) sqlite3_result_error_nomem(ctx);
      assert( pStr->bStatic );
    }else if( isFinal ){
      sqlite3_result_text(ctx, pStr->zBuf, (int)pStr->nUsed,
                          pStr->bStatic ? SQLITE_TRANSIENT : sqlite3_free);
      pStr->bStatic = 1;
    }else{
      sqlite3_result_text(ctx, pStr->zBuf, (int)pStr->nUsed, SQLITE_TRANSIENT);
      pStr->nUsed--;
    }
  }else{
    sqlite3_result_text(ctx, "{}", 2, SQLITE_STATIC);
  }
  sqlite3_result_subtype(ctx, JSON_SUBTYPE);
}
static void jsonObjectValue(sqlite3_context *ctx){
  jsonObjectCompute(ctx, 0);
}
static void jsonObjectFinal(sqlite3_context *ctx){
  jsonObjectCompute(ctx, 1);
}


#ifndef SQLITE_OMIT_VIRTUALTABLE
/****************************************************************************
** The json_each virtual table
****************************************************************************/
typedef struct JsonEachCursor JsonEachCursor;
struct JsonEachCursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  u32 iRowid;                /* The rowid */
  u32 iBegin;                /* The first node of the scan */
  u32 i;                     /* Index in sParse.aNode[] of current row */
  u32 iEnd;                  /* EOF when i equals or exceeds this value */
  u8 eType;                  /* Type of top-level element */
  u8 bRecursive;             /* True for json_tree().  False for json_each() */
  char *zJson;               /* Input JSON */
  char *zRoot;               /* Path by which to filter zJson */
  JsonParse sParse;          /* Parse of the input JSON */
};

/* Constructor for the json_each virtual table */
static int jsonEachConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  sqlite3_vtab *pNew;
  int rc;

/* Column numbers */
#define JEACH_KEY     0
#define JEACH_VALUE   1
#define JEACH_TYPE    2
#define JEACH_ATOM    3
#define JEACH_ID      4
#define JEACH_PARENT  5
#define JEACH_FULLKEY 6
#define JEACH_PATH    7
/* The xBestIndex method assumes that the JSON and ROOT columns are
** the last two columns in the table.  Should this ever changes, be
** sure to update the xBestIndex method. */
#define JEACH_JSON    8
#define JEACH_ROOT    9

  UNUSED_PARAM(pzErr);
  UNUSED_PARAM(argv);
  UNUSED_PARAM(argc);
  UNUSED_PARAM(pAux);
  rc = sqlite3_declare_vtab(db,
     "CREATE TABLE x(key,value,type,atom,id,parent,fullkey,path,"
                    "json HIDDEN,root HIDDEN)");
  if( rc==SQLITE_OK ){
    pNew = *ppVtab = sqlite3_malloc( sizeof(*pNew) );
    if( pNew==0 ) return SQLITE_NOMEM;
    memset(pNew, 0, sizeof(*pNew));
    sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS);
  }
  return rc;
}

/* destructor for json_each virtual table */
static int jsonEachDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/* constructor for a JsonEachCursor object for json_each(). */
static int jsonEachOpenEach(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  JsonEachCursor *pCur;

  UNUSED_PARAM(p);
  pCur = sqlite3_malloc( sizeof(*pCur) );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(*pCur));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/* constructor for a JsonEachCursor object for json_tree(). */
static int jsonEachOpenTree(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  int rc = jsonEachOpenEach(p, ppCursor);
  if( rc==SQLITE_OK ){
    JsonEachCursor *pCur = (JsonEachCursor*)*ppCursor;
    pCur->bRecursive = 1;
  }
  return rc;
}

/* Reset a JsonEachCursor back to its original state.  Free any memory
** held. */
static void jsonEachCursorReset(JsonEachCursor *p){
  sqlite3_free(p->zJson);
  sqlite3_free(p->zRoot);
  jsonParseReset(&p->sParse);
  p->iRowid = 0;
  p->i = 0;
  p->iEnd = 0;
  p->eType = 0;
  p->zJson = 0;
  p->zRoot = 0;
}

/* Destructor for a jsonEachCursor object */
static int jsonEachClose(sqlite3_vtab_cursor *cur){
  JsonEachCursor *p = (JsonEachCursor*)cur;
  jsonEachCursorReset(p);
  sqlite3_free(cur);
  return SQLITE_OK;
}

/* Return TRUE if the jsonEachCursor object has been advanced off the end
** of the JSON object */
static int jsonEachEof(sqlite3_vtab_cursor *cur){
  JsonEachCursor *p = (JsonEachCursor*)cur;
  return p->i >= p->iEnd;
}

/* Advance the cursor to the next element for json_tree() */
static int jsonEachNext(sqlite3_vtab_cursor *cur){
  JsonEachCursor *p = (JsonEachCursor*)cur;
  if( p->bRecursive ){
    if( p->sParse.aNode[p->i].jnFlags & JNODE_LABEL ) p->i++;
    p->i++;
    p->iRowid++;
    if( p->i<p->iEnd ){
      u32 iUp = p->sParse.aUp[p->i];
      JsonNode *pUp = &p->sParse.aNode[iUp];
      p->eType = pUp->eType;
      if( pUp->eType==JSON_ARRAY ){
        if( iUp==p->i-1 ){
          pUp->u.iKey = 0;
        }else{
          pUp->u.iKey++;
        }
      }
    }
  }else{
    switch( p->eType ){
      case JSON_ARRAY: {
        p->i += jsonNodeSize(&p->sParse.aNode[p->i]);
        p->iRowid++;
        break;
      }
      case JSON_OBJECT: {
        p->i += 1 + jsonNodeSize(&p->sParse.aNode[p->i+1]);
        p->iRowid++;
        break;
      }
      default: {
        p->i = p->iEnd;
        break;
      }
    }
  }
  return SQLITE_OK;
}

/* Append the name of the path for element i to pStr
*/
static void jsonEachComputePath(
  JsonEachCursor *p,       /* The cursor */
  JsonString *pStr,        /* Write the path here */
  u32 i                    /* Path to this element */
){
  JsonNode *pNode, *pUp;
  u32 iUp;
  if( i==0 ){
    jsonAppendChar(pStr, '$');
    return;
  }
  iUp = p->sParse.aUp[i];
  jsonEachComputePath(p, pStr, iUp);
  pNode = &p->sParse.aNode[i];
  pUp = &p->sParse.aNode[iUp];
  if( pUp->eType==JSON_ARRAY ){
    jsonPrintf(30, pStr, "[%d]", pUp->u.iKey);
  }else{
    assert( pUp->eType==JSON_OBJECT );
    if( (pNode->jnFlags & JNODE_LABEL)==0 ) pNode--;
    assert( pNode->eType==JSON_STRING );
    assert( pNode->jnFlags & JNODE_LABEL );
    jsonPrintf(pNode->n+1, pStr, ".%.*s", pNode->n-2, pNode->u.zJContent+1);
  }
}

/* Return the value of a column */
static int jsonEachColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  JsonEachCursor *p = (JsonEachCursor*)cur;
  JsonNode *pThis = &p->sParse.aNode[p->i];
  switch( i ){
    case JEACH_KEY: {
      if( p->i==0 ) break;
      if( p->eType==JSON_OBJECT ){
        jsonReturn(pThis, ctx, 0);
      }else if( p->eType==JSON_ARRAY ){
        u32 iKey;
        if( p->bRecursive ){
          if( p->iRowid==0 ) break;
          iKey = p->sParse.aNode[p->sParse.aUp[p->i]].u.iKey;
        }else{
          iKey = p->iRowid;
        }
        sqlite3_result_int64(ctx, (sqlite3_int64)iKey);
      }
      break;
    }
    case JEACH_VALUE: {
      if( pThis->jnFlags & JNODE_LABEL ) pThis++;
      jsonReturn(pThis, ctx, 0);
      break;
    }
    case JEACH_TYPE: {
      if( pThis->jnFlags & JNODE_LABEL ) pThis++;
      sqlite3_result_text(ctx, jsonType[pThis->eType], -1, SQLITE_STATIC);
      break;
    }
    case JEACH_ATOM: {
      if( pThis->jnFlags & JNODE_LABEL ) pThis++;
      if( pThis->eType>=JSON_ARRAY ) break;
      jsonReturn(pThis, ctx, 0);
      break;
    }
    case JEACH_ID: {
      sqlite3_result_int64(ctx,
         (sqlite3_int64)p->i + ((pThis->jnFlags & JNODE_LABEL)!=0));
      break;
    }
    case JEACH_PARENT: {
      if( p->i>p->iBegin && p->bRecursive ){
        sqlite3_result_int64(ctx, (sqlite3_int64)p->sParse.aUp[p->i]);
      }
      break;
    }
    case JEACH_FULLKEY: {
      JsonString x;
      jsonInit(&x, ctx);
      if( p->bRecursive ){
        jsonEachComputePath(p, &x, p->i);
      }else{
        if( p->zRoot ){
          jsonAppendRaw(&x, p->zRoot, (int)strlen(p->zRoot));
        }else{
          jsonAppendChar(&x, '$');
        }
        if( p->eType==JSON_ARRAY ){
          jsonPrintf(30, &x, "[%d]", p->iRowid);
        }else if( p->eType==JSON_OBJECT ){
          jsonPrintf(pThis->n, &x, ".%.*s", pThis->n-2, pThis->u.zJContent+1);
        }
      }
      jsonResult(&x);
      break;
    }
    case JEACH_PATH: {
      if( p->bRecursive ){
        JsonString x;
        jsonInit(&x, ctx);
        jsonEachComputePath(p, &x, p->sParse.aUp[p->i]);
        jsonResult(&x);
        break;
      }
      /* For json_each() path and root are the same so fall through
      ** into the root case */
      /* no break */ deliberate_fall_through
    }
    default: {
      const char *zRoot = p->zRoot;
      if( zRoot==0 ) zRoot = "$";
      sqlite3_result_text(ctx, zRoot, -1, SQLITE_STATIC);
      break;
    }
    case JEACH_JSON: {
      assert( i==JEACH_JSON );
      sqlite3_result_text(ctx, p->sParse.zJson, -1, SQLITE_STATIC);
      break;
    }
  }
  return SQLITE_OK;
}

/* Return the current rowid value */
static int jsonEachRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  JsonEachCursor *p = (JsonEachCursor*)cur;
  *pRowid = p->iRowid;
  return SQLITE_OK;
}

/* The query strategy is to look for an equality constraint on the json
** column.  Without such a constraint, the table cannot operate.  idxNum is
** 1 if the constraint is found, 3 if the constraint and zRoot are found,
** and 0 otherwise.
*/
static int jsonEachBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
  int i;                     /* Loop counter or computed array index */
  int aIdx[2];               /* Index of constraints for JSON and ROOT */
  int unusableMask = 0;      /* Mask of unusable JSON and ROOT constraints */
  int idxMask = 0;           /* Mask of usable == constraints JSON and ROOT */
  const struct sqlite3_index_constraint *pConstraint;

  /* This implementation assumes that JSON and ROOT are the last two
  ** columns in the table */
  assert( JEACH_ROOT == JEACH_JSON+1 );
  UNUSED_PARAM(tab);
  aIdx[0] = aIdx[1] = -1;
  pConstraint = pIdxInfo->aConstraint;
  for(i=0; i<pIdxInfo->nConstraint; i++, pConstraint++){
    int iCol;
    int iMask;
    if( pConstraint->iColumn < JEACH_JSON ) continue;
    iCol = pConstraint->iColumn - JEACH_JSON;
    assert( iCol==0 || iCol==1 );
    iMask = 1 << iCol;
    if( pConstraint->usable==0 ){
      unusableMask |= iMask;
    }else if( pConstraint->op==SQLITE_INDEX_CONSTRAINT_EQ ){
      aIdx[iCol] = i;
      idxMask |= iMask;
    }
  }
  if( (unusableMask & ~idxMask)!=0 ){
    /* If there are any unusable constraints on JSON or ROOT, then reject
    ** this entire plan */
    return SQLITE_CONSTRAINT;
  }
  if( aIdx[0]<0 ){
    /* No JSON input.  Leave estimatedCost at the huge value that it was
    ** initialized to to discourage the query planner from selecting this
    ** plan. */
    pIdxInfo->idxNum = 0;
  }else{
    pIdxInfo->estimatedCost = 1.0;
    i = aIdx[0];
    pIdxInfo->aConstraintUsage[i].argvIndex = 1;
    pIdxInfo->aConstraintUsage[i].omit = 1;
    if( aIdx[1]<0 ){
      pIdxInfo->idxNum = 1;  /* Only JSON supplied.  Plan 1 */
    }else{
      i = aIdx[1];
      pIdxInfo->aConstraintUsage[i].argvIndex = 2;
      pIdxInfo->aConstraintUsage[i].omit = 1;
      pIdxInfo->idxNum = 3;  /* Both JSON and ROOT are supplied.  Plan 3 */
    }
  }
  return SQLITE_OK;
}

/* Start a search on a new JSON string */
static int jsonEachFilter(
  sqlite3_vtab_cursor *cur,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  JsonEachCursor *p = (JsonEachCursor*)cur;
  const char *z;
  const char *zRoot = 0;
  sqlite3_int64 n;

  UNUSED_PARAM(idxStr);
  UNUSED_PARAM(argc);
  jsonEachCursorReset(p);
  if( idxNum==0 ) return SQLITE_OK;
  z = (const char*)sqlite3_value_text(argv[0]);
  if( z==0 ) return SQLITE_OK;
  n = sqlite3_value_bytes(argv[0]);
  p->zJson = sqlite3_malloc64( n+1 );
  if( p->zJson==0 ) return SQLITE_NOMEM;
  memcpy(p->zJson, z, (size_t)n+1);
  if( jsonParse(&p->sParse, 0, p->zJson) ){
    int rc = SQLITE_NOMEM;
    if( p->sParse.oom==0 ){
      sqlite3_free(cur->pVtab->zErrMsg);
      cur->pVtab->zErrMsg = sqlite3_mprintf("malformed JSON");
      if( cur->pVtab->zErrMsg ) rc = SQLITE_ERROR;
    }
    jsonEachCursorReset(p);
    return rc;
  }else if( p->bRecursive && jsonParseFindParents(&p->sParse) ){
    jsonEachCursorReset(p);
    return SQLITE_NOMEM;
  }else{
    JsonNode *pNode = 0;
    if( idxNum==3 ){
      const char *zErr = 0;
      zRoot = (const char*)sqlite3_value_text(argv[1]);
      if( zRoot==0 ) return SQLITE_OK;
      n = sqlite3_value_bytes(argv[1]);
      p->zRoot = sqlite3_malloc64( n+1 );
      if( p->zRoot==0 ) return SQLITE_NOMEM;
      memcpy(p->zRoot, zRoot, (size_t)n+1);
      if( zRoot[0]!='$' ){
        zErr = zRoot;
      }else{
        pNode = jsonLookupStep(&p->sParse, 0, p->zRoot+1, 0, &zErr);
      }
      if( zErr ){
        sqlite3_free(cur->pVtab->zErrMsg);
        cur->pVtab->zErrMsg = jsonPathSyntaxError(zErr);
        jsonEachCursorReset(p);
        return cur->pVtab->zErrMsg ? SQLITE_ERROR : SQLITE_NOMEM;
      }else if( pNode==0 ){
        return SQLITE_OK;
      }
    }else{
      pNode = p->sParse.aNode;
    }
    p->iBegin = p->i = (int)(pNode - p->sParse.aNode);
    p->eType = pNode->eType;
    if( p->eType>=JSON_ARRAY ){
      pNode->u.iKey = 0;
      p->iEnd = p->i + pNode->n + 1;
      if( p->bRecursive ){
        p->eType = p->sParse.aNode[p->sParse.aUp[p->i]].eType;
        if( p->i>0 && (p->sParse.aNode[p->i-1].jnFlags & JNODE_LABEL)!=0 ){
          p->i--;
        }
      }else{
        p->i++;
      }
    }else{
      p->iEnd = p->i+1;
    }
  }
  return SQLITE_OK;
}

/* The methods of the json_each virtual table */
static sqlite3_module jsonEachModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  jsonEachConnect,           /* xConnect */
  jsonEachBestIndex,         /* xBestIndex */
  jsonEachDisconnect,        /* xDisconnect */
  0,                         /* xDestroy */
  jsonEachOpenEach,          /* xOpen - open a cursor */
  jsonEachClose,             /* xClose - close a cursor */
  jsonEachFilter,            /* xFilter - configure scan constraints */
  jsonEachNext,              /* xNext - advance a cursor */
  jsonEachEof,               /* xEof - check for end of scan */
  jsonEachColumn,            /* xColumn - read data */
  jsonEachRowid,             /* xRowid - read data */
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

/* The methods of the json_tree virtual table. */
static sqlite3_module jsonTreeModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  jsonEachConnect,           /* xConnect */
  jsonEachBestIndex,         /* xBestIndex */
  jsonEachDisconnect,        /* xDisconnect */
  0,                         /* xDestroy */
  jsonEachOpenTree,          /* xOpen - open a cursor */
  jsonEachClose,             /* xClose - close a cursor */
  jsonEachFilter,            /* xFilter - configure scan constraints */
  jsonEachNext,              /* xNext - advance a cursor */
  jsonEachEof,               /* xEof - check for end of scan */
  jsonEachColumn,            /* xColumn - read data */
  jsonEachRowid,             /* xRowid - read data */
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

/****************************************************************************
** The following routines are the only publically visible identifiers in this
** file.  Call the following routines in order to register the various SQL
** functions and the virtual table implemented by this file.
****************************************************************************/

int sqlite3Json1Init(sqlite3 *db){
  int rc = SQLITE_OK;
  unsigned int i;
  static const struct {
     const char *zName;
     int nArg;
     int flag;
     void (*xFunc)(sqlite3_context*,int,sqlite3_value**);
  } aFunc[] = {
    { "json",                 1, 0,   jsonRemoveFunc        },
    { "json_array",          -1, 0,   jsonArrayFunc         },
    { "json_array_length",    1, 0,   jsonArrayLengthFunc   },
    { "json_array_length",    2, 0,   jsonArrayLengthFunc   },
    { "json_extract",        -1, 0,   jsonExtractFunc       },
    { "json_insert",         -1, 0,   jsonSetFunc           },
    { "json_object",         -1, 0,   jsonObjectFunc        },
    { "json_patch",           2, 0,   jsonPatchFunc         },
    { "json_quote",           1, 0,   jsonQuoteFunc         },
    { "json_remove",         -1, 0,   jsonRemoveFunc        },
    { "json_replace",        -1, 0,   jsonReplaceFunc       },
    { "json_set",            -1, 1,   jsonSetFunc           },
    { "json_type",            1, 0,   jsonTypeFunc          },
    { "json_type",            2, 0,   jsonTypeFunc          },
    { "json_valid",           1, 0,   jsonValidFunc         },

#if SQLITE_DEBUG
    /* DEBUG and TESTING functions */
    { "json_parse",           1, 0,   jsonParseFunc         },
    { "json_test1",           1, 0,   jsonTest1Func         },
#endif
  };
  static const struct {
     const char *zName;
     int nArg;
     void (*xStep)(sqlite3_context*,int,sqlite3_value**);
     void (*xFinal)(sqlite3_context*);
     void (*xValue)(sqlite3_context*);
  } aAgg[] = {
    { "json_group_array",     1,
      jsonArrayStep,   jsonArrayFinal,  jsonArrayValue  },
    { "json_group_object",    2,
      jsonObjectStep,  jsonObjectFinal, jsonObjectValue },
  };
#ifndef SQLITE_OMIT_VIRTUALTABLE
  static const struct {
     const char *zName;
     sqlite3_module *pModule;
  } aMod[] = {
    { "json_each",            &jsonEachModule               },
    { "json_tree",            &jsonTreeModule               },
  };
#endif
  static const int enc =
       SQLITE_UTF8 |
       SQLITE_DETERMINISTIC |
       SQLITE_INNOCUOUS;
  for(i=0; i<sizeof(aFunc)/sizeof(aFunc[0]) && rc==SQLITE_OK; i++){
    rc = sqlite3_create_function(db, aFunc[i].zName, aFunc[i].nArg, enc,
                                 (void*)&aFunc[i].flag,
                                 aFunc[i].xFunc, 0, 0);
  }
#ifndef SQLITE_OMIT_WINDOWFUNC
  for(i=0; i<sizeof(aAgg)/sizeof(aAgg[0]) && rc==SQLITE_OK; i++){
    rc = sqlite3_create_window_function(db, aAgg[i].zName, aAgg[i].nArg,
                                 SQLITE_SUBTYPE | enc, 0,
                                 aAgg[i].xStep, aAgg[i].xFinal,
                                 aAgg[i].xValue, jsonGroupInverse, 0);
  }
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
  for(i=0; i<sizeof(aMod)/sizeof(aMod[0]) && rc==SQLITE_OK; i++){
    rc = sqlite3_create_module(db, aMod[i].zName, aMod[i].pModule, 0);
  }
#endif
  return rc;
}


#ifndef SQLITE_CORE
#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_json_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  // SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  return sqlite3Json1Init(db);
}
#endif
#endif /* !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_JSON1) */


/*
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/noop.c
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
  assert( argc==1 );
  sqlite3_result_value(context, argv[0]);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_noop_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  // SQLITE_EXTENSION_INIT2(pApi);
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
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/regexp.c
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
          /* fall-through */
        }
// hack-sqlite
        // fall through
        case RE_OP_CC_INC: {
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
__declspec(dllexport)
#endif
int sqlite3_regexp_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  // SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused */
  rc = sqlite3_create_function(db, "regexp", 2, SQLITE_UTF8|SQLITE_INNOCUOUS,
                               0, re_sql_func, 0, 0);
  if( rc==SQLITE_OK ){
    /* The regexpi(PATTERN,STRING) function is a case-insensitive version
    ** of regexp(PATTERN,STRING). */
    rc = sqlite3_create_function(db, "regexpi", 2, SQLITE_UTF8|SQLITE_INNOCUOUS,
                                 (void*)db, re_sql_func, 0, 0);
  }
  return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.36.0/ext/misc/templatevtab.c
*/
/*
** 2018-04-19
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
** This file implements a template virtual-table.
** Developers can make a copy of this file as a baseline for writing
** new virtual tables and/or table-valued functions.
**
** Steps for writing a new virtual table implementation:
**
**     (1)  Make a copy of this file.  Perhaps call it "mynewvtab.c"
**
**     (2)  Replace this header comment with something appropriate for
**          the new virtual table
**
**     (3)  Change every occurrence of "templatevtab" to some other string
**          appropriate for the new virtual table.  Ideally, the new string
**          should be the basename of the source file: "mynewvtab".  Also
**          globally change "TEMPLATEVTAB" to "MYNEWVTAB".
**
**     (4)  Run a test compilation to make sure the unmodified virtual
**          table works.
**
**     (5)  Begin making incremental changes, testing as you go, to evolve
**          the new virtual table to do what you want it to do.
**
** This template is minimal, in the sense that it uses only the required
** methods on the sqlite3_module object.  As a result, templatevtab is
** a read-only and eponymous-only table.  Those limitation can be removed
** by adding new methods.
**
** This template implements an eponymous-only virtual table with a rowid and
** two columns named "a" and "b".  The table as 10 rows with fixed integer
** values. Usage example:
**
**     SELECT rowid, a, b FROM templatevtab;
*/
#if !defined(SQLITEINT_H)
#include "sqlite3ext.h"
#endif
// SQLITE_EXTENSION_INIT1
#include <string.h>
#include <assert.h>

/* templatevtab_vtab is a subclass of sqlite3_vtab which is
** underlying representation of the virtual table
*/
typedef struct templatevtab_vtab templatevtab_vtab;
struct templatevtab_vtab {
  sqlite3_vtab base;  /* Base class - must be first */
  /* Add new fields here, as necessary */
};

/* templatevtab_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct templatevtab_cursor templatevtab_cursor;
struct templatevtab_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  /* Insert new fields here.  For this templatevtab we only keep track
  ** of the rowid */
  sqlite3_int64 iRowid;      /* The rowid */
};

/*
** The templatevtabConnect() method is invoked to create a new
** template virtual table.
**
** Think of this routine as the constructor for templatevtab_vtab objects.
**
** All this routine needs to do is:
**
**    (1) Allocate the templatevtab_vtab object and initialize all fields.
**
**    (2) Tell SQLite (via the sqlite3_declare_vtab() interface) what the
**        result set of queries against the virtual table will look like.
*/
static int templatevtabConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  templatevtab_vtab *pNew;
  int rc;

  rc = sqlite3_declare_vtab(db,
           "CREATE TABLE x(a,b)"
       );
  /* For convenience, define symbolic names for the index to each column. */
#define TEMPLATEVTAB_A  0
#define TEMPLATEVTAB_B  1
  if( rc==SQLITE_OK ){
    pNew = sqlite3_malloc( sizeof(*pNew) );
    *ppVtab = (sqlite3_vtab*)pNew;
    if( pNew==0 ) return SQLITE_NOMEM;
    memset(pNew, 0, sizeof(*pNew));
  }
  return rc;
}

/*
** This method is the destructor for templatevtab_vtab objects.
*/
static int templatevtabDisconnect(sqlite3_vtab *pVtab){
  templatevtab_vtab *p = (templatevtab_vtab*)pVtab;
  sqlite3_free(p);
  return SQLITE_OK;
}

/*
** Constructor for a new templatevtab_cursor object.
*/
static int templatevtabOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  templatevtab_cursor *pCur;
  pCur = sqlite3_malloc( sizeof(*pCur) );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(*pCur));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/*
** Destructor for a templatevtab_cursor.
*/
static int templatevtabClose(sqlite3_vtab_cursor *cur){
  templatevtab_cursor *pCur = (templatevtab_cursor*)cur;
  sqlite3_free(pCur);
  return SQLITE_OK;
}


/*
** Advance a templatevtab_cursor to its next row of output.
*/
static int templatevtabNext(sqlite3_vtab_cursor *cur){
  templatevtab_cursor *pCur = (templatevtab_cursor*)cur;
  pCur->iRowid++;
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the templatevtab_cursor
** is currently pointing.
*/
static int templatevtabColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  templatevtab_cursor *pCur = (templatevtab_cursor*)cur;
  switch( i ){
    case TEMPLATEVTAB_A:
      sqlite3_result_int(ctx, 1000 + pCur->iRowid);
      break;
    default:
      assert( i==TEMPLATEVTAB_B );
      sqlite3_result_int(ctx, 2000 + pCur->iRowid);
      break;
  }
  return SQLITE_OK;
}

/*
** Return the rowid for the current row.  In this implementation, the
** rowid is the same as the output value.
*/
static int templatevtabRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  templatevtab_cursor *pCur = (templatevtab_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
static int templatevtabEof(sqlite3_vtab_cursor *cur){
  templatevtab_cursor *pCur = (templatevtab_cursor*)cur;
  return pCur->iRowid>=10;
}

/*
** This method is called to "rewind" the templatevtab_cursor object back
** to the first row of output.  This method is always called at least
** once prior to any call to templatevtabColumn() or templatevtabRowid() or
** templatevtabEof().
*/
static int templatevtabFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  templatevtab_cursor *pCur = (templatevtab_cursor *)pVtabCursor;
  pCur->iRowid = 1;
  return SQLITE_OK;
}

/*
** SQLite will invoke this method one or more times while planning a query
** that uses the virtual table.  This routine needs to create
** a query plan for each invocation and compute an estimated cost for that
** plan.
*/
static int templatevtabBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
  pIdxInfo->estimatedCost = (double)10;
  pIdxInfo->estimatedRows = 10;
  return SQLITE_OK;
}

/*
** This following structure defines all the methods for the
** virtual table.
*/
static sqlite3_module templatevtabModule = {
  /* iVersion    */ 0,
  /* xCreate     */ 0,
  /* xConnect    */ templatevtabConnect,
  /* xBestIndex  */ templatevtabBestIndex,
  /* xDisconnect */ templatevtabDisconnect,
  /* xDestroy    */ 0,
  /* xOpen       */ templatevtabOpen,
  /* xClose      */ templatevtabClose,
  /* xFilter     */ templatevtabFilter,
  /* xNext       */ templatevtabNext,
  /* xEof        */ templatevtabEof,
  /* xColumn     */ templatevtabColumn,
  /* xRowid      */ templatevtabRowid,
  /* xUpdate     */ 0,
  /* xBegin      */ 0,
  /* xSync       */ 0,
  /* xCommit     */ 0,
  /* xRollback   */ 0,
  /* xFindMethod */ 0,
  /* xRename     */ 0,
  /* xSavepoint  */ 0,
  /* xRelease    */ 0,
  /* xRollbackTo */ 0,
  /* xShadowName */ 0
};


#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_templatevtab_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  // SQLITE_EXTENSION_INIT2(pApi);
  rc = sqlite3_create_module(db, "templatevtab", &templatevtabModule, 0);
  return rc;
}


/*
repo https://github.com/sqlite/sqlite/tree/version-3.36.0
committed
*/


/*
file https://github.com/sqlite/sqlite/tree/version-3.36.0/contrib/download/extension-functions.c/download/extension-functions.c
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
// SQLITE_EXTENSION_INIT1, // SQLITE_EXTENSION_INIT2, so that it works with
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
void map_insert(map *m, void *e);

/*
** executes function iter over all elements in the map, in key increasing order
*/
void map_iterate(map *m, map_iterator iter, void* p);

/*
** frees all memory used by a map
*/
void map_destroy(map *m);

/*
** compares 2 integers
** to use with map_make
*/
int int_cmp(const void *a, const void *b);

/*
** compares 2 doubles
** to use with map_make
*/
int double_cmp(const void *a, const void *b);

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
// hack-sqlite
/* LMH salvaged from sqlite3 3.3.13 source code src/utf.c */
  int xtra;
  c = *(z)++;
  xtra = xtra_utf8_bytes[c];
  switch( xtra ){
    case 4: c = (int)0xFFFD; break;
    case 3: c = (c<<6) + *(z)++;
    // fall through
    case 2: c = (c<<6) + *(z)++;
    // fall through
    case 1: c = (c<<6) + *(z)++;
    c -= xtra_utf8_bits[xtra];
    if( (utf_mask[xtra]&c)==0
        || (c&0xFFFFF800)==0xD800
        || (c&0xFFFFFFFE)==0xFFFE ){  c = 0xFFFD; }
  }
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
** This is a macro that facilitates writting wrappers for math.h functions
** it creates code for a function to use in SQlite that gets one numeric input
** and returns a floating point value.
**
** Could have been implemented using pointers to functions but this way it's inline
** and thus more efficient. Lower * ranking though...
**
** Parameters:
** name:      function name to de defined (eg: sinFunc)
** function:  function defined in math.h to wrap (eg: sin)
** domain:    boolean condition that CAN'T happen in terms of the input parameter rVal
**            (eg: rval<0 for sqrt)
*/
/* LMH 2007-03-25 Changed to use errno and remove domain; no pre-checking for errors. */
// hack-sqlite
#define GEN_MATH_WRAP_DOUBLE_1(name, function)
#define GEN_MATH_WRAP_DOUBLE_2(name, function) \
static void name(sqlite3_context *context, int argc, sqlite3_value **argv){\
  double rVal = 0.0, val;\
  assert( argc==1 );\
  switch( sqlite3_value_type(argv[0]) ){\
    case SQLITE_NULL: {\
      sqlite3_result_null(context);\
      break;\
    }\
    default: {\
      rVal = sqlite3_value_double(argv[0]);\
      errno = 0;\
      val = function(rVal);\
      if (errno == 0) {\
        sqlite3_result_double(context, val);\
      } else {\
        sqlite3_result_error(context, strerror(errno), errno);\
      }\
      break;\
    }\
  }\
}\


/*
** Example of GEN_MATH_WRAP_DOUBLE_1 usage
** this creates function sqrtFunc to wrap the math.h standard function sqrt(x)=x^0.5
*/
GEN_MATH_WRAP_DOUBLE_1(sqrtFunc, sqrt)

/* trignometric functions */
GEN_MATH_WRAP_DOUBLE_1(acosFunc, acos)
GEN_MATH_WRAP_DOUBLE_1(asinFunc, asin)
GEN_MATH_WRAP_DOUBLE_1(atanFunc, atan)

/*
** Many of systems don't have inverse hyperbolic trig functions so this will emulate
** them on those systems in terms of log and sqrt (formulas are too trivial to demand
** written proof here)
*/

#ifndef HAVE_ACOSH
static double acosh(double x){
  return log(x + sqrt(x*x - 1.0));
}
#endif

GEN_MATH_WRAP_DOUBLE_1(acoshFunc, acosh)

#ifndef HAVE_ASINH
static double asinh(double x){
  return log(x + sqrt(x*x + 1.0));
}
#endif

GEN_MATH_WRAP_DOUBLE_1(asinhFunc, asinh)

#ifndef HAVE_ATANH
static double atanh(double x){
  return (1.0/2.0)*log((1+x)/(1-x)) ;
}
#endif

GEN_MATH_WRAP_DOUBLE_1(atanhFunc, atanh)

/*
** math.h doesn't require cot (cotangent) so it's defined here
*/
static double cot(double x){
  return 1.0/tan(x);
}

GEN_MATH_WRAP_DOUBLE_1(sinFunc, sin)
GEN_MATH_WRAP_DOUBLE_1(cosFunc, cos)
GEN_MATH_WRAP_DOUBLE_1(tanFunc, tan)
// hack-sqlite
GEN_MATH_WRAP_DOUBLE_2(cotFunc, cot)

static double coth(double x){
  return 1.0/tanh(x);
}

/*
** Many systems don't have hyperbolic trigonometric functions so this will emulate
** them on those systems directly from the definition in terms of exp
*/
#ifndef HAVE_SINH
static double sinh(double x){
  return (exp(x)-exp(-x))/2.0;
}
#endif

GEN_MATH_WRAP_DOUBLE_1(sinhFunc, sinh)

#ifndef HAVE_COSH
static double cosh(double x){
  return (exp(x)+exp(-x))/2.0;
}
#endif

GEN_MATH_WRAP_DOUBLE_1(coshFunc, cosh)

#ifndef HAVE_TANH
static double tanh(double x){
  return sinh(x)/cosh(x);
}
#endif

GEN_MATH_WRAP_DOUBLE_1(tanhFunc, tanh)

// hack-sqlite
GEN_MATH_WRAP_DOUBLE_2(cothFunc, coth)

/*
** Some systems lack log in base 10. This will emulate it
*/

#ifndef HAVE_LOG10
static double log10(double x){
  static double l10 = -1.0;
  if( l10<0.0 ){
    l10 = log(10.0);
  }
  return log(x)/l10;
}
#endif

GEN_MATH_WRAP_DOUBLE_1(logFunc, log)
GEN_MATH_WRAP_DOUBLE_1(log10Func, log10)
GEN_MATH_WRAP_DOUBLE_1(expFunc, exp)

/*
** Fallback for systems where math.h doesn't define M_PI
*/
#undef M_PI
#ifndef M_PI
/*
** static double PI = acos(-1.0);
** #define M_PI (PI)
*/
#define M_PI 3.14159265358979323846
#endif

/* Convert Degrees into Radians */
// hack-sqlite
#if 0
static double deg2rad(double x){
  return x*M_PI/180.0;
}

/* Convert Radians into Degrees */
static double rad2deg(double x){
  return 180.0*x/M_PI;
}

GEN_MATH_WRAP_DOUBLE_1(rad2degFunc, rad2deg)
GEN_MATH_WRAP_DOUBLE_1(deg2radFunc, deg2rad)

/* constant function that returns the value of PI=3.1415... */
static void piFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  sqlite3_result_double(context, M_PI);
}

/*
** Implements the sqrt function, it has the peculiarity of returning an integer when the
** the argument is an integer.
** Since SQLite isn't strongly typed (almost untyped actually) this is a bit pedantic
*/
// hack-sqlite
#endif
static void squareFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  i64 iVal = 0;
  double rVal = 0.0;
  assert( argc==1 );
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_INTEGER: {
      iVal = sqlite3_value_int64(argv[0]);
      sqlite3_result_int64(context, iVal*iVal);
      break;
    }
    case SQLITE_NULL: {
      sqlite3_result_null(context);
      break;
    }
    default: {
      rVal = sqlite3_value_double(argv[0]);
      sqlite3_result_double(context, rVal*rVal);
      break;
    }
  }
}

/*
** Wraps the pow math.h function
** When both the base and the exponent are integers the result should be integer
** (see sqrt just before this). Here the result is always double
*/
/* LMH 2007-03-25 Changed to use errno; no pre-checking for errors.  Also removes
  but that was present in the pre-checking that called sqlite3_result_error on
  a non-positive first argument, which is not always an error. */
// hack-sqlite
#if 0
static void powerFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  double r1 = 0.0;
  double r2 = 0.0;
  double val;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0]) == SQLITE_NULL || sqlite3_value_type(argv[1]) == SQLITE_NULL ){
    sqlite3_result_null(context);
  }else{
    r1 = sqlite3_value_double(argv[0]);
    r2 = sqlite3_value_double(argv[1]);
    errno = 0;
    val = pow(r1,r2);
    if (errno == 0) {
      sqlite3_result_double(context, val);
    } else {
      sqlite3_result_error(context, strerror(errno), errno);
    }
  }
}

/*
** atan2 wrapper
*/
// hack-sqlite
#endif
static void atn2Func(sqlite3_context *context, int argc, sqlite3_value **argv){
  double r1 = 0.0;
  double r2 = 0.0;

  assert( argc==2 );

  if( sqlite3_value_type(argv[0]) == SQLITE_NULL || sqlite3_value_type(argv[1]) == SQLITE_NULL ){
    sqlite3_result_null(context);
  }else{
    r1 = sqlite3_value_double(argv[0]);
    r2 = sqlite3_value_double(argv[1]);
    sqlite3_result_double(context, atan2(r1,r2));
  }
}

/*
** Implementation of the sign() function
** return one of 3 possibilities +1,0 or -1 when the argument is respectively
** positive, 0 or negative.
** When the argument is NULL the result is also NULL (completly conventional)
*/
static void signFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  double rVal=0.0;
  i64 iVal=0;
  assert( argc==1 );
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_INTEGER: {
      iVal = sqlite3_value_int64(argv[0]);
      iVal = ( iVal > 0) ? 1: ( iVal < 0 ) ? -1: 0;
      sqlite3_result_int64(context, iVal);
      break;
    }
    case SQLITE_NULL: {
      sqlite3_result_null(context);
      break;
    }
    default: {
 /* 2nd change below. Line for abs was: if( rVal<0 ) rVal = rVal * -1.0;  */

      rVal = sqlite3_value_double(argv[0]);
      rVal = ( rVal > 0) ? 1: ( rVal < 0 ) ? -1: 0;
      sqlite3_result_double(context, rVal);
      break;
    }
  }
}


/*
** smallest integer value not less than argument
*/
// hack-sqlite
#if 0
static void ceilFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  double rVal=0.0;
  i64 iVal=0;
  assert( argc==1 );
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_INTEGER: {
      i64 iVal = sqlite3_value_int64(argv[0]);
      sqlite3_result_int64(context, iVal);
      break;
    }
    case SQLITE_NULL: {
      sqlite3_result_null(context);
      break;
    }
    default: {
      rVal = sqlite3_value_double(argv[0]);
      sqlite3_result_int64(context, (i64) ceil(rVal));
      break;
    }
  }
}

/*
** largest integer value not greater than argument
*/
static void floorFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  double rVal=0.0;
  i64 iVal=0;
  assert( argc==1 );
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_INTEGER: {
      i64 iVal = sqlite3_value_int64(argv[0]);
      sqlite3_result_int64(context, iVal);
      break;
    }
    case SQLITE_NULL: {
      sqlite3_result_null(context);
      break;
    }
    default: {
      rVal = sqlite3_value_double(argv[0]);
      sqlite3_result_int64(context, (i64) floor(rVal));
      break;
    }
  }
}

/*
** Given a string (s) in the first argument and an integer (n) in the second returns the
** string that constains s contatenated n times
*/
// hack-sqlite
#endif
static void replicateFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
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
int isblank(char c){
  return( ' '==c || '\t'==c );
}
#endif

static void properFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
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
  while( *s==' ' )
    ++s;
  return s;
}

/*
** removes the whitespaces at the end of a string.
** !mutates the input string!
*/
void rtrim(char* s){
  char* ss = s+strlen(s)-1;
  while( ss>=s && *ss==' ' )
    --ss;
  *(ss+1)='\0';
}

/*
**  Removes the whitespace at the begining of a string
*/
static void ltrimFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
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
int RegisterExtensionFunctions(sqlite3 *db){
  static const struct FuncDef {
     char *zName;
     signed char nArg;
     u8 argType;           /* 0: none.  1: db  2: (-1) */
     u8 eTextRep;          /* 1: UTF-16.  0: UTF-8 */
     u8 needCollSeq;
     void (*xFunc)(sqlite3_context*,int,sqlite3_value **);
  } aFuncs[] = {
    /* math.h */
// hack-sqlite
    { "atn2",               2, 0, SQLITE_UTF8,    0, atn2Func  },
    { "cot",                1, 0, SQLITE_UTF8,    0, cotFunc },
    { "coth",               1, 0, SQLITE_UTF8,    0, cothFunc },
    { "difference",         2, 0, SQLITE_UTF8,    0, differenceFunc},
    { "sign",               1, 0, SQLITE_UTF8,    0, signFunc },
    { "square",             1, 0, SQLITE_UTF8,    0, squareFunc },


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
// hack-sqlite
  unsigned int i;

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
// hack-sqlite
int sqlite3_sqlmath_init(sqlite3*, char**, const sqlite3_api_routines*);
int sqlite3_extension_functions_init(
    sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi){
  // SQLITE_EXTENSION_INIT2(pApi);
  sqlite3_api=pApi;
  sqlite3_auto_extension((void(*)(void))sqlite3_carray_init);
  sqlite3_auto_extension((void(*)(void))sqlite3_compress_init);
  sqlite3_auto_extension((void(*)(void))sqlite3_csv_init);
  sqlite3_auto_extension((void(*)(void))sqlite3_noop_init);
  sqlite3_auto_extension((void(*)(void))sqlite3_regexp_init);
  sqlite3_auto_extension((void(*)(void))sqlite3_sqlmath_init);
  sqlite3_auto_extension((void(*)(void))sqlite3_templatevtab_init);
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
  void* ret = calloc(nmemb, size);
  return ret;
}

void xfree(void* p){
  free(p);
}

void node_insert(node** n, cmp_func cmp, void *e){
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

void map_insert(map *m, void *e){
  node_insert(&(m->base), m->cmp, e);
}

void node_iterate(node *n, map_iterator iter, void* p){
  if(n){
    if(n->l)
      node_iterate(n->l, iter, p);
    iter(n->data, n->count, p);
    if(n->r)
      node_iterate(n->r, iter, p);
  }
}

void map_iterate(map *m, map_iterator iter, void* p){
  node_iterate(m->base, iter, p);
}

void node_destroy(node *n){
  if(0!=n){
    xfree(n->data);
    if(n->l)
      node_destroy(n->l);
    if(n->r)
      node_destroy(n->r);

    xfree(n);
  }
}

void map_destroy(map *m){
  node_destroy(m->base);
}

int int_cmp(const void *a, const void *b){
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

int double_cmp(const void *a, const void *b){
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

// hack-sqlite
#if 0
void print_elem(void *e, int64_t c, void* p){
  int ee = *(int*)(e);
  printf("%d => %lld\n", ee,c);
}
#endif


/*
file none
*/
/*jslint-enable*/
