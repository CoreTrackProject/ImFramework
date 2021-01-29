#pragma once

#include <string>


#include <GL/glew.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

struct ImFrameworkIO {
	int Width = 1280;

	int Height = 720;

	int WindowX = 0;

	int WindowY = 0;

	std::string Title = "";

	GLFWwindow* Window = nullptr;

	bool IsOpen = true;

	bool Initialized = false;

};

class ImFramework {

public:
	static void Init();

	static void Destroy();

	static bool Begin();

	static void End();	

	static void OnResize(GLFWwindow* window, int width, int height);

	static void BeginWindow(std::string title = "", int width = 1280, int height = 720);

	static void EndWindow();

	static ImFrameworkIO GetIO();

private:
	static void createNewWindow(std::string title, int width, int height);

	static void hideWindow(ImFrameworkIO window);

	static void initOpenGL();
};