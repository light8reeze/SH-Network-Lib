#pragma once
#include "SHLib.h"

namespace SHLib
{
	/*
		클래스 : CLocker

		동기화를 담당하는 클래스(Critical Section이용)
	*/
	class SH_EXPORT CLocker
	{
	private:
		CRITICAL_SECTION m_CS;

	public:
		CLocker();
		~CLocker();

		void Clear(); //Critical Section을 정리하는 함수
		void Lock(); //Critical Section입장하는 함수
		void Unlock(); //Critical Section퇴장하는 함수
		BOOL Try();
	};
}