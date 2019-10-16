#include "GeneralMonster_HP.h"
#include <GameObject.h>


GeneralMonster_HP::GeneralMonster_HP() : m_CurHPValue(0), m_HPFullValue(0), m_bZeroHP(false)
{
}


GeneralMonster_HP::~GeneralMonster_HP()
{
}

//

bool GeneralMonster_HP::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();
	m_Renderer->ChangeMesh(L"TextureMesh3");

	return true;
}

void GeneralMonster_HP::Update()
{

}

void GeneralMonster_HP::FinalUpdate()
{

}

//

void GeneralMonster_HP::SetHP(int _Value)
{
	m_CurHPValue = 0;
	m_HPFullValue = _Value;
}

void GeneralMonster_HP::SetHPBarEtc(GameObject* _HP01, GameObject* _HP02, GameObject* _HP03)
{
	m_HPBar01 = _HP01;
	m_HPBar02 = _HP02;
	m_HPBar03 = _HP03;
}


void GeneralMonster_HP::Damage(int _Value)
{
	m_CurHPValue += _Value;


	if (m_CurHPValue >= m_HPFullValue)
	{
		m_CurHPValue = m_HPFullValue;
		m_bZeroHP = true;

		// HP바 삭제하기
		GetGameObject()->Update_Off();
		m_HPBar01->Update_Off();
		m_HPBar02->Update_Off();
		m_HPBar03->Update_Off();
		return;
	}

	float CurHPValue = (float)m_CurHPValue;
	float CurHPFullValue = (float)m_HPFullValue;

	float Percent = (float)(CurHPValue / CurHPFullValue);
	float NewSize = 50.0f * Percent;
	
	Vector4 ChangeSize = GetTransform()->Local_Scale();
	ChangeSize.x = (float)NewSize;

	GetTransform()->Local_Scale(ChangeSize);

}