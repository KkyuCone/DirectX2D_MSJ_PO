#pragma once
#include <unordered_map>
#include <vector>
#include "EnMACRO.h"

class Path
{
private:
	// unordered_map�� map���� ������
	// -> �ؽ� ����� �迭�� ���� ���̱� ������ ( �񱳿����� �ƴ϶� �ٷ� ã�⶧�� )
	static std::unordered_map<std::wstring, std::wstring> m_PathUnMap;
	static std::wstring m_RootPath;		// ������ ����ǰ� �ִ� ������ ���

	static std::vector<std::wstring> FileNameVector;

	// Init()�Լ��� ȣ����������� ���� ����
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

