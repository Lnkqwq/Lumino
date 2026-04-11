#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader 
{
 public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const;
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;
    void SetMat4Array(const std::string& name, const glm::mat4* values, int count) const;

 private:
    unsigned int m_programID;
    mutable std::unordered_map<std::string, int> m_uniformCache;

    int GetUniformLocation(const std::string& name) const;
    unsigned int CompileShader(unsigned int type, const std::string& source);
    std::string LoadShaderSource(const std::string& filepath);
};