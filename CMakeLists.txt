cmake_minimum_required(VERSION 3.16)
project(imgui_app)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/dependencies/glfw")
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

set(IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_opengl3.cpp"
)

set(IMGUI_HEADERS
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_internal.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imconfig.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imstb_rectpack.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imstb_textedit.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imstb_truetype.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_glfw.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_opengl3.h"
)

set(IMGUI_FILE_DIALOG_SOURCES
    "${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog/ImGuiFileDialog.cpp"
)

set(IMGUI_FILE_DIALOG_HEADERS
    "${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog/ImGuiFileDialog.h"
)

set(NLOHMANN_JSON_SOURCES
    "${CMAKE_SOURCE_DIR}/dependencies/nlohmann/json.hpp"
)

set(UTFCPP_SOURCES
    "${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source/utf8.h"
    "${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source/utf8/checked.h"
    "${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source/utf8/core.h"
    "${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source/utf8/cpp11.h"
    "${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source/utf8/cpp17.h"
    "${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source/utf8/unchecked.h"
)

set(MURMURHASH3_SOURCES
    "${CMAKE_SOURCE_DIR}/dependencies/MurmurHash3/MurmurHash3.h"
    "${CMAKE_SOURCE_DIR}/dependencies/MurmurHash3/MurmurHash3.cpp"
)

include_directories(
    ${CMAKE_SOURCE_DIR}/dependencies/imgui
    ${CMAKE_SOURCE_DIR}/dependencies/nlohmann
    ${CMAKE_SOURCE_DIR}/dependencies/MurmurHash3
    ${CMAKE_SOURCE_DIR}/dependencies/glfw/include
    ${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog
    ${CMAKE_SOURCE_DIR}/dependencies/utfcpp/source
    ${CMAKE_SOURCE_DIR}/src/resources
    ${CMAKE_SOURCE_DIR}/src
)

add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

file(GLOB SOURCES "src/*.cpp" "src/include/*.h")
add_executable(imgui_standalone_window ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_HEADERS} ${IMGUI_FILE_DIALOG_SOURCES} ${IMGUI_FILE_DIALOG_HEADERS} ${NLOHMANN_JSON_SOURCES} ${UTFCPP_SOURCES} ${MURMURHASH3_SOURCES})

target_link_libraries(imgui_standalone_window glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui_standalone_window OpenGL::GL)