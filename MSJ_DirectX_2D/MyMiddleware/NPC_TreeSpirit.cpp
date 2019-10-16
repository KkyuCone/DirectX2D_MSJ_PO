#include "NPC_TreeSpirit.h"



NPC_TreeSpirit::NPC_TreeSpirit()
	: OriginScale(Vector4::One)
{
}


NPC_TreeSpirit::~NPC_TreeSpirit()
{
}

//

bool NPC_TreeSpirit::Init()
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

	m_Animation->Animation_Create(L"Stand", 0, 7, true, 0.1f);
	m_Animation->Animation_Change(L"Stand");

	// Å©±â
	OriginScale = GetTransform()->Local_Scale();

	return true;
}

void NPC_TreeSpirit::Update()
{

}

void NPC_TreeSpirit::FinalUpdate()
{

}

void NPC_TreeSpirit::SetDir(std::wstring _Dir)
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
