#include "scene/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Transform::Transform() : m_position(0.0f), m_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), m_scale(1.0f) {}

glm::mat4 Transform::GetModelMatrix() const 
{
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), m_position);
    mat = mat * glm::mat4_cast(m_rotation);
    mat = glm::scale(mat, m_scale);
    return mat;
}

void Transform::SetPosition(const glm::vec3& pos) { m_position = pos; }
void Transform::SetRotation(const glm::quat& rot) { m_rotation = rot; }
void Transform::SetScale(const glm::vec3& scale) { m_scale = scale; }
void Transform::Translate(const glm::vec3& delta) { m_position += delta; }

glm::vec3 Transform::Forward() const { return m_rotation * glm::vec3(0, 0, -1); }
glm::vec3 Transform::Right() const { return m_rotation * glm::vec3(1, 0, 0); }
glm::vec3 Transform::Up() const { return m_rotation * glm::vec3(0, 1, 0); }