#include "Velaris.h"

void Velaris::Stand()
{
	// 피격시 어택모드로 변경
	if (m_bHit == true)
	{
		m_State = FSM_MState::HIT;
		m_bHit = false;
		return;
	}
	
	// 추적모드일때는 바로 MOVE상태로 변경
	if (true == m_bTraceMode)
	{
		m_State = FSM_MState::MOVE;
	}

	// Stand상태에서 이동으로 변경
	if (m_CurStateTime >= m_StandStateChangeTime)
	{
		// 방향 변경 ( 50% 확률로 현재 방향을 유지할건지 아니면 바꿀건지 설정 )
		int DirRand = rand() % 2;

		if (0 == DirRand)
		{
			DirChange();
		}

		m_CurStateTime = 0.0f;
		m_State = FSM_MState::MOVE;
		return;
	}

	m_CurStateTime += Time::DeltaTime();
}

void Velaris::Move()
{
	// 피격시 어택모드로 변경
	if (m_bHit == true)
	{
		m_State = FSM_MState::HIT;
		m_bHit = false;
		return;
	}

	// 추적모드
	if (true == m_bTraceMode)
	{
		TraceMode();
		return;
	}

	// 일반 이동
	DirMove();

	// 일반이동시 체크 (STAND 상태로 변경)
	if (m_CurStateTime >= m_MoveChangeTime)
	{
		m_CurStateTime = 0.0f;
		m_State = FSM_MState::STAND;
		return;
	}

	m_CurStateTime += Time::DeltaTime();
}

void Velaris::Hit()
{
	if (m_CurStateTime >= m_HitTime)
	{
		m_bTraceMode = true;
		KnockBack = VMOVESPEED;
		m_CurStateTime = 0.0f;
		m_State = FSM_MState::STAND;

		return;
	}

	// 가속도로 넉백 만들어주기
	Vector4 KnockBackMove = Vector4::One;

	if (L"Left" == m_Dir)
	{
		KnockBackMove = Vector4::Right * KnockBack * Time::DeltaTime();
		GetGameObject()->GetTransform()->Local_Move(KnockBackMove);
	}
	else
	{
		KnockBackMove = Vector4::Left * KnockBack * Time::DeltaTime();
		GetGameObject()->GetTransform()->Local_Move(KnockBackMove);
	}

	KnockBack -= 20.0f * Time::DeltaTime();
	m_CurStateTime += Time::DeltaTime();

	if (KnockBack <= 0.0f)
	{
		m_CurStateTime = m_HitTime;
	}
}

void Velaris::Die()
{
	if (L"Die" == m_Animation->CurAnimationName() && true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Update_Off();	// 잠시 꺼둔다.
	}
}

void Velaris::Attack()
{
	if (true == m_Animation->IsEndAnimation())
	{
		Vector4 MovePos = Vector4::Zero;

		if (L"Left" == m_Dir)
		{
			MovePos.x += m_AttackXPosValue;
		}
		else
		{
			MovePos.x -= m_AttackXPosValue;
		}
		MovePos.y += m_AttackYPosValue;
		GetTransform()->Local_Move(MovePos);

		m_State = FSM_MState::MOVE;
		return;
	}
}