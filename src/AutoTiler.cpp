#include "AutoTiler.hpp"

#include <cstdint>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "Logger.hpp"
#include "Tileset.hpp"

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

AutoTileRuleMap loadTilesJSON(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("TilesetLoader Failed to open JSON: ", path);
        return {};
    }

    nlohmann::json data;
    file >> data;

    // Step 1: Load all tilesets into a map by ID
    AutoTileRuleMap tilesetMap;

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

        tilesetMap.ruleSets.emplace(set.id, std::move(set));
    }

    // Step 2: Apply "copy" inheritance
    for (auto& [id, set] : tilesetMap.ruleSets) {
        if (!set.copy.empty()) {
            auto baseIt = tilesetMap.ruleSets.find(set.copy);
            if (baseIt != tilesetMap.ruleSets.end()) {
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

    LOG_INFO("Loaded ", tilesetMap.ruleSets.size(), " tilesets from ", path);
    return tilesetMap;
}

uint8_t AutoTileSet::getTileForMask(uint8_t mask, uint8_t ignores) const {
    for (const auto& rule : rules_) {
        if ((mask ^ ignores) == rule.mask) {
            return rule.tileIndex;
        }
    }
    return -1;  // default
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

    for (auto& [id, ruleSet] : ruleMap_.ruleSets) {
        // Ignore loading the template
        if (std::string(id) == "z") {
            continue;
        }

        // real path
        // TODO: make this find it within the system (absolute path)
        std::string relativePath = "../assets/sprites/tilesets/" + ruleSet.path + ".png";

        // Construct in-place inside the map
        auto [it, inserted] = tilesets_.tilesets.emplace(id, Tileset{});
        Tileset& tileset = it->second;  // reference to the one inside the map

        if (tileset.load(relativePath)) {
            LOG_SUCCESS("AutoTiler Loaded tileset: ", id, " = ", ruleSet.path);
        } else {
            LOG_ERROR("AutoTiler Failed to load tileset for rule set: ", id, " (", ruleSet.path, ")");
            tilesets_.tilesets.erase(it);
            success = false;
        }
    }

    return success;
}
