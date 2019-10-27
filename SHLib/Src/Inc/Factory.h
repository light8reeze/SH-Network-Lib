#pragma once
#include "SHLib.h"
#include "ObjectPool.h"
#include "Singleton.h"
#include <typeinfo>
#include <map>

namespace SHLib
{
	class SH_EXPORT CFactory
	{
		SET_SINGLETON_BASE(CFactory);

	private:
		typedef std::size_t							OBJECT_KEY;
		typedef std::map<OBJECT_KEY, IObjectPool*>	POOL_MAP;
		typedef std::pair<OBJECT_KEY, IObjectPool*> MAP_VALUE;

	private:
		#pragma warning(disable : 4251)
		POOL_MAP	m_ObjectPoolList;
		#pragma warning(default : 4251)

	public:
		CFactory();
		~CFactory();

		DWORD Initialize() { return 0; }

		template <typename TObject>
		DWORD CreatePool(WORD _ObjectCount);

		template <typename TObject>
		TObject* New();

		template <typename TObject>
		DWORD Delete(TObject* _pObject);

		void Clear();
	};
}

#include "Factory.inl"