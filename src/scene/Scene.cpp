#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include <algorithm>

Scene::Scene() : m_mainCamera(nullptr) {}
Scene::~Scene() = default;

Entity* Scene::CreateEntity(const std::string& name) 
{
    auto entity = std::make_unique<Entity>(name);
    Entity* raw = entity.get();
    m_entities.push_back(std::move(entity));
    return raw;
}

void Scene::DestroyEntity(Entity* entity) 
{
    auto it = std::find_if(m_entities.begin(), m_entities.end(),
        [entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; });
    if (it != m_entities.end()) 
    {
        m_entities.erase(it);
    }
}

void Scene::Update(float deltaTime) 
{
    for (auto& entity : m_entities) 
    {
        entity->Update(deltaTime);
    }
}

void Scene::LateUpdate(float deltaTime) 
{
    for (auto& entity : m_entities) 
    {
        entity->LateUpdate(deltaTime);
    }
}

void Scene::SetMainCamera(Camera* camera) { m_mainCamera = camera; }
Camera* Scene::GetMainCamera() const { return m_mainCamera; }