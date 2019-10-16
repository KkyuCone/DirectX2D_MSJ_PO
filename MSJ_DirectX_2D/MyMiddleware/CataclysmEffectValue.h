#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class CataclysmEffectValue : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_CataEffectRenderer;
	My_PTR<Animation2D> m_CataEffectAnim;

private:
	void IsEndAnimation();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	CataclysmEffectValue();
	~CataclysmEffectValue();
};

