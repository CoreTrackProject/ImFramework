#pragma once

#include <string>

// Forward declaration
struct GLFWwindow;


struct ImFrameworkIO {
	int Width = 1280;
	int Height = 720;

	int WindowX = 0;
	int WindowY = 0;

	std::string Title = "Window";

	GLFWwindow* Window = nullptr;
};

class ImFramework {

public:
	static void Init();

	static void Destroy();

	static bool Begin();

	static void End();	

	static ImFrameworkIO GetIO();

};