#include <REModman.h>

static int game_selection_index = 0;
std::string selected_profile_path;
std::vector<nlohmann::json> staged_mod_entries;
std::vector<nlohmann::json> available_mod_entries;
std::vector<nlohmann::json> installed_mod_entries;
ImVec2 popup_modal_size(400, 200);

std::vector<std::string> GameSelection = {
    "None",
    "MonsterHunterRise",
    "MonsterHunterWorld"};

void FileDialog::draw_file_dialog()
{
    if (ImGui::Button("Load Profile"))
    {
        ImGuiFileDialog::Instance()->OpenDialog("LoadProfileDlgKey", "Choose Folder", nullptr, "");
    }

    if (ImGuiFileDialog::Instance()->Display("LoadProfileDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            JsonUtils::create_or_update_json(filePathName + "/profile.json", "SelectedProfile", filePathName, true);
            JsonUtils::create_or_update_json(filePathName + "/profile.json", "PatchReEnginePakIndex", 2, false);
            selected_profile_path = filePathName;

            game_selection_index = ModManager::get_game_selection(selected_profile_path);
            staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
            available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
            installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void REModman::draw_load_profile()
{
    FileDialog::draw_file_dialog();

    if (!selected_profile_path.empty())
    {
        ImGui::SameLine();
        std::string label = "Loaded profile: " + selected_profile_path;
        ImGui::Text(label.c_str());
    }
}

void REModman::draw_game_selector()
{
    if (!selected_profile_path.empty())
    {
        if (ImGui::BeginCombo("##", GameSelection[game_selection_index].c_str()))
        {
            for (int i = 0; i < GameSelection.size(); i++)
            {
                bool isSelected = (game_selection_index == i);
                if (ImGui::Selectable(GameSelection[i].c_str(), isSelected))
                {
                    game_selection_index = i;
                    JsonUtils::create_or_update_json(selected_profile_path + "/profile.json", "LastSelectedGame", i, true);
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
        if (!ImGui::CollapsingHeader("Available Mods"))
        {
            return;
        }

        ImGui::TreePush("AvailableMods");
        ImGui::BeginListBox("##AvailableModsList", ImVec2(-1, 0));

        for (int i = 0; i < available_mod_entries.size(); i++)
        {
            std::filesystem::path sourcePath = available_mod_entries[i]["SourcePath"];
            std::string label = sourcePath.filename().string();

            if (ImGui::Selectable(label.c_str()))
            {
                ImGui::OpenPopup(label.c_str());
            }

            if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove))
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(ImVec2(
                    (ImGui::GetIO().DisplaySize.x / 2) - ImGui::GetContentRegionAvail().x / 2,
                    (ImGui::GetIO().DisplaySize.y / 2) - ImGui::GetContentRegionAvail().y / 2));

                ImGui::InputInt("Load Order", &load_order);
                if (ImGui::Button("Add"))
                {
                    ModManager::stage_mod(selected_profile_path, available_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/", selected_profile_path + "/Game/", load_order);

                    staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                    available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                    installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
                }
                ImGui::Separator();
                if (ImGui::Button("Exit"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::EndListBox();
        ImGui::TreePop();
    }
}

void REModman::draw_staging_mod_list()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Staged Mods"))
        {
            return;
        }

        ImGui::TreePush("Staged Mods");
        ImGui::BeginListBox("##StagedModsList", ImVec2(-1, 0));

        for (int i = 0; i < staged_mod_entries.size(); i++)
        {
            std::filesystem::path sourcePath = staged_mod_entries[i]["SourcePath"];
            std::string label = sourcePath.filename().string();

            if (ImGui::Selectable(label.c_str()))
            {
                ImGui::OpenPopup(label.c_str());
            }

            if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove))
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(ImVec2(
                    (ImGui::GetIO().DisplaySize.x / 2) - ImGui::GetContentRegionAvail().x / 2,
                    (ImGui::GetIO().DisplaySize.y / 2) - ImGui::GetContentRegionAvail().y / 2));

                if (ImGui::Button("Remove"))
                {
                    ModManager::destage_mod(selected_profile_path, staged_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/");
                    staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                    available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                    installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
                }
                ImGui::Separator();
                if (ImGui::Button("Exit"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::EndListBox();
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
                    ModManager::uninstall_pak_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/");
                }
                else
                {
                    ModManager::uninstall_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/");
                }
            }

            for (int i = 0; i < staged_mod_entries.size(); i++)
            {
                ModManager::install_mod(selected_profile_path, staged_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/", selected_profile_path + "/Game/");
                ModManager::destage_mod(selected_profile_path, staged_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/");
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
        if (!ImGui::CollapsingHeader("Installed Mods"))
        {
            return;
        }

        ImGui::TreePush("Installed Mods");
        ImGui::BeginListBox("##InstalledModList", ImVec2(-1, 0));

        for (int i = 0; i < installed_mod_entries.size(); i++)
        {
            std::filesystem::path sourcePath = installed_mod_entries[i]["SourcePath"];
            std::string label = sourcePath.filename().string();

            if (ImGui::Selectable(label.c_str()))
            {
                ImGui::OpenPopup(label.c_str());
            }

            if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove))
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(ImVec2(
                    (ImGui::GetIO().DisplaySize.x / 2) - ImGui::GetContentRegionAvail().x / 2,
                    (ImGui::GetIO().DisplaySize.y / 2) - ImGui::GetContentRegionAvail().y / 2));

                if (ImGui::Button("Uninstall"))
                {
                    if (GameSelection[game_selection_index] == "MonsterHunterRise" && ModManager::contains_pak_files(installed_mod_entries[i]["SourcePath"]))
                    {
                        ModManager::uninstall_pak_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/");
                    }
                    else
                    {
                        ModManager::uninstall_mod(selected_profile_path, installed_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/");
                    }

                    staged_mod_entries = ModManager::get_staged_mod_entries(selected_profile_path);
                    available_mod_entries = ModManager::get_available_mod_entries(selected_profile_path);
                    installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
                }
                ImGui::Separator();
                if (ImGui::Button("Exit"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::EndListBox();
        ImGui::TreePop();
    }
}