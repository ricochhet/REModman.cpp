@echo off

set BUILD_DIR=build

rem Create build directory if it does not exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

rem Change directory to build directory
cd %BUILD_DIR%

rem Generate build files using CMake
cmake ..

rem Build application using generated build files
cmake --build .

rem Run the application
.\Debug\imgui_standalone_window.exe

rem Pause so user can see output
pause
