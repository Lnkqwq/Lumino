#include "Entity.h"
#include "Transform.h"

Entity::Entity(const std::string& name) : m_name(name) 
{
    m_transform = AddComponent<Transform>();
}

Entity::~Entity() = default;

void Entity::Start() 
{
    for (auto& pair : m_components) 
    {
        pair.second->Start();
    }
}

void Entity::Update(float deltaTime) 
{
    for (auto& pair : m_components) 
    {
        pair.second->Update(deltaTime);
    }
}

void Entity::LateUpdate(float deltaTime) 
{
    for (auto& pair : m_components) 
    {
        pair.second->LateUpdate(deltaTime);
    }
}