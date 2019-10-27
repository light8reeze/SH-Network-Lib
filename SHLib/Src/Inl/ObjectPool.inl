#include "MainThread.h"

namespace SHLib
{
	template <typename TObject>
	CObjectPool<TObject>::CObjectPool() : m_ObjectPool(),
		m_wUseObject(0), m_wMaxCount(0), m_ObjectQueue(), m_Locker()
	{
	}

	template <typename TObject>
	CObjectPool<TObject>::~CObjectPool()
	{
		while (m_ObjectQueue.empty() == false)
			m_ObjectQueue.pop();

		auto iter = m_ObjectPool.begin();
		while (iter != m_ObjectPool.end())
		{
			TObject* pObject = (*iter);
			iter = m_ObjectPool.erase(iter);
			SafeDelete<TObject>(pObject);
		}
	}

	//메모리풀을 설정하는 함수
	//int _nUnitSize : 메모리 단위 크기
	template <typename TObject>
	void CObjectPool<TObject>::Initialize(WORD _nObjectCount)
	{
		assert(m_ObjectPool.size() == 0);
		m_ObjectPool.reserve(_nObjectCount);
		m_wMaxCount = _nObjectCount;

		for (int index = 0; index < _nObjectCount; ++index)
		{
			TObject* pObject = new TObject();
			m_ObjectPool.push_back(pObject);
			m_ObjectQueue.push(pObject);
		}
	}

	//메모리풀에서 메모리를 할당받는 함수
	//return char* : 사용할 메모리의 주소값
	template <typename TObject>
	TObject* CObjectPool<TObject>::New()
	{
		TObject* pObject = GetNextObject();
		if (pObject == nullptr)
			return nullptr;
		
		pObject->OnConstruct();
		return pObject;
	}

	//메모리풀에 메모리를 할당 해제하는 함수
	//char* _pMemory : 사용 해제할 메모리 주소값
	template <typename TObject>
	void CObjectPool<TObject>::Delete(TObject* _pObject)
	{
		if (_pObject == nullptr)
			return;

		{
			AUTO_LOCK(CLocker, m_Locker);
			_pObject->OnDestroy();
			m_ObjectQueue.push(_pObject);
			--m_wUseObject;
		}
	}

	//다음 메모리 주소를 반환하는 함수
	//return char* : 다음 메모리 주소
	template <typename TObject>
	TObject* CObjectPool<TObject>::GetNextObject()
	{

		TObject* pObject = nullptr;
		//Lock
		{
			AUTO_LOCK(CLocker, m_Locker);

			if (CanAlloc() == false)
				return nullptr;

			pObject = m_ObjectQueue.front();
			m_ObjectQueue.pop();
			++m_wUseObject;
		}

		return pObject;
	}

	//메모리풀에서 할당 가능한지 반환하는 함수
	//return bool : 메모리풀에서 할당 가능한지 여부
	template <typename TObject>
	BOOL CObjectPool<TObject>::CanAlloc()
	{
		return (m_ObjectQueue.empty() == true)? FALSE : TRUE;
	}

	//특정 메모리의 주소값이 메모리풀에서 할당된 주소값인지 확인하는 함수
	//char* _ptr : 확인할 메모리의 주소값
	//return bool : 메모리풀의 메모리 주소값인지 여부
	template <typename TObject>
	BOOL CObjectPool<TObject>::AddressInPool(TObject* _ptr)
	{
		if (std::find(m_ObjectPool.begin(), m_ObjectPool.end(), _ptr) == m_ObjectPool.end())
			return FALSE;
		
		return TRUE;
	}

	//메모리풀의 최대 사이즈를 반환하는 함수
	//return int : 메모리풀의 최대 크기
	template <typename TObject>
	WORD CObjectPool<TObject>::GetMaxCount()
	{
		return m_wMaxCount;
	}

	template <typename TObject>
	WORD CObjectPool<TObject>::GetUseCount()
	{
		return m_wUseObject;
	}
}