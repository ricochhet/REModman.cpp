#include <RisePakPatch.h>

std::vector<char> read_all_bytes(const std::string& path) {
    std::ifstream     input_file(path, std::ios::binary);
    std::vector<char> bytes((std::istreambuf_iterator<char>(input_file)), (std::istreambuf_iterator<char>()));
    return bytes;
}

std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}

void RisePakPatch::ProcessDirectory(const std::string& path, const std::string& outputFile) {
    std::string directory = std::filesystem::absolute(path).string();

    if (!std::filesystem::is_directory(directory)) {
        return;
    }

    if (std::filesystem::exists(outputFile)) {
        Logger::getInstance().log("Deleting existing output file...", LogLevel::Info);
        std::filesystem::remove(outputFile);
    }

    std::vector<std::string> sortedFiles;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path(directory) / "natives")) {
        if (std::filesystem::is_regular_file(entry)) {
            sortedFiles.emplace_back(entry.path().string());
        }
    }
    std::sort(sortedFiles.begin(), sortedFiles.end(), [](const std::string& a, const std::string& b) {
        if (std::filesystem::path(a).parent_path() == std::filesystem::path(b).parent_path()) {
            return std::filesystem::path(a).filename() < std::filesystem::path(b).filename();
        }
        return std::filesystem::path(a).parent_path() < std::filesystem::path(b).parent_path();
    });

    Logger::getInstance().log("Processing " + std::to_string(sortedFiles.size()) + " files", LogLevel::Info);
    std::vector<FileEntry> list;
    Writer                 writer(outputFile);
    writer.WriteUInt32(1095454795u);
    writer.WriteUInt32(4u);
    writer.WriteUInt32((uint32_t)sortedFiles.size());
    writer.WriteUInt32(0u);
    writer.Seek(48 * sortedFiles.size() + writer.Position());

    for (const std::string& obj : sortedFiles) {
        FileEntry   fileEntry2;
        std::string text = Utils::string_replace_all(obj, directory, "");
        text             = Utils::string_replace_all(text, "\\", "/");

        if (text[0] == '/') {
            text.erase(0, 1);
        }

        uint32_t          hash   = murmurhash3(to_lower(text).c_str(), UINT_MAX);
        uint32_t          hash2  = murmurhash3(to_upper(text).c_str(), UINT_MAX);
        std::vector<char> array2 = read_all_bytes(obj);

        fileEntry2.fileName      = text;
        fileEntry2.offset        = (uint64_t)writer.Position();
        fileEntry2.uncompSize    = (uint64_t)array2.size();
        fileEntry2.fileNameLower = hash;
        fileEntry2.fileNameUpper = hash2;

        list.emplace_back(fileEntry2);
        writer.Write(array2.data(), array2.size());
    }

    writer.SeekFromBeginning(16);
    for (const FileEntry& item : list) {
        Logger::getInstance().log(item.fileName + " " + std::to_string(item.fileNameUpper) + " " + std::to_string(item.fileNameLower), LogLevel::Info);
        writer.WriteUInt32(item.fileNameLower);
        writer.WriteUInt32(item.fileNameUpper);
        writer.WriteUInt64(item.offset);
        writer.WriteUInt64(item.uncompSize);
        writer.WriteUInt64(item.uncompSize);
        writer.WriteUInt64(0uL);
        writer.WriteUInt32(0u);
        writer.WriteUInt32(0u);
    }

    writer.Close();
}