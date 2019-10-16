#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "EnumOrder.h"
#include "RenderOrder.h"

class GameObject;
class MesoManager : public Script, public RenderOrder, public EnumOrder
{
private:
	static int m_Meso;

	std::vector<GameObject*> m_MoneyNumbers;
	std::vector<SpriteRenderer2D*> m_MoneyRenderer;
	std::vector<SpriteRenderer2D*> m_CommaRenderer;

private:
	void ChangeMeso();

public:
	void Money_Off();
	void Money_On();

public:
	void AddMeso(int _Value);
	void DecMeso(int _Value);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	MesoManager();
	~MesoManager();
};

