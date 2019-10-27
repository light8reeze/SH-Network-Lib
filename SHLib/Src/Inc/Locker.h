#pragma once
#include "SHLib.h"

namespace SHLib
{
	/*
		Ŭ���� : CLocker

		����ȭ�� ����ϴ� Ŭ����(Critical Section�̿�)
	*/
	class SH_EXPORT CLocker
	{
	private:
		CRITICAL_SECTION m_CS;

	public:
		CLocker();
		~CLocker();

		void Clear(); //Critical Section�� �����ϴ� �Լ�
		void Lock(); //Critical Section�����ϴ� �Լ�
		void Unlock(); //Critical Section�����ϴ� �Լ�
		BOOL Try();
	};
}