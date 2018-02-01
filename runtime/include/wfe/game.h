#ifndef WFE_GAME_H
#define WFE_GAME_H
#include <wfe/types.h>

#define WFE_WINDOW_DEFAULT_HEIGHT 800
#define WFE_WINDOW_DEFAULT_WIDTH 600
#define WFE_WINDOW_DEFAULT_TITLE "WhiteFire Game Engine 1.0"

#define WFE_GAME_OMEM WFE_MAKE_MEMORY_ERROR(21)
#define WFE_GLFW_INIT_ERROR WFE_MAKE_API_ERROR(22)
#define WFE_GLAD_INIT_ERROR WFE_MAKE_API_ERROR(23)

/**
 * wfeGame is the entire game context, it contains
 * binding data for lower API such GLFW or GLFM.
 */
typedef struct wfeGame wfeGame;

/**
 * Make a new game based on given configuration.
 *
 * Note: Will allocate required data to store game handler, you must
 * free this game calling wfeGameFinalize.
 * Params:
 *  - cname configuration file name for this game.
 *  - context argument for each API, check bellow for more information.
 * Return:
 *  - wfeGame reference, however must check errors with wfeGameLastError before using it.
 *  - NULL if no memory is available for the game struct creation.
 * Context argument:
 *  - Using GLFW: NULL.
 *  - Using GLFM: Use given GLFMDisplay* by glfmMain.
 */
wfeGame *wfeGameMake(wfeData *cname, wfeAny context);

/**
 * Release all resources and references associated with given game.
 *
 * Note: This funciton must also finalize other internal members inside game,
 * such windows, buffers and assets.
 *
 * Params:
 *  - game to be finalized and released.
 */
void wfeGameFinalize(wfeGame *game);

/**
 * Returns the last error code for the playing game.
 *
 * Params:
 *  - game to get error code if available.
 * Return:
 *  - WFE_SUCCESS if no errors are found.
 *  - Any error code if some call or background job have failed, check documentation for each operation.
 */
wfeError wfeGameLastError(wfeGame *game);

/**
 * Returns the raw pointer to used context.
 * Param:
 *  - wfeGame reference.
 * Return:
 *  - Using GLFW: GLFWwindow reference.
 *  - Using GLFM: GLFMDisplay reference.
 */
wfeAny wfeGameGetContext(wfeGame *game);


#endif /* WFE_GAME_H */
