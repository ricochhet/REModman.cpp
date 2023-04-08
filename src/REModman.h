#ifndef REMODMAN_INCLUDED
#define REMODMAN_INCLUDED

#pragma once

#include <ImGuiFileDialog.h>
#include <Logger.h>
#include <ManagerImpl.h>
#include <RisePakPatch.h>
#include <imgui.h>

namespace FileDialog {
    void draw_load_profile_dialog();
    void draw_get_game_path_dialog();
}

namespace REModman {
    void draw_load_profile();
    void draw_game_selector();
    void draw_get_game_path();

    void draw_mod_list();
    void draw_staging_mod_list();
    void draw_mod_deploy_button();
    void draw_installed_mod_list();
}

#endif  // REMODMAN_INCLUDED