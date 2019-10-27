#pragma once
#include "SHLib.h"
#include <map>

namespace SHLib
{
	//IO_CODE : Overlapped에 사용되면 완료된 IO가 어떤 종류인지 구분하기위한 코드
	enum IO_CODE
	{
		E_IO_NONE = -1,
		E_IO_ACCEPT = 0,
		E_IO_CONNECT,
		E_IO_RECV,
		E_IO_SEND,
		E_IO_PROCESS,
		E_IO_DISCONNECT,
		E_IO_TIMER,
		E_IO_COUNT
	};

	//Overlapped구조체 확장
	class SH_EXPORT COverlapped : public OVERLAPPED
	{
	public:
		COverlapped() : OVERLAPPED(), nIOMode(E_IO_NONE), wsaBuf()
		{
			wsaBuf.buf = nullptr;
			wsaBuf.len = 0;
		}

		INT		nIOMode; //IO의 종류
		WSABUF	wsaBuf;
	};

	/*
		클래스 : CSocket
		소켓과 소켓에 대한 정보를 가지고있는 클래스
	*/
	class SH_EXPORT CSocket
	{
	private:
		SOCKET			m_Socket; //통신에 사용하는 소켓
		SOCKADDR_IN		m_SockAddr; //연결된 소켓의 주소
		BOOL			m_isUseable; //소켓이 사용 가능한지 여부
		COverlapped		m_mOverlappedList[E_IO_TIMER]; //통신에 사용되는 overlapped구조체 맵
		CHAR			m_strIP[20];

	public:
		CSocket();
		~CSocket();

		DWORD		Initialize();

		CSocket*	AcceptSync();
		DWORD		AcceptSync(CSocket& _socket);
		
		//추후 구현예정
		CSocket*	AcceptAsync() { return nullptr; }
		DWORD		AcceptAsync(CSocket& _socket) { return 0; }

		BOOL		ConnectAsync(char* _pData, DWORD _dwLength);

		void SetSocket(SOCKET _socket, SOCKADDR_IN _sockAddr); //소켓을 설정하는 함수
		void SetSocket(SOCKET _socket); //소켓을 설정하는 함수
		void SetSockAddr(SOCKADDR_IN _sockAddr); //소켓의 주소를 설정하는 함수
		void Close(); //소켓의 연결을 해제하는 함수

		bool SetLinger(bool _onOff, int _nTimeOut); //소켓에 링거옵션을 설정하는 함수
		bool SetKeepalive(bool _onOff, int _nInterval, int _nRespond); //소켓에 keepalive옵션을 설정하는 함수 
		bool SetReuse(bool _onOff); //소켓의 reuse옵션을 설정하는 함수
		bool SetNodelay(bool _onOff); //Nodelay옵션을 설정하는 함수

		const CHAR*			GetIPAddress() const;
		const SOCKET&		GetSocket() const; //소켓을 반환하는 함수
		const SOCKADDR_IN&	GetSockAddr() const; //소켓의 주소를 반환하는 함수
		COverlapped&		GetOverlapped(IO_CODE _eIO); //IO에 맞는 overlapped를 받는 함수
		BOOL				IsUseable(); //소켓이 사용가능한지 반환
	};
}