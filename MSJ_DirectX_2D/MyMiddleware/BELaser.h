#pragma once
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include <Script.h>

class RectCol2D;
class BELaser : public Script
{
private:
	float m_LifeTime;
	float m_CutWidth;
	float m_CutRatio;

	float m_zRot;

	Vector4 m_monsterPos;

private:
	My_PTR<SpriteRenderer2D> m_BlazeRenderer;
	My_PTR<Animation2D> m_BlazeAnim;
	My_PTR<RectCol2D> m_RectCol;

private:
	void DeathTimer();

public:
	void SetCutWidth(float _CutWidth);
	void SetDegree(float _Rot);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;


public:
	BELaser();
	~BELaser();
};

