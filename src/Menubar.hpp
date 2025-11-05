#pragma once

#include "TilePallete.hpp"
#include "Tileset.hpp"
class Menubar {
   public:
    Menubar(TilePallete& tilePallete) : tilePallete_(tilePallete) {};
    ~Menubar() = default;

    void render();  // draw ImGui panel for selecting tiles

   private:
    TilePallete& tilePallete_;
};
