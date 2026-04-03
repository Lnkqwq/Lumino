#pragma once
#include "Engine/glm/glm.hpp"
#include <vector>

namespace Lumino {

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

struct AmbientLight {
    glm::vec3 color;
    float intensity;
};

class Light {
public:
    static void SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity);
    static void SetAmbientLight(const glm::vec3& color, float intensity);
    
    static const DirectionalLight& GetDirectionalLight() { return s_directionalLight; }
    static const AmbientLight& GetAmbientLight() { return s_ambientLight; }

private:
    static DirectionalLight s_directionalLight;
    static AmbientLight s_ambientLight;
};

}