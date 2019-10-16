#include "Pinkbin.h"
#include "RenderOrder.h"
#include "Pinkbin_AttackEffect.h"

#include <Scene.h>
#include <RectCol2D.h>

#include "MonsterHitScript.h"


void Pinkbin::BaseManager()
{

	int RandBaseState = rand() % 15;

	RandBaseState = 15;
	switch (RandBaseState)
	{
	case 0:
		SoundPlay_Pinkbin(L"3.mp3");
		m_State = MonsterState::STAND;
		break;
	case 1:
	case 2:
		SoundPlay_Pinkbin(L"29.mp3");
		if (true == m_bMove)
		{
			m_State = MonsterState::STAND;
		}
		else
		{
			m_State = MonsterState::MOVE;
		}
		break;
	case 3:
	case 4:
		SoundPlay_Pinkbin(L"29.mp3");
		m_State = MonsterState::SKILL_6;
		break;
	case 5:
	case 6:
		SoundPlay_Pinkbin(L"3.mp3");
		m_State = MonsterState::SKILL_7;
		break;
	case 7:
	case 8:
		m_State = MonsterState::SKILL_8;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		AttackManager();
		break;
	case 14:
	case 15:
		SkillManager();
	default:
		break;
	}
}

void Pinkbin::AttackManager()
{

	int RandBaseState = rand() % 9;

	switch (RandBaseState)
	{
	case 0:
		SoundPlay_Pinkbin(L"5.mp3");
		m_State = MonsterState::STAND;
		break;
	case 1:
	case 2:
		SoundPlay_Pinkbin(L"29.mp3");
		m_State = MonsterState::ATTACK1;
		break;
	case 3:
	case 4:
		SoundPlay_Pinkbin(L"Laugh_02.mp3");
		m_State = MonsterState::ATTACK2;
		break;
	case 5:
	case 6:
		SoundPlay_Pinkbin(L"5.mp3");
		m_State = MonsterState::ATTACK3;
		break;
	case 7:
	case 8:
		SoundPlay_Pinkbin(L"29.mp3");
		m_State = MonsterState::ATTACK4;
		break;
	default:
		break;
	}
}

void Pinkbin::SkillManager()
{
	int RandBaseState = rand() % 16;

	switch (RandBaseState)
	{
	case 0:
		m_State = MonsterState::STAND;
		break;
	case 1:
	case 2:
	case 3:
		SoundPlay_Pinkbin(L"29.mp3");
		m_State = MonsterState::SKILL_1;
		break;
	case 4:
	case 5:
	case 6:
		SoundPlay_Pinkbin(L"5.mp3");
		m_State = MonsterState::SKILL_2;
		break;
	case 7:
	case 8:
	case 9:
		SoundPlay_Pinkbin(L"6.mp3");
		m_State = MonsterState::SKILL_3;
		break;
	case 10:
	case 11:
	case 12:
		SoundPlay_Pinkbin(L"Laugh_01.mp3");
		m_State = MonsterState::SKILL_4;		// 순간이동
		break;
	case 13:
	case 14:
	case 15:
		SoundPlay_Pinkbin(L"6.mp3");
		m_State = MonsterState::SKILL_5;		
		break;
	default:
		break;
	}
}

// Base
void Pinkbin::Stand()
{
	if (m_CurStateTime < m_StandStateChangeTime)
	{
		m_CurStateTime += Time::DeltaTime();
		return;
	}

	m_CurStateTime = 0.0f;


	// 상태변화
	int RandomState = (rand() % 4) + 1;

	switch (RandomState)
	{
	case 1:
		BaseManager();
		break;
	case 2:
	case 3:
		AttackManager();
		break;
	case 4:
		SkillManager();
		break;
	default:
		break;
	}

}

void Pinkbin::Move()
{
	DirMove();
}

void Pinkbin::Hit()
{
	if (m_CurStateTime >= m_HitTime)
	{
		m_CurStateTime = 0.0f;
		return;
	}

	m_CurStateTime += Time::DeltaTime();
}

void Pinkbin::Die()
{
	if (L"Die" == m_Animation->CurAnimationName() && true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Death();
		return;
	}
}


// Attack
void Pinkbin::Attack1()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_bAttack = false;
		m_State = MonsterState::STAND;
	}

	if (false == m_bAttack)
	{
		My_PTR<GameObject> NewAttack1 = GetGameObject()->GetScene()->GameObject_Create(L"Pinkbin_Attack1");
		Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
		CurPos.z -= 1.0f;
		CurPos.y += 180.0f;
		NewAttack1->GetTransform()->Local_Position(CurPos);
		NewAttack1->GetTransform()->Local_Scale(Vector4(841.0f, 817.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewAttackRenderer = NewAttack1->Component_Add<SpriteRenderer2D>(RenderOrder::MONSTER_ATTACK);
		NewAttackRenderer->SetSprite(L"Attack1_Effect.png", 0);
		My_PTR<Animation2D> NewAttackAnimation = NewAttack1->Component_Add<Animation2D>();
		My_PTR<Pinkbin_AttackEffect> NewAttackEffect = NewAttack1->Component_Add<Pinkbin_AttackEffect>();
		NewAttackEffect->SetAttack(L"Attack1");

		m_bAttack = true;


		//// 캐릭터한테 이펙트 쏴주기
		//// 이펙트
		//My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
		//Vector4 CharacterPos = m_pPlayer->GetTransform()->Local_Position();
		//CharacterPos.z = -1.0f;
		//NewHitEffect->GetTransform()->Local_Position(CharacterPos);
		//NewHitEffect->GetTransform()->Local_Scale(Vector4(97.0f, 107.0f, 1.0f, 1.0f));
		//My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::PLAYER_CHILD);
		//NewHitEffectRenderer->SetSprite(L"Attack1_Hit.png", 0);
		//My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
		//My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
		//NewHitEffectScript->SetMovePos(Vector4(0.0f, 10.0f, CharacterPos.z, 0.0f));
		//NewHitEffectScript->SetSkillName(L"BOSS_01");
	}
}

void Pinkbin::Attack2()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_bAttack = false;
		m_State = MonsterState::STAND;
	}
}

void Pinkbin::Attack3()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_bAttack = false;
		m_State = MonsterState::STAND;
	}

	if (false == m_bAttack)
	{
		My_PTR<GameObject> NewAttack3 = GetGameObject()->GetScene()->GameObject_Create(L"Pinkbin_Attack3");
		Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
		CurPos.z -= 1.0f;
		NewAttack3->GetTransform()->Local_Position(CurPos);
		NewAttack3->GetTransform()->Local_Scale(Vector4(214.0f, 555.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewAttackRenderer = NewAttack3->Component_Add<SpriteRenderer2D>(RenderOrder::MONSTER_ATTACK);
		NewAttackRenderer->SetSprite(L"Attack3_Effect.png", 0);
		My_PTR<Animation2D> NewAttackAnimation = NewAttack3->Component_Add<Animation2D>();
		My_PTR<Pinkbin_AttackEffect> NewAttackEffect = NewAttack3->Component_Add<Pinkbin_AttackEffect>();
		NewAttackEffect->SetAttack(L"Attack3");

		m_bAttack = true;
	}
}

void Pinkbin::Attack4()
{

	if (true == m_Animation->IsEndAnimation())
	{
		m_bAttack = false;
		m_State = MonsterState::STAND;
	}

	if (false == m_bAttack)
	{
		My_PTR<GameObject> NewAttack3 = GetGameObject()->GetScene()->GameObject_Create(L"Pinkbin_Attack4");
		Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
		CurPos.z -= 1.0f;
		NewAttack3->GetTransform()->Local_Position(CurPos);
		NewAttack3->GetTransform()->Local_Scale(Vector4(109.0f, 159.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewAttackRenderer = NewAttack3->Component_Add<SpriteRenderer2D>(RenderOrder::MONSTER_ATTACK);
		NewAttackRenderer->SetSprite(L"Attack4_Effect.png", 0);
		My_PTR<Animation2D> NewAttackAnimation = NewAttack3->Component_Add<Animation2D>();
		My_PTR<Pinkbin_AttackEffect> NewAttackEffect = NewAttack3->Component_Add<Pinkbin_AttackEffect>();
		NewAttackEffect->SetAttack(L"Attack4");

		m_bAttack = true;
	}
}



// Skill
void Pinkbin::Skill1()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_State = MonsterState::STAND;
		return;
	}
}

void Pinkbin::Skill2()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_State = MonsterState::STAND;
		return;
	}
}

void Pinkbin::Skill3()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_State = MonsterState::STAND;
		return;
	}
}

void Pinkbin::SKill4()
{
	if (true == m_Animation->IsEndAnimation())
	{
		Vector4 CurPos = GetTransform()->Local_Position();
		CurPos.x -= 19.0f;
		GetGameObject()->GetTransform()->Local_Position(CurPos);
		m_State = MonsterState::SKILL_4_AFTER;
		return;
	}
}

void Pinkbin::Skill4_After()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_State = MonsterState::STAND;
		return;
	}
}

void Pinkbin::Skill5()
{
	if (true == m_Animation->IsEndAnimation())
	{
		m_State = MonsterState::STAND;
		return;
	}
}

// Base

void Pinkbin::Skill6()
{

}

void Pinkbin::Skill7()
{

}

void Pinkbin::Skill8()
{

}