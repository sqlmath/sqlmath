#include "extension-functions.c"

static void covarianceStep(sqlite3_context *ctx, int argc, sqlite3_value **argv){
/*
** called for each value received during a calculation of covariance
** using welford's online algorithm.
*/
    //!! StdevCtx *pp;
    //!! double dx;
    //!! double dy;
    //!! double xx;
    //!! double yy;
    //!! assert( argc==2 );
    //!! pp = sqlite3_aggregate_context(ctx, sizeof(*pp));
    //!! /* only consider non-null values */
    //!! if( SQLITE_NULL != sqlite3_value_numeric_type(argv[0]) ){
        //!! pp->cnt++;
        //!! xx = sqlite3_value_double(argv[0]);
        //!! yy = sqlite3_value_double(argv[1]);
        //!! dx = (xx-pp->rM);
        //!! pp->rM += dx/pp->cnt;
        //!! pp->rS += dx*(xx-pp->rM);
    //!! }
}

/*
** Returns the variance value
*/
static void covarianceFinalize(sqlite3_context *context){
    //!! StdevCtx *p;
    //!! p = sqlite3_aggregate_context(context, 0);
    //!! if( p && p->cnt>1 ){
        //!! sqlite3_result_double(context, p->rS/(p->cnt-1));
    //!! }else{
        //!! sqlite3_result_double(context, 0.0);
    //!! }
}

/*
** This function registered all of the above C functions as SQL
** functions.  This should be the only routine in this file with
** external linkage.
*/
int RegisterSqlmath(sqlite3 *db){
    /* Aggregate functions */
    static const struct FuncDefAgg {
        char *zName;
        signed char nArg;
        uint8_t argType;
        uint8_t needCollSeq;
        void (*xStep)(sqlite3_context*,int,sqlite3_value**);
        void (*xFinalize)(sqlite3_context*);
    } aAggs[] = {
        //!! { "stdev",            1, 0, 0, varianceStep, stdevFinalize  },
        { "covariance",         1, 0, 0, covarianceStep, covarianceFinalize  },
        //!! { "mode",             1, 0, 0, modeStep,     modeFinalize  },
        //!! { "median",           1, 0, 0, modeStep,     medianFinalize  },
        //!! { "lower_quartile",   1, 0, 0, modeStep,     lower_quartileFinalize  },
        //!! { "upper_quartile",   1, 0, 0, modeStep,     upper_quartileFinalize  },
    };
    double xx = NAN;
    unsigned long i;
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
    }
    return 0;
}

int sqlite3ext_sqlmath_init(
    sqlite3 *db,
    char **pzErrMsg,
    const sqlite3_api_routines *pApi
){
    SQLITE_EXTENSION_INIT2(pApi);
    RegisterSqlmath(db);
    return 0;
}
