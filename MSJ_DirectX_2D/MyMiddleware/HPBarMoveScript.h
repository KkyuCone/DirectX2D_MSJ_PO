#pragma once
#include <Script.h>
#include <GameObject.h>

class HPBarMoveScript : public Script
{
private:
	float m_YPivot;
	Vector4 m_Pivot;
	My_PTR<GameObject> m_Monster;

private:
	void Move();

public:
	void SetMosnter(GameObject* _Monster);
	void SetPivot(float _Pos);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	HPBarMoveScript();
	~HPBarMoveScript();
};

