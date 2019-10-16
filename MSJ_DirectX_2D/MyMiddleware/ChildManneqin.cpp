#include "ChildManneqin.h"

#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>
#include <RectCol2D.h>



ChildManneqin::ChildManneqin()
	: m_bHit(false), m_Dir(L"Left"), m_bAttack(false), m_State(FSM_MState::MOVE)
	, m_MoveChangeTime(0.0f), m_CurStateTime(0.0f), m_StandChangeTime(0.0f), m_AttackModeTime(3.0f), m_HitTime(0.4f)
	, m_HP(8000), m_LDistance(LIMDISTNACE), m_CurAttackTime(0.0f), m_MoveSpeed(CMMOVESPEED)
	, m_DirChangeTime(0.0f), m_bAttDirChange(false), OriginScale(Vector4::One), KnockBack(CMMOVESPEED)
	, m_HPBarScript(nullptr)
{
}


ChildManneqin::~ChildManneqin()
{
}

//

bool ChildManneqin::Init() 
{
	m_SpRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SpRenderer)
	{
		EAssert(true);
		return false;
	}

	m_Animation = GetComponent<Animation2D>();
	
	if(nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"Stand", 0, 3, true, 0.1f);
	m_Animation->Animation_Create(L"Move", 4, 7, true, 0.1f);
	m_Animation->Animation_Create(L"Hit", 8, 8, true, 0.1f);
	m_Animation->Animation_Create(L"Die", 9, 19, false, 0.1f);
	m_Animation->Animation_Change(L"Stand");

	int RandValue = (rand() % 4) + 2;
	m_MoveChangeTime = (float)RandValue;

	RandValue = (rand() % 2) + 2;
	m_StandChangeTime = (float)RandValue;

	// 초기 크기
	OriginScale = GetTransform()->Local_Scale();

	return true;
}

void ChildManneqin::Update()
{
	AniDirCheck();
	Gravity();

	//if (true == Input::Down(L"BaseAttack"))
	//{
	//	m_CurStateTime = 0.0f;
	//	m_State = FSM_MState::HIT;
	//}


	switch (m_State)
	{
	case FSM_MState::STAND:
		Stand();
		ChangeAnimation(L"Stand");
		break;
	case FSM_MState::MOVE:
		Move();
		ChangeAnimation(L"Move");
		break;
	case FSM_MState::HIT:
		Hit();
		ChangeAnimation(L"Hit");
		break;
	case FSM_MState::DIE:
		//Die();
		ChangeAnimation(L"Die");
		break;
	default:
		break;
	}
}

void ChildManneqin::FinalUpdate()
{
}

//
void ChildManneqin::SetPlayer(GameObject* _Player)
{
	m_pPlayer = _Player;
}

void ChildManneqin::SetFloor(GameObject* _Floor)
{
	m_pFloor = _Floor;
}

void ChildManneqin::SetHit(bool _Value)
{
	m_bHit = _Value;
	m_State = FSM_MState::HIT;
}

void ChildManneqin::SetAttack(bool _Value)
{
	m_bAttack = true;
	// 애니메이션 가만히있는걸로 변경
}

void ChildManneqin::SetBackground(Vector4 _BG)
{
	BackgroundPos = _BG;
}

void ChildManneqin::SetHPBar(GeneralMonster_HP* _Script)
{
	m_HPBarScript = _Script;
}

int ChildManneqin::GetHP()
{
	return m_HP;
}

//

void ChildManneqin::DirChange()
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

void ChildManneqin::HitChange()
{
	if (false == m_bHit)
	{
		m_bHit = true;
	}
	else
	{
		m_bHit = false;
	}
}

void ChildManneqin::DirMove()
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

	// 이동 제한
	if ((BackgroundPos.x/2) < GetTransform()->Local_Position().x + 30.0f
		|| (-(BackgroundPos.x / 2)) > GetTransform()->Local_Position().x - 30.0f)
	{
		DirChange();
	}
}

// 더 갈 수 있는지 여부 판단
bool ChildManneqin::EmptyTileCheck()
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

	// 더 갈 수있는지 체크
	TimeColor = m_pFloor->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(BasePos.x), (unsigned int)(BasePos.y - (GetTransform()->Local_Scale().y / 2) -1.0f));

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
			if (true == m_bHit)
			{
				m_bAttDirChange = true;
			}
			else
			{
				m_bAttDirChange = false;
			}

			return true;		// 갈 수 없음
		}
	}

	return false;
}


void ChildManneqin::Gravity()
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
		//m_bDownCheck = true;
		Vector4 yMove = Vector4{ 0.0f, -1.0f, 0.0f };
		GetTransform()->Local_Move(yMove);
	}

	if (TestVec.a != 0)
	{
		//m_bDownCheck = false;

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

void ChildManneqin::DirTileChange()
{
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
	}
}

void ChildManneqin::AniDirCheck()
{
	// 애니메이션 방향체크
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

// 애니메이션 바꾸기
void ChildManneqin::ChangeAnimation(std::wstring _AniName)
{
	if (_AniName != m_Animation->CurAnimationName())
	{
		m_Animation->Animation_Change(_AniName.c_str());
	}
}


void ChildManneqin::Damage(int _Value)
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

