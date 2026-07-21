#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Entities.h"
#include <glm/glm.hpp>
#include <vector>

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void Init();
    void UploadScene(const Scene& scene);
    void Update();
    void Render();
    void Shutdown();
    bool ShouldClose() const;
    // expose the GLFW window for input checks in main
    GLFWwindow* GetWindow();
    // reset accumulation buffers and counters (useful when switching scenes)
    void ResetAccumulation();
    // scene management
    void AddScene(Scene* scene, bool makeCurrent = false);
    void NextScene();
    void PrevScene();



private:
    void InitGLFW();
    void InitGL();
    void CreateComputeShader();
    void CreateFullScreenQuad();
    void OnFramebufferSize(int width, int height);

    Camera camera;
    // background clear color for the default framebuffer (RGBA)
    glm::vec4 clearColor = glm::vec4(1.0f, 0.08f, 0.08f, 1.0f);
    // input / camera control
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
    float mouseSensitivity = 0.0025f; // radians per pixel
    float moveSpeed = 5.0f; // units per second
    double prevTime = 0.0;

    int width, height;
    GLFWwindow* window;
    GLuint computeProgram;
    GLuint graphicsProgram;
    GLuint texture;
    GLuint vao, vbo;
    GLuint ssboObjects, ssboMaterials;
    // frame counter uploaded to compute shader for RNG
    int frameCount = 0;
    // uploaded counts for SSBOs (used to set shader uniforms)
    int uploadedObjectCount = 0;
    int uploadedMaterialCount = 0;




    // accumulation (ping-pong) resources
    GLuint accTex[2] = {0,0};
    GLuint accFbo = 0;
    GLuint accumProgram = 0;
    int accPing = 0; // index of next write target
    // previous camera state to detect movement for resetting accumulation
    glm::vec3 prevCameraPos = glm::vec3(0.0f);
    float prevCameraYaw = 0.0f;
    float prevCameraPitch = 0.0f;
    // cached camera basis computed only when camera changes
    glm::vec3 cachedCameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cachedCameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cachedCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // whether camera uniforms need uploading to the GPU
    bool cameraUniformsDirty = true;
    // scenes managed by renderer
    std::vector<Scene*> scenes;
    int curSceneIndex = -1;
    // key edge detection state for scene switching
    int lastLeftState = GLFW_RELEASE;
    int lastRightState = GLFW_RELEASE;
};