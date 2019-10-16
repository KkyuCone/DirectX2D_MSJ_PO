#include "CataclysmEffectValue.h"



CataclysmEffectValue::CataclysmEffectValue()
{
}


CataclysmEffectValue::~CataclysmEffectValue()
{
}

//

bool CataclysmEffectValue::Init()
{
	m_CataEffectRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_CataEffectRenderer)
	{
		EAssert(true);
		return false;
	}

	m_CataEffectAnim = GetComponent<Animation2D>();

	if (nullptr == m_CataEffectAnim)
	{
		EAssert(true);
		return false;
	}

	m_CataEffectAnim->Animation_Create(L"Repeat", 0, 14, false, 0.1f);
	m_CataEffectAnim->Animation_Change(L"Repeat");

	return true;
}

void CataclysmEffectValue::Update()
{
	IsEndAnimation();
}

void CataclysmEffectValue::FinalUpdate()
{

}

//

void CataclysmEffectValue::IsEndAnimation()
{
	if (true == m_CataEffectAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}