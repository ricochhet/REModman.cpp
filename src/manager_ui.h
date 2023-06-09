#ifndef REMODMAN_UI_INCLUDED
#define REMODMAN_UI_INCLUDED

#pragma once

#include <ImGuiFileDialog.h>
#include <imgui.h>
#include <logger.h>
#include <manager_impl.h>
#include <risepakpatch.h>

#include <format>

class ImGuiHelpers {
public:
    static void PushButtonColor(ImVec4 vec) {
        // clang-format off
        ImGui::PushStyleColor(ImGuiCol_Button,          ImVec4{vec.x, vec.y, vec.z, vec.w});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   ImVec4{vec.x + 0.1f, vec.y + 0.1f, vec.z + 0.1f, vec.w});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,    ImVec4{vec.x + 0.3f, vec.y + 0.3f, vec.z + 0.3f, vec.w});
        // clang-format on
    }

    static void PopColors() {
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
};

class ManagerUI {
public:
    static ManagerUI& Instance();

    //----------------------------------
    // [SECTION] Manager implementation
    //----------------------------------
    void setupVars();

    void drawProfileFileDlg();
    void drawGamePathFileDlg();

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
    void setLoadOrderInputInt(const int& order) { m_LoadOrderInputInt = order; }

    void setProfileFileDlgLabel(const std::string& label) { m_ProfileFileDlgLabel = label; }

    void setGamePathFileDlgLabel(const std::string& label) { m_GamePathFileDlgLabel = label; }

    void setSettingsLabel(const std::string& label) { m_SettingsLabel = label; }

    void setProfileFileDlgKey(const std::string& label) { m_ProfileFileDlgKey = label; }

    void setGamePathFileDlgKey(const std::string& label) { m_GamePathFileDlgKey = label; }

    int getLoadOrderInputInt() { return m_LoadOrderInputInt; }

    std::string getProfileFileDlgLabel() { return m_ProfileFileDlgLabel; }
    std::string getGamePathFileDlgLabel() { return m_GamePathFileDlgLabel; }
    std::string getSettingsLabel() { return m_SettingsLabel; }

    std::string getProfileFileDlgKey() { return m_ProfileFileDlgKey; }
    std::string getGamePathFileDlgKey() { return m_GamePathFileDlgKey; }

    int m_LoadOrderInputInt;

    std::string m_ProfileFileDlgLabel;
    std::string m_GamePathFileDlgLabel;
    std::string m_SettingsLabel;

    std::string m_ProfileFileDlgKey;
    std::string m_GamePathFileDlgKey;

    bool m_HandleNumericalPaks;

    //----------------------------------
    // [SECTION] Manager implementation
    //----------------------------------
    void ShowHelpMarker(const char* desc) {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(450.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
};

#endif  // REMODMAN_UI_INCLUDED