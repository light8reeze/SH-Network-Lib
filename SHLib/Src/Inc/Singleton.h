#pragma once

/*
	Warnning : �̱��� Ȯ���� �� Ŭ������ SET_SINGLETON_BASE ��ũ�θ�
	Ȯ���� ���� ���� �� SINGLETON_FINAL ��ũ�θ� ����Ѵ�.

	DLL�и��� ���μ����� �и��Ǿ� static�� �������� �����Ƿ�
	DLL���ο����� ����� �̱����� DLL���ο����� ����, ����ϰ�
	exe, DLL��� ����� �̱����� exe���� �����Ͽ� ���� Ŭ������ �����Ϳ� �ִ¹������ ����Ѵ�.
*/
#define SET_SINGLETON_FINAL(type)	private:							\
									static type m_Instance;				\
									public:								\
									static type& GetInstance()			\
									{									\
										return m_Instance;				\
									}

#ifdef _DEBUG
#include "Logger.h"
#endif
/*
	Warnning : �̱��� Ȯ���� �� Ŭ������ SET_SINGLETON_BASE ��ũ�θ�
	Ȯ���� ���� ���� �� SINGLETON_FINAL ��ũ�θ� ����Ѵ�.

	DLL�и��� ���μ����� �и��Ǿ� static�� �������� �����Ƿ�
	DLL���ο����� ����� �̱����� DLL���ο����� ����, ����ϰ�
	exe, DLL��� ����� �̱����� exe���� �����Ͽ� ���� Ŭ������ �����Ϳ� �ִ¹������ ����Ѵ�.

	SET_SINGLETON_BASE ��ũ�θ� ����� Ŭ������ ��� �����ڿ��� m_pInstance = this;
	������ �ݵ�� �ִ´�.

	���� Ŭ�������� �ݵ�� SET_SINGLETON_FINAL�� ����Ͽ� Ÿ���� Ȯ���Ѵ�.
*/
#ifdef _DEBUG
#define SET_SINGLETON_BASE(type)	private:										\
									static type* m_pInstance;						\
									public:											\
									static type& GetInstance()						\
									{												\
										if(m_pInstance == nullptr)					\
										{											\
											SHLib::Trace(#type" Create Auto!\n");	\
											m_pInstance = new type();				\
										}											\
										return (*m_pInstance);						\
									}
#else
#define SET_SINGLETON_BASE(type)	private:							\
									static type* m_pInstance;			\
									public:								\
									static type& GetInstance()			\
									{									\
										if(m_pInstance == nullptr)		\
											m_pInstance = new type();	\
										return (*m_pInstance);			\
									}
#endif

namespace SHLib
{
	//�̱��� �ν��Ͻ��� TSingleton������ ��ȯ�ϴ� �Լ�
	//<typename TSingleton> : �ν��Ͻ��� ���� Ŭ����
	//return TSingleton* : �̱��� �ν��Ͻ��� ��ȯ
	template<typename TSingleton>
	inline TSingleton& GetSingleton()
	{
		//TSingleton�� ISingleton�� �Ļ�Ŭ�������� �����ϴܰ迡�� �˻�
		return TSingleton::GetInstance();
	}
}