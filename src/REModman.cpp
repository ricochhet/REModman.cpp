#include <REModman.h>

std::vector<std::string> GameSelection = {"None", "MonsterHunterRise"};

std::string loadProfileDlgBtnLabel = "Create / Load Modding Profile";
std::string getGameDlgBtnLabel     = "Find Game Location For ";

void FileDialog::draw_load_profile_dialog() {
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
            Manager::getInstance().setCurrentWorkingDirectory(filePathName);
            Manager::getInstance().patchConfig(2);
            Manager::getInstance().doSetupChecks();

            if (!Manager::getInstance().getCurrentWorkingDirectory().empty()) {
                loadProfileDlgBtnLabel = "(Profile) " + Utils::truncate_string(Manager::getInstance().getCurrentWorkingDirectory(), 256);
                getGameDlgBtnLabel     = getGameDlgBtnLabel + GameSelection[Manager::getInstance().getSelectedGameIndex()];

                if (!Manager::getInstance().getSelectedGamePath().empty()) {
                    getGameDlgBtnLabel = "(Game Location) " + Utils::truncate_string(Manager::getInstance().getSelectedGamePath(), 256);
                }
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void FileDialog::draw_get_game_path_dialog() {
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
            Manager::getInstance().setSelectedGamePath(filePathName);

            if (!Manager::getInstance().getSelectedGamePath().empty()) {
                loadProfileDlgBtnLabel = "(Game Location) " + Utils::truncate_string(Manager::getInstance().getSelectedGamePath(), 256);
                getGameDlgBtnLabel     = getGameDlgBtnLabel + GameSelection[Manager::getInstance().getSelectedGameIndex()];
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void REModman::draw_load_profile() { FileDialog::draw_load_profile_dialog(); }

void REModman::draw_get_game_path() {
    if (!Manager::getInstance().getCurrentWorkingDirectory().empty()) {
        FileDialog::draw_get_game_path_dialog();
    }
}

void REModman::draw_game_selector() {
    if (!Manager::getInstance().getCurrentWorkingDirectory().empty()) {
        ImGui::PushItemWidth(-1);
        ImGui::SetNextWindowSize(ImVec2(-1, 0));
        if (ImGui::BeginCombo("##GameSelector", GameSelection[Manager::getInstance().getSelectedGameIndex()].c_str())) {
            for (int i = 0; i < GameSelection.size(); i++) {
                bool isSelected = (Manager::getInstance().getSelectedGameIndex() == i);
                if (ImGui::Selectable(GameSelection[i].c_str(), isSelected)) {
                    Manager::getInstance().setSelectedGameIndex(i);

                    if (Manager::getInstance().getSelectedGamePath().empty()) {
                        Manager::getInstance().setSelectedGamePath(std::string());
                        getGameDlgBtnLabel = "Find Game Location For " + GameSelection[Manager::getInstance().getSelectedGameIndex()];
                    } else {
                        Manager::getInstance().setSelectedGamePath(GameSelection[Manager::getInstance().getSelectedGameIndex()]);
                        getGameDlgBtnLabel = "(Game Location) " + Utils::truncate_string(Manager::getInstance().getSelectedGamePath(), 256);
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
void REModman::draw_mod_list() {
    if (!Manager::getInstance().getCurrentWorkingDirectory().empty() && !Manager::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Available Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("AvailableMods");
        if (ImGui::BeginListBox("##AvailableModsList", ImVec2(-1, 0))) {
            for (int i = 0; i < Manager::getInstance().getAvailableModEntries().size(); i++) {
                std::filesystem::path sourcePath = Manager::getInstance().getAvailableModEntries()[i];
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
                        Manager::getInstance().doStageMod(Manager::getInstance().getAvailableModEntries()[i], loadOrder);
                        Manager::getInstance().refreshModEntries();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::InputInt("##LoadOrder", &loadOrder);
                    if (std::filesystem::is_directory(sourcePath / "natives") && GameSelection[Manager::getInstance().getSelectedGameIndex()] == "MonsterHunterRise") {
                        ImGui::Separator();
                        if (ImGui::Button("RisePakPatch", ImVec2(-1, 0))) {
                            std::filesystem::path outputPath = sourcePath.string() + " Pak Version/" + (sourcePath.filename().string() + ".pak");
                            Utils::create_directory(outputPath.parent_path());
                            RisePakPatch::ProcessDirectory(sourcePath.string(), sourcePath.string() + " Pak Version/" + (sourcePath.filename().string() + ".pak"));
                            Manager::getInstance().refreshModEntries();
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

void REModman::draw_staging_mod_list() {
    if (!Manager::getInstance().getCurrentWorkingDirectory().empty() && !Manager::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Staged Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Staged Mods");
        if (ImGui::BeginListBox("##StagedModsList", ImVec2(-1, 0))) {
            for (int i = 0; i < Manager::getInstance().getStagedModEntries().size(); i++) {
                std::filesystem::path sourcePath = Manager::getInstance().getStagedModEntries()[i];
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
                        Manager::getInstance().doUnstageMod(Manager::getInstance().getStagedModEntries()[i]);
                        Manager::getInstance().refreshModEntries();
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

void REModman::draw_mod_deploy_button() {
    if (!Manager::getInstance().getCurrentWorkingDirectory().empty() && !Manager::getInstance().getSelectedGamePath().empty()) {
        if (ImGui::Button("Deploy", ImVec2(-1, 0))) {
            for (int i = 0; i < Manager::getInstance().getInstalledModEntries().size(); i++) {
                if (GameSelection[Manager::getInstance().getSelectedGameIndex()] == "MonsterHunterRise" &&
                    Manager::getInstance().containsPakFiles(Manager::getInstance().getInstalledModEntries()[i])) {
                    Manager::getInstance().doUninstallPak(Manager::getInstance().getInstalledModEntries()[i]);
                } else {
                    Manager::getInstance().doUninstallMod(Manager::getInstance().getInstalledModEntries()[i]);
                }

                Manager::getInstance().doStageMod(Manager::getInstance().getInstalledModEntries()[i], i);
            }

            Manager::getInstance().refreshModEntries();

            for (int i = 0; i < Manager::getInstance().getStagedModEntries().size(); i++) {
                Manager::getInstance().doInstallMod(Manager::getInstance().getStagedModEntries()[i]);
                Manager::getInstance().doUnstageMod(Manager::getInstance().getStagedModEntries()[i]);
            }

            Manager::getInstance().refreshModEntries();
        }
    }
}

void REModman::draw_installed_mod_list() {
    if (!Manager::getInstance().getCurrentWorkingDirectory().empty() && !Manager::getInstance().getSelectedGamePath().empty()) {
        if (!ImGui::CollapsingHeader("Installed Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Installed Mods");
        if (ImGui::BeginListBox("##InstalledModList", ImVec2(-1, 0))) {
            for (int i = 0; i < Manager::getInstance().getInstalledModEntries().size(); i++) {
                std::filesystem::path sourcePath = Manager::getInstance().getInstalledModEntries()[i];
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
                        if (GameSelection[Manager::getInstance().getSelectedGameIndex()] == "MonsterhunterRise" &&
                            Manager::getInstance().containsPakFiles(Manager::getInstance().getInstalledModEntries()[i])) {
                            Manager::getInstance().doUninstallPak(Manager::getInstance().getInstalledModEntries()[i]);
                        } else {
                            Manager::getInstance().doUninstallMod(Manager::getInstance().getInstalledModEntries()[i]);
                        }

                        Manager::getInstance().refreshModEntries();
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