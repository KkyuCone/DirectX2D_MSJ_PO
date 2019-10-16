#include "NPC_RockSpirit.h"



NPC_RockSpirit::NPC_RockSpirit()
	: OriginScale(Vector4::One)
{
}


NPC_RockSpirit::~NPC_RockSpirit()
{
}

//

bool NPC_RockSpirit::Init()
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

	m_Animation->Animation_Create(L"Stand", 0, 5, true, 0.1f);
	m_Animation->Animation_Create(L"Move", 6, 11, true, 0.1f);
	m_Animation->Animation_Create(L"Stand2", 12, 17, true, 0.1f);
	m_Animation->Animation_Create(L"Free", 18, 25, true, 0.1f);

	m_Animation->Animation_Change(L"Stand2");

	// Å©±â
	OriginScale = GetTransform()->Local_Scale();

	return true;
}

void NPC_RockSpirit::Update()
{
	if (L"Free" == m_Animation->CurAnimationName())
	{
		if (true == m_Animation->IsEndAnimation())
		{
			m_Animation->Animation_Change(L"Stand");
		}
	}
}

void NPC_RockSpirit::FinalUpdate()
{

}


//

void NPC_RockSpirit::SetDir(std::wstring _Dir)
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

void NPC_RockSpirit::ChangeAnimation(std::wstring _AniName)
{
	m_Animation->Animation_Change(_AniName.c_str());
}