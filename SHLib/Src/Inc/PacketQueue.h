#pragma once
#include "SHLib.h"
#include "Locker.h"
#include "Socket.h"

#define MAX_BUF	  0xFFFE //��Ŷ ť�� �ִ� ũ��
#define TEMP_SIZE 1000 //�ӽù����� ũ��

namespace SHLib
{
	/*
		Ŭ���� : CPacketQueue
		��Ŷ�� �������۱����� �����ϴ� Ŭ����
	*/
	class SH_EXPORT CPacketQueue
	{
	protected:
		char	m_pBuffer[MAX_BUF]; //ť�� ��ü ����
		char	m_pTemp[TEMP_SIZE]; //ť�� �ӽ� ����
		char*	m_pRead; //ť�� ������������ ������
		char*	m_pWrite; //ť�� �� �������� ������ 
		WORD	m_nSize; //���� ť�� ����� ũ��

		CLocker m_Locker; //ť�� ����ȭ Ŭ����

	public:
		CPacketQueue();
		~CPacketQueue();

		void	OnEnqueue(int _nRecvSize); //��Ŷ�����͸� ���� �� ó���ϴ� �Լ�
		char*	Dequeue(); //ť���� ��Ŷ �����͸� ������ �Լ�
		void	ResetQueue(); //ť�� �����͸� ��� ����� �������ִ� �Լ�
		int		GetAvailableSize(); //ť���� ���� ũ�⸦ ��ȯ���ִ� �Լ�
		BOOL	HasPacket(); //ť���� ó���� ��Ŷ�� �����ϴ��� ��ȯ�ϴ� �Լ�

	private:
		WORD	GetPacketSize(); //m_pRead�κ��� ��Ŷ�� ũ�⸦ ���ϴ� �Լ�
	};

	//_pStart���� _pEnd������ ����� �����ִ� �Լ�
	//char* _pStart : ���� ������
	//char* _pEnd : �� ������
	//return int : �� ������ ������ ũ��(����)
	inline UINT GetPointerOffset(char* _pStart, char* _pEnd)
	{
		return static_cast<UINT>(_pEnd - _pStart);
	}
}