#include "NPC_NONE.h"



NPC_NONE::NPC_NONE()
	: OriginScale(Vector4::One)
{
}


NPC_NONE::~NPC_NONE()
{
}

//

bool NPC_NONE::Init()
{
	return true;
}

void NPC_NONE::Update()
{
	if (nullptr == m_Animation)
		return;
}

void NPC_NONE::FinalUpdate()
{

}

//
void NPC_NONE::Set_Kind(std::wstring _Name)
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return;
	}

	if (_Name == L"Wind_1")
	{
		m_Animation->Animation_Create(L"NPC", 0, 22, true, 0.1f);
	}
	else if (_Name == L"Wind_2")
	{
		m_Animation->Animation_Create(L"NPC", 0, 12, true, 0.1f);
	}
	else if (_Name == L"Rock_1")
	{
		m_Animation->Animation_Create(L"NPC", 0, 28, true, 0.1f);
	}
	else if (_Name == L"Rock_2")
	{
		m_Animation->Animation_Create(L"NPC", 0, 13, true, 0.1f);
	}
	else if (_Name == L"Tree_1")
	{
		m_Animation->Animation_Create(L"NPC", 0, 14, true, 0.1f);
	}
	else if (_Name == L"Tree_2")
	{
		m_Animation->Animation_Create(L"NPC", 0, 11, true, 0.1f);
	}
	else
	{
		GetGameObject()->Death();
	}

	m_Animation->Animation_Change(L"NPC");
}

void NPC_NONE::SetDir(std::wstring _Dir)
{
	Vector4 ChangeScale = OriginScale;

	if (L"Left" == _Dir)
	{
		GetGameObject()->GetTransform()->Local_Scale(ChangeScale);
	}
	else
	{
		ChangeScale.x *= -1.0f;
		GetGameObject()->GetTransform()->Local_Scale(ChangeScale);
	}
}