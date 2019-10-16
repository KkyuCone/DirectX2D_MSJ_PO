#include "SSoulPenetration.h"
#include <RectCol2D.h>


SSoulPenetration::SSoulPenetration()
{
}


SSoulPenetration::~SSoulPenetration()
{
}

//

bool SSoulPenetration::Init()
{
	m_SoulPeRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SoulPeRenderer)
	{
		EAssert(true);
		return false;
	}

	m_SoulPeAnim = GetComponent<Animation2D>();

	if (nullptr == m_SoulPeAnim)
	{
		EAssert(true);
		return false;
	}

	m_SoulPeAnim->Animation_Create(L"SoulPenetration_Effect", 0, 21, false, 0.1f);
	m_SoulPeAnim->Animation_Change(L"SoulPenetration_Effect");

	return true;
}

void SSoulPenetration::Update()
{
	if (11 == m_SoulPeAnim->CurAnimationFram())
	{
		m_RectCol = GetGameObject()->Component_Add<RectCol2D>(L"Skill_SoulPenetration");
		m_RectCol->Local_SubPosition(Vector4(0.0f, -200.0f, 0.0f, 0.0f));
		m_RectCol->Local_SubScale(Vector4(500.0f, 100.0f, 1.0f, 1.0f));
	}

	EffectDeath();

}

void SSoulPenetration::FinalUpdate()
{

}

//

void SSoulPenetration::EffectDeath()
{
	if (true == m_SoulPeAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
		m_RectCol->Death();
	}
}
