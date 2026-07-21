# Raytracer
This raytracer simulates the physical behavior of light bouncing around a 3D scene. By tracing rays from the camera through the pixels and into the scene, it computes accurate lighting, shadows, and material interactions.
Done with [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

<img width="1339" height="1340" alt="Snímek obrazovky 2026-07-21 211650" src="https://github.com/user-attachments/assets/c4b77b10-9684-4dc7-8e83-0ec4235237d2" />
<img width="1641" height="1295" alt="Snímek obrazovky 2026-07-21 225537" src="https://github.com/user-attachments/assets/98599823-e51c-4814-9139-7a0f5ff69c72" />

# RaytracerCompute

GPU path tracer / raytracer built with C++, OpenGL 4.6, GLFW, GLAD, and GLSL compute shaders.

The renderer uses a compute shader for ray tracing, then displays the result through a fullscreen pass. It supports multiple scenes, camera movement, accumulation across frames, and simple depth-of-field parameters.

## Features

- Compute-shader based ray tracing
- Scene switching with multiple built-in scenes
- Camera movement with mouse look and WASD controls
- Frame accumulation for progressive refinement
- Fullscreen presentation pass with gamma correction
- Scene upload via SSBOs for objects and materials

## Project Structure

- `src/` - C++ source files for renderer, scene setup, shaders, and OpenGL helpers
- `shaders/` - GLSL shaders used by the compute and fullscreen pipeline
- `external/` - vendored third-party libraries (`glfw`, `glad`, `glm`)
- `build/` - generated CMake build output

## Requirements

- Windows
- CMake 3.15 or newer
- A C++17 compiler
- OpenGL-capable GPU with driver support for OpenGL 4.6

All required libraries are already included in the repository under `external/`.

## Build

From the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The executable is generated at:

```text
build/Release/RaytracerCompute.exe
```

## Run

Run the executable from the repository root so the shader paths resolve correctly:

```powershell
cd "C:\Users\thold\Desktop\OpenGLproj\Raytracer"
.\build\Release\RaytracerCompute.exe
```

The application loads shaders from relative paths such as `shaders/raytrace.comp`, so starting it from another working directory may fail unless you copy the shader folder or adjust the runtime paths.

## Controls

- `W`, `A`, `S`, `D` - move the camera
- Right mouse button + drag - look around
- `Left Arrow` - previous scene
- `Right Arrow` - next scene
- `Esc` - close the application

## Scenes

The built-in scenes are defined in `src/Scene.cpp`:

- `Scene1` - a room with multiple objects
- `Scene2` - a simpler empty room
- `Scene3` - an experimental scene with a large scale layout

Scenes are loaded in `src/main.cpp` and registered with the renderer at startup.

## Notes

- The renderer accumulates frames over time, so the image becomes cleaner the longer the camera stays still.
- When the camera moves or the scene changes, accumulation is reset.
- Camera defaults such as field of view, focus distance, and lens size are copied from the active scene into the renderer when a scene is uploaded.
- There is currently no ImGui-based UI.
- See [docs/README.md](docs/README.md) for a more detailed technical overview.

## Troubleshooting

- If the window opens but the image is black, make sure the executable is launched from the repository root so the shader files can be found.
- If shader compilation fails, check the console output for GLSL errors.
- If you change scene geometry and see no update, make sure the scene is added in `src/main.cpp` and its objects are pushed into the `objects` vector.

## License

No explicit license file is included yet.

