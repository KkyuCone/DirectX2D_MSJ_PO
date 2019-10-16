#pragma once
#include <Scene.h>
#include "TestGameScene.h"

class TextureLoadingScene : public Scene::SceneScript
{
private:
	std::vector<std::wstring> CharacterImages;
	std::vector<std::wstring> MonsterImages;
	std::vector<std::wstring> BKImages;
	std::vector<std::wstring> UIImages;
	std::vector<std::wstring> SkillImage;
	std::vector<std::wstring> NPCImage;

private:
	void TextureLoading();

public:
	bool Loading() override;
	void LateUpdate() override;

public:
	TextureLoadingScene();
	~TextureLoadingScene();
};

