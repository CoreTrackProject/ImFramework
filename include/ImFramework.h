#pragma once

#include <string>


enum class ImFramework_Feature {
	DPI_Awareness = 0,
	ImThread_HasProgress_Delta_Time = 1
};

struct ImFrameworkIO {
	int Width = 1280;

	int Height = 720;

	int WindowX = 0;

	int WindowY = 0;

	std::string Title = "";

	void* Window = nullptr;

	bool IsOpen = true;

	bool Initialized = false;

};

class ImFramework {

public:
	static void Init();

	static void Destroy();

	static bool Begin();

	static void End();	

	static void OnResize(void* window, int width, int height);

	static void BeginWindow(std::string title = "", int width = 1280, int height = 720);

	static void EndWindow();

	static ImFrameworkIO GetIO();

	static float GetScaleFactor();

	static void EnableFeature(ImFramework_Feature feature, bool value);

	static void EnableFeature(ImFramework_Feature feature, int value);

	static int GetImThreadProgressDeltaTime() { return ImFramework::imthread_progress_delta_time; }

private:
	static void createNewWindow(std::string title, int width, int height);

	static void hideWindow(ImFrameworkIO window);

	static void initOpenGL();

private:
	static bool enable_dpi_awareness;
	static float dpi_scale;

	static int imthread_progress_delta_time;
};