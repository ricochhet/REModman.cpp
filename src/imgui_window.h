#ifndef REMODMAN_IMGUIWINDOW_INCLUDED
#define REMODMAN_IMGUIWINDOW_INCLUDED
#pragma once

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <font_robotomedium.h>
#include <imgui.h>

#include <iostream>

class ImGuiThemes {
public:
    static void slate() {
        ImGui::StyleColorsDark();

        auto& style             = ImGui::GetStyle();
        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.PopupRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.ScrollbarRounding = 2.0f;
        style.GrabRounding      = 0.0f;
        style.TabRounding       = 0.0f;
        style.WindowBorderSize  = 2.0f;
        style.WindowPadding     = ImVec2(2.0f, 0.0f);

        auto& colors = ImGui::GetStyle().Colors;
        // Window BG
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

        // Navigatation highlight
        colors[ImGuiCol_NavHighlight] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};

        // Progress Bar
        colors[ImGuiCol_PlotHistogram] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};

        // Headers
        colors[ImGuiCol_Header]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive]  = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive]  = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

        // Checkbox
        colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.5505f, 0.551f, 1.0f);

        // Frame BG
        colors[ImGuiCol_FrameBg]        = ImVec4{0.211f, 0.210f, 0.25f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive]  = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab]                = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
        colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused]       = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.8f, 0.805f, 0.81f, 1.0f};

        // Resize Grip
        colors[ImGuiCol_ResizeGrip]        = ImVec4{0.2f, 0.205f, 0.21f, 0.0f};
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ResizeGripActive]  = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg]          = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
        colors[ImGuiCol_TitleBgActive]    = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
    }

    static void slateColorful() {
        ImGui::StyleColorsDark();

        auto& style             = ImGui::GetStyle();
        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.PopupRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.ScrollbarRounding = 2.0f;
        style.GrabRounding      = 0.0f;
        style.TabRounding       = 0.0f;
        style.WindowBorderSize  = 2.0f;
        style.WindowPadding     = ImVec2(2.0f, 0.0f);

        auto& colors = ImGui::GetStyle().Colors;
        // Window BG
        colors[ImGuiCol_WindowBg] = ImVec4{0.17f, 0.17f, 0.17f, 1.0f};

        // Navigatation highlight
        colors[ImGuiCol_NavHighlight] = ImVec4{0.17f, 0.17f, 0.17f, 1.0f};

        // Progress Bar
        colors[ImGuiCol_PlotHistogram] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};

        // Headers
        colors[ImGuiCol_Header]        = ImVec4{0.17f, 0.17f, 0.17f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.27f, 0.27f, 0.27f, 1.0f};
        colors[ImGuiCol_HeaderActive]  = ImVec4{0.47f, 0.47f, 0.47f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive]  = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

        // Checkbox
        colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.5505f, 0.551f, 1.0f);

        // Frame BG
        colors[ImGuiCol_FrameBg]        = ImVec4{0.3f, 0.3f, 0.3f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.6f, 0.6f, 0.6f, 1.0f};
        colors[ImGuiCol_FrameBgActive]  = ImVec4{0.9f, 0.9f, 0.9f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab]                = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
        colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused]       = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.8f, 0.805f, 0.81f, 1.0f};

        // Resize Grip
        colors[ImGuiCol_ResizeGrip]        = ImVec4{0.2f, 0.205f, 0.21f, 0.0f};
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ResizeGripActive]  = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg]          = ImVec4{0.17f, 0.17f, 0.17f, 1.0f};
        colors[ImGuiCol_TitleBgActive]    = ImVec4{0.27f, 0.27f, 0.27f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.47f, 0.47f, 0.47f, 1.0f};
    }
};

namespace ImGuiWindow {
    int  createImGuiWindow(const char* title, const char* iconFileName, const char* iniFileName);
    void setupImGuiFont(const ImGuiIO& io);
    void setupImGuiStyle();
    void setupImGuiWindow();
}

#endif  // REMODMAN_IMGUIWINDOW_INCLUDED