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

void ImGuiWindow::setupImGuiStyle() { ImGuiThemes::slateColorful(); }

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