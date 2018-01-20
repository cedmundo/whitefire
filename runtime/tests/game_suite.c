#include "minunit.h"
#include <wfe/game.h>

static char * test_types_make_finalize() {
    // Try making a window and destroying it.
    wfeGame *game = wfeGameMake(NULL, NULL);
    mu_assert("game is null", game != NULL);
    mu_assert("there are errors at window creation", !WFE_HAS_FAILED(wfeGameLastError(game)));
    wfeGameFinalize(game);
    return 0;
}

static char * game_suite() {
    mu_suite_start(types);
    mu_run_test(test_types_make_finalize);
    mu_suite_end(types);
    return 0;
}

