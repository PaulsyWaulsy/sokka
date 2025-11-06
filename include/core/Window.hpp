#pragma once

#include <SDL2/SDL.h>
#include <SDL_video.h>

#include <string>

#include "core/Base.hpp"

namespace Sokka {

inline constexpr int DEFAULT_WINDOW_WIDTH = 1280;
inline constexpr int DEFAULT_WINDOW_HEIGHT = 720;

struct WindowConfig {
    std::string title = "Sokka Level Editor";
    unsigned int width = DEFAULT_WINDOW_WIDTH;
    unsigned int height = DEFAULT_WINDOW_HEIGHT;
};

class Window {
public:
    Window(const WindowConfig& config);
    ~Window();

    unsigned int GetWidth() const { return data_.width; }
    unsigned int GetHeight() const { return data_.height; }

    bool init(const WindowConfig& config = WindowConfig());
    void close();

    void setVSync(bool enable);
    bool isVSync() const { return data_.vsync; }

    static Unique<Window> create(const WindowConfig& config = WindowConfig());

    SDL_Window* getNativeWindow() { return window_; }
    SDL_GLContext getGLContext() { return glContext_; }

private:
    // Only need the one window
    SDL_Window* window_;
    SDL_GLContext glContext_;

    struct WindowData {
        std::string title;
        unsigned int width, height;
        bool vsync = true;  // true by defualt
    };

    WindowData data_;
};

}  // namespace Sokka
