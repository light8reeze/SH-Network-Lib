#pragma once
#include "SHLib.h"
#include "Singleton.h"

namespace SHLib
{
	/*
		클래스 : CDefaultArguments

		서버 설정에 필요한 인자를 받는 인터페이스
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