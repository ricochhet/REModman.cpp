cmake_minimum_required(VERSION 3.16)
project(imgui_app)

set(CMAKE_CXX_STANDARD 17)

# GLFW
set(GLFW_DIR glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# nfd
set(NFD_DIR nativefiledialog) # Set this to point to an up-to-date nfd repo
add_library(nativefiledialog ${NFD_DIR}/src/nfd_common.c ${NFD_DIR}/src/nfd_win.cpp)
target_include_directories(nativefiledialog PUBLIC ${NFD_DIR}/src/include)

# Add the ImGui source and header files
set(IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

set(IMGUI_HEADERS
    "${CMAKE_SOURCE_DIR}/imgui/imgui.h"
    "${CMAKE_SOURCE_DIR}/imgui/imgui_internal.h"
    "${CMAKE_SOURCE_DIR}/imgui/imconfig.h"
    "${CMAKE_SOURCE_DIR}/imgui/imstb_rectpack.h"
    "${CMAKE_SOURCE_DIR}/imgui/imstb_textedit.h"
    "${CMAKE_SOURCE_DIR}/imgui/imstb_truetype.h"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h"
)

# Add the ImGuiFileDialog source and header files
set(IMGUI_FILE_DIALOG_SOURCES
    "${CMAKE_SOURCE_DIR}/ImGuiFileDialog/ImGuiFileDialog.cpp"
)

set(IMGUI_FILE_DIALOG_HEADERS
    "${CMAKE_SOURCE_DIR}/ImGuiFileDialog/ImGuiFileDialog.h"
)

# Add the nlohmann source files
set(NLOHMANN_JSON_SOURCES
    "${CMAKE_SOURCE_DIR}/nlohmann/json.hpp"
)

# Add the utfcpp source files
set(UTFCPP_SOURCES
    "${CMAKE_SOURCE_DIR}/utfcpp/source/utf8.h"
    "${CMAKE_SOURCE_DIR}/utfcpp/source/utf8/checked.h"
    "${CMAKE_SOURCE_DIR}/utfcpp/source/utf8/core.h"
    "${CMAKE_SOURCE_DIR}/utfcpp/source/utf8/cpp11.h"
    "${CMAKE_SOURCE_DIR}/utfcpp/source/utf8/cpp17.h"
    "${CMAKE_SOURCE_DIR}/utfcpp/source/utf8/unchecked.h"
)

# Add the include directories for ImGui and GLFW
include_directories(
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/glfw/include
    ${CMAKE_SOURCE_DIR}/ImGuiFileDialog
    ${CMAKE_SOURCE_DIR}/nlohmann
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/utfcpp/source
    ${NFD_DIR}/include
)

# Define IMGUI_DEFINE_MATH_OPERATORS
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# Create the executable target
# Add all the source files under "src" to the executable
file(GLOB SOURCES "src/*.cpp" "src/include/*.h")
add_executable(imgui_standalone_window ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_HEADERS} ${IMGUI_FILE_DIALOG_SOURCES} ${IMGUI_FILE_DIALOG_HEADERS} ${NLOHMANN_JSON_SOURCES} ${UTFCPP_SOURCES})

# Link the GLFW and OpenGL libraries
target_link_libraries(imgui_standalone_window glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui_standalone_window OpenGL::GL)
target_link_libraries(imgui_standalone_window nativefiledialog)
