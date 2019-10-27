#pragma once
#include "SHLib.h"

#pragma pack(push, 1)
namespace SHLib
{
	//패킷 헤더 구조체
	struct ST_PACKET_HEADER
	{
		WORD wSize;
		WORD wType;
	};
}
#pragma pack(pop, 1)