#include <glad/glad.h>
#include <wfe/types.h>
#include <wfe/mesh.h>

wfeError wfeMeshInit(wfeMesh *mesh) {
    mesh->vertices = NULL;
    mesh->indexes = NULL;
    mesh->icount = 0;
    mesh->vcount = 0;
    mesh->vao = 0;
    mesh->vbo = 0;
    mesh->ebo = 0;
    return WFE_SUCCESS;
}

void wfeMeshFinalize(wfeMesh *mesh) {
    mesh->vertices = NULL;
    mesh->indexes = NULL;
    mesh->icount = 0;
    mesh->vcount = 0;
    mesh->vao = 0;
    mesh->vbo = 0;
    mesh->ebo = 0;
}

void wfeMeshSetupBuffers(wfeMesh *mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);
    {
        // bind mesh array
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wfeVertex) * mesh->vcount, mesh->vertices, GL_STATIC_DRAW);

        // bind element array
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->icount, mesh->indexes, GL_STATIC_DRAW);

        // Attributes: vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(wfeVertex), (void *) 0);

        // Attributes: normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(wfeVertex), (void *) offsetof(wfeVertex, nor));

        // Attributes: texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(wfeVertex), (void *) offsetof(wfeVertex, txc));
    }
    glBindVertexArray(0);
}
