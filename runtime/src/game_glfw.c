#include <glad/glad.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include <wfe/asset.h>
#include <wfe/pool.h>
#include <wfe/desc.h>
#include <wfe/game.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct wfeGameConfig {
    wfeData *title;
    wfeInt height;
    wfeInt width;
    wfeInt resizable;
    wfeInt visible;
    wfeInt decorated;
    wfeInt focused;
    wfeInt floating;
    wfeInt maximized;
} wfeGameConfig;

typedef struct wfeGame {
    GLFWwindow *window;
    wfeError lastError;
    wfePool miscPool;
} wfeGame;

// Callback to handle framebuffer resizes (i.e. from window resize)
void wfeHandleFramebufferResize(GLFWwindow *window, GLint width, GLint height);

// Dumps description file into configuration
wfeError wfeConfigureGame(wfeGameConfig *config, const wfeChar *cname, wfePool *pool);

wfeGame *wfeGameMake(wfeChar *cname, wfeAny context) {
    wfeGame *game = calloc(1, sizeof(wfeGame));
    if (game == NULL) {
        return NULL;
    }

    // No errors at begining
    game->lastError = WFE_SUCCESS;

    assert(cname != NULL /* Description file */);

    // Init pool and game window configuration.
    game->lastError = wfePoolInit(&game->miscPool);
    if (WFE_HAVE_FAILED(game->lastError)) {
        goto finalize;
    }

    // Configure game
    wfeGameConfig config;
    game->lastError = wfeConfigureGame(&config, cname, &game->miscPool);
    if (WFE_HAVE_FAILED(game->lastError)) {
        goto finalize;
    }

    // Try to initialize glfw
    if(GL_FALSE == glfwInit()) {
        game->lastError = WFE_GLFW_INIT_ERROR; goto finalize;
    }

    // TODO: OSX Context configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, config.resizable);
    glfwWindowHint(GLFW_VISIBLE, config.visible);
    glfwWindowHint(GLFW_DECORATED, config.decorated);
    glfwWindowHint(GLFW_FOCUSED, config.focused);
    glfwWindowHint(GLFW_FLOATING, config.floating);
    glfwWindowHint(GLFW_MAXIMIZED, config.maximized);

    // Default window creation.
    game->window = glfwCreateWindow(config.width, config.height, config.title, NULL, NULL);
    if(game->window == NULL) {
        game->lastError = WFE_GLFW_INIT_ERROR; goto finalize;
    }

    glfwMakeContextCurrent(game->window);
    glfwSetFramebufferSizeCallback(game->window, wfeHandleFramebufferResize);

    // Try to load GLAD
    if (GL_FALSE == gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
        game->lastError = WFE_GLAD_INIT_ERROR; goto finalize;
    }

    // Default viewport
    glViewport(0, 0, config.width, config.height);

finalize:
    if (WFE_HAS_FAILED(game->lastError)) {
        if (game->window != NULL) {
            glfwDestroyWindow(game->window);
        }

        wfePoolFinalize(&game->miscPool);
        free(game);
    }

    return game;
}

void wfeGameFinalize(wfeGame *game) {
    assert(game != NULL /* A game should exists */);
    if (game->window != NULL) {
        glfwDestroyWindow(game->window);
    }

    wfePoolFinalize(&game->miscPool);
    glfwTerminate();
    free(game);
}

wfeError wfeGameLastError(wfeGame *game) {
    return game->lastError;
}

void wfeHandleFramebufferResize(GLFWwindow *window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

wfeAny wfeGameGetContext(wfeGame *game) {
    return game->window;
}

wfeError wfeConfigureGame(wfeGameConfig *config, const wfeChar *cname, wfePool *pool) {
    wfeError code = WFE_SUCCESS;
    const wfeChar *kname = NULL;
    wfeSize ksize = 0L;

    // Defaults
    config->height = WFE_WINDOW_DEFAULT_HEIGHT;
    config->width = WFE_WINDOW_DEFAULT_WIDTH;
    config->title = WFE_WINDOW_DEFAULT_TITLE;
    config->resizable = WFE_TRUE;
    config->visible = WFE_TRUE;
    config->decorated = WFE_TRUE;
    config->focused = WFE_TRUE;
    config->floating = WFE_FALSE;
    config->maximized = WFE_FALSE;

    wfeDesc desc;
    wfeDescInit(&desc);

    code = wfeAssetLoadDesc(cname, pool, &desc);
    if (WFE_HAVE_FAILED(code)) {
        return code;
    }

    code = wfeDescNextKey(&desc, &kname, &ksize);
    while (WFE_SHOULD_CONTINUE(code)){
        if (strncmp(kname, "height", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->height);
            if (WFE_HAVE_FAILED(code)) {
                config->height = WFE_WINDOW_DEFAULT_HEIGHT;
                goto finalize;
            }
        }

        if (strncmp(kname, "width", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->width);
            if (WFE_HAVE_FAILED(code)) {
                config->width = WFE_WINDOW_DEFAULT_WIDTH;
                goto finalize;
            }
        }

        if (strncmp(kname, "title", ksize) == 0) {
            const wfeChar *tstr = NULL;
            wfeSize tlen = 0L;

            code = wfeDescGetString(&desc, &tstr, &tlen);
            if (WFE_HAVE_FAILED(code)) {
                config->title = WFE_WINDOW_DEFAULT_TITLE;
                goto finalize;
            }

            // Perserve title's string on misc pool.
            if (tlen > 0) {
                config->title = wfePoolGet(pool, sizeof(char) * tlen + 1, wfeAlignOf(char));
                strncpy(config->title, tstr, tlen);
            } else {
                config->title = WFE_WINDOW_DEFAULT_TITLE;
            }
        }

        if (strncmp(kname, "resizable", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->resizable);
            if (WFE_HAVE_FAILED(code)) {
                config->resizable = WFE_TRUE;
            }
        }

        if (strncmp(kname, "visible", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->visible);
            if (WFE_HAVE_FAILED(code)) {
                config->resizable = WFE_TRUE;
            }
        }

        if (strncmp(kname, "decorated", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->decorated);
            if (WFE_HAVE_FAILED(code)) {
                config->resizable = WFE_TRUE;
            }
        }

        if (strncmp(kname, "focused", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->focused);
            if (WFE_HAVE_FAILED(code)) {
                config->resizable = WFE_TRUE;
            }
        }

        if (strncmp(kname, "floating", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->floating);
            if (WFE_HAVE_FAILED(code)) {
                config->resizable = WFE_FALSE;
            }
        }

        if (strncmp(kname, "maximized", ksize) == 0) {
            code = wfeDescGetInt(&desc, &config->maximized);
            if (WFE_HAVE_FAILED(code)) {
                config->resizable = WFE_FALSE;
            }
        }

        code = wfeDescNextKey(&desc, &kname, &ksize);
    }

finalize:
    wfeDescFinalize(&desc);
    if (WFE_HAVE_FAILED(code))
        return code;

    return WFE_SUCCESS;
}

