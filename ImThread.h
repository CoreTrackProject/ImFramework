#pragma once

#include <string>

struct ImThreadToken {
	int ID = -1;
	bool RequestCancel = false;
};

class ImThread {

public:
	static ImThreadToken* DefineThread(std::string name);

	static void StartThread(ImThreadToken* token);

	static bool IsFinished(ImThreadToken* token);

	static bool HasProgress(void* data);

};