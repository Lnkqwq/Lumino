#pragma once
#include <string>

struct GLFWwindow;

namespace Lumino {

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;
    GLFWwindow* GetNativeWindow() const { return m_window; }

private:
    GLFWwindow* m_window;
};

}