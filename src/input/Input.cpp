#include "Input.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>   // 可选，用于调试

bool Input::m_keys[512];
bool Input::m_keysDown[512];
bool Input::m_keysUp[512];
double Input::m_mouseX = 0, Input::m_mouseY = 0;
double Input::m_lastMouseX = 0, Input::m_lastMouseY = 0;
double Input::m_mouseDeltaX = 0, Input::m_mouseDeltaY = 0;
bool Input::m_mouseButtons[8];
bool Input::m_mouseButtonsDown[8];
bool Input::m_mouseButtonsUp[8];
bool Input::m_firstMouse = true;

void Input::Update() {
    // 重置每帧的按键状态（按下/抬起）
    memset(m_keysDown, 0, sizeof(m_keysDown));
    memset(m_keysUp, 0, sizeof(m_keysUp));
    memset(m_mouseButtonsDown, 0, sizeof(m_mouseButtonsDown));
    memset(m_mouseButtonsUp, 0, sizeof(m_mouseButtonsUp));

    // 重置本帧的鼠标累积偏移
    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;

    // 注意：不要修改 m_lastMouseX/m_lastMouseY，它们由 MousePosCallback 持续更新
}

bool Input::GetKey(int key) { return m_keys[key]; }
bool Input::GetKeyDown(int key) { return m_keysDown[key]; }
bool Input::GetKeyUp(int key) { return m_keysUp[key]; }
void Input::GetMouseDelta(float& dx, float& dy) { dx = (float)m_mouseDeltaX; dy = (float)m_mouseDeltaY; }
void Input::GetMousePosition(double& x, double& y) { x = m_mouseX; y = m_mouseY; }
bool Input::GetMouseButton(int button) { return m_mouseButtons[button]; }
bool Input::GetMouseButtonDown(int button) { return m_mouseButtonsDown[button]; }

void Input::KeyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!m_keys[key]) m_keysDown[key] = true;
        m_keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        if (m_keys[key]) m_keysUp[key] = true;
        m_keys[key] = false;
    }
}

void Input::MousePosCallback(double xpos, double ypos) {
    // 首次进入时，初始化上一帧位置
    if (m_firstMouse) {
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
        m_firstMouse = false;
    }

    // 计算相对于上一帧的偏移
    double dx = xpos - m_lastMouseX;
    double dy = ypos - m_lastMouseY;

    // 累加到本帧的总偏移（因为一帧内可能收到多个鼠标事件）
    m_mouseDeltaX += dx;
    m_mouseDeltaY += dy;

    // 更新上一帧位置为当前位置（为下一帧准备）
    m_lastMouseX = xpos;
    m_lastMouseY = ypos;

    // 更新当前鼠标位置（供 GetMousePosition 使用）
    m_mouseX = xpos;
    m_mouseY = ypos;
}

void Input::MouseButtonCallback(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!m_mouseButtons[button]) m_mouseButtonsDown[button] = true;
        m_mouseButtons[button] = true;
    } else if (action == GLFW_RELEASE) {
        if (m_mouseButtons[button]) m_mouseButtonsUp[button] = true;
        m_mouseButtons[button] = false;
    }
}