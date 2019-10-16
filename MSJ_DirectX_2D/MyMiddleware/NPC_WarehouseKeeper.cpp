#include "NPC_WarehouseKeeper.h"



NPC_WarehouseKeeper::NPC_WarehouseKeeper()
	: OriginScale(Vector4::One)
{
}


NPC_WarehouseKeeper::~NPC_WarehouseKeeper()
{
}

bool NPC_WarehouseKeeper::Init()
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

	m_Animation->Animation_Change(L"Stand");

	// Å©±â
	OriginScale = GetTransform()->Local_Scale();

	return true;
}

void NPC_WarehouseKeeper::Update()
{

}

void NPC_WarehouseKeeper::FinalUpdate()
{

}


//

void NPC_WarehouseKeeper::SetDir(std::wstring _Dir)
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
