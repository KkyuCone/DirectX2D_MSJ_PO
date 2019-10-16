#pragma once
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include <Script.h>

class BEEffect : public Script
{
private:
	float m_LifeTime;

private:
	My_PTR<SpriteRenderer2D> m_BlazeEffectRenderer;
	My_PTR<Animation2D> m_BlazeEffectAnim;

private:
	void DeathTimer();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BEEffect();
	~BEEffect();
};

