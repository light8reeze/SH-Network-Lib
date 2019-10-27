namespace SHLib
{
	inline CFactory::CFactory() : m_ObjectPoolList()
	{
		m_pInstance = this;
	}

	inline CFactory::~CFactory()
	{
		Clear();
	}

	template<typename TObject>
	DWORD CFactory::CreatePool(WORD _ObjectCount)
	{
		using TObjectPool = CObjectPool<TObject>;

		TObjectPool* pPool = nullptr;
		OBJECT_KEY objectKey = 0;

		try
		{
			objectKey = typeid(TObject).hash_code();

			if (m_ObjectPoolList.find(objectKey) != m_ObjectPoolList.end())
			{
				assert(FALSE);
				return 1;
			}
		}
		catch (std::exception)
		{
			assert(FALSE);
			return 2;
		}

		pPool = new TObjectPool;
		pPool->Initialize(_ObjectCount);
		m_ObjectPoolList.insert(MAP_VALUE(objectKey, pPool));
		return 0;
	}

	template<typename TObject>
	TObject* CFactory::New()
	{
		using TObjectPool = CObjectPool<TObject>;

		TObjectPool* pPool = nullptr;
		TObject* pObject = nullptr;
		OBJECT_KEY objectKey = 0;

		try
		{
			objectKey = typeid(TObject).hash_code();
		}
		catch (std::exception)
		{
			assert(FALSE);
			return nullptr;
		}

		assert(objectKey != 0);

		auto iter = m_ObjectPoolList.find(objectKey);
		if (iter == m_ObjectPoolList.end())
			return nullptr;

		pPool = static_cast<TObjectPool*>(iter->second);
		assert(pPool != nullptr);

		pObject = pPool->New();
		return pObject;
	}

	template<typename TObject>
	DWORD CFactory::Delete(TObject* _pObject)
	{
		using TObjectPool = CObjectPool<TObject>;
		assert(_pObject != nullptr);

		TObjectPool* pPool = nullptr;
		OBJECT_KEY objectKey = 0;

		try
		{
			objectKey = typeid(TObject).hash_code();
		}
		catch (std::exception)
		{
			assert(FALSE);
			return 2;
		}

		assert(objectKey != 0);

		auto iter = m_ObjectPoolList.find(objectKey);
		if (iter == m_ObjectPoolList.end())
			return 1;

		pPool = static_cast<TObjectPool*>(iter->second);
		assert(pPool != nullptr);

		if (pPool->AddressInPool(_pObject) == FALSE)
		{
			assert(FALSE);
			return 1;
		}

		pPool->Delete(_pObject);
		return 0;
	}

	inline void CFactory::Clear()
	{
		IObjectPool* pPool = nullptr;

		auto iter = m_ObjectPoolList.begin();
		while (iter != m_ObjectPoolList.end())
		{
			pPool = iter->second;
			iter = m_ObjectPoolList.erase(iter);
			SafeDelete<IObjectPool>(pPool);
		}

		assert(m_ObjectPoolList.size() == 0);
	}
}