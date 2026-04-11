#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::ivec4 BoneIndices;  // 新增
    glm::vec4 BoneWeights;   // 新增
};

class Mesh 
{
 public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Draw() const;

 private:
    unsigned int m_VAO, m_VBO, m_EBO;
    unsigned int m_indexCount;

    void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};