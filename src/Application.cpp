#include <ImGuiWindow.h>
#include <ManagerUI.h>

void ImGuiWindow::setupImGuiWindow() {
    ManagerUI::getInstance().drawProfileFileDlg();
    ManagerUI::getInstance().drawGamePathFileDlg();
    ManagerUI::getInstance().drawGameSelector();

    ManagerUI::getInstance().drawAvailableMods();
    ManagerUI::getInstance().drawStagedMods();
    ManagerUI::getInstance().drawDeployBtn();
    ManagerUI::getInstance().drawInstalledMods();
}

int main() {
    Logger::getInstance().setLogLevel(LogLevel::Error);
    Logger::getInstance().log("REModman Initialized.", LogLevel::OK);
    ManagerUI::getInstance().setupVars();
    ImGuiWindow::createImGuiWindow("REModman", "remodman-icon-1024.png", "remodman_ui.ini");
}
