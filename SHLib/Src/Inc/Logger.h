#pragma once
#include "SHLib.h"
#include "Singleton.h"
#include <stdio.h>

namespace SHLib
{
	class SH_EXPORT CLogger
	{
		SET_SINGLETON_FINAL(CLogger);

	private:
		FILE* m_pFile = nullptr;

	public:
		CLogger();
		~CLogger();

		DWORD Initialize() { return 0; }
		void Print(const char* _pLog, ...);
	};

#ifdef _DEBUG
	inline void Trace(const char* lpszFormat, ...)
	{
		char lpszBuffer[0x160]; //버퍼 크기.
		va_list fmtList;
		va_start(fmtList, lpszFormat);
		vsprintf_s(lpszBuffer, 0x160, lpszFormat, fmtList);
		va_end(fmtList);
		OutputDebugStringA(lpszBuffer);
	}
#endif
}