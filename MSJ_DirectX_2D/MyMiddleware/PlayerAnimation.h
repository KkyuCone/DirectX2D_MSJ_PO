#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "PlayerMove.h"

#define CharacterChildren 7					// 캐릭터 자식오브젝트 개수 - 애니메이션때문에 필요함

class FSMState;
class PlayerAnimation : public Script, public FSMState
{
private:
	static unsigned int AttAniCount;
	bool AttAniEnd;
	int AttAniEndCount;

private:
	My_PTR<SpriteRenderer2D> m_SpriteRender;
	My_PTR<Animation2D> m_Animation;
	size_t m_Index;

private:
	PlayerMove* m_PlayerMove;

public:
	void SetPaleyrMoveScript(PlayerMove* _PlayerScript)
	{
		m_PlayerMove = _PlayerScript;
	}

public:
	void SpecialAnimationEnd();

public:
	void Animation_Change(FSM_STATE _pState, std::wstring _Dir);

public:
	bool Init() override;
	void Update() override;

public:
	PlayerAnimation();
	~PlayerAnimation();
};

