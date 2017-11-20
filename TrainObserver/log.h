#pragma once
#include "log.h"
#include "defs.hpp"

enum OutputImportance
{
	MSG_NORMAL,
	MSG_WARNING,
	MSG_ERROR
};

class LogInterface
{
public:
	virtual void logMsg(OutputImportance importance, const char* msg) = 0;
};

void initLog(LogInterface* pInterface);


#define ENABLE_LOG 1

#if ENABLE_LOG

class Logger
{
public:
	~Logger();

	static void	init(LogInterface* pImpl);
	static void log(OutputImportance importance, const char* msg, ...);

private:
	Logger(LogInterface * pImpl);
private:
	static Logger*		s_pInstance;
	LogInterface*		m_pImpl;
};



class ConsoleLog : public LogInterface
{
public:
	ConsoleLog();

	virtual void logMsg(OutputImportance importance, const char* msg) override;
};

class WindowLog : public LogInterface
{
public:
	WindowLog(HWND parent);
	virtual void logMsg(OutputImportance importance, const char* msg) override;
private:
	HWND m_parent;
};

#define LOG(priority, msg, ...) Logger::log(priority, msg, ##__VA_ARGS__);

#else

#define LOG(priority, msg, ...)

#endif
