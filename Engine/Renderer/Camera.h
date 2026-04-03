#pragma once
#include "Engine/glm/glm.hpp"
#include "Engine/glm/gtc/matrix_transform.hpp"

namespace Lumino {

class Camera {
public:
    Camera();
    
    // 设置位置和朝向
    void SetPosition(const glm::vec3& pos);
    void SetTarget(const glm::vec3& target);
    void SetUp(const glm::vec3& up);
    
    // 移动相机
    void MoveForward(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    
    // 旋转相机（角度制）
    void Rotate(float yaw, float pitch);
    
    // 获取矩阵
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;
    
    // 获取位置/朝向
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    
    // 设置投影参数
    void SetFOV(float fov) { m_fov = fov; }
    void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
    void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
    
private:
    glm::vec3 m_position;
    glm::vec3 m_target;      // 看向的点
    glm::vec3 m_up;
    
    float m_yaw;      // 水平旋转角度
    float m_pitch;    // 垂直旋转角度
    
    float m_fov;           // 视野角度
    float m_nearPlane;     // 近平面
    float m_farPlane;      // 远平面
};

}