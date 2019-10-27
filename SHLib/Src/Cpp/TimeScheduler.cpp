#include "TimeScheduler.h"
#include "AutoLock.h"
#include "NetworkIO.h"
#include "Watch.h"
#include "Logger.h"

namespace SHLib
{
	CTimeScheduler CTimeScheduler::m_Instance;

	DWORD CTimeScheduler::PushEvent(DWORD _dwExecuteTime, DWORD _dwEventNum, CNetworkObject* _pObject, WORD _wSize)
	{
		CTimeElement* pElement = GetElement();
		if (pElement == nullptr)
			return 1;

		pElement->dwEventNum	= _dwEventNum;
		pElement->pObject		= _pObject;

		return PushEvent(_dwExecuteTime, pElement, _wSize);
	}

	DWORD CTimeScheduler::PushEvent(DWORD _dwExecuteTime, CTimeElement* _pElement, WORD _wSize)
	{
		assert(_pElement != nullptr);

		DWORD		dwNowTick = CWatch::GetTime();
		EVENT_VALUE	mapValue(dwNowTick + _dwExecuteTime, _pElement);

		_pElement->wsaBuf.len = _wSize;

		if (_pElement->pObject != nullptr)
			_pElement->pObject->SetState(E_TIMEEVENT);

		{
			AUTO_LOCK(CLocker, m_Locker);
			m_TaskMap.insert(mapValue);
		}
		return 0;
	}

	DWORD CTimeScheduler::PopEvent()
	{
		static EVENT_MAP cpyTaskMap;

		if (m_TaskMap.empty() == true)
			return 0;

		{
			AUTO_LOCK(CLocker, m_Locker);
			cpyTaskMap = m_TaskMap;
		}

		DWORD dwTick	= CWatch::GetTime();
		auto iterLow	= cpyTaskMap.lower_bound(dwTick);

		for (auto iter = cpyTaskMap.begin(); iter != iterLow;)
		{
			assert(iter->first <= dwTick);

			CTimeElement*	pElement = iter->second;
			CNetworkObject*	pObject = pElement->pObject;
			DWORD			dwElemTick = iter->first;
			{
				AUTO_LOCK(CLocker, m_Locker);
				m_TaskMap.erase(dwElemTick);
			}

			iter = cpyTaskMap.erase(iter);
			assert(pObject != nullptr);

			if (pObject->GetStateFlag() == E_NOTCONNECTED)
			{
				ReturnElement(pElement);
				continue;
			}

			BOOL bResult = GetSingleton<CNetworkIO>().PostIOCompletion(reinterpret_cast<IO_KEY>(pElement->pObject), *pElement, pElement->wsaBuf.len);

			if (bResult == FALSE)
				ReturnElement(pElement);
		}

		return 0;
	}
}