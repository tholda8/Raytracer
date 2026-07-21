cmake_minimum_required(VERSION 3.15)
project(RaytracerCompute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW + GLAD (pokud je máš v external/)
add_subdirectory(external/glfw)
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external)
target_include_directories(glad PUBLIC external/glad/include)
# Vlastní zdrojáky
file(GLOB SRC src/*.cpp)
list(APPEND SRC "${CMAKE_SOURCE_DIR}/src/GLUtils.cpp")
add_executable(RaytracerCompute ${SRC})
target_include_directories(RaytracerCompute PRIVATE src external/glad/include external/glfw/include)

target_link_libraries(RaytracerCompute glfw glad)
