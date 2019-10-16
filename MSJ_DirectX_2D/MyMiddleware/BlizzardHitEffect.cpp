#include "BlizzardHitEffect.h"
#include <GameObject.h>
#include <RectCol2D.h>
#include "BlizzardIce.h"
#include <Scene.h>

BlizzardHitEffect::BlizzardHitEffect() : m_BlizzardKind(0), m_bRect(false)
{
}


BlizzardHitEffect::~BlizzardHitEffect()
{
}

//

bool BlizzardHitEffect::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}
	m_Renderer->SetSprite(L"EmptyItem.png", 0);


	m_Animation = GetComponent<Animation2D>();
	if (nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"None", 0, 0, true, 0.1f);
	m_Animation->Animation_Create(L"Effect", 0, 5, false, 0.1f);
	m_Animation->Animation_Change(L"None");

	return true;
}

void BlizzardHitEffect::Update()
{
	if (false == m_bRect)
	{
		Create_Collision();
	}

	if (L"Effect" == m_Animation->CurAnimationName())
	{
		if (true == m_Animation->IsEndAnimation())
		{
			GetGameObject()->Death();
		}
	}
}

void BlizzardHitEffect::FinalUpdate()
{

}

//

void BlizzardHitEffect::SetBlizzardIceAniScript(BlizzardIce* _Script)
{
	m_BlizzardAniScript = _Script;
}

void BlizzardHitEffect::Create_Collision()
{
	int CurFrame = m_BlizzardAniScript->GetCurAniamation()->CurAnimationFram();

	if (15 == CurFrame)
	{
		m_Renderer->SetSprite(L"Blizzard_Hit.png", 0);
		m_Animation->Animation_Change(L"Effect");
		m_bRect = true;
		Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();

		if (1 == m_BlizzardKind)
		{
			m_RectCol = GetGameObject()->Component_Add<RectCol2D>(L"Skill_BlizzardCol");
			m_RectCol->Local_SubPosition(Vector4(0.0f, -115.0f, CurPos.z -1.0f, 1.0f));
			m_RectCol->Local_SubScale(Vector4(150.0f, 50.0f, 1.0f, 1.0f));
		}
		else if (2 == m_BlizzardKind)
		{
			m_RectCol = GetGameObject()->Component_Add<RectCol2D>(L"Skill_BlizzardCol");
			m_RectCol->Local_SubPosition(Vector4(0.0f, -40.0f, CurPos.z - 1.0f, 1.0f));
			m_RectCol->Local_SubScale(Vector4(150.0f, 50.0f, 1.0f, 1.0f));
		}

	}
}

void BlizzardHitEffect::SetBlizzardKind(int _Value)
{
	m_BlizzardKind = _Value;
}