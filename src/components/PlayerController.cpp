#include "PlayerController.h"
#include "input/Input.h"
#include "scene/Transform.h"
#include "scene/Entity.h"
#include "components/ThirdPersonCamera.h"
#include "animation/Animator.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

PlayerController::PlayerController() 
    : m_moveSpeed(5.0f), m_camera(nullptr), m_velocity(0.0f), 
      m_verticalVelocity(0.0f), m_groundHeight(0.0f), m_jumpSpeed(8.0f) {}

void PlayerController::Update(float deltaTime) 
{
    Transform* transform = GetEntity()->GetTransform();
    if (!transform) return;

    // 1. 获取移动方向 (WASD)
    glm::vec3 moveDir(0.0f);
    if (Input::GetKey(GLFW_KEY_W)) moveDir.z -= 1.0f;
    if (Input::GetKey(GLFW_KEY_S)) moveDir.z += 1.0f;
    if (Input::GetKey(GLFW_KEY_A)) moveDir.x -= 1.0f;
    if (Input::GetKey(GLFW_KEY_D)) moveDir.x += 1.0f;
    if (moveDir != glm::vec3(0.0f)) moveDir = glm::normalize(moveDir);

    // 2. 获取当前帧的初始位置
    glm::vec3 newPos = transform->GetPosition();

    // 3. 根据相机方向计算水平移动
    if (m_camera) 
    {
        float yawRad = glm::radians(m_camera->GetYaw());
        glm::vec3 forward(cos(yawRad), 0.0f, sin(yawRad));
        glm::vec3 right(forward.z, 0.0f, -forward.x);
        glm::vec3 worldMove = forward * moveDir.z + right * moveDir.x;
        newPos += worldMove * m_moveSpeed * deltaTime;
        m_velocity = worldMove;
    } else {
             newPos.x += moveDir.x * m_moveSpeed * deltaTime;
             newPos.z += moveDir.z * m_moveSpeed * deltaTime;
             m_velocity = moveDir;
           }

    // 跳跃输入检测（仅当在地面上时）
bool onGround = (newPos.y <= m_groundHeight + 0.05f); // 小容差
if (onGround && Input::GetKeyDown(GLFW_KEY_SPACE)) 
{
    m_verticalVelocity = m_jumpSpeed;
    // 可选：稍微抬高位置避免穿透（但通常不需要）
    // newPos.y = m_groundHeight + 0.01f;
}

    // 4. 重力与垂直速度
    const float gravity = -20.0f;
    m_verticalVelocity += gravity * deltaTime;
    newPos.y += m_verticalVelocity * deltaTime;

    // 5. 地面碰撞检测
if (newPos.y <= m_groundHeight + 0.05f) 
{
    newPos.y = m_groundHeight;
    m_verticalVelocity = 0.0f;
}

    // 6. 应用最终位置
    transform->SetPosition(newPos);

    // 7. 动画控制（可选）
    bool isMoving = (moveDir != glm::vec3(0.0f));
    auto animator = GetEntity()->GetComponent<Animator>();
    if (animator) 
    {
        if (isMoving) {
            if (!animator->IsPlaying()) 
            {
                animator->Resume();
            }
        } else {
                  if (animator->IsPlaying()) 
                  {
                     animator->Pause();
                     animator->SetTime(0.0f);
                  }
        }
    }
}

void PlayerController::LateUpdate(float deltaTime) 
{
    if (!m_camera) return;
    float yawRad = glm::radians(m_camera->GetYaw());
    glm::quat fixRotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat backRotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat cameraYaw = glm::angleAxis(yawRad, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat finalRotation = cameraYaw * backRotation * fixRotation;
    GetEntity()->GetTransform()->SetRotation(finalRotation);
}