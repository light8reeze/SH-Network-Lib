#pragma once
#include "NetworkObject.h"

namespace SHLib
{
	/*
		클래스 : CUserSession
		기본클래스 : CNetworkObject
		클라이언트 통신에 사용되는 기본 세션
	*/
	class CUserSession : public CNetworkObject
	{
	public:
		CUserSession() : CNetworkObject() {}
		virtual ~CUserSession() {}
	
		virtual void OnConstruct() {}
		virtual void OnDestroy() {}

	private:
		virtual void	AddProtocol() = 0; //프로토콜을 설정하는 함수
		virtual void	OnConnect() {}; //연결시작시 실행되는 함수(가상함수)
		virtual void	OnReceive(int _nResult, int _nSize) {} //Recv실행 시 실행되는 함수(가상함수)
		virtual void	OnSend(int _nResult, int _nSize) {} //Send시 실행되는 함수(가상함수)
		virtual void	OnClose() {} //연결종료시 실행되는 함수(가상함수)
	};
}