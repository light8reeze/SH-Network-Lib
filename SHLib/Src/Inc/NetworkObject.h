#pragma once
#include "SHLib.h"
#include <functional>
#include "Socket.h"
#include "ReceiveBuffer.h"

namespace SHLib
{
	//������Ʈ ����
	enum NET_OBJECT_STATE : BYTE
	{
		E_NOTCONNECTED		= 0,
		E_IDLE				= 1,
		E_CONNECT			= 1	<< 1,	//��� ���� ����
		E_ACCEPT			= 1 << 2,	//Accept���
		E_RECEIVING			= 1 << 3,	//Recv���
		E_SENDING			= 1 << 4,	//Send���
		E_PROCESSING		= 1 << 5,	//ó�� �� ����
		E_TIMEEVENT			= 1 << 6,
		E_DISCONNECT		= 1 << 7,	//��� ���� ��� ����
	};

	/*
		Ŭ���� : CNetworkObject
		��Ʈ��ũ ��ſ� ���Ǵ� �⺻ Ŭ����
		Send, Recv, ��Ŷ ó���� ����ϰ��ִ�.
	*/
	class SH_EXPORT CNetworkObject
	{
	private:
		typedef DWORD										PACKET_COMMAND;
		typedef std::function<void(const char*)>			PACKET_PROTOCOL;
		typedef std::map<PACKET_COMMAND, PACKET_PROTOCOL>	PROTOCOL_MAP;

	private:
		CSocket			m_Socket;  //Ŭ���̾�Ʈ�� ���� �� ����
		UINT			m_nState;  //��Ʈ��ũ ������Ʈ�� ����
		#pragma warning(disable : 4251)
		PROTOCOL_MAP	m_mProtocolMap; //��Ŷ �̺�Ʈ ��
		#pragma warning(default : 4251)
		CReceiveBuffer	m_PacketBuffer; //��Ʈ��ũ ������Ʈ���� ��Ŷ�� �����ϴ� ���� ����
		CLocker			m_Locker;

	public:
		CNetworkObject();
		virtual ~CNetworkObject();

		int		Send(const char* _pData, int _nLength); //Ŭ���̾�Ʈ�� �����͸� �����ϴ� �Լ�(�񵿱��)
		int		Receive(int _nRecvSize); //Ŭ���̾�Ʈ���� Recv�Ϸ�� ����Ǵ� �Լ�(�񵿱��)
		void	Connect(); //��Ʈ��ũ ������Ʈ�� �ʱ� ������ ����ϴ� �Լ�
		void	SetClose();
		void	Close(); //��Ʈ��ũ ������Ʈ�� ������ �����ϴ� �Լ�
		void	SetState(NET_OBJECT_STATE _state); //��Ʈ��ũ ������Ʈ�� ���°��� �����ϴ� �Լ�
		void	ResetState(NET_OBJECT_STATE _state); //��Ʈ��ũ ������Ʈ�� ���°��� �����ϴ� �Լ�
		void	PacketProcess(); //��Ʈ��ũ ������Ʈ�� ��Ŷ�� ó���ϴ� �Լ�

		CSocket&	GetSocket(); //��Ʈ��ũ ������Ʈ�� ������ ��ȯ�ϴ� �Լ�
		void		SetStateFlag(WORD _stateFlag);
		WORD		GetStateFlag();
		BOOL		IsState(NET_OBJECT_STATE _state); //��Ʈ��ũ ������Ʈ�� ���¸� ��ȯ�ϴ� �Լ�
		BOOL		IsState(INT _state); //��Ʈ��ũ ������Ʈ�� ���¸� ��ȯ�ϴ� �Լ�
		BOOL		IsConnect();
		BOOL		HasPacket();

		virtual void	Initialize();
		virtual void	OnSendComplete(char* _pData, int _nSize) {} //Send �Ϸ�� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnReceiveComplete(char* _pData, int _nSize) {} //Recv���� �� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnDisconnect() { SetClose(); }

	protected:
		void SetPacketProtocol(PACKET_COMMAND _nType, PACKET_PROTOCOL _fnProtocol); //��Ŷ �̺�Ʈ �߰� �Լ�

	//Interface
	private:
		virtual void	AddProtocol() = 0; //���������� �����ϴ� �Լ�
		virtual void	OnConnect() {}; //������۽� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnReceive(int _nResult, int _nSize) {} //Recv���� �� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnSend(int _nResult, int _nSize) {} //Send�� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnClose() {} //��������� ����Ǵ� �Լ�(�����Լ�)
	};
}

//��Ʈ��ũ ��ü ���� ��Ŷ ó���� ����ϴ� �Լ��� �߰��ϴ� ��ũ��
//�μ� : ��Ŷ�� Ŀ�ǵ尪(int)
//�μ� : ���� Ŭ�����Լ��� ������(std::function)
#define SET_PROTOCOL(packet, function) SetPacketProtocol((packet), std::bind((function), this, std::placeholders::_1))