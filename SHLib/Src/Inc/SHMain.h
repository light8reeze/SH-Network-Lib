#include "SHLib.h"
#include "UserSession.h"
#include "MainThread.h"
#include "SessionManager.h"
#include "TimeScheduler.h"
#include "Factory.h"
#include "Arguments.h"

namespace SHLib
{
	template<typename TMainThread = CMainThread, typename TSession = CUserSession>
	DWORD SHMain()
	{
		CConstraint<TMainThread, CMainThread> cThread;
		CConstraint<TSession, CUserSession> cSession;

		static CFactory factory;

		CMainThread* pMainThread = new TMainThread();

		GetSingleton<CDefaultArguments>().Initialize();
		GetSingleton<CSessionManager>().Initialize<TSession>(GetSingleton<CDefaultArguments>().GetSessionMax());

		HANDLE hResult = pMainThread->Initialize();
		
		if (hResult == NULL)
			return INFINITE;

		pMainThread->Start();
		
		while(TRUE)
		{
			GetSingleton<CTimeScheduler>().PopEvent();
			Sleep(1);
		}

		SafeDelete<CMainThread>(pMainThread);
		return 0;
	}
}