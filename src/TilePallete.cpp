#include "TilePallete.hpp"

#include <vector>

#include "Logger.hpp"

TilePallete::TilePallete() : autoTiler_() {
    if (!autoTiler_.getTilesets().empty()) {
        selectedTileset_ = autoTiler_.getTilesets().begin()->first;  // pick first by default
    }
}

void TilePallete::render() {
    ImGui::Begin("Tile Palette");

    // --- 1. Dropdown to choose tileset ---
    if (ImGui::BeginCombo("Tileset", selectedTileset_.c_str())) {
        for (auto& [id, tileset] : autoTiler_.getTilesets()) {
            bool isSelected = (id == selectedTileset_);
            if (ImGui::Selectable(id.c_str(), isSelected)) {
                selectedTileset_ = id;
                selectedTile_ = -1;  // reset tile selection
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // --- 2. Draw grid of tiles ---
    if (!selectedTileset_.empty()) {
        auto& tileset = autoTiler_.getTilesets().at(selectedTileset_);

        if (!tileset.isLoaded()) {
            ImGui::TextDisabled("Tileset not loaded.");
        } else {
            // Available width in the panel
            ImVec2 avail = ImGui::GetContentRegionAvail();

            // Determine how many tiles per row based on available width
            int tilesize = tileset.tileSize();
            int tilesX = tileset.textureWidth() / tileset.tileWidth();
            // int tilesY = texture_.height() / tileSize_;

            // Compute tile display scale to fit horizontally with a small margin
            float totalWidth = (float)tilesX * tileset.tileWidth();
            float displayScale = avail.x / totalWidth;

            // Clamp scaling so tiles aren't too tiny or massive
            if (displayScale < 0.25f) displayScale = 0.25f;
            if (displayScale > 6.0f) displayScale = 6.0f;

            ImVec2 displaySize(tilesize * displayScale, tilesize * displayScale);

            // Begin scrolling region if needed
            ImGui::BeginChild("TilesetScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            // Draw tile grid
            std::vector<TileUV> tileUVs = tileset.getTileUVs();
            for (int i = 0; i < (int)tileUVs.size(); ++i) {
                const TileUV& uv = tileUVs[i];
                ImVec2 uv0(uv.u0, 1.0f - uv.v1);
                ImVec2 uv1(uv.u1, 1.0f - uv.v0);

                ImGui::PushID(i);
                // LOG_INFO("Texture ID: ", tileset.textureId());
                if (ImGui::ImageButton("tile", (ImTextureID)(intptr_t)tileset.textureId(), displaySize, uv0, uv1)) {
                    selectedTile_ = i;
                    LOG_INFO("Tileset Selected tile ", i);
                }
                ImGui::PopID();

                // Highlight selected tile
                if (selectedTile_ == i) {
                    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                                                        IM_COL32(255, 255, 0, 255), 0.0f, 0, 2.0f);
                }

                // Same line until the end of a row
                if ((i + 1) % tilesX != 0) ImGui::SameLine();
            }

            ImGui::EndChild();
        }
    }

    ImGui::End();
}

Tileset* TilePallete::getSelectedTileset() {
    if (selectedTileset_.empty()) return nullptr;
    auto& tilesets = autoTiler_.getTilesets();
    auto it = tilesets.find(selectedTileset_);
    return (it != tilesets.end()) ? &it->second : nullptr;
}
