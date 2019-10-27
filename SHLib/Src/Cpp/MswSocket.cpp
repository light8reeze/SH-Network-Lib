#include "MswSocket.h"
#include "AutoLock.h"
#include <stdio.h>

namespace SHLib
{
	LPFN_CONNECTEX CMswSocket::m_lpfnConnectEx = nullptr;
	LPFN_ACCEPTEX CMswSocket::m_lpfnAcceptEx = nullptr;

	BOOL CMswSocket::ConnectEx(SOCKET& _socket, SOCKADDR_IN& _sockAddr, char* _pSendData, DWORD& _pSendSize, COverlapped& _overlapped)
	{
		static CLocker locker;
		{
			AUTO_LOCK(CLocker, locker);

			if (m_lpfnConnectEx == nullptr)
			{
				assert(m_lpfnConnectEx == nullptr);

				SOCKET	socket = WSASocketW(AF_INET, SOCK_STREAM, 0, 0, NULL, NULL);
				GUID	guid = WSAID_CONNECTEX;
				DWORD	dwSize = 0;

				assert(socket != INVALID_SOCKET);
				if (socket == INVALID_SOCKET)
					return FALSE;

				INT nResult = WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
					(VOID*)&guid, sizeof(guid), (VOID*)&m_lpfnConnectEx,
					sizeof(m_lpfnConnectEx), &dwSize, NULL, NULL);

				if (nResult == -1)
				{
					printf("%d\n", WSAGetLastError());
					return FALSE;
				}
			}
		}

		assert(m_lpfnConnectEx != nullptr);
		return m_lpfnConnectEx(_socket, (SOCKADDR*)&_sockAddr, sizeof(_sockAddr), _pSendData, _pSendSize, NULL, (LPOVERLAPPED)&_overlapped);
	}
}