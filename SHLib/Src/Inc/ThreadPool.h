#pragma once
#include <vector>
#include "WorkThread.h"
#include "Acceptor.h"
#include "Singleton.h"

namespace SHLib
{
	/*
		Ŭ���� : CThreadPool
		�⺻ Ŭ���� : CSingletonBase

		CThread(������)�� �����ϴ� Ŭ����(�̱��� ����)
	*/
	class SH_EXPORT CThreadPool
	{
		SET_SINGLETON_BASE(CThreadPool);

	private:
		typedef std::vector<CThread*> THREAD_LIST;

	private:
		#pragma warning(disable : 4251)
		THREAD_LIST		m_ThreadList;
		#pragma warning(default : 4251)

	protected:
		HANDLE * m_pThreadEvent;

	protected:
		DWORD AddThread(CThread* _pThread);

	public:
		CThreadPool();
		virtual ~CThreadPool();

		virtual DWORD Initialize();
		virtual DWORD StartAllThread();
		DWORD WaitForReady();
		void Clear(); //��� �����带 �����ϴ� �Լ�
	};
}