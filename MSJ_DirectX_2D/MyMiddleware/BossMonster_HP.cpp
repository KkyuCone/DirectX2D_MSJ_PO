#include "BossMonster_HP.h"
#include <GameObject.h>


BossMonster_HP::BossMonster_HP() : m_CurHPValue(0), m_HPFullValue(0), m_bZeroHP(false)
{
}


BossMonster_HP::~BossMonster_HP()
{
}

//
bool BossMonster_HP::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();
	m_Renderer->ChangeMesh(L"TextureMesh2");

	return true;
}

void BossMonster_HP::Update()
{

}

void BossMonster_HP::FinalUpdate()
{

}

//

void BossMonster_HP::SetHP(int _Value)
{
	m_CurHPValue = 0;
	m_HPFullValue = _Value;
}

bool BossMonster_HP::GetZeroHP()
{
	return m_bZeroHP;
}

void BossMonster_HP::Damage(int _Value)
{
	m_CurHPValue += _Value;

	if (m_CurHPValue >= m_HPFullValue)
	{
		m_CurHPValue = m_HPFullValue;
		m_bZeroHP = true;
	}

	float CurHPValue = (float)(m_CurHPValue);
	float HPFullValue = (float)(m_HPFullValue);

	float Percent = (m_HPFullValue-CurHPValue) / HPFullValue;

	float XScale = 800.0f;


	float NewSize = XScale * Percent;

	Vector4 ChangeSize = GetTransform()->Local_Scale();
	ChangeSize.x = NewSize;

	GetTransform()->Local_Scale(ChangeSize);

}