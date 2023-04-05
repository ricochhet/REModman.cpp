#include <ModManagerPatches.h>

ModManagerPatches::MonsterHunterRise::PatchReEnginePak
ModManagerPatches::MonsterHunterRise::patch_re_engine_pak(
    const std::string& path, const std::string& modPath
) {
    nlohmann::json j           = JsonUtils::load_json(path + PROFILE_FILE_NAME);
    int            pakModIndex = j["Patches"][GAME_MONSTER_HUNTER_RISE]["PatchReEnginePakIndex"];
    bool           isPakMod    = false;

    for (const auto& fileEntry : std::filesystem::directory_iterator(modPath)) {
        if (fileEntry.is_regular_file()) {
            std::string fileName = fileEntry.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak") {
                isPakMod = true;
                pakModIndex++;
                JsonUtils::create_or_update_json(
                    path + PROFILE_FILE_NAME,
                    {"Patches", GAME_MONSTER_HUNTER_RISE, "PatchReEnginePakIndex"}, pakModIndex, true
                );
                break;
            }
        }
    }

    ModManagerPatches::MonsterHunterRise::PatchReEnginePak pakInfo;
    pakInfo.isPak    = isPakMod;
    pakInfo.pakIndex = pakModIndex;
    return pakInfo;
}

bool ModManagerPatches::MonsterHunterRise::contains_pak_files(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    bool isPakMod = false;

    for (const auto& fileEntry : std::filesystem::directory_iterator(path)) {
        if (fileEntry.is_regular_file()) {
            std::string fileName = fileEntry.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak") {
                isPakMod = true;
                break;
            }
        }
    }

    return isPakMod;
}

void ModManagerPatches::MonsterHunterRise::set_patch_in_profile(
    const std::string& path, const int& index
) {
    JsonUtils::create_or_update_json(
        path + PROFILE_FILE_NAME, {"Patches", GAME_MONSTER_HUNTER_RISE, "PatchReEnginePakIndex"},
        index, true
    );
}