#include "PlayerAnimation.h"
#include <SpriteRenderer2D.h>

unsigned int PlayerAnimation::AttAniCount = 0;

PlayerAnimation::PlayerAnimation() : m_Index(0), AttAniEnd(false), AttAniEndCount(0)
{
}


PlayerAnimation::~PlayerAnimation()
{
}

//

bool PlayerAnimation::Init()
{
	m_SpriteRender = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SpriteRender)
	{
		EAssert(true);
	}

	m_Animation = GetComponent<Animation2D>();
	m_Animation->Animation_Create(L"Idle", 0, 2);
	m_Animation->Animation_Create(L"Run", 3, 6, true);
	m_Animation->Animation_Create(L"Jump", 29, 29, false);
	//m_Animation->Animation_Create(L"Down", 20, 20, false);
	m_Animation->Animation_Create(L"Down", 20, 20, false);
	m_Animation->Animation_Create(L"Down_Attack", 20, 21, false);
	m_Animation->Animation_Create(L"Rope_Idle", 8, 8, false);
	m_Animation->Animation_Create(L"Rope_Move", 8, 9, true);
	m_Animation->Animation_Create(L"Ladder_Idle", 10, 10, false);
	m_Animation->Animation_Create(L"Ladder_Move", 10, 11, true);
	m_Animation->Animation_Create(L"Attack_01", 13, 14, true, 0.4f);
	m_Animation->Animation_Create(L"Attack_02", 15, 17, true, 0.2f);
	m_Animation->Animation_Create(L"Attack_03", 18, 19, true, 0.4f);
	m_Animation->Animation_Create(L"Death", 28, 28, false);

	m_Animation->Animation_Change(L"Idle");
	return true;
}

void PlayerAnimation::Update()
{
	if (nullptr == m_PlayerMove)
	{
		EAssert(true);
		return;
	}

	switch (m_PlayerMove->m_PlayerState)
	{
	case IDLE:
	case RUN:
	case JUMP_UP:
	case JUMP_DOWN:
	case DOWN:
	case DOWN_ATTACK:
	case ROPE_IDLE:
	case ROPE_UP:
	case ROPE_DOWN:
	case LADDER_IDLE:
	case LADDER_UP:
	case LADDER_DOWN:
	case ATTACK_BASE:
	case ATTACK_SKILL_01:
	case ATTACK_SKILL_02:
	case ATTACK_SKILL_03:
	case DEATH:
		Animation_Change(m_PlayerMove->m_PlayerState, m_PlayerMove->Dir);
	default:
		break;
	}
}

void PlayerAnimation::Animation_Change(FSM_STATE _pState, std::wstring _Dir)
{
	// 애니메이션 바꾸기
	switch (_pState)
	{
	case IDLE:
		m_Animation->Animation_Change(L"Idle");
		break;
	case RUN:
		m_Animation->Animation_Change(L"Run");
		break;
	case JUMP_UP:
	case JUMP_DOWN:
		m_Animation->Animation_Change(L"Jump");
		break;
	case DOWN:
		m_Animation->Animation_Change(L"Down");
		break;
	case DOWN_ATTACK:
		m_Animation->Animation_Change(L"Down_Attack");
		break;
	case ROPE_IDLE:
		m_Animation->Animation_Change(L"Rope_Idle");
		break;
	case ROPE_UP:
	case ROPE_DOWN:
		m_Animation->Animation_Change(L"Rope_Move");
		break;
	case LADDER_IDLE:
		m_Animation->Animation_Change(L"Ladder_Idle");
		break;
	case LADDER_UP:
	case LADDER_DOWN:
		m_Animation->Animation_Change(L"Ladder_Move");
		break;
	case ATTACK_BASE:
		break;
	case ATTACK_SKILL_01:
		SpecialAnimationEnd();

		if (false == m_Animation->IsCurAnimation(L"Attack_01"))
		{
			AttAniEndCount = 0;
			m_Animation->Animation_Change(L"Attack_01");
		}
		break;
	case ATTACK_SKILL_02:
		SpecialAnimationEnd();

		if (false == m_Animation->IsCurAnimation(L"Attack_02"))
		{
			AttAniEndCount = 0;
			m_Animation->Animation_Change(L"Attack_02");
		}
		break;
	case ATTACK_SKILL_03:
		SpecialAnimationEnd();

		if (false == m_Animation->IsCurAnimation(L"Attack_03"))
		{
			AttAniEndCount = 0;
			m_Animation->Animation_Change(L"Attack_03");
		}
		break;
	case DEATH:
		m_Animation->Animation_Change(L"Death");
		break;
	case DAMAGE:
		// 피격시
		break;
	default:
		break;
	}


	// 방향 + 이미지크기 ㅇㅅㅇ
	if (L"Right" == _Dir)
	{
		// 월드행렬 -  테스트용
		//Vector4 Test = GetTransform()->Local_Scale() + (GetTransform()->Local_Scale() * 1.0f * Time::DeltaTime());
		//GetTransform()->Local_Scale(GetTransform()->Local_Scale() + (GetTransform()->Local_Scale() * 1.0f * Time::DeltaTime()));

		// 오른쪽일때는 뒤집어줌
		Vector4 Size = GetTransform()->Local_Scale();
		Size.x = -1.0f;
		Size.y = 1.0f;
		GetTransform()->Local_Scale(Size);
	}
	else
	{
		Vector4 Size = GetTransform()->Local_Scale();
		Size.x = 1.0f;
		Size.y = 1.0f;
		GetTransform()->Local_Scale(Size);
	}
}


void PlayerAnimation::SpecialAnimationEnd()
{
	int a = AttAniCount;
	if (CharacterChildren <= AttAniCount)
	{
		AttAniCount = 0;
		m_PlayerMove->m_PlayerState = IDLE;
	}

	if (true == m_Animation->IsEndAnimation())
	{
		if (0 == AttAniEndCount)
		{
			++AttAniEndCount;
			++AttAniCount;
		}
	}
}
