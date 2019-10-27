#ifndef __MEMORYDUMP_H__
#define __MEMORYDUMP_H__
#include "SHLib.h"

namespace SHLib
{
	/*
		클래스 : CMemoryDump
		런타임에서 에러 발생시 덤프파일을 생성하는 클래스
	*/
	class SH_EXPORT CMemoryDump
	{
	public:
		CMemoryDump();
		~CMemoryDump();

		void SetCallbackException(LPTOP_LEVEL_EXCEPTION_FILTER _pFilter); //예외 발생시 실행할 함수를 설정해주는 함수
	};
}
#endif