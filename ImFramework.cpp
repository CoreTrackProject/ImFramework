#include "ImFramework.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>
#include <iostream>


#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>   // for glfwGetWin32Window
#endif


static bool isFinished = true;
static int windowIndex = 0;
static std::vector<ImFrameworkIO> windows;
static bool glewIsInit = false;
static bool firstBegin = true;


void ImFramework::Init() {
	if (!glfwInit()) {
		std::abort();
	}
}

void ImFramework::Destroy() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	for (auto& io : windows) {
		ImGui::DestroyContext(io.Context);
		ImFramework::destroyWindow(io);
	}

	glfwTerminate();
}

bool ImFramework::Begin() {

	if (!isFinished) {
		std::cerr << "Error: ImFramework::End(); has not been called." << std::endl;
		std::abort();
	}

	isFinished = false;
	bool result = false;

	if (firstBegin) {
		firstBegin = false;
		result = true;

	} else {
		result = windows.size() > 0;
	}

	return result;
}

void ImFramework::End() {

	isFinished = true;
	windowIndex = 0;
}

void ImFramework::BeginWindow(std::string title, int width, int height) {

	// tmp
	if (windowIndex >= 1) {
		std::cerr << "Error, only one Window is currently supported" << std::endl;
		//std::abort();
	}

	if (windows.size() <= windowIndex) {
		ImFramework::createNewWindow(title, width, height);
	}

	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ImGui::SetCurrentContext(windows[windowIndex].Context);


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	windows[windowIndex].IsOpen = !glfwWindowShouldClose(windows[windowIndex].Window);

	
}

void ImFramework::EndWindow() {


	if (!windows[windowIndex].IsOpen) {
		ImFramework::destroyWindow(windows[windowIndex]);

		windows.erase(windows.begin() + windowIndex);
		windowIndex--;

		return;
	}


	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
	// Create new window

	ImFrameworkIO inst;

	GLFWwindow* newWnd =
		glfwCreateWindow(
			width,
			height,
			title.c_str(),
			nullptr,
			nullptr
		);

	{
		inst.Context = ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	inst.Width  = width;
	inst.Height = height;
	inst.Title  = title;
	inst.Window = newWnd;

	windows.push_back(std::move(inst));

	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	ImGui_ImplGlfw_InitForOpenGL(windows[windowIndex].Window, true);

	glfwSetFramebufferSizeCallback(windows[windowIndex].Window, ImFramework::OnResize);
	glfwMakeContextCurrent(windows[windowIndex].Window);

	if (!glewIsInit) {
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
			std::abort();
		}
		glewIsInit = true;


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

	}

}

void ImFramework::destroyWindow(ImFrameworkIO window) {
	// Just hide the window, cleanup during ImFramework::Destroy();
	glfwHideWindow(window.Window);
}