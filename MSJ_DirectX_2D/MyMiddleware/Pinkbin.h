#pragma once
#include "MonsterState.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "BossMonster_HP.h"

#define PINKBIN_HP 80000
#define PINKBIN_MOVESPEED 100
#define PINKBIN_LDISTANCE 1000
//#define PINKBIN_

class SoundPlayer;
class Pinkbin : public MonsterState, public Script
{
private:
	My_PTR<SoundPlayer> m_PinkbinPlayer;

private:
	float m_AttackYPosValue;
	float m_SkillYPosValue;

private:
	float m_HP;
	float KnockBack;
	float m_MoveSpeed;
	float m_LDistance;						// 추적가능한 거리

	float m_CurStateTime;
	float m_StandStateChangeTime;			// Stand상태 -> 변경시간
	float m_MoveChangeTime;					// Move->Stand 변경 시간
	float m_HitTime;
	float m_SkillTime;

	bool m_bUpTile;
	bool m_bHit;
	bool m_bAttackMode;
	bool m_bBaseMode;
	bool m_bSkillMode;
	
	bool m_bMove;
	bool m_bAttack;

	Vector4 BackgroundSize;
	Vector4 OriginScale;

	Vector4 CurStandPos;

	std::wstring m_Dir;

private:
	FSM_MState m_State;
	FSM_MState m_PreState;
	GameObject* m_pPlayer;
	GameObject* m_pFloor;
	My_PTR<SpriteRenderer2D> m_SpRenderer;
	My_PTR<Animation2D> m_Animation;
	My_PTR<BossMonster_HP> m_HPScript;

private:
	// 사운드
	void PinkbinSoundLoading();
	void SoundPlay_Pinkbin(std::wstring _SoundFileName);

private:
	void ChangeSpriteToBase();
	void ChangeSpriteToAttack();
	void ChangeSpriteToSkill();

private:
	void BaseManager();
	void AttackManager();
	void SkillManager();

private:
	void Stand();
	void Move();
	void Hit();
	void Die();

	void Attack1();
	void Attack2();
	void Attack3();
	void Attack4();
	void Skill4_After();

	void Skill1();
	void Skill2();
	void Skill3();
	void SKill4();
	void Skill5();

	void Skill6();
	void Skill7();
	void Skill8();
	
private:
	void AniDirCheck();
	void DirMove();
	void DirChange();
	void Gravity();
	bool EmptyTileCheck();
	
	void AttackPosInit();
	void SkillPosInit();

	void ChangeAnimation(std::wstring _AniName);

public:
	void SetPlayer(GameObject* _Player);
	void SetFloor(GameObject* _Floor);
	void SetBackground(Vector4 _Size);
	void SetUpTilePos(bool _Value);
	void SetHPScript(BossMonster_HP* _Value);

	void SetDie();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Pinkbin();
	~Pinkbin();
};

