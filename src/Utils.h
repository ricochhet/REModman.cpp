#ifndef REMODMAN_UTILS_INCLUDED
#define REMODMAN_UTILS_INCLUDED

#pragma once
#include <iostream>

namespace Utils
{
    std::string string_replace_all(const std::string &baseString, const std::string &stringToReplace, const std::string &replacement);
}

#endif REMODMAN_UTILS_INCLUDED