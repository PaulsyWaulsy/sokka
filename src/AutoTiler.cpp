#include "AutoTiler.hpp"

#include <cstdint>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "Logger.hpp"

// Ignore the centre of the mask -> is the tile
// string is xxx-xxx-xxx
static const int lookupMask[8] = {0, 1, 2, 4, 6, 8, 9, 10};

Mask parseMaskString(const std::string& s) {
    // TODO: if mask == "center", "padding"

    if (s == "padding") {
        return {0, 0};  // special meaning handled elsewhere
    }
    if (s == "center") {
        return {0, 0};  // same — these are not bitmask-based
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

TileSetManager loadTilesJSON(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("TilesetLoader Failed to open JSON: ", path);
        return {};
    }

    nlohmann::json data;
    file >> data;

    // Step 1: Load all tilesets into a map by ID
    TileSetManager tilesetMap;

    for (auto& t : data["tilesets"]) {
        Set set;
        set.id = t.value("id", "");
        set.path = "tilesets/" + t.value("path", "");
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
                set.rules.push_back(rule);
            }
        }

        tilesetMap.sets[set.id] = set;
    }

    // Step 2: Apply "copy" inheritance
    for (auto& [id, set] : tilesetMap.sets) {
        if (!set.copy.empty()) {
            auto baseIt = tilesetMap.sets.find(set.copy);
            if (baseIt != tilesetMap.sets.end()) {
                const Set& base = baseIt->second;

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

    LOG_INFO("Loaded ", tilesetMap.sets.size(), " tilesets from ", path);

    for (const auto& [id, set] : tilesetMap.sets) {
        LOG_INFO("  Tileset ID: ", id);
        LOG_INFO("    Path: ", set.path);
        LOG_INFO("    Copy: ", set.copy.empty() ? "(none)" : set.copy);
        LOG_INFO("    Ignores: ", set.ignores.empty() ? "(none)" : set.ignores);
        LOG_INFO("    Rules: ", set.rules.size());

        for (size_t i = 0; i < set.rules.size(); ++i) {
            const auto& rule = set.rules[i];
            LOG_INFO("      [", i, "] Mask: ", static_cast<int>(rule.mask.mask),
                     ", Ignore: ", static_cast<int>(rule.mask.ignores), "| Tiles: ", rule.tiles.size());
        }
    }

    return tilesetMap;
}

uint8_t AutoTileSet::getTileForMask(uint8_t mask) const {
    for (const auto& rule : rules_) {
        if (mask == rule.mask) return rule.tileIndex;
    }
    return -1;  // default
}
