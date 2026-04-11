#pragma once
#include <vector>
#include <memory>
#include <string>

class Entity;
class Camera;

class Scene 
{
 public:
    Scene();
    ~Scene();

    Entity* CreateEntity(const std::string& name = std::string());
    void DestroyEntity(Entity* entity);

    void Update(float deltaTime);
    void LateUpdate(float deltaTime);

    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera() const;

    const std::vector<std::unique_ptr<Entity>>& GetAllEntities() const { return m_entities; }

 private:
    std::vector<std::unique_ptr<Entity>> m_entities;
    Camera* m_mainCamera;
};