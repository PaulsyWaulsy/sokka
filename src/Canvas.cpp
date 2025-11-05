#include "Canvas.hpp"

#include <array>
#include <cmath>

#include "TilePallete.hpp"
#include "imgui.h"

Canvas::Canvas(TilePallete& tilePallete, int mapWidth, int mapHeight)
    : tilePallete_(tilePallete), autoTiler_(tilePallete.getAutoTiler()), mapWidth_(mapWidth), mapHeight_(mapHeight) {
    tiles_.resize(mapWidth_ * mapHeight_, -1);

    currentTileset_ = tilePallete_.getSelectedTileset();
    // TODO: add auto tiling rules
}

void Canvas::render() {
    // Create ImGui window
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    if (ImGui::Begin("Canvas", nullptr, flags)) {
        ImVec2 origin = ImGui::GetCursorScreenPos();
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        handleInput(origin);
        drawGrid(draw_list, origin, windowSize);
        drawTiles(draw_list, origin);
        drawHover(origin);

        // --- Mouse position info ---
        if (ImGui::IsWindowHovered()) {
            ImVec2 mouse = ImGui::GetMousePos();
            float localX = (mouse.x - origin.x - offset_.x) / (tileSize_ * zoom_);
            float localY = (mouse.y - origin.y - offset_.y) / (tileSize_ * zoom_);
            int tileX = static_cast<int>(floor(localX));
            int tileY = static_cast<int>(floor(localY));

            ImGui::Separator();
            ImGui::Text("Mouse (screen): [%.1f, %.1f]", mouse.x, mouse.y);
            ImGui::Text("Mouse (tile):   [%d, %d]", tileX, tileY);
        }
    }
    ImGui::End();
}

void Canvas::handleInput(const ImVec2& origin) {
    ImGuiIO& io = ImGui::GetIO();

    // --- Mouse drag to pan ---
    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl) &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 drag = io.MouseDelta;
        offset_.x += drag.x;
        offset_.y += drag.y;
    }

    // --- Mouse wheel zoom (Ctrl + wheel) ---
    if (ImGui::IsWindowHovered() && io.KeyCtrl) {
        zoom_ += io.MouseWheel * 0.1f;
        if (zoom_ < 0.1f) zoom_ = 0.1f;
        if (zoom_ > 7.5f) zoom_ = 7.5f;
    }

    // --- Wheel scroll if not zooming ---
    else if (ImGui::IsWindowHovered()) {
        offset_.y += io.MouseWheel * 40.0f;
    }

    // --- Painting ---
    if (!currentTileset_ || !currentTileset_->isLoaded()) return;

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        if (currentTileset_->isLoaded() && currentTileset_->selectedTile() >= 0) {
            ImVec2 mouse = ImGui::GetMousePos();
            float localX = (mouse.x - origin.x - offset_.x) / (tileSize_ * zoom_);
            float localY = (mouse.y - origin.y - offset_.y) / (tileSize_ * zoom_);
            int tileX = static_cast<int>(localX);
            int tileY = static_cast<int>(localY);

            if (tileX >= 0 && tileY >= 0 && tileX < mapWidth_ && tileY < mapHeight_) {
                tiles_[tileY * mapWidth_ + tileX] = currentTileset_->selectedTile();
            }
        }
    }

    // --- Erasing ---
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        ImVec2 mouse = ImGui::GetMousePos();
        float localX = (mouse.x - origin.x - offset_.x) / (tileSize_ * zoom_);
        float localY = (mouse.y - origin.y - offset_.y) / (tileSize_ * zoom_);
        int tileX = static_cast<int>(localX);
        int tileY = static_cast<int>(localY);

        if (tileX >= 0 && tileY >= 0 && tileX < mapWidth_ && tileY < mapHeight_) {
            tiles_[tileY * mapWidth_ + tileX] = -1;  // erase
        }
    }
}

void Canvas::drawGrid(ImDrawList* draw_list, const ImVec2& origin, const ImVec2& size) {
    const float grid = tileSize_ * zoom_;
    for (float x = fmodf(offset_.x, grid); x < size.x; x += grid)
        draw_list->AddLine(ImVec2(origin.x + x, origin.y), ImVec2(origin.x + x, origin.y + size.y),
                           IM_COL32(50, 50, 50, 128));

    for (float y = fmodf(offset_.y, grid); y < size.y; y += grid)
        draw_list->AddLine(ImVec2(origin.x, origin.y + y), ImVec2(origin.x + size.x, origin.y + y),
                           IM_COL32(50, 50, 50, 128));
}

void Canvas::drawTiles(ImDrawList* draw_list, const ImVec2& origin) {
    if (!currentTileset_->isLoaded()) return;

    GLuint texID = currentTileset_->textureId();

    for (int y = 0; y < mapHeight_; ++y) {
        for (int x = 0; x < mapWidth_; ++x) {
            int index = tiles_[y * mapWidth_ + x];
            if (index < 0) continue;  // empty

            const TileUV& uv = currentTileset_->getTileUV(index);
            ImVec2 uv0(uv.u0, 1.0f - uv.v1);
            ImVec2 uv1(uv.u1, 1.0f - uv.v0);

            ImVec2 min(origin.x + offset_.x + x * tileSize_ * zoom_, origin.y + offset_.y + y * tileSize_ * zoom_);
            ImVec2 max(min.x + tileSize_ * zoom_, min.y + tileSize_ * zoom_);

            draw_list->AddImage((ImTextureID)(intptr_t)texID, min, max, uv0, uv1);
        }
    }
}

void Canvas::drawHover(const ImVec2& origin) {
    // --- Hover effect ---
    if (ImGui::IsWindowHovered()) {
        ImVec2 mouse = ImGui::GetMousePos();

        // Convert mouse position to local grid coordinates
        float localX = (mouse.x - origin.x - offset_.x) / (tileSize_ * zoom_);
        float localY = (mouse.y - origin.y - offset_.y) / (tileSize_ * zoom_);
        int tileX = static_cast<int>(floor(localX));
        int tileY = static_cast<int>(floor(localY));

        // Only draw if inside bounds
        if (tileX >= 0 && tileY >= 0 && tileX < mapWidth_ && tileY < mapHeight_) {
            ImVec2 min(origin.x + offset_.x + tileX * tileSize_ * zoom_,
                       origin.y + offset_.y + tileY * tileSize_ * zoom_);
            ImVec2 max(min.x + tileSize_ * zoom_, min.y + tileSize_ * zoom_);

            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddRectFilled(min, max, IM_COL32(0, 120, 255, 50));            // translucent blue fill
            dl->AddRect(min, max, IM_COL32(0, 120, 255, 200), 0.0f, 0, 2.0f);  // blue border
        }
    }
}

int Canvas::computeMask(int x, int y) const {
    int mask = 0;

    auto get = [&](int tx, int ty) {
        if (tx < 0 || ty < 0 || tx >= mapWidth_ || ty >= mapHeight_) return false;
        return tiles_[ty * mapWidth_ + tx] != -1;
    };

    if (get(x - 1, y - 1)) mask |= 1;    // Up-Left
    if (get(x, y - 1)) mask |= 2;        // Up
    if (get(x + 1, y - 1)) mask |= 4;    // Up-Right
    if (get(x - 1, y)) mask |= 8;        // Left
    if (get(x + 1, y)) mask |= 16;       // Right
    if (get(x - 1, y + 1)) mask |= 32;   // Down-Left
    if (get(x, y + 1)) mask |= 64;       // Down
    if (get(x + 1, y + 1)) mask |= 128;  // Down-Right

    return mask;
}

void Canvas::updateAutoTiles(int x, int y) {
    const std::array<ImVec2, 5> neighbors = {ImVec2(x, y), ImVec2(x + 1, y), ImVec2(x - 1, y), ImVec2(x, y + 1),
                                             ImVec2(x, y - 1)};

    for (auto& n : neighbors) {
        int tx = (int)n.x;
        int ty = (int)n.y;
        if (tx < 0 || ty < 0 || tx >= mapWidth_ || ty >= mapHeight_) continue;

        // FIX:
        if (tiles_[ty * mapWidth_ + tx] != -1) {
            int mask = computeMask(tx, ty);
            // int newTile = autocurrentTileset_.getTileForMask(mask);
            // if (newTile != -1) tiles_[ty * mapWidth_ + tx] = newTile;
        }
    }
}
