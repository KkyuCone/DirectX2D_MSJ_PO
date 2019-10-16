#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include "ItemCode.h"

class Camera;
class Inven_MouseItem : public Script, public ItemCode
{
private:
	ItemCode::ItemCodeName m_ItemCodeName;
	My_PTR<SpriteRenderer2D> m_Renderer;

	Camera* m_Camera;

private:
	void Move();   

public:
	void SetClickItem(ItemCodeName _Code);
	void SetClickQuickSlot();
	void SetMouseCamera(Camera* _Mouse);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Inven_MouseItem();
	~Inven_MouseItem();
};

