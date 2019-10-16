#include "DamageSkinMoveScript.h"



DamageSkinMoveScript::DamageSkinMoveScript() : m_InitPos(Vector4::Zero), m_DeathTime(1.0f), m_CurTime(0.0f), m_AlphaValue(1.0f)
{
}


DamageSkinMoveScript::~DamageSkinMoveScript()
{
}

//

bool DamageSkinMoveScript::Init()
{
	m_InitPos = GetTransform()->World_Position();

	m_Renderer = GetComponent<SpriteRenderer2D>();
	m_Renderer->GetSkillBuffer()->PoisonValue.y = 1.0f;
	m_Renderer->GetSkillBuffer()->PoisonValue.w = 1.0f;

	return true;
}

void DamageSkinMoveScript::Update()
{
	DamageSkinMove();
	DamageSkinDeathTimer();
}

void DamageSkinMoveScript::FinalUpdate()
{

}

//
void DamageSkinMoveScript::DamageSkinDeathTimer()
{
	if (m_DeathTime <= m_CurTime)
	{
		GetGameObject()->Death();
	}

	if (m_AlphaValue > 0.0f)
	{
		m_AlphaValue -= Time::DeltaTime();
		m_Renderer->GetSkillBuffer()->PoisonValue.w = m_AlphaValue;
	}

	m_CurTime += Time::DeltaTime();
}


void DamageSkinMoveScript::DamageSkinMove()
{
	GetGameObject()->GetTransform()->Local_Move(Vector4::Up * 100.0f * Time::DeltaTime());
}