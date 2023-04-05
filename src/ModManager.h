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
#include <ModManagerConfig.h>
#include <ModManagerPatches.h>
#include <ModManagerDefinitions.h>

namespace ModManager
{
    void initialize(const std::string &path);
    std::vector<std::string> get_mod_directories(const std::string &path);
    std::vector<std::string> get_mod_entries(const std::string &path, const std::string &file, const bool compare);
    bool stage_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const int stagingIndex);
    bool destage_mod(const std::string &path, const std::string &modPath);
    bool install_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &gameSelection);
    bool uninstall_mod(const std::string &path, const std::string &modPath);
    bool uninstall_pak_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &gameSelection);
    std::vector<ModManagerData::Mod> remove_mod_from_list(const std::vector<ModManagerData::Mod> &listToPatch, const std::string &modPath);
}

#endif // REMODMAN_MANAGER_INCLUDED