#include "Tileset.hpp"

#include "Logger.hpp"

Tileset::Tileset() {
    manager_ = loadTilesJSON("../assets/map/Foreground.json");
    LOG_SUCCESS("Loaded tile sets");
}

bool Tileset::load(const std::string& path, int tileSize) {
    // tileSize_ = tileSize;
    // loaded_ = texture_.loadFromFile(path);
    // if (!loaded_) {
    //     LOG_ERROR("Tileset Failed to load: ", path);
    // }
    // generateUVs();
    // LOG_SUCCESS("Tileset Loaded: ", path, " (", texture_.width(), "x", texture_.height(), ")");

    return true;
}

void Tileset::generateUVs() {
    tileUVs_.clear();
    int tilesX = texture_.width() / tileSize_;
    int tilesY = texture_.height() / tileSize_;

    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            float u0 = (x * tileSize_) / (float)texture_.width();
            float v0 = (y * tileSize_) / (float)texture_.height();
            float u1 = ((x + 1) * tileSize_) / (float)texture_.width();
            float v1 = ((y + 1) * tileSize_) / (float)texture_.height();
            tileUVs_.push_back({u0, v0, u1, v1});
        }
    }
}

void Tileset::render() {
    if (!loaded_) {
        ImGui::Begin("Tileset");
        ImGui::Text("No tileset loaded.");
        ImGui::End();
        return;
    }

    ImGui::Begin("Tileset");

    // Panel header
    ImGui::Text("Tileset (%d tiles)", (int)tileUVs_.size());
    ImGui::Separator();

    // Available width in the panel
    ImVec2 avail = ImGui::GetContentRegionAvail();

    // Determine how many tiles per row based on available width
    int tilesX = texture_.width() / tileSize_;
    // int tilesY = texture_.height() / tileSize_;

    // Compute tile display scale to fit horizontally with a small margin
    float totalWidth = (float)tilesX * tileSize_;
    float displayScale = avail.x / totalWidth;

    // Clamp scaling so tiles aren't too tiny or massive
    if (displayScale < 0.25f) displayScale = 0.25f;
    if (displayScale > 6.0f) displayScale = 6.0f;

    ImVec2 displaySize(tileSize_ * displayScale, tileSize_ * displayScale);

    // Begin scrolling region if needed
    ImGui::BeginChild("TilesetScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Draw tile grid
    for (int i = 0; i < (int)tileUVs_.size(); ++i) {
        const TileUV& uv = tileUVs_[i];
        ImVec2 uv0(uv.u0, 1.0f - uv.v1);
        ImVec2 uv1(uv.u1, 1.0f - uv.v0);

        ImGui::PushID(i);
        if (ImGui::ImageButton("tile", (ImTextureID)(intptr_t)texture_.id(), displaySize, uv0, uv1)) {
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
    ImGui::End();
}
