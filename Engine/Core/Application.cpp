#include "Application.h"
#include "Window.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Lumino {

Application::Application() {
    m_window = std::make_unique<Window>("Lumino Engine", 1280, 720);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }
    
    glfwSetWindowUserPointer(m_window->GetNativeWindow(), this);
    
    glfwSetFramebufferSizeCallback(m_window->GetNativeWindow(), [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
}

Application::~Application() = default;

void Application::Run() {
    // 调用 OnStart 初始化
    OnStart();
    
    float lastTime = glfwGetTime();

    while (m_running && !m_window->ShouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        m_window->PollEvents();

        OnUpdate(deltaTime);

        m_window->SwapBuffers();
    }
}

void Application::Close() {
    m_running = false;
}

}