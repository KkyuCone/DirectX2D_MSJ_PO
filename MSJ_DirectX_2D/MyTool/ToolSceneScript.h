#pragma once
#include <Scene.h>

class ToolSceneScript : public Scene::SceneScript
{
private:
	void DebugRender() override;

public:
	bool Loading() override;

public:
	ToolSceneScript();
	~ToolSceneScript();
};

