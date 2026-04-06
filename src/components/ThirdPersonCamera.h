#pragma once
#include "scene/Component.h"
#include <glm/glm.hpp>

class Entity;

class ThirdPersonCamera : public Component {
public:
    ThirdPersonCamera();
    void Update(float deltaTime) override;

    void SetTarget(Entity* target);
    void SetDistance(float distance) { m_distance = distance; }
    void SetSensitivity(float sens) { m_sensitivity = sens; }

    float GetYaw() const { return m_yaw; }

private:
    Entity* m_target;
    float m_distance;
    float m_pitch;      // 俯仰角
    float m_yaw;        // 偏航角
    float m_sensitivity;
    bool m_firstMouse;
    float m_lastMouseX, m_lastMouseY;
};