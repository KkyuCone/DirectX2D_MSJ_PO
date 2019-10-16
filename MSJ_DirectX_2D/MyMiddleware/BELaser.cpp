#include "BELaser.h"
#include <RectCol2D.h>
#include <Scene.h>

BELaser::BELaser() : m_LifeTime(0.5f), m_CutWidth(0.0f), m_zRot(0.0f)
{
}


BELaser::~BELaser()
{
}

//

bool BELaser::Init()
{
	m_BlazeRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_BlazeRenderer)
	{
		EAssert(true);
		return false;
	}

	m_BlazeAnim = GetComponent<Animation2D>();

	if (nullptr == m_BlazeAnim)
	{
		EAssert(true);
		return false;
	}

	m_BlazeAnim->Animation_Create(L"LaserRepeat", 0, 6, true, 0.1f);
	m_BlazeAnim->Animation_Change(L"LaserRepeat");


	return true;
}

void BELaser::Update()
{
	DeathTimer();
}

void BELaser::FinalUpdate()
{

}

//
void BELaser::DeathTimer()
{
	m_LifeTime -= Time::DeltaTime();

	if (m_LifeTime < 0.0f)
	{
		GetGameObject()->Death();
	}
}

//
void BELaser::SetCutWidth(float _CutWidth)
{
	if (m_CutWidth > 0.0f)
	{
		return;
	}

	m_CutWidth = _CutWidth;
	m_CutRatio = m_CutWidth / 1133.0f;

	m_BlazeRenderer->GetSkillBuffer()->BlazeValue.w = 1.0f;
	m_BlazeRenderer->GetSkillBuffer()->BlazeValue.x = m_CutRatio;


	//My_PTR<GameObject> NewObj = GetScene()->GameObject_Create(L"Skill_BE");
	//NewObj->GetTransform()->Local_POsi
	//m_RectCol = NewObj->Component_Add<RectCol2D>(L"Skill_BlazingExtinction");
	//m_RectCol->Local_SubRotation(Vector4(0.0f, 0.0f, m_zRot, 0.0f));
	//m_RectCol->Local_SubScale(Vector4(100.0f, 76.0f, 1.0f, 1.0f));

}

void BELaser::SetDegree(float _Rot)
{
	m_zRot = _Rot;
}

