#pragma once
#include "SHLib.h"
#include <functional>
#include "Socket.h"
#include "ReceiveBuffer.h"

namespace SHLib
{
	//오브젝트 상태
	enum NET_OBJECT_STATE : BYTE
	{
		E_NOTCONNECTED		= 0,
		E_IDLE				= 1,
		E_CONNECT			= 1	<< 1,	//통신 연결 상태
		E_ACCEPT			= 1 << 2,	//Accept대기
		E_RECEIVING			= 1 << 3,	//Recv대기
		E_SENDING			= 1 << 4,	//Send대기
		E_PROCESSING		= 1 << 5,	//처리 중 상태
		E_TIMEEVENT			= 1 << 6,
		E_DISCONNECT		= 1 << 7,	//통신 해제 대기 상태
	};

	/*
		클래스 : CNetworkObject
		네트워크 통신에 사용되는 기본 클래스
		Send, Recv, 패킷 처리를 담당하고있다.
	*/
	class SH_EXPORT CNetworkObject
	{
	private:
		typedef DWORD										PACKET_COMMAND;
		typedef std::function<void(const char*)>			PACKET_PROTOCOL;
		typedef std::map<PACKET_COMMAND, PACKET_PROTOCOL>	PROTOCOL_MAP;

	private:
		CSocket			m_Socket;  //클라이언트와 연결 된 소켓
		UINT			m_nState;  //네트워크 오브젝트의 상태
		#pragma warning(disable : 4251)
		PROTOCOL_MAP	m_mProtocolMap; //패킷 이벤트 맵
		#pragma warning(default : 4251)
		CReceiveBuffer	m_PacketBuffer; //네트워크 오브젝트에서 패킷을 저장하는 원형 버퍼
		CLocker			m_Locker;

	public:
		CNetworkObject();
		virtual ~CNetworkObject();

		int		Send(const char* _pData, int _nLength); //클라이언트에 데이터를 전송하는 함수(비동기식)
		int		Receive(int _nRecvSize); //클라이언트에서 Recv완료시 실행되는 함수(비동기식)
		void	Connect(); //네트워크 오브젝트의 초기 설정을 담당하는 함수
		void	SetClose();
		void	Close(); //네트워크 오브젝트의 연결을 종료하는 함수
		void	SetState(NET_OBJECT_STATE _state); //네트워크 오브젝트의 상태값을 설정하는 함수
		void	ResetState(NET_OBJECT_STATE _state); //네트워크 오브젝트의 상태값을 설정하는 함수
		void	PacketProcess(); //네트워크 오브젝트의 패킷을 처리하는 함수

		CSocket&	GetSocket(); //네트워크 오브젝트의 소켓을 반환하는 함수
		void		SetStateFlag(WORD _stateFlag);
		WORD		GetStateFlag();
		BOOL		IsState(NET_OBJECT_STATE _state); //네트워크 오브젝트의 상태를 반환하는 함수
		BOOL		IsState(INT _state); //네트워크 오브젝트의 상태를 반환하는 함수
		BOOL		IsConnect();
		BOOL		HasPacket();

		virtual void	Initialize();
		virtual void	OnSendComplete(char* _pData, int _nSize) {} //Send 완료시 실행되는 함수(가상함수)
		virtual void	OnReceiveComplete(char* _pData, int _nSize) {} //Recv실행 시 실행되는 함수(가상함수)
		virtual void	OnDisconnect() { SetClose(); }

	protected:
		void SetPacketProtocol(PACKET_COMMAND _nType, PACKET_PROTOCOL _fnProtocol); //패킷 이벤트 추가 함수

	//Interface
	private:
		virtual void	AddProtocol() = 0; //프로토콜을 설정하는 함수
		virtual void	OnConnect() {}; //연결시작시 실행되는 함수(가상함수)
		virtual void	OnReceive(int _nResult, int _nSize) {} //Recv실행 시 실행되는 함수(가상함수)
		virtual void	OnSend(int _nResult, int _nSize) {} //Send시 실행되는 함수(가상함수)
		virtual void	OnClose() {} //연결종료시 실행되는 함수(가상함수)
	};
}

//네트워크 객체 에서 패킷 처리시 사용하는 함수를 추가하는 매크로
//인수 : 패킷의 커맨드값(int)
//인수 : 세션 클래스함수의 포인터(std::function)
#define SET_PROTOCOL(packet, function) SetPacketProtocol((packet), std::bind((function), this, std::placeholders::_1))