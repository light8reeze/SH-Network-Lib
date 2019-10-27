#include "NetworkIO.h"
#include "Logger.h"

namespace SHLib
{
	CNetworkIO CNetworkIO::m_Instance;

	CNetworkIO::CNetworkIO() : m_hPort(NULL)
	{
	}

	CNetworkIO::~CNetworkIO()
	{
	}

	DWORD CNetworkIO::Initialize()
	{
		m_hPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

		TODO_MODIFY_ERRORCODE;
		return (m_hPort == NULL)? 1 : 0;
	}

	//IOCP�� ������ ����ϴ� �Լ�
	//DWORD _dwKey : IOCP�� ��� �� Ű
	//CSocket* _pSocket : ����� ����
	//return HANDLE : CreateIoCompletionPort�� �����
	HANDLE CNetworkIO::AddIOSocket(IO_KEY _key, CSocket* _pSocket)
	{
		assert(m_hPort != NULL);
		return CreateIoCompletionPort((HANDLE)_pSocket->GetSocket(), m_hPort, _key, 0);
	}

	BOOL CNetworkIO::GetIOCompletion(IO_KEY& _key, COverlapped*& _pOverlapped, DWORD& _dwRecvSize, DWORD _dwWaitTime)
	{
		assert(m_hPort != NULL);
		return GetQueuedCompletionStatus(m_hPort, &_dwRecvSize, reinterpret_cast<PULONG_PTR>(&_key), reinterpret_cast<LPOVERLAPPED*>(&_pOverlapped), _dwWaitTime);
	}

	BOOL CNetworkIO::PostIOCompletion(IO_KEY _key, COverlapped& _overlapped, DWORD _dwSize)
	{
		assert(m_hPort != NULL);
		return PostQueuedCompletionStatus(m_hPort, _dwSize, _key, reinterpret_cast<LPOVERLAPPED>(&_overlapped));
	}
}