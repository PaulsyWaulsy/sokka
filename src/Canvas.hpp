#include "AutoTiler.hpp"
#include "Texture.hpp"
#include "Tileset.hpp"
#include "imgui.h"

class Canvas {
   public:
    Canvas(Tileset& tileset, int mapWidth = 32, int mapHeight = 32);
    void render();

   private:
    void handleInput(const ImVec2& origin);
    void drawGrid(ImDrawList* draw_list, const ImVec2& origin, const ImVec2& size);
    void drawHover(const ImVec2& origin);
    void drawTiles(ImDrawList* draw_list, const ImVec2& origin);

    int computeMask(int x, int y) const;
    void updateAutoTiles(int x, int y);

    ImVec2 offset_ = ImVec2(0.0f, 0.0f);
    float zoom_ = 3.0f;
    const float tileSize_ = 8.0f;

    Tileset& tileset_;
    int mapWidth_;
    int mapHeight_;
    std::vector<int> tiles_;  // -1 = empty, otherwise tile index

    AutoTileSet autotileSet_;

    // TEST:
    TileSetManager tilesets_;
};
