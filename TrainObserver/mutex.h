#pragma once
#include <windows.h>

class SimpleMutex
{
public:
	SimpleMutex()
	{
		InitializeCriticalSection(&mutex_);
	}

	~SimpleMutex()
	{
		DeleteCriticalSection(&mutex_);
	}

	void grab()
	{
		EnterCriticalSection(&mutex_);
	}

	void give()
	{
		LeaveCriticalSection(&mutex_);
	}

	bool grabTry()
	{
		return TryEnterCriticalSection(&mutex_);
	};

private:
	CRITICAL_SECTION	mutex_;
};

class SimpleMutexHolder
{
public:
	SimpleMutexHolder(SimpleMutex & sm) : sm_(sm) { sm_.grab(); }
	~SimpleMutexHolder() { sm_.give(); }
private:
	SimpleMutex & sm_;
};
