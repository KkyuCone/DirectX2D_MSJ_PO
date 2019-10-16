#include "ChildManneqin.h"

void ChildManneqin::Stand()
{
	// �ش� �������� �̵�
	// ĳ���Ͷ� �浹�ϸ� ���ڸ��� ���߰� 1.3�� �Ŀ� �ٽ� ���� �����ؼ� �̵��ؾ��Ѵ�.
	// ���� ������ �ð�üũ m_bAttack == true
	if (true == m_bAttack)
	{
		if (1.5f < m_CurAttackTime)
		{
			m_CurAttackTime = 0.0f;
			m_bAttack = false;
		}
		m_CurAttackTime += Time::DeltaTime();
		return;
	}

	if (m_bHit == true)
	{
		m_State = FSM_MState::MOVE;
		return;
	}

	if (m_CurStateTime >= 3.5f)
	{
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

		m_bAttack = false;
		m_CurStateTime = 0.0f;
		m_State = FSM_MState::MOVE;
		return;
	}
	
	m_CurStateTime += Time::DeltaTime();
}

void ChildManneqin::Move()
{
	// �¾��� ��� �����̵�
	if (true == m_bHit)
	{
		AttackMove();
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

void ChildManneqin::Hit()
{
	if (m_CurStateTime >= m_HitTime)
	{
		m_bHit = true;
		KnockBack = CMMOVESPEED;
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

void ChildManneqin::Die()
{
	if (L"Die" == m_Animation->CurAnimationName() && true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Update_Off();	// ��� ���д�.
	}
}

void ChildManneqin::AttackMove()
{	
	// �÷��̾ ���ٸ� �߰� X
	if ( nullptr == m_pPlayer)
	{
		m_bHit = false;
		m_State = FSM_MState::STAND;
		return;
	}

	// �̵��� ���� ����
	if (false == m_bAttDirChange)
	{
		// �÷��̾�� �Ÿ����
		Vector4 NewVector = m_pPlayer->GetTransform()->Local_Position() - GetTransform()->Local_Position();
		NewVector.z = 0.0f;
		float dis = NewVector.Norm();

		if (dis >= m_LDistance)
		{
			m_State = FSM_MState::STAND;
			m_bHit = false;
			return;
		}

		if (dis >= -0.1f && dis <= 20.0f)
		{
			m_bAttack = true;
			m_State = FSM_MState::STAND;
			return;
		}

		std::wstring CurDir = m_Dir;

		float CheckDirValue = GetTransform()->Local_Position().x - m_pPlayer->GetTransform()->Local_Position().x;

		if (0 >= CheckDirValue)
		{
			m_Dir = L"Right";
		}
		else
		{
			m_Dir = L"Left";
		}

		if (CurDir != m_Dir)
		{
			m_bDirChange = true;
		}
	}
	else
	{
		// ������ �ٲ���ٴ� ���� ���̻� �� ���� ���ٴ� �ǹ��̹Ƿ� ���� 1.0�ʰ� ������ �ٲ��� �ʴ´�.
		if (m_DirChangeTime >= 1.0f)
		{
			m_DirChangeTime = 0.0f;
			m_bAttDirChange = false;
		}

		m_DirChangeTime += Time::DeltaTime();
	}

	// �ش� �������� �̵� (�̹����� ���⿡���� ����..)
	DirMove();
}

