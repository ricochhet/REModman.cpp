#include <Window.h>

void Window::ImGuiWindow()
{
    REModman::draw_load_profile();
    REModman::draw_game_selector();
    ImGui::SameLine();
    REModman::draw_mod_list();
}

int main()
{
    Window::BaseWindow();
}
