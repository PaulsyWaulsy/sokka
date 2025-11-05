#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Texture.hpp"
#include "imgui.h"

struct TileUV {
    float u0, v0, u1, v1;
};

class Tileset {
   public:
    Tileset() = default;
    ~Tileset() { LOG_WARN("Tileset deleted: ", textureId()); };

    bool load(const std::string& path, int tileSize = DEFUALT_TILE_SIZE);

    bool isLoaded() const { return loaded_; }
    int selectedTile() const { return selectedTile_; }
    void setSelectedTile(int index) { selectedTile_ = index; }
    const TileUV& getSelectedUV() const { return tileUVs_[selectedTile_]; }
    const TileUV& getTileUV(int index) const { return tileUVs_[index]; }
    const std::vector<TileUV> getTileUVs() const { return tileUVs_; }
    GLuint textureId() const { return texture_.id(); }

    int tileWidth() const { return tileSize_; }
    int tileHeight() const { return tileSize_; }

    int textureWidth() const { return texture_.width(); }
    int textureHeight() const { return texture_.height(); }

    int tileSize() const { return tileSize_; }

   private:
    void generateUVs();

    Texture texture_;
    std::vector<TileUV> tileUVs_;

    static constexpr int DEFUALT_TILE_SIZE = 8;

    bool loaded_ = false;
    int tileSize_ = DEFUALT_TILE_SIZE;
    int selectedTile_ = -1;
};
