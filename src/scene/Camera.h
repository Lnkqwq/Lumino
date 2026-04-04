#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class Camera : public Component {
public:
    Camera();
    void Update(float deltaTime) override;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect) const;

    void SetPerspective(float fov, float nearPlane, float farPlane);
    void SetOrthographic(float size, float nearPlane, float farPlane);

    bool IsPerspective() const { return m_isPerspective; }
    float GetFOV() const { return m_fov; }
    float GetNear() const { return m_near; }
    float GetFar() const { return m_far; }

private:
    bool m_isPerspective;
    float m_fov;          // degrees, for perspective
    float m_orthoSize;    // for orthographic
    float m_near, m_far;
};