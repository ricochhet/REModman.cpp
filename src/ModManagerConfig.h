#ifndef REMODMAN_MANAGER_CONFIG_INCLUDED
#define REMODMAN_MANAGER_CONFIG_INCLUDED

#pragma once

#include <JsonUtils.h>
#include <ModManagerData.h>
#include <ModManagerDefinitions.h>

#include <iostream>

namespace ModManagerConfig {
    void        set_selected_profile(const std::string& path);
    void        set_last_selected_game(const std::string& path, const int selection);
    int         get_last_selected_game(const std::string& path);
    void        set_game_path(const std::string& path, const std::string& gameSelection, const std::string& pathSelection);
    std::string get_game_path(const std::string& path, const std::string& selection);
    std::vector<ModManagerData::Mod> get_staged_mods(const std::string& path);
    void        set_staged_mods(const std::string& path, const nlohmann::json& data);
    std::vector<ModManagerData::Mod> get_installed_mods(const std::string& path);
    void        set_installed_mods(const std::string& path, const nlohmann::json& data);
}

#endif  // REMODMAN_MANAGER_CONFIG_INCLUDED