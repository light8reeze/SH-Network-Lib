#include "NetworkIO.h"
#include "SessionManager.h"
#include "Acceptor.h"
#include "MainThread.h"
#include <list>

namespace SHLib
{
	CAcceptor::CAcceptor() : CThread(), m_AcceptSocket()
	{
	}

	CAcceptor::~CAcceptor()
	{
		Clear();
	}

	HANDLE CAcceptor::Initialize(const char* _strIP, int _nPort)
	{
		int result;
		SOCKET sListenSocket;
		SOCKADDR_IN sockAddr;

		sListenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

		ZeroMemory(&sockAddr, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = (_strIP == nullptr) ? htonl(INADDR_ANY) : inet_addr(_strIP);
		sockAddr.sin_port = htons(_nPort);

		result = bind(sListenSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

		result = listen(sListenSocket, 50);

		m_AcceptSocket.SetSocket(sListenSocket, sockAddr);

		printf("Start Acceptor IP : %s, Port : %d\n", _strIP, _nPort);

		return SUPER::Initialize();
	}

	HANDLE CAcceptor::Initialize(CSocket& _socket)
	{
		m_AcceptSocket = _socket;
		return SUPER::Initialize();
	}

	//서버로 들어오는 RunBehaviour를 실행하는 함수(쓰레드 사용)(동기식 RunBehaviour를 사용)
	INT CAcceptor::Main()
	{
		assert(m_AcceptSocket.GetSocket() != NULL);
		assert(GetSingleton<CSessionManager>().IsSessionCreated());
		
		CUserSession*		pSession = nullptr;
		HANDLE				hResult = NULL;
		CNetworkIO&			pNetworkIO = GetSingleton<CNetworkIO>();
		DWORD				dwResult = 0;

		TODO_MODIFY_ERRORCODE;
		while (TRUE)
		{
			hResult		= NULL;
			pSession	= GetSingleton<CSessionManager>().GetSession();

			if (pSession == nullptr)
			{
				Sleep(1);
				continue;
			}

			assert(pSession->GetStateFlag() == E_IDLE);

			pSession->SetState(E_ACCEPT);
			dwResult = m_AcceptSocket.AcceptSync(pSession->GetSocket());

			if (dwResult != 0)
			{
				printf("Accept Error!\n");
				continue;
			}

			hResult = pNetworkIO.AddIOSocket(reinterpret_cast<IO_KEY>(pSession), &(pSession->GetSocket()));

			if (hResult == NULL)
			{
				printf("IOPort Add Socket Error! : %d\n", GetLastError());
				continue;
			}

			BOOL bResult = pNetworkIO.PostIOCompletion(reinterpret_cast<IO_KEY>(pSession), pSession->GetSocket().GetOverlapped(E_IO_ACCEPT), 1);
			if (bResult == FALSE)
			{
				pSession->ResetState(E_ACCEPT);
				pSession->SetClose();
			}

			pSession = nullptr;
		}

		return 0;
	}

	//Acceptor을 종료하는 함수
	void CAcceptor::Clear()
	{
		m_AcceptSocket.Close();
	}
}