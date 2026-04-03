#pragma once
#include <vector>
#include <string>
#include "Engine/glm/glm.hpp"
#include "AnimationController.h"
#include "GLTFLoader.h"

struct GLTFMesh;

namespace Lumino {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct SubMesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int textureID = 0;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh();
    
    void Draw() const;
    
void AddSubMeshFromGLTF(const GLTFMesh& gltfMesh);

    static Mesh* LoadOBJ(const std::string& path);
    
    void AddSubMesh(const SubMesh& subMesh);
    void Clear();

    void SetAnimationController(AnimationController* controller) { m_animController = controller; }
    AnimationController* GetAnimationController() { return m_animController; }
    
    void SetBoneMatrices(const std::vector<glm::mat4>& matrices);

private:
    std::vector<SubMesh> m_subMeshes;

    AnimationController* m_animController = nullptr;
    unsigned int m_boneMatricesUBO = 0;  // Uniform Buffer Object for bones
};

}