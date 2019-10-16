#pragma once
#include <Script.h>
#include "ShopBuyItemBt.h"
#include "EnumOrder.h"
#include "ItemCode.h"

class GameObject;
class ShopSellItemManagerScript : public Script, public EnumOrder
{
private:
	std::vector<GameObject*> m_InvenItemConsum;
	std::vector<GameObject*> m_ShopSellMyItem;

	std::vector<GameObject*> m_ShopInvenItem;
	std::vector<bool> m_ShopInvenMyItemClickVector;

private:
	void CheckInventoryItem();

public:
	void SetInvenItemCounsumVector(std::vector<GameObject*> _InvenItemConsumVector);
	void SetMyItem(std::vector<GameObject*> _Item);

	void SetMyItemSelect(std::vector<bool> _SelectItem);

	std::vector<GameObject*> GetShopSellMyItem();
	std::vector<GameObject*> GetShopInvenMyItem();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	ShopSellItemManagerScript();
	~ShopSellItemManagerScript();
};

