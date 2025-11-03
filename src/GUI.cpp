#include "GUI.hpp"

#include <iostream>

#include "ImGuiFileDialog.h"
#include "SDL.h"
#include "imgui.h"

GUI::GUI() : canvas_(tileset_) {}

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

void GUI::renderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New...")) {
                std::cout << "New map\n";
            }
            if (ImGui::MenuItem("Open...")) {
                std::cout << "Open map\n";
            }

            // Open file dialog
            ImGui::Separator();
            if (ImGui::MenuItem("Save")) {
                std::cout << "Save map\n";
            }
            if (ImGui::MenuItem("Save As...")) {
                std::cout << "Save map\n";
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Import")) {
                std::cout << "Import\n";
                IGFD::FileDialogConfig config;

                // HACK: hard coded for easier use
                config.path = "../assets/sprites/";
                ImGuiFileDialog::Instance()->OpenDialog("ChooseTileset", "Choose File", ".png,.jpep", config);
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                SDL_Event quitEvent;
                quitEvent.type = SDL_QUIT;
                SDL_PushEvent(&quitEvent);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // For opening dialog
    if (ImGuiFileDialog::Instance()->Display("ChooseTileset")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            tileset_.load(filePath, 8);  // or use your chosen tile size
            std::cout << "[GUI] Imported tileset: " << filePath << std::endl;
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

void GUI::renderToolbar() {
    ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::RadioButton("Brush", currentTool_ == EditorTool::Brush)) currentTool_ = EditorTool::Brush;
    if (ImGui::RadioButton("Eraser", currentTool_ == EditorTool::Eraser)) currentTool_ = EditorTool::Eraser;
    if (ImGui::RadioButton("Fill", currentTool_ == EditorTool::Fill)) currentTool_ = EditorTool::Fill;
    if (ImGui::RadioButton("Select", currentTool_ == EditorTool::Select)) currentTool_ = EditorTool::Select;

    ImGui::End();
}

void GUI::renderCanvas() { canvas_.render(); }

void GUI::renderTileset() { tileset_.render(); }
