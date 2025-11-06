#pragma once

#include <SDL_video.h>

#include "core/Base.hpp"

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

    static constexpr const char* CUSTOM_FONT =
        "../assets/fonts/JetBrainsMonoNerdFontMono-Regular.ttf";
};
}  // namespace Sokka
