#ifndef REMODMAN_IMPL_INCLUDED
#define REMODMAN_IMPL_INCLUDED

#pragma once

#include <JsonUtils.h>
#include <Utils.h>

#include <json.hpp>
#include <variant>

class ManagerImpl {
public:
    static ManagerImpl& getInstance();
    //----------------------------------
    // [SECTION] Config implementation
    //----------------------------------
    std::string getCurrentWorkingDirectory() { return m_CurrentWorkingDirectory; }

    int getSelectedGameIndex() { return m_SelectedGameIndex; }

    std::string getSelectedGamePath() { return m_SelectedGamePath; }

    bool getHandlePakPatching() { return m_HandlePakPatching; }

    void setGameSelectionItems(const std::vector<std::string>& items) { m_GameSelectionItems = items; }

    void                     setCurrentWorkingDirectory(const std::string& path);
    void                     setSelectedGameIndex(const int& index);
    void                     setSelectedGameIndex();
    void                     setSelectedGamePath(const std::string& path, const std::string& selectedGameName);
    void                     setHandlePakPatching(const bool& valu);
    void                     setHandlePakPatching();
    std::vector<std::string> getGameSelectionItems() { return m_GameSelectionItems; }

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
    std::string              m_CurrentWorkingDirectory;
    int                      m_SelectedGameIndex;
    std::string              m_SelectedGamePath;
    std::string              m_SelectedGameName;
    bool                     m_HandlePakPatching;
    std::vector<std::string> m_GameSelectionItems;

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