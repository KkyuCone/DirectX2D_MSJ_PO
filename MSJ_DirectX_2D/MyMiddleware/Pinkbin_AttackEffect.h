#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class RectCol2D;
class Pinkbin_AttackEffect : public Script
{
private:
	std::wstring m_SkillName;
	int m_SkillColFrame;
	bool m_bCol;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;
	My_PTR<RectCol2D> NewCOl;
private:
	void AniEndCheck();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	void SetAttack(std::wstring _Name);

public:
	Pinkbin_AttackEffect();
	~Pinkbin_AttackEffect();
};

