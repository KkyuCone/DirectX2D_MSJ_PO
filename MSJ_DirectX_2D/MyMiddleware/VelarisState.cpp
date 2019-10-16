#include "Velaris.h"

void Velaris::Stand()
{
	// �ǰݽ� ���ø��� ����
	if (m_bHit == true)
	{
		m_State = FSM_MState::HIT;
		m_bHit = false;
		return;
	}
	
	// ��������϶��� �ٷ� MOVE���·� ����
	if (true == m_bTraceMode)
	{
		m_State = FSM_MState::MOVE;
	}

	// Stand���¿��� �̵����� ����
	if (m_CurStateTime >= m_StandStateChangeTime)
	{
		// ���� ���� ( 50% Ȯ���� ���� ������ �����Ұ��� �ƴϸ� �ٲܰ��� ���� )
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
	// �ǰݽ� ���ø��� ����
	if (m_bHit == true)
	{
		m_State = FSM_MState::HIT;
		m_bHit = false;
		return;
	}

	// �������
	if (true == m_bTraceMode)
	{
		TraceMode();
		return;
	}

	// �Ϲ� �̵�
	DirMove();

	// �Ϲ��̵��� üũ (STAND ���·� ����)
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

	// ���ӵ��� �˹� ������ֱ�
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
		GetGameObject()->Update_Off();	// ��� ���д�.
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