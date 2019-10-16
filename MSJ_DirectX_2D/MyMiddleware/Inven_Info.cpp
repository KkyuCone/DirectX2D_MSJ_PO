#include "Inven_Info.h"
#include <Sprite.h>
#include <Texture.h>
#include <Scene.h>
#include <SubTransform.h>
#include "RenderOrder.h"

Inven_Info::Inven_Info() 
	: m_Info(nullptr), m_ItemCodeName(ItemCodeName::EMPTY), m_bItemEmpty(true)
	, m_HundredNumber(nullptr), m_TensNumber(nullptr), m_UnitNumber(nullptr)
{
}


Inven_Info::~Inven_Info()
{
	if (nullptr != m_Info)
	{
		delete m_Info;
	}
}

//

bool Inven_Info::Init()
{
	m_Info = new ItemInfo();
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	CreateItemCountTexture();
	m_Info->Count = 0;

	return true;
}

void Inven_Info::Update()
{
	if (nullptr == m_Info)
		return;

	NumberMove();


}

void Inven_Info::FinalUpdate()
{

}

//
void Inven_Info::Number_On()
{
	HundredNumberRenderer->Update_On();
	TensNumberRenderer->Update_On();
	UnitNumberRenderer->Update_On();
}

void Inven_Info::Number_Off()
{
	HundredNumberRenderer->Update_Off();
	TensNumberRenderer->Update_Off();
	UnitNumberRenderer->Update_Off();
}



void Inven_Info::ChangeItemTexture(std::wstring _Name)
{
	// 아이템에 따라 이미지 바꿔주기
	if (L"RedPotion" == _Name)
	{
		// 빨간포션
		m_Renderer->SetSprite(L"RedPoiton.png", 0);
		m_Info->ItemName = L"RedPoiton.png";
	}
	else if (L"OrangePotion" == _Name)
	{
		// 주황포션
		m_Renderer->SetSprite(L"OrangePotion.png", 0);
		m_Info->ItemName = L"OrangePotion.png";
	}
	else if (L"WhitePotion" == _Name)
	{
		// 하얀포션
		m_Renderer->SetSprite(L"WhitePotion.png", 0);
		m_Info->ItemName = L"WhitePotion.png";
	}
	else
	{
		// 빈 이미지
		m_Renderer->SetSprite(L"EmptyItem.png", 0);
		m_Info->ItemName = L"EmptyItem.png";
	}

	size_t NewWidth = m_Renderer->GetSprite()->GetTexture()->Width();
	size_t NewHeight = m_Renderer->GetSprite()->GetTexture()->Height();

	Vector4 CurSize = GetGameObject()->GetTransform()->Local_Scale();
	CurSize.x = (float)NewWidth;
	CurSize.y = (float)NewHeight;

	CurSize.x = 25.0f;
	CurSize.y = 25.0f;

	GetTransform()->Local_Scale(CurSize);
}


// Get,Set
ItemInfo* Inven_Info::GetInfo()
{
	return m_Info;
}

void Inven_Info::SetInfo(ItemKind::ITEM_KIND _Kind, std::wstring _Name, int _Count, bool _bCount)
{
	if (nullptr == m_Info)
		return;

	m_Info->ItemKind = _Kind;
	m_Info->ItemName = _Name;
	m_Info->Count = _Count;
	m_Info->m_bCount = _bCount;

	ChangeItemTexture(m_Info->ItemName);

	m_bItemEmpty = true;


	// 소비, 기타템은 따로 분류해주기 (개수 세야함)
}

void Inven_Info::SetItemKind(ItemKind::ITEM_KIND _Kind)
{
	m_Info->ItemKind = _Kind;

	if (_Kind == ItemKind::ITEM_KIND::CONSUME 
		|| _Kind == ItemKind::ITEM_KIND::ETC)
	{
		m_Info->m_bCount = true;
	}
	else
	{
		m_Info->m_bCount = false;
	}
}

void Inven_Info::SetItemCount(int _Count)
{
	m_Info->Count = _Count;

	if (m_Info->ItemKind == ItemKind::ITEM_KIND::CONSUME || m_Info->ItemKind == ItemKind::ITEM_KIND::ETC)
	{
		// 개수를 표시할 수 있는 아이템 종류라면
		// 해당 렌더러 Open 
		// 총 개수는 999까지만.. 가능
		SetItemCountTexture(_Count);
	}
}

int Inven_Info::GetItemCount()
{
	return m_Info->Count;
}

void Inven_Info::CreateItemCountTexture()
{
	// 일단 나의 현재 위치도 알아야한다

	Vector4 CurSlotPos = GetGameObject()->GetTransform()->Local_Position();
	CurSlotPos.z -= 1.0f;
	CurSlotPos.y -= 10.0f;
	CurSlotPos.x -= 5.0f;

	m_HundredNumber = GetScene()->GameObject_Create(L"ItemNumber_Hundred");
	m_HundredNumber->GetTransform()->Local_Position(CurSlotPos);
	m_HundredNumber->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	HundredNumberRenderer = m_HundredNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	HundredNumberRenderer->SetSprite(L"EmptyItem.png", 0);


	CurSlotPos.x += 7.0f;

	m_TensNumber = GetScene()->GameObject_Create(L"ItemNumber_Tens");
	m_TensNumber->GetTransform()->Local_Position(CurSlotPos);
	m_TensNumber->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	TensNumberRenderer = m_TensNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	TensNumberRenderer->SetSprite(L"EmptyItem.png", 0);

	CurSlotPos.x += 7.0f;

	m_UnitNumber = GetScene()->GameObject_Create(L"ItemNumber_Unit");
	m_UnitNumber->GetTransform()->Local_Position(CurSlotPos);
	m_UnitNumber->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	UnitNumberRenderer = m_UnitNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	UnitNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	
}

void Inven_Info::RenderOff()
{
	m_Renderer->Update_Off();
}

void Inven_Info::RenderOn()
{
	m_Renderer->Update_On();
}

void Inven_Info::NumberMove()
{
	Vector4 CurSlotPos = GetGameObject()->GetTransform()->World_Position();
	CurSlotPos.z -= 1.0f;
	CurSlotPos.y -= 10.0f;
	CurSlotPos.x -= 5.0f;

	m_HundredNumber->GetTransform()->World_Position(CurSlotPos);


	CurSlotPos.x += 7.0f;

	m_TensNumber->GetTransform()->World_Position(CurSlotPos);

	CurSlotPos.x += 7.0f;

	m_UnitNumber->GetTransform()->World_Position(CurSlotPos);
}

void Inven_Info::SetItemCountTexture(int _Value)
{
	int FirstNumber = _Value / 100;												// 100의 자리
	int SecondNumber = (_Value - (FirstNumber * 100)) / 10;						// 10의 자리
	int ThirdNumber = (_Value - (FirstNumber * 100) - (SecondNumber * 10));		// 1의자리

	if (0 == FirstNumber)
	{
		HundredNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		HundredNumberRenderer->SetSprite(L"UI_StateNumber.png", FirstNumber);
	}

	if (0 == FirstNumber && 0 == SecondNumber)
	{
		TensNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		TensNumberRenderer->SetSprite(L"UI_StateNumber.png", SecondNumber);
	}

	if (0 == FirstNumber && 0 == SecondNumber && 0 == ThirdNumber)
	{
		UnitNumberRenderer->SetSprite(L"EmptyItem.png", 0);
		m_Info->ItemName = L"EmptyItem.png";
		m_Info->Count = 0;
	}
	else
	{
		UnitNumberRenderer->SetSprite(L"UI_StateNumber.png", ThirdNumber);
	}

}

void Inven_Info::SetEmptyTexture()
{
	HundredNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	TensNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	UnitNumberRenderer->SetSprite(L"EmptyItem.png", 0);
}

void Inven_Info::ItemAdd(ItemCode::ItemCodeName _Code)
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if ( nullptr == m_Renderer)
	{
		int  a = 0;
	}

	m_ItemCodeName = _Code;

	// 이미지 바꾸기
	switch (_Code)
	{
	case ItemCode::EMPTY:
		m_Renderer->SetSprite(L"EmptyItem.png", 0);
		m_Info->ItemName = L"EmptyItem.png";
		break;
	case ItemCode::POTION_RED:
		m_Renderer->SetSprite(L"RedPoiton.png", 0);
		m_Info->ItemName = L"RedPoiton.png";
		break;
	case ItemCode::POTION_BLUE:
		m_Renderer->SetSprite(L"BluePoiton.png", 0);
		m_Info->ItemName = L"BluePoiton.png";
		break;
	case ItemCode::POTION_WHITE:
		m_Renderer->SetSprite(L"WhitePotion.png", 0);
		m_Info->ItemName = L"WhitePotion.png";
		break;
	case ItemCode::POTION_ORANGE:
		m_Renderer->SetSprite(L"OrangePotion.png", 0);
		m_Info->ItemName = L"OrangePotion.png";
		break;
	case ItemCode::POTION_ELIXIR:
		m_Renderer->SetSprite(L"02000006.info.icon.png", 0);
		m_Info->ItemName = L"02000006.info.icon.png";
		break;
	default:
		break;
	}

	// 이미지 종류 설정
	switch (_Code)
	{
	case ItemCode::POTION_RED:
	case ItemCode::POTION_BLUE:
	case ItemCode::POTION_WHITE:
	case ItemCode::POTION_ORANGE:
		SetItemKind(ItemKind::ITEM_KIND::CONSUME);
		break;
	default:
		break;
	}

	if (nullptr == m_Renderer)
	{
		int a = 0;
	}

	size_t NewWidth = m_Renderer->GetSprite()->GetTexture()->Width();
	size_t NewHeight = m_Renderer->GetSprite()->GetTexture()->Height();

	Vector4 CurSize = GetGameObject()->GetTransform()->Local_Scale();
	CurSize.x = (float)NewWidth;
	CurSize.y = (float)NewHeight;

	GetTransform()->Local_Scale(CurSize);

}
