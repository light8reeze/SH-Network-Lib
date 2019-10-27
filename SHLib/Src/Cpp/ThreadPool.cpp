#include "ThreadPool.h"
#include "Arguments.h"

namespace SHLib
{
	CThreadPool* CThreadPool::m_pInstance = nullptr;

	CThreadPool::CThreadPool() : m_ThreadList(), m_pThreadEvent(nullptr)
	{
		m_pInstance = this;
	}

	CThreadPool::~CThreadPool()
	{
		Clear();
	}

	DWORD CThreadPool::AddThread(CThread* _pThread)
	{
		assert(_pThread != nullptr);

		if (_pThread == nullptr)
			return 1;

		m_ThreadList.push_back(_pThread);
		return 0;
	}

	DWORD CThreadPool::Initialize()
	{
		CDefaultArguments& pArguments = GetSingleton<CDefaultArguments>();

		for (int index = 0; index < pArguments.GetWorkThreadCnt(); ++index)
		{
			CWorkThread* pWorkThread = new CWorkThread;
			pWorkThread->Initialize();
			AddThread(pWorkThread);
		}

		for (int index = 0; index < pArguments.GetAcceptorCnt(); ++index)
		{
			CAcceptor* pAcceptor = new CAcceptor;
			pAcceptor->Initialize(pArguments.GetIPAddress(index), pArguments.GetPort(index));
			AddThread(pAcceptor);

			CSocket& socket = pAcceptor->GetSocket();

			for (int cnt = 0; cnt < pArguments.GetAcceptThreadCnt(index); ++cnt)
			{
				CAcceptor* pAcceptor2 = new CAcceptor;
				pAcceptor2->Initialize(socket);
				AddThread(pAcceptor2);
			}
		}
		return 0;
	}

	DWORD CThreadPool::WaitForReady()
	{
		return WaitForMultipleObjects(m_ThreadList.size(), m_pThreadEvent, TRUE, INFINITE);
	}

	DWORD CThreadPool::StartAllThread()
	{
		m_pThreadEvent = new HANDLE[m_ThreadList.size()];
		
		int index = 0;
		for (auto iter = m_ThreadList.begin(); iter != m_ThreadList.end(); ++iter)
		{
			assert(index < static_cast<int>(m_ThreadList.size()));
			m_pThreadEvent[index++] = (*iter)->GetEvent();
		}

		for (auto iter = m_ThreadList.begin(); iter != m_ThreadList.end(); ++iter)
			(*iter)->StartThread();

		return 0;
	}

	//모든 쓰레드를 제거하는 함수
	void CThreadPool::Clear()
	{
		auto iter = m_ThreadList.begin();
		while (iter != m_ThreadList.end())
		{
			CThread* pThread = (*iter);
			iter = m_ThreadList.erase(iter);
			SafeDelete<CThread>(pThread);
		}

		SafeArrayDelete<HANDLE>(m_pThreadEvent);
	}
}