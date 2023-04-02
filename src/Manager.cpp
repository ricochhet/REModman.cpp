#include "Manager.h"

std::vector<std::string> game_selection = {"None", "MonsterHunterRise", "MonsterHunterWorld"};
static int game_selection_index = 0;
static std::unordered_map<std::string, int> inputint_states;
static std::unordered_map<std::string, bool> checkbox_states;

ImVec2 popup_modal_size(400, 200);
ImVec2 popup_modal_pos(0, 0);

std::vector<ModInfo> mod_list;
nlohmann::json j_profile;
nlohmann::json j_cache;
std::string selected_profile_path;

void FileDialog::DrawFileDialog()
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
            Json::create_or_update_json(filePathName + "/profile.json", "SelectedProfile", filePathName);
            selected_profile_path = filePathName;

            Manager::LoadProfileJson();
            Manager::LoadCacheJson();
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Manager::DrawSetup()
{
    FileDialog::DrawFileDialog();

    if (!selected_profile_path.empty())
    {
        ImGui::SameLine();
        std::string m = "Loaded profile: " + selected_profile_path;
        ImGui::Text(m.c_str());
    }
}

void Manager::DrawGameSelector()
{
    if (!selected_profile_path.empty())
    {
        if (ImGui::BeginCombo("##", game_selection[game_selection_index].c_str()))
        {
            for (int i = 0; i < game_selection.size(); i++)
            {
                bool isSelected = (game_selection_index == i);
                if (ImGui::Selectable(game_selection[i].c_str(), isSelected))
                {
                    game_selection_index = i;
                    Json::create_or_update_json(selected_profile_path + "/profile.json", "LastSelectedGame", i);
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

void Manager::DrawRefreshButton()
{
    if (!selected_profile_path.empty())
    {
        if (ImGui::Button("Refresh"))
        {
            LoadProfileJson();
            LoadCacheJson();
        }
    }
}

void Manager::DrawDirectoryList()
{
    if (!selected_profile_path.empty())
    {
        if (!ImGui::CollapsingHeader("Mods"))
        {
            return;
        }

        ImGui::TreePush("Mods");

        for (auto &it : j_cache)
        {
            std::string label = it["Name"];
            if (ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                ImGui::OpenPopup(label.c_str());
            }

            if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowSize(popup_modal_size);
                ImGui::SetWindowPos(popup_modal_pos);
                bool enabled = checkbox_states[label];
                int value = inputint_states[label];

                if (ImGui::InputInt("Load Order", &value))
                {
                    it["LoadOrder"] = value;
                    inputint_states[label] = it["LoadOrder"].get<int>();

                    std::ofstream ofs(selected_profile_path + "/" + "mods.json");
                    ofs << j_cache.dump(4) << std::endl;
                }
                ImGui::SameLine();
                if (ImGui::Checkbox("Enable", &enabled))
                {
                    it["IsEnabled"] = enabled;
                    checkbox_states[label] = it["IsEnabled"].get<bool>();

                    std::ofstream ofs(selected_profile_path + "/" + "mods.json");
                    ofs << j_cache.dump(4) << std::endl;
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete"))
                {
                    // handle delete button click
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

void Manager::LoadProfileJson()
{
    j_profile = Json::load_json(selected_profile_path + "/" + "profile.json");

    if (j_profile != nlohmann::json())
    {
        game_selection_index = j_profile["LastSelectedGame"];
    }
}

void Manager::LoadCacheJson()
{
    j_cache = Json::load_json(selected_profile_path + "/" + "mods.json");
    
    if (j_cache == nlohmann::json() || j_cache.empty())
    {
        SearchDirectories(selected_profile_path + "/Mods/" + game_selection[game_selection_index] + "/");
    }
}

void Manager::SearchDirectories(const std::string &path)
{
    std::filesystem::path selectedPath = path.c_str();

    if (std::filesystem::is_directory(selectedPath))
    {
        for (auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                std::string otherDirName = entry.path().filename().string();
                ModInfo modInfo;
                modInfo.Name = otherDirName;

                for (auto &subentry : std::filesystem::directory_iterator(entry.path()))
                {
                    if (subentry.is_directory())
                    {
                        std::string dirName = subentry.path().filename().string();
                        for (auto &rule : REENGINE_RULES)
                        {
                            if (rule.is_directory && dirName == rule.name)
                            {
                                for (auto &fileEntry : std::filesystem::recursive_directory_iterator(subentry.path()))
                                {
                                    if (fileEntry.is_regular_file())
                                    {
                                        std::string file = FsProvider::read_file_to_string(fileEntry.path().string());
                                        uint32_t hash = crc32_fast(file.c_str(), file.length());
                                        modInfo.Files.push_back(make_mod_file("", fileEntry.path().string(), hash));
                                    }
                                }
                            }
                        }
                    }

                    if (subentry.is_regular_file())
                    {
                        std::string ext = subentry.path().extension().string();
                        for (auto &rule : REENGINE_RULES)
                        {
                            if (!rule.is_directory && ext == rule.extension)
                            {
                                std::string file = FsProvider::read_file_to_string(subentry.path().string());
                                uint32_t hash = crc32_fast(file.c_str(), file.length());
                                modInfo.Files.push_back(make_mod_file("", subentry.path().string(), hash));
                            }
                        }
                    }
                }

                if (modInfo.Files.size() != 0)
                {
                    mod_list.push_back(modInfo);
                }
            }
        }

        SaveCache();
    }
    else
    {
        std::filesystem::create_directories(path.c_str());
    }
}

void Manager::SaveCache()
{
    Json::write_json_to_file(selected_profile_path + "/" + "mods.json", Json::store_mod_info_as_json(mod_list));
}