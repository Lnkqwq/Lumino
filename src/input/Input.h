#pragma once

class Input {
public:
    static void Update();
    static bool GetKey(int key);
    static bool GetKeyDown(int key);
    static bool GetKeyUp(int key);
    static void GetMouseDelta(float& dx, float& dy);
    static void GetMousePosition(double& x, double& y);
    static bool GetMouseButton(int button);
    static bool GetMouseButtonDown(int button);

    // Callbacks from GLFW
    static void KeyCallback(int key, int scancode, int action, int mods);
    static void MousePosCallback(double xpos, double ypos);
    static void MouseButtonCallback(int button, int action, int mods);

private:
    static bool m_keys[512];
    static bool m_keysDown[512];
    static bool m_keysUp[512];
    static double m_mouseX, m_mouseY;
    static double m_lastMouseX, m_lastMouseY;
    static double m_mouseDeltaX, m_mouseDeltaY;
    static bool m_mouseButtons[8];
    static bool m_mouseButtonsDown[8];
    static bool m_mouseButtonsUp[8];
    static bool m_firstMouse;
};