#pragma once
#include "SHLib.h"
#include <functional>

namespace SHLib
{
	/*
		클래스 : CThread

		쓰레드를 설정하고, 실행, 정지하는 클래스
	*/
	class SH_EXPORT CThread
	{
	private:
		HANDLE			m_hThread; //쓰레드의 핸들
		BOOL			m_isRun; //쓰레드가 실행되는 여부
		BOOL			m_isExit; //쓰레드 종료 여부
		DWORD			m_dwSwitchingTime; //컨텍스트 스위칭 주기
		
	protected:
		LPVOID	m_pArgs; //쓰레드에 사용되는 인수
		HANDLE	m_hEvent;

	public:
		CThread();
		virtual ~CThread();

		virtual HANDLE	Initialize(); //쓰레드를 초기화하는 함수
		void	SetArguments(LPVOID _pArg);
		HANDLE	StartThread(); //쓰레드를 생성하여 실행하는 함수
		void	RestartThread(); //쓰레드를 재시작하는 함수
		void	StopThread(); //쓰레드를 정지하는 함수
		void	ExitThread(); //쓰레드를 종료하는 함수
		void	ContextSwitching(); //컨텍스트 스위칭 함수
		HANDLE	GetHandle(); //쓰레드의 핸들을 반환하는 함수
		HANDLE	GetEvent();
		BOOL	IsRun(); //쓰레드가 실행되는 여부를 반환하는 함수
		BOOL	IsExit(); //쓰레드가 종료되었는지 여부를 반환하는 함수

		static unsigned WINAPI	ThreadExecute(LPVOID _pArgs); //CThread내부 쓰레드에서 실행되는 함수

	protected:
		virtual INT Main() = 0;
	};
}