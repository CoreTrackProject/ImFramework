#pragma once


#include <iostream>
#include <ImFramework.h>

#include <imgui_docking/imgui.h>

int main() {

	ImFramework::Init();

	while (ImFramework::Begin()) {


		ImFramework::BeginWindow("Hello World", 1920, 1080);
		{
			ImGui::ShowDemoWindow();
		}
		ImFramework::EndWindow();



		ImFramework::End();
	}

	ImFramework::Destroy();

	return 0;
}