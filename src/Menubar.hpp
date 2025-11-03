#pragma once

#include "Tileset.hpp"
class Menubar {
   public:
    Menubar(Tileset& tileset) : tileset_(tileset) {};
    ~Menubar() = default;

    void render();  // draw ImGui panel for selecting tiles

   private:
    Tileset& tileset_;
};
