// copyright nobody
// LINT_C_FILE


#ifdef SQLITE3_EXT_C2
/*
file sqlmath_ext - start
*/
#include <sqlite3ext.h>
#define UNUSED(x) (void)(x)
int sqlite3_sqlmath_ext_custom_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED(db);
    UNUSED(pzErrMsg);
    UNUSED(pApi);
    return 0;
}

/*
file sqlmath_ext - end
*/


#else
// *INDENT-OFF*
#if defined(__cplusplus)
extern "C" {
#endif
// *INDENT-ON*


// header
#include "sqlmath_base.c"


/*
file sqlmath_c - start
*/
#ifndef SQLMATH_NAPI


/*
file sqlmath_c_custom_init
*/
int sqlite3_sqlmath_ext_custom_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED(db);
    UNUSED(pApi);
    UNUSED(pzErrMsg);
    return 0;
}

/*
file sqlmath_c - end
*/


/*
file sqlmath_napi - start
*/
#else                           // SQLMATH_NAPI


/*
file sqlmath_napi_custom_init
*/
napi_value napi_module_custom_init(
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
    napi_module_init(env, exports);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_custom_init)
#endif                          // SQLMATH_NAPI
/*
file sqlmath_napi - end
*/
// *INDENT-OFF*
#if defined(__cplusplus)
}
#endif
// *INDENT-ON*
#endif                          // SQLITE3_EXT_C2
