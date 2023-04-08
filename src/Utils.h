#ifndef REMODMAN_UTILS_INCLUDED
#define REMODMAN_UTILS_INCLUDED

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Utils {
    class Writer {
    public:
        Writer(std::string fileName) { outStream.open(fileName, std::ios::binary); }
        ~Writer() { outStream.close(); }
        void writeUInt32(uint32_t value) { outStream.write(reinterpret_cast<const char*>(&value), sizeof(value)); }
        void writeUInt64(uint64_t value) { outStream.write(reinterpret_cast<const char*>(&value), sizeof(value)); }
        void write(const char* data, std::streamsize count) { outStream.write(data, count); }
        void seek(std::streampos position) { outStream.seekp(position); }
        void seekFromBeginning(std::size_t position) { outStream.seekp(position, std::ios_base::beg); }
        int  position() { return outStream.tellp(); }
        void close() { outStream.close(); }

    private:
        std::ofstream outStream;
    };
    std::string       replaceAllInString(const std::string& baseString, const std::string& stringToReplace, const std::string& replacement);
    std::string       truncateString(const std::string& baseString, const int truncateAtLength);
    bool              createDirectory(const std::filesystem::path& path);
    std::vector<char> Utils::readAllBytes(const std::string& path);
    std::string       Utils::toLower(const std::string& str);
    std::string       Utils::toUpper(const std::string& str);
}

#endif  // REMODMAN_UTILS_INCLUDED