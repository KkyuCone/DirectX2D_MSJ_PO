#include "CataclysmHit.h"



CataclysmHit::CataclysmHit()
{
}


CataclysmHit::~CataclysmHit()
{
}

//

bool CataclysmHit::Init()
{
	m_CataHitRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_CataHitRenderer)
	{
		EAssert(true);
		return false;
	}

	m_CataHitAnim = GetComponent<Animation2D>();

	if (nullptr == m_CataHitAnim)
	{
		EAssert(true);
		return false;
	}

	return true;
}

void CataclysmHit::Update()
{

}

void CataclysmHit::FinalUpdate()
{

}

//
