#include "Application.h"
#include "Window.h"
#include "Renderer.h"
#include "scene/Scene.h"
#include "input/Input.h"
#include <GLFW/glfw3.h>

Application::Application()
    : m_running(true) {
    m_window = std::make_unique<Window>();
    m_renderer = std::make_unique<Renderer>();
    m_scene = std::make_unique<Scene>();
}

Application::~Application() = default;

void Application::Run() {
    float lastTime = (float)glfwGetTime();

    while (m_running && !m_window->ShouldClose()) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        m_window->PollEvents();
        Input::Update();  // 更新输入状态

        m_scene->Update(deltaTime);
        m_scene->LateUpdate(deltaTime);

        m_renderer->BeginScene(m_scene->GetMainCamera());
        m_renderer->RenderScene(m_scene.get());
        m_renderer->EndScene();

        m_window->SwapBuffers();
    }
}

void Application::Quit() {
    m_running = false;
}