#pragma once
#include <memory>
#include <glm/glm.hpp>

class Texture;

class Material 
{
 public:
    Material();
    ~Material();

    void SetBaseColorTexture(const std::shared_ptr<Texture>& texture);
    void SetBaseColor(const glm::vec3& color);
    void SetMetallic(float metallic);
    void SetRoughness(float roughness);

    void Bind() const;   // 绑定纹理和设置uniform，实际由MeshRenderer调用Shader

    // Getters
    std::shared_ptr<Texture> GetBaseColorTexture() const { return m_baseColorTexture; }
    glm::vec3 GetBaseColor() const { return m_baseColor; }
    float GetMetallic() const { return m_metallic; }
    float GetRoughness() const { return m_roughness; }

 private:
    std::shared_ptr<Texture> m_baseColorTexture;
    glm::vec3 m_baseColor;
    float m_metallic;
    float m_roughness;
};