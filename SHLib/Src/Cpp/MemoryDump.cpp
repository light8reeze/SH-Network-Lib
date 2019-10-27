#include "MemoryDump.h"
#include <stdio.h>
#include <time.h>
#include <DbgHelp.h>

namespace SHLib
{
	//예외 발생시 덤프파일을 만들어주는 함수
	//EXCEPTION_POINTERS* _pException : 발생한 예외정보
	LONG CALLBACK ExceptionHandler(EXCEPTION_POINTERS* _pException)
	{
		time_t timer;
		struct tm t;
		timer = time(NULL);
		localtime_s(&t, &timer);
		char fileName[30];

		//덤프파일 이름을 설정(현재시간으로 설정)
		wsprintf(fileName, "%d-%d-%d %d시%d분%d초.dmp", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
		printf("Unhandled Exception! dump file = %s\n", fileName);

		HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE process = GetCurrentProcess();
		DWORD processId = GetCurrentProcessId();
		DWORD threadId = GetCurrentThreadId();

		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = threadId;
		exceptionInfo.ExceptionPointers = _pException;
		exceptionInfo.ClientPointers = FALSE;

		//덤프파일 생성
		BOOL bResult = MiniDumpWriteDump(process, processId, hFile, MiniDumpWithFullMemory, &exceptionInfo, NULL, NULL);
		if (bResult == FALSE)
			printf("MiniDump Fail : %d\n", GetLastError());

		CloseHandle(hFile);

		return EXCEPTION_EXECUTE_HANDLER;
	}

	CMemoryDump::CMemoryDump()
	{
		SetCallbackException(ExceptionHandler);
	}

	CMemoryDump::~CMemoryDump()
	{
	}

	//예외 발생시 실행할 함수를 설정해주는 함수
	//LPTOP_LEVEL_EXCEPTION_FILTER _pFilter : 예외 발생시 실행할 함수 포인터
	void CMemoryDump::SetCallbackException(LPTOP_LEVEL_EXCEPTION_FILTER _pFilter)
	{
		SetUnhandledExceptionFilter(_pFilter);
	}
}