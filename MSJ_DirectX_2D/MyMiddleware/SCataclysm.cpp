#include "SCataclysm.h"
#include <Scene.h>



SCataclysm::SCataclysm()
{
}


SCataclysm::~SCataclysm()
{
}

//

bool SCataclysm::Init()
{
	m_CataclysmRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_CataclysmRenderer)
	{
		EAssert(true);
		return false;
	}

	m_CataclysmAnim = GetComponent<Animation2D>();

	if (nullptr == m_CataclysmAnim)
	{
		EAssert(true);
		return false;
	}

	m_CataclysmAnim->Animation_Create(L"CataEffect", 0, 15, false, 0.1f);
	m_CataclysmAnim->Animation_Change(L"CataEffect");

	return true;
}

void SCataclysm::Update()
{
	IsEndAnimation();
}

void SCataclysm::FinalUpdate()
{

}

//
void SCataclysm::IsEndAnimation()
{
	if (true == m_CataclysmAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}