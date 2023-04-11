#include <imgui_window.h>
#include <manager_ui.h>

void ImGuiWindow::setupImGuiWindow() {
    ManagerUI::Instance().drawProfileFileDlg();
    ManagerUI::Instance().drawGamePathFileDlg();

    ManagerUI::Instance().drawAvailableMods();
    ManagerUI::Instance().drawStagedMods();
    ManagerUI::Instance().drawDeployBtn();
    ManagerUI::Instance().drawInstalledMods();
}

int main() {
    Logger::Instance().setLogLevel(LogLevel::Error);
    Logger::Instance().log("REModman Initialized.", LogLevel::OK);
    ManagerUI::Instance().setupVars();
    ImGuiWindow::createImGuiWindow("REModman", "remodman-icon-1024.png", "remodman_ui.ini");
}
