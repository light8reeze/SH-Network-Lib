#pragma once
#include "SHLib.h"

#pragma pack(push, 1)
namespace SHLib
{
	//��Ŷ ��� ����ü
	struct ST_PACKET_HEADER
	{
		WORD wSize;
		WORD wType;
	};
}
#pragma pack(pop, 1)