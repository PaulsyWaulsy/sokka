#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <string>

#include "GUI.hpp"

const int DEFAULT_WIDTH = 1280;
const int DEFAULT_HEIGHT = 1280;

class EditorApp {
   public:
    explicit EditorApp(const std::string& title, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
    ~EditorApp();

    void run();

   private:
    void initSDL();
    void initOpenGL();
    void initImGui();
    void shutdown();

    void processEvents(bool& running);
    void init();
    void start();
    void update();
    void render();

   private:
    void setStyle(ImGuiIO& io);

    static constexpr const char* CUSTOM_FONT = "../assets/fonts/JetBrainsMonoNerdFontMono-Regular.ttf";

    bool running_;
    SDL_Window* window_;
    SDL_GLContext glContext_;
    std::unique_ptr<GUI> gui_;
    int width_;
    int height_;
    std::string title_;
};
