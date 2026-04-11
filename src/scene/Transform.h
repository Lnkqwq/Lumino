#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"

class Transform : public Component 
{
 public:
    Transform();
    glm::mat4 GetModelMatrix() const;

    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::quat& rot);
    void SetScale(const glm::vec3& scale);
    void Translate(const glm::vec3& delta);

    glm::vec3 GetPosition() const { return m_position; }
    glm::quat GetRotation() const { return m_rotation; }
    glm::vec3 GetScale() const { return m_scale; }

    glm::vec3 Forward() const;
    glm::vec3 Right() const;
    glm::vec3 Up() const;

 private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
};