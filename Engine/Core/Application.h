#pragma once
#include <memory>

namespace Lumino {

class Window;

class Application {
public:
    Application();
    virtual ~Application();

    void Run();
    void Close();

    virtual void OnStart() {}           // 添加这个
    virtual void OnUpdate(float deltaTime) {}

protected:
    std::unique_ptr<Window> m_window;
    bool m_running = true;
};

}