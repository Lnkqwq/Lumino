#pragma once
#include <memory>

class Shader;
class Scene;
class Camera;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void BeginScene(Camera* camera);
    void EndScene();
    void RenderScene(Scene* scene);

private:
    std::unique_ptr<Shader> m_basicShader;
    Camera* m_currentCamera;
    float m_currentAspect;
};