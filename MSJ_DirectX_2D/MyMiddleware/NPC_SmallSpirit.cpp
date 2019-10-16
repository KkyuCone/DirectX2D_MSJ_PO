#include "NPC_SmallSpirit.h"



NPC_SmallSpirit::NPC_SmallSpirit()
	: OriginScale(Vector4::One)
{
}


NPC_SmallSpirit::~NPC_SmallSpirit()
{
}


bool NPC_SmallSpirit::Init()
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

	m_Animation->Animation_Create(L"Stand", 0, 11, true, 0.1f);
	m_Animation->Animation_Create(L"Move", 12, 23, true, 0.1f);
	m_Animation->Animation_Create(L"Cry", 24, 29, true, 0.1f);
	m_Animation->Animation_Create(L"Boom", 30, 47, true, 0.1f);

	m_Animation->Animation_Change(L"Stand");

	// Å©±â
	OriginScale = GetTransform()->Local_Scale();

	return true;
}

void NPC_SmallSpirit::Update()
{

}

void NPC_SmallSpirit::FinalUpdate()
{

}

void NPC_SmallSpirit::SetDir(std::wstring _Dir)
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