#pragma once
#include "Thread.h"
#include "MemoryDump.h"
#include "Singleton.h"

namespace SHLib
{
	/*
		Ŭ���� : CMainThread
		�⺻Ŭ���� : CThread

		������ �ʱ�ȭ, ���Ḧ ����ϴ� ������
		(���Ŀ� Thread�� ���񽺷� ���)
	*/
	class SH_EXPORT CMainThread : public CThread
	{
	private:
		typedef CThread SUPER;
		CMemoryDump	m_Dump; //���� �߻��� ����ϴ� MemoryDump

	public:
		CMainThread();
		virtual ~CMainThread();
		
		virtual HANDLE	Initialize();
		DWORD Start();
		virtual DWORD OnStart() { return 0; }
		INT Main();
	};
}