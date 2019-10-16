#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class MonsterDamageSkin : public Script
{
public:
	void Create_DamageSkin(int _Damage);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	MonsterDamageSkin();
	~MonsterDamageSkin();
};

