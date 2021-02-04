#pragma once


#include <iostream>
#include <ImFramework.h>

#include <imgui.h>

int main() {

	ImFramework::Init();

	ImFramework::EnableFeature(ImFramework_Feature::Feature_DPI_Awareness, true);

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