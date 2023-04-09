#ifndef REMODMAN_INCLUDED
#define REMODMAN_INCLUDED

#pragma once

#include <ImGuiFileDialog.h>
#include <Logger.h>
#include <ManagerImpl.h>
#include <RisePakPatch.h>
#include <imgui.h>

#include <format>

class ManagerUI {
public:
    static ManagerUI& getInstance();

    //----------------------------------
    // [SECTION] Config implementation
    //----------------------------------
    void setLoadOrderInputInt(const int& order) { m_LoadOrderInputInt = order; }

    void setProfileFileDlgLabel(const std::string& label) { m_ProfileFileDlgLabel = label; }

    void setGamePathFileDlgLabel(const std::string& label) { m_GamePathFileDlgLabel = label; }

    void setProfileFileDlgKey(const std::string& label) { m_ProfileFileDlgKey = label; }

    void setGamePathFileDlgKey(const std::string& label) { m_GamePathFileDlgKey = label; }

    int getLoadOrderInputInt() { return m_LoadOrderInputInt; }

    std::string getProfileFileDlgLabel() { return m_ProfileFileDlgLabel; }
    std::string getGamePathFileDlgLabel() { return m_GamePathFileDlgLabel; }

    std::string getProfileFileDlgKey() { return m_ProfileFileDlgKey; }
    std::string getGamePathFileDlgKey() { return m_GamePathFileDlgKey; }

    //----------------------------------
    // [SECTION] Manager implementation
    //----------------------------------
    void setupVars();
    void drawProfileFileDlg();
    void drawGamePathFileDlg();
    void drawGameSelector();

    void drawAvailableMods();
    void drawStagedMods();
    void drawDeployBtn();
    void drawInstalledMods();

private:
    ManagerUI();
    ManagerUI(const ManagerUI&)            = delete;
    ManagerUI& operator=(const ManagerUI&) = delete;

    //----------------------------------
    // [SECTION] Config implementation
    //----------------------------------
    int m_LoadOrderInputInt;

    std::string m_ProfileFileDlgLabel;
    std::string m_GamePathFileDlgLabel;

    std::string m_ProfileFileDlgKey;
    std::string m_GamePathFileDlgKey;
};

#endif  // REMODMAN_INCLUDED