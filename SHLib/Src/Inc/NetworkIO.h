#pragma once
#include "Singleton.h"
#include "Socket.h"

namespace SHLib
{
	typedef ULONG_PTR IO_KEY;

	/*
		클래스 : CNetworkIO
		서버에서 사용하는 IOCP를 저장하는 클래스
	*/
	class SH_EXPORT CNetworkIO
	{
		SET_SINGLETON_FINAL(CNetworkIO);

	private:
		HANDLE	m_hPort; //IOCP핸들

	public:
		CNetworkIO();
		~CNetworkIO();

		DWORD	Initialize();
		HANDLE	AddIOSocket(IO_KEY _key, CSocket* _pSocket); //IOCP에 소켓을 등록하는 함수
		BOOL	GetIOCompletion(IO_KEY& _key, COverlapped*& _pOverlapped, DWORD& _dwRecvSize, DWORD _dwWaitTime = INFINITE);
		BOOL	PostIOCompletion(IO_KEY _key, COverlapped& _overlapped, DWORD _dwSize); //여기부터 작성
	};
}