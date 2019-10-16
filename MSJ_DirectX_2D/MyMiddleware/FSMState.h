#pragma once
class FSMState
{
public:
	enum FSM_STATE
	{
		IDLE = 0,
		RUN,
		JUMP_UP,
		JUMP_DOWN,
		DOWN,
		DOWN_ATTACK,
		ROPE_IDLE,
		ROPE_UP,
		ROPE_DOWN,
		LADDER_IDLE,
		LADDER_UP,
		LADDER_DOWN,
		ATTACK_BASE,
		ATTACK_SKILL_01,
		ATTACK_SKILL_02,
		ATTACK_SKILL_03,
		DEATH,
		DAMAGE,
		STATE_MAX
	};

public:
	FSMState() {};
	~FSMState() {};
};

