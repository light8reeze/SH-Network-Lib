namespace SHLib
{
	inline CTimeElement::CTimeElement() : COverlapped()
	{ 
		OnConstruct(); 
	}
	
	inline void CTimeElement::OnConstruct()
	{
		nIOMode		= E_IO_TIMER;
		dwEventNum	= 0;
		pObject		= nullptr;
		wsaBuf.buf	= pBuffer;
		ZeroMemory(pBuffer, E_TIMER_BUFFER);
	}

	inline void CTimeElement::OnDestroy()
	{
		dwEventNum	= 0;
		pObject		= nullptr;
		wsaBuf.len	= 0;
		ZeroMemory(pBuffer, E_TIMER_BUFFER);
	}

	inline DWORD CTimeScheduler::Initialize()
	{
		assert(0);
		return 0;
	}

	inline DWORD CTimeScheduler::Initialize(INT _nMax)
	{
		return GetSingleton<CFactory>().CreatePool<CTimeElement>(_nMax);
	}

	inline CTimeElement* CTimeScheduler::GetElement()
	{
		return GetSingleton<CFactory>().New<CTimeElement>();
	}

	inline void CTimeScheduler::ReturnElement(CTimeElement*& _pElement)
	{
		GetSingleton<CFactory>().Delete(_pElement);
		_pElement = nullptr;
	}
}