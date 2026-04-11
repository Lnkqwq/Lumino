#include "ThirdPersonCamera.h"
#include "input/Input.h"
#include "scene/Entity.h"
#include "scene/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

ThirdPersonCamera::ThirdPersonCamera()
    : m_target(nullptr), m_distance(2.0f), m_pitch(0.0f), m_yaw(-90.0f),
      m_sensitivity(0.1f), m_firstMouse(true), m_lastMouseX(0), m_lastMouseY(0) {}

void ThirdPersonCamera::SetTarget(Entity* target) 
{
    m_target = target;
}

void ThirdPersonCamera::Update(float deltaTime) 
{
    static int frameCount = 0;

    // 获取鼠标偏移
    float dx, dy;
    Input::GetMouseDelta(dx, dy);
    static int mouseFrame = 0;

    // 更新角度
    m_yaw += dx * m_sensitivity;
    m_pitch -= dy * m_sensitivity;
    // 限制俯仰角
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    // 计算相机位置
    glm::vec3 targetPos = m_target->GetTransform()->GetPosition();
    float radYaw = glm::radians(m_yaw);
    float radPitch = glm::radians(m_pitch);
    glm::vec3 offset;
    offset.x = cos(radYaw) * cos(radPitch);
    offset.y = sin(radPitch);
    offset.z = sin(radYaw) * cos(radPitch);
    offset *= m_distance;
    glm::vec3 cameraPos = targetPos + offset;

    // 设置相机位置和旋转
    Transform* camTransform = GetEntity()->GetTransform();
    camTransform->SetPosition(cameraPos);
    glm::vec3 forward = glm::normalize(targetPos - cameraPos);
    glm::quat rotation = glm::quatLookAt(forward, glm::vec3(0.0f, 1.0f, 0.0f));
    camTransform->SetRotation(rotation);
}