#pragma once
#include "SHLib.h"
#include "Thread.h"

namespace SHLib
{
	/*
		Ŭ���� : CAcceptor

		������ Accept�� ����ϴ� Ŭ����
	*/
	class SH_EXPORT CAcceptor : public CThread
	{
	private:
		typedef CThread SUPER;

	private:
		CSocket	m_AcceptSocket;	//RunBehaviour�� ����� ����

	public:
		CAcceptor(); //Acceptor�� ������ �����Ѵ�
		~CAcceptor();

		HANDLE Initialize(const char* _strIP, int _nPort);
		HANDLE Initialize(CSocket& _socket);
		void Clear(); //Acceptor�� �����ϴ� �Լ�

		CSocket& GetSocket();

	protected:
		INT	 Main(); //������ ������ RunBehaviour�� �����ϴ� �Լ�(������ ���)
	};
}

#include "Acceptor.Inl"