#pragma once

/*
	Warnning : 싱글톤 확장을 할 클래스는 SET_SINGLETON_BASE 매크로를
	확장을 하지 않을 시 SINGLETON_FINAL 매크로를 사용한다.

	DLL분리시 프로세스가 분리되어 static이 공유되지 않으므로
	DLL내부에서만 사용할 싱글톤은 DLL내부에서만 생성, 사용하고
	exe, DLL모두 사용할 싱글톤은 exe에서 생성하여 상위 클래스의 포인터에 넣는방식으로 사용한다.
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
	Warnning : 싱글톤 확장을 할 클래스는 SET_SINGLETON_BASE 매크로를
	확장을 하지 않을 시 SINGLETON_FINAL 매크로를 사용한다.

	DLL분리시 프로세스가 분리되어 static이 공유되지 않으므로
	DLL내부에서만 사용할 싱글톤은 DLL내부에서만 생성, 사용하고
	exe, DLL모두 사용할 싱글톤은 exe에서 생성하여 상위 클래스의 포인터에 넣는방식으로 사용한다.

	SET_SINGLETON_BASE 매크로를 사용한 클래스의 경우 생성자에서 m_pInstance = this;
	문장을 반드시 넣는다.

	하위 클래스에서 반드시 SET_SINGLETON_FINAL을 사용하여 타입을 확정한다.
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
	//싱글톤 인스턴스를 TSingleton형으로 반환하는 함수
	//<typename TSingleton> : 인스턴스를 받을 클래스
	//return TSingleton* : 싱글톤 인스턴스를 반환
	template<typename TSingleton>
	inline TSingleton& GetSingleton()
	{
		//TSingleton가 ISingleton의 파생클래스인지 컴파일단계에서 검사
		return TSingleton::GetInstance();
	}
}