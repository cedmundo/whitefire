#ifndef WFE_MESH_H
#define WFE_MESH_H
#include <lmath/vec2.h>
#include <lmath/vec3.h>
#include <wfe/types.h>
#include <glad/glad.h>

/**
 * Vertex layout for mesh data.
 *
 * Currently only three params are supported:
 *  - (pos) position float[3]
 *  - (nor) normal float[3]
 *  - (txc) texture coordinates float[2]
 */
typedef struct wfeVertex {
    vec3 pos;
    vec3 nor;
    vec2 txc;
} wfeVertex;

/**
 * Mesh data structure, stores references to:
 *  - vertices
 *  - indexes
 *  - textures* not supported yet.
 *
 * Also, internally stores the ids for VAO, VBO and EBO. Those
 * are fill when wfeMeshSetupBuffer is called.
 */
typedef struct wfeMesh {
    wfeVertex *vertices;
    GLuint *indexes;
    wfeSize icount;
    wfeSize vcount;
    GLuint vao, vbo, ebo;
} wfeMesh;

/**
 * Initializes to zero the internal state.
 * Params:
 *  - mesh to initialize.
 *
 * Return:
 *  - WFE_SUCCESS always.
 */
wfeError wfeMeshInit(wfeMesh *mesh);

/**
 * Re-initialize mesh to zero values, does not release any memory.
 *
 * Params:
 *  - mesh to be finalized.
 */
void wfeMeshFinalize(wfeMesh *mesh);

/**
 * Binds all buffers to OpenGL, initializing vao, vbo and ebo values.
 *
 * Warning: mesh should contain vertices and indexes before calling this function.
 * Params:
 *  - mesh to be bound.
 */
void wfeMeshSetupBuffers(wfeMesh *mesh);

// void wfeMeshDraw(wfeShader *shader);

#endif /* WFE_MESH_H */
