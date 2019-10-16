#pragma once
#include <Script.h>
#include "FSMState.h"

#define SCENEW 1024
#define SCENEH 768

class GameObject;
class Sprite;
class InGameCamera : public Script
{
private:
	bool m_bNewSetBGSize;
	float m_BGMinX;
	float m_BGMaxX;

private:
	float m_MapWidth;
	float m_MapHeight;
	float m_MinSpeed;
	float m_MaxSpeed;
	float m_MinJumpForce;
	float m_MaxJumpForce;

	float m_SceneHX;
	float m_SceneHY;

	float m_AccelSpeed;
	float m_AccelSpeedY;
	float m_YResult;

	bool m_bMove;
	bool m_bRMove;
	bool m_bLMove;

	Vector4 StartPos;
	Vector4 EndPos;

private:
	std::wstring BGName;

private:
	GameObject* m_pCurMap;
	GameObject* m_pPlayer;

private:
	My_PTR<GameObject> m_Snow;

private:
	void Move();

	void LMove();
	void RMove();
	void LUMove();
	void RUMove();

	void UMove();
	void DMove();
	void UUMove();
	void DUMove();


public:
	void SetCurMap(GameObject* _CurMap);
	void SetPlayer(GameObject* _Player);
	void SetMovePos(Vector4 _Pos);
	void SetMoveCheck(bool _Value);
	void SetSceneXSize(float _MinX, float _MaxX);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	InGameCamera();
	~InGameCamera();
};

