#include "WhiteSandRabbit.h"

#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>
#include <RectCol2D.h>


WhiteSandRabbit::WhiteSandRabbit() 
	: m_State(STAND), m_pPlayer(nullptr), m_pFloor(nullptr)
	, m_HP(WSR_HP), KnockBack(WSRMOVESPEED), m_MoveSpeed(WSRMOVESPEED), m_CurStateTime(0.0f)
	, m_HitTime(0.3f), OriginScale(Vector4::One), BackgroundPos(Vector4::One), m_StandStateChangeTime(0.0f)
	, m_Dir(L"Left"), m_MoveChangeTime(0.0f), m_bTraceMode(false), m_bTraceDirChange(false), m_LDistance(LIMDISTNACE)
	, m_bDirChange(false), m_bUpTile(true)
	, m_HPBarScript(nullptr)
{
}


WhiteSandRabbit::~WhiteSandRabbit()
{
}

//

bool WhiteSandRabbit::Init()
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
	m_Animation->Animation_Create(L"Attack", 13, 22, true, 0.1f);
	m_Animation->Animation_Create(L"Die",23, 39, true, 0.1f);

	m_Animation->Animation_Change(L"Stand");

	// �⺻ ũ�� ����
	OriginScale = GetTransform()->Local_Scale();

	// �������� ��ȭ�� �ð��� ���ϱ�
	m_StandStateChangeTime = (float)(rand() % 4) + 2.0f;
	m_MoveChangeTime = (float)(rand() % 5) + 3.0f;

	return true;
}

void WhiteSandRabbit::Update()
{
	AniDirCheck();
	Gravity();

	if (true == Input::Down(L"BaseAttack"))
	{
		m_CurStateTime = 0.0f;
		m_bHit = true;
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

void WhiteSandRabbit::FinalUpdate()
{

}

//

void WhiteSandRabbit::AniDirCheck()
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

void WhiteSandRabbit::DirMove()
{
	if (true == EmptyTileCheck())
	{
		DirChange();
		return;
	}

	if (L"Left" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Left * m_MoveSpeed * Time::DeltaTime());
	}
	else if (L"Right" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Right * m_MoveSpeed * Time::DeltaTime());
	}

	// �̵� ����
	if ((BackgroundPos.x / 2) < GetTransform()->Local_Position().x + 30.0f
		|| (-(BackgroundPos.x / 2)) > GetTransform()->Local_Position().x - 30.0f)
	{
		DirChange();
	}
}

void WhiteSandRabbit::Gravity()
{
	Vector4 TestVec = Vector4{ -1, -1, -1, -1 };
	Vector4 TestVec2 = Vector4{ -1, -1, -1, -1 };
	Vector4 Pos = GetTransform()->World_Position();

	if (nullptr == m_pFloor)
		return;

	TestVec = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2)));
	TestVec2 = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 1));

	if (TestVec2.a == 0 && (m_State == MOVE || m_State == STAND))
	{
		Vector4 yMove = Vector4{ 0.0f, -1.0f, 0.0f };
		GetTransform()->Local_Move(yMove);
	}

	if (TestVec.a != 0)
	{
		Vector4 Pos = GetTransform()->World_Position();
		TestVec = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 1));
		while (TestVec.a != 0)
		{
			Pos.y += 1.0f;
			TestVec = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 1));
		}
		GetTransform()->World_Position(Pos);
	}
}

bool WhiteSandRabbit::EmptyTileCheck()
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
	TimeColor = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(BasePos.x), (unsigned int)(BasePos.y - (GetTransform()->Local_Scale().y / 2) - 1.0f));

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

		TimeColor2 = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(BasePos.x), (unsigned int)(BasePos.y - (GetTransform()->Local_Scale().y / 2) - 2.0f));

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

void WhiteSandRabbit::DirChange()
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

// �ִϸ��̼� ü����
void WhiteSandRabbit::ChangeAnimation(std::wstring _AniName)
{
	if (_AniName == m_Animation->CurAnimationName())
		return;

	m_Animation->Animation_Change(_AniName.c_str());
}

// ���� �̵�
void WhiteSandRabbit::TraceMode()
{
	// �÷��̾� �̼����� �߰� X
	if (nullptr == m_pPlayer)
	{
		m_bTraceMode = false;
		m_State = FSM_MState::STAND;
		return;
	}

	// �߰�
	// �÷��̾�� �Ÿ����
	Vector4 NewVector = m_pPlayer->GetTransform()->Local_Position() - GetTransform()->Local_Position();
	NewVector.z = 0.0f;
	float dis = NewVector.Norm();


	if (false == m_bDirChange)
	{
		// ���� ������ ����� ���
		if (dis >= m_LDistance)
		{
			m_State = FSM_MState::STAND;
			m_bTraceMode = false;
			m_bTraceDirChange = false;
			m_bDirChange = false;
			return;
		}

		// ���� �����Ÿ� �ȿ� �÷��̾ ���� ��� ����
		if (dis >= -0.1f && dis <= 70.0f)
		{
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


		if (false == m_bTraceDirChange)
		{

			if (CurDir != m_Dir)
			{
				m_bTraceDirChange = true;
			}
			else
			{
				m_bTraceDirChange = false;
			}
		}
		else
		{
			// �߰� �߿� ���� ��ȯ
			if (true == EmptyTileCheck())
			{
				if (L"Left" == m_Dir)
				{
					m_Dir = L"Right";
				}
				else
				{
					m_Dir = L"Left";
				}

				m_CurStateTime = 0.0f;
				m_bDirChange = true;
			}
		}

	}
	else
	{
		// ���䳢�� �ٺ��Ǳ� '��'
		// ���� �ִ� �ֵ��� ���� ����������

		if (m_CurStateTime >= 1.0f)
		{
			m_bTraceDirChange = false;
			m_bDirChange = false;
			m_CurStateTime = 0.0f;
		}

		m_CurStateTime += Time::DeltaTime();
	}

	// �Ϲ� �̵�
	DirMove();

}

void WhiteSandRabbit::Damage(int _Value)
{
	m_HP -= _Value;

	if (0 >= m_HP)
	{
		m_HP = 0;
		GetGameObject()->Component_Add<RectCol2D>()->Update_Off();
		m_State = MonsterState::DIE;
		m_HPBarScript->Damage(_Value);
		return;
	}

	m_HPBarScript->Damage(_Value);
	m_State = MonsterState::HIT;
}