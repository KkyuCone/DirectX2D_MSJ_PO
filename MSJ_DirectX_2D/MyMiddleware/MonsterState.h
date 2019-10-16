#pragma once

#define LIMDISTNACE 500

class MonsterState
{
public:
	enum FSM_MState
	{
		REGEN = 0,
		STAND,
		MOVE,
		HIT,
		DIE,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		ATTACK4,
		ATTACKAFTER1,
		ATTACKAFTER2,
		SKILL_1,
		SKILL_2,
		SKILL_3,
		SKILL_4,
		SKILL_5,
		SKILL_6,
		SKILL_7,
		SKILL_8,
		SKILL_9,
		SKILL_10,
		SKILL_1_AFTER,
		SKILL_2_AFTER,
		SKILL_3_AFTER,
		SKILL_4_AFTER,
	};

public:
	MonsterState() {};
	~MonsterState() {};
};

