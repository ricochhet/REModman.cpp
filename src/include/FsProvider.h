#ifndef __RE_FSPROVIDER_H
#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace FsProvider
{
    std::string read_file_to_string(const std::string& filePath);
}

#endif __RE_FSPROVIDER_H