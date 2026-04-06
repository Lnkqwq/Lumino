#include "PlayerController.h"
#include "input/Input.h"
#include "scene/Transform.h"
#include "scene/Entity.h"
#include "components/ThirdPersonCamera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

PlayerController::PlayerController() : m_moveSpeed(3.0f), m_camera(nullptr) {}

void PlayerController::Update(float deltaTime) {
    Transform* transform = GetEntity()->GetTransform();
    if (!transform) return;

    // 移动逻辑（基于相机的前/右方向）
    glm::vec3 moveDir(0.0f);
    if (Input::GetKey(GLFW_KEY_W)) moveDir.z -= 1.0f;
    if (Input::GetKey(GLFW_KEY_S)) moveDir.z += 1.0f;
    if (Input::GetKey(GLFW_KEY_A)) moveDir.x -= 1.0f;
    if (Input::GetKey(GLFW_KEY_D)) moveDir.x += 1.0f;
    if (moveDir != glm::vec3(0.0f)) moveDir = glm::normalize(moveDir);

    // 获取相机的前向和右向（水平方向，忽略俯仰）
    if (m_camera) {
        float yawRad = glm::radians(m_camera->GetYaw());
        glm::vec3 forward(cos(yawRad), 0.0f, sin(yawRad));
        glm::vec3 right(forward.z, 0.0f, -forward.x);
        glm::vec3 worldMove = forward * moveDir.z + right * moveDir.x;
        glm::vec3 newPos = transform->GetPosition() + worldMove * m_moveSpeed * deltaTime;
        transform->SetPosition(newPos);
        m_velocity = worldMove;  // 记录移动方向，用于朝向
    } else {
        // 如果没有相机，回退到世界坐标移动
        glm::vec3 newPos = transform->GetPosition();
        newPos.x += moveDir.x * m_moveSpeed * deltaTime;
        newPos.z += moveDir.z * m_moveSpeed * deltaTime;
        transform->SetPosition(newPos);
        m_velocity = moveDir;
    }
}

void PlayerController::LateUpdate(float deltaTime) {
    if (!m_camera) return;

    // 获取相机水平旋转角度（度），假设 ThirdPersonCamera::GetYaw() 返回度数
    float yawDeg = m_camera->GetYaw();
    float yawRad = glm::radians(yawDeg);

    // 静态修正：先绕 X 轴 -90 度使模型正立，再绕 Y 轴 -90 度使模型背对相机
    glm::quat fixRotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat backRotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // 动态相机跟随：绕 Y 轴旋转 yaw 角度
    glm::quat cameraYaw = glm::angleAxis(yawRad, glm::vec3(0.0f, 1.0f, 0.0f));

    // 组合旋转：先应用静态修正，再应用相机水平旋转（使角色始终背对相机方向）
    glm::quat finalRotation = cameraYaw * backRotation * fixRotation;

    GetEntity()->GetTransform()->SetRotation(finalRotation);
}
