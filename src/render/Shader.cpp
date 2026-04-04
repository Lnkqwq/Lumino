#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertex);
    glAttachShader(m_programID, fragment);
    glLinkProgram(m_programID);

    int success;
    char infoLog[512];
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(m_programID);
}

void Shader::Use() const {
    glUseProgram(m_programID);
}

int Shader::GetUniformLocation(const std::string& name) const {
    if (m_uniformCache.find(name) != m_uniformCache.end())
        return m_uniformCache[name];
    int location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(GetUniformLocation(name), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}
void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}
void Shader::SetMat4Array(const std::string& name, const glm::mat4* values, int count) const {
    glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, &values[0][0][0]);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }
    return id;
}

std::string Shader::LoadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}