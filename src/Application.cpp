#include <ImGuiWindow.h>
#include <REModman.h>

void ImGuiWindow::setupImGuiWindow() {
    REModman::drawLoadProfile();
    REModman::drawContextSelector();
    // REModman::drawGetGamePath();
    // REModman::drawGameSelector();

    // REModman::drawAvailableModList();
    // REModman::drawStagedModList();
    // REModman::drawDeployButton();
    // REModman::drawInstalledModList();
}

int main() {
    Logger::getInstance().setLogLevel(LogLevel::Error);
    Logger::getInstance().log("REModman Initialized.", LogLevel::OK);
    ImGuiWindow::createImGuiWindow("REModman", "remodman-icon-1024.png", "remodman_ui.ini");
}
