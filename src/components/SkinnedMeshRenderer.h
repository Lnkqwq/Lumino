#pragma once
#include "scene/Component.h"
#include <memory>
#include <vector>

class Mesh;
class Material;
class Shader;
class Animator;

class SkinnedMeshRenderer : public Component 
{
 public:
    SkinnedMeshRenderer();
    ~SkinnedMeshRenderer();

    void SetMesh(std::shared_ptr<Mesh> mesh);
    void SetMaterial(std::shared_ptr<Material> material);
    void SetAnimator(Animator* animator);

    void Draw(Shader* shader);

 private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    Animator* m_animator;
};