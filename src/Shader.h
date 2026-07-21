#pragma once
#include <glad/glad.h>
#include <string>

GLuint LoadComputeShader(const std::string& path);
GLuint LoadShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
