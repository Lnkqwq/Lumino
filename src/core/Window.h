#pragma once
#include <string>

class Window 
{
 public:
    Window();
    ~Window();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();
    void SetTitle(const std::string& title);
    void SetCursorMode(int mode);  // GLFW_CURSOR_NORMAL/DISABLED
    int GetWidth() const;
    int GetHeight() const;
    float GetAspect() const;
    void* GetNativeWindow() const;  // GLFWwindow*

 private:
    class Impl;
    Impl* m_impl;
};