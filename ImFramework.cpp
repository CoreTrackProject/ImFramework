#include "ImFramework.h"


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_opengl3.h>

#include <imgui.h>
#include <map>
#include <iostream>

// Current window id
static int CurrentID = 0;

// Window cycle Id
static int CycleID = 0;


std::map<int, ImFrameworkIO> instances;
bool initialized = false;


void ImFramework::Init() {

	if (!glfwInit()) {
		std::abort();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	initialized = true;

}

void ImFramework::Destroy() {

	for (auto &i : instances) {
		glfwDestroyWindow(i.second.Window);
	}

	glfwTerminate();
}

bool ImFramework::Begin() {

	if (!initialized) {
		std::cerr << "Error: ImFramework::Init(); has not been called." << std::endl;
		std::abort();
	}

	// when CurrentID is == 0 and CycleID is == 0 then the very first Begin() is called
	// when CurrentID is >= 1 and CycleID is >= 1 then a Begin() whithin a Begin() is called
	// when CurrentID is >= 1 and CycleID is == 0 then a Begin() was called after an End()
	
	// Values are after method call
	//			Num	CurrentID	CycleID
	// Begin()	1	1			0
	// End()	1	0			0
	// Begin()	2	2			0
	// End()	2	1			1
	// Begin()	3	2			1
	// End()	3	1			0

	// Values are after method call
	//			Num	CurrentID	CycleID
	// Begin()	1	1			0
	// Begin()	2	2			0
	// End()	2	1			1
	// End()	1	0			0


	// Check id if ImFramework should create a new window
	if (instances.size() == CurrentID && CycleID == 0) {
		ImFrameworkIO inst;

		GLFWwindow* newWnd = glfwCreateWindow(
			inst.Width,
			inst.Height,
			inst.Title.c_str(),
			nullptr,
			nullptr
		);

		inst.Window = newWnd;
		instances.emplace(CurrentID, std::move(inst));
	}

	glfwMakeContextCurrent(instances[CurrentID].Window);
	glfwPollEvents();

	bool shouldClose = glfwWindowShouldClose(instances[CurrentID].Window);

	// Hierarchy id
	CurrentID++;

	return !shouldClose;
}

void ImFramework::End() {

	if (CurrentID == 0) {
		std::cerr << "Error: ImFramework::Begin(); has not been called" << std::endl;
		std::abort();
	}

	CurrentID--;
	CycleID = ++CycleID % instances.size();

	glfwSwapBuffers(instances[CurrentID].Window);

}	

ImFrameworkIO ImFramework::GetIO() {
	return instances[CurrentID];
}