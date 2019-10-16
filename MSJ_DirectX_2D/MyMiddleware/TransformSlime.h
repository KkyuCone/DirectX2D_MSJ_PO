#pragma once
#include "MonsterState.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "GeneralMonster_HP.h"

#define TSMOVESPEED 60
#define TS_HP 1200

class TransformSlime : public MonsterState, public Script
{
private:
	float m_AttackYPosValue;
	float m_SkillYPosValue;

private:
	int m_HP;
	float KnockBack;
	float m_MoveSpeed;
	float m_LDistance;						// ���������� �Ÿ�

	float m_CurStateTime;
	float m_StandStateChangeTime;			// Stand���� -> ����ð�
	float m_MoveChangeTime;					// Move->Stand ���� �ð�
	float m_HitTime;
	float m_SkillTime;

	bool m_bUpTile;
	bool m_bHit;
	bool m_bDirChange;
	bool m_bTraceMode;
	bool m_bTraceDirChange;					// �߰� �߿� ������ �ٲ�������(Ÿ���� ���κ�..���� ������ �ٲ�)

	Vector4 BackgroundSize;
	Vector4 OriginScale;

	std::wstring m_Dir;

private:
	FSM_MState m_State;
	GameObject* m_pPlayer;
	GameObject* m_pFloor;
	My_PTR<SpriteRenderer2D> m_SpRenderer;
	My_PTR<Animation2D> m_Animation;
	My_PTR<GeneralMonster_HP> m_HPBarScript;

private:
	void AniDirCheck();
	void DirMove();
	void DirChange();
	void Gravity();
	void TraceMode();
	bool EmptyTileCheck();

	void AttackPosInit();
	void SkillPosInit();

	void ChangeAnimation(std::wstring _AniName);

private:
	void Stand();
	void Move();
	void Hit();
	void Die();
	void Attack();
	void Skill();


public:
	void SetPlayer(GameObject* _Player);
	void SetFloor(GameObject* _Floor);
	void SetBackground(Vector4 _Size);
	void SetUpTilePos(bool _Value);
	void SetHPBar(GeneralMonster_HP* _Script);

public:
	int GetHP()
	{
		return m_HP;
	}

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	TransformSlime();
	~TransformSlime();
};


