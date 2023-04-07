#ifndef REMODMAN_UTILS_INCLUDED
#define REMODMAN_UTILS_INCLUDED

#pragma once
#include <filesystem>
#include <iostream>

namespace Utils {
    std::string string_replace_all(const std::string& baseString, const std::string& stringToReplace, const std::string& replacement);
    std::string truncate_string(const std::string& baseString, const int truncateAtLength);
    bool        create_directory(const std::filesystem::path& path);
}

#endif  // REMODMAN_UTILS_INCLUDED