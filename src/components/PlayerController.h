#pragma once
#include "scene/Component.h"

class PlayerController : public Component {
public:
    PlayerController();
    void Update(float deltaTime) override;

    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
    float GetMoveSpeed() const { return m_moveSpeed; }

private:
      float m_moveSpeed;
};