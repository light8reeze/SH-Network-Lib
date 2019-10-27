#pragma once
#include "Thread.h"
#include "MemoryDump.h"
#include "Singleton.h"

namespace SHLib
{
	/*
		클래스 : CMainThread
		기본클래스 : CThread

		서버의 초기화, 종료를 담당하는 쓰레드
		(추후에 Thread를 서비스로 사용)
	*/
	class SH_EXPORT CMainThread : public CThread
	{
	private:
		typedef CThread SUPER;
		CMemoryDump	m_Dump; //예외 발생시 사용하는 MemoryDump

	public:
		CMainThread();
		virtual ~CMainThread();
		
		virtual HANDLE	Initialize();
		DWORD Start();
		virtual DWORD OnStart() { return 0; }
		INT Main();
	};
}