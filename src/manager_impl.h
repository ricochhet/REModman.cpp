#ifndef REMODMAN_IMPL_INCLUDED
#define REMODMAN_IMPL_INCLUDED

#pragma once

#include <json_utils.h>
#include <utils.h>

#include <json.hpp>
#include <variant>

class ManagerImpl {
public:
    static ManagerImpl& Instance();
    //----------------------------------
    // [SECTION] Config implementation
    //----------------------------------
    std::string getCurrentWorkingDirectory() { return m_CurrentWorkingDirectory; }

    std::string getSelectedGamePath() { return m_SelectedGamePath; }

    bool getHandleNumericalPaks() { return m_HandleNumericalPaks; }

    void setCurrentWorkingDirectory(const std::string& path);
    void setSelectedGamePath(const std::string& path);
    void setSelectedGamePath();
    void setHandleNumericalPaks(const bool& valu);
    void setHandleNumericalPaks();

    //----------------------------------
    // [SECTION] Patch implementation
    //----------------------------------
    void patchConfig(const int& pakPatchIndex, const bool& update);
    bool containsPakFiles(const std::string& modPath);

    //----------------------------------
    // [SECTION] Manager implementation
    //----------------------------------
    void                     doSetupChecks();
    void                     refreshModEntries();
    std::vector<std::string> getAvailableModEntries() { return m_AvailableModEntries; }

    std::vector<std::string> getStagedModEntries() { return m_StagedModEntries; }

    std::vector<std::string> getInstalledModEntries() { return m_InstalledModEntries; }

    void doStageMod(const std::string& modPath, const int& stageIndex);
    void doUnstageMod(const std::string& modPath);
    void doInstallMod(const std::string& modPath);
    void doUninstallMod(const std::string& modPath);
    void doUninstallPak(const std::string& modPath);

private:
    ManagerImpl();
    ManagerImpl(const ManagerImpl&)            = delete;
    ManagerImpl& operator=(const ManagerImpl&) = delete;

    //----------------------------------
    // [SECTION] Data implementation
    //----------------------------------
    struct File;
    File createFile(const std::string& sourcePath, const std::string& installPath);
    struct Mod;
    Mod              createMod(const std::string& sourcePath, const std::vector<File>& files);
    Mod              jsonToMod(const nlohmann::json& json);
    nlohmann::json   modToJson(const Mod& mod);
    std::vector<Mod> allJsonToMods(const nlohmann::json& json);
    nlohmann::json   allModsToJson(const std::vector<Mod>& mods);

    //----------------------------------
    // [SECTION] Config implementation
    //----------------------------------
    std::string m_CurrentWorkingDirectory;
    std::string m_SelectedGamePath;
    std::string m_SelectedGameName;
    bool        m_HandleNumericalPaks;

    //----------------------------------
    // [SECTION] Patch implementation
    //----------------------------------
    struct GenericPakPatch;
    GenericPakPatch createGenericPakPatch(const bool& isPakMod, const int& pakPatchIndex);
    GenericPakPatch patchPak(const std::string& modPath);

    //----------------------------------
    // [SECTION] Manager implementation
    //----------------------------------
    std::vector<std::string> m_AvailableModEntries;
    std::vector<std::string> m_StagedModEntries;
    std::vector<std::string> m_InstalledModEntries;
    void                     setAvailableModEntries();
    void                     setStagedModEntries();
    void                     setInstalledModEntries();
    std::vector<std::string> getModDirectories();
    std::vector<std::string> getModEntries(const std::string& key, const bool& compareWithDirectories);
    std::vector<File>        getModFiles(const std::string& modPath, const bool& doPatch);
    std::vector<Mod>         removeModFromList(const std::vector<Mod>& listToRemoveFrom, const std::string& modPath);
};

#endif  // REMODMAN_IMPL_INCLUDED