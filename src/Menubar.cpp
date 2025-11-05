#include "Menubar.hpp"

#include <SDL.h>

#include "ImGuiFileDialog.h"
#include "Logger.hpp"
#include "imgui.h"

void Menubar::render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New...")) {
                LOG_INFO("New map");
            }
            if (ImGui::MenuItem("Open...")) {
                LOG_INFO("Open map");
            }

            // Open file dialog
            ImGui::Separator();
            if (ImGui::MenuItem("Save")) {
                LOG_INFO("Save map");
            }
            if (ImGui::MenuItem("Save As...")) {
                LOG_INFO("Save map");
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Import")) {
                LOG_INFO("Import");
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

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
                LOG_INFO("Undo");
            }

            if (ImGui::MenuItem("Redo")) {
                LOG_INFO("Redo");
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Settings")) {
                LOG_INFO("Open Settings");
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Map")) {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
