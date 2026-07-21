#include "Renderer.h"
#include "Entities.h"
#include "Scene.cpp"  // Include the scene definition
#include <iostream>
int main() {
    Renderer renderer(1440, 720);
    renderer.Init();

    // Create scenes (Scene1 and Scene2 should be defined in Scene.cpp)
    Scene1 scene1;
    scene1.Load();
    Scene2 scene2;
    scene2.Load();
    Scene3 scene3;
    scene3.Load();

    // Register scenes with the renderer; make the first added the active scene
    renderer.AddScene(&scene1, true);
    renderer.AddScene(&scene2, false);
    renderer.AddScene(&scene3, false);

    while (!renderer.ShouldClose()) {
        renderer.Update();
        renderer.Render();
    }

    renderer.Shutdown();
    return 0;
}