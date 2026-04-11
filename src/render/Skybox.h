#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Shader;

class Skybox 
{
 public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    void Draw(Shader* shader, const glm::mat4& view, const glm::mat4& projection);

 private:
    unsigned int m_VAO, m_VBO;
    unsigned int m_cubemapTexture;
    void loadCubemap(const std::vector<std::string>& faces);
};