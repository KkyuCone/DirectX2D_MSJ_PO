#pragma once

class QuickSlot_Kind
{
public:
	enum QuickSlotCode
	{
		EMPTY = 0,
		POTION_RED,
		POTION_BLUE,
		POTION_WHITE,
		POTION_ORANGE,
		SKILL_BE,			// ����¡ �ͽ��ü�
		SKILL_PM,			// ������ �̽�Ʈ
		SKILL_SP,			// �ҿ��۳�Ʈ���̼�
		SKILL_CC,			// īŸŬ����
		SKILL_BZ,			// ���ڵ�
	};

public:
	QuickSlot_Kind() {}
	~QuickSlot_Kind() {}
};