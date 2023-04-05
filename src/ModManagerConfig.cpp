#include <ModManagerConfig.h>

void ModManagerConfig::set_selected_profile(const std::string& path) {
    JsonUtils::create_or_update_json(path + PROFILE_FILE_NAME, {"SelectedProfile"}, path, true);
}

void ModManagerConfig::set_last_selected_game(const std::string& path, const int selection) {
    JsonUtils::create_or_update_json(
        path + PROFILE_FILE_NAME, {"LastSelectedGame"}, selection, true
    );
}

int ModManagerConfig::get_last_selected_game(const std::string& path) {
    int lastSelectedGameIndex =
        JsonUtils::get_integer_value(path + PROFILE_FILE_NAME, {"LastSelectedGame"});
    Logger::getInstance().log("Found profile path: " + path, LogLevel::Info);
    return lastSelectedGameIndex;
}

void ModManagerConfig::set_game_path(
    const std::string& path, const std::string& gameSelection, const std::string& pathSelection
) {
    JsonUtils::create_or_update_json(
        path + PROFILE_FILE_NAME, {"Games", gameSelection + "GamePath"}, pathSelection, true
    );
}

std::string ModManagerConfig::get_game_path(const std::string& path, const std::string& selection) {
    std::string gamePath =
        JsonUtils::get_string_value(path + PROFILE_FILE_NAME, {"Games", selection + "GamePath"});

    if (!gamePath.empty()) {
        Logger::getInstance().log("Found game path: " + path, LogLevel::Info);
        return gamePath;
    }

    return std::string();
}