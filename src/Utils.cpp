#include <Utils.h>

std::string Utils::string_replace_all(
    const std::string& baseString, const std::string& stringToReplace,
    const std::string& replacement
) {
    std::string result         = baseString;
    size_t      stringPosition = 0;

    while ((stringPosition = result.find(stringToReplace, stringPosition)) != std::string::npos) {
        result.replace(stringPosition, stringToReplace.length(), replacement);
        stringPosition += replacement.length();
    }

    return result;
}

std::string Utils::truncate_string(const std::string& baseString, const int truncateAtLength) {
    if (baseString.length() <= truncateAtLength) {
        return baseString;
    }

    return baseString.substr(0, truncateAtLength - 3) + "...";
}

bool Utils::create_directory(const std::filesystem::path& path) {
    if (std::filesystem::exists(path)) {
        return true;
    }

    return std::filesystem::create_directory(path) && std::filesystem::is_directory(path);
}