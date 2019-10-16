#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class Inven_BtKind : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

private:
	void UpdateCheck();

public:
	void SetAnimation(std::wstring _Name);
	void Off();
	void On();
	bool IsOn();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Inven_BtKind();
	~Inven_BtKind();
};

