#pragma once
#include "Logger.h"
#include "SHLib.h"
#include "Locker.h"

namespace SHLib
{
	template<typename TLocker>
	class CAutoLock
	{
	private:
		TLocker* m_pLocker;

	public:
		CAutoLock(TLocker* _pLocker) : m_pLocker(nullptr)
		{
			m_pLocker = _pLocker;
			m_pLocker->Lock();
		}

		~CAutoLock()
		{
			assert(m_pLocker != nullptr);
			m_pLocker->Unlock();
		}
	};
}

#define AUTO_LOCK(type, locker) SHLib::CAutoLock<type> cAutoLock(&(locker))