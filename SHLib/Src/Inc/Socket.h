#pragma once
#include "SHLib.h"
#include <map>

namespace SHLib
{
	//IO_CODE : Overlapped�� ���Ǹ� �Ϸ�� IO�� � �������� �����ϱ����� �ڵ�
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

	//Overlapped����ü Ȯ��
	class SH_EXPORT COverlapped : public OVERLAPPED
	{
	public:
		COverlapped() : OVERLAPPED(), nIOMode(E_IO_NONE), wsaBuf()
		{
			wsaBuf.buf = nullptr;
			wsaBuf.len = 0;
		}

		INT		nIOMode; //IO�� ����
		WSABUF	wsaBuf;
	};

	/*
		Ŭ���� : CSocket
		���ϰ� ���Ͽ� ���� ������ �������ִ� Ŭ����
	*/
	class SH_EXPORT CSocket
	{
	private:
		SOCKET			m_Socket; //��ſ� ����ϴ� ����
		SOCKADDR_IN		m_SockAddr; //����� ������ �ּ�
		BOOL			m_isUseable; //������ ��� �������� ����
		COverlapped		m_mOverlappedList[E_IO_TIMER]; //��ſ� ���Ǵ� overlapped����ü ��
		CHAR			m_strIP[20];

	public:
		CSocket();
		~CSocket();

		DWORD		Initialize();

		CSocket*	AcceptSync();
		DWORD		AcceptSync(CSocket& _socket);
		
		//���� ��������
		CSocket*	AcceptAsync() { return nullptr; }
		DWORD		AcceptAsync(CSocket& _socket) { return 0; }

		BOOL		ConnectAsync(char* _pData, DWORD _dwLength);

		void SetSocket(SOCKET _socket, SOCKADDR_IN _sockAddr); //������ �����ϴ� �Լ�
		void SetSocket(SOCKET _socket); //������ �����ϴ� �Լ�
		void SetSockAddr(SOCKADDR_IN _sockAddr); //������ �ּҸ� �����ϴ� �Լ�
		void Close(); //������ ������ �����ϴ� �Լ�

		bool SetLinger(bool _onOff, int _nTimeOut); //���Ͽ� ���ſɼ��� �����ϴ� �Լ�
		bool SetKeepalive(bool _onOff, int _nInterval, int _nRespond); //���Ͽ� keepalive�ɼ��� �����ϴ� �Լ� 
		bool SetReuse(bool _onOff); //������ reuse�ɼ��� �����ϴ� �Լ�
		bool SetNodelay(bool _onOff); //Nodelay�ɼ��� �����ϴ� �Լ�

		const CHAR*			GetIPAddress() const;
		const SOCKET&		GetSocket() const; //������ ��ȯ�ϴ� �Լ�
		const SOCKADDR_IN&	GetSockAddr() const; //������ �ּҸ� ��ȯ�ϴ� �Լ�
		COverlapped&		GetOverlapped(IO_CODE _eIO); //IO�� �´� overlapped�� �޴� �Լ�
		BOOL				IsUseable(); //������ ��밡������ ��ȯ
	};
}