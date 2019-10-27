#include "Locker.h"
#include "Logger.h"

namespace SHLib
{
	inline CLocker::CLocker()
	{
		BOOL bResult = InitializeCriticalSectionAndSpinCount(&m_CS, 4000);
		assert(bResult);
	}

	inline CLocker::~CLocker()
	{
		Clear();
	}

	//Critical Section을 정리하는 함수
	inline void CLocker::Clear()
	{
		DeleteCriticalSection(&m_CS);
	}

	//Critical Section입장하는 함수
	inline void CLocker::Lock()
	{
		EnterCriticalSection(&m_CS);
	}

	//Critical Section퇴장하는 함수
	inline void CLocker::Unlock()
	{
		LeaveCriticalSection(&m_CS);
	}

	inline BOOL CLocker::Try()
	{
		return TryEnterCriticalSection(&m_CS);
	}
}