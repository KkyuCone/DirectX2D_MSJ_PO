#pragma once
#include <typeinfo.h>
#include <string>
#include <atlstr.h>

class TypeBase
{
public:
	const type_info* m_TypeInformation;

public:
	void RTTISetting();

	// Ÿ�� �̸� ��ȯ - �ƽ�Ű�ڵ�
	std::string Get_TypeName_ASCII()
	{
		return m_TypeInformation->name();
	}

	// Ÿ�� �̸� ��ȯ - �����ڵ�
	std::wstring Get_TypeName_UNICODE()
	{
		std::wstring WidBtyeName = CA2W(m_TypeInformation->name());
		return WidBtyeName;
	}

	// Ÿ�� ���� ��ȯ
	const type_info* Get_TypeInfo()
	{
		return m_TypeInformation;
	}

	template<typename T>
	bool IsTypeEqual()
	{
		return m_TypeInformation == &typeid(T);
	}

public:
	TypeBase();

	// ������  ��� ���� ������ virtual �߰��ϴ°��!! 
	// virtual �����ָ� ��ӹ��� ���� �Ҹ��ڰ� �ȺҸ��� �ִٲ�~
	virtual ~TypeBase() = 0;
};

