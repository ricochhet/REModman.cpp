#ifndef REMODMAN_UTILS_INCLUDED
#define REMODMAN_UTILS_INCLUDED

#pragma once
#include <iostream>
#include <filesystem>

namespace Utils
{
    std::string string_replace_all(const std::string &baseString, const std::string &stringToReplace, const std::string &replacement);
    bool create_directory(const std::filesystem::path &path);
}

#endif REMODMAN_UTILS_INCLUDED