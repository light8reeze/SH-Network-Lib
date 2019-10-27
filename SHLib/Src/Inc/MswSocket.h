#pragma once
#include "SHLib.h"
#include <mswsock.h>

namespace SHLib
{
	class COverlapped;
	class CMswSocket
	{
	private:
		static LPFN_CONNECTEX	m_lpfnConnectEx;
		static LPFN_ACCEPTEX	m_lpfnAcceptEx;

	public:
		static BOOL ConnectEx(SOCKET& _socket, SOCKADDR_IN& _sockAddr, char* _pSendData, DWORD& _pSendSize, COverlapped& _overlapped);
		static BOOL AcceptEx() { assert(0); return FALSE; };
	};
}