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

	// 타입 이름 반환 - 아스키코드
	std::string Get_TypeName_ASCII()
	{
		return m_TypeInformation->name();
	}

	// 타입 이름 반환 - 유니코드
	std::wstring Get_TypeName_UNICODE()
	{
		std::wstring WidBtyeName = CA2W(m_TypeInformation->name());
		return WidBtyeName;
	}

	// 타입 정보 반환
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

	// ㅇㅅㅇ  상속 관계 때문에 virtual 추가하는고얌!! 
	// virtual 안해주면 상속받은 애의 소멸자가 안불릴수 있다뀨~
	virtual ~TypeBase() = 0;
};

