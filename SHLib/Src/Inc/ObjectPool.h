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
		virtual void Initialize(WORD _nObjectCount) = 0; //�޸�Ǯ�� �����ϴ� �Լ�
	};

	/*
		Ŭ���� : CObjectPool
		�޸𸮸� �̸� �Ҵ��Ͽ� �����ϴ� Ŭ����
	*/
	template <typename TObject>
	class CObjectPool : public IObjectPool
	{
	private:
		typedef std::queue<TObject*>	OBJECT_QUEUE;
		typedef std::vector<TObject*>	OBJECT_POOL;

	private:
		OBJECT_POOL			m_ObjectPool; //�޸� ����
		WORD				m_wUseObject; //������� �޸�Ǯ ũ��
		WORD				m_wMaxCount; //�ִ� �Ҵ�� �޸�Ǯ ũ��
		OBJECT_QUEUE		m_ObjectQueue; //�Ҵ�� �Ѱ��� �޸� �ּ� ����Ʈ
		CLocker				m_Locker; //����ȭ Ŭ����

	public:
		CObjectPool();
		~CObjectPool();

		void		Initialize(WORD _nObjectCount); //�޸�Ǯ�� �����ϴ� �Լ�
		TObject*	New(); //�޸�Ǯ���� �޸𸮸� �Ҵ�޴� �Լ�
		void		Delete(TObject* _pObject); //�޸�Ǯ�� �޸𸮸� �Ҵ� �����ϴ� �Լ�
		BOOL		CanAlloc(); //�޸�Ǯ���� �Ҵ� �������� ��ȯ�ϴ� �Լ�
		BOOL		AddressInPool(TObject* _ptr); //Ư�� �޸��� �ּҰ��� �޸�Ǯ���� �Ҵ�� �ּҰ����� Ȯ���ϴ� �Լ�
		WORD		GetMaxCount(); //�޸�Ǯ�� �ִ� ����� ��ȯ�ϴ� �Լ�
		WORD		GetUseCount(); //�Ҵ�� �޸� ũ�⸦ �����ִ� �Լ�

	private:
		TObject* GetNextObject(); //���� �޸� �ּҸ� ��ȯ�ϴ� �Լ�
	};
}

#include "ObjectPool.inl"