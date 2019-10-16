#include "Velaris.h"

void Velaris::SetPlayer(GameObject* _Player)
{
	if (nullptr == _Player)
		return;

	m_pPlayer = _Player;
}

void Velaris::SetFloor(GameObject* _Floor)
{
	if (nullptr == _Floor)
		return;

	m_pFloor = _Floor;
}

void Velaris::SetBackground(Vector4 _Size)
{
	BackgroundSize = _Size;
}

void Velaris::SetUpTilePos(bool _Value)
{
	m_bUpTile = _Value;
}

void Velaris::SetHPBar(GeneralMonster_HP* _Script)
{
	m_HPBarScript = _Script;
}