#include "BlizzardIcePillar.h"



BlizzardIcePillar::BlizzardIcePillar()
{
}


BlizzardIcePillar::~BlizzardIcePillar()
{
}

//

bool BlizzardIcePillar::Init()
{
	m_IcePillarRenderer = GetComponent<SpriteRenderer2D>();

	if ( nullptr == m_IcePillarRenderer)
	{
		EAssert(true);
		return false;
	}

	m_IcePillarAnim = GetComponent<Animation2D>();

	if (nullptr == m_IcePillarAnim)
	{
		EAssert(true);
		return false;
	}

	m_IcePillarAnim->Animation_Create(L"Pillar", 0, 21, false, 0.1f);
	m_IcePillarAnim->Animation_Change(L"Pillar");

	return true;
}

void BlizzardIcePillar::Update()
{
	CheckDeath();
}

void BlizzardIcePillar::FinalUpdate()
{

}


//
void BlizzardIcePillar::CheckDeath()
{
	if (true == m_IcePillarAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}