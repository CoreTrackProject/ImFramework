#pragma once

#include <string>

// Forward declaration
struct GLFWwindow;
struct ImGuiContext;

struct ImFrameworkIO {
	int Width = 1280;

	int Height = 720;

	int WindowX = 0;

	int WindowY = 0;

	std::string Title = "";

	GLFWwindow* Window = nullptr;

	bool IsOpen = true;

	ImGuiContext* Context;

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

	static void destroyWindow(ImFrameworkIO window);

};