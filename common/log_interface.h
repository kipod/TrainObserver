#pragma once

#define ENABLE_LOG 1


#ifdef ENABLE_LOG

enum OutputImportance
{
	MSG_NORMAL,
	MSG_WARNING,
	MSG_ERROR
};

void log(OutputImportance priority, const char* msg, ...);

#define LOG(priority, msg, ...) log(priority, msg, ##__VA_ARGS__);

#else

#define LOG(priority, msg, ...);

#endif
