#pragma once
#include "scene/Component.h"
#include <glm/glm.hpp>
#include <string>

class ThirdPersonCamera;

class PlayerController : public Component 
{
 public:
    PlayerController();
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;

    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
    void SetCamera(ThirdPersonCamera* camera) { m_camera = camera; }
    void SetGroundHeight(float height) { m_groundHeight = height; }  // 新增

 private:
    float m_moveSpeed;
    ThirdPersonCamera* m_camera;
    glm::vec3 m_velocity;
    std::string m_runAnimName = "animation_0";  // 根据实际动画名称修改
    float m_verticalVelocity;   // 新增：垂直速度
    float m_groundHeight;       // 新增：地面高度

    float m_jumpSpeed;          // 跳跃初速度
    bool IsOnGround() const;    // 可选，辅助函数
};