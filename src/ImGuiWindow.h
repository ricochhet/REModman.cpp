#ifndef REMODMAN_IMGUIWINDOW_INCLUDED
#define REMODMAN_IMGUIWINDOW_INCLUDED
#pragma once

#include <GLFW/glfw3.h>
#include <JsonUtils.h>
#include <MurmurHash.h>
#include <REModman.h>
#include <Windows.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <font_robotomedium.h>
#include <imgui.h>

#include <iostream>

namespace ImGuiWindow {
    int  createImGuiWindow(const char* title, const char* iconFileName, const char* iniFileName);
    void setupImGuiFont(const ImGuiIO& io);
    void setupImGuiStyle();
    void setupImGuiWindow();
}

#endif  // REMODMAN_IMGUIWINDOW_INCLUDED