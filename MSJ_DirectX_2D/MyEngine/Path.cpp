#include "Path.h"
#include "EnMACRO.h"
#include <Windows.h>
#include <io.h>

std::unordered_map<std::wstring, std::wstring> Path::m_PathUnMap;
std::wstring Path::m_RootPath;		// 게임이 실행되고 있는 폴더의 경로
std::vector<std::wstring> Path::FileNameVector;

bool Path::m_bInit = false;
// 

void Path::Init()
{
	wchar_t ArrString[256] = {};
	GetCurrentDirectoryW(sizeof(wchar_t) * 256, ArrString);
	m_RootPath = ArrString;

	size_t CutCount = m_RootPath.find_last_of(L'\\', m_RootPath.size());
	m_RootPath = m_RootPath.replace(CutCount, m_RootPath.size(), L"\\Bin\\");

	m_bInit = true;		// 초기화해줬으니깡~ true~
}

// 확장자
bool Path::IsExtention(const std::wstring _Path, const wchar_t* _Name)
{
	wchar_t ArrDrive[256];
	wchar_t ArrFolder[256];
	wchar_t ArrFile[256];
	wchar_t ArrExtention[256];


	// 파일명 및 파일 확장자 추출 
	// _wsplitpath_s ( const wchar_t * path,	// 전체 경로
	//				   wchar_t * drive,			// 드라이브 문자, 드라미브 문자가 필요하지 않는 경우 NULL
	//				   wchar_t *dir,			// 디렉토리 경로 후행 슬래시 포함. (백슬래시/) 필요하지 않는 경우 NULL
	//				   wchar_t * fname,			// 확장자명 없이 기본 파일 이름, 필요하지 않는 경우 NULL
	//				   wchar_t * ext,			// 확장자명(.포함), 필요하지 않는 경우 NULL
	//  )


	_wsplitpath_s(_Path.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExtention);

	std::wstring Value = ArrExtention;

	return Value == _Name;
}

//
std::vector<std::wstring> Path::PathToFullFileList(const std::wstring& _Path, const std::wstring& _Filter)
{
	FileNameVector.clear();

	int a = 0;
	std::wstring searchingW = _Path + _Filter;
	std::string searchingM;
	searchingM.assign(searchingW.begin(), searchingW.end());		// 매개변수때문에 멀티바이트로 일단 변환

	std::vector<std::string> FileNameVectorM;

	_finddata_t FindData;
	intptr_t Handle = _findfirst(searchingM.c_str(), &FindData);		// 현재 폴더 내 모든 파일 찾기

	if (Handle == -1)
		return FileNameVector;

	int result = 0;

	do
	{
		FileNameVectorM.push_back(FindData.name);
		result = _findnext(Handle, &FindData);

	} while (result != -1);

	_findclose(Handle);

	// 여기까지 찾음

	std::wstring FileNameW;

	for (size_t i = 0; i < FileNameVectorM.size(); i++)
	{
		FileNameW.assign(FileNameVectorM[i].begin(), FileNameVectorM[i].end());
		FileNameVector.push_back(FileNameW);			// 파일 리스트로 넣어주기
	}

	return FileNameVector;
}

// 파일명 풀네임
std::wstring Path::PathToFullFileName(const std::wstring& _Path)
{
	if (false == m_bInit)
	{
		Init();		// 초기화해주지 않았다면 초기화~
	}

	wchar_t ArrDrive[256];
	wchar_t ArrFolder[256];
	wchar_t ArrFile[256];
	wchar_t ArrExtention[256];


	// 파일명 및 파일 확장자 추출 
	// _wsplitpath_s ( const wchar_t * path,	// 전체 경로
	//				   wchar_t * drive,			// 드라이브 문자, 드라미브 문자가 필요하지 않는 경우 NULL
	//				   wchar_t *dir,			// 디렉토리 경로 후행 슬래시 포함. (백슬래시/) 필요하지 않는 경우 NULL
	//				   wchar_t * fname,			// 확장자명 없이 기본 파일 이름, 필요하지 않는 경우 NULL
	//				   wchar_t * ext,			// 확장자명(.포함), 필요하지 않는 경우 NULL
	//  )


	_wsplitpath_s(_Path.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExtention);

	std::wstring FullFileName = ArrFile;
	FullFileName += ArrExtention;

	return FullFileName;
}

std::wstring Path::FindPath(const wchar_t* _Key)
{
	if (false == m_bInit)
	{
		Init();
	}

	std::unordered_map<std::wstring, std::wstring>::iterator FindIter = m_PathUnMap.find(_Key);

	if (FindIter != m_PathUnMap.end())
	{
		return FindIter->second;
	}

	return L"";
}

bool Path::RootToCreatePath(const wchar_t* _Key)
{
	return RootToCreatePath(_Key, _Key);
}

bool Path::RootToCreatePath(const wchar_t* _Key, const wchar_t* _NewFolder)
{
	if (false == m_bInit)
	{
		Init();
	}

	std::unordered_map<std::wstring, std::wstring>::iterator FindIter = m_PathUnMap.find(_Key);

	if (FindIter != m_PathUnMap.end())
	{
		EAssert(true);
		return false;
	}

	m_PathUnMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Key, m_RootPath + _NewFolder + L"\\"));

	return true;
}


bool Path::PlusCreatePath(const wchar_t* _BaseFolder, const wchar_t* _Folder, const wchar_t* _Key)
{
	std::unordered_map<std::wstring, std::wstring>::iterator FindIter = m_PathUnMap.find(_Key);

	if (FindIter != m_PathUnMap.end())
	{
		EAssert(true);
		return false;
	}


	FindIter = m_PathUnMap.find(_BaseFolder);

	if (FindIter == m_PathUnMap.end())
	{
		EAssert(true);
		return false;
	}

	// 이거 기억하세요.
	// \\data\\ \\Back

	m_PathUnMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Key, FindIter->second + _Folder + L"\\"));
	return true;
}
