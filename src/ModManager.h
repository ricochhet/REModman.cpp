#ifndef REMODMAN_MANAGER_INCLUDED
#define REMODMAN_MANAGER_INCLUDED

#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <JsonUtils.h>
#include <Utils.h>

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

    nlohmann::json mod_to_json(const Mod &mod)
    {
        nlohmann::json j;
        j["SourcePath"] = mod.SourcePath;
        j["Files"] = nlohmann::json::array();

        for (const auto &file : mod.Files)
        {
            nlohmann::json fileJson;
            fileJson["SourcePath"] = file.SourcePath;
            fileJson["InstallPath"] = file.InstallPath;
            j["Files"].push_back(fileJson);
        }

        return j;
    }

    Mod mod_from_json(const nlohmann::json &j)
    {
        Mod mod;
        mod.SourcePath = j["SourcePath"];
        mod.Files.clear();

        for (const auto &fileJson : j["Files"])
        {
            File file;
            file.SourcePath = fileJson["SourcePath"];
            file.InstallPath = fileJson["InstallPath"];

            mod.Files.push_back(file);
        }

        return mod;
    }

    std::vector<Mod> mods_from_json(const nlohmann::json &j)
    {
        std::vector<Mod> mods;

        for (const auto &mod : j)
        {
            mods.push_back(mod_from_json(mod));
        }

        return mods;
    }
}

namespace ModManagerPatches
{
    namespace MonsterHunterRise
    {
        struct PatchReEnginePak
        {
            bool isPak;
            int pakIndex;
        };

        PatchReEnginePak patch_re_engine_pak(const std::string &path, const std::string &modPath);
    }
}

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