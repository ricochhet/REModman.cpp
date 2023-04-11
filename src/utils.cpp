#include <utils.h>

std::vector<char> Utils::readAllBytes(const std::string& path) {
    std::ifstream     inputFile(path, std::ios::binary);
    std::vector<char> bytes((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
    return bytes;
}

std::string Utils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string Utils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string Utils::replaceAllInString(const std::string& baseString, const std::string& stringToReplace, const std::string& replacement) {
    std::string result         = baseString;
    size_t      stringPosition = 0;

    while ((stringPosition = result.find(stringToReplace, stringPosition)) != std::string::npos) {
        result.replace(stringPosition, stringToReplace.length(), replacement);
        stringPosition += replacement.length();
    }

    return result;
}

std::string Utils::truncateString(const std::string& baseString, const int truncateAtLength) {
    if (baseString.length() <= truncateAtLength) {
        return baseString;
    }

    return baseString.substr(0, truncateAtLength - 3) + "...";
}

bool Utils::createDirectory(const std::filesystem::path& path) {
    if (std::filesystem::exists(path)) {
        return true;
    }

    return std::filesystem::create_directory(path) && std::filesystem::is_directory(path);
}