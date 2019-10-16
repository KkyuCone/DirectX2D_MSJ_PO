#pragma once
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include <Script.h>

class RectCol2D;
class PoisonCloud : public Script
{
private:
	float m_LifeTime;
	float m_CurTime;
	int m_PoisonCount;
	bool m_bTrans;

private:
	My_PTR<SpriteRenderer2D> m_PoisonCloudRenderer;
	My_PTR<Animation2D> m_PoisonAnim;

	My_PTR<RectCol2D> m_RectCol;

private:
	void PCSeeting();
	void PCTransTimer();
	void PCTransparent();

private:
	void RectColOff();
	void RectColOn();

public:
	void SetPosionCount(int _Count);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	PoisonCloud();
	~PoisonCloud();
};

