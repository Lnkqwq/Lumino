#include "Renderer.h"
#include "Shader.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include "Engine/glm/glm.hpp"
#include "Engine/glm/gtc/matrix_transform.hpp"
#include <iostream>

namespace Lumino {

std::unique_ptr<Shader> Renderer::s_DefaultShader = nullptr;
unsigned int Renderer::s_VAO = 0;
unsigned int Renderer::s_VBO = 0;
unsigned int Renderer::s_EBO = 0;
bool Renderer::s_initialized = false;

void Renderer::Init() 
{
    if (s_initialized) return;
    
    glEnable(GL_TEXTURE_2D);

    // 加载着色器（路径需要根据实际位置调整）
    s_DefaultShader = std::make_unique<Shader>
    (
    "C:/Users/34035/Lumino/Sandbox/Shaders/vertex.glsl",
    "C:/Users/34035/Lumino/Sandbox/Shaders/fragment.glsl"
    );
    
    glEnable(GL_DEPTH_TEST);
    
    // 立方体顶点数据 (位置 + 颜色)
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,
    };
    
    unsigned int indices[] = {
        0,1,2, 2,3,0,
        1,5,6, 6,2,1,
        5,4,7, 7,6,5,
        4,0,3, 3,7,4,
        3,2,6, 6,7,3,
        4,5,1, 1,0,4,
    };
    
    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);
    glGenBuffers(1, &s_EBO);
    
    glBindVertexArray(s_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    s_initialized = true;
    std::cout << "Renderer initialized" << std::endl;
}


void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::DrawCube(const glm::vec3& center, float size) {
    if (!s_initialized) return;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, center);
    model = glm::scale(model, glm::vec3(size));
    
    s_DefaultShader->Use();
    s_DefaultShader->SetMat4("model", model);
    // view 和 projection 需要由调用者传入，这里先设置单位矩阵
    s_DefaultShader->SetMat4("view", glm::mat4(1.0f));
    s_DefaultShader->SetMat4("projection", glm::mat4(1.0f));
    
    glBindVertexArray(s_VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}