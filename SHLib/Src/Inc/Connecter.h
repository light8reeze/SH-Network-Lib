#pragma once
#include "SHLib.h"
#include "NetworkObject.h"

namespace SHLib
{
	/*
		클래스 : CConnecter
		기본 클래스 : CNetworkObject

		다른 서버에 연결하여 통신하는 클래스
	*/
	class SH_EXPORT CConnecter : public CNetworkObject
	{
	private:
		typedef CNetworkObject SUPER;

	private:
		BOOL m_isInitialized;

	public:
		CConnecter();
		virtual ~CConnecter();

		//하위에서 확장시 super를 반드시 호출할 것
		virtual DWORD	Initialize(const char* _strIP, WORD _nPort);
		void			SetConnect(const char* _strIP, WORD _nPort); //커넥터의 소켓을 생성하는 함수
		void			Connect();
		int				StartConnect(); //커넥터의 연결을 시작하는 함수
		
		virtual void	AddProtocol() {}; //커넥터의 패킷 이벤트 설정을 해주는 함수 하위 클래스에서 이 함수에서 패킷 이벤트를 등록
		virtual void	OnConnect() {};  //커넥터 연결시 실행되는 함수
		virtual int		OnConnectFail() { return -1; };  //커넥터 연결 실패시 실행되는 함수
	};
}