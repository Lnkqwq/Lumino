#include "Camera.h"
#include "Transform.h"
#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : m_isPerspective(true), m_fov(60.0f), m_orthoSize(5.0f), m_near(0.1f), m_far(1000.0f) {}

void Camera::Update(float deltaTime) 
{
    // 空实现，留给用户扩展
}

glm::mat4 Camera::GetViewMatrix() const 
{
    Transform* t = GetEntity()->GetTransform();
    glm::vec3 pos = t->GetPosition();
    glm::vec3 forward = t->Forward();
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    return glm::lookAt(pos, pos + forward, worldUp);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect) const 
{
    if (m_isPerspective) 
    {
        return glm::perspective(glm::radians(m_fov), aspect, m_near, m_far);
    } else {
        float half = m_orthoSize;
        return glm::ortho(-half * aspect, half * aspect, -half, half, m_near, m_far);
    }
}

void Camera::SetPerspective(float fov, float nearPlane, float farPlane) 
{
    m_isPerspective = true;
    m_fov = fov;
    m_near = nearPlane;
    m_far = farPlane;
}

void Camera::SetOrthographic(float size, float nearPlane, float farPlane) 
{
    m_isPerspective = false;
    m_orthoSize = size;
    m_near = nearPlane;
    m_far = farPlane;
}