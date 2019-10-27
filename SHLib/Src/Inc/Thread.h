#pragma once
#include "SHLib.h"
#include <functional>

namespace SHLib
{
	/*
		Ŭ���� : CThread

		�����带 �����ϰ�, ����, �����ϴ� Ŭ����
	*/
	class SH_EXPORT CThread
	{
	private:
		HANDLE			m_hThread; //�������� �ڵ�
		BOOL			m_isRun; //�����尡 ����Ǵ� ����
		BOOL			m_isExit; //������ ���� ����
		DWORD			m_dwSwitchingTime; //���ؽ�Ʈ ����Ī �ֱ�
		
	protected:
		LPVOID	m_pArgs; //�����忡 ���Ǵ� �μ�
		HANDLE	m_hEvent;

	public:
		CThread();
		virtual ~CThread();

		virtual HANDLE	Initialize(); //�����带 �ʱ�ȭ�ϴ� �Լ�
		void	SetArguments(LPVOID _pArg);
		HANDLE	StartThread(); //�����带 �����Ͽ� �����ϴ� �Լ�
		void	RestartThread(); //�����带 ������ϴ� �Լ�
		void	StopThread(); //�����带 �����ϴ� �Լ�
		void	ExitThread(); //�����带 �����ϴ� �Լ�
		void	ContextSwitching(); //���ؽ�Ʈ ����Ī �Լ�
		HANDLE	GetHandle(); //�������� �ڵ��� ��ȯ�ϴ� �Լ�
		HANDLE	GetEvent();
		BOOL	IsRun(); //�����尡 ����Ǵ� ���θ� ��ȯ�ϴ� �Լ�
		BOOL	IsExit(); //�����尡 ����Ǿ����� ���θ� ��ȯ�ϴ� �Լ�

		static unsigned WINAPI	ThreadExecute(LPVOID _pArgs); //CThread���� �����忡�� ����Ǵ� �Լ�

	protected:
		virtual INT Main() = 0;
	};
}