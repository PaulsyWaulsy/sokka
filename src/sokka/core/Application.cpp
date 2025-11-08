#include "sokka/core/Application.hpp"

#include "SDL2/SDL.h"
#include "imgui_impl_sdl2.h"
#include "sokka/core/Input.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/core/Window.hpp"
#include "sokka/graphics/Renderer.hpp"

namespace Sokka {

Application* Application::Instance = nullptr;

Application::Application() {
    if (!init()) {
        SOKKA_ERROR("Unable to initialise Application");
    }
    SOKKA_SUCCESS("Starting Sokka Editor");
}

Application::~Application() { Instance = nullptr; }

void Application::run() {
    running_ = true;
    start();
    while (running_) {
        time_.update();

        processEvents();
        update(time_.getDeltaTime());
        render();

        if (!window_->isVSync()) {
            time_.limitFPS();
        }
    }
}

bool Application::init() {
    if (Instance) {
        SOKKA_INFO("Application already exists");
        return false;
    }
    Instance = this;

    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
        SOKKA_ERROR("Failed to initialize SDL: ", SDL_GetError());
        return false;
    }

    window_ = Window::create();
    if (!window_) {
        SOKKA_ERROR("Failed to create Window");
    }

    gui_ = GUI::create();
    if (!gui_) {
        SOKKA_ERROR("Failed to create GUI");
    }

    Logger::init("logs/editor.log");
    Renderer::init();
    return true;
}

void Application::start() {}

void Application::update(float deltaTime) {
    // Update logic
    gui_->update(deltaTime);
}

void Application::close() {
    // stop running
    Renderer::close();
    SDL_Quit();
}

void Application::render() { gui_->render(); }

void Application::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        Input::processEvent(&event);

        if (event.type == SDL_QUIT) running_ = false;

        // NOTE: close on escape key pressed
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running_ = false;
        }
    }

    // update inputs
    Input::update();
}

}  // namespace Sokka
