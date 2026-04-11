#pragma once
#include <memory>

class Window;
class Renderer;
class Scene;

class Application 
{
 public:
    Application();
    ~Application();

    Scene* GetScene() { return m_scene.get(); }

    void Run();
    void Quit();

 private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Scene> m_scene;
    bool m_running;
};