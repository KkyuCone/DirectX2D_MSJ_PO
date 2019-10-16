#pragma once
#include "NPCState.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class NPC_RockSpirit : public Script, public NPCState
{
private:
	Vector4 OriginScale;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	void SetDir(std::wstring _Dir);
	void ChangeAnimation(std::wstring _AniName);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	NPC_RockSpirit();
	~NPC_RockSpirit();
};

