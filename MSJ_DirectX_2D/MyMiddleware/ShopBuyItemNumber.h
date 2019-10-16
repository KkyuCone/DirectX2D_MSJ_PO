#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "EnumOrder.h"
#include "RenderOrder.h"

class GameObject;
class ShopBuyItemNumber : public Script, public EnumOrder, public RenderOrder
{
private:
	std::vector<GameObject*> m_NumbersObj;
	std::vector<SpriteRenderer2D*> m_NumbersRenderer;

public:
	void SetCount(int _Value);

public:
	void NumberUpdate_Off();
	void NumberUpdate_On();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	ShopBuyItemNumber();
	~ShopBuyItemNumber();
};

