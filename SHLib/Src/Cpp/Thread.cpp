#include "Thread.h"
#include "ThreadPool.h"
#include <process.h>

namespace SHLib
{
	CThread::CThread() : m_hThread(NULL), m_isRun(false), 
		m_isExit(false), m_dwSwitchingTime(10), m_hEvent(NULL), m_pArgs(nullptr)
	{
	}

	CThread::~CThread()
	{
		ExitThread();
	}

	//쓰레드를 초기화하는 함수
	//오버라이딩시 super를 호출할것.
	//return HANDLE : 쓰레드의 이벤트 핸들
	HANDLE CThread::Initialize()
	{
		m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		return m_hEvent;
	}

	void CThread::SetArguments(LPVOID _pArg)
	{
		m_pArgs = _pArg;
	}

	//쓰레드를 생성하여 실행하는 함수
	//THREAD_MAIN _pRoutine : 쓰레드에서 실행할 함수 포인터
	//LPVOID _pArg : 쓰레드에서 사용할 인수의 포인터
	HANDLE CThread::StartThread()
	{
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadExecute, this, 0, NULL);
		m_isRun = true;
		m_isExit = false;

		return m_hThread;
	}

	//쓰레드를 재시작하는 함수
	void CThread::RestartThread()
	{
		m_isRun = true;
	}

	//쓰레드를 정지하는 함수
	void CThread::StopThread()
	{
		m_isRun = false;
	}

	//쓰레드를 종료하는 함수
	void CThread::ExitThread()
	{
		m_isExit = true;
		m_isRun = false;
	}

	void CThread::ContextSwitching()
	{
		static DWORD dwLastSwitching = GetTickCount();

		if (GetTickCount() - dwLastSwitching >= m_dwSwitchingTime)
		{
			Sleep(1);
			dwLastSwitching = GetTickCount();
		}
	}

	//쓰레드의 핸들을 반환하는 함수
	//return HANDLE : 쓰레드의 핸들
	HANDLE CThread::GetHandle()
	{
		return m_hThread;
	}

	HANDLE CThread::GetEvent()
	{
		return m_hEvent;
	}

	//쓰레드가 실행되는 여부를 반환하는 함수
	//return BOOL : 실행중인 여부
	BOOL CThread::IsRun()
	{
		return m_isRun;
	}

	//쓰레드가 종료되었는지 여부를 반환하는 함수
	//return BOOL : 쓰레드 종료 여부
	BOOL CThread::IsExit()
	{
		return m_isExit;
	}

	//CThread내부 쓰레드에서 실행되는 함수
	//LPVOID _pArgs : CThread객체
	//return unsigned : 사용하지 않음
	unsigned WINAPI CThread::ThreadExecute(LPVOID _pArgs)
	{
		CThread* pThread = static_cast<CThread*>(_pArgs);
		int nResult = 0;

		SetEvent(pThread->GetEvent());
		GetSingleton<CThreadPool>().WaitForReady();

		//THREAD_MAIN실행
		nResult = pThread->Main();

		return nResult;
	}
}