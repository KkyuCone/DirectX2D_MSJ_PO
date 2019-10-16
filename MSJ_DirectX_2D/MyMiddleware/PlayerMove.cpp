#include "PlayerMove.h"
#include <SpriteRenderer2D.h>
#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>
#include <Scene.h>
#include "RenderOrder.h"
#include <SoundPlayer.h>

PlayerMove::PlayerMove()
	: m_MoveSpeed(130.0f), Dir(L"None"), m_PlayerState(IDLE), m_JumpForce(JUMPFORCE), AttackPattern(-1), m_SoundPlayer(nullptr)
	, m_bDownCheck(true), m_bMoveCheck(false)
{
}


PlayerMove::~PlayerMove()
{
}

//
bool PlayerMove::Init()
{

	// 이름태그
	m_PlayerNameTag = GetScene()->GameObject_Create(L"Tag_PlayerName");
	m_PlayerNameTag->GetTransform()->Local_Position(Vector4(1.0f, 40.0f, 0.1f, 1.0f));
	m_PlayerNameTag->GetTransform()->Local_Scale(Vector4(77.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PlayerNameTagRenderer = m_PlayerNameTag->Component_Add<SpriteRenderer2D>(RenderOrder::PLAYER_CHILD);
	PlayerNameTagRenderer->SetSprite(L"Tag_MyName.png", 0);

	return true;
}

void PlayerMove::Update()
{
	DirMove();
	GDown();

	switch (m_PlayerState)
	{
	case PlayerMove::IDLE:
		Idle();
		break;
	case PlayerMove::RUN:
		Run();
		break;
	case PlayerMove::JUMP_UP:
		Jump_Up();
		break;
	case PlayerMove::JUMP_DOWN:
		Jump_Down();
		break;
	case PlayerMove::DOWN:
	case PlayerMove::DOWN_ATTACK:
		Down();
		break;
	case PlayerMove::ROPE_IDLE:
	case PlayerMove::ROPE_UP:
	case PlayerMove::ROPE_DOWN:
		Rope();
		break;
	case PlayerMove::LADDER_IDLE:
	case PlayerMove::LADDER_UP:
	case PlayerMove::LADDER_DOWN:
		Ladder();
		break;
	case PlayerMove::ATTACK_BASE:
	case PlayerMove::ATTACK_SKILL_01:
	case PlayerMove::ATTACK_SKILL_02:
	case PlayerMove::ATTACK_SKILL_03:
		break;
	case PlayerMove::DEATH:
		Death();
		break;
	default:
		break;
	}

	// 이름태그 따라다니게하기
	Vector4 CurPlayerPos = GetTransform()->Local_Position();
	CurPlayerPos.z = 9.0f;
	CurPlayerPos.y -= 70.0f;
	m_PlayerNameTag->GetTransform()->Local_Position(CurPlayerPos);
}


//바닥 추가해주기
void PlayerMove::AddFloor(GameObject* _Floor)
{
	m_Floors = _Floor;
}

void PlayerMove::GDown()
{
	Vector4 CheckPixel = Vector4{ -1, -1, -1, -1 };
	Vector4 CheckPixel2 = Vector4{ -1, -1, -1, -1 };
	Vector4 CheckPixel3 = Vector4{ -1, -1, -1, -1 };
	Vector4 CheckPixel4 = Vector4{ -1, -1, -1, -1 };
	Vector4 Pos = GetTransform()->World_Position();

	CheckPixel = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 8));
	CheckPixel2 = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + (8 - 1) ));
	CheckPixel3 = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + (8 - 2)));
	CheckPixel4 = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + (8 - 1)));

	if (CheckPixel3.a == 0 && (m_PlayerState == IDLE))
	{
		m_bDownCheck = true;
	}

	if (CheckPixel4.a == 0 && (m_PlayerState == RUN))
	{
		Vector4 yMove = Vector4{ 0.0f, -1.0f, 0.0f };
		GetTransform()->Local_Move(yMove);
	}

	if (CheckPixel2.a == 0)
	{
		if (m_PlayerState != JUMP_DOWN && m_bDownCheck == true)
		{
			m_JumpForce = 0.0f;
			m_PlayerState = JUMP_DOWN;
		}
	}

	if (CheckPixel.a != 0)
	{
		m_bDownCheck = false;

		Vector4 Pos = GetTransform()->World_Position();
		CheckPixel = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->
			ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 8));
		
		while (CheckPixel.a != 0)
		{
			Pos.y += 1.0f;
			CheckPixel = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->
				ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) + 8));
		}

		GetTransform()->World_Position(Pos);
	}
}

void PlayerMove::FinalUpdate()
{

}

//

bool PlayerMove::GetIsMove()
{
	return m_bMoveCheck;
}

float PlayerMove::GetMoveSpeed()
{
	return m_MoveSpeed;
}

Vector4 PlayerMove::GetWPos()
{
	return GetTransform()->World_Position();
}

FSMState::FSM_STATE PlayerMove::GetState()
{
	return m_PlayerState;
}

PlayerMove* PlayerMove::GetPlayerMoveScript()
{
	return this;
}

std::wstring PlayerMove::GetDir()
{
	return Dir;
}

void PlayerMove::SetSoundPlayer(SoundPlayer* _Player)
{
	m_SoundPlayer = _Player;
}