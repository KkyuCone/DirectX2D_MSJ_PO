#pragma once
#include "MonsterState.h"

#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include <Script.h>
#include "GeneralMonster_HP.h"

#define CMMOVESPEED 80

class GameObject;
class ChildManneqin : public MonsterState, public Script
{
private:
	int m_HP;
	float KnockBack;

private:
	float m_MoveSpeed;
	float m_CurStateTime;
	float m_StandChangeTime;
	float m_MoveChangeTime;
	float m_AttackModeTime;			// 캐릭터와 일정 거리가 벗어나고서 시간체크..공격모드해제
	float m_HitTime;
	float m_LDistance;				// 거리 한계
	float m_CurAttackTime;
	float m_DirChangeTime;

	bool m_bDirChange;
	bool m_bAttDirChange;
	bool m_bAttack;
	bool m_bHit;
	std::wstring m_Dir;

	Vector4 OriginScale;
	Vector4 BackgroundPos;

private:
	FSM_MState m_State;
	GameObject* m_pPlayer;
	GameObject* m_pFloor;
	My_PTR<SpriteRenderer2D> m_SpRenderer;
	My_PTR<Animation2D> m_Animation;
	My_PTR<GeneralMonster_HP> m_HPBarScript;

private:
	void Gravity();
	void DirChange();
	void DirTileChange();
	void HitChange();
	bool EmptyTileCheck();

	void ChangeAnimation(std::wstring _AniName);

private:
	void Stand();
	void Move();
	void AttackMove();
	void Hit();
	void Die();

	void AniDirCheck();
	void DirMove();

public:
	void SetPlayer(GameObject* _Player);
	void SetFloor(GameObject* _Floor);
	void SetHit(bool _Value);
	void SetAttack(bool _Value);
	void SetBackground(Vector4 _BG);
	void SetHPBar(GeneralMonster_HP* _Script);

	int GetHP();

	void Damage(int _Value);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;
	

public:
	ChildManneqin();
	~ChildManneqin();
};

