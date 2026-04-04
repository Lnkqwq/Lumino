#pragma once
#include <string>

class Entity;
class Scene;

class glTFLoader {
public:
    static Entity* Load(const std::string& path, Scene* scene);
};