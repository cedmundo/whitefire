#include <stdio.h>
#include "minunit.h"

// include all suites
#include "types_suite.c"

int tests_run = 0;
static char * all_suites() {
    mu_run_suite(types_suite);
    return 0;
}

int main() {
    char *result = all_suites();
    if (result != 0) {
        fprintf(stderr, "%s\n", result);
    } else {
        fprintf(stderr, "ALL TESTS PASSED\n");
    }

    fprintf(stderr, "Tests run: %d\n", tests_run);
    return result != 0;
}
