#pragma once

class ItemCode
{
public:
	enum ItemCodeName
	{
		EMPTY = 0,
		POTION_RED,
		POTION_BLUE,
		POTION_WHITE,
		POTION_ORANGE,
		POTION_ELIXIR,
		SKILL_BE,			// 블레이징 익스팅션
		SKILL_PM,			// 포이즌 미스트
		SKILL_SP,			// 소울퍼네트레이션
		SKILL_CC,			// 카타클리즘
		SKILL_BZ,			// 블리자드
	};

public:
	ItemCode() {}
	~ItemCode() {}
};