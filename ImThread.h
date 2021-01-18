#pragma once

#include <imgui.h>

struct ImFrameworkIO {
	int Width;
	int Height;

	int WindowX;
	int WindowY;
}

class ImFramework {

public:
	static void Init();

	static bool Begin();

	static void End();	

	static ImFrameworkIO GetIO();
};