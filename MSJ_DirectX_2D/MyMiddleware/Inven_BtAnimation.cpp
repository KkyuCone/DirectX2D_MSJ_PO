#include "Inven_BtAnimation.h"
#include <GameObject.h>


Inven_BtAnimation::Inven_BtAnimation() 
	: m_CurPressedTime(0.0f)
	, m_bPressed(false)
{
}


Inven_BtAnimation::~Inven_BtAnimation()
{
}

//

bool Inven_BtAnimation::Init()
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

	m_Animation->Animation_Create(L"Disabled", 0, 0, true, 0.1f);
	m_Animation->Animation_Create(L"MouseOver", 1, 1, true, 0.1f);
	m_Animation->Animation_Create(L"Normal", 2, 2, true, 0.1f);
	m_Animation->Animation_Create(L"Pressed", 3, 3, true, 0.1f);
	m_Animation->Animation_Change(L"Normal");

	return true;
}

void Inven_BtAnimation::Update()
{
	PressedAniCheck();
}

void Inven_BtAnimation::FinalUpdate()
{

}

//
void Inven_BtAnimation::PressedAniCheck()
{
	if (false == m_bPressed)
		return;

	if (0.2f <= m_CurPressedTime)
	{
		m_CurPressedTime = 0.0f;
		m_bPressed = false;
		SetAnimation(L"Normal");
	}


	m_CurPressedTime += Time::DeltaTime();
}


// 애니메이션 체인지
void Inven_BtAnimation::SetAnimation(std::wstring _Name)
{
	if (L"Disabled" == _Name 
		|| L"MouseOver" == _Name
		|| L"Normal" == _Name
		|| L"Pressed" == _Name)
	{
		if (L"Pressed" == _Name)
		{
			m_bPressed = true;
		}

		m_Animation->Animation_Change(_Name.c_str());
	}
	else
	{
		EAssert(true);
		return;
	}

}
