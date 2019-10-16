#pragma once
#include <Script.h>

class GameObject;
class UIMove : public Script
{
private:
	float m_BasePosX;
	float m_BasePosY;
	float m_BasePosZ;

private:
	GameObject* m_pCamera;

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

private:
	void Move();

public:
	void SetCamera(GameObject* _Obj);
	void SetBasePos(Vector4 _Pos);


public:
	UIMove();
	~UIMove();
};

