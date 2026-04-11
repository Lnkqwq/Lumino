#include "core/Renderer.h"
#include "render/Shader.h"
#include "scene/Scene.h"
#include "scene/Camera.h"
#include "scene/Entity.h"
#include "scene/MeshRenderer.h"   
#include "components/SkinnedMeshRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer() : m_currentCamera(nullptr), m_currentAspect(1.0f) 
{
    // 初始化OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

    m_basicShader = std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag");
    // 加载天空盒着色器
    m_skyboxShader = std::make_unique<Shader>("shaders/skybox_quad.vert", "shaders/skybox_quad.frag");
    // 加载天空盒纹理
 std::vector<std::string> faces = 
 {
    "assets/skybox/bluecloud_rt.jpg",   // right
    "assets/skybox/bluecloud_lf.jpg",   // left
    "assets/skybox/bluecloud_up.jpg",   // top
    "assets/skybox/bluecloud_dn.jpg",   // bottom
    "assets/skybox/bluecloud_ft.jpg",   // front
    "assets/skybox/bluecloud_bk.jpg"    // back
 };

 m_skybox = std::make_unique<Skybox>(faces);

}

Renderer::~Renderer() = default;

 void Renderer::BeginScene(Camera* camera) 
{
    if (!camera) return;
    m_currentCamera = camera;
    m_currentAspect = 1280.0f / 720.0f; // 临时，应该从window获取
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndScene() 
{
    // 空
}

void Renderer::RenderScene(Scene* scene) 
{
    if (!m_currentCamera) return;
    glm::mat4 view = m_currentCamera->GetViewMatrix();
    glm::mat4 proj = m_currentCamera->GetProjectionMatrix(m_currentAspect);

    // 渲染天空盒（全屏四边形方案）
    if (m_skybox && m_skyboxShader) 
    {
      glDepthMask(GL_FALSE);
      m_skybox->Draw(m_skyboxShader.get(), view, proj); // 直接传入原始 view 和 proj
      glDepthMask(GL_TRUE);
    }

    m_basicShader->Use();
    m_basicShader->SetMat4("view", view);
    m_basicShader->SetMat4("projection", proj);
    m_basicShader->SetVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    m_basicShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    glm::vec3 camPos = m_currentCamera->GetEntity()->GetTransform()->GetPosition();
    m_basicShader->SetVec3("viewPos", camPos);

    int meshCount = 0;
    for (auto& entity : scene->GetAllEntities()) 
    {
        auto mr = entity->GetComponent<MeshRenderer>();
        if (mr) 
        {
            meshCount++;
            glm::mat4 model = entity->GetTransform()->GetModelMatrix();
            m_basicShader->SetMat4("model", model);
            mr->Draw(m_basicShader.get());
        }
        auto skinned = entity->GetComponent<SkinnedMeshRenderer>();
        if (skinned) 
        {
            meshCount++;
            glm::mat4 model = entity->GetTransform()->GetModelMatrix();
            m_basicShader->SetMat4("model", model);
            skinned->Draw(m_basicShader.get());
        }
    }
    static int frame = 0;
    if (frame++ % 60 == 0) 
    {
        std::cout << "Frame " << frame << ", meshes rendered: " << meshCount << std::endl;
    }
}