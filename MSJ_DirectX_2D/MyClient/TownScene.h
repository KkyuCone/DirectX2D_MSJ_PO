#pragma once
#include <Scene.h>
#include <RenderOrder.h>
#include <EnumOrder.h>
#include <QuestDialogSituation.h>

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
class BGFade;
class TownScene : public Scene::SceneScript, public RenderOrder, public EnumOrder
{
private:
	My_PTR<GameObject> m_BGMEmptyObject;
	My_PTR<SoundPlayer> m_BGMPlayer;

	My_PTR<GameObject> m_EffectSoundObject;
	My_PTR<SoundPlayer> m_EffectSoundPlayer;

	My_PTR<GameObject> m_VoiceSoundObject;
	My_PTR<SoundPlayer> m_VoiceSoundPlayer;

private:
	bool m_bFirstScene;
private:
	My_PTR<GameObject> m_Mouse;
	My_PTR<MouseCursorScript> MouseScript;

	My_PTR<GameObject> m_InGameMouse;
	My_PTR<WorldMouseCursorScript> m_InGameMouseWorldScript;


private:
	My_PTR<GameObject> m_Background;
	//My_PTR<GameObject> m_TileObject;
	My_PTR<GameObject> m_MoveTile;
	My_PTR<PlayerMove> m_PlayerMoveScript;
	My_PTR<SkillManager> m_SkillManager;

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
	// UI Script
	My_PTR<UIMove> m_StatusBarBackgroundScript;
	My_PTR<UIMove> m_StatusBarBaseScript;
	My_PTR<UIMove> m_StatusQuickSlotScript;

	My_PTR<UIMove> m_StatusBarGaugeScript;
	My_PTR<UIMove> m_GaugeGraduationScript;
	My_PTR<UIMove> m_HPGrayScript;
	My_PTR<UIMove> m_MPGrayScript;
	My_PTR<UIMove> m_ExpGrayScript;

private:
	// ĳ���� �κ� �����������.. !
	My_PTR<GameObject> m_GamePlayer;		// �� ĳ���� ������Ʈ
	My_PTR<GameObject> m_PlayerCoatArm;		// ��Ʈ �Ⱥκ�
	My_PTR<GameObject> m_PlayerArm;			// ��
	My_PTR<GameObject> m_PlayerEye;			// ��
	My_PTR<GameObject> m_PlayerHair;		// ��� ������Ʈ
	My_PTR<GameObject> m_PlayerHead;		// �Ӹ� (��Ӹ�)
	My_PTR<GameObject> m_PlayerCoat;		// ��Ʈ ��ݽ� ��ü
	My_PTR<GameObject> m_PlayerBody;		// ��


	My_PTR<GameObject> m_Monster;			// ����

	My_PTR<GameObject> m_CMMonster;			// ���� ����ŷ ����
	My_PTR<GameObject> m_WSRMosnter;		// �� ���䳢 ����
	My_PTR<GameObject> m_TSMonster;			// ������ ������ ����
	My_PTR<GameObject> m_VMonster;			// ���󸮽�
	My_PTR<GameObject> m_PinkbinMonster;

	std::vector<My_PTR<GameObject>> m_MonstersVector;			// ���͵��� ��Ƶ� ����

	My_PTR<RectCol2D> m_PlayerCol;


private:
	// NPC
	My_PTR<GameObject> m_WindSpiritNPC;
	My_PTR<GameObject> m_TreeSpiritNPC;
	My_PTR<GameObject> m_SmallSpiritNPC;
	My_PTR<GameObject> m_RockSpiritNPC;

	My_PTR<GameObject> m_PotionNPC;
	My_PTR<GameObject> m_WarehouseNPC;

private:
	// �̸��±�
	My_PTR<GameObject> m_PotionNPCNameTag;
	My_PTR<GameObject> m_WarehouseNPCNameTag;
	My_PTR<GameObject> m_WindSpiritNPCNameTag;
	My_PTR<GameObject> m_TreeSpiritNPCNameTag;
	My_PTR<GameObject> m_SmallSpiritNPCNameTag;
	My_PTR<GameObject> m_RockSpiritNPCNameTag;

private:
	///////////////////////////////////////////////////////// ����
	// ���� Ŭ�� �ݶ��̴� -> NPC�� ����
	std::vector<RectCol2D*> m_ColNPCVec;

	// ����
	My_PTR<GameObject> m_ShopEmptyObj;
	My_PTR<GameObject> m_ShopBackground;
	My_PTR<GameObject> m_Shop;

	// ��ư
	My_PTR<GameObject> m_ShopBtEquip;
	My_PTR<GameObject> m_ShopBtComsume;
	My_PTR<GameObject> m_ShopBtEtc;
	My_PTR<GameObject> m_ShopBtInstall;
	My_PTR<GameObject> m_ShopCash;

	My_PTR<GameObject> m_ShopBtAll;
	My_PTR<GameObject> m_ShopBtReBuy;

	My_PTR<GameObject> m_ShopBtItemBuy;
	My_PTR<GameObject> m_ShopBtItemSell;
	My_PTR<GameObject> m_ShopBtExit;

	// ���� ��ư �ִϸ��̼� ��ũ��Ʈ
	My_PTR<Inven_BtKind> m_ShopEquipAniScript;			// ���
	My_PTR<Inven_BtKind> m_ShopConsumeAniScript;		// �Һ�
	My_PTR<Inven_BtKind> m_ShopEtcAniScript;			// ��Ÿ
	My_PTR<Inven_BtKind> m_ShopInstallAniScript;		// ��ġ
	My_PTR<Inven_BtKind> m_ShopCashAniScript;			// ĳ��

	My_PTR<Inven_BtKind> m_ShopAllAniScript;			// ��ü
	My_PTR<Inven_BtKind> m_ShopRebuyAniScript;			// �籸��

	My_PTR<Inven_BtAnimation> m_ShopExitAniScript;		// ����������
	My_PTR<Inven_BtAnimation> m_ShopItemBuyAniScript;	// ���� �����۱���
	My_PTR<Inven_BtAnimation> m_ShopItemSellAniScript;	// ���� �������Ǹ�

	// �浹ü
	My_PTR<RectCol2D> m_ShopEquipCol;					// ���
	My_PTR<RectCol2D> m_ShopConsumeCol;					// �Һ�
	My_PTR<RectCol2D> m_ShopEtcCol;						// ��Ÿ
	My_PTR<RectCol2D> m_ShopInstallCol;					// ��ġ
	My_PTR<RectCol2D> m_ShopCashCol;					// ĳ��

	My_PTR<RectCol2D> m_ShopAllCol;						// ��ü
	My_PTR<RectCol2D> m_ShopRebuyCol;					// �籸��

	My_PTR<RectCol2D> m_ShopExitCol;					// ����������
	My_PTR<RectCol2D> m_ShopItemBuyCol;					// ���� ������ ���
	My_PTR<RectCol2D> m_ShopItemSellCol;				// ���� ������ �ȱ�


	// ���� �Ǹ�����(������) -> �ѹ��� 50���� �絵�� ����
	My_PTR<GameObject> m_ShopBuyRedPotion;
	My_PTR<GameObject> m_ShopBuyBluePotion;
	My_PTR<GameObject> m_ShopBuyWhitePoiton;
	My_PTR<GameObject> m_ShopBuyOrangePoiton;
	My_PTR<GameObject> m_ShopBuyManaElixir;

	My_PTR<ShopBuyItemBt> m_ShopBuyRedPoitonAniScript;
	My_PTR<ShopBuyItemBt> m_ShopBuyBluePoitonAniScript;
	My_PTR<ShopBuyItemBt> m_ShopBuyWhitePoitonAniScript;
	My_PTR<ShopBuyItemBt> m_ShopBuyOrangePoitonAniScript;
	My_PTR<ShopBuyItemBt> m_ShopBuyManaElxirAniScript;

	My_PTR<RectCol2D> m_ShopBuyRedPotionCol;
	My_PTR<RectCol2D> m_ShopBuyBluePotionCol;
	My_PTR<RectCol2D> m_ShopBuyWhitePotionCol;
	My_PTR<RectCol2D> m_ShopBuyOrangePoitonCol;
	My_PTR<RectCol2D> m_ShopBuyElixirCol;

	// ������ ������ ������ ��� ���� wstring
	std::wstring m_ShopBuySelectItem;
	GameObject* m_shopSellSelectItem = nullptr;// ���� �� ������ �����ϱ�
	int CurSelectShopMyItemOrder = 0;

	// �� �κ��丮 ���� (�ȼ��ִ� ������ �������.. 5��)
	My_PTR<GameObject> m_ShopSellMyItemManager;
	My_PTR<ShopSellItemManagerScript> m_ShopSellMyItemManagerScript;
	My_PTR<GameObject> m_ShopSellMyItem[5];
	My_PTR<ShopBuyItemBt> m_ShopSellMyItemAniScript[5];
	My_PTR<ShopBuyItemNumber> m_ShopSellMyItemNumberScript[5];
	My_PTR<RectCol2D> m_ShopSellMyItemCol[5];

private:
	// ������
	My_PTR<GameObject> m_InvenMoneyManagerObject;
	My_PTR<GameObject> m_ShopMeoneyManagerObject;

	My_PTR<MesoManager> m_InvenMoenyManagerScript;
	My_PTR<MesoManager> m_ShopManagerScript;

private:
	// ��Ż
	My_PTR<GameObject> m_PrePortal;
	My_PTR<GameObject> m_NextPortal;

	My_PTR<RectCol2D> m_PrePortalCol;
	My_PTR<RectCol2D> m_NextPortalCol;

private:
	// ����
	My_PTR<GameObject> m_Snow01;

private:
	// ����Ʈ ����
	My_PTR<GameObject> m_RockQuestAlarm;
	My_PTR<QuestAlarm> RockQuestAlarmScript;

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
	My_PTR<RectCol2D> CursorCol;
	My_PTR<RectCol2D> InGameCursorCol;

private:
	// ������ ->   ������ ���(�ǵڿ��ִ� �̹���) -> ��ų or ���� �̹��� -> ������ ��Ÿ�� -> ������ ����̹��� (�Ǿ�)
	My_PTR<GameObject> m_QuickSlotEmptyObj;			// �θ� ����
	My_PTR<GameObject> m_QuickSlotBGBack;			// ������ ��� (�� �ڿ��ִ� �̹���)
	My_PTR<GameObject> m_QuickSlotBGFront;			// ������ ��� ( �� �տ� �ִ� �̹��� )

	std::vector<GameObject*> m_QuickSlotVector;		// ������ - �Һ���, ��ų ������ ���� �ִ� ���� 
	std::vector<RectCol2D*> m_QuickSlotCol;


private:
	// ����Ʈ ���� - �˸���
	My_PTR<GameObject> m_QuestAlarmEmptyObj;
	My_PTR<GameObject> m_QuestAlarmBG;						// ����Ʈ �˸��� ���
	My_PTR<GameObject> m_QuesetAlarmManaElixirNumbers;		// ������������ ������ �˷��� ���� -> ��ũ��Ʈ �ʿ�
	My_PTR<QuestAlarmElixirNumberScript> m_QuestAlarmManaElixirNumbersScript;		// ���� ������ ���� ��ũ��Ʈ

	// ����Ʈ ���� - ��ȭ
	My_PTR<GameObject> m_QuestDialogEmptyObj;				// ����Ʈ ��ȭâ �⺻ ��ġ�� ���� �ִ� �� ������Ʈ
	My_PTR<GameObject> m_NPCDialogBaseBackground;			// NPC ��ȭâ ���(����)
	My_PTR<GameObject> m_NPCTexture2D;						// ��ȭ���� NPC �̹��� 
	My_PTR<GameObject> m_NPCDialogChat;						// NPC ��ȭ(��ȭ)
	My_PTR<GameObject> m_NPCNameTag;						// NPC �̸�ǥ(�Ķ���)

	My_PTR<GameObject> m_QuestBtDialogExit;					// ��ȭ �׸��ϱ�
	My_PTR<GameObject> m_QuestBtDialogYes;					// �� / ����
	My_PTR<GameObject> m_QuestBtDialogNo;					// �ƴϿ� / ����
	My_PTR<GameObject> m_QuestBtDialogCheck;				// Ȯ��

	// ����Ʈ ��ư �浹ü
	My_PTR<RectCol2D> m_QuestBtDialogExitCol;
	My_PTR<RectCol2D> m_QuestBtDialogYesCol;
	My_PTR<RectCol2D> m_QuestBtDialogNoCol;
	My_PTR<RectCol2D> m_QuestBtDialogCheckCol;

	// ��ư ��ũ��Ʈ
	My_PTR<Inven_BtAnimation> m_QDialogExitBtAniScript;
	My_PTR<Inven_BtAnimation> m_QDialogYesBtAniScript;
	My_PTR<Inven_BtAnimation> m_QDialogNoBtAniScript;
	My_PTR<Inven_BtAnimation> m_QDialogCheckAniScript;

	QuestDialogSituation::Dialog_Situation m_CurQuestSituation;
	QuestDialogSituation::Dialog_NPC m_DialogNPC;

	int m_RockQuestPresent;			// 0����Ʈ �ޱ��� , 1 ����Ʈ ���� �� ������, 2����Ʈ ���� 3 ����Ʈ �Ϸ� �� 




public:
	/*void CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent);*/
	void CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z);

private:
	void BGLoading();
	void UILoading();
	void UIScriptLoading();
	void CreateCamera();
	void PlayerLoading();
	void CameraScriptLoading();
	void NPCLoading();
	void PortalLoading();
	void WeatherLoading();
	void InventoryLoading();
	void MouseLoading();
	void CollisionNameLink();
	void QuickSlotLoading();
	void QuestAlarmLoading();
	void ShopLoading();
	void MoneyLoading();
	void QuestLoading();



private:
	void ShopOff();
	void ShopOn();

	void ShopBuyItemAniChange_None();
	void ShopBuyItemAniChange_Pressed();

	void ShopSellItemAniChange_None();
	void ShopSellItemAniChange_Pressed();

	void ShopBuyAllItemOn();
	void ShopBuyAllItemOff();

	void ShopBuyItemManager(std::wstring _SelectItem);
	void ShopSellItemManager(GameObject* _SelectItem);

	void ShopMyItem_On();
	void ShopMyItem_Off();

	void QuestDialog_AllOff();			// ����Ʈ ��ȭâ ���� ����
	void QuestRockDialog_On(QuestDialogSituation::Dialog_NPC _NPC);

	void QuestAlarm_Off();
	void QuestAlarm_On();


private:

	void InventoryCreate();
	void InventorySlotCreate();

	void InventoryOff();
	void InventoryOn();
	void InventorySlotOff(std::vector<GameObject*> _Slot);
	void InventorySlotOn(std::vector<GameObject*> _Slot);

	void ClickItemInfo(GameObject* _ItemObj);

private:
	// ����
	void BGMSoundLoading();
	void SoundPlay_BGM(std::wstring _SoundFileName);

	void EffectSoundLoading();
	void SoundPlay_Effect(std::wstring _SoundFileName);

	void VoiceSoundLoading();
	void SoundPlaye_Voice(std::wstring _SoundFileName);

public:
	bool Loading() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;

	void DebugRender() override;

public:
	// �浹
	void ColTestEnter(Collision* _This, Collision* _Other);
	void ColTestStay(Collision* _This, Collision* _Other);
	void ColTestExit(Collision* _This, Collision* _Other);

	// �浹 - ��Ż
	void ColPortalEnter(Collision* _This, Collision* _Other);
	void ColPortalStay(Collision* _This, Collision* _Other);
	void ColPortalExit(Collision* _This, Collision* _Other);

	// UI �浹
	void Col_InventoryEnter(Collision* _This, Collision* _Other);
	void Col_InventoryStay(Collision* _This, Collision* _Other);
	void Col_InventoryExit(Collision* _This, Collision* _Other);

	// ����Ʈ �ޱ�
	void Col_QuestEnter(Collision* _This, Collision* _Other);
	void Col_QuestStay(Collision* _This, Collision* _Other);
	void Col_QuestExit(Collision* _This, Collision* _Other);

public:
	TownScene();
	~TownScene();
};

