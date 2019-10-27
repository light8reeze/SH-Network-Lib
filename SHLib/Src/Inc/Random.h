#pragma once
#include "SHLib.h"
#include "Singleton.h"
#include <math.h>

//특정 소수점 반올림을 해주는 함수
//float값의 고정을 위해서 사용
#define ROUND(x, digit) (floor((x) * pow(static_cast<float>(10), digit) + 0.5f) / pow(static_cast<float>(10), digit))

namespace SHLib
{
	/*
		클래스 : CRandom

		WELL512알고리즘을 이용하여 난수를 발생시킨다
		GetTickCount보다 더 고른 분포의 난수를 발생시킨다.
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