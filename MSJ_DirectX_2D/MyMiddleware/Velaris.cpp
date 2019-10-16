#include "Velaris.h"

#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>

Velaris::Velaris() 
	: m_AttackYPosValue(8.0f), m_AttackXPosValue(100.0f), m_DieYPosValue(18.0f), m_StandYPosValue(8.0f)
	, m_HP(V_HP), KnockBack(VMOVESPEED), m_MoveSpeed(VMOVESPEED), m_LDistance(LIMDISTNACE)
	, m_CurStateTime(0.0f), m_StandStateChangeTime(0.0f), m_MoveChangeTime(0.0f), m_HitTime(0.4f), m_SkillTime(0.0f)
	, m_bUpTile(false), m_bHit(false), m_bDirChange(false), m_bTraceMode(false), m_bTraceDirChange(false)
	, BackgroundSize(Vector4::One), OriginScale(Vector4::One)
	, m_Dir(L"Left")
	, m_State(MonsterState::STAND)
	, m_pPlayer(nullptr), m_pFloor(nullptr)
	, m_SpRenderer(nullptr), m_Animation(nullptr), m_HPBarScript(nullptr)
{
}


Velaris::~Velaris()
{
}


bool Velaris::Init()
{
	m_SpRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SpRenderer)
	{
		EAssert(true);
		return false;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}
	m_Animation->Animation_Create(L"Stand", 0, 5, true, 0.1f);
	m_Animation->Animation_Create(L"Move", 6, 11, true, 0.1f);
	m_Animation->Animation_Create(L"Hit", 12, 12, true, 0.1f);
	m_Animation->Animation_Create(L"Attack", 13, 30, true, 0.1f);
	m_Animation->Animation_Create(L"Die", 31, 49, true, 0.1f);

	m_Animation->Animation_Change(L"Stand");

	// 기본 크기 지정
	OriginScale = GetTransform()->Local_Scale();

	// 랜덤으로 변화할 시간들 정하기
	m_StandStateChangeTime = (float)(rand() % 4) + 2.0f;
	m_MoveChangeTime = (float)(rand() % 7) + 3.0f;

	return true;
}

void Velaris::Update()
{
	if (true == Input::Down(L"BaseAttack"))
	{
		m_CurStateTime = 0.0f;
		m_bHit = true;
		//m_State = DIE;
	}


	if (MonsterState::DIE != m_State && MonsterState::ATTACK1 != m_State)
	{
		AniDirCheck();
		Gravity();
	}

	switch (m_State)
	{
	case MonsterState::STAND:
		Stand();
		ChangeAnimation(L"Stand");
		break;
	case MonsterState::MOVE:
		Move();
		ChangeAnimation(L"Move");
		break;
	case MonsterState::HIT:
		Hit();
		ChangeAnimation(L"Hit");
		break;
	case MonsterState::DIE:
		Die();
		ChangeAnimation(L"Die");
		break;
	case MonsterState::ATTACK1:
		Attack();
		ChangeAnimation(L"Attack");
		break;
	default:
		break;
	}
}

void Velaris::FinalUpdate()
{

}

