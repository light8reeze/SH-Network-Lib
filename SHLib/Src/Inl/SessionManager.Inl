namespace SHLib
{
	template<typename TSession>
	DWORD CSessionManager::Initialize(INT _nMaxUser)
	{
		if (m_nMaxUser > 0)
			return 0;

		CConstraint<TSession, CUserSession> c;

		m_nMaxUser = _nMaxUser;
		m_vSessionList.reserve(_nMaxUser);
		GetSingleton<CFactory>().CreatePool<TSession>(m_nMaxUser);

		for (int index = 0; index < _nMaxUser; ++index)
		{
			TSession* pSession = GetSingleton<CFactory>().New<TSession>();

			assert(pSession != nullptr);
			pSession->OnDestroy();
			m_SessionQueue.push(pSession);
		}

		return Initialize();
	}

	//�ִ� �������� ��ȯ�ϴ� �Լ�
	//return int : ���� ������ ������ �ִ� ������
	inline int CSessionManager::GetMaxUser()
	{
		return m_nMaxUser;
	}

	//���� ������ �������� ��ȯ�ϴ� �Լ�
	//return int : ���� ���ǿ� �����ϴ� ���� ��
	inline int CSessionManager::GetUserCount()
	{
		return m_nUser;
	}

	inline BOOL CSessionManager::IsSessionCreated()
	{
		return (m_nMaxUser > 0) ? TRUE : FALSE;
	}
}