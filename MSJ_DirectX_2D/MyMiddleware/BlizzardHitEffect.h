#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class RectCol2D;
class BlizzardIce;
class BlizzardHitEffect : public Script
{
private:
	int m_BlizzardKind;			// 1이면 높이 있는거, 2는 가까이 있는것
	bool m_bRect;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;
	My_PTR<RectCol2D> m_RectCol;

private:
	BlizzardIce* m_BlizzardAniScript;

private:
	void Create_Collision();

public:
	void SetBlizzardIceAniScript(BlizzardIce* _Script);
	void SetBlizzardKind(int _Value);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BlizzardHitEffect();
	~BlizzardHitEffect();
};

