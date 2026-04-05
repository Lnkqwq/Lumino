#include "core/Application.h"
#include "core/Window.h"
#include "core/Renderer.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Camera.h"
#include "components/PlayerController.h"
#include "components/ThirdPersonCamera.h"
#include "resource/glTFLoader.h"
#include <iostream>

int main() {
    Application app;
    Scene* scene = app.GetScene();
    if (!scene) {
        std::cerr << "Failed to get scene!" << std::endl;
        return -1;
    }

    // 1. 加载盒子模型（放在右侧偏上位置）
    Entity* boxEntity = glTFLoader::Load("assets/BoxAnimated.glb", scene);
    if (boxEntity) {
        boxEntity->GetTransform()->SetPosition(glm::vec3(5.0f, 2.0f, 3.0f));
        std::cout << "Box loaded at (5,2,3)" << std::endl;
    } else {
        std::cerr << "Failed to load box model!" << std::endl;
    }

    // 2. 加载小人模型（玩家，放在原点）
    Entity* playerEntity = glTFLoader::Load("assets/character.glb", scene);
    if (!playerEntity) {
        std::cerr << "Failed to load character model, using box instead" << std::endl;
        playerEntity = glTFLoader::Load("assets/BoxAnimated.glb", scene);
    }
    if (playerEntity) {
        // 修正小人倒立：绕 X 轴旋转 180 度
        glm::quat rotZ = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::quat rotX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat fixRotation = rotX * rotZ;  // 注意顺序：先应用 rotZ，再 rotX
        playerEntity->GetTransform()->SetRotation(fixRotation);
        playerEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        playerEntity->AddComponent<PlayerController>();
        std::cout << "Character loaded at (0,0,0) and rotated upright" << std::endl;
    } else {
        std::cerr << "No player model available!" << std::endl;
        return -1;
    }

    // 3. 创建相机
    Entity* cameraEntity = scene->CreateEntity("MainCamera");
    Camera* camera = cameraEntity->AddComponent<Camera>();
    camera->SetPerspective(60.0f, 0.1f, 100.0f);
    scene->SetMainCamera(camera);

    // 4. 第三人称相机跟随玩家
    auto thirdPerson = cameraEntity->AddComponent<ThirdPersonCamera>();
    thirdPerson->SetTarget(playerEntity);
    thirdPerson->SetDistance(5.0f);
    thirdPerson->SetSensitivity(0.1f);
    std::cout << "ThirdPersonCamera attached, target = character" << std::endl;

    // 可选：手动设置相机初始位置（组件会覆盖，但可用于初始视角）
    cameraEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 2.0f, 5.0f));

    app.Run();
    return 0;
}