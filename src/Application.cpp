#include <ImGuiWindow.h>
#include <Logger.h>

lua_State *L = luaL_newstate();

void ImGuiWindow::setup_imgui_window()
{
    REModman::draw_load_profile();
    ImGui::SameLine();
    REModman::draw_game_selector();
    REModman::draw_mod_list();
    REModman::draw_installed_mod_list();

    LuaBindings::imgui_ctx(L, ImGui::GetCurrentContext());
    LuaBindings::bind_create_button(L);
}

int main()
{
    AllocConsole();
    FILE *stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);

    Logger::getInstance().setLogLevel(LogLevel::Error);
    Logger::getInstance().log("Logger initialized.", LogLevel::Error);
    LuaBindings::load_lua(L);

    ImGuiWindow::create_imgui_window();
}
