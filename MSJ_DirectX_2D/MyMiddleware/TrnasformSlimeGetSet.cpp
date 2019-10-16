#include "TransformSlime.h"

void TransformSlime::SetPlayer(GameObject* _Player)
{
	if (nullptr == _Player)
		return;

	m_pPlayer = _Player;
}

void TransformSlime::SetFloor(GameObject* _Floor)
{
	if (nullptr == _Floor)
		return;

	m_pFloor = _Floor;
}

void TransformSlime::SetBackground(Vector4 _Size)
{
	BackgroundSize = _Size;
}

void TransformSlime::SetUpTilePos(bool _Value)
{
	m_bUpTile = _Value;
}

void TransformSlime::SetHPBar(GeneralMonster_HP* _Script)
{
	m_HPBarScript = _Script;
}