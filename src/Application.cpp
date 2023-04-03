#include <ImGuiWindow.h>
#include <Logger.h>

void ImGuiWindow::setup_imgui_window()
{
    REModman::draw_load_profile();
    ImGui::SameLine();
    REModman::draw_game_selector();

    REModman::draw_mod_list();
    REModman::draw_staging_mod_list();
    REModman::draw_mod_deploy_button();
    REModman::draw_installed_mod_list();
}

int main()
{
    AllocConsole();
    FILE *stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);

    Logger::getInstance().setLogLevel(LogLevel::Error);
    Logger::getInstance().log("Logger initialized.", LogLevel::Error);

    ImGuiWindow::create_imgui_window();
    LuaBindings::create_lua_state();
}
