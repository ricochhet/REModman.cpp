# REModman
Mod Manager for video-games.


## Building

Download the following tools if you do not have them already installed.
- Clang: [download here](https://github.com/llvm/llvm-project/releases)
- Ninja: [download here](https://github.com/ninja-build/ninja/releases)
- Cmake: [download here](https://cmake.org/download/)

- Run the `ninja_build_debug.bat` or `ninja_build_release` to build and run with Ninja.
- Run the `build.bat` to build and run with the default build system.


## Supported Games
- Monster Hunter Rise


## Suggestions & PRs
Suggestions and pull requests are very appreciated, just keep in mind to follow the project architecture to keep it consistent.


## Notes
- The functionality of ImGuiFileDialog was changed in commit [2f30510](https://github.com/ricochhet/REModman.cpp/commit/2f305104f04df9f821091db327190853dfe85f86#diff-ab29181026bdfc58e40677980f93983d42e0c64d26dc6485f2900612a46eb433), which includes the addition of a `vPos` parameter to the Display() function. This allows us to specify a position for our window without disrupting the flow of the UI. This change is not included in the official repo for this dependency.