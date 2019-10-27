#include "ReceiveBuffer.h"

namespace SHLib
{
	CReceiveBuffer::CReceiveBuffer() : CPacketQueue()
	{
	}


	CReceiveBuffer::~CReceiveBuffer()
	{
	}

	//ť�� ��Ŷ �����͸� recv��û�ϴ� �Լ�(����ȭ)(�񵿱�� recv���)
	//CSocket* _socket : recv��û�� ��Ʈ��ũ ������ �ּҰ�
	//return int : WSARecv�Լ��� ������� ��ȯ
	int CReceiveBuffer::SetReceive(CSocket& _socket, INT _nSize)
	{
		INT				nBufSize	= (_nSize != -1)? _nSize : GetRecvSize();
		INT				nResult		= 0;
		DWORD			dwFlag		= 0;
		COverlapped&	overlapped	= _socket.GetOverlapped(E_IO_RECV);
		WSABUF&			wsaBuf		= overlapped.wsaBuf;

		//���ۿ� �� ũ�Ⱑ �����Ҷ� �Լ� ����
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

		//WSARecv�Լ� ���н� 
		//SOCKET_ERROR(-1)��ȯ
		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("WSARecv Error Message : %d\n", WSAGetLastError());
			return SOCKET_ERROR;
		}

		return 1;
	}

	//ť���� recv�� ����� ���ϴ� �Լ�
	//return int : ť���� recv�� ������
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