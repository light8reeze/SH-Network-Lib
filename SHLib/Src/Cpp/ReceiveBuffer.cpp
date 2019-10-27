#include "ReceiveBuffer.h"

namespace SHLib
{
	CReceiveBuffer::CReceiveBuffer() : CPacketQueue()
	{
	}


	CReceiveBuffer::~CReceiveBuffer()
	{
	}

	//큐에 패킷 데이터를 recv요청하는 함수(동기화)(비동기식 recv사용)
	//CSocket* _socket : recv요청할 네트워크 소켓의 주소값
	//return int : WSARecv함수의 결과값을 반환
	int CReceiveBuffer::SetReceive(CSocket& _socket, INT _nSize)
	{
		INT				nBufSize	= (_nSize != -1)? _nSize : GetRecvSize();
		INT				nResult		= 0;
		DWORD			dwFlag		= 0;
		COverlapped&	overlapped	= _socket.GetOverlapped(E_IO_RECV);
		WSABUF&			wsaBuf		= overlapped.wsaBuf;

		//버퍼에 들어갈 크기가 부족할때 함수 종료
		if (GetRecvSize() < sizeof(WORD))
		{
			if (m_pWrite != (m_pBuffer + MAX_BUF - 1))
			{
				printf("Buffer Is Full!\n");
				return SOCKET_ERROR;
			}
		}

		ZeroMemory(&overlapped, sizeof(OVERLAPPED));
		wsaBuf.buf = m_pWrite;
		wsaBuf.len = static_cast<ULONG>(nBufSize);

		nResult = WSARecv(_socket.GetSocket(), &wsaBuf, 1,
			NULL, &dwFlag, &overlapped, NULL);

		//WSARecv함수 실패시 
		//SOCKET_ERROR(-1)반환
		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("WSARecv Error Message : %d\n", WSAGetLastError());
			return SOCKET_ERROR;
		}

		return 1;
	}

	//큐에서 recv할 사이즈를 구하는 함수
	//return int : 큐에서 recv할 사이즈
	int CReceiveBuffer::GetRecvSize()
	{
		char* pWrite = m_pWrite;
		char* pRead = m_pRead;
		char* pEnd = (pRead > pWrite) ? pRead : m_pBuffer + MAX_BUF;
		int nSize = m_nSize;
		int nBufSize = GetPointerOffset(pWrite, pEnd);

		if (MAX_BUF - nSize < sizeof(WORD))
			nBufSize = MAX_BUF - nSize;

		return nBufSize;
	}
}