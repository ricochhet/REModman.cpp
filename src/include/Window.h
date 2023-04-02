#ifndef __WINDOW_H
#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <Manager.h>
#include <Json.h>
#include <Murmur3.h>
#include <font_robotomedium.h>

namespace Window
{
    int BaseWindow();
    void ImGuiWindow();
}

#endif __WINDOW_h