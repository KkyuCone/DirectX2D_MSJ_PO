#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class SBlizzard : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_BlizzardRenderer;
	My_PTR<Animation2D> m_BlizzardAnim;

private:
	void CheckDeath();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	SBlizzard();
	~SBlizzard();
};

