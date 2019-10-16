#pragma once
#include <string>

class NameBase
{
public:
	std::wstring m_Name;

public:
	const wchar_t* GetName()
	{
		return m_Name.c_str();
	}

	void SetName(const wchar_t* _Name)
	{
		m_Name = _Name;
	}

	bool Equal(const wchar_t* _Name)
	{
		return m_Name == _Name;
	}

	bool Equal(std::wstring _Name)
	{
		return m_Name == _Name;
	}

public:
	NameBase() {}
	virtual ~NameBase() = 0 {}
};

