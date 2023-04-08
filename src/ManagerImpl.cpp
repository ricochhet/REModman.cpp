#include "ManagerImpl.h"

ManagerImpl& ManagerImpl::getInstance() {
    static ManagerImpl instance;
    return instance;
}

//----------------------------------
// [SECTION] Data implementation
//----------------------------------
struct ManagerImpl::File {
    std::string SourcePath;
    std::string InstallPath;
};

ManagerImpl::File ManagerImpl::createFile(const std::string& sourcePath, const std::string& installPath) {
    File tempFile;
    tempFile.SourcePath  = sourcePath;
    tempFile.InstallPath = installPath;

    return tempFile;
}

struct ManagerImpl::Mod {
    std::string       SourcePath;
    std::vector<File> Files;
};

ManagerImpl::Mod ManagerImpl::createMod(const std::string& sourcePath, const std::vector<File>& files) {
    Mod tempMod;
    tempMod.SourcePath = sourcePath;
    tempMod.Files      = files;

    return tempMod;
}

ManagerImpl::Mod ManagerImpl::jsonToMod(const nlohmann::json& json) {
    Mod tempMod;
    tempMod.SourcePath = json["SourcePath"];
    tempMod.Files.clear();

    for (const auto& obj : json["Files"]) {
        File file;
        file.SourcePath  = obj["SourcePath"];
        file.InstallPath = obj["InstallPath"];

        tempMod.Files.push_back(file);
    }

    return tempMod;
}

nlohmann::json ManagerImpl::modToJson(const Mod& mod) {
    nlohmann::json tempJson;
    tempJson["SourcePath"] = mod.SourcePath;
    tempJson["Files"]      = nlohmann::json::array();

    for (const auto& file : mod.Files) {
        nlohmann::json obj;
        obj["SourcePath"]  = file.SourcePath;
        obj["InstallPath"] = file.InstallPath;

        tempJson["Files"].push_back(obj);
    }

    return tempJson;
}

std::vector<ManagerImpl::Mod> ManagerImpl::allJsonToMods(const nlohmann::json& json) {
    std::vector<Mod> tempMods;

    for (const auto& obj : json) {
        tempMods.push_back(jsonToMod(obj));
    }

    return tempMods;
}

nlohmann::json ManagerImpl::allModsToJson(const std::vector<Mod>& mods) {
    nlohmann::json tempJson;

    for (const auto& mod : mods) {
        tempJson.push_back(modToJson(mod));
    }

    if (tempJson.empty()) {
        return nlohmann::json::array();
    }

    return tempJson;
}

//----------------------------------
// [SECTION] Config implementation
//----------------------------------
void ManagerImpl::setCurrentWorkingDirectory(const std::string& path) {
    m_CurrentWorkingDirectory = path;
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"SelectedProfile"}, m_CurrentWorkingDirectory, true);
}

void ManagerImpl::setSelectedGameIndex(const int& index) {
    if (!m_CurrentWorkingDirectory.empty()) {
        JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"LastSelectedGame"}, index, true);
        int value           = JsonUtils::getInt(m_CurrentWorkingDirectory + "/profile.json", {"LastSelectedGame"});
        m_SelectedGameIndex = value;
    }
}

void ManagerImpl::setSelectedGameIndex() {
    int value           = JsonUtils::getInt(m_CurrentWorkingDirectory + "/profile.json", {"LastSelectedGame"});
    m_SelectedGameIndex = value;
}

void ManagerImpl::setSelectedGamePath(const std::string& path, const std::string& selectedGameName) {
    if (!m_CurrentWorkingDirectory.empty()) {
        std::string value  = JsonUtils::getString(m_CurrentWorkingDirectory + "/profile.json", {"Games", selectedGameName});
        m_SelectedGameName = selectedGameName;

        if (!value.empty()) {
            m_SelectedGamePath = value;
        } else if (value.empty() && !path.empty()) {
            m_SelectedGamePath = path;
            JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"Games", selectedGameName}, m_SelectedGamePath, true);
        }
    }
}

//----------------------------------
// [SECTION] Patch implementation
//----------------------------------
struct ManagerImpl::GenericPakPatch {
    bool isPakMod;
    int  pakPatchIndex;
};

ManagerImpl::GenericPakPatch ManagerImpl::createGenericPakPatch(const bool& isPakMod, const int& pakPatchIndex) {
    GenericPakPatch tempGenericPakPatch;
    tempGenericPakPatch.isPakMod      = isPakMod;
    tempGenericPakPatch.pakPatchIndex = pakPatchIndex;

    return tempGenericPakPatch;
}

ManagerImpl::GenericPakPatch ManagerImpl::patchPak(const std::string& modPath) {
    int  pakPatchIndex = JsonUtils::getInt(m_CurrentWorkingDirectory + "/profile.json", {"Patches", "MonsterHunterRise", "PatchPakIndex"});
    bool isPakMod      = false;
    for (const auto& file : std::filesystem::directory_iterator(modPath)) {
        if (file.is_regular_file()) {
            std::string fileName = file.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak") {
                isPakMod = true;
                pakPatchIndex++;

                JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"Patches", "MonsterHunterRise", "PatchPakIndex"}, pakPatchIndex, true);
                break;
            }
        }
    }

    return createGenericPakPatch(isPakMod, pakPatchIndex);
}

bool ManagerImpl::containsPakFiles(const std::string& modPath) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory) || !std::filesystem::exists(modPath)) {
        Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
        return false;
    }

    for (const auto& file : std::filesystem::directory_iterator(modPath)) {
        if (file.is_regular_file()) {
            std::string fileName = file.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak") {
                return true;
            }
        }
    }

    return false;
}

void ManagerImpl::patchConfig(const int& pakPatchIndex) {
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"Patches", "MonsterHunterRise", "PatchPakIndex"}, pakPatchIndex, true);
}

//----------------------------------
// [SECTION] Manager implementation
//----------------------------------
void ManagerImpl::refreshModEntries() {
    setAvailableModEntries();
    setStagedModEntries();
    setInstalledModEntries();
}

void ManagerImpl::setAvailableModEntries() { m_AvailableModEntries = getModEntries("InstalledMods", true); }

void ManagerImpl::setStagedModEntries() { m_StagedModEntries = getModEntries("StagedMods", false); }

void ManagerImpl::setInstalledModEntries() { m_InstalledModEntries = getModEntries("InstalledMods", false); }

void ManagerImpl::doSetupChecks() {
    Utils::createDirectory(m_CurrentWorkingDirectory + "/Mods/");
    Utils::createDirectory(m_CurrentWorkingDirectory + "/Downloads/");

    JsonUtils::writeJson(m_CurrentWorkingDirectory + "/profile.json");
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"StagedMods"}, nlohmann::json::array(), false);
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"InstalledMods"}, nlohmann::json::array(), false);
}

std::vector<std::string> ManagerImpl::getModDirectories() {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory)) {
        Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
        return std::vector<std::string>();
    }

    std::vector<std::string> modDirectories;
    for (const auto& path : std::filesystem::directory_iterator(m_CurrentWorkingDirectory + "/Mods/")) {
        if (path.is_directory()) {
            modDirectories.push_back(path.path().string());
        }
    }

    return modDirectories;
}

std::vector<std::string> ManagerImpl::getModEntries(const std::string& key, const bool& compareWithDirectories) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory)) {
        Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
        return std::vector<std::string>();
    }

    std::vector<Mod>         installedMods = allJsonToMods(JsonUtils::loadJson(m_CurrentWorkingDirectory + "/profile.json")[key]);
    std::vector<std::string> compareDirectories;
    std::vector<std::string> modsToReturn;

    if (compareWithDirectories) {
        compareDirectories = getModDirectories();

        for (const auto& path : compareDirectories) {
            bool found = false;

            for (const auto& installedMod : installedMods) {
                if (installedMod.SourcePath == path) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                modsToReturn.push_back(path);
            }
        }
    } else {
        for (const auto& path : installedMods) {
            modsToReturn.push_back(path.SourcePath);
        }
    }

    return modsToReturn;
}

std::vector<ManagerImpl::File> ManagerImpl::getModFiles(const std::string& modPath, const bool& doPatch) {
    std::vector<File> modFiles;

    GenericPakPatch genericPakPatch;
    if (doPatch) {
        genericPakPatch = patchPak(modPath);
    }

    for (const auto& fileEntry : std::filesystem::recursive_directory_iterator(modPath)) {
        if (fileEntry.is_regular_file()) {
            std::string relativePath = std::filesystem::relative(fileEntry.path(), modPath).string();
            std::string fileName     = fileEntry.path().filename().string();

            if (genericPakPatch.isPakMod && m_SelectedGameName == "MonsterHunterRise" && doPatch) {
                std::string pakFileName =
                    "re_chunk_000.pak.patch_" + std::to_string(genericPakPatch.pakPatchIndex).insert(0, 3 - std::to_string(genericPakPatch.pakPatchIndex).length(), '0') + ".pak";
                relativePath = Utils::replaceAllInString(relativePath, fileName, pakFileName);
            }

            modFiles.push_back(createFile(fileEntry.path().string(), m_SelectedGamePath + "/" + relativePath));
        }
    }

    return modFiles;
}

void ManagerImpl::doStageMod(const std::string& modPath, const int& stageIndex) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory) || !std::filesystem::exists(modPath)) {
        return Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
    }

    std::vector<File> modFiles          = getModFiles(modPath, false);
    Mod               modToStage        = createMod(modPath, modFiles);
    std::vector<Mod>  currentStagedMods = allJsonToMods(JsonUtils::loadJson(m_CurrentWorkingDirectory + "/profile.json")["StagedMods"]);
    size_t            stageIndexSizeT;
    if (stageIndexSizeT >= currentStagedMods.size()) {
        stageIndexSizeT = currentStagedMods.size();
    }

    std::vector<Mod>::iterator iterator = currentStagedMods.begin() + stageIndexSizeT;
    currentStagedMods.insert(iterator, modToStage);
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"StagedMods"}, allModsToJson(currentStagedMods), true);
}

void ManagerImpl::doUnstageMod(const std::string& modPath) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory) || !std::filesystem::exists(modPath)) {
        return Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
    }

    std::vector<Mod> currentStagedMods = allJsonToMods(JsonUtils::loadJson(m_CurrentWorkingDirectory + "/profile.json")["StagedMods"]);
    currentStagedMods                  = removeModFromList(currentStagedMods, modPath);
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"StagedMods"}, allModsToJson(currentStagedMods), true);
}

void ManagerImpl::doInstallMod(const std::string& modPath) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory) || !std::filesystem::exists(modPath)) {
        return Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
    }

    std::vector<File> modFiles             = getModFiles(modPath, true);
    for (const auto& file : modFiles) {
        std::filesystem::create_directories(std::filesystem::path(file.InstallPath).parent_path());
        std::filesystem::copy_file(file.SourcePath, file.InstallPath, std::filesystem::copy_options::overwrite_existing);
    }

    std::vector<Mod>  currentInstalledMods = allJsonToMods(JsonUtils::loadJson(m_CurrentWorkingDirectory + "/profile.json")["InstalledMods"]);
    currentInstalledMods.push_back(createMod(modPath, modFiles));
    JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"InstalledMods"}, allModsToJson(currentInstalledMods), true);
}

void ManagerImpl::doUninstallMod(const std::string& modPath) {
    std::vector<Mod> currentInstalledMods = allJsonToMods(JsonUtils::loadJson(m_CurrentWorkingDirectory + "/profile.json")["InstalledMods"]);

    for (const auto& installedMod : currentInstalledMods) {
        if (installedMod.SourcePath == modPath) {
            for (const auto& file : installedMod.Files) {
                if (std::filesystem::exists(file.InstallPath)) {
                    std::filesystem::remove(file.InstallPath);
                }
            }

            currentInstalledMods = removeModFromList(currentInstalledMods, installedMod.SourcePath);
            JsonUtils::updateJson(m_CurrentWorkingDirectory + "/profile.json", {"InstalledMods"}, allModsToJson(currentInstalledMods), true);
            break;
        }
    }
}

void ManagerImpl::doUninstallPak(const std::string& modPath) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory) || !std::filesystem::exists(modPath)) {
        return Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
    }

    std::vector<Mod> currentInstalledMods = allJsonToMods(JsonUtils::loadJson(m_CurrentWorkingDirectory + "/profile.json")["InstalledMods"]);
    std::vector<Mod> paksToUninstall;

    for (const auto& installedMod : currentInstalledMods) {
        for (const auto& file : installedMod.Files) {
            if (file.SourcePath.substr(file.SourcePath.find_last_of(".") + 1) == "pak") {
                paksToUninstall.push_back(installedMod);
                break;
            }
        }
    }

    for (const auto& installedPak : paksToUninstall) {
        doUninstallMod(installedPak.SourcePath);
    }

    std::vector<Mod> paksToInstall = removeModFromList(paksToUninstall, modPath);

    if (m_SelectedGameName == "MonsterHunterRise") {
        patchConfig(2);
    }

    for (const auto& pakToReinstall : paksToInstall) {
        doInstallMod(pakToReinstall.SourcePath);
    }
}

std::vector<ManagerImpl::Mod> ManagerImpl::removeModFromList(const std::vector<Mod>& listToRemoveFrom, const std::string& modPath) {
    if (!std::filesystem::exists(m_CurrentWorkingDirectory) || !std::filesystem::exists(modPath)) {
        Logger::getInstance().log("File not found: " + m_CurrentWorkingDirectory, LogLevel::Warning);
        return std::vector<Mod>();
    }

    std::vector<Mod> tempModList = listToRemoveFrom;
    for (auto it = tempModList.begin(); it != tempModList.end();) {
        if ((*it).SourcePath == modPath) {
            it = tempModList.erase(it);
        } else {
            ++it;
        }
    }

    return tempModList;
}

ManagerImpl::ManagerImpl() {}