#include "ReferenceBase.h"



ReferenceBase::ReferenceBase() : m_ref(0), m_bDebug(false)
{
}


ReferenceBase::~ReferenceBase()
{
}


void ReferenceBase::Add()
{
	++m_ref;
}

void ReferenceBase::Dec()
{
	--m_ref;

	if (0 >= m_ref) 
	{
		delete this;
	}
}