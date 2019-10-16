#pragma once

class NPCState
{
public:
	enum NPC_STATE
	{
		STAND = 0,
		MOVE,
		CRY,
		WIND,
		FREE
	};

public:
	NPCState() {};
	~NPCState() {};
};
