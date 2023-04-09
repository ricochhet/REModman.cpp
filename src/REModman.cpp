#include <REModman.h>

std::vector<std::string> GameSelection = {"None", "MonsterHunterRise"};

std::string loadProfileDlgBtnLabel = "Create / Load Modding Profile";
std::string getGameDlgBtnLabel     = "Find Game Location For ";

void FileDialog::drawLoadProfileDlg() {
    if (ImGui::Button(loadProfileDlgBtnLabel.c_str(), ImVec2(-1, 0))) {
        ImGuiFileDialog::Instance()->OpenDialog("LoadProfileDlgKey", "Choose Profile Folder", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }

    ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)));
    if (ImGuiFileDialog::Instance()->Display("LoadProfileDlgKey", 32)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            ManagerImpl::getInstance().setCurrentWorkingDirectory(filePathName);
            ManagerImpl::getInstance().patchConfig(2);
            ManagerImpl::getInstance().doSetupChecks();
            ManagerImpl::getInstance().refreshModEntries();

            if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty()) {
                ManagerImpl::getInstance().setSelectedGameIndex();
                ManagerImpl::getInstance().setSelectedGamePath("", GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()]);
                loadProfileDlgBtnLabel = std::format("Profile Context - {}", Utils::truncateString(ManagerImpl::getInstance().getCurrentWorkingDirectory(), 256));

                if (!ManagerImpl::getInstance().getSelectedGamePath().empty()) {
                    getGameDlgBtnLabel = std::format("Game Context ({}) - {}", GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()], Utils::truncateString(ManagerImpl::getInstance().getSelectedGamePath(), 256));
                }
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void FileDialog::drawGetGamePathDlg() {
    if (ImGui::Button(getGameDlgBtnLabel.c_str(), ImVec2(-1, 0))) {
        ImGuiFileDialog::Instance()->OpenDialog("GetGameInstallDlgKey", "Choose Game Location", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }

    ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)));
    if (ImGuiFileDialog::Instance()->Display("GetGameInstallDlgKey", 32)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            ManagerImpl::getInstance().setSelectedGamePath(filePathName, GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()]);

            if (!ManagerImpl::getInstance().getSelectedGamePath().empty()) {
                getGameDlgBtnLabel = std::format("Game Context ({}) - {}", GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()], Utils::truncateString(ManagerImpl::getInstance().getSelectedGamePath(), 256));
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void REModman::drawLoadProfile() { FileDialog::drawLoadProfileDlg(); }

void REModman::drawGetGamePath() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty()) {
        FileDialog::drawGetGamePathDlg();
    }
}

void REModman::drawGameSelector() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty()) {
        ImGui::PushItemWidth(-1);
        ImGui::SetNextWindowSize(ImVec2(-1, 0));
        if (ImGui::BeginCombo("##GameSelector", GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()].c_str())) {
            for (int i = 0; i < GameSelection.size(); i++) {
                bool isSelected = (ManagerImpl::getInstance().getSelectedGameIndex() == i);
                if (ImGui::Selectable(GameSelection[i].c_str(), isSelected)) {
                    ManagerImpl::getInstance().setSelectedGameIndex(i);
                    ManagerImpl::getInstance().setSelectedGamePath("", GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()]);

                    if (ManagerImpl::getInstance().getSelectedGamePath().empty()) {
                        getGameDlgBtnLabel = "Find Game Location For " + GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()];
                    } else {
                        ManagerImpl::getInstance().setSelectedGamePath(
                            ManagerImpl::getInstance().getSelectedGamePath(), GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()]
                        );
                        getGameDlgBtnLabel = std::format("Game Context ({}) - {}", GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()], Utils::truncateString(ManagerImpl::getInstance().getSelectedGamePath(), 256));
                    }
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }
}

int  loadOrder = 0;
void REModman::drawAvailableModList() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Available Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("AvailableMods");
        if (ImGui::BeginListBox("##AvailableModsList", ImVec2(-1, 0))) {
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
                        ManagerImpl::getInstance().doStageMod(ManagerImpl::getInstance().getAvailableModEntries()[i], loadOrder);
                        ManagerImpl::getInstance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::InputInt("##LoadOrder", &loadOrder);
                    if (std::filesystem::is_directory(sourcePath / "natives") && GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()] == "MonsterHunterRise") {
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

void REModman::drawStagedModList() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Staged Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Staged Mods");
        if (ImGui::BeginListBox("##StagedModsList", ImVec2(-1, 0))) {
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

void REModman::drawDeployButton() {
    if (!ManagerImpl::getInstance().getCurrentWorkingDirectory().empty() && !ManagerImpl::getInstance().getSelectedGamePath().empty()) {
        if (ImGui::Button("Deploy", ImVec2(-1, 0))) {
            for (int i = 0; i < ManagerImpl::getInstance().getInstalledModEntries().size(); i++) {
                if (GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()] == "MonsterHunterRise" &&
                    ManagerImpl::getInstance().containsPakFiles(ManagerImpl::getInstance().getInstalledModEntries()[i])) {
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

void REModman::drawInstalledModList() {
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
                        if (GameSelection[ManagerImpl::getInstance().getSelectedGameIndex()] == "MonsterHunterRise" &&
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