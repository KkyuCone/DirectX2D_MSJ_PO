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
		SKILL_BE,			// ����¡ �ͽ��ü�
		SKILL_PM,			// ������ �̽�Ʈ
		SKILL_SP,			// �ҿ��۳�Ʈ���̼�
		SKILL_CC,			// īŸŬ����
		SKILL_BZ,			// ���ڵ�
	};

public:
	ItemCode() {}
	~ItemCode() {}
};