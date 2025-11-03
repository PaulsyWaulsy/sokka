#include "Canvas.hpp"

#include <cmath>
#include <iostream>

#include "imgui.h"

Canvas::Canvas(Tileset& tileset, int mapWidth, int mapHeight)
    : tileset_(tileset), mapWidth_(mapWidth), mapHeight_(mapHeight) {
    tiles_.resize(mapWidth_ * mapHeight_, -1);
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
        drawHover(draw_list, origin);
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
        if (zoom_ > 10.0f) zoom_ = 10.0f;
    }

    // --- Wheel scroll if not zooming ---
    else if (ImGui::IsWindowHovered()) {
        offset_.y += io.MouseWheel * 40.0f;
    }

    // --- Painting ---
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        if (tileset_.isLoaded() && tileset_.selectedTile() >= 0) {
            ImVec2 mouse = ImGui::GetMousePos();
            float localX = (mouse.x - origin.x - offset_.x) / (tileSize_ * zoom_);
            float localY = (mouse.y - origin.y - offset_.y) / (tileSize_ * zoom_);
            int tileX = static_cast<int>(localX);
            int tileY = static_cast<int>(localY);

            if (tileX >= 0 && tileY >= 0 && tileX < mapWidth_ && tileY < mapHeight_) {
                tiles_[tileY * mapWidth_ + tileX] = tileset_.selectedTile();
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
                           IM_COL32(50, 50, 50, 255));

    for (float y = fmodf(offset_.y, grid); y < size.y; y += grid)
        draw_list->AddLine(ImVec2(origin.x, origin.y + y), ImVec2(origin.x + size.x, origin.y + y),
                           IM_COL32(50, 50, 50, 255));
}

void Canvas::drawTiles(ImDrawList* draw_list, const ImVec2& origin) {
    if (!tileset_.isLoaded()) return;

    GLuint texID = tileset_.textureId();

    for (int y = 0; y < mapHeight_; ++y) {
        for (int x = 0; x < mapWidth_; ++x) {
            int index = tiles_[y * mapWidth_ + x];
            if (index < 0) continue;  // empty

            const TileUV& uv = tileset_.getTileUV(index);
            ImVec2 uv0(uv.u0, 1.0f - uv.v1);
            ImVec2 uv1(uv.u1, 1.0f - uv.v0);

            ImVec2 min(origin.x + offset_.x + x * tileSize_ * zoom_, origin.y + offset_.y + y * tileSize_ * zoom_);
            ImVec2 max(min.x + tileSize_ * zoom_, min.y + tileSize_ * zoom_);

            draw_list->AddImage((ImTextureID)(intptr_t)texID, min, max, uv0, uv1);
        }
    }
}

void Canvas::drawHover(ImDrawList* draw_list, const ImVec2& origin) {
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
