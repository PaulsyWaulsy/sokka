#pragma once

#include "SDL_video.h"
#include "sokka/core/Base.hpp"
#include "sokka/ui/Menubar.hpp"

namespace Sokka {

class GUI {
public:
    GUI();
    ~GUI();
    bool init();
    void close();
    void render();

    static Unique<GUI> create();

private:
    bool initImGui();
    void imGuiBegin();
    void imGuiEnd();
    void setImGuiStyle();

    // Render components
    void renderDockspace();
    void renderDemo();

    SDL_Window* window_;
    SDL_GLContext glContext_;

    Unique<Menubar> menubar_;

    static constexpr const char* CUSTOM_FONT = "assets/fonts/JetBrainsMonoNerdFontMono-Regular.ttf";
};
}  // namespace Sokka
