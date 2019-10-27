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

	//�����带 �ʱ�ȭ�ϴ� �Լ�
	//�������̵��� super�� ȣ���Ұ�.
	//return HANDLE : �������� �̺�Ʈ �ڵ�
	HANDLE CThread::Initialize()
	{
		m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		return m_hEvent;
	}

	void CThread::SetArguments(LPVOID _pArg)
	{
		m_pArgs = _pArg;
	}

	//�����带 �����Ͽ� �����ϴ� �Լ�
	//THREAD_MAIN _pRoutine : �����忡�� ������ �Լ� ������
	//LPVOID _pArg : �����忡�� ����� �μ��� ������
	HANDLE CThread::StartThread()
	{
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadExecute, this, 0, NULL);
		m_isRun = true;
		m_isExit = false;

		return m_hThread;
	}

	//�����带 ������ϴ� �Լ�
	void CThread::RestartThread()
	{
		m_isRun = true;
	}

	//�����带 �����ϴ� �Լ�
	void CThread::StopThread()
	{
		m_isRun = false;
	}

	//�����带 �����ϴ� �Լ�
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

	//�������� �ڵ��� ��ȯ�ϴ� �Լ�
	//return HANDLE : �������� �ڵ�
	HANDLE CThread::GetHandle()
	{
		return m_hThread;
	}

	HANDLE CThread::GetEvent()
	{
		return m_hEvent;
	}

	//�����尡 ����Ǵ� ���θ� ��ȯ�ϴ� �Լ�
	//return BOOL : �������� ����
	BOOL CThread::IsRun()
	{
		return m_isRun;
	}

	//�����尡 ����Ǿ����� ���θ� ��ȯ�ϴ� �Լ�
	//return BOOL : ������ ���� ����
	BOOL CThread::IsExit()
	{
		return m_isExit;
	}

	//CThread���� �����忡�� ����Ǵ� �Լ�
	//LPVOID _pArgs : CThread��ü
	//return unsigned : ������� ����
	unsigned WINAPI CThread::ThreadExecute(LPVOID _pArgs)
	{
		CThread* pThread = static_cast<CThread*>(_pArgs);
		int nResult = 0;

		SetEvent(pThread->GetEvent());
		GetSingleton<CThreadPool>().WaitForReady();

		//THREAD_MAIN����
		nResult = pThread->Main();

		return nResult;
	}
}