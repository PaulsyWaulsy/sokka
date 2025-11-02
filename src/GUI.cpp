#include "GUI.hpp"

#include <iostream>

#include "SDL.h"
#include "imgui.h"

GUI::GUI() {}

void GUI::render() {
    renderMenuBar();
    renderDockspace();
    renderPanels();
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
            if (ImGui::MenuItem("New Map")) {
                std::cout << "New map\n";
            }
            if (ImGui::MenuItem("Open...")) {
                std::cout << "Open map\n";
            }
            if (ImGui::MenuItem("Save")) {
                std::cout << "Save map\n";
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
}

void GUI::renderPanels() {
    if (ImGui::Begin("Tileset")) {
        ImGui::Text("Tileset Browser");
        ImGui::Separator();
        ImGui::Text("Display available tiles here");
    }
    ImGui::End();

    if (ImGui::Begin("Canvas")) {
        ImGui::Text("Tilemap Canvas");
        ImGui::Separator();
        ImGui::Text("Here the map would be drawn and edited");
    }
    ImGui::End();

    if (ImGui::Begin("Inspector")) {
        ImGui::Text("Inspector Panel");
        ImGui::Separator();
        ImGui::Text("Tile / Entity properties");
    }
    ImGui::End();
}
