#pragma once
#include "GameInstance.h"

namespace Client
{
	typedef struct tagBlockDesc
	{
		BLOCKTYPE eBlockType{ BLOCKTYPE::BLOCKTYPE_END };
		_float3 fPosition{ 0.f, 0.f, 0.f };
	}BLOCKDESC;
}