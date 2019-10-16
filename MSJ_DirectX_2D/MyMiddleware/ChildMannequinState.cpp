#include "ChildManneqin.h"

void ChildManneqin::Stand()
{
	// 해당 방향으로 이동
	// 캐릭터랑 충돌하면 그자리에 멈추고 1.3초 후에 다시 방향 설정해서 이동해야한다.
	// 어택 성공시 시간체크 m_bAttack == true
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
	// 맞았을 경우 어택이동
	if (true == m_bHit)
	{
		AttackMove();
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

void ChildManneqin::Die()
{
	if (L"Die" == m_Animation->CurAnimationName() && true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Update_Off();	// 잠시 꺼둔다.
	}
}

void ChildManneqin::AttackMove()
{	
	// 플레이어가 없다면 추격 X
	if ( nullptr == m_pPlayer)
	{
		m_bHit = false;
		m_State = FSM_MState::STAND;
		return;
	}

	// 이동할 방향 설정
	if (false == m_bAttDirChange)
	{
		// 플레이어와 거리계산
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
		// 방향이 바뀌었다는 것은 더이상 갈 곳이 없다는 의미이므로 향후 1.0초간 방향을 바꾸지 않는다.
		if (m_DirChangeTime >= 1.0f)
		{
			m_DirChangeTime = 0.0f;
			m_bAttDirChange = false;
		}

		m_DirChangeTime += Time::DeltaTime();
	}

	// 해당 방향으로 이동 (이미지도 방향에따라 변경..)
	DirMove();
}

