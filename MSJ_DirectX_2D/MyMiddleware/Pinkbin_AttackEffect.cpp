#include "Pinkbin_AttackEffect.h"
#include <RectCol2D.h>
Pinkbin_AttackEffect::Pinkbin_AttackEffect() : m_SkillName(L""), m_SkillColFrame(-1), m_bCol(false)
{
}


Pinkbin_AttackEffect::~Pinkbin_AttackEffect()
{
}

//

bool Pinkbin_AttackEffect::Init()
{
	return true;
}

void Pinkbin_AttackEffect::Update()
{
	AniEndCheck();

	if (m_bCol == false)
	{
		if (nullptr == m_Animation)
		{
			return;
		}

		if (m_SkillColFrame == -1)
		{
			return;
		}

		if (m_Animation->CurAnimationFram() == m_SkillColFrame)
		{
			if (L"Attack1" == m_SkillName)
			{
				NewCOl = GetGameObject()->Component_Add<RectCol2D>(L"Pinkbin_Attack01");
			}
			else if (L"Attack2" == m_SkillName)
			{
				NewCOl = GetGameObject()->Component_Add<RectCol2D>(L"Pinkbin_Attack02");
				NewCOl->Local_SubScale(Vector4(600.0f, 600.0f, 1.0f, 1.0f));
			}
			else if (L"Pinkbin_Attack03" == m_SkillName)
			{
				NewCOl = GetGameObject()->Component_Add<RectCol2D>(L"Pinkbin_Attack03");
				NewCOl->Local_SubScale(Vector4(600.0f, 600.0f, 1.0f, 1.0f));
			}
			else if (L"Attack4" == m_SkillName)
			{
				NewCOl = GetGameObject()->Component_Add<RectCol2D>(L"Pinkbin_Attack04");
				NewCOl->Local_SubScale(Vector4(600.0f, 600.0f, 1.0f, 1.0f));
			}

		}
	}
}

void Pinkbin_AttackEffect::FinalUpdate()
{

}

//
void Pinkbin_AttackEffect::AniEndCheck()
{
	if (true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Death();
		return;
	}
}

//

void Pinkbin_AttackEffect::SetAttack(std::wstring _Name)
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

	m_SkillName = _Name;

	if (L"Attack1" == _Name)
	{
		// ÇÎÅ©ºó Á¦³×½Ã½º
		m_Animation->Animation_Create(L"Pinkbin_Attack1", 0, 27,true, 0.1f);
		m_Animation->Animation_Change(L"Pinkbin_Attack1");

		m_SkillColFrame = 10;
	}
	else if (L"Attack2" == _Name)
	{
		// ÇÎÅ©ºó À½Ç¥
		m_Animation->Animation_Create(L"Pinkbin_Attack2", 0, 29, true, 0.1f);
		m_Animation->Animation_Change(L"Pinkbin_Attack2");
		m_SkillColFrame = 10;
	}
	else if (L"Attack4" == _Name)
	{
		// ...?? »Ð»ÐÀÌ 
		m_Animation->Animation_Create(L"Pinkbin_Attack4", 0, 6, true, 0.1f);
		m_Animation->Animation_Change(L"Pinkbin_Attack4");
		m_SkillColFrame = 2;
	}
	else
	{
		GetGameObject()->Death();
		return;
	}
}
