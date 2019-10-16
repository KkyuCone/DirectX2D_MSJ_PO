#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class RectCol2D;
class SSoulPenetration : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_SoulPeRenderer;
	My_PTR<Animation2D> m_SoulPeAnim;
	My_PTR<RectCol2D> m_RectCol;

private:
	void EffectDeath();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	SSoulPenetration();
	~SSoulPenetration();
};

