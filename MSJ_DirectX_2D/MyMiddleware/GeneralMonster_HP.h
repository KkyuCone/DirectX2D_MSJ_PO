#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "RenderOrder.h"
#include <SpriteRenderer2D.h>

class GeneralMonster_HP : public Script, public RenderOrder
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<GameObject> m_Monster;

	GameObject* m_HPBar01;
	GameObject* m_HPBar02;
	GameObject* m_HPBar03;

private:
	bool m_bZeroHP;
	int m_CurHPValue;
	int m_HPFullValue;

public:
	void SetHP(int _Value);
	void SetHPBarEtc(GameObject* _HP01, GameObject* _HP02, GameObject* _HP03);

public:
	void Damage(int _Value);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	GeneralMonster_HP();
	~GeneralMonster_HP();
};

