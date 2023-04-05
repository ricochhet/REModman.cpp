#ifndef REMODMAN_MANAGER_PATCHES_INCLUDED
#define REMODMAN_MANAGER_PATCHES_INCLUDED

#pragma once

#include <iostream>
#include <json.hpp>
#include <filesystem>
#include <JsonUtils.h>

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

#endif // REMODMAN_MANAGER_PATCHES_INCLUDED