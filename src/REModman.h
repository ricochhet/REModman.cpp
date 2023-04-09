#ifndef REMODMAN_INCLUDED
#define REMODMAN_INCLUDED

#pragma once

#include <ImGuiFileDialog.h>
#include <Logger.h>
#include <ManagerImpl.h>
#include <RisePakPatch.h>
#include <imgui.h>
#include <format>

namespace FileDialog {
    void drawLoadProfileDlg();
    void drawGetGamePathDlg();
}

namespace REModman {
    void drawLoadProfile();
    void drawGetGamePath();
    void drawGameSelector();

    void drawAvailableModList();
    void drawStagedModList();
    void drawDeployButton();
    void drawInstalledModList();
}

#endif  // REMODMAN_INCLUDED