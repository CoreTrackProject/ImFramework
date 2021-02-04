@echo off

if not exist imgui (
	git clone --branch docking https://github.com/ocornut/imgui.git imgui
)

if not exist vcpkg (
	git clone --branch 2020.11-1 https://github.com/microsoft/vcpkg.git vcpkg
	cd vcpkg

	cmd /c call ./bootstrap-vcpkg.bat
	cmd /c vcpkg.exe install glfw3 glew --triplet x64-windows

	cd ..
)


if not exist build (
	mkdir build
	cd build

	cmake .. -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" -DBUILD_EXAMPLE=ON
)









