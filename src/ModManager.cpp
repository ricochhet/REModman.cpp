#include <ModManager.h>

void ModManager::startup_health_checks(const std::string &path)
{
    Utils::create_directory(path + "/Mods/");
    Utils::create_directory(path + "/Downloads/");
    JsonUtils::write_empty_json_to_file(path + "/" + "profile.json");
    JsonUtils::write_empty_json_to_file(path + "/" + "mods_staging.json");
    JsonUtils::write_empty_json_to_file(path + "/" + "mods_installed.json");
}

std::vector<std::string> ModManager::get_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    std::vector<std::string> modEntries;

    for (const auto &fileEntry : std::filesystem::directory_iterator(path + "/Mods/"))
    {
        if (fileEntry.is_directory())
        {
            modEntries.push_back(fileEntry.path().string());
        }
    }

    return modEntries;
}

std::vector<std::string> ModManager::get_available_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    std::vector<ModManagerData::Mod> modInstallations = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/" + "mods_installed.json"));
    std::vector<std::string> modEntries = get_mod_entries(path);
    std::vector<std::string> currentModEntries;

    for (auto &entry : modEntries)
    {
        bool found = false;
        for (auto &installedMod : modInstallations)
        {
            if (installedMod.SourcePath == entry)
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

std::vector<std::string> ModManager::get_staged_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    std::vector<ModManagerData::Mod> stagedMods = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/mods_staging.json"));
    std::vector<std::string> stagedModEntries;

    for (auto &entry : stagedMods)
    {
        stagedModEntries.push_back(entry.SourcePath);
    }

    return stagedModEntries;
}

std::vector<std::string> ModManager::get_installed_mod_entries(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    std::vector<ModManagerData::Mod> modInstallations = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/mods_installed.json"));
    std::vector<std::string> modInstallationEntries;

    for (auto &installedMod : modInstallations)
    {
        modInstallationEntries.push_back(installedMod.SourcePath);
    }

    return modInstallationEntries;
}

bool ModManager::contains_pak_files(const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
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

bool ModManager::stage_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const int stagingIndex)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath) || !std::filesystem::exists(gamePath))
    {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        return false;
    }

    std::vector<ModManagerData::File> modFiles;

    for (const auto &fileEntry : std::filesystem::recursive_directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string relative_path = std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string filename = fileEntry.path().filename().string();
            std::string gameFile = gamePath + "/" + relative_path;
            
            ModManagerData::File file;
            file.SourcePath = fileEntry.path().string();
            file.InstallPath = gameFile;
            modFiles.push_back(file);
        }
    }

    ModManagerData::Mod installedMod;
    installedMod.SourcePath = modPath;
    installedMod.Files = modFiles;

    std::vector<ModManagerData::Mod> modInstallations = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/" + "mods_staging.json"));
    size_t stagingIterator = stagingIndex;
    if (stagingIterator >= modInstallations.size())
    {
        stagingIterator = modInstallations.size();
    }

    std::vector<ModManagerData::Mod>::iterator stageIter = modInstallations.begin() + stagingIterator;
    modInstallations.insert(stageIter, installedMod);
    JsonUtils::write_json_to_file(path + "/" + "mods_staging.json", ModManagerData::mods_to_json(modInstallations));

    return true;
}

bool ModManager::destage_mod(const std::string &path, const std::string &modPath)
{
    std::vector<ModManagerData::Mod> j = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/" + "mods_staging.json"));
    j = ModManager::remove_mod_from_list(j, modPath);
    JsonUtils::write_json_to_file(path + "/" + "mods_staging.json", ModManagerData::mods_to_json(j));

    return true;
}

bool ModManager::install_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &gameSelection)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath) || !std::filesystem::exists(gamePath))
    {
        return false;
    }

    ModManagerPatches::MonsterHunterRise::PatchReEnginePak mhrPatchReEnginePak =
        ModManagerPatches::MonsterHunterRise::patch_re_engine_pak(path, modPath);

    std::vector<ModManagerData::File> modFiles;
    for (const auto &fileEntry : std::filesystem::recursive_directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string relativePath = std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string fileName = fileEntry.path().filename().string();

            if (mhrPatchReEnginePak.isPak && gameSelection == "MonsterHunterRise")
            {
                std::string pakFileName =
                    "re_chunk_000.pak.patch_" + std::to_string(mhrPatchReEnginePak.pakIndex).insert(0, 3 - std::to_string(mhrPatchReEnginePak.pakIndex).length(), '0') + ".pak";
                relativePath = Utils::string_replace_all(relativePath, fileName, pakFileName);
            }

            std::string gameFile = gamePath + "/" + relativePath;
            ModManagerData::File file;
            file.SourcePath = fileEntry.path().string();
            file.InstallPath = gameFile;
            modFiles.push_back(file);
        }
    }

    for (const auto &file : modFiles)
    {
        std::filesystem::create_directories(std::filesystem::path(file.InstallPath).parent_path());
        std::filesystem::copy_file(file.SourcePath, file.InstallPath, std::filesystem::copy_options::overwrite_existing);
    }

    ModManagerData::Mod installedMod;
    installedMod.SourcePath = modPath;
    installedMod.Files = modFiles;

    std::vector<ModManagerData::Mod> modInstallations = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/" + "mods_installed.json"));
    modInstallations.push_back(installedMod);
    JsonUtils::write_json_to_file(path + "/" + "mods_installed.json", ModManagerData::mods_to_json(modInstallations));

    return true;
}

bool ModManager::uninstall_mod(const std::string &path, const std::string &modPath)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath))
    {
        return false;
    }

    std::vector<ModManagerData::Mod> modInstallations = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/" + "mods_installed.json"));

    for (const auto &installedMod : modInstallations)
    {
        if (installedMod.SourcePath == modPath)
        {
            for (const auto &file : installedMod.Files)
            {
                if (std::filesystem::exists(file.InstallPath))
                {
                    std::filesystem::remove(file.InstallPath);
                }
            }

            modInstallations = ModManager::remove_mod_from_list(modInstallations, installedMod.SourcePath);
            JsonUtils::write_json_to_file(path + "/mods_installed.json", ModManagerData::mods_to_json(modInstallations));

            return true;
        }
    }

    Logger::getInstance().log("Mod installation not found.", LogLevel::Warning);
    throw std::runtime_error("Mod installation not found.");
}

bool ModManager::uninstall_pak_mod(const std::string &path, const std::string &modPath, const std::string &gamePath, const std::string &gameSelection)
{
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath))
    {
        return false;
    }

    std::vector<ModManagerData::Mod> modInstallations = ModManagerData::mods_from_json(JsonUtils::load_json(path + "/" + "mods_installed.json"));
    std::vector<ModManagerData::Mod> pakModInstallations;

    for (const auto &installedMod : modInstallations)
    {
        bool IsPakMod = false;
        for (const auto &file : installedMod.Files)
        {
            if (file.SourcePath.substr(file.SourcePath.find_last_of(".") + 1) == "pak")
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

    for (const auto &installedPakMod : pakModInstallations)
    {
        ModManager::uninstall_mod(path, installedPakMod.SourcePath);
    }

    std::vector<ModManagerData::Mod> pakModsToReinstall = ModManager::remove_mod_from_list(pakModInstallations, modPath);
    JsonUtils::create_or_update_json(path + "/profile.json", {"Patches", "MonsterHunterRise", "PatchReEnginePakIndex"}, 2, true);

    for (const auto &modToReinstall : pakModsToReinstall)
    {
        ModManager::install_mod(path, modToReinstall.SourcePath, gamePath, gameSelection);
    }

    return true;
}

std::vector<ModManagerData::Mod> ModManager::remove_mod_from_list(const std::vector<ModManagerData::Mod> &listToPatch, const std::string &modPath)
{
    if (!std::filesystem::exists(modPath))
    {
        Logger::getInstance().log("Mod path does not exist: " + modPath, LogLevel::Warning);
        throw;
    }

    std::vector<ModManagerData::Mod> modInstallations = listToPatch;

    for (auto it = modInstallations.begin(); it != modInstallations.end();)
    {
        if ((*it).SourcePath == modPath)
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

int ModManager::get_last_selected_game(const std::string &path)
{
    int lastSelectedGameIndex = JsonUtils::get_integer_value(path + "/" + "profile.json", {"LastSelectedGame"});
    Logger::getInstance().log("Found profile path: " + path, LogLevel::Info);
    return lastSelectedGameIndex;
}

std::string ModManager::get_game_path(const std::string &path, const std::string &selection)
{
    std::string gamePath = JsonUtils::get_string_value(path + "/" + "profile.json", {"Games", selection + "GamePath"});

    if (!gamePath.empty())
    {
        Logger::getInstance().log("Found game path: " + path, LogLevel::Info);
        return gamePath;
    }

    return std::string();
}