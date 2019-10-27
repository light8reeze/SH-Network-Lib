#include "MemoryDump.h"
#include <stdio.h>
#include <time.h>
#include <DbgHelp.h>

namespace SHLib
{
	//���� �߻��� ���������� ������ִ� �Լ�
	//EXCEPTION_POINTERS* _pException : �߻��� ��������
	LONG CALLBACK ExceptionHandler(EXCEPTION_POINTERS* _pException)
	{
		time_t timer;
		struct tm t;
		timer = time(NULL);
		localtime_s(&t, &timer);
		char fileName[30];

		//�������� �̸��� ����(����ð����� ����)
		wsprintf(fileName, "%d-%d-%d %d��%d��%d��.dmp", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
		printf("Unhandled Exception! dump file = %s\n", fileName);

		HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE process = GetCurrentProcess();
		DWORD processId = GetCurrentProcessId();
		DWORD threadId = GetCurrentThreadId();

		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = threadId;
		exceptionInfo.ExceptionPointers = _pException;
		exceptionInfo.ClientPointers = FALSE;

		//�������� ����
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

	//���� �߻��� ������ �Լ��� �������ִ� �Լ�
	//LPTOP_LEVEL_EXCEPTION_FILTER _pFilter : ���� �߻��� ������ �Լ� ������
	void CMemoryDump::SetCallbackException(LPTOP_LEVEL_EXCEPTION_FILTER _pFilter)
	{
		SetUnhandledExceptionFilter(_pFilter);
	}
}