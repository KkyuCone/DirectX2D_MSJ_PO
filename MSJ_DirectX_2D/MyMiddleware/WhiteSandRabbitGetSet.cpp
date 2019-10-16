#include "WhiteSandRabbit.h"

// Get

// Set
void WhiteSandRabbit::SetPlayer(GameObject* _Player)
{
	if (nullptr == _Player)
		return;

	m_pPlayer = _Player;
}

void WhiteSandRabbit::SetFloor(GameObject* _Floor)
{
	if (nullptr == _Floor)
		return;

	m_pFloor = _Floor;
}

void WhiteSandRabbit::SetBackground(Vector4 _Size)
{
	BackgroundPos = _Size;
}

void WhiteSandRabbit::SetUpTilePos(bool _Value)
{
	m_bUpTile = _Value;
}

void WhiteSandRabbit::SetHPBar(GeneralMonster_HP* _Script)
{
	m_HPBarScript = _Script;
}

int WhiteSandRabbit::GetHP()
{
	return m_HP;
}