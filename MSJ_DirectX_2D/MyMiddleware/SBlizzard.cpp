#include "SBlizzard.h"



SBlizzard::SBlizzard()
{
}


SBlizzard::~SBlizzard()
{
}

//

bool SBlizzard::Init()
{
	m_BlizzardRenderer = GetComponent<SpriteRenderer2D>();

	if ( nullptr == m_BlizzardRenderer)
	{
		EAssert(true);
		return false;
	}

	m_BlizzardAnim = GetComponent<Animation2D>();

	if ( nullptr == m_BlizzardAnim )
	{
		EAssert(true);
		return false;
	}

	m_BlizzardAnim->Animation_Create(L"BBase", 0, 6, false, 0.1f);
	m_BlizzardAnim->Animation_Change(L"BBase");

	return true;
}

void SBlizzard::Update()
{
	CheckDeath();
}

void SBlizzard::FinalUpdate()
{

}

//

void SBlizzard::CheckDeath()
{
	if (true == m_BlizzardAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}