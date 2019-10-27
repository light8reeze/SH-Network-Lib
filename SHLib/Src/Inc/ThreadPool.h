#pragma once
#include <vector>
#include "WorkThread.h"
#include "Acceptor.h"
#include "Singleton.h"

namespace SHLib
{
	/*
		클래스 : CThreadPool
		기본 클래스 : CSingletonBase

		CThread(쓰레드)를 관리하는 클래스(싱글톤 구조)
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
		void Clear(); //모든 쓰레드를 제거하는 함수
	};
}