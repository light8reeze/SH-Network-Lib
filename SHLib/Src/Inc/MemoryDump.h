#ifndef __MEMORYDUMP_H__
#define __MEMORYDUMP_H__
#include "SHLib.h"

namespace SHLib
{
	/*
		Ŭ���� : CMemoryDump
		��Ÿ�ӿ��� ���� �߻��� ���������� �����ϴ� Ŭ����
	*/
	class SH_EXPORT CMemoryDump
	{
	public:
		CMemoryDump();
		~CMemoryDump();

		void SetCallbackException(LPTOP_LEVEL_EXCEPTION_FILTER _pFilter); //���� �߻��� ������ �Լ��� �������ִ� �Լ�
	};
}
#endif