#pragma once
#include "Constraint.h"
#include "AutoLock.h"
#include "SHLib.h"
#include <queue>

namespace SHLib
{
	class SH_EXPORT IObjectPool
	{
	public:
		IObjectPool() {}
		IObjectPool(const IObjectPool& _copy) = delete;
		IObjectPool& operator=(const IObjectPool& _copy) = delete;

		virtual ~IObjectPool() {};
		virtual void Initialize(WORD _nObjectCount) = 0; //메모리풀을 설정하는 함수
	};

	/*
		클래스 : CObjectPool
		메모리를 미리 할당하여 관리하는 클래스
	*/
	template <typename TObject>
	class CObjectPool : public IObjectPool
	{
	private:
		typedef std::queue<TObject*>	OBJECT_QUEUE;
		typedef std::vector<TObject*>	OBJECT_POOL;

	private:
		OBJECT_POOL			m_ObjectPool; //메모리 버퍼
		WORD				m_wUseObject; //사용중인 메모리풀 크기
		WORD				m_wMaxCount; //최대 할당된 메모리풀 크기
		OBJECT_QUEUE		m_ObjectQueue; //할당시 넘겨줄 메모리 주소 리스트
		CLocker				m_Locker; //동기화 클래스

	public:
		CObjectPool();
		~CObjectPool();

		void		Initialize(WORD _nObjectCount); //메모리풀을 설정하는 함수
		TObject*	New(); //메모리풀에서 메모리를 할당받는 함수
		void		Delete(TObject* _pObject); //메모리풀에 메모리를 할당 해제하는 함수
		BOOL		CanAlloc(); //메모리풀에서 할당 가능한지 반환하는 함수
		BOOL		AddressInPool(TObject* _ptr); //특정 메모리의 주소값이 메모리풀에서 할당된 주소값인지 확인하는 함수
		WORD		GetMaxCount(); //메모리풀의 최대 사이즈를 반환하는 함수
		WORD		GetUseCount(); //할당된 메모리 크기를 구해주는 함수

	private:
		TObject* GetNextObject(); //다음 메모리 주소를 반환하는 함수
	};
}

#include "ObjectPool.inl"