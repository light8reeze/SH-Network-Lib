#pragma once
#include "PacketQueue.h"

namespace SHLib
{
	/*
		Ŭ���� : CReceiveBuffer
		�⺻ Ŭ���� : CPacketQueue

		���۹��� �����͸� �����ϴ� ���� Ŭ����
	*/
	class SH_EXPORT CReceiveBuffer : public CPacketQueue
	{
	public:
		CReceiveBuffer();
		~CReceiveBuffer();

		int		SetReceive(CSocket& _socket, INT _nSize = -1); //ť�� ��Ŷ �����͸� recv��û�ϴ� �Լ�(����ȭ)(�񵿱�� recv���)

	private:
		int		GetRecvSize(); //ť���� recv�� ����� ���ϴ� �Լ�
	};
}