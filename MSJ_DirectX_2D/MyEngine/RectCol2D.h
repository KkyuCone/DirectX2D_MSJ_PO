#pragma once
#include "Collision.h"
//#include "SpriteRenderer2D.h"
//#include"Transform.h"

class Camera;
class RectCol2D : public Collision
{
private:
	bool m_bUI;
	Camera* m_UICamera;

private:
	Matrix VP;
	Matrix UIVP;
	// 충돌체 그림(?)띄우기 - 사각형
	//SpriteRenderer2D* m_CollisionRender;

private:
	Vector4 Data;

public:
	bool Collision_Check(Collision* _OtherCol) override;
	void DebugRender() override;
	void CollisionDataUpdate() override;

public:
	void SetUICamera(Camera* _UICamera);

public:
	void SetColSize(Vector2 _Size);
	Vector2 GetColSize();

public:
	RectCol2D();
	~RectCol2D();
};

