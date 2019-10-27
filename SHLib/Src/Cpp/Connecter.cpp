#include "Connecter.h"
#include "MswSocket.h"
#include "NetworkIO.h"

namespace SHLib
{
	CConnecter::CConnecter() : CNetworkObject(), m_isInitialized(FALSE)
	{
	}

	CConnecter::~CConnecter()
	{
		Close();
	}

	//�������� Ȯ�� �� �ݵ�� super�� ȣ���Ұ�
	DWORD CConnecter::Initialize(const char* _strIP, WORD _nPort)
	{
		SUPER::Initialize();
		SetConnect(_strIP, _nPort);

		return 0;
	}

	//Ŀ������ ������ ����� �ִ� �Լ�
	//char* _strIP : ������ ������ IP
	//int _nPort : ������ ������ Port
	void CConnecter::SetConnect(const char* _strIP, WORD _nPort)
	{
		assert(_strIP != nullptr && strcmp(_strIP, "") != 0);

		CSocket& socket = GetSocket();
		SOCKET connectSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		SOCKADDR_IN connectAddr;

		ZeroMemory(&connectAddr, sizeof(connectAddr));
		connectAddr.sin_family = AF_INET;
		connectAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		connectAddr.sin_port = htons(0);

		int result = bind(connectSocket, (SOCKADDR*)&connectAddr, sizeof(connectAddr));
		if (result != 0)
			return ;

		connectAddr.sin_family = AF_INET;
		connectAddr.sin_addr.s_addr = inet_addr(_strIP);
		connectAddr.sin_port = htons(_nPort);

		socket.SetSocket(connectSocket, connectAddr);
		m_isInitialized = TRUE;
	}

	void CConnecter::Connect()
	{
		CSocket& socket = GetSocket();
		INT nResult = setsockopt(socket.GetSocket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);

		if (nResult == SOCKET_ERROR)
		{
			printf("Connecter Error! : %d\n", WSAGetLastError());
			return ;
		}

		SUPER::Connect();
	}

	//Ŀ������ ������ �����ϴ� �Լ�(����� connect�Լ��� ����Ѵ�)
	//return int: ������ ���� ���� �ʾ��� ������ -2 ��ȯ
	//		  ���� ���н� SOCKET_ERROR(-1)��ȯ
	//		  ������ 0��ȯ
	int CConnecter::StartConnect()
	{
		CSocket& socket = GetSocket();
		HANDLE hResult = GetSingleton<CNetworkIO>().AddIOSocket(reinterpret_cast<IO_KEY>(this), &socket);
		
		if (hResult == NULL)
			return 1;

		BOOL bResult = socket.ConnectAsync(nullptr, 0);

		return 0;
	}
}