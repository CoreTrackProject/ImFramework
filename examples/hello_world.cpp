#pragma once

#include <iostream>

#include <ImFramework.h>
#include <ImThread.h>

#include <imgui.h>

int main() {
	ImFramework::Init();

	ImFramework::EnableFeature(ImFramework_Feature::DPI_Awareness, true);
	ImFramework::EnableFeature(ImFramework_Feature::ImThread_HasProgress_Delta_Time, 10000);

	ThreadToken token_thread_1 = ImThread::DefineThread("TestThread1",
		[](ThreadToken* data) {
			for (int i = 0; i <= 1000; i++) {
				if (data->RequestCancel) {
					data->SetData<std::string>("TestThread1 cancel Requested");
					break;
				}
				std::this_thread::sleep_for(std::chrono::microseconds(10000));
				data->SetData<std::string>("TestThread1 reporting: " + std::to_string(i));
			}
			data->IsFinished = true;
		}
	);

	ThreadToken token_thread_2 = ImThread::DefineThread("TestThread2",
		[](ThreadToken* data) {
			for (int i = 0; i <= 1000; i++) {
				if (data->RequestCancel) {
					data->SetData<std::string>("TestThread2 cancel Requested");
					break;
				}
				std::this_thread::sleep_for(std::chrono::microseconds(5000));
				data->SetData<std::string>("TestThread2 reporting: " + std::to_string(i));
			}
			data->IsFinished = true;
		}
	);

	while (ImFramework::Begin()) {

		ImFramework::BeginWindow("Hello World", 1920, 1080);
		{
			ImGui::ShowDemoWindow();

			if (ImGui::Button("Start Thread")) {
				ImThread::StartThread(token_thread_1);
				ImThread::StartThread(token_thread_2);
			}

			if (token_thread_1.IsRunning && ImGui::Button("Cancel Task 1")) {
				token_thread_1.RequestCancel = true;
				token_thread_2.RequestCancel = true;
			}



			if (ImThread::HasProgress(token_thread_1)) {
				std::cout << token_thread_1.GetValue<std::string>() << std::endl;
			}

			if (ImThread::IsFinished(token_thread_1)) {
				std::cout << "Thread 1 is finished" << std::endl;
			}



			if (ImThread::HasProgress(token_thread_2)) {
				std::cout << token_thread_2.GetValue<std::string>() << std::endl;
			}

			if (ImThread::IsFinished(token_thread_2)) {
				std::cout << "Thread 2 is finished" << std::endl;
			}

			
		}
		ImFramework::EndWindow();



		ImFramework::End();
	}

	ImFramework::Destroy();

	return 0;
}