#pragma once
#include "MonsterState.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "GeneralMonster_HP.h"

#define WSRMOVESPEED 60
#define WSR_HP 8000
#define KNOCKBACKVALUE 20

class WhiteSandRabbit : public MonsterState, public Script
{
private:
	int m_HP;
	float KnockBack;
	float m_MoveSpeed;
	float m_LDistance;						// 추적가능한 거리

	float m_CurStateTime;
	float m_StandStateChangeTime;			// Stand상태 -> 변경시간
	float m_MoveChangeTime;					// Move->Stand 변경 시간
	float m_HitTime;						// 맞았을때 시간(애니메이션)

	bool m_bUpTile;

	bool m_bHit;
	bool m_bDirChange;
	bool m_bTraceMode;
	bool m_bTraceDirChange;					// 추격 중에 방향이 바뀌었을경우(타일의 끝부분..에서 방향이 바뀔때)

	std::wstring m_Dir;

private:
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
	void AniDirCheck();
	void DirMove();
	void DirChange();
	void Gravity();
	void TraceMode();

	bool EmptyTileCheck();

	void ChangeAnimation(std::wstring _AniName);

public:
	void SetPlayer(GameObject* _Player);
	void SetFloor(GameObject* _Floor);
	void SetBackground(Vector4 _Size);
	void SetUpTilePos(bool _Value);
	void SetHPBar(GeneralMonster_HP* _Script);

public:
	int GetHP();
	void Damage(int _Value);

private:
	void Stand();
	void Move();
	void Hit();
	void Die();
	void Attack();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	WhiteSandRabbit();
	~WhiteSandRabbit();
};

