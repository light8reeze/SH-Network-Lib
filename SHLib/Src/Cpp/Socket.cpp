#include "Socket.h"
#include "MswSocket.h"
#include "Logger.h"
#include <mstcpip.h>

namespace SHLib
{
	CSocket::CSocket() : m_Socket(0), m_SockAddr(),
		m_isUseable(false), m_mOverlappedList(), m_strIP()
	{
		ZeroMemory(m_strIP, 20);
	}

	CSocket::~CSocket()
	{
		Close();
	}

	DWORD CSocket::Initialize()
	{
		for (int index = 0; index < E_IO_COUNT; ++index)
			m_mOverlappedList[index].nIOMode = static_cast<IO_CODE>(index);

		return 0;
	}

	CSocket* CSocket::AcceptSync()
	{
		CSocket* pSocket = new CSocket;

		AcceptSync(*pSocket);

		return pSocket;
	}

	DWORD CSocket::AcceptSync(CSocket& _socket)
	{
		assert(m_Socket != NULL);

		SOCKET socket = NULL;
		SOCKADDR_IN sockAddr = _socket.GetSockAddr();
		int size = sizeof(sockAddr);

		socket = accept(m_Socket, reinterpret_cast<SOCKADDR*>(&sockAddr), &size);
		_socket.SetSocket(socket);
		_socket.SetSockAddr(sockAddr);

		if (socket == INVALID_SOCKET)
			return 1;

		return 0;
	}

	BOOL CSocket::ConnectAsync(char* _pData, DWORD _dwLength)
	{
		COverlapped& overlapped = m_mOverlappedList[E_IO_CONNECT];

		ZeroMemory(&overlapped, sizeof(OVERLAPPED));

		overlapped.wsaBuf.buf = _pData;
		overlapped.wsaBuf.len = _dwLength;

		BOOL bResult = CMswSocket::ConnectEx(m_Socket, m_SockAddr, _pData, _dwLength, m_mOverlappedList[E_IO_CONNECT]);
		if (bResult == FALSE)
		{
			INT nErr = WSAGetLastError();
			if (nErr != WSA_IO_PENDING)
				return FALSE;
		}

		return TRUE;
	}

	//������ �����ϴ� �Լ�
	//SOCKET& _socket : ����� ����
	//SOCKADDR_IN& _sockAddr : ����� ������ �ּ�
	void CSocket::SetSocket(SOCKET _socket, SOCKADDR_IN _sockAddr)
	{
		SetSocket(_socket);
		SetSockAddr(_sockAddr);
	}

	//������ �����ϴ� �Լ�
	//SOCKET& _socket : ����� ����
	void CSocket::SetSocket(SOCKET _socket)
	{
		m_Socket = _socket;
		m_isUseable = true;
	}

	//������ ������ �����ϴ� �Լ�
	//SOCKADDR_IN& _sockAddr : ������ �ּ�
	void CSocket::SetSockAddr(SOCKADDR_IN _sockAddr)
	{
		CHAR* pIP = nullptr;
		m_SockAddr = _sockAddr;
		pIP = inet_ntoa(((struct sockaddr_in)m_SockAddr).sin_addr);
		memcpy_s(m_strIP, 20, pIP, strnlen_s(pIP, 20));
	}

	//������ ������ �����ϴ� �Լ�
	void CSocket::Close()
	{
		if (m_isUseable == TRUE)
			closesocket(m_Socket);

		m_isUseable = FALSE;
	}

	//���Ͽ� ���ſɼ��� �����ϴ� �Լ�
	//bool _onOff : �ɼ��� ���, ���� ����
	//int _nTimeOut : ������ Ÿ�Ӿƿ� ����
	//return bool : �ɼ� ���� ���� ����
	bool CSocket::SetLinger(bool _onOff, int _nTimeOut)
	{
		LINGER stLinger;
		int result;

		stLinger.l_onoff = (_onOff == true) ? 1 : 0;
		stLinger.l_linger = _nTimeOut;
		result = setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		return (result == SOCKET_ERROR) ? false : true;
	}

	//���Ͽ� keepalive�ɼ��� �����ϴ� �Լ�
	//bool _onOff : �ɼ� ���, ���� ����
	//int _nInterval : �˻� ��Ŷ�� �ֱ�
	//int _nRespond : Ÿ�Ӿƿ� �ð�
	//return bool : �ɼ� ���� ���� ����
	bool CSocket::SetKeepalive(bool _onOff, int _nInterval, int _nRespond)
	{
		tcp_keepalive stKeepAlive;
		int result;
		stKeepAlive.onoff = (_onOff == true) ? 1 : 0;
		stKeepAlive.keepaliveinterval = _nInterval;
		stKeepAlive.keepalivetime = _nRespond;

		result = setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&stKeepAlive, sizeof(stKeepAlive));

		return (result == SOCKET_ERROR) ? false : true;
	}

	//������ reuse�ɼ��� �����ϴ� �Լ�
	//bool _onOff : reuse�ɼ� ��� ����
	//return bool : �ɼ� ���� ���� ����
	bool CSocket::SetReuse(bool _onOff)
	{
		int nReuse = (_onOff == true) ? 1 : 0;
		int result = setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nReuse, sizeof(nReuse));

		return (result == SOCKET_ERROR) ? false : true;
	}

	//NAGLE�˰��� ����� �����ϴ� �Լ�
	//bool _onOff : �ɼ� ���� ����
	//return bool : �ɼ� ���� ��������
	bool CSocket::SetNodelay(bool _onOff)
	{
		BOOL bNodelay = (_onOff == true) ? TRUE : FALSE;
		int result = setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&bNodelay, sizeof(bNodelay));

		return (result == SOCKET_ERROR) ? false : true;
	}

	const CHAR* CSocket::GetIPAddress() const
	{
		return m_strIP;
	}

	//������ ��ȯ�ϴ� �Լ�
	//return const SOCKET& : ������� ����
	const SOCKET& CSocket::GetSocket() const
	{
		return m_Socket;
	}

	//������ �ּҸ� ��ȯ�ϴ� �Լ�
	//return const SOCKADDR_IN : ������� ������ �ּ�
	const SOCKADDR_IN& CSocket::GetSockAddr() const
	{
		return m_SockAddr;
	}

	//IO�� �´� overlapped�� �޴� �Լ�
	COverlapped& CSocket::GetOverlapped(IO_CODE _eIO)
	{
		assert(_eIO < E_IO_TIMER);

		return m_mOverlappedList[_eIO];
	}

	//������ ��밡������ ��ȯ
	//return bool : ��� ������ ����
	BOOL CSocket::IsUseable()
	{
		return m_isUseable;
	}
}