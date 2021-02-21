#include "ImThread.h"

//std::vector<ThreadToken> ImThread::tokens;
std::mutex ImThread::mtx;

ThreadToken ImThread::DefineThread(std::string name, void (*async_func)(ThreadToken*)) {
	std::lock_guard<std::mutex> lock(mtx);

	ThreadToken newToken;
	//newToken.ID = static_cast<int>(ImThread::tokens.size());
	newToken.AsyncFunc = async_func;

	//ImThread::tokens.push_back(std::move(newToken));

	return std::move(newToken);
}

void ImThread::StartThread(ThreadToken& token) {
	std::lock_guard<std::mutex> lock(mtx);

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
	std::lock_guard<std::mutex> lock(mtx);


	int idx = token.ID;

	if (token.IsFinished) {
		token.IsRunning = false;
		token.IsFinished = false;
		return true;
	}

	return false;
}

bool ImThread::HasProgress(ThreadToken& token) {
	std::lock_guard<std::mutex> lock(mtx);

	int idx = token.ID;

	bool hasProgress = token.HasProgress() && token.IsRunning;

	return hasProgress;
}