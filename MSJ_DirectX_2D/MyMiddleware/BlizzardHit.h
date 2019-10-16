#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class BlizzardHit : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_HitRenderer;
	My_PTR<Animation2D> m_HitAnim;

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BlizzardHit();
	~BlizzardHit();
};

