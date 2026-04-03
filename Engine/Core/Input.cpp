#include "Input.h"
#include "Application.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <map>  // 添加这一行

namespace Lumino {

static std::map<int, bool> s_keyState;
static std::map<int, bool> s_keyJustPressed;

bool Input::IsKeyPressed(int keycode) {
    GLFWwindow* window = glfwGetCurrentContext();
    return glfwGetKey(window, keycode) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button) {
    GLFWwindow* window = glfwGetCurrentContext();
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

void Input::GetMousePosition(double& x, double& y) {
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetCursorPos(window, &x, &y);
}

bool Input::IsKeyJustPressed(int keycode) {
    GLFWwindow* window = glfwGetCurrentContext();
    bool currentState = glfwGetKey(window, keycode) == GLFW_PRESS;
    bool& lastState = s_keyState[keycode];
    
    if (currentState && !lastState) {
        lastState = currentState;
        return true;
    }
    lastState = currentState;
    return false;
}

}