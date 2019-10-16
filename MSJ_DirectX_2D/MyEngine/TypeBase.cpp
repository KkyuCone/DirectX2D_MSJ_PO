#include "TypeBase.h"



TypeBase::TypeBase() : m_TypeInformation(nullptr)
{
}


TypeBase::~TypeBase()
{
}

//
// RTTI (Run-Time Type Infomation )
// : 런타임 시 객체의 자료형에 대한 정보를 드러내는 C++ 매커니즘(구조)을 말한다.
void TypeBase::RTTISetting()
{
 	m_TypeInformation = &(typeid(*this));
	//size_t b = typeid(bool).hash_code();
	const char* ab = typeid(*this).name();
}
