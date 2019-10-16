#include "ShopSellItemManagerScript.h"
#include "ShopBuyItemBt.h"
#include "ShopBuyItemNumber.h"
#include "Inven_Info.h"
#include <GameObject.h>


ShopSellItemManagerScript::ShopSellItemManagerScript()
{
}


ShopSellItemManagerScript::~ShopSellItemManagerScript()
{
}

//

bool ShopSellItemManagerScript::Init()
{
	return true;
}

void ShopSellItemManagerScript::Update()
{
	if (0 == m_ShopSellMyItem.size())
		return;

	if (0 == m_InvenItemConsum.size())
		return;

	CheckInventoryItem();
}

void ShopSellItemManagerScript::FinalUpdate()
{

}


//

void ShopSellItemManagerScript::SetMyItem(std::vector<GameObject*> _GameObj)
{
	m_ShopSellMyItem = _GameObj;
}

void ShopSellItemManagerScript::SetInvenItemCounsumVector(std::vector<GameObject*> _InvenItemConsumVector)
{
	m_InvenItemConsum = _InvenItemConsumVector;
}

void ShopSellItemManagerScript::SetMyItemSelect(std::vector<bool> _SelectItem)
{
	m_ShopInvenMyItemClickVector = _SelectItem;
}

std::vector<GameObject*> ShopSellItemManagerScript::GetShopSellMyItem()
{
	return m_ShopSellMyItem;
}

std::vector<GameObject*> ShopSellItemManagerScript::GetShopInvenMyItem()
{
	return m_ShopInvenItem;
}

//

void ShopSellItemManagerScript::CheckInventoryItem()
{
	// 벡터 비워주기
	m_ShopInvenItem.erase(m_ShopInvenItem.begin(), m_ShopInvenItem.end());

	std::vector<GameObject*> InventoryItem;

	for (size_t i = 0; i < m_InvenItemConsum.size(); ++i)
	{
		ItemCode::ItemCodeName InvenItemCodeName = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCode();

		if (ItemCode::ItemCodeName::EMPTY != InvenItemCodeName)
		{
			InventoryItem.push_back(m_InvenItemConsum[i]);
			m_ShopInvenItem.push_back(m_InvenItemConsum[i]);
		}

		if (5 <= InventoryItem.size())
			break;
	}
	

	// 맞는 이미지 넣어주기, 개수도 넣어주기
	for (size_t k = 0; k < InventoryItem.size(); ++k)
	{
		ItemCode::ItemCodeName InvenItemCodeName = InventoryItem[k]->GetComponent<Inven_Info>()->GetItemCode();
		int Count = InventoryItem[k]->GetComponent<Inven_Info>()->GetItemCount();

		if (ItemCode::ItemCodeName::POTION_RED == InvenItemCodeName)
		{
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemBt>()->SetSprite(L"Shop_RedPoiton.png");
		}
		else if (ItemCode::ItemCodeName::POTION_BLUE == InvenItemCodeName)
		{
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemBt>()->SetSprite(L"Shop_BluePotion.png");
		}
		else if (ItemCode::ItemCodeName::POTION_ORANGE == InvenItemCodeName)
		{
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemBt>()->SetSprite(L"Shop_OrangePotion.png");
		}
		else if (ItemCode::ItemCodeName::POTION_WHITE == InvenItemCodeName)
		{
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemBt>()->SetSprite(L"Shop_WhitePotion.png");
		}
		else if (ItemCode::ItemCodeName::POTION_ELIXIR == InvenItemCodeName)
		{
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemBt>()->SetSprite(L"Shop_ManaElixir.png");
		}

		if (ItemCode::ItemCodeName::EMPTY == InvenItemCodeName)
		{
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemBt>()->SetSprite(L"EmptyItem.png");
			m_ShopSellMyItem[k]->GetComponent<ShopBuyItemNumber>()->SetCount(0);
			return;
		}

		m_ShopSellMyItem[k]->GetComponent<ShopBuyItemNumber>()->SetCount(Count);

	}

	// 5개 미만일때 나머지 채워주기
	if (5 > InventoryItem.size())
	{
		size_t InvenItemValue = InventoryItem.size();

		if( 0 == InvenItemValue)
		{
			for (size_t i = 0 ; i < 5; ++i)
			{
				m_ShopSellMyItem[i]->GetComponent<ShopBuyItemBt>()->SetSprite(L"EmptyItem.png");
			}
		}
		else
		{
			for (size_t i = InvenItemValue; i < 5; ++i)
			{
				m_ShopSellMyItem[i]->GetComponent<ShopBuyItemBt>()->SetSprite(L"EmptyItem.png");
			}
		}
	}
}

