#ifndef __MODINFO_H
#pragma once

#include <json.hpp>

struct ModFile
{
    std::string InstallPath;
    std::string SourcePath;
    uint32_t Hash;
};

struct ModInfo
{
    std::string Name;
    uint32_t Hash;
    int LoadOrder = 0;
    std::string BasePath;
    bool IsEnabled = false;
    std::vector<ModFile> Files;
};

ModFile make_mod_file(std::string install_path, std::string source_path, uint32_t hash);
ModInfo make_mod_info(std::string name, uint32_t hash, int load_order, std::string base_path, bool is_enabled, std::vector<ModFile> files);

#endif __MODINFO_H