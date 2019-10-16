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
	// 마우스
	My_PTR<GameObject> m_Mouse;
	My_PTR<MouseCursorScript> MouseScript;

	My_PTR<GameObject> m_InGameMouse;
	My_PTR<WorldMouseCursorScript> m_InGameMouseWorldScript;

	My_PTR<RectCol2D> CursorCol;
	My_PTR<RectCol2D> InGameCursorCol;

private:
	My_PTR<GameObject> m_GamePlayer;
	My_PTR<GameObject> m_PlayerCoatArm;		// 코트 팔부분
	My_PTR<GameObject> m_PlayerArm;			// 팔
	My_PTR<GameObject> m_PlayerEye;			// 눈
	My_PTR<GameObject> m_PlayerHair;		// 헤어 오브젝트
	My_PTR<GameObject> m_PlayerHead;		// 머리 (대머리)
	My_PTR<GameObject> m_PlayerCoat;		// 코트 상반신 전체
	My_PTR<GameObject> m_PlayerBody;		// 몸

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
	My_PTR<GameObject> m_StatusBarBackground;		// 기본 배경
	My_PTR<GameObject> m_StatusBarBase;				// 상태 배경
	My_PTR<GameObject> m_StatusQuickSlot;			// 퀵슬롯

	My_PTR<GameObject> m_StatusBarGauge;			// 게이지바
	My_PTR<GameObject> m_GaugeGraduation;
	My_PTR<GameObject> m_HPGray;					// HP
	My_PTR<GameObject> m_MPGray;					// MP
	My_PTR<GameObject> m_ExpGray;					// 경험치

private:
	// 포탈
	My_PTR<GameObject> m_PrePortal;
	My_PTR<GameObject> m_NextPortal;

	My_PTR<RectCol2D> m_PrePortalCol;
	My_PTR<RectCol2D> m_NextPortalCol;

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
	// 돈관리
	My_PTR<GameObject> m_InvenMoneyManagerObject;
	My_PTR<GameObject> m_ShopMeoneyManagerObject;

	My_PTR<MesoManager> m_InvenMoenyManagerScript;

private:
	// 퀵슬롯 ->   퀵슬롯 배경(맨뒤에있는 이미지) -> 스킬 or 물약 이미지 -> 퀵슬롯 쿨타임 -> 퀵슬롯 배경이미지 (맨앞)
	My_PTR<GameObject> m_QuickSlotEmptyObj;			// 부모 슬롯
	My_PTR<GameObject> m_QuickSlotBGBack;			// 퀵슬롯 배경 (맨 뒤에있는 이미지)
	My_PTR<GameObject> m_QuickSlotBGFront;			// 퀵슬롯 배경 ( 맨 앞에 있는 이미지 )

	std::vector<GameObject*> m_QuickSlotVector;		// 퀵슬롯 - 소비템, 스킬 정보를 갖고 있는 벡터 
	std::vector<RectCol2D*> m_QuickSlotCol;


private:
	// 몬스터 포스
	std::vector<ChewChewIslandMonsterPos> m_ChildMannequinMosnterPos;
	std::unordered_map<int, GameObject*> m_ChildMannequinMonsterMap;
	std::vector<GameObject*> m_ChildMannequinMonsterVector;
	std::vector<RectCol2D*> m_ChildMannequinMonsterCol;

	std::vector<ChewChewIslandMonsterPos> m_WhiteSandRabbitMonsterPos;
	std::unordered_map<int, GameObject*> m_WhiteSandRabbitMonsterMap;
	std::vector<GameObject*> m_WhiteRabbitMonsterVector;
	std::vector<RectCol2D*> m_WhiteSandRabbitMonsterCol;

private:
	// 아이템
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
	// 사운드
	void BGMSoundLoading();
	void SoundPlay_BGM(std::wstring _SoundFileName);

	void EffectSoundLoading();
	void SoundPlay_Effect(std::wstring _SoundFileName);

	void VoiceSoundLoading();
	void SoundPlaye_Voice(std::wstring _SoundFileName);

private:
	// 인벤토리
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
	// UI 충돌
	void Col_InventoryEnter(Collision* _This, Collision* _Other);
	void Col_InventoryStay(Collision* _This, Collision* _Other);
	void Col_InventoryExit(Collision* _This, Collision* _Other);

	// 몬스터 충돌
	void Col_MonsterEnter(Collision* _This, Collision* _Other);
	void Col_MonsterStay(Collision* _This, Collision* _Other);
	void Col_MonsterExit(Collision* _This, Collision* _Other);

	// 플레이어와 충돌 - 아이템
	void Col_PlayerEnter(Collision* _This, Collision* _Other);
	void Col_PlayerStay(Collision* _This, Collision* _Other);
	void Col_PlayerExit(Collision* _This, Collision* _Other);

public:
	MonsterStageScene1();
	~MonsterStageScene1();
};

