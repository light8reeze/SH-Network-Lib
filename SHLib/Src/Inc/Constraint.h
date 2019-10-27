#pragma once

namespace SHLib
{
	/*
		Ŭ���� : CConstraint
		<typename TDriven> : �Ļ�Ŭ����
		<typename TBase> : �⺻Ŭ����

		Ŭ������ ���ø��� <TBase>Ŭ������ �Ļ�Ŭ������ �����ϴ� �뵵�� ���Ǵ� Ŭ����
		<TDriven>Ŭ������ <TBase>�� �Ļ�Ŭ������ �ƴҶ� ������ ������ �߻���Ų��
	*/
	template<typename TDriven, typename TBase>
	class CConstraint
	{
	private:
		static void ClassConstraint(TDriven* _ptr) //Ŭ���� Ÿ���� �˻��ϴ� �Լ�
		{
			//D*�� B*�� ĳ����
			//ĳ���� ���н� ������ ���� �߻�
			TBase* pBase = _ptr;
		}

	public:
		CConstraint() { void(*func)(TDriven*) = ClassConstraint; } //CConstraint�� ������ ClassConstraint�� �ν��Ͻ�ȭ �Ͽ� Ÿ���� �˻��Ѵ�.
		~CConstraint() {};
	};
}