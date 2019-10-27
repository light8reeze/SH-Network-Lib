#pragma once
#include <vector>
#include "AutoLock.h"
#include "UserSession.h"
#include "Singleton.h"
#include "Factory.h"
#include "Constraint.h"

namespace SHLib
{
	/*
		클래스 : CSessionManager
		기본 클래스 : CSingletonBase

		CUserSession을 관리하는 클래스(싱글톤 구조)
	*/
	class SH_EXPORT CSessionManager
	{
		SET_SINGLETON_FINAL(CSessionManager);

	private:
		typedef std::vector<CUserSession*>	SESSION_LIST;
		typedef std::queue<CUserSession*>	SESSION_QUEUE;

	private:
		#pragma warning(disable : 4251)
		SESSION_LIST	m_vSessionList; //세션보관 벡터
		SESSION_QUEUE	m_SessionQueue;
		#pragma warning(default : 4251)
		CLocker			m_Locker; //동기화 객체

	protected:
		INT	m_nMaxUser; //서버에서 받는 최대 유저 수
		INT	m_nUser; //서버에 접속한 유저 수

	public:
		CSessionManager();
		~CSessionManager();

		DWORD			Initialize();

		template<typename TSession>
		DWORD			Initialize(INT _nMaxUser);
		
		void			Clear(); //모든 세션을 정리하는 함수
		DWORD			CloseGhost();
		virtual DWORD	OnUserConnect(CUserSession* _pSession);
		virtual DWORD	OnUserDisconnect();

		CUserSession*	GetSession(); 
		int				GetMaxUser(); //최대 유저수를 반환하는 함수
		int				GetUserCount(); //현재 접속한 유저수를 반환하는 함수
		BOOL			IsSessionCreated();
	};
}

#include "SessionManager.Inl"