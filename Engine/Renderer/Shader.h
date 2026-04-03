#pragma once
#include <string>
#include "Engine/glm/glm.hpp"

namespace Lumino {

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    void Use() const;
    
    void SetFloat(const std::string& name, float value) const;

    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetInt(const std::string& name, int value) const;

private:
    unsigned int m_ID;
    
    void CheckCompileErrors(unsigned int shader, const std::string& type);
};

}