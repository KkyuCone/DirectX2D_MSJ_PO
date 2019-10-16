#include "stdafx.h"
#include "ToolSceneScript.h"
#include <GameDebug.h>


ToolSceneScript::ToolSceneScript()
{
}


ToolSceneScript::~ToolSceneScript()
{
}

bool ToolSceneScript::Loading()
{
	My_PTR<GameObject> NewGameObj = GetScene()->GameObject_Create();

	My_PTR<Camera> CamPTR = NewGameObj->Component_Add<Camera>(0);
	CamPTR->SettingViewGroup(0, 1, 2, 3, 4, 5);
	CamPTR->GetTransform()->Local_Position(Vector4(0.0f, 0.04f, -100.0f));

	return true;
}


void ToolSceneScript::DebugRender()
{
	//GameDebug::DrawStr(L"¤·¤µ¤· ¸Þ·Õ¸Þ·Õ ³ÄÇÏÇÏÇÏÇÖ", 40, { 10.0f, 40.0f }, { 1.0f, 1.0f, 1.0f });
}