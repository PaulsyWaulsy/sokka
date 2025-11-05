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
    uint8_t tileIndex;
};

Mask parseMaskString(const std::string& string);
AutoTileRuleMap loadTilesJSON(const std::string& path);

class AutoTileSet {
   public:
    void addRule(uint8_t mask, uint8_t tileIndex) { rules_.push_back({mask, tileIndex}); }
    const std::vector<AutoTileRule>& getRules() const { return rules_; }
    uint8_t getTileForMask(uint8_t mask, uint8_t ignores) const;
    AutoTileSet loadTileSet(const std::string& path);

   private:
    std::vector<AutoTileRule> rules_;
};

class AutoTiler {
   public:
    // Load all the tilesets
    AutoTiler(const std::string& path = FOREGROUND_PATH);
    ~AutoTiler() = default;

    const std::unordered_map<std::string, Tileset>& getTilesets() const { return tilesets_.tilesets; }
    std::unordered_map<std::string, Tileset>& getTilesets() { return tilesets_.tilesets; }

   private:
    static constexpr const char* FOREGROUND_PATH = "../assets/map/Foreground.json";

    bool load(const std::string& path);
    TileSetMap tilesets_;
    AutoTileRuleMap ruleMap_;
};
