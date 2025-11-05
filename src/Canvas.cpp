#include "Canvas.hpp"

#include <array>
#include <bitset>
#include <cmath>

#include "Logger.hpp"
#include "TilePallete.hpp"
#include "Tileset.hpp"
#include "imgui.h"

Canvas::Canvas(TilePallete& tilePallete, int mapWidth, int mapHeight)
    : tilePallete_(tilePallete), autoTiler_(tilePallete.getAutoTiler()), mapWidth_(mapWidth), mapHeight_(mapHeight) {
    tiles_.resize(mapWidth_ * mapHeight_, -1);

    currentTileset_ = tilePallete_.getSelectedTileset();
    currentAutoTileset_ = tilePallete_.getSelectedAutoTileset();
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
    if (!currentTileset_ || !currentTileset_->isLoaded()) {
        LOG_WARN("Current Tileset is NULL or not loaded");
        return;
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        if (currentTileset_->isLoaded() && currentTileset_->selectedTile() >= 0) {
            ImVec2 mouse = ImGui::GetMousePos();
            float localX = (mouse.x - origin.x - offset_.x) / (tileSize_ * zoom_);
            float localY = (mouse.y - origin.y - offset_.y) / (tileSize_ * zoom_);
            int tileX = static_cast<int>(localX);
            int tileY = static_cast<int>(localY);

            if (tileX >= 0 && tileY >= 0 && tileX < mapWidth_ && tileY < mapHeight_) {
                // Compute the surrounding bitmask
                int mask = computeMask(tileX, tileY);
                int tileIndex = currentAutoTileset_->getTileForMask(mask, tileX, tileY);
                tiles_[tileY * mapWidth_ + tileX] = tileIndex;

                std::bitset<8> maskBits(mask);

                LOG_INFO("[AutoTile] Painted tile at (", tileX, ", ", tileY,
                         ") | "
                         "Mask: ",
                         (int)mask, " (", maskBits,
                         ") | "
                         "Tile Index: ",
                         tileIndex);

                // Update neighboring tiles
                updateAutoTiles(tileX, tileY);
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
            updateAutoTiles(tileX, tileY);
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

int Canvas::checkCenter(int x, int y) const {
    auto get = [&](int tx, int ty) {
        if (tx < 0 || ty < 0 || tx >= mapWidth_ || ty >= mapHeight_) return -1;  // treat out of bounds as empty
        return tiles_[ty * mapWidth_ + tx];
    };

    // Check two tiles away in each cardinal direction
    bool topEmpty = get(x, y - 2) == -1;
    bool bottomEmpty = get(x, y + 2) == -1;
    bool leftEmpty = get(x - 2, y) == -1;
    bool rightEmpty = get(x + 2, y) == -1;

    // Return 1 if any direction is empty, 0 otherwise
    return (topEmpty || bottomEmpty || leftEmpty || rightEmpty) ? 0 : 1;
}

int Canvas::computeMask(int x, int y) const {
    int mask = 0;

    auto get = [&](int tx, int ty) {
        if (tx < 0 || ty < 0 || tx >= mapWidth_ || ty >= mapHeight_) return false;
        return tiles_[ty * mapWidth_ + tx] != -1;
    };

    // Bit order: Up-Left (bit 7) → Down-Right (bit 0)
    if (get(x - 1, y - 1)) mask |= 1 << 7;  // Up-Left
    if (get(x, y - 1)) mask |= 1 << 6;      // Up
    if (get(x + 1, y - 1)) mask |= 1 << 5;  // Up-Right
    if (get(x - 1, y)) mask |= 1 << 4;      // Left
    if (get(x + 1, y)) mask |= 1 << 3;      // Right
    if (get(x - 1, y + 1)) mask |= 1 << 2;  // Down-Left
    if (get(x, y + 1)) mask |= 1 << 1;      // Down
    if (get(x + 1, y + 1)) mask |= 1 << 0;  // Down-Right

    return mask;
}

void Canvas::updateAutoTiles(int x, int y) {
    if (!currentAutoTileset_) {
        LOG_ERROR("Auto Tileset is NULL");
        return;
    }

    const int radius = 2;  // 5x5 area → radius 2

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int tx = x + dx;
            int ty = y + dy;

            if (tx < 0 || ty < 0 || tx >= mapWidth_ || ty >= mapHeight_) continue;

            int index = ty * mapWidth_ + tx;
            if (tiles_[index] == -1) continue;  // skip empty cells

            int mask = computeMask(tx, ty);
            int newTile = currentAutoTileset_->getTileForMask(mask, tx, ty);

            if (newTile != -1) tiles_[index] = newTile;
        }
    }
}
