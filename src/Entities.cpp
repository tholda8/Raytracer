#include "Entities.h"




void Scene::UploadToGPU() {
    // vytvoříš SSBO (Shader Storage Buffer Object) pro objekty a materiály
    // Objects
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, objectBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        objects.size() * sizeof(Object), objects.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, objectBuffer);

    // Materials
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
         materials.size() * sizeof(Material), materials.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, materialBuffer);

    // Unbind for cleanliness
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}



namespace Create{
    Material createDiffuseMaterial(const glm::vec3& albedo) {
        Material mat;
        mat.type = MATERIAL_TYPE_DIFFUSE;
        mat.albedo = glm::vec4(albedo, 0.0f); // pack parameter in w (unused)
        return mat;
    }  


    Material createMetallicMaterial(const glm::vec3& albedo, float roughness) {
        Material mat;
        mat.type = MATERIAL_TYPE_METALLIC;
        // pack roughness into albedo.w
        mat.albedo = glm::vec4(albedo, roughness);
        return mat;
    }

    Material createDielectricMaterial(const glm::vec3& albedo, float refractiveIndex) {
        Material mat;
        mat.type = MATERIAL_TYPE_DIELECTRIC;
        // pack refractive index into albedo.w
        mat.albedo = glm::vec4(albedo, refractiveIndex);
        return mat;
    }

    Material createEmissiveMaterial(const glm::vec3& color, float emissivity) {
        Material mat;
        mat.type = MATERIAL_TYPE_DIFFUSE;
        mat.emissivity = emissivity;
        mat.albedo = glm::vec4(color, 0.0f);
        return mat;
    }

    Object createSphere(const glm::vec3& center, float radius, unsigned int materialId) {
        Object obj;
        obj.type = OBJECT_TYPE_SPHERE;
        obj.materialId = materialId;
        obj.pad0 = 0;
        obj.instanceAngle = 0.0f;
        obj.pos = glm::vec4(center, 0.0f);
        obj.parameter1 = glm::vec4(radius, 0.0f, 0.0f, 0.0f);
        obj.parameter2 = glm::vec4(0.0f);
        return obj;
    }

    Object createQuad(const glm::vec3& pos, const glm::vec3& u, const glm::vec3& v, unsigned int materialId) {
        Object obj;
        obj.type = OBJECT_TYPE_QUAD;
        obj.materialId = materialId;
        obj.pad0 = 0;
        obj.instanceAngle = 0.0f;
        obj.pos = glm::vec4(pos, 0.0f);
        obj.parameter1 = glm::vec4(u, 0.0f); // store vector u in parameter1
        obj.parameter2 = glm::vec4(v, 0.0f); // store vector v in parameter2
        return obj;
    }

    Object createBox(const glm::vec3& minCorner, const glm::vec3& size, unsigned int materialId) {
        Object obj;
        obj.type = OBJECT_TYPE_BOX;
        obj.materialId = materialId;
        obj.pad0 = 0;
        obj.instanceAngle = 0.0f;
        obj.pos = glm::vec4(minCorner, 0.0f);
        obj.parameter1 = glm::vec4(size, 0.0f); // store size in parameter1
        obj.parameter2 = glm::vec4(0.0f);
        return obj;
    }


    Object createCylinder(const glm::vec3& bottomCenter, const float radius, const float height, unsigned int materialId) {
        Object obj;
        obj.type = OBJECT_TYPE_CYLINDER;
        obj.materialId = materialId;
        obj.pad0 = 0;
        obj.instanceAngle = 0.0f;
        obj.pos = glm::vec4(bottomCenter, 0.0f);
        obj.parameter1 = glm::vec4(radius, height, 0.0f, 0.0f); // store radius and height
        obj.parameter2 = glm::vec4(0.0f);
        return obj;
    }
}

// Scene constructor/destructor: generate and delete GL buffers
Scene::Scene() {
    objectBuffer = 0;
    materialBuffer = 0;
    glGenBuffers(1, &objectBuffer);
    glGenBuffers(1, &materialBuffer);
    // sensible defaults for camera so UploadScene doesn't copy uninitialized values
    camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.Yaw = 0.0f;
    camera.Pitch = 0.0f;
    camera.Roll = 0.0f;
}

Scene::~Scene() {
    if (objectBuffer) glDeleteBuffers(1, &objectBuffer);
    if (materialBuffer) glDeleteBuffers(1, &materialBuffer);
}