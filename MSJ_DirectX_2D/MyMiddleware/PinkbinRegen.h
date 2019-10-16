#pragma once
#include "MonsterState.h"
#include "RenderOrder.h"


#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>


class PinkbinRegen : public MonsterState, public Script
{
private:
	bool m_bAniEnd;
	bool m_bUpTile;
	Vector4 BackgroundSize;
	Vector4 OriginScale;

private:
	FSM_MState m_State;
	GameObject* m_pPlayer;
	GameObject* m_pFloor;
	My_PTR<SpriteRenderer2D> m_SpRenderer;
	My_PTR<Animation2D> m_Animation;

	My_PTR<GameObject> m_NewPinkbin;


private:
	void CheckAnimationEnd();
	void Create_Pinkbin();

public:
	void PinkbinDeath();

public:
	void SetPlayer(GameObject* _Player);
	void SetFloor(GameObject* _Floor);
	void SetBackground(Vector4 _Size);
	void SetUpTilePos(bool _Value);
	GameObject* GetPinkbin();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	PinkbinRegen();
	~PinkbinRegen();
};

