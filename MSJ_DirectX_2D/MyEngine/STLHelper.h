#pragma once
#include "ReferenceBase.h"

template<typename RETURN, typename MAP, typename KEY>
RETURN Map_Find_PTR(MAP& _Map, const KEY& _Key)
{
	MAP::iterator Find_Iter = _Map.find(_Key);

	if (Find_Iter != _Map.end())
	{
		return Find_Iter->second;
	}

	return nullptr;
}