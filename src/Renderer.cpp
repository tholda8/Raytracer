#include "Renderer.h"
#include "Shader.h"
#include "GLUtils.h"
#include <iostream>
#include <string>


Renderer::Renderer(int w, int h)
        : width(w), height(h), window(nullptr),
            computeProgram(0), texture(0), vao(0), vbo(0),
            ssboObjects(0), ssboMaterials(0) {
        // sensible defaults for camera
        camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
        camera.Yaw = 0.0f;
        camera.Pitch = 0.0f;
        camera.Roll = 0.0f;
    // default clear color (can be changed externally)
    clearColor = glm::vec4(0.08f, 0.08f, 0.12f, 1.0f);
}


Renderer::~Renderer() {}

void Renderer::Init() {
    InitGLFW();
    InitGL();
    // Query the actual framebuffer size (accounts for DPI scaling) and use it
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    if (fbw > 0 && fbh > 0) {
        width = fbw;
        height = fbh;
    }
    // set GL viewport to the framebuffer size
    glViewport(0, 0, width, height);
    CreateComputeShader();
    // create compute and graphics pipelines
    // create fullscreen quad (VAO/VBO) via GL utilities
    {
        FullscreenQuad quad = CreateFullscreenQuad();
        vao = quad.vao;
        vbo = quad.vbo;
    }
    // load fullscreen graphics program (vertex + fragment)
    graphicsProgram = LoadShaderProgram("shaders/fullscreen.vert", "shaders/fullscreen.frag");
    if (graphicsProgram) {
        glUseProgram(graphicsProgram);
        GLint loc = glGetUniformLocation(graphicsProgram, "resultImage");
        if (loc >= 0) glUniform1i(loc, 0); // bind sampler to texture unit 0
        // default gamma settings for display
        SetUniform1i(graphicsProgram, "ApplyGamma", 1);
        SetUniform1f(graphicsProgram, "Gamma", 2.2f);
    }
    // vytvoří texturu pro výstup (use helper)
    texture = CreateOutputTexture(width, height);
    // create two accumulation textures and FBO for ping-pong accumulation
    accTex[0] = CreateOutputTexture(width, height);
    accTex[1] = CreateOutputTexture(width, height);
    glGenFramebuffers(1, &accFbo);
    // load accumulation shader program (uses fullscreen.vert)
    accumProgram = LoadShaderProgram("shaders/fullscreen.vert", "shaders/accum.frag");
    // Ensure compute image unit 0 is bound to the compute output texture (texture)
    // GLUtils::CreateOutputTexture previously bound image unit 0 to the last-created texture,
    // so rebind it explicitly to the intended compute output texture here.
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    // initialize timing for movement
    prevTime = glfwGetTime();
    // compute initial camera basis and cache it
    {
        float yaw = camera.Yaw;
        float pitch = camera.Pitch;
        float cy = cos(yaw);
        float sy = sin(yaw);
        float cp = cos(pitch);
        float sp = sin(pitch);
        glm::vec3 forward = glm::normalize(glm::vec3(cp * sy, sp, -cp * cy));
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
        if (glm::length(right) < 1e-6f) right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));
        float c = cos(camera.Roll);
        float s = sin(camera.Roll);
        glm::vec3 rightR = right * c - up * s;
        glm::vec3 upR = right * s + up * c;
        cachedCameraForward = forward;
        cachedCameraRight = rightR;
        cachedCameraUp = upR;
        cameraUniformsDirty = true;
    }
    // Now that GL is initialized, register framebuffer callback so we can reallocate the texture on resize
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int newW, int newH) {
        Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(w));
        if (r) r->OnFramebufferSize(newW, newH);
    });
}

void Renderer::InitGLFW() {
    if (!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Raytracer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
}

void Renderer::InitGL() {
    if (!gladLoadGL()) throw std::runtime_error("Failed to init GLAD");
}

void clearTextureUsingFbo(GLuint fbo, GLuint tex, int w, int h) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    GLenum draw = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &draw);
    glViewport(0,0,w,h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::CreateComputeShader() {
    computeProgram = LoadComputeShader("shaders/raytrace.comp");
    if (computeProgram == 0) {
        std::cerr << "Warning: compute shader failed to load. Uniforms will not be set." << std::endl;
        return;
    }
}

void Renderer::CreateFullScreenQuad() {
    // creation is handled by GLUtils::CreateFullscreenQuad in Init();
    (void)0;
}

void Renderer::UploadScene(const Scene& scene) {
    // (no debug logging)

    // copy required camera fields individually to avoid using a deleted Camera::operator=
    camera.position = scene.camera.position;
    camera.Yaw = scene.camera.Yaw;
    camera.Pitch = scene.camera.Pitch;
    camera.Roll = scene.camera.Roll;
    camera.FovDegrees = scene.camera.FovDegrees;
    camera.focalDist = scene.camera.focalDist;
    camera.diskSize = scene.camera.diskSize;

    // --- SSBO pro objekty ---
    glGenBuffers(1, &ssboObjects);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboObjects);
    glBufferData(GL_SHADER_STORAGE_BUFFER, scene.objects.size() * sizeof(Object),
                 scene.objects.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboObjects);

    // --- SSBO pro materiály ---
    glGenBuffers(1, &ssboMaterials);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboMaterials);


    glBufferData(GL_SHADER_STORAGE_BUFFER, scene.materials.size() * sizeof(Material),
                 scene.materials.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboMaterials);
    // store counts for shader
    uploadedObjectCount = static_cast<int>(scene.objects.size());
    uploadedMaterialCount = static_cast<int>(scene.materials.size());

    // compute tan(fov/2) on CPU and upload once (scene load)
    if (computeProgram != 0) {
        glUseProgram(computeProgram);
        float tanFov = tan(glm::radians(camera.FovDegrees) * 0.5f);
        SetUniform1f(computeProgram, "DiskSize", camera.diskSize);
        SetUniform1f(computeProgram, "FocalDist", camera.focalDist);
        SetUniform1f(computeProgram, "TanFov", tanFov);
    }
    // ensure camera uniforms get uploaded after a new scene is loaded
    cameraUniformsDirty = true;

    // unbind SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Renderer::AddScene(Scene* scene, bool makeCurrent) {
    if (!scene) return;
    scenes.push_back(scene);
    if (makeCurrent || curSceneIndex == -1) {
        curSceneIndex = static_cast<int>(scenes.size()) - 1;
        UploadScene(*scenes[curSceneIndex]);
        ResetAccumulation();
    }
}

void Renderer::NextScene() {
    if (scenes.empty()) return;
    curSceneIndex = (curSceneIndex + 1) % static_cast<int>(scenes.size());
    UploadScene(*scenes[curSceneIndex]);
    ResetAccumulation();
}

void Renderer::PrevScene() {
    if (scenes.empty()) return;
    curSceneIndex = (curSceneIndex - 1 + static_cast<int>(scenes.size())) % static_cast<int>(scenes.size());
    UploadScene(*scenes[curSceneIndex]);
    ResetAccumulation();
}

void ResetAccumulationIfNeeded(Renderer* r) {
    // Not used here; accumulation reset handled inside Update when movement detected
}

void Renderer::Update() {
    // Poll events first
    glfwPollEvents();

    // Close on Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    // compute delta time
    double currentTime = glfwGetTime();
    float dt = float(currentTime - prevTime);
    if (dt < 0.0f) dt = 0.0f;
    prevTime = currentTime;

    // Mouse look while right mouse button is held
    int mouseRightState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (mouseRightState == GLFW_PRESS) {
        // hide/lock cursor while rotating
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (firstMouse) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }
        double dx = xpos - lastMouseX;
        double dy = lastMouseY - ypos; // reversed: y ranges top->bottom
        lastMouseX = xpos;
        lastMouseY = ypos;

        camera.Yaw += float(dx) * mouseSensitivity;
        camera.Pitch += float(dy) * mouseSensitivity;
        // clamp pitch to avoid gimbal/flip
        const float pitchLimit = 1.55334303f; // ~89 degrees in radians
        if (camera.Pitch > pitchLimit) camera.Pitch = pitchLimit;
        if (camera.Pitch < -pitchLimit) camera.Pitch = -pitchLimit;
    } else {
        // release cursor
        if (!firstMouse) {
            firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    // Movement: WASD relative to the camera's rotation (forward includes pitch)
    float yaw = camera.Yaw;
    float pitch = camera.Pitch;
    float cy = cos(yaw);
    float sy = sin(yaw);
    float cp = cos(pitch);
    float sp = sin(pitch);
    glm::vec3 forward = glm::normalize(glm::vec3(cp * sy, sp, -cp * cy));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::vec3 moveDir(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir += right;

    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);
        camera.position += moveDir * moveSpeed * dt;
    }

    // detect camera movement/rotation to reset accumulation
    const float posEps = 1e-4f;
    bool moved = (glm::distance(camera.position, prevCameraPos) > posEps) ||
                 (fabs(camera.Yaw - prevCameraYaw) > 1e-5f) ||
                 (fabs(camera.Pitch - prevCameraPitch) > 1e-5f);
    if (moved) {
        // reset accumulation
        frameCount = 0; // next rendered frame will be treated as first (1)
        accPing = 0;
        // clear both acc textures
        clearTextureUsingFbo(accFbo, accTex[0], width, height);
        clearTextureUsingFbo(accFbo, accTex[1], width, height);
        // compute and cache camera basis for use in compute shader
        {
            float yaw = camera.Yaw;
            float pitch = camera.Pitch;
            float cy = cos(yaw);
            float sy = sin(yaw);
            float cp = cos(pitch);
            float sp = sin(pitch);
            glm::vec3 forward = glm::normalize(glm::vec3(cp * sy, sp, -cp * cy));
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
            if (glm::length(right) < 1e-6f) right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f)));
            glm::vec3 up = glm::normalize(glm::cross(right, forward));
            float c = cos(camera.Roll);
            float s = sin(camera.Roll);
            glm::vec3 rightR = right * c - up * s;
            glm::vec3 upR = right * s + up * c;
            cachedCameraForward = forward;
            cachedCameraRight = rightR;
            cachedCameraUp = upR;
        }
        // update stored camera state
        prevCameraPos = camera.position;
        prevCameraYaw = camera.Yaw;
        prevCameraPitch = camera.Pitch;
        // mark that GPU camera uniforms should be updated
        cameraUniformsDirty = true;
    }

    // Scene switching input (left/right arrows) - edge detect to avoid repeats
    int leftState = glfwGetKey(window, GLFW_KEY_LEFT);
    if (leftState == GLFW_PRESS && lastLeftState == GLFW_RELEASE) {
        PrevScene();
    }
    lastLeftState = leftState;

    int rightState = glfwGetKey(window, GLFW_KEY_RIGHT);
    if (rightState == GLFW_PRESS && lastRightState == GLFW_RELEASE) {
        NextScene();
    }
    lastRightState = rightState;
}

void Renderer::Render() {
    // Run compute shader
    glUseProgram(computeProgram);
    // upload camera uniforms to compute shader only when camera changed
    if (cameraUniformsDirty) {
        SetUniform3f(computeProgram, "CameraPos", camera.position);
        // upload cached camera basis (recomputed only when camera moves)
        SetUniform3f(computeProgram, "CameraForward", cachedCameraForward);
        SetUniform3f(computeProgram, "CameraRight", cachedCameraRight);
        SetUniform3f(computeProgram, "CameraUp", cachedCameraUp);
        cameraUniformsDirty = false;
    }

    // upload frame counter for RNG (use 1-based count for this frame)
    int useFrameCount = frameCount > 0 ? frameCount + 1 : 1;
    SetUniform1i(computeProgram, "FrameCount", useFrameCount);

    // debugging: tell compute shader to output material parameter (albedo.w)
    SetUniform1i(computeProgram, "DebugMode", 0);

    // upload object/material counts so shader doesn't iterate past valid entries
    SetUniform1i(computeProgram, "ObjectCount", uploadedObjectCount);
    SetUniform1i(computeProgram, "MaterialCount", uploadedMaterialCount);


    glDispatchCompute((width + 7) / 8, (height + 7) / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Accumulate: current compute output is in 'texture'
    // We'll write accumulated result into accTex[accPing] using previous accTex[1-accPing]
    int prev = 1 - accPing;
    // bind FBO and attach target accumulation texture
    glBindFramebuffer(GL_FRAMEBUFFER, accFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accTex[accPing], 0);
    GLenum draw = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &draw);
    glViewport(0, 0, width, height);

    // run accumulation shader
    if (accumProgram) glUseProgram(accumProgram);
    // set samplers
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture); // current frame
    // set sampler uniforms using helper functions
    SetUniform1i(accumProgram, "currTex", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, accTex[prev]); // previous accumulation
    SetUniform1i(accumProgram, "prevAccTex", 1);
    // pass frame count (1-based)
    SetUniform1i(accumProgram, "FrameCount", useFrameCount);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // unbind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Now display the accumulated texture (accTex[accPing]) to default framebuffer
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
    if (graphicsProgram) glUseProgram(graphicsProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, accTex[accPing]);
    // graphicsProgram expects uniform 'resultImage' at unit 0 (set in Init())
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // advance ping and frame counter
    accPing = 1 - accPing;
    frameCount++;

    glfwSwapBuffers(window);

    // Update window title with current accumulation frame count so user can see
    // how many frames have been averaged. Use at least 1 for display.
    int displayFrames = frameCount > 0 ? frameCount : 1;
    std::string sceneName = scenes[curSceneIndex]->name;
    std::string title = std::string("Raytracer - ") + sceneName + 
                        std::string(" - frames: ") + std::to_string(displayFrames);
    if (window) glfwSetWindowTitle(window, title.c_str());
}

bool Renderer::ShouldClose() const {
    return glfwWindowShouldClose(window);
}

GLFWwindow* Renderer::GetWindow() {
    return window;
}

void Renderer::ResetAccumulation() {
    // reset counters
    frameCount = 0;
    accPing = 0;
    // clear accumulation textures
    if (accFbo != 0) {
        if (accTex[0] != 0) clearTextureUsingFbo(accFbo, accTex[0], width, height);
        if (accTex[1] != 0) clearTextureUsingFbo(accFbo, accTex[1], width, height);
    }
    // mark camera uniforms dirty so they are uploaded for the new scene
    cameraUniformsDirty = true;
}

void Renderer::Shutdown() {
    glfwTerminate();
}

void Renderer::OnFramebufferSize(int newWidth, int newHeight) {
    // update stored size
    width = newWidth;
    height = newHeight;

    // update GL viewport (requires a current context)
    glViewport(0, 0, width, height);
    
    // If texture already exists, reallocate storage for the new size and rebind image unit
    if (texture != 0) {
        ResizeOutputTexture(texture, width, height);
    }
    // resize accumulation textures as well
    if (accTex[0] != 0) ResizeOutputTexture(accTex[0], width, height);
    if (accTex[1] != 0) ResizeOutputTexture(accTex[1], width, height);
    // Rebind compute image unit to the compute output texture to be safe
    if (texture != 0) {
        glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    }
    // Clear accumulation textures and reset accumulation state so prumerovani restarts
    if (accFbo != 0) {
        if (accTex[0] != 0) clearTextureUsingFbo(accFbo, accTex[0], width, height);
        if (accTex[1] != 0) clearTextureUsingFbo(accFbo, accTex[1], width, height);
    }
    // reset accumulation counters
    frameCount = 0;
    accPing = 0;
    
}
