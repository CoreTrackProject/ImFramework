#include "ImThread.h"
#include "ImFramework.h"

#include <chrono>

ThreadToken ImThread::DefineThread(std::string name, void (*async_func)(ThreadToken*)) {


	ThreadToken newToken;
	//newToken.ID = static_cast<int>(ImThread::tokens.size());
	newToken.AsyncFunc = async_func;

	//ImThread::tokens.push_back(std::move(newToken));

	return std::move(newToken);
}

void ImThread::StartThread(ThreadToken& token) {


	int idx = token.ID;

	token.IsRunning = true;
	token.IsFinished = false;
	token.RequestCancel = false;


	if (token.AsyncFunc != nullptr) {

		token.Thread = std::thread(
			token.AsyncFunc,
			&token
		);
		token.Thread.detach();

	}

}

bool ImThread::IsFinished(ThreadToken& token) {



	int idx = token.ID;

	if (token.IsFinished) {
		token.IsRunning = false;
		token.IsFinished = false;
		return true;
	}

	return false;
}

bool ImThread::HasProgress(ThreadToken& token) {
	
	// Delay feature, only report progress for ex. when every 10 seconds are past
	int delta_time = ImFramework::GetImThreadProgressDeltaTime();
	if (delta_time != 0 && token.IsRunning) {
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto mil_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		
		static long long last_time = 0;

		if ((mil_seconds - last_time) > (long long)delta_time) {
			last_time = mil_seconds;
		} else {
			return false;
		}
	}

	int idx = token.ID;

	bool hasProgress = token.HasProgress() && token.IsRunning;

	return hasProgress;
}