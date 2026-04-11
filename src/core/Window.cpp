#include "Window.h"
#include "input/Input.h"
#include <GLFW/glfw3.h>
#include <iostream>

class Window::Impl 
{
 public:
    GLFWwindow* window;
    int width, height;

    Impl() : window(nullptr), width(1280), height(720) 
    {
        if (!glfwInit()) 
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 #endif

        window = glfwCreateWindow(width, height, "Lumino Engine", nullptr, nullptr);
        if (!window) 
        {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 默认捕获鼠标

        // 设置回调，传递指针到Input静态类
        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) 
        {
            Input::KeyCallback(key, scancode, action, mods);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) 
        {
            Input::MousePosCallback(xpos, ypos);
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) 
        {
            Input::MouseButtonCallback(button, action, mods);
        });
    }

    ~Impl() 
    {
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
    }
};

Window::Window() : m_impl(new Impl()) {}
Window::~Window() { delete m_impl; }
bool Window::ShouldClose() const { return glfwWindowShouldClose(m_impl->window); }
void Window::PollEvents() { glfwPollEvents(); }
void Window::SwapBuffers() { glfwSwapBuffers(m_impl->window); }
void Window::SetTitle(const std::string& title) { glfwSetWindowTitle(m_impl->window, title.c_str()); }
void Window::SetCursorMode(int mode) { glfwSetInputMode(m_impl->window, GLFW_CURSOR, mode); }
int Window::GetWidth() const { int w, h; glfwGetWindowSize(m_impl->window, &w, &h); return w; }
int Window::GetHeight() const { int w, h; glfwGetWindowSize(m_impl->window, &w, &h); return h; }
float Window::GetAspect() const { return (float)GetWidth() / (float)GetHeight(); }
void* Window::GetNativeWindow() const { return m_impl->window; }