#pragma once
#include <Script.h>
#include "FSMState.h"
#include <string>


#define JUMPFORCE 600

class GameObject;
class PlayerAnimation;
class SoundPlayer;
class PlayerMove : public Script, public FSMState
{
	friend class PlayerAnimation;
private:
	enum INPUT_KEY
	{
		I_UP,
		I_LEFT,
		I_RIGHT,
		I_DOWN,
		I_MAX,
	};

private:
	SoundPlayer* m_SoundPlayer;

private:
	GameObject* m_Floors;			// 픽셀충돌하려고 추가
	My_PTR<GameObject> m_PlayerNameTag;

public:
	bool GetIsMove();
	float GetMoveSpeed();
	Vector4 GetWPos();
	FSM_STATE GetState();
	PlayerMove* GetPlayerMoveScript();
	
public:
	void AddFloor(GameObject* _Floor);			// 바닥 이미지 추가

private:
	const float m_Acceleration = 30.0f;
	float  m_JumpForce;
	float m_MoveSpeed;
	int AttackPattern;
	bool m_bDownCheck;
	bool m_bMoveCheck;
	FSM_STATE m_PlayerState;
	std::wstring Dir;				// 방향



public:
	PlayerMove* GetPlayerScript()
	{
		return this;
	}

	std::wstring GetDir();

	void SetSoundPlayer(SoundPlayer* _Player);

private:
	void Idle();
	void Run();
	void Jump_Up();
	void Jump_Down();
	void Down();
	void Rope();
	void Ladder();
	void Attack();
	void Death();

private:
	Vector4 DirMove();
	void GDown();

public:
	void AttackAniChange();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	PlayerMove();
	~PlayerMove();
};

