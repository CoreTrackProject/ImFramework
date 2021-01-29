#include "ImFramework.h"


#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>


#include <vector>
#include <iostream>


#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>   // for glfwGetWin32Window
#endif


static bool isFinished = true;
static int windowIndex = 0;
std::vector<ImFrameworkIO> windows;
static bool glewIsInit = false;
static bool firstBegin = true;


void ImFramework::Init() {
	if (!glfwInit()) {
		std::abort();
	}

	// Imgui init
	ImGui::CreateContext();
	
	
}

void ImFramework::Destroy() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for (auto& wnd : windows) {
		glfwDestroyWindow(wnd.Window);
	}
	
	glfwTerminate();
}

bool ImFramework::Begin() {
	if (!isFinished) {
		std::cerr << "Error: ImFramework::End(); has not been called." << std::endl;
		std::abort();
	}

	return firstBegin || windows[0].IsOpen;
}

void ImFramework::End() {
	isFinished = true;
	firstBegin = false;
	windowIndex = 0;
}

void ImFramework::BeginWindow(std::string title, int width, int height) {

	// Main window
	if (windowIndex >= windows.size()) {
		ImFramework::createNewWindow(title, width, height);
		windows[windowIndex].Initialized = true;
	}

	glfwMakeContextCurrent(windows[windowIndex].Window);

	if (windowIndex == 0) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	windows[windowIndex].IsOpen = !glfwWindowShouldClose(windows[windowIndex].Window);

}

void ImFramework::EndWindow() {

	if (!windows[windowIndex].IsOpen) {
		ImFramework::hideWindow(windows[windowIndex]);
	}
	
	// main window
	if (windowIndex == 0) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	glfwSwapBuffers(windows[windowIndex].Window);


	windowIndex++;
}

void ImFramework::OnResize(GLFWwindow* window, int width, int height) {
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
}

ImFrameworkIO ImFramework::GetIO() {
	return windows[windowIndex];
}


void ImFramework::createNewWindow(std::string title, int width, int height) {

	ImFrameworkIO inst;

	inst.Width = width;
	inst.Height = height;
	inst.Title = title;

	// Create new window
	GLFWwindow* newWnd = 
		glfwCreateWindow(
			width,
			height,
			title.c_str(),
			nullptr,
			nullptr
		);

	inst.Window = newWnd;

	windows.push_back(std::move(inst));

	glfwWindowHint(GLFW_DOUBLEBUFFER,   1);
	glfwWindowHint(GLFW_DEPTH_BITS,    24);
	glfwWindowHint(GLFW_STENCIL_BITS,   8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if (windowIndex == 0) {
		
		

		glfwSetFramebufferSizeCallback(windows[0].Window, ImFramework::OnResize);
		glfwMakeContextCurrent(windows[0].Window);

		ImGui_ImplGlfw_InitForOpenGL(windows[0].Window, true);
		ImFramework::initOpenGL();


		return;
	}



	

	



}

void ImFramework::hideWindow(ImFrameworkIO window) {
	// Just hide the window, cleanup during ImFramework::Destroy();
	glfwHideWindow(window.Window);
}

void ImFramework::initOpenGL() {

	std::string glsl_version = "";
#ifdef __APPLE__
	// GL 3.2 + GLSL 150
	glsl_version = "#version 150";
	glfwWindowHint( // required on Mac OS
		GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE
	);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif __linux__
	// GL 3.2 + GLSL 150
	glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
	// GL 3.2 + GLSL 150
	glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
	//float highDPIscaleFactor = 1.0;
#ifdef _WIN32
	// if it's a HighDPI monitor, try to scale everything
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	float xscale, yscale;
	glfwGetMonitorContentScale(monitor, &xscale, &yscale);
	if (xscale > 1 || yscale > 1)
	{
		//highDPIscaleFactor = xscale;
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	}
#elif __APPLE__
	// to prevent 1200x800 from becoming 2400x1600
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

	ImGui_ImplOpenGL3_Init(glsl_version.c_str());

	if (!glewIsInit) {
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
			std::abort();
		}
		glewIsInit = true;
	}

}