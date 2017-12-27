#include "minunit.h"
#include <wfe/types.h>

static char * test_types_int() {
    mu_assert("failed: sizes of wfeInt16 <= wfeInt32", sizeof(wfeInt16) <= sizeof(wfeInt32));
    mu_assert("failed: sizes of wfeInt32 <= wfeInt64", sizeof(wfeInt32) <= sizeof(wfeInt64));
    mu_assert("failed: sizes of wfeUint32 != wfeInt32", sizeof(wfeUint32) == sizeof(wfeInt32));
    mu_assert("failed: sizes of wfeUint64 != wfeInt64", sizeof(wfeUint64) == sizeof(wfeInt64));
    mu_assert("failed: sizes of wfeUint != wfeInt", sizeof(wfeUint) == sizeof(wfeInt));
    return 0;
}

static char * test_types_float() {
    mu_assert("failed: size of wfeFloat32 <= wfeFloat64", sizeof(wfeFloat32) <= sizeof(wfeFloat64));
    return 0;
}

static char * types_suite() {
    mu_run_test(test_types_int);
    mu_run_test(test_types_float);
    //mu_run_test(test_types_float);
    return 0;
}

