#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class MonsterHitScript : public Script
{
private:
	bool m_bAniCreate;
private:
	Vector4 m_MovePos;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	void SetSkillName(std::wstring _Name);
	void SetMovePos(Vector4 _Pos);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	MonsterHitScript();
	~MonsterHitScript();
};

