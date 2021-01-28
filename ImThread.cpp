#include "ImThread.h"

#include <memory>
#include <thread>
#include <vector>


struct ImThreadTokenInternal : public ImThreadToken {
	std::thread Thread;
	void (*AsyncFunc)(bool*, std::mutex*) = nullptr;
	bool IsRunning = false;
	void* Data = nullptr;
};

std::vector<ImThreadTokenInternal> tokens;

ImThreadToken* ImThread::DefineThread(std::string name, void (*async_func)(bool*, std::mutex*)) {

	ImThreadTokenInternal newToken;
	newToken.ID = static_cast<int>(tokens.size());
	newToken.AsyncFunc = async_func;
	

	tokens.push_back(std::move(newToken));

	return &tokens[tokens.size() - 1];
}

void ImThread::StartThread(ImThreadToken* token) {
	if (tokens.size() <= 0) {
		return;
	}

	int idx = token->ID - 1;

	if (tokens[idx].AsyncFunc != nullptr) {
		tokens[idx].IsRunning = true;
		tokens[idx].Thread = std::thread(tokens[idx].AsyncFunc, &tokens[idx].RequestCancel, tokens[idx].SyncObject);
		tokens[idx].Thread.detach();
	}
}

bool ImThread::IsFinished(ImThreadToken* token) {
	if (tokens.size() <= 0) {
		return true;
	}

	int idx = token->ID - 1;

	return tokens[idx].Thread.joinable();
}

bool ImThread::HasProgress(ImThreadToken* token, void* data) {


	int idx = token->ID - 1;

	

	return true;
}