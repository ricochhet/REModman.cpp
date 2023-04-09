#include "ManagerUI.h"

ManagerUI& ManagerUI::getInstance() {
    static ManagerUI instance;
    return instance;
}

void ManagerUI::setupVars() {
    setProfileFileDlgLabel("Create or load a modding profile");
    setGamePathFileDlgLabel("Find a game installation path");
    setSettingsLabel("Settings");

    setProfileFileDlgKey("##ProfileFileDlgKey");
    setGamePathFileDlgKey("##GamePathFileDlgKey");
}

void ManagerUI::drawProfileFileDlg() {
    if (ImGui::Button(m_ProfileFileDlgLabel.c_str(), ImVec2(-1, 0))) {
        ImGuiFileDialog::Instance()->OpenDialog(m_ProfileFileDlgKey, "Choose Profile Folder", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }

    ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

    if (ImGuiFileDialog::Instance()->Display(m_ProfileFileDlgKey, 32, size, size, ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)))) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            ManagerImpl::getInstance().setCurrentWorkingDirectory(filePathName);
            ManagerImpl::getInstance().doSetupChecks();
            ManagerImpl::getInstance().refreshModEntries();
            ManagerImpl::getInstance().patchConfig(2, false);

            if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty()) {
                setProfileFileDlgLabel(std::format("Profile Context - {}", Utils::truncateString(ManagerImpl::getInstance().getCurrentWorkingDirectory(), 256)));
                ManagerImpl::getInstance().setSelectedGamePath();

                if (!ManagerImpl::getInstance().getSelectedGamePath().empty()) {
                    setGamePathFileDlgLabel(std::format("Game Context - {}", Utils::truncateString(ManagerImpl::getInstance().getSelectedGamePath(), 256)));
                } else {
                    setGamePathFileDlgLabel("Find a game installation path");
                }
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void ManagerUI::drawGamePathFileDlg() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty()) {
        if (ImGui::Button(m_GamePathFileDlgLabel.c_str(), ImVec2(ImGui::GetWindowSize().x * 0.5f, 0))) {
            ImGuiFileDialog::Instance()->OpenDialog(m_GamePathFileDlgKey, "Choose Game Folder", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
        }

        ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

        if (ImGuiFileDialog::Instance()->Display(m_GamePathFileDlgKey, 32, size, size, ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)))) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                ManagerImpl::getInstance().setSelectedGamePath(filePathName);

                if (!ManagerImpl::getInstance().getSelectedGamePath().empty()) {
                    setGamePathFileDlgLabel(std::format("Game Context - {}", Utils::truncateString(ManagerImpl::getInstance().getSelectedGamePath(), 256)));
                } else {
                    setGamePathFileDlgLabel("Find a game installation path");
                }
            }

            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::SameLine();
        if (ImGui::Button(m_SettingsLabel.c_str(), ImVec2(ImGui::GetWindowSize().x * 0.5f, 0))) {
            ImGui::OpenPopup(m_SettingsLabel.c_str());
        }

        ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
        if (ImGui::BeginPopupModal(m_SettingsLabel.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::SetWindowPos(
                ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
            );
            m_HandlePakPatchingCheckBox = ManagerImpl::getInstance().getHandlePakPatching();
            if (ImGui::Checkbox("Handle Pak Patching", &m_HandlePakPatchingCheckBox)) {
                ManagerImpl::getInstance().setHandlePakPatching(m_HandlePakPatchingCheckBox);
            }
            ImGui::Separator();
            if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void ManagerUI::drawAvailableMods() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Available Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("AvailableMods");
        if (ImGui::BeginListBox("##AvailableModsKey", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::getInstance().getAvailableModEntries().size(); i++) {
                std::filesystem::path sourcePath = ManagerImpl::getInstance().getAvailableModEntries()[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                    ImGui::SetWindowPos(
                        ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
                    );

                    if (ImGui::Button("Add", ImVec2(-1, 0))) {
                        ManagerImpl::getInstance().doStageMod(ManagerImpl::getInstance().getAvailableModEntries()[i], m_LoadOrderInputInt);
                        ManagerImpl::getInstance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::InputInt("##LoadOrder", &m_LoadOrderInputInt);

                    if (std::filesystem::is_directory(sourcePath / "natives") && ManagerImpl::getInstance().getHandlePakPatching()) {
                        ImGui::Separator();

                        if (ImGui::Button("RisePakPatch", ImVec2(-1, 0))) {
                            std::filesystem::path outputPath = sourcePath.string() + " Pak Version/" + (sourcePath.filename().string() + ".pak");
                            Utils::createDirectory(outputPath.parent_path());
                            RisePakPatch::processDirectory(sourcePath.string(), outputPath.string());
                            ManagerImpl::getInstance().refreshModEntries();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }
}

void ManagerUI::drawStagedMods() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Staged Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Staged Mods");
        if (ImGui::BeginListBox("##StagedModsKey", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::getInstance().getStagedModEntries().size(); i++) {
                std::filesystem::path sourcePath = ManagerImpl::getInstance().getStagedModEntries()[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                    ImGui::SetWindowPos(
                        ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
                    );

                    if (ImGui::Button("Remove", ImVec2(-1, 0))) {
                        ManagerImpl::getInstance().doUnstageMod(ManagerImpl::getInstance().getStagedModEntries()[i]);
                        ManagerImpl::getInstance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }
}

void ManagerUI::drawDeployBtn() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (ImGui::Button("Deploy", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::getInstance().getInstalledModEntries().size(); i++) {
                if (ManagerImpl::getInstance().getHandlePakPatching() && ManagerImpl::getInstance().containsPakFiles(ManagerImpl::getInstance().getInstalledModEntries()[i])) {
                    ManagerImpl::getInstance().doUninstallPak(ManagerImpl::getInstance().getInstalledModEntries()[i]);
                } else {
                    ManagerImpl::getInstance().doUninstallMod(ManagerImpl::getInstance().getInstalledModEntries()[i]);
                }

                ManagerImpl::getInstance().doStageMod(ManagerImpl::getInstance().getInstalledModEntries()[i], i);
            }

            ManagerImpl::getInstance().refreshModEntries();

            for (int i = 0; i < ManagerImpl::getInstance().getStagedModEntries().size(); i++) {
                ManagerImpl::getInstance().doInstallMod(ManagerImpl::getInstance().getStagedModEntries()[i]);
                ManagerImpl::getInstance().doUnstageMod(ManagerImpl::getInstance().getStagedModEntries()[i]);
            }

            ManagerImpl::getInstance().refreshModEntries();
        }
    }
}

void ManagerUI::drawInstalledMods() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Installed Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Installed Mods");
        if (ImGui::BeginListBox("##InstalledModList", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::getInstance().getInstalledModEntries().size(); i++) {
                std::filesystem::path sourcePath = ManagerImpl::getInstance().getInstalledModEntries()[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                    ImGui::SetWindowPos(
                        ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
                    );

                    if (ImGui::Button("Uninstall", ImVec2(-1, 0))) {
                        if (ManagerImpl::getInstance().getHandlePakPatching() &&
                            ManagerImpl::getInstance().containsPakFiles(ManagerImpl::getInstance().getInstalledModEntries()[i])) {
                            ManagerImpl::getInstance().doUninstallPak(ManagerImpl::getInstance().getInstalledModEntries()[i]);
                        } else {
                            ManagerImpl::getInstance().doUninstallMod(ManagerImpl::getInstance().getInstalledModEntries()[i]);
                        }

                        ManagerImpl::getInstance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }
}

ManagerUI::ManagerUI() {}