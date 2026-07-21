#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLuint LoadComputeShader(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader: " << path << std::endl;
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string src = ss.str();
    const char* srcPtr = src.c_str();

    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &srcPtr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Compute shader compile error:\n" << log << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);
    glDeleteShader(shader);

    // Check link status
    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, log);
        std::cerr << "Compute shader link error:\n" << log << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

static std::string ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return std::string();
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint LoadShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vsrc = ReadFile(vertexPath);
    std::string fsrc = ReadFile(fragmentPath);
    if (vsrc.empty() || fsrc.empty()) {
        std::cerr << "Failed to read shader files: " << vertexPath << ", " << fragmentPath << std::endl;
        return 0;
    }

    const char* vptr = vsrc.c_str();
    const char* fptr = fsrc.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vptr, nullptr);
    glCompileShader(vs);
    GLint ok;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(vs, 1024, nullptr, log);
        std::cerr << "Vertex shader compile error:\n" << log << std::endl;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fptr, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(fs, 1024, nullptr, log);
        std::cerr << "Fragment shader compile error:\n" << log << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, log);
        std::cerr << "Shader program link error:\n" << log << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    return program;
}
