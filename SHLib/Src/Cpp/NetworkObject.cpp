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

	//클라이언트에 데이터를 전송하는 함수(비동기식)
	//const char* _pData : 전송할 데이터의 주소값
	//int _nLength : 전송할 데이터의 크기
	//return int : 데이터 전송후 결과값
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

	//클라이언트에서 Recv완료시 실행되는 함수(비동기식)
	//int _nRecvSize : Recv된 데이터의 크기
	//return int : WSARecv의 결과 값
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

	//네트워크 오브젝트의 초기 설정을 담당하는 함수
	//CSocket* _pSocket : 설정할 소켓의 주소값
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

	//네트워크 오브젝트의 연결을 종료하는 함수
	void CNetworkObject::Close()
	{
		GetSingleton<CLogger>().Print("%x Close\n", this);

		AUTO_LOCK(CLocker, m_Locker);

		m_PacketBuffer.ResetQueue();
		OnClose();
		m_Socket.Close();
		SetStateFlag(E_NOTCONNECTED);
	}

	//네트워크 오브젝트의 상태값을 설정하는 함수
	//NET_OBJECT_STATE _state : 설정할 세션의 상태값
	void CNetworkObject::SetState(NET_OBJECT_STATE _state)
	{
		{
			AUTO_LOCK(CLocker, m_Locker);
			m_nState |= _state;
		}
	}

	//네트워크 오브젝트의 상태값을 설정하는 함수
	//NET_OBJECT_STATE _state : 설정할 세션의 상태값
	void CNetworkObject::ResetState(NET_OBJECT_STATE _state)
	{
		{
			AUTO_LOCK(CLocker, m_Locker);
			m_nState &= (~_state);
		}
	}

	//네트워크 오브젝트의 패킷을 처리하는 함수
	void CNetworkObject::PacketProcess()
	{
		char* packet = nullptr;
		
		//패킷 큐에서 패킷을 꺼내온다.
		while ((packet = m_PacketBuffer.Dequeue()) != nullptr && IsState(E_DISCONNECT) == FALSE)
		{
			assert(packet != nullptr);

			WORD type = *((WORD*)packet + 1);

			//패킷에 등록된 이벤트 시 패킷 이벤트 실행
			auto iter = m_mProtocolMap.find(type);
			if (iter != m_mProtocolMap.end())
				(iter->second)(packet);
		}
	}

	//네트워크 오브젝트의 소켓을 반환하는 함수
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

	//네트워크 오브젝트의 상태를 반환하는 함수
	BOOL CNetworkObject::IsState(NET_OBJECT_STATE _state)
	{
		AUTO_LOCK(CLocker, m_Locker);
		return (m_nState & _state)? TRUE : FALSE;
	}
	
	//네트워크 오브젝트의 상태를 반환하는 함수
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

	//패킷 이벤트 추가 함수
	//PACKET_COMMAND _nType : 추가할 패킷 커맨드 값
	//PACKET_PROTOCOL _fnProtocol : 패킷 처리시 실행할 이벤트 함수
	void CNetworkObject::SetPacketProtocol(PACKET_COMMAND _nType, PACKET_PROTOCOL _fnProtocol)
	{
		m_mProtocolMap[_nType] = _fnProtocol;
	}
}