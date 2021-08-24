// LINT_C_FILE
// copyright nobody
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <sqlite3.h>
#include <node_api.h>
#define ASSERT_NAPI_OK_OR_RETURN(env, errcode) \
    if (!assertNapiOk(env, errcode)) {return NULL;}
#define ASSERT_NOT_NULL_OR_RETURN(env, ptr) \
    if (!assertNotNull(env, ptr)) {return NULL;}
#define ASSERT_OR_RETURN(env, cond, errmsg) \
    if (!assertOrThrow(env, cond, errmsg)) {return NULL;}
#define ASSERT_SQLITE_OK_OR_RETURN(env, db, errcode) \
    if (!assertSqliteOk(env, db, errcode)) {return NULL;}
#define JSBATON_CREATE_OR_RETURN(env, info) \
    jsbatonCreate(env, info); if (baton == NULL) {return NULL;}
#define SQLITE_MAX_LENGTH 1000000000
#define UNUSED(x) (void)(x)
/* *INDENT-OFF* */
void jssqlExec(sqlite3 *, const char *, char **, int *, const char **);
/* *INDENT-ON* */
typedef struct Jsbaton {
    // ctx
    napi_env napiEnv;
    sqlite3 *sqlDb;
    // data
    char *buf8[8];
    double num8[8];
    // err
    const char *errMsg;
} Jsbaton;

bool assertNapiOk(
    napi_env env,
    napi_status errcode
) {
// this function will throw error if <errcode> != napi_ok
// derived from https://github.com/nodejs/node-addon-api/blob/3.2.1/napi-inl.h
// typedef struct {
//   const char* error_message;
//   void* engine_reserved;
//   uint32_t engine_error_code;
//   napi_status error_code;
// } napi_extended_error_info;
#define ASSERT_NAPI_OK_OR_FATAL(msg) \
    if (errcode != napi_ok) { \
        napi_fatal_error("assertNapiOk", NAPI_AUTO_LENGTH, msg, \
            NAPI_AUTO_LENGTH); \
    }
    if (errcode == napi_ok) {
        return true;
    }
    // declare var
    bool is_exception_pending;
    const napi_extended_error_info *info;
    napi_value error = NULL;
    // We must retrieve the last error info before doing anything else, because
    // doing anything else will replace the last error info.
    errcode = napi_get_last_error_info(env, &info);
    ASSERT_NAPI_OK_OR_FATAL("napi_get_last_error_info");
    errcode = napi_is_exception_pending(env, &is_exception_pending);
    ASSERT_NAPI_OK_OR_FATAL("napi_is_exception_pending");
    // A pending exception takes precedence over any internal error status.
    if (is_exception_pending) {
        errcode = napi_get_and_clear_last_exception(env, &error);
        ASSERT_NAPI_OK_OR_FATAL("napi_get_and_clear_last_exception");
        napi_throw(env, error);
        return false;
    }
    const char *error_message =
        (info->error_message !=
        NULL ? info->error_message : "Error in native callback");
    napi_value message;
    errcode =
        napi_create_string_utf8(env, error_message, strlen(error_message),
        &message);
    ASSERT_NAPI_OK_OR_FATAL("napi_create_string_utf8");
    switch (info->error_code) {
    case napi_object_expected:
    case napi_string_expected:
    case napi_boolean_expected:
    case napi_number_expected:
        errcode = napi_create_type_error(env, NULL, message, &error);
        ASSERT_NAPI_OK_OR_FATAL("napi_create_type_error");
        break;
    default:
        errcode = napi_create_error(env, NULL, message, &error);
        ASSERT_NAPI_OK_OR_FATAL("napi_create_error");
        break;
    }
    napi_throw(env, error);
    return false;
}

bool assertOrThrow(
    napi_env env,
    bool cond,
    const char *errmsg
) {
// this function will throw <errmsg> if <cond> is falsy
    if (!cond) {
        napi_throw_error(env, NULL, errmsg);
        return false;
    }
    return true;
}

bool assertSqliteOk(
    napi_env env,
    sqlite3 * db,
    int errcode
) {
// this function will throw error if <errcode> != SQLITE_OK
    if (errcode != SQLITE_OK) {
        napi_throw_error(env, NULL,
            (db == NULL ? sqlite3_errstr(errcode) : sqlite3_errmsg(db)));
        return false;
    }
    return true;
}

bool assertNotNull(
    napi_env env,
    void *ptr
) {
// this function will throw error if <ptr> == NULL
    return assertOrThrow(env, ptr != NULL, "null pointer");
}

napi_value jsbatonInfo(
    napi_env env,
    napi_callback_info info
) {
// this function will return info about data-structure of Jsbaton
#define JSBATON_INFO_MEMBER_OFFSET(name, elemsize) \
    size = sizeof(baton.name); \
    wordsize = (elemsize == 0 ? size : elemsize); \
    offset += (wordsize - (offset % wordsize)) % wordsize; \
    pp += snprintf( \
        pp, \
        sizeof(json), \
        "\"" #name "\":[%zu,%zu],", \
        offset, size); \
    offset += size;
    // declare var
    Jsbaton baton = { 0 };
    char *pp;
    char json[512];
    int errcode;
    napi_value result;
    size_t offset = 0;
    size_t size;
    size_t wordsize;
    // init pp
    pp = json;
    pp += snprintf(pp, sizeof(json), "{");
    // ctx
    JSBATON_INFO_MEMBER_OFFSET(napiEnv, 0);
    JSBATON_INFO_MEMBER_OFFSET(sqlDb, 0);
    // data
    JSBATON_INFO_MEMBER_OFFSET(buf8, sizeof(void *));
    JSBATON_INFO_MEMBER_OFFSET(num8, 8);
    // err
    JSBATON_INFO_MEMBER_OFFSET(errMsg, 0);
    pp +=
        snprintf(pp, sizeof(json), "\"sizeof\":[%zu,%zu]}", offset,
        sizeof(baton));
    // printf("\n\n[json=%s]\n\n", json);
    errcode = napi_create_string_utf8(env, json, NAPI_AUTO_LENGTH, &result);
    ASSERT_NAPI_OK_OR_RETURN(env, errcode);
    return result;
}

Jsbaton *jsbatonCreate(
    napi_env env,
    napi_callback_info info
) {
// this function will create a baton for passing data between nodejs <-> c
    // declare var
    Jsbaton *baton;
    int errcode;
    napi_value argv[1 + 8];
    size_t argc;
    size_t ii;
    size_t length;
    void *zTmp;
    // init argc
    errcode = napi_get_cb_info(env, info, &argc, NULL, NULL, NULL);
    ASSERT_NAPI_OK_OR_RETURN(env, errcode);
    // init argv
    errcode = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    ASSERT_NAPI_OK_OR_RETURN(env, errcode);
    // init baton
    errcode = napi_get_buffer_info(env, argv[0], &zTmp, &length);
    ASSERT_NAPI_OK_OR_RETURN(env, errcode);
    baton = (Jsbaton *) zTmp;
    baton->napiEnv = env;
    // init buf8
    ii = 1;
    while (ii < argc) {
        errcode = napi_get_buffer_info(env, argv[ii], &zTmp, &length);
        ASSERT_NAPI_OK_OR_RETURN(env, errcode);
        baton->buf8[ii - 1] = (char *) zTmp;
        ii += 1;
    }
    return baton;
}

void napiFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED(env);
    UNUSED(finalize_hint);
    free(finalize_data);
}

napi_value napiNoop(
    napi_env env,
    napi_callback_info info
) {
// this function will do nothing
    Jsbaton *baton = JSBATON_CREATE_OR_RETURN(env, info);
    return NULL;
}

napi_value _sqlite3_close(
    napi_env env,
    napi_callback_info info
) {
// int sqlite3_close(sqlite3*);
    Jsbaton *baton = JSBATON_CREATE_OR_RETURN(env, info);
    sqlite3 *db = baton->sqlDb;
    int errcode = sqlite3_close_v2(db);
    ASSERT_SQLITE_OK_OR_RETURN(env, db, errcode);
    return NULL;
}

napi_value _sqlite3_open_v2(
    napi_env env,
    napi_callback_info info
) {
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
    Jsbaton *baton = JSBATON_CREATE_OR_RETURN(env, info);
    int errcode = 0;
    int flags = (int) baton->num8[2];
    sqlite3 *db = NULL;
    const char *filename = (const char *) baton->buf8[0];
    ASSERT_NOT_NULL_OR_RETURN(env, (void *) filename);
    errcode = sqlite3_open_v2(filename, &db, flags, NULL);
    ASSERT_SQLITE_OK_OR_RETURN(env, db, errcode);
    baton->sqlDb = db;
    return NULL;
}

napi_value _jssqlExec(
    napi_env env,
    napi_callback_info info
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized a json-string in
// <pResult>.
    Jsbaton *baton = JSBATON_CREATE_OR_RETURN(env, info);
    // declare var
    char *zBuf = NULL;
    char *zErrmsg = NULL;
    const char *zSql = baton->buf8[0];
    ASSERT_NOT_NULL_OR_RETURN(env, (void *) zSql);
    int errcode = napi_ok;
    int nUsed = 0;
    napi_value result = NULL;
    sqlite3 *db = baton->sqlDb;
    zSql = (const char *) baton->buf8[0];
    // exec sql
    jssqlExec(db, zSql, &zBuf, &nUsed, (const char **) &zErrmsg);
    ASSERT_OR_RETURN(env, zErrmsg == NULL, zErrmsg);
    // init result
    errcode =
        napi_create_external_arraybuffer(env, (void *) zBuf, (size_t) nUsed,
        napiFinalize, NULL, &result);
    ASSERT_NAPI_OK_OR_RETURN(env, errcode);
    return result;
}

//!! static void test(
//!! ) {
//!! // this function will run tests
//!! }

napi_value napi_module_init(
    napi_env env,
    napi_value exports
) {
#define NAPI_EXPORT_MEMBER(name) {#name, 0, name, 0, 0, 0, napi_default, 0}
    int errcode;
    const napi_property_descriptor propList[] = {
        NAPI_EXPORT_MEMBER(_jssqlExec),
        NAPI_EXPORT_MEMBER(_sqlite3_close),
        NAPI_EXPORT_MEMBER(_sqlite3_open_v2),
        NAPI_EXPORT_MEMBER(jsbatonInfo),
        NAPI_EXPORT_MEMBER(napiNoop),
    };
    errcode = napi_define_properties(env, exports,
        sizeof(propList) / sizeof(napi_property_descriptor), propList);
    ASSERT_NAPI_OK_OR_RETURN(env, errcode);
//!! #ifdef DEBUG
    //!! test();
//!! #endif
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_init)
