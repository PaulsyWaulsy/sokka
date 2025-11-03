#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Texture.hpp"
#include "imgui.h"

struct TileUV {
    float u0, v0, u1, v1;
};

class Tileset {
   public:
    Tileset() = default;
    ~Tileset() = default;

    bool load(const std::string& path, int tileSize);
    void render();  // draw ImGui panel for selecting tiles

    bool isLoaded() const { return loaded_; }
    int selectedTile() const { return selectedTile_; }
    const TileUV& getSelectedUV() const { return tileUVs_[selectedTile_]; }
    const TileUV& getTileUV(int index) const { return tileUVs_[index]; }
    GLuint textureId() const { return texture_.id(); }

    int tileWidth() const { return tileSize_; }
    int tileHeight() const { return tileSize_; }

   private:
    void generateUVs();

    Texture texture_;
    std::vector<TileUV> tileUVs_;

    static constexpr int DEFUALT_TILE_SIZE = 8;

    bool loaded_ = false;
    int tileSize_ = DEFUALT_TILE_SIZE;
    int selectedTile_ = -1;
};
