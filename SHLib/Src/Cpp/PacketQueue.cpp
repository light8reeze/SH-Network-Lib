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

	//��Ŷ�����͸� ���� �� ó���ϴ� �Լ�(����ȭ)
	//int _nRecvSize : ť�� recv�� ũ��
	void CPacketQueue::OnEnqueue(int _nRecvSize)
	{
		char* writePos = m_pWrite;

		AUTO_LOCK(CLocker, m_Locker);

		//ť�� write������ ��ġ�� ����
		//write�������� ��ġ�� ������ ���κ��̸�
		//������ ���ۺκ����� write�����͸� �Ű��ش�
		if (writePos + _nRecvSize > (m_pBuffer + MAX_BUF - 1))
			m_pWrite = m_pBuffer;
		else
			m_pWrite += _nRecvSize;


		m_nSize += _nRecvSize;
	}

	//ť���� ��Ŷ �����͸� ������ �Լ�
	//return char* : ����� ��Ŷ�� �ּҰ�, ��Ŷ�� ������ nullptr��ȯ
	char* CPacketQueue::Dequeue()
	{
		if (HasPacket() == FALSE)
			return nullptr;

		char* pRead = m_pRead;
		WORD wPacketSize = GetPacketSize();
		WORD wType;

		//��Ŷ�� ũ�Ⱑ ������ ũ�⸦ �ʰ��ҽÿ�
		//������ �պκп��� �����κ��� �߰��� ����
		if (pRead + wPacketSize > (m_pBuffer + MAX_BUF - 1))
		{
			//������ ������ ũ��� �߸� ��Ŷ�� ũ�⸦ ���
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

	//ť�� �����͸� ��� ����� �������ִ� �Լ�
	void CPacketQueue::ResetQueue()
	{
		AUTO_LOCK(CLocker, m_Locker);

		m_pRead = m_pBuffer;
		m_pWrite = m_pBuffer;
		m_nSize = 0;
	}

	//ť���� ���� ũ�⸦ ��ȯ���ִ� �Լ�
	//return int : ť���� ���� ����� ��ȯ
	int CPacketQueue::GetAvailableSize()
	{
		return MAX_BUF - m_nSize;
	}

	//ť���� ó���� ��Ŷ�� �����ϴ��� ��ȯ�ϴ� �Լ�
	//return bool : ��Ŷ ���翩�� ��ȯ
	BOOL CPacketQueue::HasPacket()
	{
		if (m_nSize < sizeof(WORD))
			return FALSE;

		char* pRead = m_pRead;
		char* pWrite = m_pWrite;
		int nowSize = m_nSize;
		WORD wPacketSize = GetPacketSize();
		int readToWrite;

		//read�����Ϳ��� write�����ͱ����� ũ�⸦ ���
		if (pWrite == pRead)
			readToWrite = (m_nSize == 0) ? MAX_BUF : m_nSize;
		else
		{
			char* pStart = (pWrite < pRead) ? pRead - 1 : pRead;
			char* pEnd = (pWrite < pRead) ? pWrite + MAX_BUF : pWrite;
			readToWrite = GetPointerOffset(pStart, pEnd);
		}

		//���� ��Ŷ ���� �߸��Ǿ��� ��� ť�� ���½�Ŵ
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

	//m_pRead�κ��� ��Ŷ�� ũ�⸦ ���ϴ� �Լ�
	//return WORD : ��Ŷ�� ũ��
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