#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class BGFade : public Script
{
private:
	bool m_bFade;
	float m_CurTime;

public:
	bool m_bFadeDown;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BGFade();
	~BGFade();
};

