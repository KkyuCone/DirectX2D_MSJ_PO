#include "Velaris.h"

#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>

void Velaris::AniDirCheck()
{
	Vector4 AniDir = OriginScale;
	if (L"Right" == m_Dir)
	{
		AniDir.x *= -1.0f;
		AniDir.y *= 1.0f;
		GetTransform()->Local_Scale(AniDir);
	}
	else
	{
		AniDir.x *= 1.0f;
		AniDir.y *= 1.0f;
		GetTransform()->Local_Scale(AniDir);
	}
}

void Velaris::DirMove()
{
	if (m_bTraceMode == false)
	{
		if (true == EmptyTileCheck())
		{
			if (L"Left" == m_Dir)
			{
				m_Dir == L"Right";
			}
			else
			{
				m_Dir = L"Left";
			}
		}
	}


	if (L"Left" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Left * m_MoveSpeed * Time::DeltaTime());

		if ((-(BackgroundSize.x / 2)) > GetTransform()->Local_Position().x - 200.0f)
		{
			m_Dir = L"Right";
		}

	}
	else if (L"Right" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Right * m_MoveSpeed * Time::DeltaTime());

		if ((BackgroundSize.x / 2) < GetTransform()->Local_Position().x + 200.0f)
		{
			m_Dir = L"Left";
		}
	}
}

void Velaris::DirChange()
{
	if (L"Left" == m_Dir)
	{
		m_Dir == L"Right";
	}
	else
	{
		m_Dir = L"Left";
	}
}

void Velaris::Gravity()
{
	Vector4 TestVec = Vector4{ -1, -1, -1, -1 };
	Vector4 TestVec2 = Vector4{ -1, -1, -1, -1 };
	Vector4 Pos = GetTransform()->World_Position();

	if (nullptr == m_pFloor)
		return;

	TestVec = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 8));
	TestVec2 = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 8 + 1));

	if (TestVec2.a == 0 && (m_State == MOVE || m_State == STAND))
	{
		Vector4 yMove = Vector4{ 0.0f, -1.0f, 0.0f };
		GetTransform()->Local_Move(yMove);
	}

	if (TestVec.a != 0)
	{
		Vector4 Pos = GetTransform()->World_Position();
		TestVec = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 1 + 8));
		while (TestVec.a != 0)
		{
			Pos.y += 1.0f;
			TestVec = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 1 + 8));
		}
		GetTransform()->World_Position(Pos);
	}
}

void Velaris::TraceMode()
{
	// 플레이어 미설정시 추격 X
	if (nullptr == m_pPlayer)
	{
		m_bTraceMode = false;
		m_State = FSM_MState::STAND;
		return;
	}

	// 추격 모드
	// 플레이어와 거리계산
	Vector4 NewVector = m_pPlayer->GetTransform()->Local_Position() - GetTransform()->Local_Position();
	NewVector.z = 0.0f;
	float dis = NewVector.Norm();

	// 추적 범위를 벗어났을 경우
	if (dis >= m_LDistance)
	{
		m_State = FSM_MState::STAND;
		m_bTraceMode = false;
		m_bTraceDirChange = false;
		m_bDirChange = false;
		m_SkillTime = 0.0f;
		return;
	}

	// m_bDirChange 방향이 바뀐지 여부
	if (false == m_bDirChange)
	{
		// 공격 사정거리 안에 플레이어가 있을 경우 공격
		if (dis >= -0.1f && dis <= 300.0f)
		{
			AttackPosInit();
			m_State = FSM_MState::ATTACK1;
			return;
		}
		std::wstring CurDir = m_Dir;

		float CheckDirValue = GetTransform()->Local_Position().x - m_pPlayer->GetTransform()->Local_Position().x;

		if (0 >= CheckDirValue)
		{
			// 플레이어가 오른쪽에 있을 경우 (몬스터기준)
			m_Dir = L"Right";
		}
		else
		{
			// 플레이어가 왼쪽에 있을 경우 (몬스터기준)
			m_Dir = L"Left";
		}


		// 방향이 바뀐지 여부에 따라 값 변경 (바보 설정 부분)
		if (true == EmptyTileCheck() && true == m_bUpTile)
		{
			m_bDirChange = true;

			if (L"Left" == m_Dir)
			{
				m_Dir = L"Right";
			}
			else
			{
				m_Dir = L"Left";
			}
		}
	}
	else
	{
		if (m_CurStateTime >= 1.0f)
		{
			m_CurStateTime = 0.0f;
			m_bDirChange = false;
		}

		m_CurStateTime += Time::DeltaTime();
	}
	// 일반 이동
	DirMove();
}

bool Velaris::EmptyTileCheck()
{
	Vector4 BasePos = GetTransform()->Local_Position();
	Vector4 TimeColor = Vector4::One;
	Vector4 TimeColor2 = Vector4::One;

	if (L"Left" == m_Dir)
	{
		BasePos.x -= 1.0f;
	}
	else if (L"Right" == m_Dir)
	{
		BasePos.x += 1.0f;
	}

	// 더 갈 수있는지 체크 -> 수정 필요
	TimeColor = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(BasePos.x), (unsigned int)(BasePos.y - (GetTransform()->Local_Scale().y / 2) - 1.0f + 8.0f));

	if (TimeColor.a < 1.0f)
	{
		if (L"Left" == m_Dir)
		{
			BasePos.x -= 1.0f;
		}
		else if (L"Right" == m_Dir)
		{
			BasePos.x += 1.0f;
		}

		TimeColor2 = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(BasePos.x), (unsigned int)(BasePos.y - (GetTransform()->Local_Scale().y / 2) - 2.0f + 8.0f ));

		if (TimeColor2.a < 1.0f)
		{
			// 타일몹인지 아닌지 판단하자
			if (true == m_bUpTile)
			{
				return true;		// 갈 수 없음
			}
		}
	}

	return false;
}

void Velaris::AttackPosInit()
{
	Vector4 MovePos = Vector4::Zero;

	if (L"Left" == m_Dir)
	{
		MovePos.x -= m_AttackXPosValue;
	}
	else
	{
		MovePos.x += m_AttackXPosValue;
	}
	MovePos.y -= m_AttackYPosValue;

	GetTransform()->Local_Move(MovePos);
}

void Velaris::DiePosInit()
{
	Vector4 M_Pos = GetTransform()->Local_Position();

	Vector4 MovePos = Vector4::Zero;
	MovePos.y -= m_DieYPosValue;

	GetTransform()->Local_Move(MovePos);

	Vector4 CurPos = GetTransform()->Local_Position();
}

void Velaris::ChangeAnimation(std::wstring _AniName)
{
	if (_AniName == m_Animation->CurAnimationName())
		return;

	m_Animation->Animation_Change(_AniName.c_str());
}