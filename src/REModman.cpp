#include <REModman.h>

static int               game_selection_index = 0;
std::string              selected_profile_path;
std::string              selected_game_path;
std::vector<std::string> staged_mod_entries;
std::vector<std::string> available_mod_entries;
std::vector<std::string> installed_mod_entries;

std::vector<std::string> GameSelection = {REMM_GAME_NONE, REMM_GAME_MONSTER_HUNTER_RISE};

std::string loadProfileDlgBtnLabel = "Create / Load Modding Profile";
std::string getGameDlgBtnLabel     = "Find Game Location For ";

void FileDialog::draw_load_profile_dialog() {
    if (ImGui::Button(loadProfileDlgBtnLabel.c_str(), ImVec2(-1, 0))) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "LoadProfileDlgKey", "Choose Profile Folder", nullptr, "", 1, nullptr,
            ImGuiFileDialogFlags_Modal
        );
    }

    ImVec2 size =
        ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center =
        ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)));
    if (ImGuiFileDialog::Instance()->Display("LoadProfileDlgKey", 32)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            ModManagerConfig::set_selected_profile(filePathName);
            ModManagerPatches::MonsterHunterRise::set_patch_in_profile(filePathName, 2);

            selected_profile_path = filePathName;
            ModManager::initialize(selected_profile_path);
            REModman::reload_mod_entries();

            if (!selected_profile_path.empty()) {
                game_selection_index =
                    ModManagerConfig::get_last_selected_game(selected_profile_path);
                selected_game_path = ModManagerConfig::get_game_path(
                    selected_profile_path, GameSelection[game_selection_index]
                );
                loadProfileDlgBtnLabel =
                    "(Profile) " + Utils::truncate_string(selected_profile_path, 256);
                getGameDlgBtnLabel = getGameDlgBtnLabel + GameSelection[game_selection_index];

                if (!selected_game_path.empty()) {
                    getGameDlgBtnLabel =
                        "(Game Location) " + Utils::truncate_string(selected_game_path, 256);
                }
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void FileDialog::draw_get_game_path_dialog() {
    if (ImGui::Button(getGameDlgBtnLabel.c_str(), ImVec2(-1, 0))) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "GetGameInstallDlgKey", "Choose Game Location", nullptr, "", 1, nullptr,
            ImGuiFileDialogFlags_Modal
        );
    }

    ImVec2 size =
        ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center =
        ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)));
    if (ImGuiFileDialog::Instance()->Display("GetGameInstallDlgKey", 32)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            ModManagerConfig::set_game_path(
                selected_profile_path, GameSelection[game_selection_index], filePathName
            );
            selected_game_path = filePathName;

            if (!selected_game_path.empty()) {
                getGameDlgBtnLabel =
                    "(Game Location) " + Utils::truncate_string(selected_game_path, 256);
                selected_game_path = ModManagerConfig::get_game_path(
                    selected_profile_path, GameSelection[game_selection_index]
                );
            }

            Logger::getInstance().log(
                "Found existing game path: " + selected_game_path, LogLevel::Info
            );
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void REModman::draw_load_profile() { FileDialog::draw_load_profile_dialog(); }

void REModman::draw_get_game_path() {
    if (!selected_profile_path.empty()) {
        FileDialog::draw_get_game_path_dialog();
    }
}

void REModman::draw_game_selector() {
    if (!selected_profile_path.empty()) {
        ImGui::PushItemWidth(-1);
        ImGui::SetNextWindowSize(ImVec2(-1, 0));
        if (ImGui::BeginCombo("##GameSelector", GameSelection[game_selection_index].c_str())) {
            for (int i = 0; i < GameSelection.size(); i++) {
                bool isSelected = (game_selection_index == i);
                if (ImGui::Selectable(GameSelection[i].c_str(), isSelected)) {
                    game_selection_index = i;
                    ModManagerConfig::set_last_selected_game(selected_profile_path, i);

                    if (ModManagerConfig::get_game_path(
                            selected_profile_path, GameSelection[game_selection_index]
                        )
                            .empty()) {
                        selected_game_path = std::string();
                        getGameDlgBtnLabel =
                            "Find Game Location For " + GameSelection[game_selection_index];
                    } else {
                        selected_game_path = ModManagerConfig::get_game_path(
                            selected_profile_path, GameSelection[game_selection_index]
                        );
                        getGameDlgBtnLabel =
                            "(Game Location) " + Utils::truncate_string(selected_game_path, 256);
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

int  load_order = 0;
void REModman::draw_mod_list() {
    if (!selected_profile_path.empty() && !selected_game_path.empty()) {
        if (!ImGui::CollapsingHeader("Available Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("AvailableMods");
        if (ImGui::BeginListBox("##AvailableModsList", ImVec2(-1, 0))) {
            for (int i = 0; i < available_mod_entries.size(); i++) {
                std::filesystem::path sourcePath = available_mod_entries[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(
                        label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
                    )) {
                    ImGui::SetWindowPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2),
                        (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)
                    ));

                    if (ImGui::Button("Add", ImVec2(-1, 0))) {
                        ModManager::stage_mod(
                            selected_profile_path, available_mod_entries[i], selected_game_path,
                            GameSelection[game_selection_index], load_order
                        );
                        reload_mod_entries();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::InputInt("##LoadOrder", &load_order);
                    if (std::filesystem::is_directory(sourcePath / "natives") &&
                        GameSelection[game_selection_index] == REMM_GAME_MONSTER_HUNTER_RISE) {
                        ImGui::Separator();
                        if (ImGui::Button("RisePakPatch", ImVec2(-1, 0))) {
                            std::filesystem::path outputPath =
                                sourcePath.string() + " Pak Version/" +
                                (sourcePath.filename().string() + ".pak");
                            Utils::create_directory(outputPath.parent_path());
                            RisePakPatch::ProcessDirectory(
                                sourcePath.string(), sourcePath.string() + " Pak Version/" +
                                                         (sourcePath.filename().string() + ".pak")
                            );
                            reload_mod_entries();
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
    if (!selected_profile_path.empty() && !selected_game_path.empty()) {
        if (!ImGui::CollapsingHeader("Staged Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Staged Mods");
        if (ImGui::BeginListBox("##StagedModsList", ImVec2(-1, 0))) {
            for (int i = 0; i < staged_mod_entries.size(); i++) {
                std::filesystem::path sourcePath = staged_mod_entries[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(
                        label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
                    )) {
                    ImGui::SetWindowPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2),
                        (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)
                    ));

                    if (ImGui::Button("Remove", ImVec2(-1, 0))) {
                        ModManager::destage_mod(selected_profile_path, staged_mod_entries[i]);
                        reload_mod_entries();
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
    if (!selected_profile_path.empty() && !selected_game_path.empty()) {
        if (ImGui::Button("Deploy", ImVec2(-1, 0))) {
            for (int i = 0; i < installed_mod_entries.size(); i++) {
                if (GameSelection[game_selection_index] == REMM_GAME_MONSTER_HUNTER_RISE &&
                    ModManagerPatches::MonsterHunterRise::contains_pak_files(
                        installed_mod_entries[i]
                    )) {
                    ModManager::uninstall_pak_mod(
                        selected_profile_path, installed_mod_entries[i], selected_game_path,
                        GameSelection[game_selection_index]
                    );
                } else {
                    ModManager::uninstall_mod(selected_profile_path, installed_mod_entries[i]);
                }

                ModManager::stage_mod(
                    selected_profile_path, installed_mod_entries[i], selected_game_path,
                    GameSelection[game_selection_index], i
                );
            }

            reload_mod_entries();

            for (int i = 0; i < staged_mod_entries.size(); i++) {
                ModManager::install_mod(
                    selected_profile_path, staged_mod_entries[i], selected_game_path,
                    GameSelection[game_selection_index]
                );
                ModManager::destage_mod(selected_profile_path, staged_mod_entries[i]);
            }
        }

        reload_mod_entries();
    }
}

void REModman::draw_installed_mod_list() {
    if (!selected_profile_path.empty() && !selected_game_path.empty()) {
        if (!ImGui::CollapsingHeader("Installed Mods", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::TreePush("Installed Mods");
        if (ImGui::BeginListBox("##InstalledModList", ImVec2(-1, 0))) {
            for (int i = 0; i < installed_mod_entries.size(); i++) {
                std::filesystem::path sourcePath = installed_mod_entries[i];
                std::string           label      = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str())) {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(
                        label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
                    )) {
                    ImGui::SetWindowPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2),
                        (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)
                    ));

                    if (ImGui::Button("Uninstall", ImVec2(-1, 0))) {
                        if (GameSelection[game_selection_index] == REMM_GAME_MONSTER_HUNTER_RISE &&
                            ModManagerPatches::MonsterHunterRise::contains_pak_files(
                                installed_mod_entries[i]
                            )) {
                            ModManager::uninstall_pak_mod(
                                selected_profile_path, installed_mod_entries[i], selected_game_path,
                                GameSelection[game_selection_index]
                            );
                        } else {
                            ModManager::uninstall_mod(
                                selected_profile_path, installed_mod_entries[i]
                            );
                        }

                        reload_mod_entries();
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

void REModman::reload_mod_entries() {
    staged_mod_entries =
        ModManager::get_mod_entries(selected_profile_path, REMM_MODS_STAGING_FILE_NAME, false);
    available_mod_entries =
        ModManager::get_mod_entries(selected_profile_path, REMM_MODS_INSTALLED_FILE_NAME, true);
    installed_mod_entries =
        ModManager::get_mod_entries(selected_profile_path, REMM_MODS_INSTALLED_FILE_NAME, false);
}