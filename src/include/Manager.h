#ifndef __MANAGER_H
#pragma once

#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <imgui.h>
#include "Json.h"
#include "ModInfo.h"
#include "FsProvider.h"
#include "Crc32.h"
#include "Rules.h"
#include <ImGuiFileDialog.h>

namespace FileDialog
{
    void DrawFileDialog();
}

namespace Manager 
{
    void SearchDirectories(const std::string& path);
    void LoadProfileJson();
    void LoadCacheJson();
    void SaveCache();

    void DrawSetup();
    void DrawGameSelector();
    void DrawRefreshButton();
    void DrawDirectoryList();
}

#endif __MANAGER_H