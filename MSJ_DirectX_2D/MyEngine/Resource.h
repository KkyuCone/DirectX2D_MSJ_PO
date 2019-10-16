#pragma once
#include "ReferenceBase.h"
#include "TypeBase.h"
#include "ResourceManager.h"
#include "EnMACRO.h"

#include <string>

class Resource : public ReferenceBase, public TypeBase
{
private:
	template<typename Resource, typename Key>
	friend class ResourceManager;

	class Creator {};
	class Loader {};

private:
	std::wstring m_Path;

public:
	bool IsExtension(const wchar_t* _Name);

public:
	std::wstring GetPath();
	const wchar_t* Const_Path();
	void SetPath(const wchar_t* _Path);
	void SetPath(const std::wstring& _Path);

	std::wstring Full_FileName();
	std::wstring FileName();
	std::wstring Extension();			// »Æ¿Â¿⁄
public:
	Resource();
	virtual ~Resource() = 0;
};

