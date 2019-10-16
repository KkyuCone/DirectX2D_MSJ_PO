#include "ShopBuyItemNumber.h"
#include <Scene.h>


ShopBuyItemNumber::ShopBuyItemNumber()
{
}


ShopBuyItemNumber::~ShopBuyItemNumber()
{
}


//

bool ShopBuyItemNumber::Init()
{
	Vector4 CurPos = GetTransform()->Local_Position();
	CurPos.z -= 1.0f;
	CurPos.x -= 71.5f;
	CurPos.y -= 11.0f;

	My_PTR<GameObject> UnitNumberObj = GetScene()->GameObject_Create(L"Shop_MyItemNumber_1");
	UnitNumberObj->GetTransform()->Local_Position(CurPos);
	UnitNumberObj->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> UnitNumberRenderer = UnitNumberObj->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	UnitNumberRenderer->SetSprite(L"EmptyItem.png");

	CurPos.x -= 7.0f;
	My_PTR<GameObject> TensNumberObj = GetScene()->GameObject_Create(L"Shop_MyItemNumber_10");
	TensNumberObj->GetTransform()->Local_Position(CurPos);
	TensNumberObj->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> TensNumberRenderer = TensNumberObj->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	TensNumberRenderer->SetSprite(L"EmptyItem.png");

	CurPos.x -= 7.0f;
	My_PTR<GameObject> HundredNumberObj = GetScene()->GameObject_Create(L"Shop_MyItemNumber_100");
	HundredNumberObj->GetTransform()->Local_Position(CurPos);
	HundredNumberObj->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> HundredNumberRenderer = HundredNumberObj->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	HundredNumberRenderer->SetSprite(L"EmptyItem.png");

	m_NumbersObj.push_back(HundredNumberObj);
	m_NumbersObj.push_back(TensNumberObj);
	m_NumbersObj.push_back(UnitNumberObj);

	m_NumbersRenderer.push_back(HundredNumberRenderer);
	m_NumbersRenderer.push_back(TensNumberRenderer);
	m_NumbersRenderer.push_back(UnitNumberRenderer);

	return true;
}

void ShopBuyItemNumber::Update()
{

}

void ShopBuyItemNumber::FinalUpdate()
{

}

//
void ShopBuyItemNumber::SetCount(int _Value)
{
	int HundredValue = _Value / 100;
	int TensValue = (_Value - (100 * HundredValue)) / 10;
	int UnitValue = ((_Value - (100 * HundredValue)) - (10 * TensValue));

	if (0 == HundredValue)
	{
		m_NumbersRenderer[EnumOrder::FIRST]->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		m_NumbersRenderer[EnumOrder::FIRST]->SetSprite(L"UI_StateNumber.png", HundredValue);
	}

	if (0 == HundredValue && 0 == TensValue)
	{
		m_NumbersRenderer[EnumOrder::SECOND]->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		m_NumbersRenderer[EnumOrder::SECOND]->SetSprite(L"UI_StateNumber.png", TensValue);
	}

	if (0 == HundredValue && 0 == TensValue && 0 == UnitValue)
	{
		m_NumbersRenderer[EnumOrder::THIRD]->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		m_NumbersRenderer[EnumOrder::THIRD]->SetSprite(L"UI_StateNumber.png", UnitValue);
	}
}


void ShopBuyItemNumber::NumberUpdate_Off()
{
	m_NumbersObj[EnumOrder::FIRST]->Update_Off();
	m_NumbersObj[EnumOrder::SECOND]->Update_Off();
	m_NumbersObj[EnumOrder::THIRD]->Update_Off();
}

void ShopBuyItemNumber::NumberUpdate_On()
{
	m_NumbersObj[EnumOrder::FIRST]->Update_On();
	m_NumbersObj[EnumOrder::SECOND]->Update_On();
	m_NumbersObj[EnumOrder::THIRD]->Update_On();
}