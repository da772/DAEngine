#include "win_crash_handler.h"

#ifdef DA_PLATFORM_WINDOWS
#include "crash_handler.h"
#include <Dbghelp.h>
#include <tchar.h>
#include <csignal>  
#include <core/logger.h>

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType, CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

#define FORCE_CRASH() int* p = 0; *p = 5;

void create_minidump(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	HANDLE hFile = CreateFile(L"core.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		// No actual exception info here; using a NULL pointer
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ExceptionPointers = apExceptionInfo;
		dumpInfo.ClientPointers = FALSE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		CloseHandle(hFile);
	}
}

void SignalHandler(int signal) {
	
	da::CCrashHandler::createDump();
}

LONG WINAPI unhandled_handler(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	create_minidump(apExceptionInfo);
	LOG_CALLSTACK(da::ELogType::Assert, da::ELogChannel::Core);
	return EXCEPTION_EXECUTE_HANDLER;
}


void da::CCrashHandler::registerCrashHandler()
{
	signal(SIGABRT, SignalHandler);
	SetUnhandledExceptionFilter(unhandled_handler);
}

void da::CCrashHandler::createDump()
{
#ifdef DA_REVIEW
	if (!IsDebuggerPresent())
#endif
	{
		
		SetUnhandledExceptionFilter(unhandled_handler);
		FORCE_CRASH()
	}
}

#else

void da::CCrashHandler::registerCrashHandler()
{

}

void da::CCrashHandler::createDump()
{
	
}
#endif