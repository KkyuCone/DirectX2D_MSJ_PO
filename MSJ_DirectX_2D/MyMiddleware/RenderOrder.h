#pragma once


class RenderOrder
{
public:
	enum RENDER_ORDER
	{
		BACKGROUND = 0,						// 배경
		BG_Layer02,
		BG_Layer03,
		BG_Layer04,
		BG_Layer05,
		BG_Layer06,
		BG_Layer07,
		NPC,
		PLAYER_PARENT,					// 플레이어 - 부모
		PLAYER_CHILD,					// 플레이어 하위 오브젝트들 (코트, 눈, 얼굴 등)
		MONSTER,						// 몬스터
		MONSTER_ATTACK,
		UI_GENHPBAR,
		COLTILE,					// 충돌할 바닥(타일부분)
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
		SKILL_EFF0 = 120,						// 스킬 이펙트 0
		SKILL_EFF1,						// 스킬 이펙트 1
		SKILL_EFF2,						// 스킬 이펙트 2
		SKILL_EFF3,						// 스킬 이펙트 3
		SKILL_EFF4,						// 스킬 이펙트 4
		SKILL_EFF5,						// 스킬 이펙트 5
		SKILL_EFF6,						// 스킬 이펙트 6
		SKILL_EFF7,						// 스킬 이펙트 7
		SKILL_EFF8,						// 스킬 이펙트 8
		SKILL_EFF9,						// 스킬 이펙트 9
		SKILL_EFF10,						// 스킬 이펙트 9
		SKILL_EFF11,						// 스킬 이펙트 9
		SKILL_EFF12,						// 스킬 이펙트 9
		SKILL_EFF13,						// 스킬 이펙트 9

	};
};