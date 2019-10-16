#include "ShopBuyItemBt.h"



ShopBuyItemBt::ShopBuyItemBt()
{
}


ShopBuyItemBt::~ShopBuyItemBt()
{
}

//
bool ShopBuyItemBt::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"ClickNone", 0, 0, true, 0.1f);
	m_Animation->Animation_Create(L"ClickPressed", 1, 1, true, 0.1f);
	m_Animation->Animation_Change(L"ClickNone");

	return true;
}

void ShopBuyItemBt::Update()
{

}

void ShopBuyItemBt::FinalUpdate()
{

}

//

void ShopBuyItemBt::SetSprite(std::wstring _TextureName)
{
	if (L"ClickNone" == m_Animation->CurAnimationName())
	{
		m_Renderer->SetSprite(_TextureName.c_str(), 0);
	}
	else
	{
		m_Renderer->SetSprite(_TextureName.c_str(), 1);
	}
}

void ShopBuyItemBt::SetAnimation(std::wstring _AniName)
{
	if (_AniName == m_Animation->CurAnimationName())
	{
		return;
	}
	m_Animation->Animation_Change(_AniName.c_str());
}