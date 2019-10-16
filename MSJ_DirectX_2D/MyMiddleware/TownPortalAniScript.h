#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class TownPortalAniScript : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	TownPortalAniScript();
	~TownPortalAniScript();
};

