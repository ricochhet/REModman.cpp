#ifndef REMODMAN_IMGUIWINDOW_INCLUDED
#define REMODMAN_IMGUIWINDOW_INCLUDED
#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <JsonUtils.h>
#include <murmurhash.h>
#include <font_robotomedium.h>
#include <REModman.h>

namespace ImGuiWindow
{
    int create_imgui_window();
    void setup_imgui_fonts(const ImGuiIO &io);
    void setup_imgui_style();
    void setup_imgui_window();
}

#endif REMODMAN_IMGUIWINDOW_INCLUDED