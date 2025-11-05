#include "AutoTiler.hpp"

#include <bitset>
#include <cstdint>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <unordered_map>

#include "Logger.hpp"
#include "Tileset.hpp"

// Ignore the centre of the mask -> is the tile
// string is xxx-xxx-xxx
static const int lookupMask[8] = {0, 1, 2, 4, 6, 8, 9, 10};

int getIndexFromCoord(int x, int y, int width, int height) {
    // Validate inputs (optional, helps catch out-of-bounds)
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return -1;  // Invalid index
    }

    // Convert (x, y) → linear index
    return y * width + x;
}

Coord getCoordFromIndex(int index, int width, int height) {
    if (index < 0 || index >= width * height) {
        return {-1, -1};  // Invalid
    }
    return {index % width, index / width};
}

Mask parseMaskString(const std::string& s) {
    if (s == "padding") {
        return {0xFF, 0xFF};  // special meaning handled elsewhere
    }
    if (s == "center") {
        return {0xFF, 0xFF};  // same — these are not bitmask-based
    }

    uint8_t tileMask = 0, ignoreMask = 0;
    int bit = 7;
    for (int idx : lookupMask) {
        if (idx >= (int)s.size()) continue;
        char c = s[idx];
        tileMask |= (c == '1') << bit;
        ignoreMask |= (c != 'x') << bit;
        bit--;
    }
    return {tileMask, ignoreMask};
}

std::unordered_map<std::string, RuleSet> loadTilesJSON(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("TilesetLoader Failed to open JSON: ", path);
        return {};
    }

    nlohmann::json data;
    file >> data;

    // Step 1: Load all tilesets into a map by ID
    std::unordered_map<std::string, RuleSet> tilesetMap;

    for (auto& t : data["tilesets"]) {
        RuleSet set;
        set.id = t.value("id", "");
        set.path = t.value("path", "");
        set.copy = t.value("copy", "");  // new field for inheritance
        set.ignores = t.value("ignores", "");

        if (t.contains("rules")) {
            for (auto& ruleJson : t["rules"]) {
                MaskRule rule;
                rule.mask = parseMaskString(ruleJson.value("mask", ""));
                if (ruleJson.contains("tiles")) {
                    for (auto& arr : ruleJson["tiles"]) {
                        if (arr.is_array() && arr.size() == 2)
                            rule.tiles.push_back({arr[0].get<int>(), arr[1].get<int>()});
                    }
                }
                set.rules.emplace_back(std::move(rule));
            }
        }

        tilesetMap.emplace(set.id, std::move(set));
    }

    // Step 2: Apply "copy" inheritance
    for (auto& [id, set] : tilesetMap) {
        if (!set.copy.empty()) {
            auto baseIt = tilesetMap.find(set.copy);
            if (baseIt != tilesetMap.end()) {
                const RuleSet& base = baseIt->second;

                // Copy base properties if missing
                if (set.ignores.empty()) set.ignores = base.ignores;
                if (set.path.empty()) set.path = base.path;
                if (set.rules.empty()) {
                    set.rules = base.rules;
                }

                LOG_INFO("Tileset ", id, " copied from ", set.copy);
            } else {
                LOG_WARN("Tileset ", id, " tried to copy missing base: ", set.copy);
            }
        }
    }

    LOG_INFO("Loaded ", tilesetMap.size(), " tilesets from ", path);
    return tilesetMap;
}

uint8_t AutoTileSet::getTileForMask(uint8_t mask, int x, int y) const {
    std::vector<uint8_t> matches;
    std::bitset<8> maskBits(mask);

    LOG_INFO("[AutoTiler] Checking mask=", (int)mask, " (", maskBits, ")");

    for (size_t i = 0; i < rules_.size(); ++i) {
        const auto& rule = rules_[i];
        uint8_t relevant = rule.ignores;
        bool matched = ((mask & relevant) == (rule.mask & relevant));

        std::bitset<8> ruleBits(rule.mask);
        std::bitset<8> ignoreBits(rule.ignores);

        if (matched) {
            matches.push_back(rule.tileIndex);
            LOG_INFO("  ├─ MATCH [Rule ", i, "] mask=", (int)rule.mask, "(", ruleBits, ")",
                     " ignores=", (int)rule.ignores, "(", ignoreBits, ")", " → tile=", (int)rule.tileIndex);
        }
    }

    if (!matches.empty()) {
        // Deterministic seed from tile position + mask
        uint32_t seed = static_cast<uint32_t>((x * 73856093) ^ (y * 19349663) ^ (mask * 83492791));
        std::mt19937 rng(seed);
        std::uniform_int_distribution<size_t> dist(0, matches.size() - 1);
        return matches[dist(rng)];
    }

    return 0;
}

void AutoTileSet::addRule(uint8_t mask, uint8_t ignores, uint8_t tileIndex) {
    rules_.push_back({mask, ignores, tileIndex});
}

AutoTiler::AutoTiler(const std::string& path) {
    if (!load(path)) {
        LOG_ERROR("AutoTiler failed to load tilesets");
    } else {
        LOG_SUCCESS("AutoTiler Loaded tilesets");
    }
}

bool AutoTiler::load(const std::string& path) {
    ruleMap_ = loadTilesJSON(path);
    bool success = true;

    for (auto& [id, ruleSet] : ruleMap_) {
        // Ignore loading the template
        if (std::string(id) == "z") {
            continue;
        }

        // real path
        // TODO: make this find it within the system (absolute path)
        std::string relativePath = "../assets/sprites/tilesets/" + ruleSet.path + ".png";

        // Construct in-place inside the map
        auto [it, inserted] = tilesets_.emplace(id, Tileset{});
        Tileset& tileset = it->second;  // reference to the one inside the map

        if (tileset.load(relativePath)) {
            LOG_SUCCESS("AutoTiler Loaded tileset: ", id, " = ", ruleSet.path);
        } else {
            LOG_ERROR("AutoTiler Failed to load tileset for rule set: ", id, " (", ruleSet.path, ")");
            tilesets_.erase(it);
            success = false;
            continue;
        }

        // load the auto tile rules
        setupAutoTileset(ruleSet, id);
    }

    return success;
}

void AutoTiler::setupAutoTileset(const RuleSet& ruleSet, const std::string& id) {
    for (auto& rule : ruleSet.rules) {
        Mask mask = rule.mask;
        for (auto& tile : rule.tiles) {
            uint8_t index = getIndexFromCoord(tile.x, tile.y);
            autoTilesets_[id].addRule(mask.mask, mask.ignores, index);
        }
    }
    LOG_SUCCESS("Added AutoTileset for ", id);
}
