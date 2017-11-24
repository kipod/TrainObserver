#include "log.h"

#if ENABLE_LOG

#include <stdio.h>
#include <windows.h>

LogInterface::~LogInterface()
{
	s_pInstance = nullptr;
}

LogInterface* LogInterface::s_pInstance = nullptr;

void LogInterface::init(LogInterface* pImpl)
{
	s_pInstance = pImpl;
}

void LogInterface::log(OutputImportance importance, const char* msg)
{
	if (s_pInstance)
	{
		s_pInstance->logMsg(importance, msg);
	}
}

void initLog(LogInterface* pInterface)
{
	LogInterface::init(pInterface);
}

ConsoleLog::ConsoleLog()
{
}

void ConsoleLog::logMsg(OutputImportance importance, const char* msg)
{
	if (MSG_NORMAL == importance)
	{
		printf("[INFO] %s\n", msg);
	}
	else if (MSG_WARNING == importance)
	{
		printf("[WARNING] %s\n", msg);
	}
	else if (MSG_ERROR == importance)
	{
		printf("[ERROR] %s\n", msg);
	}
}

WindowLog::WindowLog(HWND parent):
	m_parent(parent)
{

}

void WindowLog::logMsg(OutputImportance importance, const char* msg)
{
	if (importance > MSG_WARNING)
	{
		MessageBox(m_parent, msg, "Error!", MB_OK | MB_APPLMODAL | MB_ICONASTERISK);
	}
}

void log(OutputImportance priority, const char* msg, ...)
{
	va_list argList;
	char buffer[2000];
	__crt_va_start(argList, msg);
	int result = vsprintf_s(buffer, 2000, msg, argList);
	__crt_va_end(argList);

	LogInterface::log(priority, buffer);
}

#endif