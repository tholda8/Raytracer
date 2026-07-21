#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct FullscreenQuad {
    GLuint vao;
    GLuint vbo;
};

// Creates a VAO/VBO for drawing a fullscreen triangle/quad. Returns vao/vbo handles.
FullscreenQuad CreateFullscreenQuad();
void DestroyFullscreenQuad(const FullscreenQuad& quad);

// Create an RGBA32F texture sized width x height suitable for compute shader image store.
GLuint CreateOutputTexture(int width, int height);
// Resize an existing texture (level 0) to new dimensions.
void ResizeOutputTexture(GLuint texture, int width, int height);
// Destroy a texture created with CreateOutputTexture
void DestroyOutputTexture(GLuint texture);

void SetUniform3f(GLuint program, const char* name, glm::vec3 value);
void SetUniform1f(GLuint program, const char* name, float value);
void SetUniform1i(GLuint program, const char* name, int value);