#include "Input.h"
#include <GLFW/glfw3.h>
#include <cstring>

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
    // Reset per-frame states
    memset(m_keysDown, 0, sizeof(m_keysDown));
    memset(m_keysUp, 0, sizeof(m_keysUp));
    memset(m_mouseButtonsDown, 0, sizeof(m_mouseButtonsDown));
    memset(m_mouseButtonsUp, 0, sizeof(m_mouseButtonsUp));
    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;

    // Store previous mouse position for delta calculation
    m_lastMouseX = m_mouseX;
    m_lastMouseY = m_mouseY;
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
    if (m_firstMouse) {
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
        m_firstMouse = false;
    }
    m_mouseX = xpos;
    m_mouseY = ypos;
    m_mouseDeltaX = xpos - m_lastMouseX;
    m_mouseDeltaY = ypos - m_lastMouseY;
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