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
		Ŭ���� : CSessionManager
		�⺻ Ŭ���� : CSingletonBase

		CUserSession�� �����ϴ� Ŭ����(�̱��� ����)
	*/
	class SH_EXPORT CSessionManager
	{
		SET_SINGLETON_FINAL(CSessionManager);

	private:
		typedef std::vector<CUserSession*>	SESSION_LIST;
		typedef std::queue<CUserSession*>	SESSION_QUEUE;

	private:
		#pragma warning(disable : 4251)
		SESSION_LIST	m_vSessionList; //���Ǻ��� ����
		SESSION_QUEUE	m_SessionQueue;
		#pragma warning(default : 4251)
		CLocker			m_Locker; //����ȭ ��ü

	protected:
		INT	m_nMaxUser; //�������� �޴� �ִ� ���� ��
		INT	m_nUser; //������ ������ ���� ��

	public:
		CSessionManager();
		~CSessionManager();

		DWORD			Initialize();

		template<typename TSession>
		DWORD			Initialize(INT _nMaxUser);
		
		void			Clear(); //��� ������ �����ϴ� �Լ�
		DWORD			CloseGhost();
		virtual DWORD	OnUserConnect(CUserSession* _pSession);
		virtual DWORD	OnUserDisconnect();

		CUserSession*	GetSession(); 
		int				GetMaxUser(); //�ִ� �������� ��ȯ�ϴ� �Լ�
		int				GetUserCount(); //���� ������ �������� ��ȯ�ϴ� �Լ�
		BOOL			IsSessionCreated();
	};
}

#include "SessionManager.Inl"