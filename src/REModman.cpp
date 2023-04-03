#include <REModman.h>

static int game_selection_index = 0;
std::string selected_profile_path;
std::vector<nlohmann::json> uninstalled_mod_entries;
std::vector<nlohmann::json> installed_mod_entries;

ImVec2 popup_modal_size(400, 200);
ImVec2 popup_modal_pos(0, 0);

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
            uninstalled_mod_entries = ModManager::get_uninstalled_mod_entries(selected_profile_path);
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
int selected_item = -1;
void REModman::draw_mod_list()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Mods"))
        {
            return;
        }

        ImGui::TreePush("Mods");

        ImGui::ListBoxHeader("##List", uninstalled_mod_entries.size(), 4);
        for (int i = 0; i < uninstalled_mod_entries.size(); i++) {
            std::filesystem::path sourcePath = uninstalled_mod_entries[i]["SourcePath"];
            std::string label = sourcePath.filename().string();

            if (ImGui::Selectable(label.c_str(), selected_item == i)) {
                selected_item = i;
                ImGui::OpenPopup(label.c_str());
            }

            if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove)) 
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(popup_modal_pos);

                if (ImGui::Button("Install"))
                {
                    ModManager::install_mod(selected_profile_path, uninstalled_mod_entries[i]["SourcePath"], selected_profile_path + "/Game/", selected_profile_path + "/Game/");

                    uninstalled_mod_entries = ModManager::get_uninstalled_mod_entries(selected_profile_path);
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
        ImGui::ListBoxFooter();

        /*for (auto &it : uninstalled_mod_entries)
        {
            std::filesystem::path sourcePath = it["SourcePath"];
            std::string label = sourcePath.filename().string();

            if (ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                ImGui::OpenPopup(label.c_str());
            }

            if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(popup_modal_pos);

                if (ImGui::Button("Install"))
                {
                    ModManager::install_mod(selected_profile_path, it["SourcePath"], selected_profile_path + "/Game/", selected_profile_path + "/Game/");

                    uninstalled_mod_entries = ModManager::get_uninstalled_mod_entries(selected_profile_path);
                    installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
                }
            
                ImGui::Separator();
                if (ImGui::Button("Exit"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }*/

        ImGui::TreePop();
    }
}

void REModman::draw_installed_mod_list()
{
    ImGui::Separator();
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Installed Mods"))
        {
            return;
        }

        ImGui::TreePush("Installed Mods");

        for (auto &it : installed_mod_entries)
        {
            std::filesystem::path sourcePath = it["SourcePath"];

            if (ImGui::Button(sourcePath.filename().string().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                ImGui::OpenPopup(sourcePath.filename().string().c_str());
            }

            if (ImGui::BeginPopupModal(sourcePath.filename().string().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(popup_modal_pos);

                if (ImGui::Button("Uninstall"))
                {
                    if (GameSelection[game_selection_index] == "MonsterHunterRise" && ModManager::contains_pak_files(it["SourcePath"]))
                    {
                        ModManager::uninstall_pak_mod(selected_profile_path, it["SourcePath"], selected_profile_path + "/Game/");
                    }
                    else
                    {
                        ModManager::uninstall_mod(selected_profile_path, it["SourcePath"], selected_profile_path + "/Game/");
                    }

                    uninstalled_mod_entries = ModManager::get_uninstalled_mod_entries(selected_profile_path);
                    installed_mod_entries = ModManager::get_installed_mod_entries(selected_profile_path);
                }
            }
        }

        ImGui::TreePop();
    }
}