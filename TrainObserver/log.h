#pragma once
#include "defs.hpp"
#include "common/log_interface.h"



#if ENABLE_LOG

class LogInterface
{
public:
	virtual ~LogInterface();

	static void	init(LogInterface* pImpl);
	static void log(OutputImportance importance, const char* msg);
	virtual void logMsg(OutputImportance importance, const char* msg) = 0;

protected:
	LogInterface() {};

private:
	static LogInterface* s_pInstance;
};

void initLog(LogInterface* pInterface);

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

#endif
