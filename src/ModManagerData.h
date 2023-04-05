#ifndef REMODMAN_MANAGER_DATA_INCLUDED
#define REMODMAN_MANAGER_DATA_INCLUDED

#pragma once

#include <iostream>
#include <json.hpp>
#include <filesystem>
#include <JsonUtils.h>

namespace ModManagerData
{
    struct File
    {
        std::string SourcePath;
        std::string InstallPath;
    };

    struct Mod
    {
        std::string SourcePath;
        std::vector<File> Files;
    };

    nlohmann::json mod_to_json(const Mod &mod);
    Mod mod_from_json(const nlohmann::json &j);
    nlohmann::json mods_to_json(const std::vector<ModManagerData::Mod> &mods);
    std::vector<Mod> mods_from_json(const nlohmann::json &j);
}

#endif // REMODMAN_MANAGER_DATA_INCLUDED