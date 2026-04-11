#pragma once
#include <memory>
#include "render/Skybox.h"

class Shader;
class Scene;
class Camera;

class Renderer 
{
 public:
    Renderer();
    ~Renderer();

    void BeginScene(Camera* camera);
    void EndScene();
    void RenderScene(Scene* scene);

 private:
    std::unique_ptr<Shader> m_basicShader;
    std::unique_ptr<Skybox> m_skybox;
    std::unique_ptr<Shader> m_skyboxShader;
    Camera* m_currentCamera;
    float m_currentAspect;
};