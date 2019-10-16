#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class CataclysmScene : public Script
{
private:
	static int CataAnimCount;

private:
	bool m_bEndScene;

private:
	My_PTR<SpriteRenderer2D> m_CataSceneRenderer;
	My_PTR<Animation2D> m_CataSceneAnim;

public:
	void CreateAnimation(int _Count);
	void CheckEndAnimation();
	void CataDeath();
	
public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	CataclysmScene();
	~CataclysmScene();
};

