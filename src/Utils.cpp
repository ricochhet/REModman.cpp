#include <Utils.h>

std::string Utils::string_replace_all(const std::string &baseString, const std::string &stringToReplace, const std::string &replacement)
{
    std::string result = baseString;
    size_t stringPosition = 0;

    while ((stringPosition = result.find(stringToReplace, stringPosition)) != std::string::npos)
    {
        result.replace(stringPosition, stringToReplace.length(), replacement);
        stringPosition += replacement.length();
    }

    return result;
}