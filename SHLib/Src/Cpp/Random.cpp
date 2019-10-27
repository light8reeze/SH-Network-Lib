#include "Random.h"
#include <time.h>
#include <math.h>

namespace SHLib
{
	CRandom CRandom::m_Instance;

	CRandom::CRandom() : m_dwState(), m_nIndex(0), m_isSeed(false)
	{
	}

	CRandom::~CRandom()
	{
	}

	void CRandom::SetSeed()
	{
		for (int index = 0; index < 16; ++index)
			m_dwState[index] = (DWORD)time(NULL) + GetTickCount();
	}

	void CRandom::SetSeed(int _nSeed)
	{
		for (int index = 0; index < 16; ++index)
			m_dwState[index] = _nSeed + GetTickCount();
	}

	//Well512 알고리즘을 이용하여 난수 생성해주는 함수
	DWORD CRandom::GetNumber()
	{
		//시드값이 없으면 자동으로 생성
		if (m_isSeed == false)
		{
			SetSeed();
			m_isSeed = true;
		}

		DWORD a, b, c, d;

		a = m_dwState[m_nIndex];
		c = m_dwState[(m_nIndex + 13) & 15];
		b = a ^ c ^ (a << 16) ^ (c << 15);
		c = m_dwState[(m_nIndex + 9) & 15];
		c ^= (c >> 11);
		a = m_dwState[m_nIndex] = b ^ c;
		d = a ^ ((a << 5) & 0xDA442D20UL);
		m_nIndex = (m_nIndex + 15) & 15;
		a = m_dwState[m_nIndex];
		m_dwState[m_nIndex] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return m_dwState[m_nIndex];
	}

	//_dwMin ~ _dwMax 까지의 난수를 생성해주는 함수
	DWORD CRandom::GetNumber(DWORD _dwMin, DWORD _dwMax)
	{
		return GetNumber() % (_dwMax - _dwMin + 1) + _dwMin;
	}

	//0.0 ~ 1.0까지의 소수를 반환하는 함수
	float CRandom::GetFloat()
	{
		DWORD dwRandom = GetNumber();

		union { DWORD dw; float f; } result;
		result.dw = (((dwRandom *= 16807) & 0x007fffff) - 1) | 0x3f800000;

		return (result.f - 1.0f);
	}

	//_fMin ~ _fMax 까지의 소수를 반환하는 함수
	float CRandom::GetFloat(float _fMin, float _fMax)
	{
		float fResult = (float)(GetFloat() * (ROUND(_fMax, 2) - ROUND(_fMin, 2)) + ROUND(_fMin, 2));
		return fResult;
	}
}