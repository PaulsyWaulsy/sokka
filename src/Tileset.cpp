#include "Tileset.hpp"

#include "Logger.hpp"
#include "Texture.hpp"

bool Tileset::load(const std::string& path, int tileSize) {
    tileSize_ = tileSize;
    loaded_ = texture_.loadFromFile(path);
    if (!loaded_) {
        LOG_ERROR("Tileset Failed to load: ", path);
    }
    generateUVs();
    LOG_SUCCESS("Tileset Loaded: ", path, " (", texture_.width(), "x", texture_.height(), ")");

    LOG_INFO("Tileset ", path.c_str(), " Texture ID: ", texture_.id(), " (", texture_.width(), ", ", texture_.height(),
             ")");

    return true;
}

void Tileset::generateUVs() {
    tileUVs_.clear();
    int tilesX = texture_.width() / tileSize_;
    int tilesY = texture_.height() / tileSize_;

    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            float u0 = (x * tileSize_) / (float)texture_.width();
            float u1 = ((x + 1) * tileSize_) / (float)texture_.width();

            // Flip Y so that y=0 starts at the top of the image
            float v1 = 1.0f - (y * tileSize_) / (float)texture_.height();
            float v0 = 1.0f - ((y + 1) * tileSize_) / (float)texture_.height();

            tileUVs_.push_back({u0, v0, u1, v1});
        }
    }
}
