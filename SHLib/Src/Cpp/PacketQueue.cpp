#include "PacketQueue.h"
#include "NetworkIO.h"
#include "AutoLock.h"

namespace SHLib
{
	CPacketQueue::CPacketQueue() : m_pBuffer(), m_pTemp(),
		m_pRead(m_pBuffer), m_pWrite(m_pBuffer),
		m_nSize(0), m_Locker()
	{
		memset(m_pBuffer, 0, MAX_BUF);
		memset(m_pTemp, 0, TEMP_SIZE);

		m_pRead = m_pBuffer;
		m_pWrite = m_pBuffer;
	}

	CPacketQueue::~CPacketQueue()
	{
	}

	//패킷데이터를 받을 때 처리하는 함수(동기화)
	//int _nRecvSize : 큐에 recv한 크기
	void CPacketQueue::OnEnqueue(int _nRecvSize)
	{
		char* writePos = m_pWrite;

		AUTO_LOCK(CLocker, m_Locker);

		//큐의 write포인터 위치를 조정
		//write포인터의 위치가 버퍼의 끝부분이면
		//버퍼의 시작부분으로 write포인터를 옮겨준다
		if (writePos + _nRecvSize > (m_pBuffer + MAX_BUF - 1))
			m_pWrite = m_pBuffer;
		else
			m_pWrite += _nRecvSize;


		m_nSize += _nRecvSize;
	}

	//큐에서 패킷 데이터를 꺼내는 함수
	//return char* : 저장된 패킷의 주소값, 패킷이 없으면 nullptr반환
	char* CPacketQueue::Dequeue()
	{
		if (HasPacket() == FALSE)
			return nullptr;

		char* pRead = m_pRead;
		WORD wPacketSize = GetPacketSize();
		WORD wType;

		//패킷의 크기가 버퍼의 크기를 초과할시에
		//버퍼의 앞부분에서 남은부분을 추가로 복사
		if (pRead + wPacketSize > (m_pBuffer + MAX_BUF - 1))
		{
			//버퍼의 복사할 크기와 잘린 패킷의 크기를 계산
			int bufSize = GetPointerOffset(pRead, (m_pBuffer + MAX_BUF));
			int cpySize = wPacketSize - bufSize;

			memcpy_s(m_pTemp, bufSize, pRead, bufSize);
			memcpy_s(m_pTemp + bufSize, cpySize, m_pBuffer, cpySize);
			wType = *((WORD*)m_pTemp + 1);

			//Lock
			{
				AUTO_LOCK(CLocker, m_Locker);
				m_pRead = m_pBuffer + cpySize;
				m_nSize -= wPacketSize;
			}

			return m_pTemp;
		}

		//Lock
		{
			AUTO_LOCK(CLocker, m_Locker);
			m_pRead += wPacketSize;
			m_nSize -= wPacketSize;
		}

		wType = *((WORD*)pRead + 1);

		return pRead;
	}

	//큐의 데이터를 모두 지우고 리셋해주는 함수
	void CPacketQueue::ResetQueue()
	{
		AUTO_LOCK(CLocker, m_Locker);

		m_pRead = m_pBuffer;
		m_pWrite = m_pBuffer;
		m_nSize = 0;
	}

	//큐에서 남은 크기를 반환해주는 함수
	//return int : 큐에서 남은 사이즈를 반환
	int CPacketQueue::GetAvailableSize()
	{
		return MAX_BUF - m_nSize;
	}

	//큐에서 처리할 패킷이 존재하는지 반환하는 함수
	//return bool : 패킷 존재여부 반환
	BOOL CPacketQueue::HasPacket()
	{
		if (m_nSize < sizeof(WORD))
			return FALSE;

		char* pRead = m_pRead;
		char* pWrite = m_pWrite;
		int nowSize = m_nSize;
		WORD wPacketSize = GetPacketSize();
		int readToWrite;

		//read포인터에서 write포인터까지의 크기를 계산
		if (pWrite == pRead)
			readToWrite = (m_nSize == 0) ? MAX_BUF : m_nSize;
		else
		{
			char* pStart = (pWrite < pRead) ? pRead - 1 : pRead;
			char* pEnd = (pWrite < pRead) ? pWrite + MAX_BUF : pWrite;
			readToWrite = GetPointerOffset(pStart, pEnd);
		}

		//들어온 패킷 값이 잘못되었을 경우 큐를 리셋시킴
		if (wPacketSize < sizeof(WORD) && nowSize >= sizeof(WORD))
		{
			ResetQueue();
			return FALSE;
		}

		if (nowSize < wPacketSize || readToWrite < wPacketSize)
			return FALSE;
		else
			return TRUE;
	}

	//m_pRead로부터 패킷의 크기를 구하는 함수
	//return WORD : 패킷의 크기
	WORD CPacketQueue::GetPacketSize()
	{
		int nBufSize = GetPointerOffset(m_pRead, m_pBuffer + MAX_BUF);
		WORD wPacketSize = 0;

		if (nBufSize < sizeof(WORD))
		{
			char pPacketSize[2];
			pPacketSize[0] = (*m_pRead);
			pPacketSize[1] = (*m_pBuffer);
			wPacketSize = *(reinterpret_cast<WORD*>(pPacketSize));
		}
		else
			wPacketSize = *(reinterpret_cast<WORD*>(m_pRead));

		return wPacketSize;
	}
}