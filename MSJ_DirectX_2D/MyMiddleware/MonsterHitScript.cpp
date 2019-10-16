#include "MonsterHitScript.h"



MonsterHitScript::MonsterHitScript() : m_MovePos(Vector4::Zero), m_bAniCreate(false)
{
}


MonsterHitScript::~MonsterHitScript()
{
}

//

bool MonsterHitScript::Init()
{
	return true;
}

void MonsterHitScript::Update()
{
	if (false == m_bAniCreate)
		return;

	if (true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}

void MonsterHitScript::FinalUpdate()
{

}

void MonsterHitScript::SetSkillName(std::wstring _Name)
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return;
	}

	Vector4 CurPos = GetTransform()->Local_Position();
	CurPos += m_MovePos;

	CurPos.z = -8.0f;

	GetGameObject()->GetTransform()->Local_Position(CurPos);

	if (L"HE_BE" == _Name)
	{
		m_Renderer->SetSprite(L"BlazingExtinction_Hit.png", 0);
		m_Animation->Animation_Create(L"HE_BE", 0, 6, false, 0.1f);
		m_Animation->Animation_Change(L"HE_BE");

		m_bAniCreate = true;
	}
	else if (L"HE_BZ" == _Name)
	{
		// Blizzard_Hit
		m_Renderer->SetSprite(L"Blizzard_Hit.png", 0);
		m_Animation->Animation_Create(L"HE_BZ", 0, 5, false, 0.1f);
		m_Animation->Animation_Change(L"HE_BZ");

		m_bAniCreate = true;
	}
	else if (L"PM" == _Name)
	{
		GetGameObject()->Death();
		return;
	}
	else if (L"HE_CC" == _Name)
	{
		m_Renderer->SetSprite(L"Cataclysm_Hit.png", 0);
		m_Animation->Animation_Create(L"HE_CC", 0, 5, false, 0.1f);
		m_Animation->Animation_Change(L"HE_CC");

		m_bAniCreate = true;
	}
	else if (L"HE_SP" == _Name)
	{
		m_Renderer->SetSprite(L"SoulPenetration_Hit.png", 0);
		m_Animation->Animation_Create(L"HE_SP", 0, 5, false, 0.1f);
		m_Animation->Animation_Change(L"HE_SP");

		m_bAniCreate = true;
	}
	else if (L"BOSS_01" == _Name)
	{
		m_Renderer->SetSprite(L"Attack1_Hit.png", 0);
		m_Animation->Animation_Create(L"BOSS_01", 0, 6, false, 0.1f);
		m_Animation->Animation_Change(L"BOSS_01");

		m_bAniCreate = true;
	}
}

void MonsterHitScript::SetMovePos(Vector4 _Pos)
{
	m_MovePos = _Pos;
}
