#pragma once
#include "SHLib.h"
#include "NetworkObject.h"

namespace SHLib
{
	/*
		Ŭ���� : CConnecter
		�⺻ Ŭ���� : CNetworkObject

		�ٸ� ������ �����Ͽ� ����ϴ� Ŭ����
	*/
	class SH_EXPORT CConnecter : public CNetworkObject
	{
	private:
		typedef CNetworkObject SUPER;

	private:
		BOOL m_isInitialized;

	public:
		CConnecter();
		virtual ~CConnecter();

		//�������� Ȯ��� super�� �ݵ�� ȣ���� ��
		virtual DWORD	Initialize(const char* _strIP, WORD _nPort);
		void			SetConnect(const char* _strIP, WORD _nPort); //Ŀ������ ������ �����ϴ� �Լ�
		void			Connect();
		int				StartConnect(); //Ŀ������ ������ �����ϴ� �Լ�
		
		virtual void	AddProtocol() {}; //Ŀ������ ��Ŷ �̺�Ʈ ������ ���ִ� �Լ� ���� Ŭ�������� �� �Լ����� ��Ŷ �̺�Ʈ�� ���
		virtual void	OnConnect() {};  //Ŀ���� ����� ����Ǵ� �Լ�
		virtual int		OnConnectFail() { return -1; };  //Ŀ���� ���� ���н� ����Ǵ� �Լ�
	};
}