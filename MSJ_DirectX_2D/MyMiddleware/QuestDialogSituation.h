#pragma once

class QuestDialogSituation
{
public:
	enum Dialog_Situation
	{
		DIALOG_ROCK_BEFORE = 0,					// ���� ���� ����Ʈ �ޱ���
		DIALOG_ROCK_AFTER,						// ���� ���� ����Ʈ �Ϸ���
		DIALOG_ROCK_QING,						// ���� ���� ����Ʈ ������
		DIALOG_ROCK_QDEFUSAL,					// ���� ���� ����Ʈ ����
		DILAOG_TREE_BEFORE,						// ������ ���ɴ�ȭ
		DIALOG_WIND_BEFORE,						// �ٶ��� ���ɴ�ȭ + ���׸� ����
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
