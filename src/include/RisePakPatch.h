#ifndef __RE_RISEPAKPATCH_H
#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <Murmur3.h>

struct FileEntry
{
    std::string filename;
    uint32_t filenameLower;
    uint32_t filenameUpper;
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
    void Seek(std::streampos pos)
    {
        outStream.seekp(pos);
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

#endif __RE_RISEPAKPATCH_H