#include "minunit.h"
#include <wfe/pool.h>
#include <wfe/game.h>
#include <wfe/mesh.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include "wshader.h"

// populates vertex data of a mesh
void populate_vertices(wfeMesh *mesh, wfePool *pool);

// populates indexes of a mesh
void populate_indexes (wfeMesh *mesh, wfePool *pool);

static char * test_mesh_init_finalize() {
    wfeMesh mesh;
    mu_assert("uninitialized mesh", !WFE_HAVE_FAILED(wfeMeshInit(&mesh)));
    mu_assert("invalid zero value for vao", mesh.vao == 0);
    mu_assert("invalid zero value for vbo", mesh.vbo == 0);
    mu_assert("invalid zero value for ebo", mesh.ebo == 0);

    wfeMeshFinalize(&mesh);
    mu_assert("invalid zero value for vao", mesh.vao == 0);
    mu_assert("invalid zero value for vbo", mesh.vbo == 0);
    mu_assert("invalid zero value for ebo", mesh.ebo == 0);
    return 0;
}

static char * test_mesh_setup_mesh() {
    wfeGame *game = wfeGameMake("test_game_glfw", NULL);
    mu_assert("game is null", game != NULL);

    wfePool pool;
    mu_assert("could not init pool", wfePoolInit(&pool) == WFE_SUCCESS);

    wfeMesh mesh;
    mu_assert("could not init mesh", !WFE_HAVE_FAILED(wfeMeshInit(&mesh)));

    populate_indexes(&mesh, &pool);
    populate_vertices(&mesh, &pool);
    wfeMeshSetupBuffers(&mesh);
    mu_assert("invalid value for vao", mesh.vao != 0);
    mu_assert("invalid value for vbo", mesh.vbo != 0);
    mu_assert("invalid value for ebo", mesh.ebo != 0);

    GLuint shaderProgram = get_testing_shader();
    GLFWwindow *window = (GLFWwindow *) wfeGameGetContext(game);
    while(GL_FALSE == glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw figure
        glBindVertexArray(mesh.vbo);
        {
            // Use the specified shaderProgram
            glUseProgram(shaderProgram);
            // Draw the object
            glDrawElements(GL_TRIANGLES, mesh.icount, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    wfePoolFinalize(&pool);
    wfeGameFinalize(game);
    return 0;
}

static char * mesh_suite() {
    char *envsp = getenv("WFE_SEARCH_PATH");
    if (envsp != NULL)
        wfeAssetSetSearchPath(envsp);
    else
        wfeAssetSetSearchPath("tests/assets");

    mu_suite_start(mesh);
    mu_run_test(test_mesh_init_finalize);
    mu_run_test(test_mesh_setup_mesh);
    mu_suite_end(mesh);
    return 0;
}

void populate_vertices(wfeMesh *mesh, wfePool *pool) {
    mesh->vertices = (wfeVertex *) wfePoolGet(pool, sizeof(wfeVertex) * 3, wfeAlignOf(wfeVertex));
    mesh->vcount = 3;

    mesh->vertices[0] = (wfeVertex){{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
    mesh->vertices[1] = (wfeVertex){{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
    mesh->vertices[2] = (wfeVertex){{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
}

void populate_indexes (wfeMesh *mesh, wfePool *pool) {
    mesh->indexes = (GLuint *) wfePoolGet(pool, sizeof(GLuint) * 3, wfeAlignOf(GLuint));
    mesh->icount = 3;

    mesh->indexes[0] = 0;
    mesh->indexes[1] = 1;
    mesh->indexes[2] = 2;
}
