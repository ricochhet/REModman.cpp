#ifndef REMODMAN_INCLUDED
#define REMODMAN_INCLUDED

#pragma once

#include <imgui.h>
#include <ImGuiFileDialog.h>
#include <JsonUtils.h>
#include <ModManager.h>

namespace FileDialog
{
    void draw_file_dialog();
}

namespace REModman 
{
    void draw_load_profile();
    void draw_game_selector();
    void draw_mod_list();
}

#endif REMODMAN_INCLUDED