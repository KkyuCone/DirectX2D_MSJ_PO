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
	My_PTR<GameObject> m_StatusBarBackground;		// 기본 배경
	My_PTR<GameObject> m_StatusBarBase;				// 상태 배경
	My_PTR<GameObject> m_StatusQuickSlot;			// 퀵슬롯

	My_PTR<GameObject> m_StatusBarGauge;			// 게이지바
	My_PTR<GameObject> m_GaugeGraduation;
	My_PTR<GameObject> m_HPGray;					// HP
	My_PTR<GameObject> m_MPGray;					// MP
	My_PTR<GameObject> m_ExpGray;					// 경험치

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
	// 캐릭터 부분 순서대로했음.. !
	My_PTR<GameObject> m_GamePlayer;		// 빈 캐릭터 오브젝트
	My_PTR<GameObject> m_PlayerCoatArm;		// 코트 팔부분
	My_PTR<GameObject> m_PlayerArm;			// 팔
	My_PTR<GameObject> m_PlayerEye;			// 눈
	My_PTR<GameObject> m_PlayerHair;		// 헤어 오브젝트
	My_PTR<GameObject> m_PlayerHead;		// 머리 (대머리)
	My_PTR<GameObject> m_PlayerCoat;		// 코트 상반신 전체
	My_PTR<GameObject> m_PlayerBody;		// 몸


	My_PTR<GameObject> m_Monster;			// 몬스터

	My_PTR<GameObject> m_CMMonster;			// 아이 마네킹 몬스터
	My_PTR<GameObject> m_WSRMosnter;		// 흰 모래토끼 몬스터
	My_PTR<GameObject> m_TSMonster;			// 변형된 슬라임 몬스터
	My_PTR<GameObject> m_VMonster;			// 벨라리스
	My_PTR<GameObject> m_PinkbinMonster;

	std::vector<My_PTR<GameObject>> m_MonstersVector;			// 몬스터들을 모아둔 벡터

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
	// 이름태그
	My_PTR<GameObject> m_PotionNPCNameTag;
	My_PTR<GameObject> m_WarehouseNPCNameTag;
	My_PTR<GameObject> m_WindSpiritNPCNameTag;
	My_PTR<GameObject> m_TreeSpiritNPCNameTag;
	My_PTR<GameObject> m_SmallSpiritNPCNameTag;
	My_PTR<GameObject> m_RockSpiritNPCNameTag;

private:
	///////////////////////////////////////////////////////// 상점
	// 상점 클릭 콜라이더 -> NPC에 있음
	std::vector<RectCol2D*> m_ColNPCVec;

	// 상점
	My_PTR<GameObject> m_ShopEmptyObj;
	My_PTR<GameObject> m_ShopBackground;
	My_PTR<GameObject> m_Shop;

	// 버튼
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

	// 상점 버튼 애니메이션 스크립트
	My_PTR<Inven_BtKind> m_ShopEquipAniScript;			// 장비
	My_PTR<Inven_BtKind> m_ShopConsumeAniScript;		// 소비
	My_PTR<Inven_BtKind> m_ShopEtcAniScript;			// 기타
	My_PTR<Inven_BtKind> m_ShopInstallAniScript;		// 설치
	My_PTR<Inven_BtKind> m_ShopCashAniScript;			// 캐시

	My_PTR<Inven_BtKind> m_ShopAllAniScript;			// 전체
	My_PTR<Inven_BtKind> m_ShopRebuyAniScript;			// 재구매

	My_PTR<Inven_BtAnimation> m_ShopExitAniScript;		// 상점나가기
	My_PTR<Inven_BtAnimation> m_ShopItemBuyAniScript;	// 상점 아이템구매
	My_PTR<Inven_BtAnimation> m_ShopItemSellAniScript;	// 상점 아이템판매

	// 충돌체
	My_PTR<RectCol2D> m_ShopEquipCol;					// 장비
	My_PTR<RectCol2D> m_ShopConsumeCol;					// 소비
	My_PTR<RectCol2D> m_ShopEtcCol;						// 기타
	My_PTR<RectCol2D> m_ShopInstallCol;					// 설치
	My_PTR<RectCol2D> m_ShopCashCol;					// 캐쉬

	My_PTR<RectCol2D> m_ShopAllCol;						// 전체
	My_PTR<RectCol2D> m_ShopRebuyCol;					// 재구매

	My_PTR<RectCol2D> m_ShopExitCol;					// 상점나가기
	My_PTR<RectCol2D> m_ShopItemBuyCol;					// 상점 아이템 사기
	My_PTR<RectCol2D> m_ShopItemSellCol;				// 상점 아이템 팔기


	// 상점 판매정보(아이템) -> 한번에 50개씩 사도록 하자
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

	// 선택한 아이템 정보를 담고 있을 wstring
	std::wstring m_ShopBuySelectItem;
	GameObject* m_shopSellSelectItem = nullptr;// 내가 팔 아이템 선택하기
	int CurSelectShopMyItemOrder = 0;

	// 내 인벤토리 정보 (팔수있는 아이템 순서대로.. 5개)
	My_PTR<GameObject> m_ShopSellMyItemManager;
	My_PTR<ShopSellItemManagerScript> m_ShopSellMyItemManagerScript;
	My_PTR<GameObject> m_ShopSellMyItem[5];
	My_PTR<ShopBuyItemBt> m_ShopSellMyItemAniScript[5];
	My_PTR<ShopBuyItemNumber> m_ShopSellMyItemNumberScript[5];
	My_PTR<RectCol2D> m_ShopSellMyItemCol[5];

private:
	// 돈관리
	My_PTR<GameObject> m_InvenMoneyManagerObject;
	My_PTR<GameObject> m_ShopMeoneyManagerObject;

	My_PTR<MesoManager> m_InvenMoenyManagerScript;
	My_PTR<MesoManager> m_ShopManagerScript;

private:
	// 포탈
	My_PTR<GameObject> m_PrePortal;
	My_PTR<GameObject> m_NextPortal;

	My_PTR<RectCol2D> m_PrePortalCol;
	My_PTR<RectCol2D> m_NextPortalCol;

private:
	// 날씨
	My_PTR<GameObject> m_Snow01;

private:
	// 퀘스트 관련
	My_PTR<GameObject> m_RockQuestAlarm;
	My_PTR<QuestAlarm> RockQuestAlarmScript;

private:
	// 인벤토리
	int m_Meso;
	int m_MaplePoint;

	My_PTR<GameObject> m_InventoryEmptyObj;			// 빈 부모 오브젝트
	My_PTR<GameObject> m_InvenMoveObj;				// 움직일 오브젝트 ( 부모를 움직일 것임 )
	My_PTR<GameObject> m_InvenBase;					// 인벤토리 베이스

	My_PTR<GameObject> m_InvenBtClose;				// 인벤토리 닫기 버튼

	My_PTR<GameObject> m_InvenBtDisassemble;		// 인벤토리 톱니바퀴 버튼
	My_PTR<GameObject> m_InvenBtExtract;			// 인벤토리 물약버튼
	My_PTR<GameObject> m_InvenBtAppraise;			// 인벤토리 돋보기버튼
	My_PTR<GameObject> m_InvenBtPot;				// 인벤토리 연인 버튼
	My_PTR<GameObject> m_InvenBtToad;				// 인벤토리 망치->화살표 버튼
	My_PTR<GameObject> m_InvenBtUpgrade;			// 인벤토리 강화 버튼

	My_PTR<GameObject> m_InvenBtEquip;				// 인벤토리 장비 버튼
	My_PTR<GameObject> m_InvenBtConsum;				// 인벤토리 소비 버튼
	My_PTR<GameObject> m_InvenBtEtc;				// 인벤토리 기타 버튼
	My_PTR<GameObject> m_InvenBtInstall;			// 인벤토리 설치 버튼
	My_PTR<GameObject> m_InvenBtCash;				// 인벤토리 캐시 버튼

	My_PTR<GameObject> m_InvenClickMouseImage;
	My_PTR<GameObject> m_ClickItemInfo;
	My_PTR<Inven_MouseItem> InvenClickImageScript;

	// 인벤토리 스크립트
	// 애니메이션
	My_PTR<Inven_BtAnimation> InvenBtCloseAniScript;		// 닫기
	My_PTR<Inven_BtAnimation> InvenBtDisassembleAniScript;	// 톱니바퀴
	My_PTR<Inven_BtAnimation> InvenBtExtractAniScript;		// 물약버튼
	My_PTR<Inven_BtAnimation> InvenBtAppraiseAniScript;		// 돋보기
	My_PTR<Inven_BtAnimation> InvenBtToadAniScript;			// 망치 -> 화살표
	My_PTR<Inven_BtAnimation> InvenBtUpgradeAniScript;		// 망치 강화 버튼

	My_PTR<Inven_BtKind> InvenBtEquipAniScript;				// 장비
	My_PTR<Inven_BtKind> InvenBtComsumAniScript;			// 소비
	My_PTR<Inven_BtKind> InvenBtEtcAniScript;				// 기타
	My_PTR<Inven_BtKind> InvenBtInstallAniScript;			// 설치
	My_PTR<Inven_BtKind> InvenBtCashAniScript;				// 기타

	//
	My_PTR<Inven_MoveScript> InvenMoveObjectScript;


	// 인벤토리 충돌체들
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

	std::vector<RectCol2D*> m_InventoryButtonColVector;		// 보내줘야할것

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
	// 퀵슬롯 ->   퀵슬롯 배경(맨뒤에있는 이미지) -> 스킬 or 물약 이미지 -> 퀵슬롯 쿨타임 -> 퀵슬롯 배경이미지 (맨앞)
	My_PTR<GameObject> m_QuickSlotEmptyObj;			// 부모 슬롯
	My_PTR<GameObject> m_QuickSlotBGBack;			// 퀵슬롯 배경 (맨 뒤에있는 이미지)
	My_PTR<GameObject> m_QuickSlotBGFront;			// 퀵슬롯 배경 ( 맨 앞에 있는 이미지 )

	std::vector<GameObject*> m_QuickSlotVector;		// 퀵슬롯 - 소비템, 스킬 정보를 갖고 있는 벡터 
	std::vector<RectCol2D*> m_QuickSlotCol;


private:
	// 퀘스트 관련 - 알리미
	My_PTR<GameObject> m_QuestAlarmEmptyObj;
	My_PTR<GameObject> m_QuestAlarmBG;						// 퀘스트 알리미 배경
	My_PTR<GameObject> m_QuesetAlarmManaElixirNumbers;		// 마나엘릭서의 개수를 알려줄 변수 -> 스크립트 필요
	My_PTR<QuestAlarmElixirNumberScript> m_QuestAlarmManaElixirNumbersScript;		// 마나 엘릭서 개수 스크립트

	// 퀘스트 관련 - 대화
	My_PTR<GameObject> m_QuestDialogEmptyObj;				// 퀘스트 대화창 기본 위치만 갖고 있는 빈 오브젝트
	My_PTR<GameObject> m_NPCDialogBaseBackground;			// NPC 대화창 배경(갈색)
	My_PTR<GameObject> m_NPCTexture2D;						// 대화중의 NPC 이미지 
	My_PTR<GameObject> m_NPCDialogChat;						// NPC 대화(대화)
	My_PTR<GameObject> m_NPCNameTag;						// NPC 이름표(파랑색)

	My_PTR<GameObject> m_QuestBtDialogExit;					// 대화 그만하기
	My_PTR<GameObject> m_QuestBtDialogYes;					// 예 / 수락
	My_PTR<GameObject> m_QuestBtDialogNo;					// 아니오 / 거절
	My_PTR<GameObject> m_QuestBtDialogCheck;				// 확인

	// 퀘스트 버튼 충돌체
	My_PTR<RectCol2D> m_QuestBtDialogExitCol;
	My_PTR<RectCol2D> m_QuestBtDialogYesCol;
	My_PTR<RectCol2D> m_QuestBtDialogNoCol;
	My_PTR<RectCol2D> m_QuestBtDialogCheckCol;

	// 버튼 스크립트
	My_PTR<Inven_BtAnimation> m_QDialogExitBtAniScript;
	My_PTR<Inven_BtAnimation> m_QDialogYesBtAniScript;
	My_PTR<Inven_BtAnimation> m_QDialogNoBtAniScript;
	My_PTR<Inven_BtAnimation> m_QDialogCheckAniScript;

	QuestDialogSituation::Dialog_Situation m_CurQuestSituation;
	QuestDialogSituation::Dialog_NPC m_DialogNPC;

	int m_RockQuestPresent;			// 0퀘스트 받기전 , 1 퀘스트 받은 후 진행형, 2퀘스트 거절 3 퀘스트 완료 후 




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

	void QuestDialog_AllOff();			// 퀘스트 대화창 전부 끄기
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
	// 사운드
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
	// 충돌
	void ColTestEnter(Collision* _This, Collision* _Other);
	void ColTestStay(Collision* _This, Collision* _Other);
	void ColTestExit(Collision* _This, Collision* _Other);

	// 충돌 - 포탈
	void ColPortalEnter(Collision* _This, Collision* _Other);
	void ColPortalStay(Collision* _This, Collision* _Other);
	void ColPortalExit(Collision* _This, Collision* _Other);

	// UI 충돌
	void Col_InventoryEnter(Collision* _This, Collision* _Other);
	void Col_InventoryStay(Collision* _This, Collision* _Other);
	void Col_InventoryExit(Collision* _This, Collision* _Other);

	// 퀘스트 받기
	void Col_QuestEnter(Collision* _This, Collision* _Other);
	void Col_QuestStay(Collision* _This, Collision* _Other);
	void Col_QuestExit(Collision* _This, Collision* _Other);

public:
	TownScene();
	~TownScene();
};

