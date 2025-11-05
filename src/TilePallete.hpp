#pragma once
#include "AutoTiler.hpp"
class TilePallete {
   public:
    TilePallete();
    ~TilePallete() = default;
    void render();  // draw ImGui panel for selecting tiles

    const std::string& selectedTileset() const { return selectedTileset_; }
    AutoTiler& getAutoTiler() { return autoTiler_; }
    int selectedTile() const { return selectedTile_; }

    Tileset* getSelectedTileset();
    AutoTileSet* getSelectedAutoTileset();

   private:
    AutoTiler autoTiler_;
    std::string selectedTileset_;  // which tileset is active
    int selectedTile_ = -1;        // index of the tile selected in that tileset
};
