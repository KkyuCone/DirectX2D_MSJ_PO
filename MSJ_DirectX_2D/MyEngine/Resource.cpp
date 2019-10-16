#include "Resource.h"
#include "Path.h"



Resource::Resource()
{
}


Resource::~Resource()
{
}

//

std::wstring Resource::GetPath()
{
	return m_Path;
}

const wchar_t* Resource::Const_Path()
{
	return m_Path.c_str();
}

void Resource::SetPath(const wchar_t* _Path)
{
	m_Path = _Path;
}

void Resource::SetPath(const std::wstring& _Path)
{
	m_Path = _Path;
}

// 파일명 풀네임 (확장자까지)
std::wstring Resource::Full_FileName()
{
	return Path::PathToFullFileName(m_Path);
}

// 확장자 
bool Resource::IsExtension(const wchar_t* _Name)
{
	return Path::IsExtention(m_Path, _Name);
}

std::wstring Resource::FileName()
{
	wchar_t ArrDrive[256];
	wchar_t ArrFolder[256];
	wchar_t ArrFile[256];
	wchar_t ArrExtention[256];

	_wsplitpath_s(m_Path.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExtention);

	return ArrFile;
}

// 확장자
std::wstring Resource::Extension()	
{
	wchar_t ArrDrive[256];
	wchar_t ArrFolder[256];
	wchar_t ArrFile[256];
	wchar_t ArrExtention[256];

	_wsplitpath_s(m_Path.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExtention);

	return ArrExtention;
}
