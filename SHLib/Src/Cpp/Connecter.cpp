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

	//하위에서 확장 시 반드시 super를 호출할것
	DWORD CConnecter::Initialize(const char* _strIP, WORD _nPort)
	{
		SUPER::Initialize();
		SetConnect(_strIP, _nPort);

		return 0;
	}

	//커넥터의 소켓을 만들어 주는 함수
	//char* _strIP : 연결할 서버의 IP
	//int _nPort : 연결할 서버의 Port
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

	//커넥터의 연결을 시작하는 함수(동기식 connect함수를 사용한다)
	//return int: 소켓이 설정 되지 않았을 때에는 -2 반환
	//		  연결 실패시 SOCKET_ERROR(-1)반환
	//		  성공시 0반환
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