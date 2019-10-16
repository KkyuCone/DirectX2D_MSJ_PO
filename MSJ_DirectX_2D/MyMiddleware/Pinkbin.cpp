#include "Pinkbin.h"

#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>

#include <SoundPlayer.h>

Pinkbin::Pinkbin()
	: m_AttackYPosValue(0.0f), m_SkillYPosValue(0.0f)
	, m_HP(PINKBIN_HP), KnockBack(PINKBIN_MOVESPEED), m_MoveSpeed(PINKBIN_MOVESPEED), m_LDistance(PINKBIN_LDISTANCE)
	, m_CurStateTime(0.0f), m_StandStateChangeTime(0.0f), m_MoveChangeTime(0.0f), m_HitTime(0.3f), m_SkillTime(0.0f)
	, m_bUpTile(false), m_bHit(false), m_bAttackMode(false), m_bBaseMode(false), m_bSkillMode(false), m_bMove(false), m_bAttack(false)
	, BackgroundSize(Vector4::One), OriginScale(Vector4::One), CurStandPos(Vector4::One)
	, m_Dir(L"Left")
	, m_State(MonsterState::STAND), m_PreState(MonsterState::STAND)
	, m_pPlayer(nullptr), m_pFloor(nullptr), m_SpRenderer(nullptr), m_Animation(nullptr), m_HPScript(nullptr)
{
}


Pinkbin::~Pinkbin()
{
}


bool Pinkbin::Init()
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

	PinkbinSoundLoading();

	Vector4 CurPos = GetTransform()->Local_Position();

	// 기본
	m_Animation->Animation_Create(L"Stand", 0, 5, true, 0.1f);
	m_Animation->Animation_Create(L"Move", 6, 13, true, 0.1f);
	m_Animation->Animation_Create(L"Hit", 14, 14, true, 0.1f);
	m_Animation->Animation_Create(L"Skill6", 15, 47, true, 0.1f);
	m_Animation->Animation_Create(L"Skill7", 48, 89, true, 0.1f);
	m_Animation->Animation_Create(L"Skill8", 90, 119, true, 0.1f);
	m_Animation->Animation_Create(L"Die", 120, 181, true, 0.1f);


	// 공격
	ChangeSpriteToAttack();
	m_Animation->Animation_Create(L"Attack1", 0, 26, true, 0.1f);
	m_Animation->Animation_Create(L"Attack2", 27, 50, true, 0.1f);
	m_Animation->Animation_Create(L"Attack3", 51, 70, true, 0.1f);
	m_Animation->Animation_Create(L"Attack4", 71, 91, true, 0.1f);

	// 스킬
	ChangeSpriteToSkill();
	m_Animation->Animation_Create(L"Skill1", 0, 15, true, 0.1f);
	m_Animation->Animation_Create(L"Skill2", 16, 35, true, 0.1f);
	m_Animation->Animation_Create(L"Skill3", 36, 59, true, 0.1f);
	m_Animation->Animation_Create(L"Skill4", 60, 64, true, 0.1f);
	m_Animation->Animation_Create(L"Skill4After", 65, 78, true, 0.1f);
	m_Animation->Animation_Create(L"Skill5", 79, 107, true, 0.1f);


	// 기본 Base의 stand로 변경
	m_bBaseMode = true;
	m_bAttackMode = false;
	m_bSkillMode = false;

	m_SpRenderer->SetSprite(L"Pinkbin_Base.png", 0);
	GetGameObject()->GetTransform()->Local_Position(CurPos);
	GetGameObject()->GetTransform()->Local_Scale(Vector4(199.0f, 139.0f, 1.0f, 1.0f));
	m_Animation->Animation_Change(L"Stand");

	// 시간 설정
	m_StandStateChangeTime = (float)((rand() % 2) + 1);

	m_StandStateChangeTime = 1.0f;

	OriginScale = GetGameObject()->GetTransform()->Local_Scale();
	CurStandPos = GetGameObject()->GetTransform()->Local_Position();

	return true;
}

void Pinkbin::Update()
{
	Gravity();
	AniDirCheck();

	//// 핑크빈은 중력을 받는 상태가 따로 있다. 
	//if (m_State == MonsterState::STAND
	//	|| m_State == MonsterState::MOVE)
	//{
	//	Gravity();
	//}

	// 상태에 따른 상태함수와 연결
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
		Attack1();
		ChangeAnimation(L"Attack1");
		break;
	case MonsterState::ATTACK2:
		Attack2();
		ChangeAnimation(L"Attack2");
		break;
	case MonsterState::ATTACK3:
		Attack3();
		ChangeAnimation(L"Attack3");
		break;
	case MonsterState::ATTACK4:
		Attack4();
		ChangeAnimation(L"Attack4");
		break;
	case MonsterState::SKILL_1:
		Skill1();
		ChangeAnimation(L"Skill1");
		break;
	case MonsterState::SKILL_2:
		Skill2();
		ChangeAnimation(L"Skill2");
		break;
	case MonsterState::SKILL_3:
		Skill3();
		ChangeAnimation(L"Skill3");
		break;
	case MonsterState::SKILL_4:
		SKill4();
		ChangeAnimation(L"Skill4");
		break;
	case MonsterState::SKILL_4_AFTER:
		Skill4_After();
		ChangeAnimation(L"Skill4After");
		break;
	case MonsterState::SKILL_5:
		m_State = MonsterState::STAND;
		break;

	// Base에 있는 스킬모션들
	case MonsterState::SKILL_6:
		Skill6();
		ChangeAnimation(L"Skill6");
		break;
	case MonsterState::SKILL_7:
		Skill7();
		ChangeAnimation(L"Skill7");
		break;
	case MonsterState::SKILL_8:
		Skill8();
		ChangeAnimation(L"Skill8");
		break;
	default:
		break;
	}
}

void Pinkbin::FinalUpdate()
{

}

// 스프라이트 이미지를 변경.

void Pinkbin::ChangeSpriteToBase()
{

	m_SpRenderer->SetSprite(L"Pinkbin_Base.png", 0);
	GetGameObject()->GetTransform()->Local_Scale(Vector4(199.0f, 139.0f, 1.0f, 1.0f));
	OriginScale = GetGameObject()->GetTransform()->Local_Scale();

	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	Vector4 MovePos = CurPos;

	if (true == m_bAttackMode)
	{

		switch (m_PreState)
		{
		case MonsterState::ATTACK1:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 60.0f;
				MovePos.y -= 80.0f;
			}
			else
			{
				MovePos.x -= 60.0f;
				MovePos.y -= 80.0f;
			}
			break;
		case MonsterState::ATTACK2:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 78.0f;
				MovePos.y -= 35.0f;
			}
			else
			{
				MovePos.x -= 78.0f;
				MovePos.y -= 35.0f;
			}
			break;
		case MonsterState::ATTACK3:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 47.0f;
				MovePos.y -= 50.0f;
			}
			else
			{
				MovePos.x -= 47.0f;
				MovePos.y -= 50.0f;
			}
			break;
		case MonsterState::ATTACK4:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 125.0f;
				MovePos.y -= 83.0f;
			}
			else
			{
				MovePos.x += 125.0f;
				MovePos.y += 83.0f;
			}
			break;
		default:
			break;
		}
	}
	else if (true == m_bSkillMode)
	{
		switch (m_PreState)
		{
		case MonsterState::SKILL_1:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 48.5f;
				MovePos.y += 37.5f;
			}
			else
			{
				MovePos.x -= 48.5f;
				MovePos.y += 37.5f;
			}
			break;
		case MonsterState::SKILL_2:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 30.5f;
				MovePos.y -= 74.5f;
			}
			else
			{
				MovePos.x -= 30.5f;
				MovePos.y -= 74.5f;
			}
			break;
		case MonsterState::SKILL_3:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 21.5f;
				MovePos.y -= 64.5f;
			}
			else
			{
				MovePos.x -= 21.5f;
				MovePos.y -= 64.5f;
			}
			break;
		case MonsterState::SKILL_4:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 2.5f;
				MovePos.y -= 26.5f;
			}
			else
			{
				MovePos.x += 2.5f;
				MovePos.y -= 26.5f;
			}
			break;
		case MonsterState::SKILL_5:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 16.5f;
				MovePos.y -= 28.5f;
			}
			else
			{
				MovePos.x -= 16.5f;
				MovePos.y -= 28.5f;
			}
			break;
		case MonsterState::SKILL_4_AFTER:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 16.5f;
				MovePos.y -= 26.5f;
			}
			else
			{
				MovePos.x -= 16.5f;
				MovePos.y -= 26.5f;
			}
			break;
		default:
			break;
		}
	}

	GetGameObject()->GetTransform()->Local_Position(MovePos);
	m_bBaseMode = true;
	m_bAttackMode = false;
	m_bSkillMode = false;
}

void Pinkbin::SetDie()
{
	m_State = MonsterState::DIE;
}

void Pinkbin::ChangeSpriteToAttack()
{
	m_SpRenderer->SetSprite(L"Pinkbin_Attack.png", 0);
	GetGameObject()->GetTransform()->Local_Scale(Vector4(457.0f, 331.0f, 1.0f, 1.0f));
	OriginScale = GetGameObject()->GetTransform()->Local_Scale();

	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	Vector4 MovePos = CurPos;

	if (true == m_bBaseMode)
	{
		switch (m_State)
		{
		case MonsterState::ATTACK1:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 63.0f;
				MovePos.y += 83.0f;
			}
			else
			{
				MovePos.x += 63.0f;
				MovePos.y += 83.0f;
			}
			break;
		case MonsterState::ATTACK2:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 72.0f;
				MovePos.y += 33.0f;
			}
			else
			{
				MovePos.x += 72.0f;
				MovePos.y += 33.0f;
			}
			break;
		case MonsterState::ATTACK3:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 69.0f;
				MovePos.y += 46.0f;
			}
			else
			{
				MovePos.x += 69.0f;
				MovePos.y += 46.0f;
			}
			break;
		case MonsterState::ATTACK4:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 127.0f;
				MovePos.y += 83.0f;
			}
			else
			{
				MovePos.x -= 127.0f;
				MovePos.y += 83.0f;
			}
			break;
		default:
			break;
		}
	}


	GetGameObject()->GetTransform()->Local_Position(MovePos);

	m_PreState = m_State;

	m_bBaseMode = false;
	m_bAttackMode = true;
	m_bSkillMode = false;
}

void Pinkbin::ChangeSpriteToSkill()
{
	m_SpRenderer->SetSprite(L"Pinkbin_SkillEtc.png", 0);
	GetGameObject()->GetTransform()->Local_Scale(Vector4(400.0f, 312.0f, 1.0f, 1.0f));
	OriginScale = GetGameObject()->GetTransform()->Local_Scale();

	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	Vector4 MovePos = CurPos;

	if (true == m_bBaseMode)
	{
		switch (m_State)
		{
		case MonsterState::SKILL_1:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 48.5f;
				MovePos.y -= 37.5f;
			}
			else
			{
				MovePos.x += 48.5f;
				MovePos.y -= 37.5f;
			}
			break;
		case MonsterState::SKILL_2:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 30.5f;
				MovePos.y += 74.5f;
			}
			else
			{
				MovePos.x += 30.5f;
				MovePos.y += 74.5f;
			}
			break;
		case MonsterState::SKILL_3:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 21.5f;
				MovePos.y += 64.5f;
			}
			else
			{
				MovePos.x += 21.5f;
				MovePos.y += 64.5f;
			}
			break;
		case MonsterState::SKILL_4:
			if (L"Left" == m_Dir)
			{
				MovePos.x += 2.5f;
				MovePos.y += 26.5f;
			}
			else
			{
				MovePos.x -= 2.5f;
				MovePos.y += 26.5f;
			}
			break;
		case MonsterState::SKILL_5:
			if (L"Left" == m_Dir)
			{
				MovePos.x -= 16.5f;
				MovePos.y += 28.5f;
			}
			else
			{
				MovePos.x += 16.5f;
				MovePos.y += 28.5f;
			}
			break;
		case MonsterState::SKILL_4_AFTER:
			break;
		default:
			break;
		}
	}

	GetGameObject()->GetTransform()->Local_Position(MovePos);

	m_PreState = m_State;

	m_bBaseMode = false;
	m_bAttackMode = false;
	m_bSkillMode = true;
}

//

void Pinkbin::ChangeAnimation(std::wstring _AniName)
{
	if (_AniName == m_Animation->CurAnimationName())
		return;

	switch (m_State)
	{
	case MonsterState::STAND:
	case MonsterState::MOVE:
	case MonsterState::HIT:
	case MonsterState::DIE:
	case MonsterState::SKILL_6:
	case MonsterState::SKILL_7:
	case MonsterState::SKILL_8:
		ChangeSpriteToBase();
		break;
	case MonsterState::ATTACK1:
	case MonsterState::ATTACK2:
	case MonsterState::ATTACK3:
	case MonsterState::ATTACK4:
		ChangeSpriteToAttack();
		break;
	case MonsterState::SKILL_1:
	case MonsterState::SKILL_2:
	case MonsterState::SKILL_3:
	case MonsterState::SKILL_4:
	case MonsterState::SKILL_5:
	case MonsterState::SKILL_4_AFTER:
		ChangeSpriteToSkill();
		break;
	default:
		return;
	}
	m_Animation->Animation_Change(_AniName.c_str());
}


//

void Pinkbin::AniDirCheck()
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

	Vector4 aaa = AniDir;
}

void Pinkbin::DirMove()
{
	if (L"Left" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Left * m_MoveSpeed * Time::DeltaTime());

		if ((-(BackgroundSize.x / 2)) > GetTransform()->Local_Position().x - 150.0f)
		{
			m_Dir = L"Right";
		}
	}
	else if (L"Right" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Right * m_MoveSpeed * Time::DeltaTime());

		if ((BackgroundSize.x / 2) < GetTransform()->Local_Position().x + 150.0f)
		{
			m_Dir = L"Left";
		}
	}
}

void Pinkbin::DirChange()
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

void Pinkbin::Gravity()
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

bool Pinkbin::EmptyTileCheck()
{
	Vector4 BasePos = GetTransform()->Local_Position();
	Vector4 TimeColor = Vector4::One;
	Vector4 TimeColor2 = Vector4::One;

	if (L"Left" == m_Dir)
	{
		BasePos.x -= 5.0f;
	}
	else if (L"Right" == m_Dir)
	{
		BasePos.x += 5.0f;
	}

	// 더 갈 수있는지 체크 -> 수정 필요
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
			// 타일몹인지 아닌지 판단하자
			if (true == m_bUpTile)
			{
				return true;		// 갈 수 없음
			}
		}
	}

	return false;
}

void Pinkbin::AttackPosInit()
{

}

void Pinkbin::SkillPosInit()
{

}

void Pinkbin::PinkbinSoundLoading()
{
	// 사운드플레이
	m_PinkbinPlayer = GetGameObject()->Component_Add<SoundPlayer>();
}

void Pinkbin::SoundPlay_Pinkbin(std::wstring _SoundFileName)
{
	m_PinkbinPlayer->Play(_SoundFileName.c_str());
}