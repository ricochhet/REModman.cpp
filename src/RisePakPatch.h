#ifndef REMODMAN_RISEPAKPATCH_INCLUDED
#define REMODMAN_RISEPAKPATCH_INCLUDED

#pragma once

#include <Logger.h>
#include <MurmurHash.h>
#include <Utils.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace RisePakPatch {
    struct FileEntry {
        std::string fileName;
        uint32_t    fileNameLower;
        uint32_t    fileNameUpper;
        uint64_t    offset;
        uint64_t    uncompSize;
    };

    void processDirectory(const std::string& path, const std::string& outputFile);
}

#endif  // REMODMAN_RISEPAKPATCH_INCLUDED