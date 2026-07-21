#include "GLUtils.h"
#include <iostream>

FullscreenQuad CreateFullscreenQuad() {
    FullscreenQuad q{0,0};
    float quadVertices[] = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         3.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  3.0f,  0.0f, 1.0f
    };

    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    // no vertex attribs required; shader uses gl_VertexID
    glBindVertexArray(0);

    q.vao = vao;
    q.vbo = vbo;
    return q;
}

void DestroyFullscreenQuad(const FullscreenQuad& quad) {
    if (quad.vbo) glDeleteBuffers(1, &quad.vbo);
    if (quad.vao) glDeleteVertexArrays(1, &quad.vao);
}

GLuint CreateOutputTexture(int width, int height) {
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    // Bind as image unit 0 by convention; callers can rebind if needed
    glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    // unbind texture from array target (keep it bound to image unit)
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

void ResizeOutputTexture(GLuint texture, int width, int height) {
    if (texture == 0) return;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DestroyOutputTexture(GLuint texture) {
    if (texture) glDeleteTextures(1, &texture);
}

void SetUniform3f(GLuint program, const char* name, glm::vec3 value) {
    GLint loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform3f(loc, value.x, value.y, value.z);
    } else {
        //std::cerr << "Warning: uniform '" << name << "' not found." << std::endl;
    }
}

void SetUniform1f(GLuint program, const char* name, float value) {
    GLint loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform1f(loc, value);
    } else {
        //std::cerr << "Warning: uniform '" << name << "' not found." << std::endl;
    }
}


void SetUniform1i(GLuint program, const char* name, int value) {
    GLint loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform1i(loc, value);
    } else {
        //std::cerr << "Warning: uniform '" << name << "' not found." << std::endl;
    }
}