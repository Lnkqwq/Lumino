#include "PlayerController.h"
#include "input/Input.h"
#include "scene/Transform.h"
#include "scene/Entity.h"
#include <GLFW/glfw3.h>
#include <iostream>

PlayerController::PlayerController() : m_moveSpeed(2.0f) {}

 void PlayerController::Update(float deltaTime) {
    Transform* transform = GetEntity()->GetTransform();
    if (!transform) return;

    glm::vec3 moveDir(0.0f);
    if (Input::GetKey(GLFW_KEY_W)) moveDir.z += 1.0f;
    if (Input::GetKey(GLFW_KEY_S)) moveDir.z -= 1.0f;
    if (Input::GetKey(GLFW_KEY_A)) moveDir.x += 1.0f;
    if (Input::GetKey(GLFW_KEY_D)) moveDir.x -= 1.0f;
    if (moveDir != glm::vec3(0.0f)) moveDir = glm::normalize(moveDir);

    glm::vec3 newPos = transform->GetPosition();
    newPos.x += moveDir.x * m_moveSpeed * deltaTime;
    newPos.z += moveDir.z * m_moveSpeed * deltaTime;
    // 不修改 Y 坐标
    transform->SetPosition(newPos);
}