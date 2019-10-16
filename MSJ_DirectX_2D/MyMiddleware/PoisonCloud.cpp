#include "PoisonCloud.h"
#include <RectCol2D.h>


PoisonCloud::PoisonCloud() : m_LifeTime(20.0f), m_PoisonCount(0), m_CurTime(0.0f), m_bTrans(false)
{
}


PoisonCloud::~PoisonCloud()
{
}


//

bool PoisonCloud::Init()
{


	return true;
}

void PoisonCloud::Update()
{
	if (false == m_bTrans)
	{
		PCTransTimer();
	}
	else 
	{
		PCTransparent();
	}
}

void PoisonCloud::FinalUpdate()
{

}

//

void PoisonCloud::SetPosionCount(int _Count)
{
	m_PoisonCount = _Count;
	PCSeeting();

}

void PoisonCloud::PCSeeting()
{
	m_PoisonCloudRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_PoisonCloudRenderer)
	{
		EAssert(true);
		return;
	}

	m_PoisonAnim = GetComponent<Animation2D>();

	if (nullptr == m_PoisonAnim)
	{
		EAssert(true);
		return;
	}

	//m_PoisonCloudRenderer->GetSkillBuffer()->PoisonValue.x = 1.0f;

	m_PoisonAnim->Animation_Create(L"Moving", 0, m_PoisonCount, true, 0.1f);
	m_PoisonAnim->Animation_Change(L"Moving");

	// 충돌체 만들어주기
	m_RectCol = GetGameObject()->Component_Add<RectCol2D>(L"Skill_PoisonMist");
	m_RectCol->Local_SubScale(GetGameObject()->GetTransform()->Local_Scale());
}

void PoisonCloud::PCTransTimer()
{
	if (m_CurTime >= 12.0f)
	{
		m_bTrans = true;
		m_PoisonCloudRenderer->GetSkillBuffer()->PoisonValue.x = 1.0f;
	}

	if ( (0.0 <= m_CurTime && m_CurTime <= 2.0f)
		|| (4.0f <= m_CurTime && m_CurTime <= 6.0f)
		|| (8.0f <= m_CurTime && m_CurTime <= 11.0f))
	{
		m_RectCol->Update_On();
	}
	else
	{
		m_RectCol->Update_Off();
	}

	m_CurTime += Time::DeltaTime();
}

void PoisonCloud::PCTransparent()
{
	if (nullptr == m_PoisonCloudRenderer)
	{
		return;
	}


	if (m_PoisonCloudRenderer->GetSkillBuffer()->PoisonValue.w <= 0.0f)
	{
		m_PoisonCloudRenderer->GetSkillBuffer()->PoisonValue.w = 0.0f;
		return;
		GetGameObject()->Death();
	}

	m_PoisonCloudRenderer->GetSkillBuffer()->PoisonValue.w -= (Time::DeltaTime() * 0.2f);
}

//

void PoisonCloud::RectColOff()
{
	m_RectCol->Update_Off();
}

void PoisonCloud::RectColOn()
{
	m_RectCol->Update_On();
}