#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class Inven_BtAnimation : public Script
{
private:
	bool m_bPressed;
	float m_CurPressedTime;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

private:
	void PressedAniCheck();

public:
	void SetAnimation(std::wstring _Name);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Inven_BtAnimation();
	~Inven_BtAnimation();
};

