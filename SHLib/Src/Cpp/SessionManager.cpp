#include "SessionManager.h"

namespace SHLib
{
	CSessionManager CSessionManager::m_Instance;

	CSessionManager::CSessionManager() : m_vSessionList(), m_SessionQueue(), m_Locker(), 
		m_nMaxUser(0), m_nUser(0)
	{
	}

	CSessionManager::~CSessionManager()
	{
		Clear();
	}

	DWORD CSessionManager::Initialize()
	{
		assert(m_nMaxUser > 0);
		return 0;
	}

	//모든 세션을 정리하는 함수
	void CSessionManager::Clear()
	{
		AUTO_LOCK(CLocker, m_Locker);

		CUserSession* temp = nullptr;
		for (size_t index = 0; index < m_vSessionList.size(); ++index)
		{
			temp = m_vSessionList[index];
			--m_nUser;
			SafeDelete<CUserSession>(temp);
		}

		m_vSessionList.clear();
	}

	DWORD CSessionManager::CloseGhost()
	{
		CUserSession* pSession = nullptr;

		AUTO_LOCK(CLocker, m_Locker);

		for (auto cIter = m_vSessionList.cbegin(); cIter != m_vSessionList.cend();)
		{
			pSession = *cIter;

			if (pSession != nullptr && pSession->GetStateFlag() == (E_DISCONNECT | E_CONNECT))
			{
				cIter = m_vSessionList.erase(cIter);
				pSession->Close();
				m_SessionQueue.push(pSession);
			}
			else
				++cIter;
		}
		return 0;
	}

	DWORD CSessionManager::OnUserConnect(CUserSession* _pSession)
	{
		AUTO_LOCK(CLocker, m_Locker);
		++m_nUser;
		m_vSessionList.push_back(_pSession);

		return 0;
	}

	DWORD CSessionManager::OnUserDisconnect()
	{
		AUTO_LOCK(CLocker, m_Locker);
		--m_nUser;

		return 0;
	}

	//삭제상태의 세션을 반환하는 함수
	//return CUserSession* : 새로운 세션의 주소값
	CUserSession* CSessionManager::GetSession()
	{
		CUserSession* pSession = nullptr;
		{
			AUTO_LOCK(CLocker, m_Locker);

			if (m_SessionQueue.empty())
				return nullptr;

			pSession = m_SessionQueue.front();
			m_SessionQueue.pop();
		}

		if (pSession == nullptr)
			return nullptr;

		pSession->Initialize();
		pSession->SetStateFlag(E_IDLE);

		return pSession;
	}
}