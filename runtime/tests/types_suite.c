#include "minunit.h"
#include <wfe/types.h>

static char * test_types_int() {
    mu_assert("sizes of wfeInt16 <= wfeInt32", sizeof(wfeInt16) <= sizeof(wfeInt32));
    mu_assert("sizes of wfeInt32 <= wfeInt64", sizeof(wfeInt32) <= sizeof(wfeInt64));
    mu_assert("sizes of wfeUint32 != wfeInt32", sizeof(wfeUint32) == sizeof(wfeInt32));
    mu_assert("sizes of wfeUint64 != wfeInt64", sizeof(wfeUint64) == sizeof(wfeInt64));
    mu_assert("sizes of wfeUint != wfeInt", sizeof(wfeUint) == sizeof(wfeInt));
    return 0;
}

static char * test_types_float() {
    mu_assert("size of wfeFloat32 <= wfeFloat64", sizeof(wfeFloat32) <= sizeof(wfeFloat64));
    return 0;
}

static char * test_types_bool() {
    mu_assert("boolean true == false", WFE_TRUE != WFE_FALSE);
    mu_assert("boolean true != ~false", WFE_TRUE == !WFE_FALSE);
    return 0;
}

static char * test_types_char() {
    mu_assert("size of wfeChar16 <= wfeChar32", sizeof(wfeChar16) <= sizeof(wfeChar32));
    return 0;
}

static char * test_types_alignof() {
    mu_assert("unexpected aligment for wfeInt16 and wfeInt32", wfeAlignOf(wfeInt16)*2 == wfeAlignOf(wfeInt32));
    mu_assert("unexpected aligment for wfeInt32 and wfeInt64", wfeAlignOf(wfeInt32)*2 == wfeAlignOf(wfeInt64));
    return 0;
}

static char * test_types_error() {
    wfeError custom = 0;

    custom = WFE_MAKE_MEMORY_ERROR(40);
    mu_assert("custom mem error should be a failure", WFE_HAS_FAILED(custom));
    mu_assert("custom mem error should be a memory error", WFE_HAS_MEMORY_ERROR(custom) == WFE_TRUE);
    mu_assert("custom mem error should not be a file error", WFE_HAS_FILE_ERROR(custom) == WFE_FALSE);
    mu_assert("unexpected custom mem error code", WFE_USER_ERROR_CODE(custom) == 40);

    custom = WFE_MAKE_FILE_ERROR(34);
    mu_assert("custom file error should be a failure", WFE_HAS_FAILED(custom));
    mu_assert("custom file error should not be a memory error", WFE_HAS_MEMORY_ERROR(custom) == WFE_FALSE);
    mu_assert("custom file error should be a file error", WFE_HAS_FILE_ERROR(custom) == WFE_TRUE);
    mu_assert("unexpected custom file error code", WFE_USER_ERROR_CODE(custom) == 34);
    return 0;
}

static char * types_suite() {
    mu_suite_start(types);
    mu_run_test(test_types_int);
    mu_run_test(test_types_float);
    mu_run_test(test_types_bool);
    mu_run_test(test_types_char);
    mu_run_test(test_types_alignof);
    mu_run_test(test_types_error);
    mu_suite_end(types);
    return 0;
}

