#pragma once
#include "Component.h"
#include <memory>

class Mesh;
class Material;
class Shader;

class MeshRenderer : public Component 
{
 public:
    MeshRenderer();
    ~MeshRenderer();

    void SetMesh(std::shared_ptr<Mesh> mesh);
    void SetMaterial(std::shared_ptr<Material> material);

    void Draw(Shader* shader);

 private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
};