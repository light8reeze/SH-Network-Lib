#pragma once
#include "NetworkObject.h"
#include "Singleton.h"
#include "Factory.h"
#include <iostream>
#include <queue>
#include <memory>

namespace SHLib
{
	enum TIME_ELEMENT
	{
		E_TIMER_BUFFER = 1024
	};

	class CTimeElement : public COverlapped
	{
	public:
		CTimeElement();
		~CTimeElement() {}

		void OnConstruct();
		void OnDestroy();

	public:
		DWORD			dwEventNum;		//이벤트 번호
		CNetworkObject* pObject;		//이벤트 오브젝트
		char			pBuffer[E_TIMER_BUFFER];
	};

	class SH_EXPORT CTimeScheduler
	{
		SET_SINGLETON_FINAL(CTimeScheduler);

	private:
		typedef std::map<DWORD, CTimeElement*>	EVENT_MAP; //Key : Tick, Value : TimeElement
		typedef std::pair<DWORD, CTimeElement*>	EVENT_VALUE;

	private:
		#pragma warning(disable : 4251)
		EVENT_MAP	m_TaskMap;
		#pragma warning(default : 4251)
		CLocker		m_Locker;

	public:
		CTimeScheduler() : m_TaskMap(), m_Locker() {}
		~CTimeScheduler() {}

		DWORD Initialize();
		DWORD Initialize(INT _nMax);
		DWORD PushEvent(DWORD _dwExecuteTime, DWORD _dwEventNum, CNetworkObject* _pObject, WORD _wSize = 0);
		DWORD PushEvent(DWORD _dwExecuteTime, CTimeElement* _pElement, WORD _wSize = 0);
		DWORD PopEvent();

		CTimeElement*	GetElement();
		void			ReturnElement(CTimeElement*& _pElement);
	};
}

#include "TimeScheduler.inl"