#include <ModManager.h>

std::vector<ModEntry> ModManager::get_mod_entries(const std::string &path)
{
    std::vector<ModEntry> modEntries;

    for (const auto &fileEntry : std::filesystem::directory_iterator(path + "/Mods/"))
    {
        if (fileEntry.is_directory())
        {
            ModEntry modEntry;
            modEntry.Name = fileEntry.path().filename().string();
            modEntry.Path = fileEntry.path().string();
            modEntries.push_back(modEntry);
        }
    }

    return modEntries;
}

bool ModManager::contains_pak_files(const std::string &path)
{
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

bool ModManager::install_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &modInstallPath)
{
    nlohmann::json j = JsonUtils::load_json(path + "/profile.json");
    std::string pakModPrefix = "re_chunk_000.pak.patch_";
    std::string pakModSuffix = ".pak";
    nlohmann::json modFiles;
    int pakModIndex = j["PATCH_REENGINE_PAK"];
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
                JsonUtils::create_or_update_json(path + "/profile.json", "PATCH_REENGINE_PAK", pakModIndex, true);
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
        if (std::filesystem::exists(gameFile.get<std::string>()))
        {
            throw std::runtime_error("Mod file already exists in game directory.");
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

    nlohmann::json modInstallations;
    std::ifstream fileIn(path + "/" + "mods.json");
    if (fileIn.is_open())
    {
        fileIn >> modInstallations;
        fileIn.close();
    }
    modInstallations.push_back(installedMod);

    JsonUtils::write_json_to_file(path + "/" + "mods.json", modInstallations);
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

bool ModManager::uninstall_mod(const std::string &path, const std::string &modPath, const std::string &modInstallPath)
{
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

bool ModManager::uninstall_pak_mod(const ModEntry &modEntry, const std::string &path, const std::string &modPath, const std::string &modInstallPath)
{
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

    std::vector<nlohmann::json> pakModsToReinstall = ModManager::remove_mod_from_list(pakModInstallations, modEntry.Path, path + "/Game/");
    JsonUtils::create_or_update_json(path + "/profile.json", "PATCH_REENGINE_PAK", 2, true);

    for (auto &modToReinstall : pakModsToReinstall)
    {
        ModManager::install_mod(path, modToReinstall["SourcePath"], path + "/Game/", path + "/Game/");
    }
}

std::vector<nlohmann::json> ModManager::remove_mod_from_list(const std::vector<nlohmann::json> &listToPatch, const std::string &modPath, const std::string &modInstallPath)
{
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