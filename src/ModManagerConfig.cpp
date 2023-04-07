#include <ModManagerConfig.h>

void ModManagerConfig::set_selected_profile(const std::string& path) { JsonUtils::create_or_update_json(path + REMM_PROFILE_FILE_NAME, {REMM_CONF_SELECTED_PROFILE}, path, true); }

void ModManagerConfig::set_last_selected_game(const std::string& path, const int selection) {
    JsonUtils::create_or_update_json(path + REMM_PROFILE_FILE_NAME, {REMM_CONF_LAST_SELECTED_GAME}, selection, true);
}

int ModManagerConfig::get_last_selected_game(const std::string& path) {
    return JsonUtils::get_integer_value(path + REMM_PROFILE_FILE_NAME, {REMM_CONF_LAST_SELECTED_GAME});
}

void ModManagerConfig::set_game_path(const std::string& path, const std::string& gameSelection, const std::string& pathSelection) {
    JsonUtils::create_or_update_json(path + REMM_PROFILE_FILE_NAME, {REMM_CONF_GAMES, gameSelection + REMM_CONF_GAME_PATH}, pathSelection, true);
}

std::string ModManagerConfig::get_game_path(const std::string& path, const std::string& selection) {
    return JsonUtils::get_string_value(path + REMM_PROFILE_FILE_NAME, {REMM_CONF_GAMES, selection + REMM_CONF_GAME_PATH});
}

std::vector<ModManagerData::Mod> ModManagerConfig::get_staged_mods(const std::string& path)
{
    return ModManagerData::mods_from_json(JsonUtils::get_json_value(path + REMM_PROFILE_FILE_NAME, {"StagedMods"}));
}

void ModManagerConfig::set_staged_mods(const std::string& path, const nlohmann::json& data)
{
    JsonUtils::create_or_update_json(path + REMM_PROFILE_FILE_NAME, {"StagedMods"}, data, true);
}

std::vector<ModManagerData::Mod> ModManagerConfig::get_installed_mods(const std::string& path)
{
    return ModManagerData::mods_from_json(JsonUtils::get_json_value(path + REMM_PROFILE_FILE_NAME, {"InstalledMods"}));
}

void ModManagerConfig::set_installed_mods(const std::string& path, const nlohmann::json& data)
{
    JsonUtils::create_or_update_json(path + REMM_PROFILE_FILE_NAME, {"InstalledMods"}, data, true);
}