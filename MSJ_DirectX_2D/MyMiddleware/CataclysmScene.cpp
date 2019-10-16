#include "CataclysmScene.h"
#include <Scene.h>

int CataclysmScene::CataAnimCount = 1;

CataclysmScene::CataclysmScene() : m_bEndScene(false)
{
}


CataclysmScene::~CataclysmScene()
{
}

//

bool CataclysmScene::Init()
{
	return true;
}

void CataclysmScene::Update()
{
	CheckEndAnimation();
}

void CataclysmScene::FinalUpdate()
{

}

//

void CataclysmScene::CreateAnimation(int _Count)
{
	m_CataSceneRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_CataSceneRenderer)
	{
		EAssert(true);
		return;
	}

	m_CataSceneAnim = GetComponent<Animation2D>();

	if (nullptr == m_CataSceneAnim)
	{
		EAssert(true);
		return;
	}


	std::wstring Count = std::to_wstring(CataAnimCount);
	std::wstring NewAniName = L"CataAni" + Count;

	m_CataSceneAnim->Animation_Create(NewAniName.c_str(), 0, 6, false, 0.1f);
	m_CataSceneAnim->Animation_Change(NewAniName.c_str());
}

void CataclysmScene::CheckEndAnimation()
{
	if (true == m_CataSceneAnim->IsEndAnimation())
	{
		Vector4 CurBasePos = GetTransform()->Local_Position();

		++CataAnimCount;

		if ( 8 < CataAnimCount )
		{
			CataAnimCount = 1;
			CataDeath();
			return;
		}

		// ´Ù¸¥ ¾ÀÀÌÆåÆ® ¸¸µé±â

		std::wstring CataCount = std::to_wstring(CataAnimCount);
		std::wstring SkillName = GetGameObject()->GetName();
		SkillName += L"Scene";
		SkillName += CataCount;

		My_PTR<GameObject> NewCataclysmSceneObj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
		NewCataclysmSceneObj->GetTransform()->Local_Position(Vector4(CurBasePos));
		NewCataclysmSceneObj->GetTransform()->Local_Scale(Vector4(1532.0f, 1100.0f, 1.0f, 1.0f));

		My_PTR<SpriteRenderer2D> NewCataRenderer = NewCataclysmSceneObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
		std::wstring SprietNumber = std::to_wstring(CataAnimCount);
		std::wstring SetSpriteName = L"Cataclysm_Scene0" + SprietNumber;
		SetSpriteName += L".png";
		NewCataRenderer->SetSprite(SetSpriteName.c_str(), 0);
		//NewCataRenderer->SetSprite(L"Cataclysm_Scene02", 0);

		My_PTR<Animation2D> NewCataAnim = NewCataclysmSceneObj->Component_Add<Animation2D>();
		My_PTR<CataclysmScene> NewCataScript = NewCataclysmSceneObj->Component_Add<CataclysmScene>();
		NewCataScript->CreateAnimation(0);

		CataDeath();
	}
}



void CataclysmScene::CataDeath()
{
	GetGameObject()->Death();
}