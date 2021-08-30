// copyright nobody
// LINT_C_FILE


/*
file sqlmath_napi.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <sqlite3.h>
#include <node_api.h>
#define ALLOCF free
#define SIZEOF_BUFFER_DEFAULT 1024
#define UNUSED(x) (void)(x)


/*
file sqlmath_assert.c
*/
// printf("\n\n[napi errcode=%d]\n\n", errcode);

// this function will if <cond> is falsy, set <baton->errmsg> to <msg>
#define ASSERT_BATON(baton, cond, msg) \
    if (baton->errmsg[0] != 0) { \
        return 0; \
    } \
    if (!(cond)) { \
        __snprintfTrace(baton->errmsg, msg, __func__, __FILE__, __LINE__); \
        return 0; \
    }

// this function will if <cond> is falsy, terminate process with <msg>
#define ASSERT_FATAL(cond, msg) \
    if (!(cond)) { \
        napi_fatal_error(__func__, NAPI_AUTO_LENGTH , msg, NAPI_AUTO_LENGTH); \
    }

// this function will if <cond> is falsy, throw <msg> in <env>
#define ASSERT_NAPI(env, cond, msg) \
    if (!(cond)) { \
        char buf[256] = { 0 }; \
        napi_throw_error(env, NULL, \
            __snprintfTrace(buf, msg, __func__, __FILE__, __LINE__)); \
        return 0; \
    }

// this function will assert <errcode> == napi_ok in <env>
#define ASSERT_NAPI_OK(env, errcode) \
    if (0 != assertNapiOk(env, __func__, __FILE__, __LINE__, errcode)) { \
        return 0; \
    }

// this function will assert <errcode> == SQLITE_OK in <db>
#define ASSERT_SQLITE_OK(baton, db, errcode) \
    if (errcode != SQLITE_OK) { \
        ASSERT_BATON( \
            baton, \
            false, \
            (db == NULL ? sqlite3_errstr(errcode) : sqlite3_errmsg(db))); \
    }

static const char *__snprintfTrace(
    char *buf,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
) {
// this function will write <errmsg> to <buf> with additional trace-info
    if (snprintf(buf,           // NO_LINT
            256, "%s\n    at %s (%s:%d)", errmsg, func, file, line) < 0) {
        abort();
    }
    return (const char *) buf;
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
    char buf[256] = { 0 };
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
    ASSERT_FATAL(errcode == 0, "napi_get_last_error_info");
    errcode = napi_is_exception_pending(env, &is_exception_pending);
    ASSERT_FATAL(errcode == 0, "napi_is_exception_pending");
    // A pending exception takes precedence over any internal error status.
    if (is_exception_pending) {
        errcode = napi_get_and_clear_last_exception(env, &val);
        ASSERT_FATAL(errcode == 0, "napi_get_and_clear_last_exception");
        napi_throw(env, val);
        return errcode;
    }
    errcode =
        napi_throw_error(env, NULL, __snprintfTrace(buf,
            (info->error_message !=
                NULL ? info->error_message : "error in native code"), func,
            file, line));
    ASSERT_FATAL(errcode == 0, "napi_throw_error");
    return errcode;
}


/*
file sqlmath_promise.c
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

typedef struct Jsbaton {
    // data
    int64_t int8[8];
    char *buf8[8];
    char *out8[8];
    char errmsg[256];
    napi_value result;
    // async
    napi_async_work work;
    napi_deferred deferred;
} Jsbaton;

static void jsbatonBufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED(env);
    UNUSED(finalize_hint);
    // printf("\n\n[napi finalize_data=%s]\n\n", (const char *) finalize_data);
    // printf("\n\n[napi finalize_hint=%s]\n\n", (const char *) finalize_hint);
    ALLOCF(finalize_data);
}

static Jsbaton *jsbatonCreate(
    napi_env env,
    napi_callback_info info
) {
// this function will create a baton for passing data between nodejs <-> c
    // declare var
    Jsbaton *baton = NULL;
    bool istrue;
    int errcode = 0;
    napi_value tmp;
    size_t ii = 1;
    // init argv
    errcode = napi_get_cb_info(env, info, &ii, &tmp, NULL, NULL);
    ASSERT_NAPI_OK(env, errcode);
    // init baton
    errcode = napi_get_element(env, tmp, 0, (napi_value *) & baton);
    ASSERT_NAPI_OK(env, errcode);
    errcode =
        napi_get_typedarray_info(env, (napi_value) baton, NULL, NULL,
        (void **) &baton, NULL, NULL);
    ASSERT_NAPI_OK(env, errcode);
    baton->result = tmp;
    // init buf8
    ii = 0;
    while (ii < 8) {
        errcode = napi_get_element(env, baton->result, ii + 1, &tmp);
        ASSERT_NAPI_OK(env, errcode);
        errcode = napi_is_buffer(env, tmp, &istrue);
        ASSERT_NAPI_OK(env, errcode);
        if (istrue) {
            errcode =
                napi_get_buffer_info(env, tmp,
                (void **) &baton->buf8[ii], NULL);
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
    while (ii < 8) {
        if (baton->out8[ii] == NULL) {
            errcode = napi_create_double(env, (double) baton->int8[ii], &val);
            ASSERT_NAPI_OK(env, errcode);
        } else {
            errcode = napi_create_external_arraybuffer(env,     // napi_env env,
                (void *) baton->out8[ii],       // void* external_data,
                (size_t) baton->int8[ii],       // size_t byte_length,
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
    ASSERT_FATAL(errcode == 0, __func__);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    napi_ref ref = (napi_ref) baton->result;
    uint32_t refcount = 1;
    // dereference result to allow gc
    errcode = napi_reference_unref(env, ref, &refcount);
    ASSERT_FATAL(errcode == 0, "napi_reference_unref");
    ASSERT_FATAL(refcount == 0, "memory leak");
    errcode = napi_get_reference_value(env, ref, &baton->result);
    ASSERT_FATAL(errcode == 0, "napi_get_reference_value");
    errcode = napi_delete_reference(env, ref);
    ASSERT_FATAL(errcode == 0, "napi_delete_reference");
    // Resolve or reject the promise associated with the deferred depending on
    // whether the asynchronous action succeeded.
    if (baton->errmsg[0] == 0) {
        // resolve promise with result
        if (jsbatonExport(env, baton) == NULL) {
            return 0;
        }
        errcode = napi_resolve_deferred(env, baton->deferred, baton->result);
        ASSERT_FATAL(errcode == 0, "napi_resolve_deferred");
    } else {
        // declare var
        napi_value err;
        // create error
        errcode =
            napi_create_error(env, NULL, jsstringCreate(env, baton->errmsg),
            &err);
        ASSERT_FATAL(errcode == 0, "napi_create_error");
        // reject promise with error
        errcode = napi_reject_deferred(env, baton->deferred, err);
        ASSERT_FATAL(errcode == 0, "napi_reject_deferred");
    }
    // Clean up the work item associated with this run.
    errcode = napi_delete_async_work(env, baton->work);
    ASSERT_FATAL(errcode == 0, "napi_delete_async_work");
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
file sqlmath_db.c
*/
static int JSPROMISE_CREATE(
    __sqlite3_close_v2,
    env,
    data
) {
// int sqlite3_close_v2(sqlite3*);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->int8[0];
    // printf("\n\n[napi close=%zd]\n\n", (intptr_t) db);
    // call sqlite3_close_v2()
    errcode = sqlite3_close_v2(db);
    ASSERT_SQLITE_OK(baton, db, errcode);
    // save result
    baton->int8[0] = (int64_t) (intptr_t) db;
    return 0;
}

static int JSPROMISE_CREATE(
    __sqlite3_open_v2,
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
    const char *filename = (const char *) baton->buf8[0];
    int errcode = 0;
    int flags = (int) baton->int8[2];
    sqlite3 *db = NULL;
    // validate filename
    // printf("\n\n[napi filename=%s]\n\n", filename);
    ASSERT_BATON(baton, filename != NULL, "filename == NULL");
    // call sqlite3_open_v2()
    errcode = sqlite3_open_v2(filename, &db, flags, NULL);
    ASSERT_SQLITE_OK(baton, db, errcode);
    // printf("\n\n[napi db=%zd]\n\n", (intptr_t) db);
    // save result
    baton->int8[0] = (int64_t) (intptr_t) db;
    return 0;
}

static int JSPROMISE_CREATE(
    _dbExec,
    env,
    data
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized a json-string in
// <pResult>.
/* *INDENT-OFF* */
void dbExec(sqlite3 *, const char *, char **, int *, char *);
/* *INDENT-ON* */
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    char *zBuf = NULL;
    char zErrmsg[256] = { 0 };
    const char *zSql = (const char *) baton->buf8[1];
    int nAlloced = 0;
    sqlite3 *db = (sqlite3 *) (intptr_t) baton->int8[0];
    // validate zSql
    // printf("\n\n[napi zSql=%s]\n\n", zSql);
    ASSERT_BATON(baton, zSql != NULL, "zSql == NULL");
    // call dbExec()
    dbExec(db, zSql, &zBuf, &nAlloced, zErrmsg);
    // printf("\n\n[napi nAlloced=%d zBuf=%s]\n\n", nAlloced, zBuf);
    ASSERT_BATON(baton, zErrmsg[0] == '\x00', zErrmsg);
    // save result
    baton->int8[0] = (int64_t) nAlloced;
    baton->out8[0] = zBuf;
    return 0;
}


/*
file sqlmath_noop.c
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
file napi_module_init.c
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
    int errcode = 0;
    const napi_property_descriptor propList[] = {
        NAPI_EXPORT_MEMBER(_dbExec),
        NAPI_EXPORT_MEMBER(__sqlite3_close_v2),
        NAPI_EXPORT_MEMBER(__sqlite3_open_v2),
        NAPI_EXPORT_MEMBER(noopAsync),
        NAPI_EXPORT_MEMBER(noopSync),
    };
    errcode = napi_define_properties(env, exports,
        sizeof(propList) / sizeof(napi_property_descriptor), propList);
    ASSERT_NAPI_OK(env, errcode);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_init)
