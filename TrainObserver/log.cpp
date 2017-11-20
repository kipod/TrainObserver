#include "log.h"
#include <stdio.h>
#include <windows.h>


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

Logger::Logger(LogInterface* pImpl) : m_pImpl(pImpl)
{
}

Logger::~Logger()
{
	s_pInstance = nullptr;
}

Logger* Logger::s_pInstance = nullptr;

void Logger::init(LogInterface* pImpl)
{
	s_pInstance = new Logger(pImpl);
}

void Logger::log(OutputImportance importance, const char* msg, ...)
{
	if (s_pInstance && s_pInstance->m_pImpl)
	{
		va_list argList;
		char buffer[2000];
		__crt_va_start(argList, msg);
		int result = vsprintf_s(buffer, 2000, msg, argList);
		__crt_va_end(argList);

		s_pInstance->m_pImpl->logMsg(importance, buffer);
	}
}

#if ENABLE_LOG

void initLog(LogInterface* pInterface)
{
	Logger::init(pInterface);
}

#else

void initLog(LogInterface* pInterface)
{
}

#endif

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
