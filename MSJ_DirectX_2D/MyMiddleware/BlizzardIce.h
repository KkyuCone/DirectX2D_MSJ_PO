#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class GameObject;
class BlizzardIce : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_IceRenderer;
	My_PTR<Animation2D> m_IceAnim;

private:
	void CheckDeath();

public:
	Animation2D* GetCurAniamation();
	GameObject* GetThisGameObject();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BlizzardIce();
	~BlizzardIce();
};

