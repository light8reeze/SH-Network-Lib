#include "WorkThread.h"
#include "SessionManager.h"
#include "Connecter.h"
#include "MainThread.h"
#include "TimeScheduler.h"

namespace SHLib
{
	CWorkThread::CWorkThread() : CThread(), m_mIOProc()
	{
	}

	CWorkThread::~CWorkThread()
	{
		StopThread();
	}

	//CWorkThread�� IO��� �̺�Ʈ�� �����ϴ� �Լ�
	//IO_RESULT _eIOResult : ������ ��� ��
	//IO_RESULT_PROC _fnIOEvent : ������ �Լ� ������
	void CWorkThread::SetIOCompleteProc(IO_RESULT _eIOResult, IO_RESULT_PROC _fnIOEvent)
	{
		m_mIOProc[_eIOResult] = _fnIOEvent;
	}

	DWORD CWorkThread::OnAccept(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwResult)
	{
		assert(_pNetworkObject->IsState(E_ACCEPT) == TRUE);

		_pNetworkObject->Connect();
		GetSingleton<CSessionManager>().OnUserConnect(static_cast<CUserSession*>(_pNetworkObject));
		return 0;
	}

	DWORD CWorkThread::OnRecv(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwRecvSize)
	{
		if (_pNetworkObject->IsState(E_DISCONNECT) == TRUE)
			return 0;

		int result = _pNetworkObject->Receive(_dwRecvSize);

		if (result == SOCKET_ERROR)
			_pNetworkObject->OnDisconnect();

		_pNetworkObject->OnReceiveComplete(_pOverlapped->wsaBuf.buf, _pOverlapped->wsaBuf.len);

		if (_pNetworkObject->IsState(E_DISCONNECT) == FALSE  && _pNetworkObject->IsState(E_PROCESSING) == FALSE && _pNetworkObject->HasPacket() == TRUE)
		{
			_pNetworkObject->SetState(E_PROCESSING);
			BOOL bResult = GetSingleton<CNetworkIO>().PostIOCompletion(reinterpret_cast<IO_KEY>(_pNetworkObject), _pNetworkObject->GetSocket().GetOverlapped(E_IO_PROCESS), _dwRecvSize);
			
			if (bResult == FALSE)
				_pNetworkObject->ResetState(E_PROCESSING);
		}

		return 0;
	}

	DWORD CWorkThread::OnSend(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwSendSize)
	{
		if (_pNetworkObject->IsState(E_DISCONNECT) == TRUE)
			return 1;

		_pNetworkObject->OnSendComplete(_pOverlapped->wsaBuf.buf, _pOverlapped->wsaBuf.len);
		return 0;
	}

	DWORD CWorkThread::OnProcess(CNetworkObject* _pNetworkObject)
	{
		if (_pNetworkObject->IsState(E_DISCONNECT) == TRUE)
		{
			_pNetworkObject->ResetState(E_PROCESSING);
			return 1;
		}

		_pNetworkObject->PacketProcess();
		_pNetworkObject->ResetState(E_PROCESSING);
		return 0;
	}

	DWORD CWorkThread::OnDisconnect(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped)
	{
		switch (_pOverlapped->nIOMode)
		{
		case E_IO_RECV:
			_pNetworkObject->ResetState(E_RECEIVING);
			break;

		case E_IO_SEND:
			_pNetworkObject->ResetState(E_SENDING);
			break;

		case E_IO_PROCESS:
			_pNetworkObject->ResetState(E_PROCESSING);
			break;

		case E_IO_TIMER:
			CTimeElement * pElement = reinterpret_cast<CTimeElement*>(_pOverlapped);
			GetSingleton<CTimeScheduler>().ReturnElement(pElement);
			_pNetworkObject->ResetState(E_TIMEEVENT);
			break;
		}

		_pNetworkObject->OnDisconnect();
		return 0;
	}

	DWORD CWorkThread::OnConnect(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped)
	{
		CConnecter* pConnecter = dynamic_cast<CConnecter*>(_pNetworkObject);

		if (pConnecter == nullptr)
			return 1;

		pConnecter->Connect();
		return 0;
	}

	DWORD CWorkThread::OnError(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped)
	{
		//printf("GQCS Error Message : %d\n", WSAGetLastError());

		if (ExistEvent(E_IO_FAIL) == true)
			m_mIOProc[E_IO_FAIL](_pNetworkObject, _pOverlapped);

		OnDisconnect(_pNetworkObject, _pOverlapped);

		TODO_MODIFY_ERRORCODE;
		return GetLastError();
	}

	DWORD CWorkThread::OnTimer(CNetworkObject* _pNetworkObject, COverlapped* _pOverlapped, DWORD _dwSize)
	{
		CTimeElement* pElement = reinterpret_cast<CTimeElement*>(_pOverlapped);

		GetSingleton<CTimeScheduler>().ReturnElement(pElement);
		return 0;
	}

	//Recv ó���� ����ϴ� �Լ�(������ ���)
	//return int : ������ ���� �ڵ�
	INT CWorkThread::Main()
	{
		CNetworkObject*		pObject		= nullptr;
		COverlapped*		pOverlapped = nullptr;
		BOOL				bIOResult	= FALSE;
		DWORD				dwSize		= 0;
		IO_KEY				pIOKey		= 0;
		DWORD				dwExitCode	= 0;

		while (TRUE)
		{
			pObject		= nullptr;
			pOverlapped = nullptr;
			bIOResult	= FALSE;
			dwSize		= 0;
			pIOKey		= 0;

			bIOResult = GetSingleton<CNetworkIO>().GetIOCompletion(pIOKey, pOverlapped, dwSize);
			pObject = reinterpret_cast<CNetworkObject*>(pIOKey);

			if (pObject == nullptr || pObject->IsState(E_NOTCONNECTED) == TRUE)
			{
				TODO_MODIFY_ERRORCODE;
				assert(pObject != nullptr);
				assert(pObject->IsState(E_NOTCONNECTED) == FALSE);

				dwExitCode = 1;
				break;
			}

			if (dwSize == 0 || bIOResult == FALSE)
			{
				//�Ϻη� 0����Ʈ IO�� �ɾ��� ��� ����
				if (!((dwSize == 0) && (pOverlapped->wsaBuf.len == 0) && (bIOResult == TRUE)))
				{
					OnError(pObject, pOverlapped);
					continue;
				}
			}

			if (pObject->IsState(E_DISCONNECT))
			{
				OnDisconnect(pObject, pOverlapped);
				continue;
			}

			assert(pOverlapped != nullptr);
			switch (pOverlapped->nIOMode)
			{
			case E_IO_ACCEPT:
				OnAccept(pObject, pOverlapped, dwSize);
				break;

			case E_IO_CONNECT:
				OnConnect(pObject, pOverlapped);
				break;

			case E_IO_RECV:
				pObject->ResetState(E_RECEIVING);
				OnRecv(pObject, pOverlapped, dwSize);
				break;

			case E_IO_SEND:
				pObject->ResetState(E_SENDING);
				//OnSend(pObject, pOverlapped, dwSize);
				break;

			case E_IO_PROCESS:
				OnProcess(pObject);
				break;

			case E_IO_TIMER:
				OnTimer(pObject, pOverlapped, dwSize);
				pObject->ResetState(E_TIMEEVENT);
				break;
			}

			if (ExistEvent(E_IO_SUCCESS) == true)
				m_mIOProc[E_IO_SUCCESS](pObject, pOverlapped);
		}
		return dwExitCode;
	}

	//Ư�� �̺�Ʈ�� �����ϴ��� Ȯ���ϴ� �Լ�
	//IO_RESULT _eIOResult : Ȯ���� IO_RESULT��
	//return bool : �̺�Ʈ ���� ����
	bool CWorkThread::ExistEvent(IO_RESULT _eIOResult)
	{
		return (m_mIOProc[_eIOResult] != nullptr);
	}
}