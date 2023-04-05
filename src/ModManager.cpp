#include <ModManager.h>

void ModManager::initialize(const std::string& path) {
    Utils::create_directory(path + MODS_FOLDER);
    Utils::create_directory(path + DOWNLOADS_FOLDER);
    JsonUtils::write_empty_json_to_file(path + PROFILE_FILE_NAME);
    JsonUtils::write_empty_json_to_file(path + MODS_STAGING_FILE_NAME);
    JsonUtils::write_empty_json_to_file(path + MODS_INSTALLED_FILE_NAME);
}

std::vector<std::string> ModManager::get_mod_directories(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    std::vector<std::string> modEntries;

    for (const auto& fileEntry : std::filesystem::directory_iterator(path + MODS_FOLDER)) {
        if (fileEntry.is_directory()) {
            modEntries.push_back(fileEntry.path().string());
        }
    }

    return modEntries;
}

std::vector<std::string> ModManager::get_mod_entries(
    const std::string& path, const std::string& file, const bool compare
) {
    if (!std::filesystem::exists(path)) {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        throw;
    }

    std::vector<ModManagerData::Mod> modInstallations =
        ModManagerData::mods_from_json(JsonUtils::load_json(path + file));
    std::vector<std::string> modEntries;
    std::vector<std::string> compareEntries;

    if (compare) {
        compareEntries = get_mod_directories(path);

        for (const auto& entry : compareEntries) {
            bool found = false;
            for (const auto& installedMod : modInstallations) {
                if (installedMod.SourcePath == entry) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                modEntries.push_back(entry);
            }
        }
    } else {
        for (auto& entry : modInstallations) {
            modEntries.push_back(entry.SourcePath);
        }
    }

    return modEntries;
}

bool ModManager::stage_mod(
    const std::string& path, const std::string& modPath, const std::string& gamePath,
    const int stagingIndex
) {
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath) ||
        !std::filesystem::exists(gamePath)) {
        Logger::getInstance().log("File path does not exist: " + path, LogLevel::Warning);
        return false;
    }

    std::vector<ModManagerData::File> modFiles;

    for (const auto& fileEntry : std::filesystem::recursive_directory_iterator(modPath)) {
        if (fileEntry.is_regular_file()) {
            std::string relativePath =
                std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string gameFile = gamePath + "/" + relativePath;

            ModManagerData::File file;
            file.SourcePath  = fileEntry.path().string();
            file.InstallPath = gameFile;
            modFiles.push_back(file);
        }
    }

    ModManagerData::Mod installedMod;
    installedMod.SourcePath = modPath;
    installedMod.Files      = modFiles;

    std::vector<ModManagerData::Mod> modInstallations =
        ModManagerData::mods_from_json(JsonUtils::load_json(path + MODS_STAGING_FILE_NAME));
    size_t stagingIterator = stagingIndex;
    if (stagingIterator >= modInstallations.size()) {
        stagingIterator = modInstallations.size();
    }

    std::vector<ModManagerData::Mod>::iterator stageIter =
        modInstallations.begin() + stagingIterator;
    modInstallations.insert(stageIter, installedMod);
    JsonUtils::write_json_to_file(
        path + MODS_STAGING_FILE_NAME, ModManagerData::mods_to_json(modInstallations)
    );

    return true;
}

bool ModManager::destage_mod(const std::string& path, const std::string& modPath) {
    std::vector<ModManagerData::Mod> j =
        ModManagerData::mods_from_json(JsonUtils::load_json(path + MODS_STAGING_FILE_NAME));
    j = ModManager::remove_mod_from_list(j, modPath);
    JsonUtils::write_json_to_file(path + MODS_STAGING_FILE_NAME, ModManagerData::mods_to_json(j));

    return true;
}

bool ModManager::install_mod(
    const std::string& path, const std::string& modPath, const std::string& gamePath,
    const std::string& gameSelection
) {
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath) ||
        !std::filesystem::exists(gamePath)) {
        return false;
    }

    ModManagerPatches::MonsterHunterRise::PatchReEnginePak mhrPatchReEnginePak =
        ModManagerPatches::MonsterHunterRise::patch_re_engine_pak(path, modPath);

    std::vector<ModManagerData::File> modFiles;
    for (const auto& fileEntry : std::filesystem::recursive_directory_iterator(modPath)) {
        if (fileEntry.is_regular_file()) {
            std::string relativePath =
                std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string fileName = fileEntry.path().filename().string();

            if (mhrPatchReEnginePak.isPak && gameSelection == "MonsterHunterRise") {
                std::string pakFileName =
                    "re_chunk_000.pak.patch_" +
                    std::to_string(mhrPatchReEnginePak.pakIndex)
                        .insert(0, 3 - std::to_string(mhrPatchReEnginePak.pakIndex).length(), '0') +
                    ".pak";
                relativePath = Utils::string_replace_all(relativePath, fileName, pakFileName);
            }

            std::string          gameFile = gamePath + "/" + relativePath;
            ModManagerData::File file;
            file.SourcePath  = fileEntry.path().string();
            file.InstallPath = gameFile;
            modFiles.push_back(file);
        }
    }

    for (const auto& file : modFiles) {
        std::filesystem::create_directories(std::filesystem::path(file.InstallPath).parent_path());
        std::filesystem::copy_file(
            file.SourcePath, file.InstallPath, std::filesystem::copy_options::overwrite_existing
        );
    }

    ModManagerData::Mod installedMod;
    installedMod.SourcePath = modPath;
    installedMod.Files      = modFiles;

    std::vector<ModManagerData::Mod> modInstallations =
        ModManagerData::mods_from_json(JsonUtils::load_json(path + MODS_INSTALLED_FILE_NAME));
    modInstallations.push_back(installedMod);
    JsonUtils::write_json_to_file(
        path + MODS_INSTALLED_FILE_NAME, ModManagerData::mods_to_json(modInstallations)
    );

    return true;
}

bool ModManager::uninstall_mod(const std::string& path, const std::string& modPath) {
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath)) {
        return false;
    }

    std::vector<ModManagerData::Mod> modInstallations =
        ModManagerData::mods_from_json(JsonUtils::load_json(path + MODS_INSTALLED_FILE_NAME));

    for (const auto& installedMod : modInstallations) {
        if (installedMod.SourcePath == modPath) {
            for (const auto& file : installedMod.Files) {
                if (std::filesystem::exists(file.InstallPath)) {
                    std::filesystem::remove(file.InstallPath);
                }
            }

            modInstallations =
                ModManager::remove_mod_from_list(modInstallations, installedMod.SourcePath);
            JsonUtils::write_json_to_file(
                path + MODS_INSTALLED_FILE_NAME, ModManagerData::mods_to_json(modInstallations)
            );

            return true;
        }
    }

    Logger::getInstance().log("Mod installation not found.", LogLevel::Warning);
    throw std::runtime_error("Mod installation not found.");
}

bool ModManager::uninstall_pak_mod(
    const std::string& path, const std::string& modPath, const std::string& gamePath,
    const std::string& gameSelection
) {
    if (!std::filesystem::exists(path) || !std::filesystem::exists(modPath)) {
        return false;
    }

    std::vector<ModManagerData::Mod> modInstallations =
        ModManagerData::mods_from_json(JsonUtils::load_json(path + MODS_INSTALLED_FILE_NAME));
    std::vector<ModManagerData::Mod> pakModInstallations;

    for (const auto& installedMod : modInstallations) {
        bool IsPakMod = false;
        for (const auto& file : installedMod.Files) {
            if (file.SourcePath.substr(file.SourcePath.find_last_of(".") + 1) == "pak") {
                IsPakMod = true;
                break;
            }
        }

        if (IsPakMod) {
            pakModInstallations.push_back(installedMod);
        }
    }

    for (const auto& installedPakMod : pakModInstallations) {
        ModManager::uninstall_mod(path, installedPakMod.SourcePath);
    }

    std::vector<ModManagerData::Mod> pakModsToReinstall =
        ModManager::remove_mod_from_list(pakModInstallations, modPath);

    if (gameSelection == "MonsterHunterRise") {
        ModManagerPatches::MonsterHunterRise::set_patch_in_profile(path, 2);
    }

    for (const auto& modToReinstall : pakModsToReinstall) {
        ModManager::install_mod(path, modToReinstall.SourcePath, gamePath, gameSelection);
    }

    return true;
}

std::vector<ModManagerData::Mod> ModManager::remove_mod_from_list(
    const std::vector<ModManagerData::Mod>& listToPatch, const std::string& modPath
) {
    if (!std::filesystem::exists(modPath)) {
        Logger::getInstance().log("Mod path does not exist: " + modPath, LogLevel::Warning);
        throw;
    }

    std::vector<ModManagerData::Mod> modInstallations = listToPatch;

    for (auto it = modInstallations.begin(); it != modInstallations.end();) {
        if ((*it).SourcePath == modPath) {
            it = modInstallations.erase(it);
        } else {
            ++it;
        }
    }

    return modInstallations;
}