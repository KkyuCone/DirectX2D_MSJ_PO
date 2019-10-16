#include "TownPortalAniScript.h"



TownPortalAniScript::TownPortalAniScript()
{
}


TownPortalAniScript::~TownPortalAniScript()
{
}

//

bool TownPortalAniScript::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"PortalAni", 0, 7, 0.1f);
	m_Animation->Animation_Change(L"PortalAni");

	return true;
}

void TownPortalAniScript::Update()
{

}

void TownPortalAniScript::FinalUpdate()
{

}