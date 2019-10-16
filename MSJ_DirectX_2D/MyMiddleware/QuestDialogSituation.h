#pragma once

class QuestDialogSituation
{
public:
	enum Dialog_Situation
	{
		DIALOG_ROCK_BEFORE = 0,					// 돌의 정령 퀘스트 받기전
		DIALOG_ROCK_AFTER,						// 돌의 정령 퀘스트 완료후
		DIALOG_ROCK_QING,						// 돌의 정령 퀘스트 진행중
		DIALOG_ROCK_QDEFUSAL,					// 돌의 정령 퀘스트 거절
		DILAOG_TREE_BEFORE,						// 나무의 정령대화
		DIALOG_WIND_BEFORE,						// 바람의 정령대화 + 조그만 정령
		DIALOG_NONE,
	};

	enum Dialog_NPC
	{
		NPC_ROCK = 0,
		NPC_SMALL,
		NPC_WIND,
		NPC_TREE,
		NPC_NONE,
	};

public:
	QuestDialogSituation() {}
	~QuestDialogSituation() {}
};
