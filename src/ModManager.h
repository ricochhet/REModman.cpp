#ifndef REMODMAN_MANAGER_INCLUDED
#define REMODMAN_MANAGER_INCLUDED

#pragma once

#include <JsonUtils.h>
#include <ModManagerConfig.h>
#include <ModManagerData.h>
#include <ModManagerDefinitions.h>
#include <ModManagerPatches.h>
#include <Utils.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace ModManager {
    void                     initialize(const std::string& path);
    std::vector<std::string> get_mod_directories(const std::string& path);
    std::vector<std::string> get_mod_entries(
        const std::string& path, const std::string& file, const bool compare
    );
    std::vector<ModManagerData::File> load_mod_files(
        const std::string& path, const std::string& modPath, const std::string& gamePath,
        const std::string& gameSelection
    );
    bool stage_mod(
        const std::string& path, const std::string& modPath, const std::string& gamePath,
        const std::string& gameSelection, const int stagingIndex
    );
    bool destage_mod(const std::string& path, const std::string& modPath);
    bool install_mod(
        const std::string& path, const std::string& modPath, const std::string& gamePath,
        const std::string& gameSelection
    );
    bool uninstall_mod(const std::string& path, const std::string& modPath);
    bool uninstall_pak_mod(
        const std::string& path, const std::string& modPath, const std::string& gamePath,
        const std::string& gameSelection
    );
    std::vector<ModManagerData::Mod> remove_mod_from_list(
        const std::vector<ModManagerData::Mod>& listToPatch, const std::string& modPath
    );
}

#endif  // REMODMAN_MANAGER_INCLUDED