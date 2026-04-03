#pragma once

namespace Lumino {

class Input {
public:
static bool IsKeyJustPressed(int keycode);

    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);
    static void GetMousePosition(double& x, double& y);
};

}