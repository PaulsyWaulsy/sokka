#pragma once
#include <vector>

struct Tile {
    int id = 0;
};

class Tilemap {
   public:
    Tilemap(int width, int height);
    Tile& at(int x, int y);
    void clear();
    void fill(int tileId);

    int width() const { return width_; }
    int height() const { return height_; }

   private:
    int width_, height_;
    std::vector<Tile> tiles_;
};
