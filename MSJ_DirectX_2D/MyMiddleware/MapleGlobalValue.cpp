#include "MapleGlobalValue.h"

// �÷��̾�
Vector4 MapleGlobalValue::PlayerPostion = Vector4::Zero;
int MapleGlobalValue::PlayerHP = 0;
int MapleGlobalValue::PlayerMP = 0;
int MapleGlobalValue::PlayerEXP = 0;

// ī�޶�
Vector4 MapleGlobalValue::CameraMovePos = Vector4::Zero;

// �κ��丮
int MapleGlobalValue::Meso = 0;
Vector4 MapleGlobalValue::InventoryEmptyObjectPos = Vector4::Zero;
std::vector<GameObject*> MapleGlobalValue::InvenItemConsum;

// ������
std::vector<GameObject*> MapleGlobalValue::QuickSlotVector;

//
void MapleGlobalValue::SetInvenItemConsume(std::vector<GameObject*> _ConsumeVector)
{
	InvenItemConsum = _ConsumeVector;
}

void MapleGlobalValue::SetQuickSlotVector(std::vector<GameObject*> _QuickSlotVector)
{
	QuickSlotVector = _QuickSlotVector;
}

void MapleGlobalValue::PlayerInfo(Vector4 _Pos, int _HP, int _MP, int _EXP)
{
	PlayerPostion = _Pos;

	PlayerHP = _HP;
	PlayerMP = _MP;
	PlayerEXP = _EXP;
}
void MapleGlobalValue::SetCameraMovePos(Vector4 _MovePos)
{
	CameraMovePos = _MovePos;
}

