#include "TypeBase.h"



TypeBase::TypeBase() : m_TypeInformation(nullptr)
{
}


TypeBase::~TypeBase()
{
}

//
// RTTI (Run-Time Type Infomation )
// : ��Ÿ�� �� ��ü�� �ڷ����� ���� ������ �巯���� C++ ��Ŀ����(����)�� ���Ѵ�.
void TypeBase::RTTISetting()
{
 	m_TypeInformation = &(typeid(*this));
	//size_t b = typeid(bool).hash_code();
	const char* ab = typeid(*this).name();
}
