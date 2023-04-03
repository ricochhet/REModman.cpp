#include <REModman.h>

static int game_selection_index = 0;
std::string selected_profile_path;

ImVec2 popup_modal_size(400, 200);
ImVec2 popup_modal_pos(0, 0);

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

void REModman::draw_mod_list()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Mods"))
        {
            return;
        }

        ImGui::TreePush("Mods");

        for (auto &it : ModManager::get_mod_entries(selected_profile_path))
        {
            std::string label = it.Name;

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
                    ModManager::install_mod(selected_profile_path, it.Path, selected_profile_path + "/Game/", selected_profile_path + "/Game/");
                }

                ImGui::SameLine();

                if (ImGui::Button("Uninstall"))
                {
                    if (GameSelection[game_selection_index] == "MonsterHunterRise" && ModManager::contains_pak_files(it.Path))
                    {
                        ModManager::uninstall_pak_mod(it, selected_profile_path, it.Path, selected_profile_path + "/Game/");
                    }
                    else
                    {
                        ModManager::uninstall_mod(selected_profile_path, it.Path, selected_profile_path + "/Game/");
                    }
                }
                ImGui::Separator();
                if (ImGui::Button("Exit"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::TreePop();
    }
}