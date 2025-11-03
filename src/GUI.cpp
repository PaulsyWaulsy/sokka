#include "GUI.hpp"

#include <iostream>

#include "ImGuiFileDialog.h"
#include "SDL.h"
#include "imgui.h"

GUI::GUI() : canvas_(tileset_), menubar_(tileset_) {}

void GUI::render() {
    renderMenuBar();
    renderDockspace();
    renderCanvas();
    renderTileset();
}

void GUI::renderDockspace() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    // Main docking node
    ImGuiID dockspace_id = ImGui::GetID("SokkaDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::End();
}

void GUI::renderToolbar() {
    ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::RadioButton("Brush", currentTool_ == EditorTool::Brush)) currentTool_ = EditorTool::Brush;
    if (ImGui::RadioButton("Eraser", currentTool_ == EditorTool::Eraser)) currentTool_ = EditorTool::Eraser;
    if (ImGui::RadioButton("Fill", currentTool_ == EditorTool::Fill)) currentTool_ = EditorTool::Fill;
    if (ImGui::RadioButton("Select", currentTool_ == EditorTool::Select)) currentTool_ = EditorTool::Select;

    ImGui::End();
}

void GUI::renderMenuBar() { menubar_.render(); }

void GUI::renderCanvas() { canvas_.render(); }

void GUI::renderTileset() { tileset_.render(); }
