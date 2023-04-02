#include <FsProvider.h>

std::string FsProvider::read_file_to_string(const std::string &filePath)
{
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return "";
    }

    std::string fileContents;
    inputFile.seekg(0, std::ios::end);
    fileContents.reserve(inputFile.tellg());
    inputFile.seekg(0, std::ios::beg);
    fileContents.assign((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());
    inputFile.close();

    return fileContents;
}