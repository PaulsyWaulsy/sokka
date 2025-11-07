#pragma once

#include "sokka/core/Base.hpp"
#include "sokka/core/Time.hpp"
#include "sokka/core/Window.hpp"
#include "sokka/ui/GUI.hpp"

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
    void update(float deltaTime);
    void render();
    void processEvents();

    static Application* Instance;
    bool running_;
    Time time_;
    Unique<Window> window_;
    Unique<GUI> gui_;
};
}  // namespace Sokka
