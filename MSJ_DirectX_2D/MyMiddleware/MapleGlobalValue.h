#pragma once
#include <DXHeader.h>
#include <STLHelper.h>

#include <GameObject.h>
#include <RectCol2D.h>

#include <vector>

class MapleGlobalValue
{
public:
	// ĳ���� -  ��ġ, HP, MP, EXP, ����
	static Vector4 PlayerPostion;
	static Vector4 CameraMovePos;
	static int PlayerHP;
	static int PlayerMP;
	static int PlayerEXP;

	// �κ��丮
	static int Meso;		// ��

	static Vector4 InventoryEmptyObjectPos;	// �κ��丮 �� �θ� ������Ʈ
	static std::vector<GameObject*> InvenItemConsum;	// �Һ�â ����

	// ������
	static std::vector<GameObject*> QuickSlotVector;	// ������ ���� ( ��Ÿ��, ������ġ �� )


public:
	static void SetInvenItemConsume(std::vector<GameObject*> _ConsumeVector);
	static void SetQuickSlotVector(std::vector<GameObject*> _QuickSlotVector);
	static void PlayerInfo(Vector4 _Pos, int HP, int _MP, int _EXP);
	static void SetCameraMovePos(Vector4 _MovePos);


private:
	MapleGlobalValue() {}
	virtual  ~MapleGlobalValue() = 0;
};

