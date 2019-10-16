#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class BlizzardIcePillar : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_IcePillarRenderer;
	My_PTR<Animation2D> m_IcePillarAnim;

private:
	void CheckDeath();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BlizzardIcePillar();
	~BlizzardIcePillar();
};

