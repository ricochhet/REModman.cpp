#include "manager_ui.h"

ManagerUI& ManagerUI::Instance() {
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
    ImGuiHelpers::PushButtonColor({0.81f, 0.56f, 0.28f, 1.0f});
    if (ImGui::Button(m_ProfileFileDlgLabel.c_str(), ImVec2(-1, 0))) {
        ImGuiFileDialog::Instance()->OpenDialog(m_ProfileFileDlgKey, "Choose Profile Folder", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }
    ImGuiHelpers::PopColors();

    ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

    if (ImGuiFileDialog::Instance()->Display(m_ProfileFileDlgKey, 32, size, size, ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)))) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            ManagerImpl::Instance().setCurrentWorkingDirectory(filePathName);
            ManagerImpl::Instance().doSetupChecks();
            ManagerImpl::Instance().refreshModEntries();
            ManagerImpl::Instance().patchConfig(2, false);

            if (!ManagerImpl::Instance().getCurrentWorkingDirectory().empty()) {
                setProfileFileDlgLabel(std::format("Profile Context - {}", Utils::truncateString(ManagerImpl::Instance().getCurrentWorkingDirectory(), 256)));
                ManagerImpl::Instance().setSelectedGamePath();
                ManagerImpl::Instance().setHandleNumericalPaks();

                if (!ManagerImpl::Instance().getSelectedGamePath().empty()) {
                    setGamePathFileDlgLabel(std::format("Game Context - {}", Utils::truncateString(ManagerImpl::Instance().getSelectedGamePath(), 256)));
                } else {
                    setGamePathFileDlgLabel("Find a game installation path");
                }
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void ManagerUI::drawGamePathFileDlg() {
    if (!ManagerImpl::Instance().getCurrentWorkingDirectory().empty()) {
        ImGuiHelpers::PushButtonColor({0.81f, 0.56f, 0.28f, 1.0f});
        if (ImGui::Button(m_GamePathFileDlgLabel.c_str(), ImVec2(ImGui::GetWindowSize().x * 0.5f, 0))) {
            ImGuiFileDialog::Instance()->OpenDialog(m_GamePathFileDlgKey, "Choose Game Folder", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
        }
        ImGuiHelpers::PopColors();

        ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

        if (ImGuiFileDialog::Instance()->Display(m_GamePathFileDlgKey, 32, size, size, ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)))) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                ManagerImpl::Instance().setSelectedGamePath(filePathName);

                if (!ManagerImpl::Instance().getSelectedGamePath().empty()) {
                    setGamePathFileDlgLabel(std::format("Game Context - {}", Utils::truncateString(ManagerImpl::Instance().getSelectedGamePath(), 256)));
                } else {
                    setGamePathFileDlgLabel("Find a game installation path");
                }
            }

            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::SameLine();
        ImGuiHelpers::PushButtonColor({0.81f, 0.56f, 0.28f, 1.0f});
        if (ImGui::Button(m_SettingsLabel.c_str(), ImVec2(ImGui::GetWindowSize().x * 0.5f, 0))) {
            ImGui::OpenPopup(m_SettingsLabel.c_str());
        }
        ImGuiHelpers::PopColors();

        ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
        if (ImGui::BeginPopupModal(m_SettingsLabel.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::SetWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)));
            m_HandleNumericalPaks = ManagerImpl::Instance().getHandleNumericalPaks();
            if (ImGui::Checkbox("Handle Numerical Paks", &m_HandleNumericalPaks)) {
                ManagerImpl::Instance().setHandleNumericalPaks(m_HandleNumericalPaks);
            }
            ImGui::SameLine();
            ShowHelpMarker("Mods that follow the 're_chunk_000.pak.patch_XYZ' format, will automatically be numerically incremented.");
            ImGui::Separator();
            ImGuiHelpers::PushButtonColor({0.66f, 0.66f, 0.66f, 1.0f});
            if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGuiHelpers::PopColors();
            ImGui::EndPopup();
        }
    }
}

void ManagerUI::drawAvailableMods() {
    if (!ManagerImpl::Instance().getCurrentWorkingDirectory().empty() && !ManagerImpl::Instance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Available Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("AvailableMods");
        if (ImGui::BeginListBox("##AvailableModsKey", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::Instance().getAvailableModEntries().size(); i++) {
                std::filesystem::path sourcePath = ManagerImpl::Instance().getAvailableModEntries()[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                    ImGui::SetWindowPos(
                        ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
                    );

                    ImGuiHelpers::PushButtonColor({0.54f, 0.73f, 0.32f, 1.0f});
                    if (ImGui::Button("Add", ImVec2(-1, 0))) {
                        ManagerImpl::Instance().doStageMod(ManagerImpl::Instance().getAvailableModEntries()[i], m_LoadOrderInputInt);
                        ManagerImpl::Instance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGuiHelpers::PopColors();

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::InputInt("##LoadOrder", &m_LoadOrderInputInt);

                    if (std::filesystem::is_directory(sourcePath / "natives") && ManagerImpl::Instance().getHandleNumericalPaks()) {
                        ImGui::Separator();

                        if (ImGui::Button("RisePakPatch")) {
                            std::filesystem::path outputPath = sourcePath.string() + " Pak Version/" + (sourcePath.filename().string() + ".pak");
                            Utils::createDirectory(outputPath.parent_path());
                            RisePakPatch::processDirectory(sourcePath.string(), outputPath.string());
                            ManagerImpl::Instance().refreshModEntries();
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::SameLine();
                        ShowHelpMarker("Compile the 'natives' folder of a mod into a pak file.");
                    }
                    ImGui::Separator();
                    ImGuiHelpers::PushButtonColor({0.66f, 0.66f, 0.66f, 1.0f});
                    if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGuiHelpers::PopColors();
                    ImGui::EndPopup();
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }
}

void ManagerUI::drawStagedMods() {
    if (!ManagerImpl::Instance().getCurrentWorkingDirectory().empty() && !ManagerImpl::Instance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Staged Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Staged Mods");
        if (ImGui::BeginListBox("##StagedModsKey", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::Instance().getStagedModEntries().size(); i++) {
                std::filesystem::path sourcePath = ManagerImpl::Instance().getStagedModEntries()[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                    ImGui::SetWindowPos(
                        ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
                    );

                    ImGuiHelpers::PushButtonColor({0.81f, 0.56f, 0.28f, 1.0f});
                    if (ImGui::Button("Remove", ImVec2(-1, 0))) {
                        ManagerImpl::Instance().doUnstageMod(ManagerImpl::Instance().getStagedModEntries()[i]);
                        ManagerImpl::Instance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGuiHelpers::PopColors();
                    ImGui::Separator();
                    ImGuiHelpers::PushButtonColor({0.66f, 0.66f, 0.66f, 1.0f});
                    if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGuiHelpers::PopColors();
                    ImGui::EndPopup();
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }
}

void ManagerUI::drawDeployBtn() {
    if (!ManagerImpl::Instance().getCurrentWorkingDirectory().empty() && !ManagerImpl::Instance().getSelectedGamePath().empty()) {
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.15f, 0.68f, 0.37f, 1.0f});

        ImGuiHelpers::PushButtonColor({0.81f, 0.56f, 0.28f, 1.0f});
        if (ImGui::Button("Deploy", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::Instance().getInstalledModEntries().size(); i++) {
                if (ManagerImpl::Instance().getHandleNumericalPaks() && ManagerImpl::Instance().containsPakFiles(ManagerImpl::Instance().getInstalledModEntries()[i])) {
                    ManagerImpl::Instance().doUninstallPak(ManagerImpl::Instance().getInstalledModEntries()[i]);
                } else {
                    ManagerImpl::Instance().doUninstallMod(ManagerImpl::Instance().getInstalledModEntries()[i]);
                }

                ManagerImpl::Instance().doStageMod(ManagerImpl::Instance().getInstalledModEntries()[i], i);
            }

            ManagerImpl::Instance().refreshModEntries();

            for (int i = 0; i < ManagerImpl::Instance().getStagedModEntries().size(); i++) {
                ManagerImpl::Instance().doInstallMod(ManagerImpl::Instance().getStagedModEntries()[i]);
                ManagerImpl::Instance().doUnstageMod(ManagerImpl::Instance().getStagedModEntries()[i]);
            }

            ManagerImpl::Instance().refreshModEntries();
        }
        ImGuiHelpers::PopColors();
    }
}

void ManagerUI::drawInstalledMods() {
    if (!ManagerImpl::Instance().getCurrentWorkingDirectory().empty() && !ManagerImpl::Instance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Installed Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Installed Mods");
        if (ImGui::BeginListBox("##InstalledModList", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::Instance().getInstalledModEntries().size(); i++) {
                std::filesystem::path sourcePath = ManagerImpl::Instance().getInstalledModEntries()[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
                    ImGui::SetWindowPos(
                        ImVec2((ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2), (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2))
                    );

                    ImGuiHelpers::PushButtonColor({0.3f, 0.76f, 0.96f, 1.0f});
                    if (ImGui::Button("Uninstall", ImVec2(-1, 0))) {
                        if (ManagerImpl::Instance().getHandleNumericalPaks() && ManagerImpl::Instance().containsPakFiles(ManagerImpl::Instance().getInstalledModEntries()[i])) {
                            ManagerImpl::Instance().doUninstallPak(ManagerImpl::Instance().getInstalledModEntries()[i]);
                        } else {
                            ManagerImpl::Instance().doUninstallMod(ManagerImpl::Instance().getInstalledModEntries()[i]);
                        }

                        ManagerImpl::Instance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGuiHelpers::PopColors();
                    ImGui::Separator();
                    ImGuiHelpers::PushButtonColor({0.66f, 0.66f, 0.66f, 1.0f});
                    if (ImGui::Button("Exit", ImVec2(-1, 0))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGuiHelpers::PopColors();
                    ImGui::EndPopup();
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }
}

ManagerUI::ManagerUI() {}