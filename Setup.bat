@echo off

git clone --branch docking https://github.com/ocornut/imgui.git imgui
git clone --branch 2020.11-1 https://github.com/microsoft/vcpkg.git vcpkg

cd vcpkg

cmd /c call ./bootstrap-vcpkg.bat
cmd /c vcpkg.exe install glfw3 glew --triplet x64-windows









