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

	//�޸�Ǯ�� �����ϴ� �Լ�
	//int _nUnitSize : �޸� ���� ũ��
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

	//�޸�Ǯ���� �޸𸮸� �Ҵ�޴� �Լ�
	//return char* : ����� �޸��� �ּҰ�
	template <typename TObject>
	TObject* CObjectPool<TObject>::New()
	{
		TObject* pObject = GetNextObject();
		if (pObject == nullptr)
			return nullptr;
		
		pObject->OnConstruct();
		return pObject;
	}

	//�޸�Ǯ�� �޸𸮸� �Ҵ� �����ϴ� �Լ�
	//char* _pMemory : ��� ������ �޸� �ּҰ�
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

	//���� �޸� �ּҸ� ��ȯ�ϴ� �Լ�
	//return char* : ���� �޸� �ּ�
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

	//�޸�Ǯ���� �Ҵ� �������� ��ȯ�ϴ� �Լ�
	//return bool : �޸�Ǯ���� �Ҵ� �������� ����
	template <typename TObject>
	BOOL CObjectPool<TObject>::CanAlloc()
	{
		return (m_ObjectQueue.empty() == true)? FALSE : TRUE;
	}

	//Ư�� �޸��� �ּҰ��� �޸�Ǯ���� �Ҵ�� �ּҰ����� Ȯ���ϴ� �Լ�
	//char* _ptr : Ȯ���� �޸��� �ּҰ�
	//return bool : �޸�Ǯ�� �޸� �ּҰ����� ����
	template <typename TObject>
	BOOL CObjectPool<TObject>::AddressInPool(TObject* _ptr)
	{
		if (std::find(m_ObjectPool.begin(), m_ObjectPool.end(), _ptr) == m_ObjectPool.end())
			return FALSE;
		
		return TRUE;
	}

	//�޸�Ǯ�� �ִ� ����� ��ȯ�ϴ� �Լ�
	//return int : �޸�Ǯ�� �ִ� ũ��
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