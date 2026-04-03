#include "Camera.h"
#include <cmath>

namespace Lumino {

Camera::Camera()
    : m_position(0.0f, 0.0f, 5.0f)
    , m_target(0.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)   // 默认看向 -Z 方向
    , m_pitch(0.0f)
    , m_fov(45.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(100.0f)
{
}

void Camera::SetPosition(const glm::vec3& pos) {
    m_position = pos;
}

void Camera::SetTarget(const glm::vec3& target) {
    m_target = target;
}

void Camera::SetUp(const glm::vec3& up) {
    m_up = up;
}

void Camera::MoveForward(float distance) {
    glm::vec3 forward = GetForward();
    m_position += forward * distance;
    m_target += forward * distance;
}

void Camera::MoveRight(float distance) {
    glm::vec3 right = GetRight();
    m_position += right * distance;
    m_target += right * distance;
}

void Camera::MoveUp(float distance) {
    m_position.y += distance;
    m_target.y += distance;
}

void Camera::Rotate(float yaw, float pitch) {
    m_yaw += yaw;
    m_pitch += pitch;
    
    // 限制垂直角度，防止翻转
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;
    
    // 根据欧拉角计算新的朝向
    glm::vec3 forward;
    forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    forward.y = sin(glm::radians(m_pitch));
    forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    forward = glm::normalize(forward);
    
    m_target = m_position + forward;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_target, m_up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(m_fov), aspectRatio, m_nearPlane, m_farPlane);
}

glm::vec3 Camera::GetForward() const {
    return glm::normalize(m_target - m_position);
}

glm::vec3 Camera::GetRight() const {
    return glm::normalize(glm::cross(GetForward(), m_up));
}

}