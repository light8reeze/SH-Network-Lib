#include "Arguments.h"

namespace SHLib
{
	CDefaultArguments* CDefaultArguments::m_pInstance = nullptr;

	CDefaultArguments::CDefaultArguments()
	{
		m_pInstance = this;
	}

	const char* CDefaultArguments::GetIPAddress(WORD _wIndex)
	{
		return nullptr;
	}

	WORD CDefaultArguments::GetPort(WORD _wIndex)
	{
		return 4832;
	}

	INT CDefaultArguments::GetSessionMax()
	{
		return 5000;
	}

	INT CDefaultArguments::GetTimeEventMax()
	{
		return 10000;
	}

	INT CDefaultArguments::GetWorkThreadCnt()
	{
		return 10;
	}

	INT CDefaultArguments::GetAcceptThreadCnt(WORD _wIndex)
	{
		return 10;
	}

	INT CDefaultArguments::GetAcceptorCnt()
	{
		return 1;
	}
}