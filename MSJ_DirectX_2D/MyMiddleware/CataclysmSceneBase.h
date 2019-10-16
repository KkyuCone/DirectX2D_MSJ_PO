#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class RectCol2D;
class CataclysmSceneBase : public Script
{
private:
	bool m_bAlpha;
	float m_CurTime;
	bool m_bRectCreate;

private:
	My_PTR<SpriteRenderer2D> m_BaseSceneRenderer;
	My_PTR<Animation2D> m_BaseSceneAnim;

	My_PTR<RectCol2D> m_RectCol;

private:
	void DeathTimer();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	CataclysmSceneBase();
	~CataclysmSceneBase();
};

