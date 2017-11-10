#include "log.h"
#include <stdio.h>


ConsoleLog::ConsoleLog()
{
}

void ConsoleLog::logMsg(OutputImportance importance, const wchar_t* msg)
{
	if (MSG_NORMAL == importance)
	{
		wprintf(L"[INFO] %s\n", msg);
	}
	else if (MSG_WARNING == importance)
	{
		wprintf(L"[WARNING] %s\n", msg);
	}
	else if (MSG_ERROR == importance)
	{
		wprintf(L"[ERROR] %s\n", msg);
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

void Logger::log(OutputImportance importance, const wchar_t* msg, ...)
{
	if (s_pInstance && s_pInstance->m_pImpl)
	{
		va_list argList;
		wchar_t buffer[260];
		__crt_va_start(argList, msg);
		int result = vswprintf_s(buffer, 260, msg, argList);
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
