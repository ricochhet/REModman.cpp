#ifndef REMODMAN_UTILS_INCLUDED
#define REMODMAN_UTILS_INCLUDED

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Utils {
    std::string       replaceAllInString(const std::string& baseString, const std::string& stringToReplace, const std::string& replacement);
    std::string       truncateString(const std::string& baseString, const int truncateAtLength);
    bool              createDirectory(const std::filesystem::path& path);
    std::vector<char> readAllBytes(const std::string& path);
    std::string       toLower(const std::string& str);
    std::string       toUpper(const std::string& str);
}

#endif  // REMODMAN_UTILS_INCLUDED