#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class NPC_NONE : public Script
{
private:
	Vector4 OriginScale;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	void Set_Kind(std::wstring _Name);
	void SetDir(std::wstring _Dir);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	NPC_NONE();
	~NPC_NONE();
};

