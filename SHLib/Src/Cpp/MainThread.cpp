#include "MainThread.h"
#include "ThreadPool.h"
#include "SessionManager.h"
#include "TimeScheduler.h"
#include "Logger.h"
#include "Arguments.h"

namespace SHLib
{
	CMainThread::CMainThread() : CThread()
	{
		static INT nCreate = 0;

		assert(nCreate == 0);
		++nCreate;
	}

	CMainThread::~CMainThread()
	{
		WSACleanup();
	}

	//반드시 상위에서 하위로 호출할것(super -> this)
	HANDLE CMainThread::Initialize()
	{
		DWORD dwResult = 0;

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			return NULL;

		TODO_MODIFY_ERRORCODE;
		HANDLE hMainEvent = SUPER::Initialize();
		if (hMainEvent == NULL)
			return NULL;
		
		dwResult = GetSingleton<CNetworkIO>().Initialize();
		dwResult = GetSingleton<CThreadPool>().Initialize();
		GetSingleton<CTimeScheduler>().Initialize(GetSingleton<CDefaultArguments>().GetTimeEventMax());
		
		return hMainEvent;
	}

	DWORD CMainThread::Start()
	{
		TODO_MODIFY_ERRORCODE;
		DWORD dwResult;

		dwResult = OnStart();

		dwResult = GetSingleton<CThreadPool>().StartAllThread();

		StartThread();

		return 0;
	}

	INT CMainThread::Main()
	{
		while (TRUE)
		{
			GetSingleton<CSessionManager>().CloseGhost();
			Sleep(1000);
		}

		return 0;
	}
}