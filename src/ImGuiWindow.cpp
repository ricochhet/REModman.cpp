#include <Window.h>

void Window::ImGuiWindow()
{
    /*ImGui::Text("This is a standalone window using the ImGui library.");

    FileDialog::DrawFileDialog();

    if (ImGui::Button("Test function"))
    {
        Manager::SearchDirectories("D:\\dev\\imgui-app\\Mods\\MonsterHunterRise\\");
    }*/

    Manager::DrawSetup();
    Manager::DrawGameSelector();
    ImGui::SameLine();
    Manager::DrawRefreshButton();
    Manager::DrawDirectoryList();
}