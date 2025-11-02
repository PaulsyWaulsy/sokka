#include "EditorApp.hpp"

#include <SDL_opengl.h>

#include <iostream>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"

EditorApp::EditorApp(const std::string& title, int width, int height)
    : window_(nullptr), glContext_(nullptr), width_(width), height_(height), title_(title) {}

EditorApp::~EditorApp() { shutdown(); }

void EditorApp::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window_ =
        SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, window_flags);

    if (!window_) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glContext_ = SDL_GL_CreateContext(window_);
    if (glContext_ == nullptr) {
        std::cerr << "Failed to create GL_Context: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    SDL_GL_MakeCurrent(window_, glContext_);
    SDL_GL_SetSwapInterval(1);  // Enable vsync
}

void EditorApp::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontFromFileTTF("../assets/fonts/JetBrainsMonoNerdFontMono-Regular.ttf", 18.0f);

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);

    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void EditorApp::initOpenGL() {
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
}

void EditorApp::run() {
    initSDL();
    initOpenGL();
    initImGui();

    running_ = true;
    while (running_) {
        processEvents(running_);
        update();
        render();
    }
}

void EditorApp::processEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) running = false;
    }
}

void EditorApp::update() {
    // Update game/editor logic here
}

void EditorApp::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    gui_.render();

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window_);
}

void EditorApp::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (glContext_) SDL_GL_DeleteContext(glContext_);
    if (window_) SDL_DestroyWindow(window_);
    SDL_Quit();
}
