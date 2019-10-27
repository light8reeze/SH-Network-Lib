#pragma once
#include "SHLib.h"
#include "Singleton.h"
#include <math.h>

//Ư�� �Ҽ��� �ݿø��� ���ִ� �Լ�
//float���� ������ ���ؼ� ���
#define ROUND(x, digit) (floor((x) * pow(static_cast<float>(10), digit) + 0.5f) / pow(static_cast<float>(10), digit))

namespace SHLib
{
	/*
		Ŭ���� : CRandom

		WELL512�˰����� �̿��Ͽ� ������ �߻���Ų��
		GetTickCount���� �� �� ������ ������ �߻���Ų��.
	*/
	class SH_EXPORT CRandom
	{
		SET_SINGLETON_FINAL(CRandom);

	private:
		DWORD m_dwState[16];
		unsigned int m_nIndex;
		bool m_isSeed;

	public:
		CRandom(void);
		~CRandom(void);

		DWORD Initialize() { SetSeed(); return 0; }
		void SetSeed(int _nSeed);
		void SetSeed();

		DWORD GetNumber();
		DWORD GetNumber(DWORD _dwMin, DWORD _dwMax);
		float GetFloat();
		float GetFloat(float _fMin, float _fMax);
	};
}