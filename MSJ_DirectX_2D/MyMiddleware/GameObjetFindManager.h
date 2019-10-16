#pragma once
#include <GameObject.h>
#include <map>
#include <string>
#include <STLHelper.h>

class GameObjetFindManager
{
private:
	static std::map<std::wstring, My_PTR<GameObject>> m_FindMap;

public:
	static void AddFindGameObject(const wchar_t* _Name, My_PTR<GameObject> _Object)
	{
		m_FindMap.insert(std::map<std::wstring, My_PTR<GameObject>>::value_type(_Name, _Object));
	}

	static My_PTR<GameObject> FindGameObject(const wchar_t* _Name)
	{
		return Map_Find_PTR<My_PTR<GameObject>>(m_FindMap, _Name);
	}
public:
	GameObjetFindManager();
	~GameObjetFindManager();
};

