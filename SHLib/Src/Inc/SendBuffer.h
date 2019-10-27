#pragma once
#include "PacketQueue.h"

namespace SHLib
{
	/*
		클래스 : CSendBuffer
		기본 클래스 : CPacketQueue

		전송할 데이터를 보관하는 버퍼 클래스
	*/
	class CSendBuffer : public CPacketQueue
	{
	public:
		CSendBuffer() : CPacketQueue() {}
		~CSendBuffer() {};

		//버퍼에 데이터를 쓸 위치를 구하는 함수
		//int _nSize : 넣을 데이터의 크기
		//return char* : 버퍼 메모리의 위치
		char* GetWritePointer(int _nSize);
	};

	inline char* CSendBuffer::GetWritePointer(int _nSize)
	{
		if (m_pWrite + _nSize > (m_pBuffer + MAX_BUF - 1))
			m_pWrite = m_pBuffer;

		return m_pWrite;
	}
}