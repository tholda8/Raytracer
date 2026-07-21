#include "Entities.h"






class Scene1:public Scene
{
public:
    void Load() override {
        name = "Some objects";
        camera.position = glm::vec3(0.0f, 0.0f, 4.5f);

        //materials
        Material whiteDiffuse = Create::createDiffuseMaterial(glm::vec3(0.9f, 0.9f, 0.9f));
        materials.push_back(whiteDiffuse);
        Material redDiffuse = Create::createDiffuseMaterial(glm::vec3(0.8f, 0.1f, 0.1f));
        materials.push_back(redDiffuse);
        Material greenDiffuse = Create::createDiffuseMaterial(glm::vec3(0.1f, 0.8f, 0.1f));
        materials.push_back(greenDiffuse);
        Material emissiveMat = Create::createEmissiveMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 10.0f);
        materials.push_back(emissiveMat);
        Material metallicMat = Create::createMetallicMaterial(glm::vec3(0.3f, 0.3f, 0.8f), 0.05f);
        materials.push_back(metallicMat);
        Material dielectricMat = Create::createDielectricMaterial(glm::vec3(0.9f), 1.5f);
        materials.push_back(dielectricMat);
        Material pinkDiffuse = Create::createDiffuseMaterial(glm::vec3(0.8f, 0.1f, 0.8f));
        materials.push_back(pinkDiffuse);
        //objects
        float roomSize = 5.0f;
        // Flip the room along the Z axis so it is in front of a camera looking down -Z
        Object leftWall = Create::createQuad(glm::vec3(-roomSize/2.0f, -roomSize/2.0f, 0.0f),
                             glm::vec3(0.0f, 0.0f, -roomSize),
                             glm::vec3(0.0f, roomSize, 0.0f),
                             1);
        objects.push_back(leftWall);
        Object rightWall = Create::createQuad(glm::vec3(roomSize/2.0f, -roomSize/2.0f, 0.0f),
                              glm::vec3(0.0f, 0.0f, -roomSize),
                              glm::vec3(0.0f, roomSize, 0.0f),
                              2);
        objects.push_back(rightWall);

        Object backWall = Create::createQuad(glm::vec3(-roomSize/2.0f, -roomSize/2.0f, -roomSize),
                             glm::vec3(roomSize, 0.0f, 0.0f),
                             glm::vec3(0.0f, roomSize, 0.0f),
                             0);
        objects.push_back(backWall);

        Object roof = Create::createQuad(glm::vec3(-roomSize/2.0f, roomSize/2.0f, 0.0f),
                         glm::vec3(roomSize, 0.0f, 0.0f),
                         glm::vec3(0.0f, 0.0f, -roomSize),
                         0);
        objects.push_back(roof);

        Object floor = Create::createQuad(glm::vec3(-roomSize/2.0f, -roomSize/2.0f, 0.0f),
                          glm::vec3(roomSize, 0.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -roomSize),
                          0);
        objects.push_back(floor);

        float lightSize = 1.0f;
        Object light = Create::createQuad(
            glm::vec3(-lightSize/2.0f, roomSize/2.0f - 0.01f, -roomSize/2.0f + lightSize/2.0f),
                          glm::vec3(lightSize, 0.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -lightSize),
                          3);
        objects.push_back(light);
        
        Object sphere1 = Create::createSphere(glm::vec3(1.0f, -roomSize/2.0f + 1.0f, -4.0f), 1.0f, 4);
        objects.push_back(sphere1);
        
        Object sphere2 = Create::createSphere(glm::vec3(0.75f, -roomSize/2.0f + 0.65f, -1.0f), 0.65f, 5);
        objects.push_back(sphere2);

        Object cube1 = Create::createBox(glm::vec3(0.0f, -roomSize/2.0f, -4.0f), 
                                         glm::vec3(1.25f,2.0f,1.25f), 6);
        cube1.instanceAngle = glm::radians(25.0f);
        objects.push_back(cube1);

        Object cylinder1 = Create::createCylinder(glm::vec3(-0.75f, -roomSize/2.0f, -1.0f), 0.75f, 1.25f, 5);
        objects.push_back(cylinder1);

    }
};



class Scene2:public Scene
{
public:
    void Load() override {
        name = "Empty room";
        camera.position = glm::vec3(0.0f, 0.0f, 4.5f);

        //materials
        Material whiteDiffuse = Create::createDiffuseMaterial(glm::vec3(0.9f, 0.9f, 0.9f));
        materials.push_back(whiteDiffuse);
        Material redDiffuse = Create::createDiffuseMaterial(glm::vec3(0.8f, 0.1f, 0.1f));
        materials.push_back(redDiffuse);
        Material greenDiffuse = Create::createDiffuseMaterial(glm::vec3(0.1f, 0.8f, 0.1f));
        materials.push_back(greenDiffuse);
        Material emissiveMat = Create::createEmissiveMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 3.0f);
        materials.push_back(emissiveMat);
        //objects
        float roomSize = 5.0f;
        // Flip the room along the Z axis so it is in front of a camera looking down -Z
        Object leftWall = Create::createQuad(glm::vec3(-roomSize/2.0f, -roomSize/2.0f, 0.0f),
                             glm::vec3(0.0f, 0.0f, -roomSize),
                             glm::vec3(0.0f, roomSize, 0.0f),
                             1);
        objects.push_back(leftWall);
        Object rightWall = Create::createQuad(glm::vec3(roomSize/2.0f, -roomSize/2.0f, 0.0f),
                              glm::vec3(0.0f, 0.0f, -roomSize),
                              glm::vec3(0.0f, roomSize, 0.0f),
                              2);
        objects.push_back(rightWall);

        Object backWall = Create::createQuad(glm::vec3(-roomSize/2.0f, -roomSize/2.0f, -roomSize),
                             glm::vec3(roomSize, 0.0f, 0.0f),
                             glm::vec3(0.0f, roomSize, 0.0f),
                             0);
        objects.push_back(backWall);

        Object roof = Create::createQuad(glm::vec3(-roomSize/2.0f, roomSize/2.0f, 0.0f),
                         glm::vec3(roomSize, 0.0f, 0.0f),
                         glm::vec3(0.0f, 0.0f, -roomSize),
                         0);
        objects.push_back(roof);

        Object floor = Create::createQuad(glm::vec3(-roomSize/2.0f, -roomSize/2.0f, 0.0f),
                          glm::vec3(roomSize, 0.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -roomSize),
                          0);
        objects.push_back(floor);

        float lightSize = 2.0f;
        Object light = Create::createQuad(
            glm::vec3(-lightSize/2.0f, roomSize/2.0f - 0.01f, -roomSize/2.0f + lightSize/2.0f),
                          glm::vec3(lightSize, 0.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -lightSize),
                          3);
        objects.push_back(light);
    }
};




class Scene3:public Scene
{
public:
    void Load() override {
        name = "room1";
        camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
        camera.diskSize = 0.2f;
        camera.focalDist = 13.0f;
        //materials
        Material whiteDiffuse = Create::createDiffuseMaterial(glm::vec3(0.9f, 0.9f, 0.9f));
        materials.push_back(whiteDiffuse);
        Material redDiffuse = Create::createDiffuseMaterial(glm::vec3(0.8f, 0.1f, 0.1f));
        materials.push_back(redDiffuse);
        Material greenDiffuse = Create::createDiffuseMaterial(glm::vec3(0.1f, 0.8f, 0.1f));
        materials.push_back(greenDiffuse);
        Material emissiveMat = Create::createEmissiveMaterial(glm::vec3(0.7f, 0.2f, 0.2f), 1.1f);
        materials.push_back(emissiveMat);
        Material emissiveMat1 = Create::createEmissiveMaterial(glm::vec3(0.9f, 0.9f, 0.9f), 3.0f);
        materials.push_back(emissiveMat1);
        Material emissiveMat2 = Create::createEmissiveMaterial(glm::vec3(0.2f, 0.2f, 0.7f), 0.8f);
        materials.push_back(emissiveMat2);
        Material dielectricMat = Create::createDielectricMaterial(glm::vec3(0.9f), 1.5f);
        materials.push_back(dielectricMat);
        Material emissiveMat3 = Create::createEmissiveMaterial(glm::vec3(0.6f, 0.6f, 0.2f), 0.4f);
        materials.push_back(emissiveMat3);
        Material mirr = Create::createMetallicMaterial(glm::vec3(0.6f, 0.6f, 0.2f), 0.4f);
        materials.push_back(mirr);
        //objects
        // Flip the room along the Z axis so it is in front of a camera looking down -Z

        Object floor = Create::createQuad(glm::vec3(-100.0f, -5.0f, -0.0f),
                          glm::vec3(200.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -200.0f),
                          0);
        objects.push_back(floor);
        float lightSize = 2.0f;
        Object light = Create::createQuad(
            glm::vec3(-lightSize/2.0f, 20.0f/2.0f - 0.01f, -20.0f/2.0f + lightSize/2.0f),
                          glm::vec3(lightSize, 0.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -lightSize),
                          3);
        //objects.push_back(light);

        Object ball = Create::createSphere(glm::vec3(0.0f, -4.0f, -10.0f), 1.0f, 3);
        objects.push_back(ball);
        Object bal4 = Create::createSphere(glm::vec3(1.0f, -4.0f, -17.0f), 1.0f, 4);
        objects.push_back(bal4);
        Object ball1 = Create::createSphere(glm::vec3(-2.0f, -1.0f, -13.0f), 1.0f, 2);
        objects.push_back(ball1);
        Object ball2 = Create::createSphere(glm::vec3(3.0f, 1.0f, -18.0f), 2.0f, 8);
        objects.push_back(ball2);
        Object cyl = Create::createCylinder(glm::vec3(2.0f, -5.0f, -7.0f), 1.0f, 3.0f, 6);
        objects.push_back(cyl);

        Object box = Create::createBox(glm::vec3(-2.0f, -4.0f, -7.0f), glm::vec3(-2.0f, -3.0f, -2.0f), 6);
        objects.push_back(box);
        Object ball3 = Create::createSphere(glm::vec3(0.0f, 2.0f, -50.0f), 5.0f, 7);
        objects.push_back(ball3);

    }
};