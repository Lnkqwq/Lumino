#include "MeshRenderer.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/Texture.h"
#include "render/Shader.h"

MeshRenderer::MeshRenderer() = default;
MeshRenderer::~MeshRenderer() = default;

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh) {
    m_mesh = mesh;
}

void MeshRenderer::SetMaterial(std::shared_ptr<Material> material) {
    m_material = material;
}

void MeshRenderer::Draw(Shader* shader) {
    if (!m_mesh || !m_material) return;

    // 绑定材质纹理
    if (m_material->GetBaseColorTexture()) {
        m_material->GetBaseColorTexture()->Bind(0);
        shader->SetInt("material.diffuse", 0);
    }
    shader->SetVec3("material.color", m_material->GetBaseColor());
    shader->SetFloat("material.metallic", m_material->GetMetallic());
    shader->SetFloat("material.roughness", m_material->GetRoughness());

    m_mesh->Draw();
}