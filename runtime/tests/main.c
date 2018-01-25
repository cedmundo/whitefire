#include <stdio.h>
#include "minunit.h"

// include all suites
#include "types_suite.c"
#include "pool_suite.c"
#include "desc_suite.c"
#include "asset_suite.c"
#include "game_suite.c"

int tests_run = 0;
static char * all_suites() {
    mu_msg("Running tests for WhiteFire Game Engine");
    mu_run_suite(types_suite);
    mu_run_suite(pool_suite);
    mu_run_suite(desc_suite);
    mu_run_suite(asset_suite);
    mu_run_suite(game_suite);
    return 0;
}

int main() {
    char *result = all_suites();
    if (result != 0) {
        fprintf(stderr, "\t=> failed: %s\n", result);
    } else {
        fprintf(stderr, "ALL TESTS PASSED\n");
    }

    fprintf(stderr, "Tests run: %d\n", tests_run);
    return result != 0;
}
