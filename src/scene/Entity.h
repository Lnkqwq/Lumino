#pragma once
#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Transform.h"

class Component;

class Entity 
{
 public:
    Entity(const std::string& name = "");
    ~Entity();

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) 
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        T* comp = new T(std::forward<Args>(args)...);
        comp->m_entity = this;
        m_components[std::type_index(typeid(T))].reset(comp);
        return comp;
    }

    template<typename T>
    T* GetComponent() 
    {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end())
            return static_cast<T*>(it->second.get());
        return nullptr;
    }

    Transform* GetTransform() { return m_transform; }
    const std::string& GetName() const { return m_name; }

    void Start();
    void Update(float deltaTime);
    void LateUpdate(float deltaTime);

 private:
    std::string m_name;
    Transform* m_transform;  // shortcut, also stored in components
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};