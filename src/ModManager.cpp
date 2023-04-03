#include <ModManager.h>

std::vector<nlohmann::json> ModManager::get_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        throw;
    }

    std::vector<nlohmann::json> modEntries;

    for (const auto &fileEntry : std::filesystem::directory_iterator(path + "/Mods/"))
    {
        if (fileEntry.is_directory())
        {
            nlohmann::json modEntry;
            modEntry["SourcePath"] = fileEntry.path().string();
            modEntries.push_back(modEntry);
        }
    }

    return modEntries;
}

std::vector<nlohmann::json> ModManager::get_uninstalled_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        throw;
    }

    nlohmann::json modInstallations = JsonUtils::load_json(path + "/mods.json");
    std::vector<nlohmann::json> modEntries = get_mod_entries(path);
    std::vector<nlohmann::json> currentModEntries;

    for (auto &entry : modEntries)
    {
        bool found = false;
        for (auto &installedMod : modInstallations)
        {
            if (installedMod["SourcePath"] == entry["SourcePath"])
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            currentModEntries.push_back(entry);
        }
    }

    return currentModEntries;
}

std::vector<nlohmann::json> ModManager::get_staged_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        throw;
    }

    nlohmann::json modInstallations = JsonUtils::load_json(path + "/mods_staging.json");
    std::vector<nlohmann::json> modEntries = get_mod_entries(path);
    std::vector<nlohmann::json> currentModEntries;

    for (auto &entry : modEntries)
    {
        bool found = false;
        for (auto &installedMod : modInstallations)
        {
            if (installedMod["SourcePath"] == entry["SourcePath"])
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            currentModEntries.push_back(entry);
        }
    }

    return currentModEntries;
}

std::vector<nlohmann::json> ModManager::get_installed_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        throw;
    }

    nlohmann::json modInstallations = JsonUtils::load_json(path + "/mods.json");
    std::vector<nlohmann::json> modInstallationEntries;

    for (auto &installedMod : modInstallations)
    {
        modInstallationEntries.push_back(installedMod);
    }

    return modInstallationEntries;
}

bool ModManager::contains_pak_files(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        throw;
    }

    bool isPakMod = false;

    for (const auto &fileEntry : std::filesystem::directory_iterator(path))
    {
        if (fileEntry.is_regular_file())
        {
            std::string fileName = fileEntry.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak")
            {
                isPakMod = true;
                break;
            }
        }
    }

    return isPakMod;
}

bool ModManager::stage_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &modInstallPath, const int stagingIndex)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath) || !std::filesystem::exists(gamePath))
    {
        return false;
    }

    nlohmann::json j = JsonUtils::load_json(path + "/profile.json");
    std::string pakModPrefix = "re_chunk_000.pak.patch_";
    std::string pakModSuffix = ".pak";
    nlohmann::json modFiles;
    int pakModIndex = j["PatchReEnginePakIndex"];
    bool isPakMod = false;

    for (const auto &fileEntry : std::filesystem::directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string fileName = fileEntry.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak")
            {
                isPakMod = true;
                pakModIndex++;
                JsonUtils::create_or_update_json(path + "/profile.json", "PatchReEnginePakIndex", pakModIndex, true);
                break;
            }
        }
    }

    for (const auto &fileEntry : std::filesystem::recursive_directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string relative_path = std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string filename = fileEntry.path().filename().string();

            if (isPakMod)
            {
                std::string pakFileName = pakModPrefix + std::to_string(pakModIndex).insert(0, 3 - std::to_string(pakModIndex).length(), '0') + pakModSuffix;
                relative_path = Utils::string_replace_all(relative_path, filename, pakFileName);
            }

            std::string game_file = gamePath + "/" + relative_path;
            modFiles[fileEntry.path().string()] = game_file;
        }
    }

    nlohmann::json installedMod;
    installedMod["SourcePath"] = modPath;
    installedMod["InstallPath"] = modInstallPath;
    installedMod["Files"] = modFiles;

    nlohmann::json modInstallations = JsonUtils::load_json(path + "/" + "mods_staging.json");
    size_t stagingIterator = stagingIndex;
    if (stagingIterator >= modInstallations.size())
    {
        stagingIterator = modInstallations.size();
    }

    nlohmann::json::iterator stageIter = modInstallations.begin() + stagingIterator;
    modInstallations.insert(stageIter, installedMod);
    JsonUtils::write_json_to_file(path + "/" + "mods_staging.json", modInstallations);

    return true;
}

bool ModManager::install_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &modInstallPath)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath) || !std::filesystem::exists(gamePath))
    {
        return false;
    }

    nlohmann::json j = JsonUtils::load_json(path + "/profile.json");
    std::string pakModPrefix = "re_chunk_000.pak.patch_";
    std::string pakModSuffix = ".pak";
    nlohmann::json modFiles;
    int pakModIndex = j["PatchReEnginePakIndex"];
    bool isPakMod = false;

    for (const auto &fileEntry : std::filesystem::directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string fileName = fileEntry.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak")
            {
                isPakMod = true;
                pakModIndex++;
                JsonUtils::create_or_update_json(path + "/profile.json", "PatchReEnginePakIndex", pakModIndex, true);
                break;
            }
        }
    }

    for (const auto &fileEntry : std::filesystem::recursive_directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string relative_path = std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string filename = fileEntry.path().filename().string();

            if (isPakMod)
            {
                std::string pakFileName = pakModPrefix + std::to_string(pakModIndex).insert(0, 3 - std::to_string(pakModIndex).length(), '0') + pakModSuffix;
                relative_path = Utils::string_replace_all(relative_path, filename, pakFileName);
            }

            std::string game_file = gamePath + "/" + relative_path;
            modFiles[fileEntry.path().string()] = game_file;
        }
    }

    for (const auto &[modFile, gameFile] : modFiles.items())
    {
        std::filesystem::create_directories(std::filesystem::path(gameFile.get<std::string>()).parent_path());
        std::filesystem::copy_file(modFile, gameFile.get<std::string>(), std::filesystem::copy_options::overwrite_existing);
    }

    nlohmann::json installedMod;
    installedMod["SourcePath"] = modPath;
    installedMod["InstallPath"] = modInstallPath;
    installedMod["Files"] = modFiles;

    nlohmann::json modInstallations = JsonUtils::load_json(path + "/" + "mods.json");
    modInstallations.push_back(installedMod);
    JsonUtils::write_json_to_file(path + "/" + "mods.json", modInstallations);

    return true;
}

bool ModManager::uninstall_mod(const std::string &path, const std::string &modPath, const std::string &modInstallPath)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath))
    {
        return false;
    }

    nlohmann::json modInstallations;
    std::ifstream fileIn(path + "/" + "mods.json");
    if (fileIn.is_open())
    {
        fileIn >> modInstallations;
        fileIn.close();
    }
    else
    {
        throw std::runtime_error("Failed to open mods.json.");
    }

    for (const auto &installedMod : modInstallations)
    {
        if (installedMod["SourcePath"] == modPath && installedMod["InstallPath"] == modInstallPath)
        {
            for (const auto &[modFile, gameFile] : installedMod["Files"].items())
            {
                if (std::filesystem::exists(gameFile.get<std::string>()))
                {
                    std::filesystem::remove(gameFile.get<std::string>());
                }
            }

            modInstallations.erase(std::remove(modInstallations.begin(), modInstallations.end(), installedMod), modInstallations.end());

            std::ofstream fileOut(path + "/" + "mods.json");
            if (fileOut.is_open())
            {
                fileOut << modInstallations.dump(4);
                fileOut.close();
            }
            else
            {
                throw std::runtime_error("Failed to open mods.json.");
            }

            return true;
        }
    }

    throw std::runtime_error("Mod installation not found.");
}

bool ModManager::uninstall_pak_mod(const std::string &path, const std::string &modPath, const std::string &modInstallPath)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath))
    {
        return false;
    }

    nlohmann::json modInstallations = JsonUtils::load_json(path + "/mods.json");
    std::vector<nlohmann::json> pakModInstallations;

    for (auto &installedMod : modInstallations)
    {
        bool IsPakMod = false;
        for (const auto &[modFile, gameFile] : installedMod["Files"].items())
        {
            if (modFile.substr(modFile.find_last_of(".") + 1) == "pak")
            {
                IsPakMod = true;
                break;
            }
        }

        if (IsPakMod)
        {
            pakModInstallations.push_back(installedMod);
        }
    }

    for (auto &installedPakMod : pakModInstallations)
    {
        ModManager::uninstall_mod(path, installedPakMod["SourcePath"], path + "/Game/");
    }

    std::vector<nlohmann::json> pakModsToReinstall = ModManager::remove_mod_from_list(pakModInstallations, modPath, path + "/Game/");
    JsonUtils::create_or_update_json(path + "/profile.json", "PatchReEnginePakIndex", 2, true);

    for (auto &modToReinstall : pakModsToReinstall)
    {
        ModManager::install_mod(path, modToReinstall["SourcePath"], path + "/Game/", path + "/Game/");
    }

    return true;
}

std::vector<nlohmann::json> ModManager::remove_mod_from_list(const std::vector<nlohmann::json> &listToPatch, const std::string &modPath, const std::string &modInstallPath)
{
    if (!std::filesystem::exists(modPath))
    {
        throw;
    }

    std::vector<nlohmann::json> modInstallations = listToPatch;

    for (auto it = modInstallations.begin(); it != modInstallations.end();)
    {
        if ((*it)["SourcePath"] == modPath && (*it)["InstallPath"] == modInstallPath)
        {
            it = modInstallations.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return modInstallations;
}

int ModManager::get_game_selection(const std::string &path)
{
    nlohmann::json j = JsonUtils::load_json(path + "/" + "profile.json");

    if (j != nlohmann::json())
    {
        return j["LastSelectedGame"];
    }

    return 0;
}