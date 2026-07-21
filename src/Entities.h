#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <cstdint>
#include <iostream>

#define MATERIAL_TYPE_DIFFUSE 0
#define MATERIAL_TYPE_METALLIC 1
#define MATERIAL_TYPE_DIELECTRIC 2

#define OBJECT_TYPE_SPHERE 0
#define OBJECT_TYPE_QUAD 1
#define OBJECT_TYPE_BOX 2
#define OBJECT_TYPE_CYLINDER 3
#define OBJECT_TYPE_PLANE 4



struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitRecord {
    float t;
    glm::vec3 point;
    glm::vec3 normal;
    unsigned int materialId;
    bool hit;
    bool frontFace;
};


class Material {
public:
    uint32_t type; // 0: diffuse, 1: metallic
    float emissivity = 0.0f;
    uint32_t pad1;
    uint32_t pad2;
    // Use vec4 so the layout matches std430 expectations (16-byte alignment).
    // We pack the per-material parameter (roughness) into albedo.w for simplicity.
    glm::vec4 albedo; // xyz = color, w = parameter
};

class Object {
public:
    uint32_t type; // 0: sphere, 1: plane
    uint32_t materialId;
    // pad to 16 bytes so struct matches std430 layout in shader
    uint32_t pad0;
    float instanceAngle;
    glm::vec4 pos; // xyz = position, w unused
    glm::vec4 parameter1; // e.g. radius in .x for sphere
    glm::vec4 parameter2;
    glm::vec4 instancePos = glm::vec4(0.0f); // default no instance offset
    glm::vec4 instanceRotAxis = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f); // default rotation axis
};

class Camera {
public:
    glm::vec3 position;
    float Yaw;   // radians
    float Pitch; // radians
    float Roll;  // radians
    float FovDegrees = 60.0f; // field of view in degrees (used to compute tanFov on CPU)
    const float viewportWidth = 4.0f;
    const float viewportHeight = 2.0f;
    float focalDist = 10.0f;
    float diskSize = 0.01f;
};

class Scene {

public:
    // Declare destructor; definition is provided in the .cpp to manage GL resources
    virtual ~Scene();

    virtual void Load() = 0;      // naplní objekty a materiály
    virtual void Update(float dt) {}  // volitelný update (animace, pohyb kamery)
    void UploadToGPU();   // pošle data do shaderu
    Scene();

    std::string name;
    std::vector<Object> objects;
    std::vector<Material> materials;
    glm::vec3 backgroundColor;
    Camera camera;
private:
    GLuint objectBuffer, materialBuffer;
};



namespace Create{
    Material createDiffuseMaterial(const glm::vec3& albedo);
    Material createMetallicMaterial(const glm::vec3& albedo, float roughness);
    Material createDielectricMaterial(const glm::vec3& albedo, float refractiveIndex);
    Material createEmissiveMaterial(const glm::vec3& color, float emissivity);

    Object createSphere(const glm::vec3& center, float radius, unsigned int materialId);
    Object createQuad(const glm::vec3& pos, const glm::vec3& u, const glm::vec3& v, unsigned int materialId);
    Object createBox(const glm::vec3& minCorner, const glm::vec3& size, unsigned int materialId);
    Object createCylinder(const glm::vec3& bottomCenter, const float radius, const float height, unsigned int materialId);
}