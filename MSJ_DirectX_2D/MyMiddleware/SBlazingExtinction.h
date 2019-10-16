#pragma once
#include "RenderOrder.h"

#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include <Script.h>
#include <GameObject.h>
#include <vector>

#define UTIME 6
#define MOVESPEED 60
#define IMAGEWIDTH 800


class SBlazingExtinction : public Script, public RenderOrder
{
private:
	bool m_bMoveStart;
	float m_CurTime;
	float UseTime;
	float m_MoveSpeed;
	float m_MapWidth;
	float m_BlazeTime;

	int m_CurLaserValue;

	std::wstring m_Dir;

	std::vector<GameObject*> m_MonstersVec;

private:
	My_PTR<SpriteRenderer2D> m_SphereRenderer;
	My_PTR<Animation2D> m_SpAnim;

private:
	void Move();
	void TimeCheck();
	void DirCheck();
	void ChangeRepeat();
	void CreateBlaze();
	void CreateBEEffect();

public:
	void SetDir(std::wstring _Dir);
	void SetMapWidth(float _Width);
	void SetMonsters(std::vector<GameObject*> _Monsters);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	SBlazingExtinction();
	~SBlazingExtinction();
};

