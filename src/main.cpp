#include "core/Application.h"
#include "core/Window.h"
#include "core/Renderer.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Camera.h"
#include "components/PlayerController.h"
#include "components/ThirdPersonCamera.h"
#include "resource/glTFLoader.h"
#include "input/Input.h"
#include <iostream>

int main() {
    Application app;
    Scene* scene = app.GetScene();
    if (!scene) {
        std::cerr << "Failed to get scene!" << std::endl;
        return -1;
    }

    // 加载玩家模型
    Entity* playerEntity = glTFLoader::Load("assets/character.glb", scene);
    if (!playerEntity) {
        std::cerr << "Failed to load model!" << std::endl;
        return -1;
    }
    playerEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    playerEntity->AddComponent<PlayerController>();

    // 创建相机实体
    Entity* cameraEntity = scene->CreateEntity("MainCamera");
    Camera* camera = cameraEntity->AddComponent<Camera>();
    camera->SetPerspective(60.0f, 0.1f, 100.0f);
    scene->SetMainCamera(camera);

    // 添加第三人称相机控制组件
    auto thirdPerson = cameraEntity->AddComponent<ThirdPersonCamera>();
    thirdPerson->SetTarget(playerEntity);
    thirdPerson->SetDistance(5.0f);
    thirdPerson->SetSensitivity(0.1f);

    // 可选：设置相机初始位置（组件会覆盖）
    cameraEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 2.0f, 5.0f));

    app.Run();
    return 0;
}