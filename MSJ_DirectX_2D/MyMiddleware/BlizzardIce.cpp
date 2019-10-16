#include "BlizzardIce.h"



BlizzardIce::BlizzardIce()
{
}


BlizzardIce::~BlizzardIce()
{
}

//

bool BlizzardIce::Init()
{
	m_IceRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_IceRenderer)
	{
		EAssert(true);
		return false;
	}

	m_IceAnim = GetComponent<Animation2D>();

	if (nullptr == m_IceAnim)
	{
		EAssert(true);
		return false;
	}

	m_IceAnim->Animation_Create(L"IceAttack", 0, 23, false, 0.1f);
	m_IceAnim->Animation_Change(L"IceAttack");

	return true;
}

void BlizzardIce::Update()
{
	CheckDeath();
}

void BlizzardIce::FinalUpdate()
{

}

//

void BlizzardIce::CheckDeath()
{
	if (true == m_IceAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}


Animation2D* BlizzardIce::GetCurAniamation()
{
	return m_IceAnim;
}

GameObject* BlizzardIce::GetThisGameObject()
{
	return GetGameObject();
}