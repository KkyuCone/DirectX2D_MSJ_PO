#include "Pinkbin.h"

void Pinkbin::SetPlayer(GameObject* _Player)
{
	if (nullptr == _Player)
		return;

	m_pPlayer = _Player;
}

void Pinkbin::SetFloor(GameObject* _Floor)
{
	if (nullptr == _Floor)
		return;

	m_pFloor = _Floor;
}

void Pinkbin::SetBackground(Vector4 _Size)
{
	BackgroundSize = _Size;
}

void Pinkbin::SetUpTilePos(bool _Value)
{
	m_bUpTile = _Value;
}

void Pinkbin::SetHPScript(BossMonster_HP* _Value)
{
	m_HPScript = _Value;
}