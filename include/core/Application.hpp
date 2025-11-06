#pragma once

#include "core/Base.hpp"
#include "core/Window.hpp"
#include "ui/GUI.hpp"

namespace Sokka {
class Application {
public:
    Application();
    ~Application();

    void run();

    static Application& get() { return *Instance; }
    Window& getWindow() { return *window_; }

private:
    bool init();
    void start();
    void close();
    void render();
    void processEvents();

    static Application* Instance;
    bool running_;
    Unique<Window> window_;
    Unique<GUI> gui_;
};
}  // namespace Sokka
