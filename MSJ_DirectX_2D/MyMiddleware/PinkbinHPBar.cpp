#include "PinkbinHPBar.h"
#include "BossMonster_HP.h"


PinkbinHPBar::PinkbinHPBar() : m_FirstLineScript(nullptr), m_SecondLindScript(nullptr), m_HP(BOSSPINKBINHP), m_FirstHP(BOSSPINKBINHALFHP), m_SecondHP(BOSSPINKBINHALFHP)
{
}


PinkbinHPBar::~PinkbinHPBar()
{
}

//

bool PinkbinHPBar::Init()
{
	return true;
}

void PinkbinHPBar::Update()
{

}

void PinkbinHPBar::FinalUpdate()
{

}

//

void PinkbinHPBar::SetFirstLineScript(BossMonster_HP* _Script)
{
	m_FirstLineScript = _Script;
}

void PinkbinHPBar::SetSecondLineScript(BossMonster_HP* _Script)
{
	m_SecondLindScript = _Script;
}

void PinkbinHPBar::Damage(int _Value)
{
	//if (m_HP > 20000)
	//{
	//	m_HP -= _Value;
	//	if (m_HP < 20000)
	//	{
	//		m_FirstLineScript->Damage(_Value);

	//		int aaa = 20000 - _Value;
	//		int bbb = 20000 - aaa;
	//		m_SecondLindScript->Damage(bbb);
	//		return;

	//	}
	//	m_HP += _Value;
	//}


	m_HP -= _Value;

	if(m_HP < _Value && m_HP < 0)
	{
		m_HP = 0;
	}

	// 첫번째라인이랑 두번째라인 비교
	if (m_HP > 20000)
	{
		// 첫번째라인
		m_FirstLineScript->Damage(_Value);
	}
	else
	{
		// 두번째라인
		m_SecondLindScript->Damage(_Value);
	}
}

int PinkbinHPBar::GetHP()
{
	return m_HP;
}