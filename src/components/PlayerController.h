#pragma once
#include "scene/Component.h"
#include <glm/glm.hpp>

class ThirdPersonCamera;

class PlayerController : public Component {
public:
    PlayerController();
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;  // 新增，用于在动画之后设置朝向

    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
    void SetCamera(ThirdPersonCamera* camera) { m_camera = camera; }

private:
    float m_moveSpeed;
    ThirdPersonCamera* m_camera;
    glm::vec3 m_velocity;  // 可选，用于移动
};