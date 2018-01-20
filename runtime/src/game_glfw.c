#include <glad/glad.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include <wfe/game.h>
#include <stdlib.h>
#include <assert.h>

typedef struct wfeGame {
    GLFWwindow *window;
    wfeError lastError;
} wfeGame;

// Callback to handle framebuffer resizes (i.e. from window resize)
void wfeHandleFramebufferResize(GLFWwindow *window, GLint width, GLint height);

wfeGame *wfeGameMake(wfeData *cname, wfeAny context) {
    wfeGame *game = calloc(1, sizeof(wfeGame));
    if (game == NULL) {
        return NULL;
    }

    assert(cname == NULL /* Not implemented yet */);

    // No errors at begining
    game->lastError = WFE_SUCCESS;

    // Try to initialize glfw
    if(GL_FALSE == glfwInit()) {
        game->lastError = WFE_GLFW_INIT_ERROR; goto finalize;
    }

    // TODO: OSX Context configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Default window creation.
    game->window = glfwCreateWindow(
            WFE_WINDOW_DEFAULT_WIDTH,
            WFE_WINDOW_DEFAULT_HEIGHT,
            WFE_WINDOW_DEFAULT_TITLE, NULL, NULL);
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
    glViewport(0, 0, WFE_WINDOW_DEFAULT_WIDTH, WFE_WINDOW_DEFAULT_HEIGHT);

finalize:
    if (WFE_HAS_FAILED(game->lastError)) {
        if (game->window != NULL) {
            glfwDestroyWindow(game->window);
        }

        free(game);
    }

    return game;
}

void wfeGameFinalize(wfeGame *game) {
    assert(game != NULL /* A game should exists */);
    if (game->window != NULL) {
        glfwDestroyWindow(game->window);
    }

    glfwTerminate();
    free(game);
}

wfeError wfeGameLastError(wfeGame *game) {
    return game->lastError;
}

void wfeHandleFramebufferResize(GLFWwindow *window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}
