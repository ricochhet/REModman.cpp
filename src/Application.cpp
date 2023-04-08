#include <ImGuiWindow.h>

void ImGuiWindow::setupImGuiWindow() {
    REModman::drawLoadProfile();
    REModman::drawGetGamePath();
    REModman::drawGameSelector();

    REModman::drawAvailableModList();
    REModman::drawStagedModList();
    REModman::drawDeployButton();
    REModman::drawInstalledModList();
}

int main() {
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    Logger::getInstance().setLogLevel(LogLevel::Error);
    Logger::getInstance().log("REModman Initialized.", LogLevel::OK);
    ImGuiWindow::createImGuiWindow("REModman", "remodman-icon-1024.png", "remodman_ui.ini");
}
