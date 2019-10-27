#pragma once
#include "SHLib.h"
#include "Thread.h"

namespace SHLib
{
	/*
		클래스 : CAcceptor

		서버의 Accept를 담당하는 클래스
	*/
	class SH_EXPORT CAcceptor : public CThread
	{
	private:
		typedef CThread SUPER;

	private:
		CSocket	m_AcceptSocket;	//RunBehaviour에 사용할 소켓

	public:
		CAcceptor(); //Acceptor의 소켓을 생성한다
		~CAcceptor();

		HANDLE Initialize(const char* _strIP, int _nPort);
		HANDLE Initialize(CSocket& _socket);
		void Clear(); //Acceptor를 종료하는 함수

		CSocket& GetSocket();

	protected:
		INT	 Main(); //서버로 들어오는 RunBehaviour를 실행하는 함수(쓰레드 사용)
	};
}

#include "Acceptor.Inl"