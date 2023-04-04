#include <REModman.h>

static int game_selection_index = 0;
std::string selected_profile_path;
std::string selected_game_path;
std::vector<nlohmann::json> staged_mod_entries;
std::vector<nlohmann::json> available_mod_entries;
std::vector<nlohmann::json> installed_mod_entries;

std::vector<std::string> GameSelection = {
    "None",
    "MonsterHunterRise",
    "MonsterHunterWorld"};

std::string loadProfileDlgBtnLabel = "Create / Load Modding Profile";
std::string getGameDlgBtnLabel = "Find Game Location For ";

void FileDialog::draw_load_profile_dialog()
{
    if (ImGui::Button(loadProfileDlgBtnLabel.c_str(), ImVec2(-1, 0)))
    {
        ImGuiFileDialog::Instance()->OpenDialog("LoadProfileDlgKey", "Choose Profile Folder", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }

    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)));
    if (ImGuiFileDialog::Instance()->Display("LoadProfileDlgKey", 32))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            JsonUtils::create_or_update_json(filePathName + "/" + "profile.json", "SelectedProfile", filePathName, true);
            JsonUtils::create_or_update_json(filePathName + "/" + "profile.json", "PatchReEnginePakIndex", 2, false);
            selected_profile_path = filePathName;

            ModManager::startup_health_checks(selected_profile_path);
            staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
            available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
            installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);

            if (!selected_profile_path.empty())
            {
                game_selection_index = ModManager::get_last_selected_game(selected_profile_path);
                selected_game_path = ModManager::get_game_path(selected_profile_path, GameSelection[game_selection_index]);
                loadProfileDlgBtnLabel = "(Profile) " + Utils::truncate_string(selected_profile_path, 256);
                getGameDlgBtnLabel = getGameDlgBtnLabel + GameSelection[game_selection_index];

                if (!selected_game_path.empty())
                {
                    getGameDlgBtnLabel = "(Game Location) " + Utils::truncate_string(selected_game_path, 256);
                }
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void FileDialog::draw_get_game_path_dialog()
{
    if (ImGui::Button(getGameDlgBtnLabel.c_str(), ImVec2(-1, 0)))
    {
        ImGuiFileDialog::Instance()->OpenDialog("GetGameInstallDlgKey", "Choose Game Location", nullptr, "", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }

    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.95f);
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(center.x - (size.x * 0.5f), center.y - (size.y * 0.5f)));
    if (ImGuiFileDialog::Instance()->Display("GetGameInstallDlgKey", 32))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            JsonUtils::create_or_update_json(selected_profile_path + "/" + "profile.json", GameSelection[game_selection_index] + "GamePath", filePathName, true);
            selected_game_path = filePathName;

            Logger::getInstance().log("Found existing game path: " + selected_game_path, LogLevel::Info);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void REModman::draw_load_profile()
{
    FileDialog::draw_load_profile_dialog();
}

void REModman::draw_get_game_path()
{
    if (!selected_profile_path.empty())
    {
        FileDialog::draw_get_game_path_dialog();
    }
}

void REModman::draw_game_selector()
{
    if (!selected_profile_path.empty())
    {
        ImGui::PushItemWidth(-1);
        ImGui::SetNextWindowSize(ImVec2(-1, 0));
        if (ImGui::BeginCombo("##GameSelector", GameSelection[game_selection_index].c_str()))
        {
            for (int i = 0; i < GameSelection.size(); i++)
            {
                bool isSelected = (game_selection_index == i);
                if (ImGui::Selectable(GameSelection[i].c_str(), isSelected))
                {
                    game_selection_index = i;
                    JsonUtils::create_or_update_json(selected_profile_path + "/" + "profile.json", "LastSelectedGame", i, true);
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }
}

int load_order = 0;
void REModman::draw_mod_list()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Available Mods", ImGuiTreeNodeFlags_DefaultOpen))
        {
            return;
        }

        ImGui::TreePush("AvailableMods");
        if (ImGui::BeginListBox("##AvailableModsList", ImVec2(-1, 0)))
        {
            for (int i = 0; i < available_mod_entries.size(); i++)
            {
                std::filesystem::path sourcePath = available_mod_entries[i]["SourcePath"];
                std::string label = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str()))
                {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
                {
                    ImGui::SetWindowPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2),
                        (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)));

                    if (ImGui::Button("Add", ImVec2(-1, 0)))
                    {
                        ModManager::stage_mod(selected_profile_path, available_mod_entries[i]["SourcePath"], selected_game_path, load_order);
                        staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                        available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                        installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::InputInt("##LoadOrder", &load_order);
                    if (std::filesystem::is_directory(sourcePath / "natives") && GameSelection[game_selection_index] == "MonsterHunterRise")
                    {
                        ImGui::Separator();
                        if (ImGui::Button("RisePakPatch", ImVec2(-1, 0)))
                        {
                            std::filesystem::path outputPath = sourcePath.string() + " Pak Version/" + (sourcePath.filename().string() + ".pak");
                            Utils::create_directory(outputPath.parent_path());
                            RisePakPatch::ProcessDirectory(sourcePath.string(), sourcePath.string() + " Pak Version/" + (sourcePath.filename().string() + ".pak"));
                            staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                            available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                            installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);

                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Exit", ImVec2(-1, 0)))
                    {
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

void REModman::draw_staging_mod_list()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Staged Mods", ImGuiTreeNodeFlags_DefaultOpen))
        {
            return;
        }

        ImGui::TreePush("Staged Mods");
        if (ImGui::BeginListBox("##StagedModsList", ImVec2(-1, 0)))
        {
            for (int i = 0; i < staged_mod_entries.size(); i++)
            {
                std::filesystem::path sourcePath = staged_mod_entries[i]["SourcePath"];
                std::string label = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str()))
                {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
                {
                    ImGui::SetWindowPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2),
                        (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)));

                    if (ImGui::Button("Remove", ImVec2(-1, 0)))
                    {
                        ModManager::destage_mod(selected_profile_path, staged_mod_entries[i]["SourcePath"]);
                        staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                        available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                        installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);

                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Exit", ImVec2(-1, 0)))
                    {
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

void REModman::draw_mod_deploy_button()
{
    if (!selected_profile_path.empty())
    {
        if (ImGui::Button("Deploy", ImVec2(-1, 0)))
        {
            for (int i = 0; i < installed_mod_entries.size(); i++)
            {
                if (GameSelection[game_selection_index] == "MonsterHunterRise" && ModManager::contains_pak_files(installed_mod_entries[i]["SourcePath"]))
                {
                    ModManager::uninstall_pak_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"], selected_game_path);
                }
                else
                {
                    ModManager::uninstall_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"]);
                }
            }

            for (int i = 0; i < staged_mod_entries.size(); i++)
            {
                ModManager::install_mod(selected_profile_path, staged_mod_entries[i]["SourcePath"], selected_game_path);
                ModManager::destage_mod(selected_profile_path, staged_mod_entries[i]["SourcePath"]);
            }

            staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
            available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
            installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
        }
    }
}

void REModman::draw_installed_mod_list()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Installed Mods", ImGuiTreeNodeFlags_DefaultOpen))
        {
            return;
        }

        ImGui::TreePush("Installed Mods");
        if (ImGui::BeginListBox("##InstalledModList", ImVec2(-1, 0)))
        {
            for (int i = 0; i < installed_mod_entries.size(); i++)
            {
                std::filesystem::path sourcePath = installed_mod_entries[i]["SourcePath"];
                std::string label = sourcePath.filename().string();

                if (ImGui::Selectable(label.c_str()))
                {
                    ImGui::OpenPopup(label.c_str());
                }

                ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.5f, -1));
                if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
                {
                    ImGui::SetWindowPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x / 2) - (ImGui::GetWindowSize().x / 2),
                        (ImGui::GetIO().DisplaySize.y / 2) - (ImGui::GetWindowSize().y / 2)));

                    if (ImGui::Button("Uninstall", ImVec2(-1, 0)))
                    {
                        if (GameSelection[game_selection_index] == "MonsterHunterRise" && ModManager::contains_pak_files(installed_mod_entries[i]["SourcePath"]))
                        {
                            ModManager::uninstall_pak_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"], selected_game_path);
                        }
                        else
                        {
                            ModManager::uninstall_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"]);
                        }

                        staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                        available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                        installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);

                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Exit", ImVec2(-1, 0)))
                    {
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