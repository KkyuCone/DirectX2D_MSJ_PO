#include "stdafx.h"
#include "ToolCoreBuilder.h"
#include <Scene.h>
#include <MiddleStart.h>
#include "ToolSceneScript.h"



ToolCoreBuilder::ToolCoreBuilder()
{
}


ToolCoreBuilder::~ToolCoreBuilder()
{
}

bool ToolCoreBuilder::Build()
{
	new int;		// 안전용.. 릭
	//MiddleStart::Init();
	//My_PTR<Scene> ToolScene = Scene::Scene_Create<ToolSceneScript>(L"ToolScene");
	//Scene::Scene_Change(L"ToolScene");

	return true;
}
