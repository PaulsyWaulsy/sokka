#include "sokka/ui/Menubar.hpp"

#include <ImGuiFileDialog.h>
#include <SDL_events.h>
#include <imgui.h>

#include "sokka/core/Base.hpp"
#include "sokka/core/Logger.hpp"

namespace Sokka {

void Menubar::render() {
    if (!ImGui::BeginMainMenuBar()) return;

    renderFile();
    renderEdit();
    renderView();

    ImGui::EndMainMenuBar();
}

void Menubar::renderFile() {
    if (!ImGui::BeginMenu("File")) return;

    if (ImGui::MenuItem("New...")) SOKKA_INFO("New map");
    if (ImGui::MenuItem("Open...")) SOKKA_INFO("Open map");

    // Open file dialog
    ImGui::Separator();
    if (ImGui::MenuItem("Save")) SOKKA_INFO("Save map");
    if (ImGui::MenuItem("Save As...")) SOKKA_INFO("Save map");

    ImGui::Separator();
    if (ImGui::MenuItem("Import")) openImportDialog();

    ImGui::Separator();
    if (ImGui::MenuItem("Exit")) requestExitSDL();

    ImGui::EndMenu();
}

void Menubar::renderEdit() {
    if (!ImGui::BeginMenu("Edit")) return;

    if (ImGui::MenuItem("Undo")) SOKKA_INFO("Undo");
    if (ImGui::MenuItem("Redo")) SOKKA_INFO("Redo");

    ImGui::Separator();
    if (ImGui::MenuItem("Settings")) SOKKA_INFO("Open Settings");

    ImGui::EndMenu();
}
void Menubar::renderView() {
    if (!ImGui::BeginMenu("View")) return;

    ImGui::EndMenu();
}

void Menubar::openImportDialog() {
    SOKKA_INFO("Import");
    IGFD::FileDialogConfig config;

    config.path = SPRITE_FILE_PATH;
    ImGuiFileDialog::Instance()->OpenDialog("ChooseTileset", "Choose File", ".png,.jpep", config);
}

void Menubar::requestExitSDL() {
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);
}

Unique<Menubar> Menubar::create() { return makeUnique<Menubar>(); }

}  // namespace Sokka
//
//
