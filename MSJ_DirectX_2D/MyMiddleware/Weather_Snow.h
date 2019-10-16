#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class Weather_Snow : public Script
{
private:
	bool m_bSnowAni1;
	bool m_bSnowAni2;
	bool m_bSnowAni3;
	bool m_bSnowAni4;
	bool m_bSnowAni5;
	bool m_bSnowAni6;


private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

private:
	void SnowEndAni();
	void SnowChange();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Weather_Snow();
	~Weather_Snow();
};

