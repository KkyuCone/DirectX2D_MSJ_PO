#include "CoreBuilder.h"
#include "TestGameScene.h"
#include "TextureLoadingScene.h"
#include "TownScene.h"
#include "BossMonsterStageScene.h"
#include "MonsterStageScene1.h"
#include "MonsterStageScene2.h"
#include "TitleScene.h"

#include <ReferenceBase.h>
#include <TypeBase.h>
#include <GameObject.h>
#include <Window.h>

#include "MonsterCreator.h"
#include <MiddleStart.h>

class TEST : public ReferenceBase
{
public:
	int a;
	int b;

	My_PTR<TEST> m_Ptr;
};


CoreBuilder::CoreBuilder()
	: pTestScene(nullptr), pTextureLoadingScene(nullptr), pTownScene(nullptr), pBossScene(nullptr), pChewChewIslandScene(nullptr), pEsferaScene(nullptr)
{
}


CoreBuilder::~CoreBuilder()
{
}


bool CoreBuilder::Build()
{
	MiddleStart::Init();

	pTextureLoadingScene = Scene::Scene_Create<TextureLoadingScene>(L"ImageLoadingScene");
	pTestScene = Scene::Scene_Create<TestGameScene>(L"TestScene");
	pTownScene = Scene::Scene_Create<TownScene>(L"ArcanaTownScene");
	pBossScene = Scene::Scene_Create<BossMonsterStageScene>(L"PinkbinScene");
	pTitleScene = Scene::Scene_Create<TitleScene>(L"TitleScene");
	pChewChewIslandScene = Scene::Scene_Create<MonsterStageScene1>(L"ChewChewIslandScene");
	pEsferaScene = Scene::Scene_Create<MonsterStageScene2>(L"EsferaScene");
	Scene::Scene_Change(L"ImageLoadingScene");


#pragma region Etc

	//MonsterDataInit MonData;
	//MonData.MonsterMaxCount = 1000;
	//MonData.MonsterGenTime = 0.1f;

	//pScene->Updater_Create<MonsterCreator>(MonData);


	/*My_PTR<Scene> pScene = Scene::Scene_Create(L"TestSece");
	Scene::Scene_Change(L"TestSece");

	pScene->GameObject_Create(L"Camera");*/

	//  순환참조 (문제점)
	//TEST* p = nullptr;
	//My_PTR<TEST> m_Ptr1 = nullptr;
	//My_PTR<TEST> m_Ptr2 = new TEST();

	//m_Ptr2->m_Ptr = m_Ptr1;
	//m_Ptr1->m_Ptr = m_Ptr2;

	//My_PTR<Object> NewObj = new Object();
	//NewObj->RTTISetting();

#pragma endregion
	return true;
}
