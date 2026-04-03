#include "Character.h"
#include "Engine/glm/gtc/matrix_transform.hpp"
#include <cmath>

namespace Lumino {

Character::Character()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
{
}

void Character::MoveForward(float distance) {
    m_position += GetForward() * distance;
}

void Character::MoveRight(float distance) {
    m_position += GetRight() * distance;
}

void Character::Rotate(float yaw, float pitch) {
    m_yaw += yaw;
    m_pitch += pitch;
    
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;
}

glm::vec3 Character::GetForward() const {
    glm::vec3 forward;
    forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    forward.y = sin(glm::radians(m_pitch));
    forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    return glm::normalize(forward);
}

glm::vec3 Character::GetRight() const {
    glm::vec3 forward = GetForward();
    return glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}

}