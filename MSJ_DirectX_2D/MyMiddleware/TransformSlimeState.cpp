#include "TransformSlime.h"

void TransformSlime::Stand()
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
			if (L"Left" == m_Dir)
			{
				m_Dir = L"Right";
			}
			else
			{
				m_Dir = L"Left";
			}
		}

		m_CurStateTime = 0.0f;
		m_State = FSM_MState::MOVE;
		return;
	}

	m_CurStateTime += Time::DeltaTime();
}

void TransformSlime::Move()
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

void TransformSlime::Hit()
{
	if (m_CurStateTime >= m_HitTime)
	{
		m_bTraceMode = true;
		KnockBack = TSMOVESPEED;
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

void TransformSlime::Die()
{
	if (L"Die" == m_Animation->CurAnimationName() && true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Update_Off();	// 잠시 꺼둔다.
	}
}

void TransformSlime::Attack()
{
	if (true == m_Animation->IsEndAnimation())
	{
		Vector4 MovePos = Vector4::Zero;
		MovePos.y += m_AttackYPosValue;
		GetTransform()->Local_Move(MovePos);

		m_State = FSM_MState::MOVE;
		return;
	}
}

void TransformSlime::Skill()
{
	// 스킬 사용시 슬라임의 공격력이 증가한다. 추후 추가
	if (true == m_Animation->IsEndAnimation())
	{
		Vector4 MovePos = Vector4::Zero;
		MovePos.y += m_SkillYPosValue;
		GetTransform()->Local_Move(MovePos);

		m_State = FSM_MState::STAND;
		return;
	}
}