#ifndef WFE_TEST_WSHADER_H
#define WFE_TEST_WSHADER_H
#include <glad/glad.h>
#include <stdio.h>

GLuint get_testing_shader() {
    static GLuint program = 0;
    if (program != 0) {
        return program;
    }

    //const char *vsource = "#version 330 core\nlayout (location = 0) in vec3 aPos;layout (location = 1) in vec3 aNor; layout (location = 2) in vec2 aTex; \
    //                       void main() {gl_Position = vec4(aPos, 1.0);}";
    const char *vsource = "#version 330 core\nlayout(location = 0) in vec3 aPos; void main () { gl_Position = vec4(aPos, 1.0);}";
    const char *fsource = "#version 330 core\nout vec4 FragColor;\nvoid main(){ FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); }";
    GLint compileSuccess = 0;
    GLchar compileLog[512];

    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsid, 1, &vsource, NULL);
    glCompileShader(vsid);

    // Check compilation status
    glGetShaderiv(vsid, GL_COMPILE_STATUS, &compileSuccess);
    if (GL_FALSE == compileSuccess) {
        glGetShaderInfoLog(vsid, 512, NULL, compileLog);
        fprintf(stderr, "[ERROR] vertex shader compilation failed: %s\n", compileLog);
        return GL_FALSE;
    }

    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsid, 1, &fsource, NULL);
    glCompileShader(fsid);

    glGetShaderiv(fsid, GL_COMPILE_STATUS, &compileSuccess);
    if (GL_FALSE == compileSuccess) {
        glGetShaderInfoLog(fsid, 512, NULL, compileLog);
        fprintf(stderr, "[ERROR] fragment shader compilation failed: %s\n", compileLog);
        return GL_FALSE;
    }

    program = glCreateProgram();
    glAttachShader(program, vsid);
    glAttachShader(program, fsid);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &compileSuccess);
    if (GL_FALSE == compileSuccess) {
        glGetProgramInfoLog(program, 512, NULL, compileLog);
        fprintf(stderr, "[ERROR] Shader program linking error: %s\n", compileLog);
        return GL_FALSE;
    }

    return program;
}

#endif /* WFE_TEST_WSHADER_H */
