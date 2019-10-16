#pragma once
#include <DXHeader.h>
#include <STLHelper.h>

#include <GameObject.h>
#include <RectCol2D.h>

#include <vector>

class MapleGlobalValue
{
public:
	// 캐릭터 -  위치, HP, MP, EXP, 레벨
	static Vector4 PlayerPostion;
	static Vector4 CameraMovePos;
	static int PlayerHP;
	static int PlayerMP;
	static int PlayerEXP;

	// 인벤토리
	static int Meso;		// 돈

	static Vector4 InventoryEmptyObjectPos;	// 인벤토리 빈 부모 오브젝트
	static std::vector<GameObject*> InvenItemConsum;	// 소비창 정보

	// 퀵슬롯
	static std::vector<GameObject*> QuickSlotVector;	// 퀵슬롯 정보 ( 쿨타임, 물약위치 등 )


public:
	static void SetInvenItemConsume(std::vector<GameObject*> _ConsumeVector);
	static void SetQuickSlotVector(std::vector<GameObject*> _QuickSlotVector);
	static void PlayerInfo(Vector4 _Pos, int HP, int _MP, int _EXP);
	static void SetCameraMovePos(Vector4 _MovePos);


private:
	MapleGlobalValue() {}
	virtual  ~MapleGlobalValue() = 0;
};

