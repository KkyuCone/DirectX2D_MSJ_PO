#pragma once
#include "RenderOrder.h"
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>


class SPoisonMist : public Script
{	
private:
	bool m_bCreatePC;
private:
	My_PTR<SpriteRenderer2D> m_SPRenderer;
	My_PTR<Animation2D> m_SPAnim;

private:
	void SPCreateEffectCheck();
	void SPDeath();

	void CreatePoisonCloud(std::wstring _TexName, std::wstring _SCount, int _PoisonCount, RenderOrder::RENDER_ORDER _RenderOrder , Vector4 _Pos, Vector4 _Scale);
	void CreatePCManager();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	SPoisonMist();
	~SPoisonMist();
};

