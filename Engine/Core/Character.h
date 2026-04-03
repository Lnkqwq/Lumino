#pragma once
#include "Engine/glm/glm.hpp"

namespace Lumino {

class Character {
public:
    Character();
    
    void MoveForward(float distance);
    void MoveRight(float distance);
    void Rotate(float yaw, float pitch);
    
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    
    void SetPosition(const glm::vec3& pos) { m_position = pos; }
    void SetYaw(float yaw) { m_yaw = yaw; }
    float GetYaw() const { return m_yaw; }

private:
    glm::vec3 m_position;
    float m_yaw;      // 水平旋转角度（面向）
    float m_pitch;    // 垂直角度（限制范围）
};

}