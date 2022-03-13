// copyright nobody
// LINT_C_FILE


// header
#include "sqlmath_base.c"


/*
file sqlmath_ext - start
*/
int sqlite3_sqlmath_ext_custom_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    // coverage-hack
    noop();
    UNUSED(db);
    UNUSED(pApi);
    UNUSED(pzErrMsg);
    return 0;
}

/*
file sqlmath_ext - end
*/
