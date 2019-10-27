#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "Dbghelp")

#include <winsock2.h>
#include <windows.h>
#include <assert.h>
#include <tchar.h>

//#define __DEBUG__

#ifdef SH_IMPORT
	#define SH_EXPORT __declspec(dllimport)
#else
	#define SH_EXPORT __declspec(dllexport)
#endif

#define TODO_MODIFY_ERRORCODE	//�����ڵ� �߰��� ������ �κ�

/*
#ifndef _DEBUG
#define new new(std::nothrow)
#endif
*/

namespace SHLib
{
	//�����Ϳ� �Ҵ�� �޸𸮸� �����ϴ� �Լ�
	//<typename T> : ������ �������� Ÿ��
	//T*& _ptr : ������ �������� ���۷���
	template<typename T>
	inline void SafeDelete(T*& _ptr)
	{
		if (_ptr)
		{
			delete _ptr;
			_ptr = nullptr;
		}
	}

	//�����Ϳ� �Ҵ�� �޸𸮸� �����ϴ� �Լ�(�迭�� ���)
	//<typename T> : ������ �������� Ÿ��
	//T*& _ptr : ������ �������� ���۷���
	template<typename T>
	inline void SafeArrayDelete(T*& _ptr)
	{
		if (_ptr)
		{
			delete[] _ptr;
			_ptr = nullptr;
		}
	}
}