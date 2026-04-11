#include "core/Application.h"
#include "core/Window.h"
#include "core/Renderer.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Camera.h"
#include "components/PlayerController.h"
#include "components/ThirdPersonCamera.h"
#include "resource/glTFLoader.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "scene/MeshRenderer.h"
#include <iostream>

// 辅助函数：创建一个平面网格（用于地面）
Entity* CreatePlane(Scene* scene, float width, float depth, const glm::vec3& color, float yOffset = 0.0f) 
{
    Entity* planeEntity = scene->CreateEntity("GroundPlane");
    planeEntity->GetTransform()->SetPosition(glm::vec3(0.0f, yOffset, 0.0f));

    float halfW = width / 2.0f;
    float halfD = depth / 2.0f;
    std::vector<Vertex> vertices = 
    {
        {{-halfW, 0.0f, -halfD}, {0,1,0}, {0,0}},
        {{ halfW, 0.0f, -halfD}, {0,1,0}, {1,0}},
        {{ halfW, 0.0f,  halfD}, {0,1,0}, {1,1}},
        {{-halfW, 0.0f,  halfD}, {0,1,0}, {0,1}}
    };
    std::vector<unsigned int> indices = {0,1,2, 0,2,3};
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    auto material = std::make_shared<Material>();
    material->SetBaseColor(color);
    auto renderer = planeEntity->AddComponent<MeshRenderer>();
    renderer->SetMesh(mesh);
    renderer->SetMaterial(material);
    return planeEntity;
}

int main() 
{
    Application app;
    Scene* scene = app.GetScene();
    if (!scene) 
    {
        std::cerr << "Failed to get scene!" << std::endl;
        return -1;
    }

    // 1. 加载盒子模型（静态参照物，但不再播放动画，位置可设置）
    Entity* boxEntity = glTFLoader::Load("assets/BoxAnimated.glb", scene);
    if (boxEntity) 
    {
        // 盒子模型可能有多个网格节点，全部移动到 (5,2,3)
        for (auto& e : scene->GetAllEntities()) 
        {
            if (e->GetComponent<MeshRenderer>()) 
            {
                e->GetTransform()->SetPosition(glm::vec3(5.0f, 2.0f, 3.0f));
            }
        }
        std::cout << "Box loaded and moved to (5,2,3)" << std::endl;
    }

    // 2. 加载小人模型（玩家）
    Entity* playerEntity = glTFLoader::Load("assets/character.glb", scene);
    if (!playerEntity) 
    {
        std::cerr << "Failed to load character model!" << std::endl;
        return -1;
    }
    // 修正小人朝向（根据你的需求）
    playerEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
    auto playerCtrl = playerEntity->AddComponent<PlayerController>();
    std::cout << "Character loaded at (0,0,0)" << std::endl;

    // 3. 创建地面（位于 Y = -0.5，上表面在 Y=0）
    CreatePlane(scene, 30.0f, 30.0f, glm::vec3(0.4f, 0.7f, 0.4f), -0.5f);
    std::cout << "Ground created" << std::endl;

    // 4. 设置玩家控制器参数
    if (playerCtrl) 
    {
        playerCtrl->SetMoveSpeed(5.0f);
        playerCtrl->SetGroundHeight(0.0f);   // 地面高度
    }

    // 5. 创建相机实体
    Entity* cameraEntity = scene->CreateEntity("MainCamera");
    Camera* camera = cameraEntity->AddComponent<Camera>();
    camera->SetPerspective(60.0f, 0.1f, 100.0f);
    scene->SetMainCamera(camera);

    // 6. 添加第三人称相机组件，跟随玩家
    auto thirdPerson = cameraEntity->AddComponent<ThirdPersonCamera>();
    thirdPerson->SetTarget(playerEntity);
    thirdPerson->SetDistance(5.0f);
    thirdPerson->SetSensitivity(0.1f);
    std::cout << "ThirdPersonCamera attached" << std::endl;

    // 7. 将相机传递给玩家控制器（用于移动方向计算）
    if (playerCtrl) 
    {
        playerCtrl->SetCamera(thirdPerson);
    }

    app.Run();
    return 0;
}