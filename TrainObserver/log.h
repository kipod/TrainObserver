#pragma once
#include "log.h"

enum OutputImportance
{
	MSG_NORMAL,
	MSG_WARNING,
	MSG_ERROR
};

class LogInterface
{
public:
	virtual void logMsg(OutputImportance importance, const wchar_t* msg) = 0;
};

void initLog(LogInterface* pInterface);


#define ENABLE_LOG 1

#if ENABLE_LOG

class Logger
{
public:
	~Logger();

	static void	init(LogInterface* pImpl);
	static void log(OutputImportance importance, const wchar_t* msg, ...);

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

	virtual void logMsg(OutputImportance importance, const wchar_t* msg) override;
};

#define LOG(priority, msg, ...) Logger::log(priority, msg, ##__VA_ARGS__);

#else

#define LOG(priority, msg, ...)

#endif
