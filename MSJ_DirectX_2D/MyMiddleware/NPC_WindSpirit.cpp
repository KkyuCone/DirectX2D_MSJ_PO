#include "NPC_WindSpirit.h"
#include <GameObject.h>


NPC_WindSpirit::NPC_WindSpirit() : m_Renderer(nullptr), m_Animation(nullptr)
{
}


NPC_WindSpirit::~NPC_WindSpirit()
{
}

//

bool NPC_WindSpirit::Init()
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

	m_Animation->Animation_Create(L"Stand", 0, 15, true, 0.1f);
	m_Animation->Animation_Create(L"Move", 16, 31, true, 0.1f);
	m_Animation->Animation_Create(L"Wind", 32, 47, true, 0.1f);

	m_Animation->Animation_Change(L"Stand");

	return true;
}

void NPC_WindSpirit::Update()
{

}

void NPC_WindSpirit::FinalUpdate()
{

}


//

void NPC_WindSpirit::SetDir(std::wstring _Dir)
{
	if (L"Left" == _Dir)
	{
		Vector4 ChangeScale = Vector4(199.0f, 177.0f, 1.0f, 1.0f);
		GetGameObject()->GetTransform()->Local_Scale(ChangeScale);
	}
	else
	{
		Vector4 ChangeScale = Vector4(-199.0f, 177.0f, 1.0f, 1.0f);
		GetGameObject()->GetTransform()->Local_Scale(ChangeScale);
	}
}