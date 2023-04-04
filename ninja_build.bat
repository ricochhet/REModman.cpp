@echo off

set BUILD_DIR=build_ninja

rem Create build directory if it does not exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

rem Change directory to build directory
cd %BUILD_DIR%

rem Generate build files using CMake
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G "Ninja" ..

rem Build application using generated build files
ninja

rem Run the application
.\REModman.exe

rem Pause so user can see output
pause
