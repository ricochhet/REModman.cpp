#ifndef REMODMAN_MANAGER_INCLUDED
#define REMODMAN_MANAGER_INCLUDED

#pragma once

#include <string>
#include <vector>
#include <Utils.h>
#include <iostream>
#include <filesystem>
#include <JsonUtils.h>
#include <ModManagerData.h>
#include <ModManagerPatches.h>

namespace ModManager
{
    void startup_health_checks(const std::string &path);
    std::vector<std::string> get_mod_entries(const std::string &path);
    std::vector<std::string> get_available_mod_entries(const std::string &path);
    std::vector<std::string> get_staged_mod_entries(const std::string &path);
    std::vector<std::string> get_installed_mod_entries(const std::string &path);
    bool contains_pak_files(const std::string &path);
    bool stage_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const int stagingIndex);
    bool destage_mod(const std::string &path, const std::string &modPath);
    bool install_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &gameSelection);
    bool uninstall_mod(const std::string &path, const std::string &modPath);
    bool uninstall_pak_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &gameSelection);
    std::vector<nlohmann::json> remove_mod_from_list(const std::vector<nlohmann::json> &listToPatch, const std::string &modPath);
    int get_last_selected_game(const std::string &path);
    std::string get_game_path(const std::string &path, const std::string &selection);
}

#endif // REMODMAN_MANAGER_INCLUDED