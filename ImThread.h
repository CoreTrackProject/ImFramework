#pragma once

#include <string>
#include <functional>
#include <mutex>


struct ImThreadToken {
	int ID = -1;
	bool RequestCancel = false;
	std::mutex* SyncObject = nullptr;
};

class ImThread {

public:
	static ImThreadToken* DefineThread(std::string name, void (*async_func)(bool*, std::mutex*));

	static void StartThread(ImThreadToken* token);

	static bool IsFinished(ImThreadToken* token);

	static bool HasProgress(ImThreadToken* token, void* data);

};