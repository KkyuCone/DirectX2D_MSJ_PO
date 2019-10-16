#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class ShopBuyItemBt : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	void SetSprite(std::wstring _TextureName);
	void SetAnimation(std::wstring _AniName);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	ShopBuyItemBt();
	~ShopBuyItemBt();
};

