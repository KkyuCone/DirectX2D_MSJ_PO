#include "PlayerMove.h"
#include <Time.h>
#include <SpriteRenderer2D.h>
#include <Sprite.h>
#include <Texture.h>
#include <Transform.h>
#include <SoundPlayer.h>

void PlayerMove::Idle()
{
	if (true == Input::Down(L"Left") || true == Input::Down(L"Right"))
	{
		m_bMoveCheck = true;
	}

	if (true == Input::Up(L"Left") || true == Input::Up(L"Right"))
	{
		m_bMoveCheck = false;
	}

	if (true == Input::DownStay(L"Left"))
	{
		Dir = L"Left";
		m_PlayerState = RUN;
	}
	else if (true == Input::DownStay(L"Right"))
	{
		Dir = L"Right";
		m_PlayerState = RUN;
	}
	else if (true == Input::DownStay(L"Up"))
	{
		// 루프나 사다리가 있다면 타기
		//Dir = L"None";
		//m_PlayerState = JUMP_UP;

	}
	else if (true == Input::DownStay(L"Down"))
	{
		// 엎드리기
		//Dir = L"None";
		m_PlayerState = DOWN;
	}


	if (true == Input::Down(L"Jump"))
	{
		m_PlayerState = JUMP_UP;

		if (nullptr != m_SoundPlayer)
		{
			m_SoundPlayer->Stop();
			m_SoundPlayer->Play(L"Jump.mp3");
		}
		return;
	}

	if (true == Input::Down(L"BaseAttack"))
	{
		m_PlayerState = ATTACK_BASE;
		Attack();
	}
}

void PlayerMove::Run()
{
	if (true == Input::Down(L"Jump"))
	{
		m_JumpForce = JUMPFORCE;
		m_PlayerState = JUMP_UP;
		m_SoundPlayer->Play(L"Jump.mp3");
		return;
	}


	if (L"Left" == Dir)
	{
		if (true == Input::Up(L"Left"))
		{
			m_PlayerState = IDLE;
			return;
		}
	}
	else if (L"Right" == Dir)
	{
		if (true == Input::Up(L"Right"))
		{
			m_PlayerState = IDLE;
			return;
		}
	}


}

void PlayerMove::Jump_Up()
{
	// 점프중인 상태일때
	// 점프의 최대치에 도달했을때
	if (0.0f >= m_JumpForce)
	{
		m_JumpForce = 0.0f;
		m_PlayerState = JUMP_DOWN;
		return;
	}

	Vector4 JumpPos = Vector4::Up * m_JumpForce * Time::DeltaTime();

	Vector4 MovePos = JumpPos;
	GetTransform()->Local_Move(MovePos);

	m_JumpForce -= 1700.0f * Time::DeltaTime();
}

void PlayerMove::Jump_Down()
{
	// 아래로 떨어지는 상태

	Vector4 JumpPos = Vector4::Up * m_JumpForce * Time::DeltaTime();

	Vector4 MovePos = JumpPos /*+ DirPos*/;
	GetTransform()->Local_Move(MovePos);

	m_JumpForce -= 1700.0f * Time::DeltaTime();

	if (m_JumpForce <= -(JUMPFORCE))
	{
		m_JumpForce = -(JUMPFORCE);
	}

	// 픽셀충돌하기
	Vector4 TestVec = Vector4{ -1, -1, -1, -1 };


	TestVec = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(GetTransform()->World_Position().x), (unsigned int)(GetTransform()->World_Position().y - (GetTransform()->Local_Scale().y / 2) + 8));

	if (TestVec.a > 0)
	{
		m_JumpForce = JUMPFORCE;
		m_PlayerState = IDLE;
		GetTransform()->Local_Move(Vector4::Up);
		return;
	}
}

void PlayerMove::Down()
{
	if (true == Input::Up(L"Down"))
	{
		m_PlayerState = IDLE;
	}

	if (true == Input::DownStay(L"BaseAttack"))
	{
		m_PlayerState = DOWN_ATTACK;
	}
}

void PlayerMove::Rope()
{
	// 로프 충돌
}

void PlayerMove::Ladder()
{
	// 사다리 충돌
}

void PlayerMove::Attack()
{
	AttackPattern = rand() % 3;

	if (0 == AttackPattern)
	{
		m_PlayerState = ATTACK_SKILL_01;
	}
	else if (1 == AttackPattern)
	{
		m_PlayerState = ATTACK_SKILL_02;
	}
	else
	{
		m_PlayerState = ATTACK_SKILL_03;
	}
}

void PlayerMove::AttackAniChange()
{
	AttackPattern = rand() % 3;

	if (0 == AttackPattern)
	{
		m_PlayerState = ATTACK_SKILL_01;
	}
	else if (1 == AttackPattern)
	{
		m_PlayerState = ATTACK_SKILL_02;
	}
	else
	{
		m_PlayerState = ATTACK_SKILL_03;
	}
}

void PlayerMove::Death()
{

}

Vector4 PlayerMove::DirMove()
{
	Vector4 DirPos = Vector4::Zero;

	if (true == Input::DownStay(L"Left"))
	{
		Dir = L"Left";
		GetTransform()->Local_Move(Vector4::Left * m_MoveSpeed * Time::DeltaTime());
	}
	else if (true == Input::DownStay(L"Right"))
	{
		Dir = L"Right";
		GetTransform()->Local_Move(Vector4::Right * m_MoveSpeed * Time::DeltaTime());
	}
	
	return Vector4::Zero;
}

