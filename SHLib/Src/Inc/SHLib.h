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

#define TODO_MODIFY_ERRORCODE	//에러코드 추가시 수정할 부분

/*
#ifndef _DEBUG
#define new new(std::nothrow)
#endif
*/

namespace SHLib
{
	//포인터에 할당된 메모리를 제거하는 함수
	//<typename T> : 해제할 포인터의 타입
	//T*& _ptr : 해제할 포인터의 레퍼런스
	template<typename T>
	inline void SafeDelete(T*& _ptr)
	{
		if (_ptr)
		{
			delete _ptr;
			_ptr = nullptr;
		}
	}

	//포인터에 할당된 메모리를 제거하는 함수(배열에 사용)
	//<typename T> : 해제할 포인터의 타입
	//T*& _ptr : 해제할 포인터의 레퍼런스
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