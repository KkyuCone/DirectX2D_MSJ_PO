#include "Inven_MouseItem.h"
#include <GameObject.h>
#include <Camera.h>


Inven_MouseItem::Inven_MouseItem() : m_Renderer(nullptr), m_ItemCodeName(ItemCodeName::EMPTY), m_Camera(nullptr)
{
}


Inven_MouseItem::~Inven_MouseItem()
{
}

bool Inven_MouseItem::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	return true;
}

void Inven_MouseItem::Update()
{
	Move();
}

void Inven_MouseItem::FinalUpdate()
{
}

//

void Inven_MouseItem::Move()
{
	if (nullptr == m_Camera)
	{
		return;
	}

	Vector2 CurMousePos = m_Camera->WMousePosition();

	Vector4 ChangeMousePos = GetTransform()->Local_Position();
	ChangeMousePos.x = CurMousePos.x - 7.0f;
	ChangeMousePos.y = CurMousePos.y + 4.0f;

	GetGameObject()->GetTransform()->Local_Position(ChangeMousePos);
}

void Inven_MouseItem::SetClickItem(ItemCodeName _Code)
{
	switch (_Code)
	{
	case ItemCode::EMPTY:
		m_Renderer->SetSprite(L"EmptyItem.png", 0);
		break;
	case ItemCode::POTION_RED:
		m_Renderer->SetSprite(L"RedPoiton.png", 0);
		break;
	case ItemCode::POTION_BLUE:
		m_Renderer->SetSprite(L"BluePoiton.png", 0);
		break;
	case ItemCode::POTION_WHITE:
		m_Renderer->SetSprite(L"WhitePotion.png", 0);
		break;
	case ItemCode::POTION_ORANGE:
		m_Renderer->SetSprite(L"OrangePotion.png", 0);
		break;
	case ItemCode::POTION_ELIXIR:
		m_Renderer->SetSprite(L"02000006.info.icon.png", 0);
		break;
	case ItemCode::SKILL_BE:
		m_Renderer->SetSprite(L"BlazingExtinction_Icon.png", 0);
		break;
	case ItemCode::SKILL_BZ:
		m_Renderer->SetSprite(L"Blizzard_Icon.png", 0);
		break;
	case ItemCode::SKILL_CC:
		m_Renderer->SetSprite(L"Cataclysm_Icon.png", 0);
		break;
	case ItemCode::SKILL_PM:
		m_Renderer->SetSprite(L"PoisonMist_Icon.png", 0);
		break;
	case ItemCode::SKILL_SP:
		m_Renderer->SetSprite(L"SoulPenetration_Icon.png", 0);
		break;
	default:
		break;
	}
}

void Inven_MouseItem::SetMouseCamera(Camera* _Mouse)
{
	m_Camera = _Mouse;
}