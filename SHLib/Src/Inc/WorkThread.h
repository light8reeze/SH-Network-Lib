#pragma once
#include "NetworkIO.h"
#include "Thread.h"
#include "NetworkObject.h"
#include <functional>

namespace SHLib
{
	//��Ʈ��ũ IO�� �����
	enum IO_RESULT
	{
		E_IO_FAIL = 0,
		E_IO_SUCCESS,
		E_IO_RESULTCNT
	};

	/*
		Ŭ���� : CWorkThread
		�⺻Ŭ���� : CThread

		��Ʈ��ũ�� Recv�� ����ϴ� ������
	*/
	class SH_EXPORT CWorkThread : public CThread
	{
	private:
		typedef std::function<int(const CNetworkObject*, const COverlapped*)> IO_RESULT_PROC; //��Ʈ��ũ IO�̺�Ʈ �Լ�

	private:
		#pragma warning(disable : 4251)
		IO_RESULT_PROC	m_mIOProc[E_IO_RESULTCNT];
		#pragma warning(default : 4251)

	public:
		CWorkThread();
		~CWorkThread();

		void SetIOCompleteProc(IO_RESULT _eIOResult, IO_RESULT_PROC _fnIOEvent); //ReceiveThread�� IO��� �̺�Ʈ�� �����ϴ� �Լ�

	private:
		DWORD OnAccept(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwResult);
		DWORD OnRecv(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwRecvSize);
		DWORD OnSend(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwSendSize);
		DWORD OnProcess(CNetworkObject* _pNetworkObject);
		DWORD OnDisconnect(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped);
		DWORD OnConnect(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped);
		DWORD OnError(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped);
		DWORD OnTimer(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwSize);

		bool ExistEvent(IO_RESULT _eIOResult); //Ư�� �̺�Ʈ�� �����ϴ��� Ȯ���ϴ� �Լ�

	protected:
		INT Main(); //Recv ó���� ����ϴ� �Լ�(������ ���)
	};
}