#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "ItemCode.h"
#include "ItemKind.h"

class ItemInfo : public ItemKind
{
public:
	friend class Inven_Info;

public:
	ItemKind::ITEM_KIND ItemKind = ItemKind::ITEM_KIND::NONE;
	std::wstring ItemName = L"EmptyItem.png";
	int Count = 0;
	bool m_bCount = false;

public:
	ItemInfo() {}
	~ItemInfo() {}
};

class Inven_Info : public Script, public ItemCode
{
private:
	bool m_bItemEmpty;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;

private:
	ItemInfo* m_Info;
	ItemCode::ItemCodeName m_ItemCodeName;

private:
	// 개수 표시
	My_PTR<GameObject> m_HundredNumber;
	My_PTR<GameObject> m_TensNumber;
	My_PTR<GameObject> m_UnitNumber;


	My_PTR<SpriteRenderer2D> HundredNumberRenderer;
	My_PTR<SpriteRenderer2D> TensNumberRenderer;
	My_PTR<SpriteRenderer2D> UnitNumberRenderer;

	void NumberMove();

public:
	void Number_On();
	void Number_Off();

private:
	void ChangeItemTexture(std::wstring _Name);
	void CreateItemCountTexture();
	void SetItemCountTexture(int _Value);

public:
	ItemInfo* GetInfo();
	ItemCode::ItemCodeName GetItemCode()
	{
		return m_ItemCodeName;
	}
	int GetItemCount();
	SpriteRenderer2D* GetRenderer()
	{
		return m_Renderer;
	}

	void RenderOff();
	void RenderOn();


public:
	void SetInfo(ItemKind::ITEM_KIND _Kind, std::wstring _Name, int Count = 1, bool _bCount = false);
	void SetItemKind(ItemKind::ITEM_KIND _Kind);
	void SetItemCount(int _Count);

	void SetEmptyTexture();

public:
	void ItemAdd(ItemCode::ItemCodeName _Code);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Inven_Info();
	~Inven_Info();
};

