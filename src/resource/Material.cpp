#include "Material.h"
#include "Texture.h"

Material::Material() : m_baseColor(1.0f), m_metallic(0.0f), m_roughness(0.5f) {}
Material::~Material() = default;

void Material::SetBaseColorTexture(const std::shared_ptr<Texture>& texture) {
    m_baseColorTexture = texture;
}

void Material::SetBaseColor(const glm::vec3& color) {
    m_baseColor = color;
}

void Material::SetMetallic(float metallic) {
    m_metallic = metallic;
}

void Material::SetRoughness(float roughness) {
    m_roughness = roughness;
}

void Material::Bind() const {
    if (m_baseColorTexture) {
        m_baseColorTexture->Bind(0);
    }
    // 实际 uniform 设置在 MeshRenderer 中调用 shader->Set... 完成
}