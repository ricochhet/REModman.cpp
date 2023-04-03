#ifndef REMODMAN_MANAGER_INCLUDED
#define REMODMAN_MANAGER_INCLUDED

#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <JsonUtils.h>
#include <Utils.h>

namespace ModManager
{
    std::vector<nlohmann::json> get_mod_entries(const std::string &path);
    std::vector<nlohmann::json> get_uninstalled_mod_entries(const std::string &path);
    std::vector<nlohmann::json> get_installed_mod_entries(const std::string &path);
    bool contains_pak_files(const std::string &path);
    bool install_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &modInstallPath);
    bool uninstall_mod(const std::string &path, const std::string &modPath, const std::string &modInstallPath);
    bool uninstall_pak_mod(const std::string &path, const std::string &modPath, const std::string &modInstallPath);
    std::vector<nlohmann::json> remove_mod_from_list(const std::vector<nlohmann::json> &listToPatch, const std::string &modPath, const std::string &modInstallPath);
    int get_game_selection(const std::string &path);
}

#endif REMODMAN_MANAGER_INCLUDED