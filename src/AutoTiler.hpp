#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct Coord {
    int x, y;
};

struct Mask {
    uint8_t mask;  // e.g. 0b00001111 for up/down/left/right
    uint8_t ignores;
};

struct MaskRule {
    Mask mask;
    std::vector<Coord> tiles;
};

struct Set {
    std::string id;
    std::string path;
    std::string copy;
    std::string ignores;

    std::vector<MaskRule> rules;
};

struct TileSetManager {
    std::unordered_map<std::string, Set> sets;
};

Mask parseMaskString(const std::string& string);
TileSetManager loadTilesJSON(const std::string& path);

struct AutoTileRule {
    uint8_t mask;
    uint8_t tileIndex;
};

class AutoTileSet {
   public:
    void addRule(uint8_t mask, uint8_t tileIndex) { rules_.push_back({mask, tileIndex}); }
    const std::vector<AutoTileRule>& getRules() const { return rules_; }
    uint8_t getTileForMask(uint8_t mask) const;
    AutoTileSet loadTileSet(const std::string& path);

   private:
    std::vector<AutoTileRule> rules_;
};
