#pragma once

namespace SHLib
{
	/*
		클래스 : CConstraint
		<typename TDriven> : 파생클래스
		<typename TBase> : 기본클래스

		클래스의 템플릿을 <TBase>클래스의 파생클래스로 제한하는 용도로 사용되는 클래스
		<TDriven>클래스가 <TBase>의 파생클래스가 아닐때 컴파일 에러를 발생시킨다
	*/
	template<typename TDriven, typename TBase>
	class CConstraint
	{
	private:
		static void ClassConstraint(TDriven* _ptr) //클래스 타입을 검사하는 함수
		{
			//D*를 B*로 캐스팅
			//캐스팅 실패시 컴파일 에러 발생
			TBase* pBase = _ptr;
		}

	public:
		CConstraint() { void(*func)(TDriven*) = ClassConstraint; } //CConstraint의 생성자 ClassConstraint를 인스턴스화 하여 타입을 검사한다.
		~CConstraint() {};
	};
}