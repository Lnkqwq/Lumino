#pragma once
#include "Engine/glm/glm.hpp"
#include <memory>

namespace Lumino {

class Shader;

class Renderer 
{
 public:
    static void Init();
    static void Clear();
    static void SetClearColor(float r, float g, float b, float a);
    static void DrawCube(const glm::vec3& center, float size);
    
    static Shader* GetDefaultShader() { return s_DefaultShader.get(); }
    
    void SetInt(const std::string& name, int value) const;

 private:
    static std::unique_ptr<Shader> s_DefaultShader;
    static unsigned int s_VAO, s_VBO, s_EBO;
    static bool s_initialized;
};

}