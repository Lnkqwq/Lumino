#include "Light.h"

namespace Lumino {

DirectionalLight Light::s_directionalLight = {
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f
};

AmbientLight Light::s_ambientLight = {
    glm::vec3(0.2f, 0.2f, 0.3f),
    1.0f
};

void Light::SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity) {
    s_directionalLight.direction = glm::normalize(direction);
    s_directionalLight.color = color;
    s_directionalLight.intensity = intensity;
}

void Light::SetAmbientLight(const glm::vec3& color, float intensity) {
    s_ambientLight.color = color;
    s_ambientLight.intensity = intensity;
}

}