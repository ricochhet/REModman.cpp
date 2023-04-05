#ifndef REMODMAN_MANAGER_PATCHES_INCLUDED
#define REMODMAN_MANAGER_PATCHES_INCLUDED

#pragma once

#include <JsonUtils.h>

#include <filesystem>
#include <iostream>
#include <json.hpp>

namespace ModManagerPatches {
    namespace MonsterHunterRise {
        struct PatchReEnginePak {
            bool isPak;
            int  pakIndex;
        };

        PatchReEnginePak patch_re_engine_pak(const std::string& path, const std::string& modPath);
        bool             contains_pak_files(const std::string& path);
        void             set_patch_in_profile(const std::string& path, const int& index);
    }
}

#endif  // REMODMAN_MANAGER_PATCHES_INCLUDED