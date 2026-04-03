#include "Mesh.h"
#include "GLTFLoader.h"  // 在这里包含，因为要用 GLTFMesh 的完整定义
#include "Engine/Renderer/Renderer.h"
#include "Shader.h"
#include "glad.h"
#include "GLTFLoader.h"
#include <GLFW/glfw3.h>
#include "Engine/glm/glm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>   // 使用 tinygltf 里的版本

namespace Lumino {

Mesh::~Mesh() {
    Clear();
}

void Mesh::Clear() {
    for (auto& subMesh : m_subMeshes) {
        if (subMesh.VAO) glDeleteVertexArrays(1, &subMesh.VAO);
        if (subMesh.VBO) glDeleteBuffers(1, &subMesh.VBO);
        if (subMesh.EBO) glDeleteBuffers(1, &subMesh.EBO);
        if (subMesh.textureID) glDeleteTextures(1, &subMesh.textureID);
    }
    m_subMeshes.clear();
}

void Mesh::AddSubMesh(const SubMesh& subMesh) {
    SubMesh newSubMesh = subMesh;
    
    glGenVertexArrays(1, &newSubMesh.VAO);
    glGenBuffers(1, &newSubMesh.VBO);
    glGenBuffers(1, &newSubMesh.EBO);
    
    glBindVertexArray(newSubMesh.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, newSubMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, newSubMesh.vertices.size() * sizeof(Vertex), 
                 newSubMesh.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newSubMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSubMesh.indices.size() * sizeof(unsigned int), 
                 newSubMesh.indices.data(), GL_STATIC_DRAW);
    
    // 位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // 法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
    glBindVertexArray(0);
    
    m_subMeshes.push_back(newSubMesh);
}

void Mesh::Draw() const {
    auto shader = Renderer::GetDefaultShader();
    if (!shader) return;
    
    shader->Use();
    
    // 设置 model 矩阵（单位矩阵，由调用者控制）
    shader->SetMat4("model", glm::mat4(1.0f));
    
    for (const auto& subMesh : m_subMeshes) {
        if (subMesh.textureID) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, subMesh.textureID);
            shader->SetInt("ourTexture", 0);
        }
        
        glBindVertexArray(subMesh.VAO);
        glDrawElements(GL_TRIANGLES, subMesh.indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

Mesh* Mesh::LoadOBJ(const std::string& path) {
    // 保留 OBJ 加载器，但需要改成多材质版本
    // 由于代码较长，先省略，以后再加
    std::cerr << "OBJ loader not implemented in multi-material version yet" << std::endl;
    return nullptr;
}

// 在文件末尾添加：
void Mesh::AddSubMeshFromGLTF(const GLTFMesh& gltfMesh) {
    SubMesh subMesh;
    
    // 转换顶点
    for (size_t i = 0; i < gltfMesh.positions.size(); i++) {
        Vertex v;
        v.position = gltfMesh.positions[i];
        v.normal = i < gltfMesh.normals.size() ? gltfMesh.normals[i] : glm::vec3(0.0f, 1.0f, 0.0f);
        v.texCoord = i < gltfMesh.texCoords.size() ? gltfMesh.texCoords[i] : glm::vec2(0.0f);
        subMesh.vertices.push_back(v);
    }
    subMesh.indices = gltfMesh.indices;
    
    AddSubMesh(subMesh);
}

void Mesh::SetBoneMatrices(const std::vector<glm::mat4>& matrices) {
    // 如果还没有创建 UBO，先创建
    if (m_boneMatricesUBO == 0) {
        glGenBuffers(1, &m_boneMatricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, m_boneMatricesUBO);
        // 假设最多 100 块骨骼，每块矩阵 64 字节
        glBufferData(GL_UNIFORM_BUFFER, 100 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        // 绑定到绑定点 0（需要在 shader 中对应）
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_boneMatricesUBO);
    }
    
    // 更新骨骼矩阵数据
    glBindBuffer(GL_UNIFORM_BUFFER, m_boneMatricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}