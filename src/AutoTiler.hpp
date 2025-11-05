#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Tileset.hpp"

// which tile in the tileset to
struct Coord {
    int x, y;
};

struct Mask {
    uint8_t mask;     // e.g. 0b00001111 for up/down/left/right
    uint8_t ignores;  // which bits must match (1 = check, 0 = ignore)
};

struct MaskRule {
    Mask mask;
    std::vector<Coord> tiles;
};

struct RuleSet {
    std::string id;
    std::string path;
    std::string copy;
    std::string ignores;

    std::vector<MaskRule> rules;
};

struct AutoTileRuleMap {
    std::unordered_map<std::string, RuleSet> ruleSets;
};

struct TileSetMap {
    std::unordered_map<std::string, Tileset> tilesets;
};

struct AutoTileRule {
    uint8_t mask;
    uint8_t ignores;
    int tileIndex;  // can be negative
};

const int DEFAULT_TILESET_HEIGHT = 15;
const int DEFAULT_TILESET_WIDTH = 6;

int getIndexFromCoord(int x, int y, int width = DEFAULT_TILESET_WIDTH, int height = DEFAULT_TILESET_HEIGHT);
Coord getCoordFromIndex(int index, int width = DEFAULT_TILESET_WIDTH, int height = DEFAULT_TILESET_HEIGHT);
Mask parseMaskString(const std::string& string);
std::unordered_map<std::string, RuleSet> loadTilesJSON(const std::string& path);

class AutoTileSet {
   public:
    void addRule(uint8_t mask, uint8_t ignores, uint8_t tileIndex);
    uint8_t getTileForMask(uint8_t mask, int x, int y) const;

   private:
    std::vector<AutoTileRule> rules_;
};

class AutoTiler {
   public:
    // Load all the tilesets
    AutoTiler(const std::string& path = FOREGROUND_PATH);
    ~AutoTiler() = default;

    const std::unordered_map<std::string, Tileset>& getTilesets() const { return tilesets_; }
    std::unordered_map<std::string, Tileset>& getTilesets() { return tilesets_; }

    const std::unordered_map<std::string, AutoTileSet>& getAutoTilesets() const { return autoTilesets_; }
    std::unordered_map<std::string, AutoTileSet>& getAutoTilesets() { return autoTilesets_; }

   private:
    static constexpr const char* FOREGROUND_PATH = "../assets/map/Foreground.json";

    std::unordered_map<std::string, Tileset> tilesets_;
    std::unordered_map<std::string, RuleSet> ruleMap_;  // only need for creating the auto tile sets
    std::unordered_map<std::string, AutoTileSet> autoTilesets_;

    bool load(const std::string& path);
    void setupAutoTileset(const RuleSet& ruleSet, const std::string& id);
};
