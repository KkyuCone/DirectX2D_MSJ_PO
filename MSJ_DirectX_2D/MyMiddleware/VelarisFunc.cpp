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
	// �÷��̾� �̼����� �߰� X
	if (nullptr == m_pPlayer)
	{
		m_bTraceMode = false;
		m_State = FSM_MState::STAND;
		return;
	}

	// �߰� ���
	// �÷��̾�� �Ÿ����
	Vector4 NewVector = m_pPlayer->GetTransform()->Local_Position() - GetTransform()->Local_Position();
	NewVector.z = 0.0f;
	float dis = NewVector.Norm();

	// ���� ������ ����� ���
	if (dis >= m_LDistance)
	{
		m_State = FSM_MState::STAND;
		m_bTraceMode = false;
		m_bTraceDirChange = false;
		m_bDirChange = false;
		m_SkillTime = 0.0f;
		return;
	}

	// m_bDirChange ������ �ٲ��� ����
	if (false == m_bDirChange)
	{
		// ���� �����Ÿ� �ȿ� �÷��̾ ���� ��� ����
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
			// �÷��̾ �����ʿ� ���� ��� (���ͱ���)
			m_Dir = L"Right";
		}
		else
		{
			// �÷��̾ ���ʿ� ���� ��� (���ͱ���)
			m_Dir = L"Left";
		}


		// ������ �ٲ��� ���ο� ���� �� ���� (�ٺ� ���� �κ�)
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
	// �Ϲ� �̵�
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

	// �� �� ���ִ��� üũ -> ���� �ʿ�
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
			// Ÿ�ϸ����� �ƴ��� �Ǵ�����
			if (true == m_bUpTile)
			{
				return true;		// �� �� ����
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