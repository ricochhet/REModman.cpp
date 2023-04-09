#ifndef REMODMAN_INCLUDED
#define REMODMAN_INCLUDED

#pragma once

#include <ImGuiFileDialog.h>
#include <Logger.h>
#include <ManagerImpl.h>
#include <RisePakPatch.h>
#include <imgui.h>

namespace FileDialog {
    void drawLoadProfileDlg();
    void drawGetGamePathDlg();
}

namespace REModman {
    void drawLoadProfile();
    void drawContextSelector();
    void drawAddGameFolderDlg();
    void drawGetGamePath();
    void drawGameSelector();

    void drawAvailableModList();
    void drawStagedModList();
    void drawDeployButton();
    void drawInstalledModList();
}

#endif  // REMODMAN_INCLUDED