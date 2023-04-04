#ifndef REMODMAN_RISEPAKPATCH_INCLUDED
#define REMODMAN_RISEPAKPATCH_INCLUDED

#pragma once

#include <MurmurHash.h>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <Utils.h>
#include <Logger.h>

struct FileEntry
{
    std::string fileName;
    uint32_t fileNameLower;
    uint32_t fileNameUpper;
    uint64_t offset;
    uint64_t uncompSize;
};

class Writer
{
public:
    Writer(std::string fileName)
    {
        outStream.open(fileName, std::ios::binary);
    }
    ~Writer()
    {
        outStream.close();
    }
    void WriteUInt32(uint32_t value)
    {
        outStream.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
    void WriteUInt64(uint64_t value)
    {
        outStream.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
    void Write(const char *data, std::streamsize count)
    {
        outStream.write(data, count);
    }
    void Seek(std::streampos position)
    {
        outStream.seekp(position);
    }
    void SeekFromBeginning(std::size_t position)
    {
        outStream.seekp(position, std::ios_base::beg);
    }
    int Position()
    {
        return outStream.tellp();
    }
    void Close()
    {
        outStream.close();
    }

private:
    std::ofstream outStream;
};

namespace RisePakPatch
{
    void ProcessDirectory(const std::string &path, const std::string &outputFile);
}

#endif // REMODMAN_RISEPAKPATCH_INCLUDED