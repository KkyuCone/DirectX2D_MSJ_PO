#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class CataclysmHit : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_CataHitRenderer;
	My_PTR<Animation2D> m_CataHitAnim;

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	CataclysmHit();
	~CataclysmHit();
};

