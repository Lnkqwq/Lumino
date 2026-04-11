#pragma once

class Entity;

class Component 
{
 public:
    virtual ~Component() = default;
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void LateUpdate(float deltaTime) {}

    Entity* GetEntity() const { return m_entity; }

 private:
    friend class Entity;
    Entity* m_entity = nullptr;
};