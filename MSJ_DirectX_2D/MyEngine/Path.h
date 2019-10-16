#pragma once
#include <unordered_map>
#include <vector>
#include "EnMACRO.h"

class Path
{
private:
	// unordered_map가 map보다 빠르다
	// -> 해쉬 기반의 배열을 통한 맵이기 때문임 ( 비교연산이 아니라 바로 찾기때문 )
	static std::unordered_map<std::wstring, std::wstring> m_PathUnMap;
	static std::wstring m_RootPath;		// 게임이 실행되고 있는 폴더의 경로

	static std::vector<std::wstring> FileNameVector;

	// Init()함수를 호출해줬는지에 대한 여부
	static bool m_bInit;

public:
	static void Init();

	static bool IsExtention(const std::wstring _Path, const wchar_t* _Name);

	static std::wstring PathToFullFileName(const std::wstring& _Path);

	static std::vector<std::wstring> Path::PathToFullFileList(const std::wstring& _Path, const std::wstring& _Filter);

	static std::wstring FindPath(const wchar_t* _Key);

	static bool RootToCreatePath(const wchar_t* _Key);
	static bool RootToCreatePath(const wchar_t* _Key, const wchar_t* _NewFolder);

	static bool PlusCreatePath(const wchar_t* _BaseFolder, const wchar_t* _Folder, const wchar_t* _Key);


public:
	Path() {}
	virtual ~Path() = 0 {}
};

