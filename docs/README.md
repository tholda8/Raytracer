# Documentation

This directory contains a short technical overview of the raytracer and the way the current codebase is organized.

## Rendering Pipeline

The application uses a simple three-stage pipeline:

1. Scene data is created on the CPU in `src/Scene.cpp`.
2. Scene objects and materials are uploaded to the GPU through SSBOs in `Renderer::UploadScene`.
3. The compute shader in `shaders/raytrace.comp` traces rays into a floating-point output texture, which is then displayed by a fullscreen pass.

The final image is accumulated over multiple frames. When the camera moves or the active scene changes, the accumulation buffers are cleared so the progressive result starts over.

## Core Files

- `src/main.cpp` - application entry point and scene registration
- `src/Renderer.cpp` - window creation, input handling, scene upload, rendering, and accumulation
- `src/Scene.cpp` - built-in scene definitions
- `src/Entities.h` / `src/Entities.cpp` - scene, camera, object, and material data structures
- `src/Shader.cpp` - shader loading and compilation
- `src/GLUtils.cpp` - helper functions for textures, fullscreen geometry, and uniforms
- `shaders/raytrace.comp` - compute shader that performs the ray tracing work
- `shaders/fullscreen.vert` / `shaders/fullscreen.frag` - fullscreen presentation pass
- `shaders/accum.frag` - accumulation pass for progressive refinement

## Camera Data

Each scene owns a `Camera` instance. When a scene is activated, the renderer copies these values into its runtime camera state:

- position and orientation
- field of view
- focal distance
- lens / disk size

If a scene does not override one of these values, the defaults from `Camera` are used.

## Scene Switching

Scenes are added in `src/main.cpp` and can be switched at runtime with the left and right arrow keys.

Switching scenes does the following:

- uploads the new scene's SSBO data
- resets accumulation
- refreshes camera uniforms
- updates the window title with the active scene name

## Input Model

The renderer uses a minimal input model:

- `W`, `A`, `S`, `D` move the camera relative to its current orientation
- Right mouse button enables look-around mode
- `Esc` closes the window

## Runtime Notes

- The executable expects to be launched from the repository root because shader paths are relative.
- There is no immediate-mode UI layer in the project.
- The codebase is intentionally lightweight and does not use an external asset pipeline.

## Extending The Project

To add a new scene:

1. Define a new `Scene` subclass in `src/Scene.cpp`.
2. Fill `objects`, `materials`, and camera settings in `Load()`.
3. Instantiate the scene in `src/main.cpp`.
4. Register it with `renderer.AddScene(...)`.

To adjust the renderer behavior, the main entry points are `src/Renderer.cpp` and `shaders/raytrace.comp`.
