#pragma once
#include <Script.h>
#include "RenderOrder.h"
#include <SpriteRenderer2D.h>

class BossMonster_HP : public Script, public RenderOrder
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;

private:
	bool m_bZeroHP;
	int m_CurHPValue;
	int m_HPFullValue;

public:
	void SetHP(int _Value);

public:
	bool GetZeroHP();

public:
	void Damage(int _Value);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BossMonster_HP();
	~BossMonster_HP();
};

