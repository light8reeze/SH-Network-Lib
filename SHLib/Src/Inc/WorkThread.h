#pragma once
#include "NetworkIO.h"
#include "Thread.h"
#include "NetworkObject.h"
#include <functional>

namespace SHLib
{
	//네트워크 IO의 결과값
	enum IO_RESULT
	{
		E_IO_FAIL = 0,
		E_IO_SUCCESS,
		E_IO_RESULTCNT
	};

	/*
		클래스 : CWorkThread
		기본클래스 : CThread

		네트워크의 Recv를 담당하는 쓰레드
	*/
	class SH_EXPORT CWorkThread : public CThread
	{
	private:
		typedef std::function<int(const CNetworkObject*, const COverlapped*)> IO_RESULT_PROC; //네트워크 IO이벤트 함수

	private:
		#pragma warning(disable : 4251)
		IO_RESULT_PROC	m_mIOProc[E_IO_RESULTCNT];
		#pragma warning(default : 4251)

	public:
		CWorkThread();
		~CWorkThread();

		void SetIOCompleteProc(IO_RESULT _eIOResult, IO_RESULT_PROC _fnIOEvent); //ReceiveThread의 IO결과 이벤트를 설정하는 함수

	private:
		DWORD OnAccept(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwResult);
		DWORD OnRecv(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwRecvSize);
		DWORD OnSend(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwSendSize);
		DWORD OnProcess(CNetworkObject* _pNetworkObject);
		DWORD OnDisconnect(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped);
		DWORD OnConnect(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped);
		DWORD OnError(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped);
		DWORD OnTimer(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwSize);

		bool ExistEvent(IO_RESULT _eIOResult); //특정 이벤트가 존재하는지 확인하는 함수

	protected:
		INT Main(); //Recv 처리를 담당하는 함수(쓰레드 사용)
	};
}