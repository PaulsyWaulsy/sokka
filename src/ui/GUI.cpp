#include "ui/GUI.hpp"

#include <SDL_opengl.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <imgui.h>

#include "core/Application.hpp"
#include "core/Base.hpp"
#include "core/Logger.hpp"

namespace Sokka {

GUI::GUI() {
    // initialise GUI
    if (!init()) {
        SOKKA_ERROR("Failed to initialise GUI");
    }
}

GUI::~GUI() {
    // close GUI
    close();
}
bool GUI::init() {
    Application& app = Application::get();
    window_ = app.getWindow().getNativeWindow();
    if (!window_) {
        SOKKA_ERROR("Failed to retrieve window from application inside GUI");
        return false;
    }

    glContext_ = app.getWindow().getGLContext();
    if (!glContext_) {
        SOKKA_ERROR("Failed to retrieve GLContext from application inside GUI");
        return false;
    }

    if (!initImGui()) {
        SOKKA_ERROR("Failed to initialise ImGui");
        return false;
    }
    return true;
}

void GUI::close() {
    // close GUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::render() {
    imGuiBegin();

    // Render logic here
    renderDockspace();
    renderDemo();

    imGuiEnd();
}

void GUI::renderDockspace() {
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpaceWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(2);

    // Main docking node
    ImGuiID dockspaceId = ImGui::GetID("SokkaDockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
    ImGui::End();
}

void GUI::renderDemo() { ImGui::ShowDemoWindow(); }

Unique<GUI> GUI::create() { return makeUnique<GUI>(); }

bool GUI::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // IO flags
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    setImGuiStyle();

    if (!ImGui_ImplSDL2_InitForOpenGL(window_, glContext_)) {
        SOKKA_ERROR("Failed to Initialise OpendGL for SDL");
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        SOKKA_ERROR("Failed to Initialise OpendGL for ImGui");
    }
    return true;
}
void GUI::imGuiBegin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::imGuiEnd() {
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window_);
}

void GUI::setImGuiStyle() {
    // Font
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(CUSTOM_FONT, 18.0f);
    if (!font) {
        SOKKA_ERROR("Unable to load font: ", CUSTOM_FONT);
        io.Fonts->AddFontDefault();
    } else {
        io.FontDefault = font;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsLight();

    // Removed rounding
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;

    // Scaling
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;
}

}  // namespace Sokka
