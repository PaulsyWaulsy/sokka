#include "sokka/core/Window.hpp"

#include "GL/glew.h"
#include "SDL_video.h"
#include "sokka/core/Logger.hpp"

namespace Sokka {

Window::Window(const WindowConfig& config) {
    // init window
    if (!init(config)) {
        SOKKA_ERROR("Failed to initialize Window");
    }
}

Window::~Window() {
    // close window
    close();
}

bool Window::init(const WindowConfig& config) {
    data_.title = config.title;
    data_.height = config.height;
    data_.width = config.width;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window_ = SDL_CreateWindow(data_.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               data_.width, data_.height, window_flags);

    if (!window_) {
        SOKKA_ERROR("Failed to create window: ", SDL_GetError());
        return false;
    }

    glContext_ = SDL_GL_CreateContext(window_);
    if (glContext_ == nullptr) {
        SOKKA_ERROR("Failed to create GL_Context: ", SDL_GetError());
        if (window_) SDL_DestroyWindow(window_);
        return false;
    }

    SDL_GL_MakeCurrent(window_, glContext_);
    SDL_GL_SetSwapInterval(data_.vsync ? 1 : 0);  // Enable vsync
    if (glewInit() != GLEW_OK) {
        SOKKA_ERROR("Failed to initialize GLEW");
        if (glContext_) SDL_GL_DeleteContext(glContext_);
        if (window_) SDL_DestroyWindow(window_);
        return false;
    }
    return true;
}

void Window::close() {
    if (glContext_) {
        SDL_GL_DeleteContext(glContext_);
        glContext_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

void Window::setVSync(bool enable) {
    data_.vsync = enable;
    int on = (enable) ? 1 : 0;
    SDL_GL_SetSwapInterval(on);  // Enable vsync
}

Unique<Window> Window::create(const WindowConfig& config) {
    // create window
    return makeUnique<Window>(config);
}

}  // namespace Sokka
