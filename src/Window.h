#ifndef REMODMAN_WINDOW_INCLUDED
#define REMODMAN_WINDOW_INCLUDED
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

namespace Window
{
    int BaseWindow();
    void ImGuiWindow();
}

#endif REMODMAN_WINDOW_INCLUDED