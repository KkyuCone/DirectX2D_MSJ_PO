#include "Inven_BtKind.h"
#include <GameObject.h>


Inven_BtKind::Inven_BtKind()
{
}


Inven_BtKind::~Inven_BtKind()
{
}


//

bool Inven_BtKind::Init()
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
	m_Animation->Animation_Create(L"Enabled", 1, 1, true, 0.1f);
	m_Animation->Animation_Change(L"Disabled");

	return true;
}

void Inven_BtKind::Update()
{
	UpdateCheck();
}

void Inven_BtKind::FinalUpdate()
{

}

//

void Inven_BtKind::SetAnimation(std::wstring _Name)
{
	if (L"Disabled" == _Name
		|| L"Enabled" == _Name)
	{
		m_Animation->Animation_Change(_Name.c_str());
	}
	else
	{
		EAssert(true);
		return;
	}
}

void Inven_BtKind::Off()
{
	m_Animation->Animation_Change(L"Disabled");
}

void Inven_BtKind::On()
{
	m_Animation->Animation_Change(L"Enabled");
}

bool Inven_BtKind::IsOn()
{
	if (true == m_Animation->IsCurAnimation(L"Enabled"))
	{
		return true;
	}
	return false;
}


void Inven_BtKind::UpdateCheck()
{
	GameObject* ParentObj = GetGameObject()->GetParent();

	if (true == ParentObj->IsUpdate())
	{
		GetGameObject()->Update_On();
	}
	else
	{
		GetGameObject()->Update_Off();
	}
}