#include "CataclysmSceneBase.h"
#include <RectCol2D.h>
#include <GameObject.h>

CataclysmSceneBase::CataclysmSceneBase() : m_bAlpha(true), m_CurTime(0.0f), m_bRectCreate(false)
{
}


CataclysmSceneBase::~CataclysmSceneBase()
{
}

//

bool CataclysmSceneBase::Init()
{
	m_BaseSceneRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_BaseSceneRenderer)
	{
		EAssert(true);
		return false;
	}

	m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.x = 1.0f;
	m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w = 0.0f;

	m_BaseSceneAnim = GetComponent<Animation2D>();

	if (nullptr == m_BaseSceneAnim)
	{
		EAssert(true);
		return false;
	}

	m_BaseSceneAnim->Animation_Create(L"BaseSceneAni", 0, 0, false, 0.1f);
	m_BaseSceneAnim->Animation_Change(L"BaseSceneAni");

	return true;
}

void CataclysmSceneBase::Update()
{
	if (false == GetGameObject()->IsDeath())
	{
		DeathTimer();
	}
}

void CataclysmSceneBase::FinalUpdate()
{

}

//
void CataclysmSceneBase::DeathTimer()
{
	m_CurTime += Time::DeltaTime();

	if (true == m_bAlpha)
	{
		if (m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w < 1.0f)
		{
			m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w += (Time::DeltaTime() * 0.5f);
		}

		if (m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w >= 0.7f)
		{
			m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w = 0.7f;
			m_bAlpha = false;
		}
	}
	else
	{
		if (m_CurTime >= 2.5f && m_CurTime <= 3.9f)
		{
			if (m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w < 0)
			{
				m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w = 0.0f;
			}
			else
			{
				m_BaseSceneRenderer->GetSkillBuffer()->CataclysmValue.w -= (Time::DeltaTime() * 0.2f);
			}

		}

		if (m_bRectCreate == false)
		{

			if (m_CurTime >= 2.7f && m_CurTime <= 3.0f)
			{
				// 콜리전 만들어주기[
				m_RectCol = GetGameObject()->Component_Add<RectCol2D>(L"Skill_Cataclysm");
				m_bRectCreate = true;
			}
		}

	}

	if (m_CurTime >= 3.9f)
	{
		GetGameObject()->Death();
	}
}