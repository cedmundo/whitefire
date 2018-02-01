#include "minunit.h"
#include <wfe/game.h>

static char * test_game_make_finalize() {
    // Try making a window and destroying it.
    //wfeGame *game = wfeGameMake("test_game_glfw", NULL);
    //mu_assert("game is null", game != NULL);
    //mu_assert("there are errors at window creation", !WFE_HAS_FAILED(wfeGameLastError(game)));
    //wfeGameFinalize(game);
    return 0;
}

static char * game_suite() {
    char *envsp = getenv("WFE_SEARCH_PATH");
    if (envsp != NULL)
        wfeAssetSetSearchPath(envsp);
    else
        wfeAssetSetSearchPath("tests/assets");

    mu_suite_start(game);
    mu_run_test(test_game_make_finalize);
    mu_suite_end(game);
    return 0;
}

