#include "PlayerController.h"
#include "input/Input.h"
#include "scene/Transform.h"
#include "scene/Entity.h"
#include <GLFW/glfw3.h>
#include <iostream>

PlayerController::PlayerController() : m_moveSpeed(2.0f) {}

void PlayerController::Update(float deltaTime) {
    static int frame = 0;
    frame++;
    if (frame % 60 == 0) {
        std::cout << "PlayerController Update called, deltaTime=" << deltaTime << std::endl;
    }

    Transform* transform = GetEntity()->GetTransform();
    if (!transform) return;

    // 打印按键状态（每60帧）
    if (frame % 60 == 0) {
        bool w = Input::GetKey(GLFW_KEY_W);
        bool s = Input::GetKey(GLFW_KEY_S);
        bool a = Input::GetKey(GLFW_KEY_A);
        bool d = Input::GetKey(GLFW_KEY_D);
        std::cout << "W=" << w << " S=" << s << " A=" << a << " D=" << d << std::endl;
    }

    glm::vec3 moveDir(0.0f);
    if (Input::GetKey(GLFW_KEY_W)) moveDir.z += 1.0f;
    if (Input::GetKey(GLFW_KEY_S)) moveDir.z -= 1.0f;
    if (Input::GetKey(GLFW_KEY_A)) moveDir.x += 1.0f;
    if (Input::GetKey(GLFW_KEY_D)) moveDir.x -= 1.0f;
    if (moveDir != glm::vec3(0.0f)) moveDir = glm::normalize(moveDir);

    glm::vec3 oldPos = transform->GetPosition();
    glm::vec3 newPos = oldPos;
    newPos.x += moveDir.x * m_moveSpeed * deltaTime;
    newPos.z += moveDir.z * m_moveSpeed * deltaTime;

    // 无条件打印每帧位置（或每60帧）
    if (frame % 60 == 0) {
        std::cout << "Player position: old=(" << oldPos.x << "," << oldPos.y << "," << oldPos.z
                  << ") new=(" << newPos.x << "," << newPos.y << "," << newPos.z << ")" << std::endl;
    }

    transform->SetPosition(newPos);
}