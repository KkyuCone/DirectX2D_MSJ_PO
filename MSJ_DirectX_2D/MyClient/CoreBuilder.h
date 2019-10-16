#pragma once
#include <Core.h>
#include <Scene.h>

class CoreBuilder : public Core::My_CoreBuilder
{
private:
	My_PTR<Scene> pTestScene;
	My_PTR<Scene> pTextureLoadingScene;
	My_PTR<Scene> pTownScene;
	My_PTR<Scene> pBossScene;
	My_PTR<Scene> pChewChewIslandScene;
	My_PTR<Scene> pEsferaScene;
	My_PTR<Scene> pTitleScene;

public:
	bool Build() override;

public:
	CoreBuilder();
	~CoreBuilder();
};

MYCORESTART(CoreBuilder, L"YomiWindow", Vector2_Int(1024, 768));


