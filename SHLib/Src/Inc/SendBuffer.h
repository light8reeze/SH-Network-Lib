#pragma once
#include "PacketQueue.h"

namespace SHLib
{
	/*
		Ŭ���� : CSendBuffer
		�⺻ Ŭ���� : CPacketQueue

		������ �����͸� �����ϴ� ���� Ŭ����
	*/
	class CSendBuffer : public CPacketQueue
	{
	public:
		CSendBuffer() : CPacketQueue() {}
		~CSendBuffer() {};

		//���ۿ� �����͸� �� ��ġ�� ���ϴ� �Լ�
		//int _nSize : ���� �������� ũ��
		//return char* : ���� �޸��� ��ġ
		char* GetWritePointer(int _nSize);
	};

	inline char* CSendBuffer::GetWritePointer(int _nSize)
	{
		if (m_pWrite + _nSize > (m_pBuffer + MAX_BUF - 1))
			m_pWrite = m_pBuffer;

		return m_pWrite;
	}
}