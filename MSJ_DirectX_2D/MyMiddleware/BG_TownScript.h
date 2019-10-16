#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class GameObject;
class BG_TownScript : public Script
{
private:
	bool m_bBGAni1;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animaiton;

private:
	void Check_BGAni();

	void ChangeBGSprite();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BG_TownScript();
	~BG_TownScript();
};

