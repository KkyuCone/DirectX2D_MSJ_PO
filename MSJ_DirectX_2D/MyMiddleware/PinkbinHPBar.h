#pragma once
#include <Script.h>

#define BOSSPINKBINHP 40000
#define BOSSPINKBINHALFHP 20000

class BossMonster_HP;
class PinkbinHPBar : public Script
{
private:
	int m_HP;
	int m_FirstHP;
	int m_SecondHP;

private:
	BossMonster_HP* m_FirstLineScript;
	BossMonster_HP* m_SecondLindScript;

public:
	void Damage(int _Value);

public:
	void SetFirstLineScript(BossMonster_HP* _Script);
	void SetSecondLineScript(BossMonster_HP* _Script);

	int GetHP();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	PinkbinHPBar();
	~PinkbinHPBar();
};

