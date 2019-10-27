#pragma once
#include "SHLib.h"
#include "Singleton.h"

namespace SHLib
{
	/*
		Ŭ���� : CDefaultArguments

		���� ������ �ʿ��� ���ڸ� �޴� �������̽�
	*/
	class SH_EXPORT CDefaultArguments
	{
		SET_SINGLETON_BASE(CDefaultArguments);

	public:
		CDefaultArguments();
		virtual ~CDefaultArguments() {}

		virtual DWORD		Initialize() { return 0; }
		virtual const char* GetIPAddress(WORD _wIndex);
		virtual WORD		GetPort(WORD _wIndex);
		virtual INT			GetSessionMax();
		virtual INT			GetTimeEventMax();
		virtual INT			GetWorkThreadCnt();
		virtual INT			GetAcceptThreadCnt(WORD _wIndex);
		virtual INT			GetAcceptorCnt();
	};
}