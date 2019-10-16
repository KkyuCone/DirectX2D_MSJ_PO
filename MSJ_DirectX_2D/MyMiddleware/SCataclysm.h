#pragma once
#include "RenderOrder.h"
#include "CataclysmEffectValue.h"
#include "CataclysmScene.h"

#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class SCataclysm : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_CataclysmRenderer;
	My_PTR<Animation2D> m_CataclysmAnim;

private:
	void IsEndAnimation();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	SCataclysm();
	~SCataclysm();
};

