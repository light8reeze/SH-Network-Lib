#pragma once
#include "PacketQueue.h"

namespace SHLib
{
	/*
		클래스 : CReceiveBuffer
		기본 클래스 : CPacketQueue

		전송받은 데이터를 보관하는 버퍼 클래스
	*/
	class SH_EXPORT CReceiveBuffer : public CPacketQueue
	{
	public:
		CReceiveBuffer();
		~CReceiveBuffer();

		int		SetReceive(CSocket& _socket, INT _nSize = -1); //큐에 패킷 데이터를 recv요청하는 함수(동기화)(비동기식 recv사용)

	private:
		int		GetRecvSize(); //큐에서 recv할 사이즈를 구하는 함수
	};
}