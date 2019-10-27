#pragma once
#include "Singleton.h"
#include "Socket.h"

namespace SHLib
{
	typedef ULONG_PTR IO_KEY;

	/*
		Ŭ���� : CNetworkIO
		�������� ����ϴ� IOCP�� �����ϴ� Ŭ����
	*/
	class SH_EXPORT CNetworkIO
	{
		SET_SINGLETON_FINAL(CNetworkIO);

	private:
		HANDLE	m_hPort; //IOCP�ڵ�

	public:
		CNetworkIO();
		~CNetworkIO();

		DWORD	Initialize();
		HANDLE	AddIOSocket(IO_KEY _key, CSocket* _pSocket); //IOCP�� ������ ����ϴ� �Լ�
		BOOL	GetIOCompletion(IO_KEY& _key, COverlapped*& _pOverlapped, DWORD& _dwRecvSize, DWORD _dwWaitTime = INFINITE);
		BOOL	PostIOCompletion(IO_KEY _key, COverlapped& _overlapped, DWORD _dwSize); //������� �ۼ�
	};
}