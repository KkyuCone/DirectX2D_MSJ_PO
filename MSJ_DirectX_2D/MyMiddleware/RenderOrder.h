#pragma once


class RenderOrder
{
public:
	enum RENDER_ORDER
	{
		BACKGROUND = 0,						// ���
		BG_Layer02,
		BG_Layer03,
		BG_Layer04,
		BG_Layer05,
		BG_Layer06,
		BG_Layer07,
		NPC,
		PLAYER_PARENT,					// �÷��̾� - �θ�
		PLAYER_CHILD,					// �÷��̾� ���� ������Ʈ�� (��Ʈ, ��, �� ��)
		MONSTER,						// ����
		MONSTER_ATTACK,
		UI_GENHPBAR,
		COLTILE,					// �浹�� �ٴ�(Ÿ�Ϻκ�)
		UI_PORTAL,
		UI_MOUSECURSOR = 50,
		UI_QUICKSLOT,
		UI_QUESTDIALOG,
		UI_ITEM,
		UI_INVENTORY,
		WEATHER_SNOW,
		UI_PINKBIN_FIRSTLINE,
		UI_PINKBIN_SECONDLINE,
		UI_PINKBIN_GRAYLINE,
		UI_OBJECT,
		UI_BG,
		UI_QUESTNPCTEXTURE2D,
		SKILL_EFF0 = 120,						// ��ų ����Ʈ 0
		SKILL_EFF1,						// ��ų ����Ʈ 1
		SKILL_EFF2,						// ��ų ����Ʈ 2
		SKILL_EFF3,						// ��ų ����Ʈ 3
		SKILL_EFF4,						// ��ų ����Ʈ 4
		SKILL_EFF5,						// ��ų ����Ʈ 5
		SKILL_EFF6,						// ��ų ����Ʈ 6
		SKILL_EFF7,						// ��ų ����Ʈ 7
		SKILL_EFF8,						// ��ų ����Ʈ 8
		SKILL_EFF9,						// ��ų ����Ʈ 9
		SKILL_EFF10,						// ��ų ����Ʈ 9
		SKILL_EFF11,						// ��ų ����Ʈ 9
		SKILL_EFF12,						// ��ų ����Ʈ 9
		SKILL_EFF13,						// ��ų ����Ʈ 9

	};
};