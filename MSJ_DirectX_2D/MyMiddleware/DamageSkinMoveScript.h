#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>

class DamageSkinMoveScript : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
private:
	float m_AlphaValue;
	float m_CurTime;
	float m_DeathTime;
	Vector4 m_InitPos;

private:
	void DamageSkinDeathTimer();
	void DamageSkinMove();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	DamageSkinMoveScript();
	~DamageSkinMoveScript();
};

