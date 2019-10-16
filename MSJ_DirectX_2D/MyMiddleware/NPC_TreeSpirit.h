#pragma once
#include <Script.h>
#include "NPCState.h"

#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class NPC_TreeSpirit : public NPCState, public Script
{
private:
	Vector4 OriginScale;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	void SetDir(std::wstring _Dir);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;


public:
	NPC_TreeSpirit();
	~NPC_TreeSpirit();
};

