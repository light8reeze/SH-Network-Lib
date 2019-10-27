#include "PacketQueue.h"
#include "NetworkIO.h"
#include "NetworkObject.h"
#include "MainThread.h"
#include "Logger.h"
#include "AutoLock.h"

namespace SHLib
{
	CNetworkObject::CNetworkObject() : m_Socket(), m_nState(E_NOTCONNECTED),
		m_mProtocolMap(), m_PacketBuffer(), m_Locker()
	{
	}

	CNetworkObject::~CNetworkObject()
	{
		Close();
	}

	//Ŭ���̾�Ʈ�� �����͸� �����ϴ� �Լ�(�񵿱��)
	//const char* _pData : ������ �������� �ּҰ�
	//int _nLength : ������ �������� ũ��
	//return int : ������ ������ �����
	int CNetworkObject::Send(const char* _pData, int _nLength)
	{
		int				nResult		= 0;
		COverlapped&	overlapped	= m_Socket.GetOverlapped(E_IO_SEND);
		WSABUF&			wsaBuf		= overlapped.wsaBuf;

		if (IsState(E_DISCONNECT) == TRUE || IsConnect() == FALSE)
			return -1;

		ZeroMemory(&overlapped, sizeof(OVERLAPPED));
		wsaBuf.buf = (CHAR*)_pData;
		wsaBuf.len = _nLength;

		nResult = WSASend(m_Socket.GetSocket(), &wsaBuf, 1, (LPDWORD)&_nLength, 0, &overlapped, NULL);

		if (nResult == SOCKET_ERROR && nResult != WSA_IO_PENDING)
			OnDisconnect();
		else
			SetState(E_SENDING);

		OnSend(nResult, _nLength);

		return nResult;
	}

	//Ŭ���̾�Ʈ���� Recv�Ϸ�� ����Ǵ� �Լ�(�񵿱��)
	//int _nRecvSize : Recv�� �������� ũ��
	//return int : WSARecv�� ��� ��
	int CNetworkObject::Receive(int _nRecvSize)
	{
		m_PacketBuffer.OnEnqueue(_nRecvSize);
		int result = m_PacketBuffer.SetReceive(m_Socket);

		if (result == SOCKET_ERROR)
			return result;

		SetState(E_RECEIVING);
		OnReceive(result, _nRecvSize);

		return result;
	}

	//��Ʈ��ũ ������Ʈ�� �ʱ� ������ ����ϴ� �Լ�
	//CSocket* _pSocket : ������ ������ �ּҰ�
	void CNetworkObject::Connect()
	{
		AUTO_LOCK(CLocker, m_Locker);

		SetStateFlag(E_CONNECT);

		int result = m_PacketBuffer.SetReceive(m_Socket, 0);

		if (result == SOCKET_ERROR)
		{
			SetClose();
		}
		else
		{
			SetState(E_RECEIVING);
			OnConnect();
		}
	}

	void CNetworkObject::SetClose()
	{
		GetSingleton<CLogger>().Print("%x SetClose\n", this);
		AUTO_LOCK(CLocker, m_Locker);

		if (IsState(E_DISCONNECT) == TRUE)
			return;

		SetState(E_DISCONNECT);
	}

	//��Ʈ��ũ ������Ʈ�� ������ �����ϴ� �Լ�
	void CNetworkObject::Close()
	{
		GetSingleton<CLogger>().Print("%x Close\n", this);

		AUTO_LOCK(CLocker, m_Locker);

		m_PacketBuffer.ResetQueue();
		OnClose();
		m_Socket.Close();
		SetStateFlag(E_NOTCONNECTED);
	}

	//��Ʈ��ũ ������Ʈ�� ���°��� �����ϴ� �Լ�
	//NET_OBJECT_STATE _state : ������ ������ ���°�
	void CNetworkObject::SetState(NET_OBJECT_STATE _state)
	{
		{
			AUTO_LOCK(CLocker, m_Locker);
			m_nState |= _state;
		}
	}

	//��Ʈ��ũ ������Ʈ�� ���°��� �����ϴ� �Լ�
	//NET_OBJECT_STATE _state : ������ ������ ���°�
	void CNetworkObject::ResetState(NET_OBJECT_STATE _state)
	{
		{
			AUTO_LOCK(CLocker, m_Locker);
			m_nState &= (~_state);
		}
	}

	//��Ʈ��ũ ������Ʈ�� ��Ŷ�� ó���ϴ� �Լ�
	void CNetworkObject::PacketProcess()
	{
		char* packet = nullptr;
		
		//��Ŷ ť���� ��Ŷ�� �����´�.
		while ((packet = m_PacketBuffer.Dequeue()) != nullptr && IsState(E_DISCONNECT) == FALSE)
		{
			assert(packet != nullptr);

			WORD type = *((WORD*)packet + 1);

			//��Ŷ�� ��ϵ� �̺�Ʈ �� ��Ŷ �̺�Ʈ ����
			auto iter = m_mProtocolMap.find(type);
			if (iter != m_mProtocolMap.end())
				(iter->second)(packet);
		}
	}

	//��Ʈ��ũ ������Ʈ�� ������ ��ȯ�ϴ� �Լ�
	CSocket& CNetworkObject::GetSocket()
	{
		return m_Socket;
	}

	void CNetworkObject::SetStateFlag(WORD _stateFlag)
	{
		{
			AUTO_LOCK(CLocker, m_Locker);
			m_nState = _stateFlag;
		}
	}

	WORD CNetworkObject::GetStateFlag()
	{
		return m_nState;
	}

	//��Ʈ��ũ ������Ʈ�� ���¸� ��ȯ�ϴ� �Լ�
	BOOL CNetworkObject::IsState(NET_OBJECT_STATE _state)
	{
		AUTO_LOCK(CLocker, m_Locker);
		return (m_nState & _state)? TRUE : FALSE;
	}
	
	//��Ʈ��ũ ������Ʈ�� ���¸� ��ȯ�ϴ� �Լ�
	BOOL CNetworkObject::IsState(INT _state)
	{
		AUTO_LOCK(CLocker, m_Locker);
		return (m_nState & _state) ? TRUE : FALSE;
	}

	BOOL CNetworkObject::IsConnect()
	{
		return (m_nState != E_NOTCONNECTED) ? TRUE : FALSE;
	}

	BOOL CNetworkObject::HasPacket()
	{
		return m_PacketBuffer.HasPacket();
	}

	void CNetworkObject::Initialize()
	{
		assert(GetStateFlag() == E_NOTCONNECTED);

		AUTO_LOCK(CLocker, m_Locker);
		AddProtocol();
		m_Socket.Initialize();
	}

	//��Ŷ �̺�Ʈ �߰� �Լ�
	//PACKET_COMMAND _nType : �߰��� ��Ŷ Ŀ�ǵ� ��
	//PACKET_PROTOCOL _fnProtocol : ��Ŷ ó���� ������ �̺�Ʈ �Լ�
	void CNetworkObject::SetPacketProtocol(PACKET_COMMAND _nType, PACKET_PROTOCOL _fnProtocol)
	{
		m_mProtocolMap[_nType] = _fnProtocol;
	}
}