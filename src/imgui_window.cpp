#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif  // STB_IMAGE_IMPLEMENTATION

#include <imgui_window.h>
#include <stb_image.h>

static void glfwErrorCallback(int error, const char* description) { std::cerr << "GLFW Error " << error << ": " << description << std::endl; }

void ImGuiWindow::setupImGuiFont(const ImGuiIO& io) {
    io.Fonts->Clear();
    io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, (float)16);
    io.Fonts->Build();
}

void ImGuiWindow::setupImGuiStyle() {
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

int ImGuiWindow::createImGuiWindow(const char* title, const char* iconFileName, const char* iniFileName) {
    // Setup window
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 650, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    GLFWimage icon[1];
    icon[0].pixels = stbi_load(iconFileName, &icon[0].width, &icon[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, icon);
    stbi_image_free(icon[0].pixels);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    setupImGuiFont(io);
    (void)io;
    ImGui::GetIO().IniFilename = iniFileName;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    setupImGuiStyle();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
        ImGui::Begin("Main", NULL, window_flags);
        ImGuiWindow::setupImGuiWindow();
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}