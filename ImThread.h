#pragma once

#include <string>
#include <functional>
#include <mutex>

#include <vector>
#include <thread>


struct ThreadToken {

	template<typename T>
	void SetData(T value) {
		static T value_data;
		value_data = value;
		internal_data = static_cast<void*>(&value_data);
		this->has_progress = true;
	}

	template<typename T>
	T GetValue() {
		this->has_progress = false;
		auto res = static_cast<T*>(internal_data);

		if (res == nullptr) {
			*res = T();
		}

		return *res;
	}

	bool HasProgress() { return this->has_progress; }

	


public:
	int ID = -1;

	bool RequestCancel = false;

	std::thread Thread;

	bool IsRunning = false;

	bool IsFinished = false;

	void (*AsyncFunc)(ThreadToken*);


private:
	void* internal_data = nullptr;
	bool has_progress   = false;
};



class ImThread {

public:
	static ThreadToken DefineThread(std::string name, void (*async_func)(ThreadToken*));

	static void StartThread(ThreadToken& token);

	static bool IsFinished(ThreadToken& token);

	static bool IsFinished(ThreadToken& ...) {

		va_list()
	}

	static bool HasProgress(ThreadToken& token);

	
private:
	static std::mutex mtx;

};