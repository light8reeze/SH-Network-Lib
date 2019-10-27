#pragma once
#include "SHLib.h"
#include "Locker.h"
#include "Socket.h"

#define MAX_BUF	  0xFFFE //패킷 큐의 최대 크기
#define TEMP_SIZE 1000 //임시버퍼의 크기

namespace SHLib
{
	/*
		클래스 : CPacketQueue
		패킷을 원형버퍼구조로 관리하는 클래스
	*/
	class SH_EXPORT CPacketQueue
	{
	protected:
		char	m_pBuffer[MAX_BUF]; //큐의 전체 버퍼
		char	m_pTemp[TEMP_SIZE]; //큐의 임시 버퍼
		char*	m_pRead; //큐의 읽을데이터의 포인터
		char*	m_pWrite; //큐의 쓸 데이터의 포인터 
		WORD	m_nSize; //현재 큐에 저장된 크기

		CLocker m_Locker; //큐의 동기화 클래스

	public:
		CPacketQueue();
		~CPacketQueue();

		void	OnEnqueue(int _nRecvSize); //패킷데이터를 받을 때 처리하는 함수
		char*	Dequeue(); //큐에서 패킷 데이터를 꺼내는 함수
		void	ResetQueue(); //큐의 데이터를 모두 지우고 리셋해주는 함수
		int		GetAvailableSize(); //큐에서 남은 크기를 반환해주는 함수
		BOOL	HasPacket(); //큐에서 처리할 패킷이 존재하는지 반환하는 함수

	private:
		WORD	GetPacketSize(); //m_pRead로부터 패킷의 크기를 구하는 함수
	};

	//_pStart에서 _pEnd까지의 사이즈를 구해주는 함수
	//char* _pStart : 시작 포인터
	//char* _pEnd : 끝 포인터
	//return int : 두 포인터 사이의 크기(차이)
	inline UINT GetPointerOffset(char* _pStart, char* _pEnd)
	{
		return static_cast<UINT>(_pEnd - _pStart);
	}
}