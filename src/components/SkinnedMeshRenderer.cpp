#include "SkinnedMeshRenderer.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/Texture.h"
#include "animation/Animator.h"
#include "render/Shader.h"

SkinnedMeshRenderer::SkinnedMeshRenderer() : m_animator(nullptr) {}
SkinnedMeshRenderer::~SkinnedMeshRenderer() = default;

void SkinnedMeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh) {
    m_mesh = mesh;
}

void SkinnedMeshRenderer::SetMaterial(std::shared_ptr<Material> material) {
    m_material = material;
}

void SkinnedMeshRenderer::SetAnimator(Animator* animator) {
    m_animator = animator;
}

void SkinnedMeshRenderer::Draw(Shader* shader) {
    if (!m_mesh || !m_material) return;

    if (m_material->GetBaseColorTexture()) {
        m_material->GetBaseColorTexture()->Bind(0);
        shader->SetInt("material.diffuse", 0);
    }
    shader->SetVec3("material.color", m_material->GetBaseColor());
    shader->SetFloat("material.metallic", m_material->GetMetallic());
    shader->SetFloat("material.roughness", m_material->GetRoughness());

    if (m_animator) {
        const auto& boneMatrices = m_animator->GetBoneMatrices();
        if (!boneMatrices.empty()) {
            shader->SetMat4Array("boneMatrices", boneMatrices.data(), boneMatrices.size());
        }
    }

    m_mesh->Draw();
}