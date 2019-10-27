#pragma once
#include "NetworkObject.h"

namespace SHLib
{
	/*
		Ŭ���� : CUserSession
		�⺻Ŭ���� : CNetworkObject
		Ŭ���̾�Ʈ ��ſ� ���Ǵ� �⺻ ����
	*/
	class CUserSession : public CNetworkObject
	{
	public:
		CUserSession() : CNetworkObject() {}
		virtual ~CUserSession() {}
	
		virtual void OnConstruct() {}
		virtual void OnDestroy() {}

	private:
		virtual void	AddProtocol() = 0; //���������� �����ϴ� �Լ�
		virtual void	OnConnect() {}; //������۽� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnReceive(int _nResult, int _nSize) {} //Recv���� �� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnSend(int _nResult, int _nSize) {} //Send�� ����Ǵ� �Լ�(�����Լ�)
		virtual void	OnClose() {} //��������� ����Ǵ� �Լ�(�����Լ�)
	};
}