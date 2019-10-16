#pragma once
#include <Scene.h>
#include <RenderOrder.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class ChewChewIslandMonsterPos
{
public:
	friend class MonsterStageScene1;
public:

	Vector4 MonsterPos;
	bool m_bTile;
	bool m_life;

public:
	ChewChewIslandMonsterPos() {};
	~ChewChewIslandMonsterPos() {};

};

class Collision;
class GameObject;
class InGameCamera;
class UIMove;
class PlayerMove;
class SkillManager;
class MouseCursorScript;
class Inven_BtAnimation;
class Inven_BtKind;
class Inven_MouseItem;
class Inven_MoveScript;
class WorldMouseCursorScript;
class QuestAlarm;
class ShopBuyItemBt;
class ShopSellItemManagerScript;
class ShopBuyItemNumber;
class MesoManager;
class QuestAlarmElixirNumberScript;
class RectCol2D;
class SoundPlayer;
class MonsterStageScene1 : public Scene::SceneScript, public RenderOrder
{
private:
	My_PTR<GameObject> m_BGMEmptyObject;
	My_PTR<SoundPlayer> m_BGMPlayer;

	My_PTR<GameObject> m_EffectSoundObject;
	My_PTR<SoundPlayer> m_EffectSoundPlayer;

	My_PTR<GameObject> m_VoiceSoundObject;
	My_PTR<SoundPlayer> m_VoiceSoundPlayer;

private:
	// ���콺
	My_PTR<GameObject> m_Mouse;
	My_PTR<MouseCursorScript> MouseScript;

	My_PTR<GameObject> m_InGameMouse;
	My_PTR<WorldMouseCursorScript> m_InGameMouseWorldScript;

	My_PTR<RectCol2D> CursorCol;
	My_PTR<RectCol2D> InGameCursorCol;

private:
	My_PTR<GameObject> m_GamePlayer;
	My_PTR<GameObject> m_PlayerCoatArm;		// ��Ʈ �Ⱥκ�
	My_PTR<GameObject> m_PlayerArm;			// ��
	My_PTR<GameObject> m_PlayerEye;			// ��
	My_PTR<GameObject> m_PlayerHair;		// ��� ������Ʈ
	My_PTR<GameObject> m_PlayerHead;		// �Ӹ� (��Ӹ�)
	My_PTR<GameObject> m_PlayerCoat;		// ��Ʈ ��ݽ� ��ü
	My_PTR<GameObject> m_PlayerBody;		// ��

	My_PTR<PlayerMove> m_PlayerMoveScript;
	My_PTR<SkillManager> m_SkillManager;

	My_PTR<RectCol2D> m_PlayerCol;
private:
	My_PTR<GameObject> m_ChildManeequin;
	My_PTR<GameObject> m_TransformSlime;

private:
	My_PTR<GameObject> m_Background;
	My_PTR<GameObject> m_MoveTile;

private:
	My_PTR<GameObject> m_MainCamera;
	My_PTR<Camera> m_CameraCom;
	My_PTR<InGameCamera> m_CamScript;

private:
	My_PTR<GameObject> m_UICamera;
	My_PTR<Camera> m_UICameraCom;

private:
	// UI
	My_PTR<GameObject> m_StatusBarBackground;		// �⺻ ���
	My_PTR<GameObject> m_StatusBarBase;				// ���� ���
	My_PTR<GameObject> m_StatusQuickSlot;			// ������

	My_PTR<GameObject> m_StatusBarGauge;			// ��������
	My_PTR<GameObject> m_GaugeGraduation;
	My_PTR<GameObject> m_HPGray;					// HP
	My_PTR<GameObject> m_MPGray;					// MP
	My_PTR<GameObject> m_ExpGray;					// ����ġ

private:
	// ��Ż
	My_PTR<GameObject> m_PrePortal;
	My_PTR<GameObject> m_NextPortal;

	My_PTR<RectCol2D> m_PrePortalCol;
	My_PTR<RectCol2D> m_NextPortalCol;

private:
	// �κ��丮
	int m_Meso;
	int m_MaplePoint;

	My_PTR<GameObject> m_InventoryEmptyObj;			// �� �θ� ������Ʈ
	My_PTR<GameObject> m_InvenMoveObj;				// ������ ������Ʈ ( �θ� ������ ���� )
	My_PTR<GameObject> m_InvenBase;					// �κ��丮 ���̽�

	My_PTR<GameObject> m_InvenBtClose;				// �κ��丮 �ݱ� ��ư

	My_PTR<GameObject> m_InvenBtDisassemble;		// �κ��丮 ��Ϲ��� ��ư
	My_PTR<GameObject> m_InvenBtExtract;			// �κ��丮 �����ư
	My_PTR<GameObject> m_InvenBtAppraise;			// �κ��丮 �������ư
	My_PTR<GameObject> m_InvenBtPot;				// �κ��丮 ���� ��ư
	My_PTR<GameObject> m_InvenBtToad;				// �κ��丮 ��ġ->ȭ��ǥ ��ư
	My_PTR<GameObject> m_InvenBtUpgrade;			// �κ��丮 ��ȭ ��ư

	My_PTR<GameObject> m_InvenBtEquip;				// �κ��丮 ��� ��ư
	My_PTR<GameObject> m_InvenBtConsum;				// �κ��丮 �Һ� ��ư
	My_PTR<GameObject> m_InvenBtEtc;				// �κ��丮 ��Ÿ ��ư
	My_PTR<GameObject> m_InvenBtInstall;			// �κ��丮 ��ġ ��ư
	My_PTR<GameObject> m_InvenBtCash;				// �κ��丮 ĳ�� ��ư

	My_PTR<GameObject> m_InvenClickMouseImage;
	My_PTR<GameObject> m_ClickItemInfo;
	My_PTR<Inven_MouseItem> InvenClickImageScript;

	// �κ��丮 ��ũ��Ʈ
	// �ִϸ��̼�
	My_PTR<Inven_BtAnimation> InvenBtCloseAniScript;		// �ݱ�
	My_PTR<Inven_BtAnimation> InvenBtDisassembleAniScript;	// ��Ϲ���
	My_PTR<Inven_BtAnimation> InvenBtExtractAniScript;		// �����ư
	My_PTR<Inven_BtAnimation> InvenBtAppraiseAniScript;		// ������
	My_PTR<Inven_BtAnimation> InvenBtToadAniScript;			// ��ġ -> ȭ��ǥ
	My_PTR<Inven_BtAnimation> InvenBtUpgradeAniScript;		// ��ġ ��ȭ ��ư

	My_PTR<Inven_BtKind> InvenBtEquipAniScript;				// ���
	My_PTR<Inven_BtKind> InvenBtComsumAniScript;			// �Һ�
	My_PTR<Inven_BtKind> InvenBtEtcAniScript;				// ��Ÿ
	My_PTR<Inven_BtKind> InvenBtInstallAniScript;			// ��ġ
	My_PTR<Inven_BtKind> InvenBtCashAniScript;				// ��Ÿ

															//
	My_PTR<Inven_MoveScript> InvenMoveObjectScript;


	// �κ��丮 �浹ü��
	My_PTR<RectCol2D> InvenMoveCol;
	My_PTR<RectCol2D> InvenBtCloseCol;
	My_PTR<RectCol2D> InvenBtDisassembleCol;
	My_PTR<RectCol2D> InvenBtExtractCol;
	My_PTR<RectCol2D> InvenBtAppraiseCol;
	My_PTR<RectCol2D> InvenBtToadCol;
	My_PTR<RectCol2D> InvenBtUpgradeCol;
	My_PTR<RectCol2D> InvenBtEquipCol;
	My_PTR<RectCol2D> InvenBtComsumCol;
	My_PTR<RectCol2D> InvenBtEtcCol;
	My_PTR<RectCol2D> InvenBtInstallCol;
	My_PTR<RectCol2D> InvenBtCashCol;

	std::vector<RectCol2D*> m_InventoryButtonColVector;		// ��������Ұ�

	std::vector<GameObject*> m_InvenItemEquip;
	std::vector<GameObject*> m_InvenItemConsum;
	std::vector<GameObject*> m_InvenItemEtc;
	std::vector<GameObject*> m_InvenItemInstall;
	std::vector<GameObject*> m_InvenItemCash;

	std::vector<RectCol2D*> InvenItemEquipCol;
	std::vector<RectCol2D*> InvenItemConSumCol;
	std::vector<RectCol2D*> InvenItemEtcCol;
	std::vector<RectCol2D*> InvenItemInstallCol;
	std::vector<RectCol2D*> InvenItemCashCol;

	std::vector<std::vector<GameObject*>> m_AllItems;


private:
	// ������
	My_PTR<GameObject> m_InvenMoneyManagerObject;
	My_PTR<GameObject> m_ShopMeoneyManagerObject;

	My_PTR<MesoManager> m_InvenMoenyManagerScript;

private:
	// ������ ->   ������ ���(�ǵڿ��ִ� �̹���) -> ��ų or ���� �̹��� -> ������ ��Ÿ�� -> ������ ����̹��� (�Ǿ�)
	My_PTR<GameObject> m_QuickSlotEmptyObj;			// �θ� ����
	My_PTR<GameObject> m_QuickSlotBGBack;			// ������ ��� (�� �ڿ��ִ� �̹���)
	My_PTR<GameObject> m_QuickSlotBGFront;			// ������ ��� ( �� �տ� �ִ� �̹��� )

	std::vector<GameObject*> m_QuickSlotVector;		// ������ - �Һ���, ��ų ������ ���� �ִ� ���� 
	std::vector<RectCol2D*> m_QuickSlotCol;


private:
	// ���� ����
	std::vector<ChewChewIslandMonsterPos> m_ChildMannequinMosnterPos;
	std::unordered_map<int, GameObject*> m_ChildMannequinMonsterMap;
	std::vector<GameObject*> m_ChildMannequinMonsterVector;
	std::vector<RectCol2D*> m_ChildMannequinMonsterCol;

	std::vector<ChewChewIslandMonsterPos> m_WhiteSandRabbitMonsterPos;
	std::unordered_map<int, GameObject*> m_WhiteSandRabbitMonsterMap;
	std::vector<GameObject*> m_WhiteRabbitMonsterVector;
	std::vector<RectCol2D*> m_WhiteSandRabbitMonsterCol;

private:
	// ������
	std::vector<GameObject*> m_ItemObjVector;
	std::vector<RectCol2D*> m_ItemColVector;


private:
	void MonsterLoading();
	void MonsterPosInit();
	void ChildMannequinPosInit();
	void WhiteSandRabbitPosInit();

	void BGLoading();
	void UILoading();
	void CreateCamera();
	void PlayerLoading();
	void CameraScriptLoading();
	void PortalLoading();
	void InventoryLoading();
	void MouseLoading();
	void CollisionNameLink();
	void QuickSlotLoading();
	void MoneyLoading();

private:
	// ����
	void BGMSoundLoading();
	void SoundPlay_BGM(std::wstring _SoundFileName);

	void EffectSoundLoading();
	void SoundPlay_Effect(std::wstring _SoundFileName);

	void VoiceSoundLoading();
	void SoundPlaye_Voice(std::wstring _SoundFileName);

private:
	// �κ��丮
	void InventoryCreate();
	void InventorySlotCreate();

	void InventoryOff();
	void InventoryOn();
	void InventorySlotOff(std::vector<GameObject*> _Slot);
	void InventorySlotOn(std::vector<GameObject*> _Slot);

	void ClickItemInfo(GameObject* _ItemObj);

private:
	void CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z);

public:
	bool Loading() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;
	void DebugRender() override;


public:
	// UI �浹
	void Col_InventoryEnter(Collision* _This, Collision* _Other);
	void Col_InventoryStay(Collision* _This, Collision* _Other);
	void Col_InventoryExit(Collision* _This, Collision* _Other);

	// ���� �浹
	void Col_MonsterEnter(Collision* _This, Collision* _Other);
	void Col_MonsterStay(Collision* _This, Collision* _Other);
	void Col_MonsterExit(Collision* _This, Collision* _Other);

	// �÷��̾�� �浹 - ������
	void Col_PlayerEnter(Collision* _This, Collision* _Other);
	void Col_PlayerStay(Collision* _This, Collision* _Other);
	void Col_PlayerExit(Collision* _This, Collision* _Other);

public:
	MonsterStageScene1();
	~MonsterStageScene1();
};

