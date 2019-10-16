#include "TownScene.h"

#include <GameObject.h>
#include <DXHeader.h>
#include <Camera.h>
#include <Animation2D.h>
#include <SpriteRenderer2D.h>

// 내꺼
#include <PlayerMove.h>
#include <PlayerAnimation.h>
#include <SkillManager.h>
#include <UIMove.h>

// 몬스터
#include <ChildManneqin.h>
#include <WhiteSandRabbit.h>
#include <TransformSlime.h>
#include <Velaris.h>

#include <PinkbinRegen.h>

// 배경
#include <BG_TownScript.h>

// 인벤토리
#include <Inven_BtAnimation.h>
#include <Inven_BtKind.h>
#include <Inven_Info.h>
#include <ItemCode.h>
#include <Inven_MouseItem.h>
#include <Inven_MoveScript.h>

// 퀘스트
#include <QuestAlarm.h>
#include <QuestAlarmElixirNumberScript.h>

// 퀵슬롯
#include <QuickSlotScript.h>

// 상점
#include <ShopBuyItemBt.h>
#include <ShopSellItemManagerScript.h>
#include <ShopBuyItemNumber.h>

// 돈
#include <MesoManager.h>

// 커서
#include <MouseCursorScript.h>
#include <WorldMouseCursorScript.h>

// NPC
#include <NPC_NONE.h>
#include <NPC_WindSpirit.h>
#include <NPC_RockSpirit.h>
#include <NPC_SmallSpirit.h>
#include <NPC_TreeSpirit.h>
#include <NPC_PotionMerchant.h>
#include <NPC_WarehouseKeeper.h>
#include <TownPortalAniScript.h>

// 날씨
#include <Weather_Snow.h>


//
#include <Texture.h>
#include <Sprite.h>
#include <RectCol2D.h>
#include <GameDebug.h>
#include <InGameCamera.h>


#include<MapleGlobalValue.h>
#include <CoolTimeScript.h>

// 사운드
#include <SoundPlayer.h>

TownScene::TownScene()
	: m_GamePlayer(nullptr), m_Monster(nullptr)
	, m_Background(nullptr), m_MoveTile(nullptr)
	, m_MainCamera(nullptr), m_CamScript(nullptr), m_CameraCom(nullptr)
	, m_StatusBarBackground(nullptr), m_StatusBarBase(nullptr), m_StatusQuickSlot(nullptr)
	, m_StatusBarGauge(nullptr), m_GaugeGraduation(nullptr)
	, m_HPGray(nullptr), m_MPGray(nullptr), m_ExpGray(nullptr)
	, m_WindSpiritNPC(nullptr), m_TreeSpiritNPC(nullptr), m_SmallSpiritNPC(nullptr), m_RockSpiritNPC(nullptr)
	, m_PotionNPC(nullptr), m_WarehouseNPC(nullptr)
	, m_PrePortal(nullptr), m_NextPortal(nullptr)
	, m_InventoryEmptyObj(nullptr), m_InvenMoveObj(nullptr), m_InvenBase(nullptr)
	, m_InvenBtClose(nullptr)
	, m_InvenBtDisassemble(nullptr), m_InvenBtExtract(nullptr), m_InvenBtAppraise(nullptr), m_InvenBtPot(nullptr), m_InvenBtToad(nullptr), m_InvenBtUpgrade(nullptr)
	, m_InvenBtEquip(nullptr), m_InvenBtConsum(nullptr), m_InvenBtEtc(nullptr), m_InvenBtInstall(nullptr), m_InvenBtCash(nullptr)
	, m_Meso(0), m_MaplePoint(0)
	, InvenMoveCol(nullptr), InvenBtCloseCol(nullptr), InvenBtDisassembleCol(nullptr), InvenBtExtractCol(nullptr), InvenBtAppraiseCol(nullptr), InvenBtToadCol(nullptr)
	, InvenBtUpgradeCol(nullptr), InvenBtEquipCol(nullptr), InvenBtComsumCol(nullptr), InvenBtEtcCol(nullptr), InvenBtInstallCol(nullptr), InvenBtCashCol(nullptr)
	, m_Mouse(nullptr), MouseScript(nullptr), m_InvenClickMouseImage(nullptr), m_ClickItemInfo(nullptr)
	, m_ShopBuySelectItem(L""), m_CurQuestSituation(QuestDialogSituation::Dialog_Situation::DIALOG_NONE), m_DialogNPC(QuestDialogSituation::Dialog_NPC::NPC_NONE), m_RockQuestPresent(0)
	, m_bFirstScene(false)
{
}


TownScene::~TownScene()
{
}


// Start~Update
bool TownScene::Loading()
{
	// 사운드
	BGMSoundLoading();
	EffectSoundLoading();
	VoiceSoundLoading();
	//
	BGLoading();
	UILoading();
	PortalLoading();

	// 카메라 로딩
	CreateCamera();
	UIScriptLoading();

	// 충돌관련 - 링크해주기
	CollisionNameLink();

	// NPC 로딩
	NPCLoading();

	// 플레이어 로딩
	PlayerLoading();
	CameraScriptLoading();

	// 몬스터로딩

	// 날씨
	//WeatherLoading();

	// 인벤토리
	InventoryLoading();

	// 퀵슬롯
	QuickSlotLoading();

	// 퀘스트
	QuestLoading();
	QuestAlarmLoading();

	// 상점
	ShopLoading();

	// 마우스
	MouseLoading();

	// 돈 관련
	MoneyLoading();


	// 충돌체
	// 플레이어
	m_PlayerCol = m_GamePlayer->Component_Add<RectCol2D>(L"Player");
	m_PlayerCol->Local_SubScale(Vector4(50.0f, 50.0f, 50.0f));
	m_PlayerCol->Local_SubPosition(Vector4(0.0f, -10.0f, 0.0f));
	m_PlayerCol->PushCollisionEnter(&TownScene::ColPortalEnter, this);
	m_PlayerCol->PushCollisionStay(&TownScene::ColPortalStay, this);
	m_PlayerCol->PushCollisionExit(&TownScene::ColPortalExit, this);
	//ColPlayer->SetColSize(Vector2{ 300.0f, 300.0f });




	// 인벤토리 끄기
	InventoryOff();

	// 상점끄기
	ShopOff();

	// 퀘스트 알리미 끄기
	QuestAlarm_Off();

	// 퀘스트 대화창 끄기
	QuestDialog_AllOff();

	return true;
}

void TownScene::Start()
{
	SoundPlay_BGM(L"journey.mp3");

	if (false == m_bFirstScene)
	{
		m_bFirstScene = true;
	}
	else
	{
		for (size_t i = 0; i < MapleGlobalValue::InvenItemConsum.size(); ++i)
		{
			ItemCode::ItemCodeName NewInvenItemCode = MapleGlobalValue::InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCode();
			int NewCount = MapleGlobalValue::InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCount();

			m_InvenItemConsum[i]->GetComponent<Inven_Info>()->ItemAdd(NewInvenItemCode);
			m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(NewCount);
		}

		for (size_t i = 0; i < MapleGlobalValue::QuickSlotVector.size(); ++i)
		{
			ItemCode::ItemCodeName NewQuickSlotItemCode = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();
			float NewCoolTime = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTime();
			std::wstring NewSkilName = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillName();
			int NewCount = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCount();


			m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(NewQuickSlotItemCode);
			m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(NewCoolTime);
			m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(NewSkilName);

			SkillManager* NewSkillManagerScript = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillManager();
			if (nullptr != NewSkillManagerScript)
			{
				// 널이아니면 현재 캐릭터의 스킬매니저를 넣어준다.
				m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(m_SkillManager);

				// 그리고 스킬이 현재 쿨타임인지 알아야한다. (쿨타임 체크) -> 똑같이 해주기
				CoolTimeScript* NewCoolTimeScirpt = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTimeScript();
				std::wstring CurSkillCoolAniName = NewCoolTimeScirpt->GetCurAnimationName();

				if (L"Cool" == CurSkillCoolAniName)
				{
					// 쿨타임 갱신
					float NewCurCoolTime = NewCoolTimeScirpt->GetCurCoolTime();
					int NewCurAniFrame = NewCoolTimeScirpt->GetCurCoolTimeFrame();

					CoolTimeScript* NewCoolTimeScript = m_QuickSlotVector[i]->
						GetComponent<QuickSlotScript>()->GetCoolTimeScript();

					NewCoolTimeScript->SetCoolTimeChange(NewCoolTime);
					NewCoolTimeScript->ChangeCoolTimeAni();
					NewCoolTimeScript->SetCoolTimeAnimationCurTime(NewCurCoolTime);
					NewCoolTimeScript->SetCoolTimeAnimationFrame(NewCurAniFrame);
				}

			}
			else
			{
				// null이라면 현재 스킬매니저에다가 nullptr을 넣어준다
				m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);

				// 소비템인지도 확인하기
				GameObject* NewInventory = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetInventoryItem();
				if (nullptr != NewInventory)
				{
					// 소비템임 
					// 소비템일 경우 인벤토리도 검사해야한다.
					// 아이템 코드명이 일치해야하고 개수도 맞아야한다.
					for (size_t k = 0; k < m_InvenItemConsum.size(); ++k)
					{
						ItemCode::ItemCodeName InventoryItemCode = m_InvenItemConsum[k]->GetComponent<Inven_Info>()->GetItemCode();
						int InvenCount = m_InvenItemConsum[k]->GetComponent<Inven_Info>()->GetItemCount();

						if (InventoryItemCode == NewQuickSlotItemCode && InvenCount == NewCount)
						{
							m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(m_InvenItemConsum[k]);
						}

					}
				}
				else
				{
					// 그냥 빈칸임
					m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
				}

			}
		}
	}
}

void TownScene::Update()
{
	// 인벤토리 켜기
	if (true == Input::Down(L"Inventory"))
	{
		if (false == m_InventoryEmptyObj->IsUpdate())
		{
			InventoryOn();
		}
	}

	if (true == m_RockQuestAlarm->IsUpdate())
	{
		if (1 == m_RockQuestPresent && true == m_QuestAlarmManaElixirNumbersScript->GetQuestComp())
		{
			RockQuestAlarmScript->ChangeAnimation(L"Quest_Completion");
		}
	}
}

void TownScene::LateUpdate()
{
}

void TownScene::End()
{

}

////////////////////////////////////////////////////////////////////// 로딩관련

void TownScene::BGLoading()
{
	// 배경
	m_Background = GetScene()->GameObject_Create(L"Arcana_Bacground");
	m_Background->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 300.0f));
	m_Background->GetTransform()->Local_Scale(Vector4(3326.0f, 880.0f, 1.0f));
	m_Background->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BACKGROUND)->SetSprite(L"ArcanaBGAni_1.png", 0);
	My_PTR<Animation2D> BGANi = m_Background->Component_Add<Animation2D>();
	My_PTR<BG_TownScript> BGScript = m_Background->Component_Add<BG_TownScript>();

	// 충돌할 타일
	m_MoveTile = GetScene()->GameObject_Create(L"Arcana_MoveTile");
	m_MoveTile->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 400.0f));
	m_MoveTile->GetTransform()->Local_Scale(Vector4(3326.0f, 880.0f, 1.0f));
	m_MoveTile->Component_Add<SpriteRenderer2D>(RENDER_ORDER::COLTILE)->SetSprite(L"ArcanaTile.png", 0);
}

void TownScene::UILoading()
{
	m_StatusBarBackground = GetScene()->GameObject_Create(L"StatusBackground");
	m_StatusBarBackground->GetTransform()->Local_Position(Vector4(0.0f, -349.0f, -2.0f));
	m_StatusBarBackground->GetTransform()->Local_Scale(Vector4(1024.0f, 71.0f, 1.0f));
	m_StatusBarBackground->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_BG)->SetSprite(L"StatusBarBaseBackground.png", 0);

	m_StatusBarBase = GetScene()->GameObject_Create(L"StatusBaseBackground");
	m_StatusBarBase->GetTransform()->Local_Position(Vector4(-225.0f, -349.0f, -4.0f));
	m_StatusBarBase->GetTransform()->Local_Scale(Vector4(570.0f, 71.0f, 1.0f));
	m_StatusBarBase->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_BG)->SetSprite(L"StatusBarBaseBackgrnd2.png", 0);

	m_StatusBarGauge = GetScene()->GameObject_Create(L"StatusBarGauge");
	m_StatusBarGauge->GetTransform()->Local_Position(Vector4(-124.0f, -366.0f, -6.0f));
	m_StatusBarGauge->GetTransform()->Local_Scale(Vector4(340.0f, 31.0f, 1.0f));
	m_StatusBarGauge->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeBar.png", 0);

	m_HPGray = GetScene()->GameObject_Create(L"HPGray");
	m_HPGray->GetTransform()->Local_Position(Vector4(-185.0f, -373.0f, -8.0f));
	m_HPGray->GetTransform()->Local_Scale(Vector4(1.0f, 16.0f, 1.0f));
	m_HPGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGray.png", 0);

	m_MPGray = GetScene()->GameObject_Create(L"MPGray");
	m_MPGray->GetTransform()->Local_Position(Vector4(-77.0f, -373.0f, -8.0f));
	m_MPGray->GetTransform()->Local_Scale(Vector4(1.0f, 16.0f, 1.0f));
	m_MPGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGray.png", 0);

	m_ExpGray = GetScene()->GameObject_Create(L"EXPGray");
	m_ExpGray->GetTransform()->Local_Position(Vector4(44.0f, -373.0f, -8.0f));
	m_ExpGray->GetTransform()->Local_Scale(Vector4(1.0f, 16.0f, 1.0f));
	m_ExpGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGray.png", 0);

	m_GaugeGraduation = GetScene()->GameObject_Create(L"StatusBarGauge_Graduation");
	m_GaugeGraduation->GetTransform()->Local_Position(Vector4(-124.0f, -365.0f, -7.0f));
	m_GaugeGraduation->GetTransform()->Local_Scale(Vector4(340.0f, 31.0f, 1.0f));
	m_GaugeGraduation->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGraduation.png", 0);

}

// UI 스크립트 로딩
void TownScene::UIScriptLoading()
{
	//m_StatusBarBackgroundScript = m_StatusBarBackground->Component_Add<UIMove>();
	//m_StatusBarBackgroundScript->SetCamera(m_MainCamera);
	//m_StatusBarBackgroundScript->SetBasePos(m_StatusBarBackground->GetTransform()->Local_Position());

	//m_StatusBarBaseScript = m_StatusBarBase->Component_Add<UIMove>();
	//m_StatusBarBaseScript->SetCamera(m_MainCamera);
	//m_StatusBarBaseScript->SetBasePos(m_StatusBarBase->GetTransform()->Local_Position());

	//m_StatusQuickSlotScript = m_StatusQuickSlot->Component_Add<UIMove>();
	//m_StatusQuickSlotScript->SetCamera(m_MainCamera);
	//m_StatusQuickSlotScript->SetBasePos(m_StatusQuickSlot->GetTransform()->Local_Position());

	//m_GaugeGraduationScript = m_GaugeGraduation->Component_Add<UIMove>();
	//m_GaugeGraduationScript->SetCamera(m_MainCamera);
	//m_GaugeGraduationScript->SetBasePos(m_GaugeGraduation->GetTransform()->Local_Position());

	//m_StatusBarGaugeScript = m_StatusBarGauge->Component_Add<UIMove>();
	//m_StatusBarGaugeScript->SetCamera(m_MainCamera);
	//m_StatusBarGaugeScript->SetBasePos(m_StatusBarGauge->GetTransform()->Local_Position());

	//m_HPGrayScript = m_HPGray->Component_Add<UIMove>();
	//m_HPGrayScript->SetCamera(m_MainCamera);
	//m_HPGrayScript->SetBasePos(m_HPGray->GetTransform()->Local_Position());

	//m_MPGrayScript = m_MPGray->Component_Add<UIMove>();
	//m_MPGrayScript->SetCamera(m_MainCamera);
	//m_MPGrayScript->SetBasePos(m_MPGray->GetTransform()->Local_Position());

	//m_ExpGrayScript = m_ExpGray->Component_Add<UIMove>();
	//m_ExpGrayScript->SetCamera(m_MainCamera);
	//m_ExpGrayScript->SetBasePos(m_ExpGray->GetTransform()->Local_Position());

}

// 카메라 생성
void TownScene::CreateCamera()
{
	// 메인 카메라
	m_MainCamera = GetScene()->GameObject_Create(L"MainCamera");

	m_CameraCom = m_MainCamera->Component_Add<Camera>(0);
	m_CameraCom->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133);
	m_CameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));

	// UI 카메라
	m_UICamera = GetScene()->GameObject_Create(L"UICamera");
	m_UICameraCom = m_UICamera->Component_Add<Camera>(1);
	m_UICameraCom->SettingViewGroup(50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65);
	m_UICameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -20.0f));
}


void TownScene::PlayerLoading()
{
	// 캐릭터 부모
	m_GamePlayer = GetScene()->GameObject_Create(L"Player");
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_GamePlayer->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 1.0f));

	m_PlayerMoveScript = m_GamePlayer->Component_Add<PlayerMove>();
	m_PlayerMoveScript->AddFloor(m_MoveTile);
	m_PlayerMoveScript->SetSoundPlayer(m_EffectSoundPlayer);
	//m_GamePlayer->GetComponent<PlayerMove>()->AddFloor(m_MoveTile);
	m_SkillManager = m_GamePlayer->Component_Add<SkillManager>();
	m_SkillManager->SetPlayerScript(m_PlayerMoveScript->GetPlayerMoveScript());
	m_SkillManager->SetMapWidth(m_Background->GetTransform()->Local_Scale().x);
	m_GamePlayer->DebugMode_On();


	CharacterChildInit(m_PlayerCoatArm, L"Player_CoatArm", L"CoatArm.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 1.0f);		// 코트 팔
	CharacterChildInit(m_PlayerArm, L"Player_Arm", L"Arm.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 2.0f);					// 팔
	CharacterChildInit(m_PlayerArm, L"Player_Weapon", L"Weapon_Rabbit.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 3.0f);		// 무기
	CharacterChildInit(m_PlayerArm, L"Player_Eye", L"Eye.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 4.0f);					// 눈
	CharacterChildInit(m_PlayerArm, L"Playe_Hair", L"Hair.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 5.0f);					// 헤어
	CharacterChildInit(m_PlayerArm, L"Player_Head", L"Head.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 6.0f);					// 머리
	CharacterChildInit(m_PlayerArm, L"Player_Coat", L"Coat.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 7.0f);					// 코트옷
	CharacterChildInit(m_PlayerArm, L"Player_Body", L"Body.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 8.0f);					// 바디


	// 룰루 내일하장~!!

	//My_PTR<GameObject> dddd = GetScene()->GameObject_Create(L"ddddddddd");
	//dddd->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -9.0f, 1.0f));
	//dddd->GetTransform()->Local_Scale(Vector4(400.0f, 400.0f, 1.0f, 1.0f));
	//My_PTR<SpriteRenderer2D> daaaaaa = dddd->Component_Add<SpriteRenderer2D>(RENDER_ORDER::PLAYER_CHILD);
	//daaaaaa->SetSprite(L"SmallSpiritFace.png", 0);
	//Scene::OverGameObjectInfo_Create(dddd);
	//Scene::PushOverInfo(dddd, L"EsferaScene");
}

void TownScene::CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z)
{
	_GameObject = GetScene()->GameObject_Create(_ObjName);
	_GameObject->SetParent(_Parent);
	_GameObject->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, _Z));
	_GameObject->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f));

	_GameObject->Component_Add<SpriteRenderer2D>(_RenderOrder)->SetSprite(_ImageName, 0);
	_GameObject->Component_Add<Animation2D>();
	_GameObject->Component_Add<PlayerAnimation>();
	_GameObject->GetComponent<PlayerAnimation>()->SetPaleyrMoveScript(_Parent->GetComponent<PlayerMove>()->GetPlayerScript());
	_GameObject->DebugMode_On();
}


void TownScene::CameraScriptLoading()
{
	m_CamScript = m_MainCamera->Component_Add<InGameCamera>();
	m_CamScript->SetCurMap(m_Background);
	m_CamScript->SetPlayer(m_GamePlayer);
	m_CamScript->SetSceneXSize(25.0f, 3235.0f);
}

void TownScene::NPCLoading()
{
	m_WindSpiritNPC = GetScene()->GameObject_Create(L"NPC_WindSpirit");
	m_WindSpiritNPC->GetTransform()->Local_Position(Vector4(-270.0f, -220.0f, 200.0f, 1.0f));
	m_WindSpiritNPC->GetTransform()->Local_Scale(Vector4(199.0f, 177.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> WindRenderer = m_WindSpiritNPC->Component_Add<SpriteRenderer2D>(RenderOrder::NPC);
	WindRenderer->SetSprite(L"WindSpiritBaseMotion.png", 0);
	My_PTR<Animation2D> WindAni = m_WindSpiritNPC->Component_Add<Animation2D>();
	My_PTR<NPC_WindSpirit> WindScript = m_WindSpiritNPC->Component_Add<NPC_WindSpirit>();
	WindScript->SetDir(L"Right");
	My_PTR<RectCol2D> WindSpiritNPCCOl = m_WindSpiritNPC->Component_Add<RectCol2D>(L"Col_WindSpiritNPC");
	WindSpiritNPCCOl->Local_SubPosition(Vector4(0.0f, 0.0f, -1.0f, 1.0f));
	WindSpiritNPCCOl->Local_SubScale(Vector4(80.0f, 100.0f, 1.0f, 1.0f));
	m_ColNPCVec.push_back(WindSpiritNPCCOl);

	m_RockSpiritNPC = GetScene()->GameObject_Create(L"NPC_RockSpirit");
	m_RockSpiritNPC->GetTransform()->Local_Position(Vector4(300.0f, -210.0f, 200.0f, 1.0f));
	m_RockSpiritNPC->GetTransform()->Local_Scale(Vector4(320.0f, 358.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> RockRenderer = m_RockSpiritNPC->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	RockRenderer->SetSprite(L"RockSpiritBaseMotion.png", 0);
	My_PTR<Animation2D> RockAni = m_RockSpiritNPC->Component_Add<Animation2D>();
	My_PTR<NPC_RockSpirit> RockScript = m_RockSpiritNPC->Component_Add<NPC_RockSpirit>();
	RockScript->SetDir(L"Left");
	My_PTR<RectCol2D> RockSPiritNPCCol = m_RockSpiritNPC->Component_Add<RectCol2D>(L"Col_RockSpiritNPC");
	RockSPiritNPCCol->Local_SubPosition(Vector4(0.0f, 0.0f, -1.0f, 0.0f));
	RockSPiritNPCCol->Local_SubScale(Vector4(170.0f, 100.0f, 1.0f, 1.0f));
	m_ColNPCVec.push_back(RockSPiritNPCCol);

	m_TreeSpiritNPC = GetScene()->GameObject_Create(L"NPC_TreeSpirit");
	m_TreeSpiritNPC->GetTransform()->Local_Position(Vector4(-650.0f, -210.0f, 200.0f, 1.0f));
	m_TreeSpiritNPC->GetTransform()->Local_Scale(Vector4(201.0f, 123.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> TreeRenderer = m_TreeSpiritNPC->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	TreeRenderer->SetSprite(L"TreeSpiritMotion.png", 0);
	My_PTR<Animation2D> TreeAni = m_TreeSpiritNPC->Component_Add<Animation2D>();
	My_PTR<NPC_TreeSpirit> TreeScript = m_TreeSpiritNPC->Component_Add<NPC_TreeSpirit>();
	TreeScript->SetDir(L"Right");
	My_PTR<RectCol2D> TreeSPiritNPCCol = m_TreeSpiritNPC->Component_Add<RectCol2D>(L"Col_TreeSpiritNPC");
	TreeSPiritNPCCol->Local_SubPosition(Vector4(0.0f, 0.0f, -1.0f, 0.0f));
	TreeSPiritNPCCol->Local_SubScale(Vector4(200.0f, 100.0f, 1.0f, 1.0f));
	m_ColNPCVec.push_back(TreeSPiritNPCCol);

	m_SmallSpiritNPC = GetScene()->GameObject_Create(L"NPC_SmallSpirit");
	m_SmallSpiritNPC->GetTransform()->Local_Position(Vector4(100.0f, -240.0f, 200.0f, 1.0f));
	m_SmallSpiritNPC->GetTransform()->Local_Scale(Vector4(335.0f, 334.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SmallRenderer = m_SmallSpiritNPC->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	SmallRenderer->SetSprite(L"SmallSpiritBaseMotion.png", 0);
	My_PTR<Animation2D> SmallAni = m_SmallSpiritNPC->Component_Add<Animation2D>();
	My_PTR<NPC_SmallSpirit> SmallScript = m_SmallSpiritNPC->Component_Add<NPC_SmallSpirit>();
	SmallScript->SetDir(L"Left");
	My_PTR<RectCol2D> SmallSpiritNPCCol = m_SmallSpiritNPC->Component_Add<RectCol2D>(L"Col_SmallSpiritNPC");
	SmallSpiritNPCCol->Local_SubPosition(Vector4(0.0f, 0.0f, -1.0f, 0.0f));
	SmallSpiritNPCCol->Local_SubScale(Vector4(80.0f, 80.0f, 1.0f, 1.0f));
	m_ColNPCVec.push_back(SmallSpiritNPCCol);

	// 상인

	m_PotionNPC = GetScene()->GameObject_Create(L"NPC_Potion");
	m_PotionNPC->GetTransform()->Local_Position(Vector4(700.0f, -225.0f, 200.0f, 1.0f));
	m_PotionNPC->GetTransform()->Local_Scale(Vector4(164.0f, 159.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PotionRenderer = m_PotionNPC->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	PotionRenderer->SetSprite(L"PotionMerchant.png", 0);
	My_PTR<Animation2D> PotionAni = m_PotionNPC->Component_Add<Animation2D>();
	My_PTR<NPC_PotionMerchant> PotionScript = m_PotionNPC->Component_Add<NPC_PotionMerchant>();
	PotionScript->SetDir(L"Right");

	My_PTR<RectCol2D> PotionNPCCol = m_PotionNPC->Component_Add<RectCol2D>(L"Col_PotionNPC");
	PotionNPCCol->Local_SubPosition(Vector4(0.0f, 0.0f, -1.0f, 1.0f));
	PotionNPCCol->Local_SubScale(Vector4(80.0f, 100.0f, 1.0f, 1.0f));
	m_ColNPCVec.push_back(PotionNPCCol);


	m_WarehouseNPC = GetScene()->GameObject_Create(L"NPC_Warehouse");
	m_WarehouseNPC->GetTransform()->Local_Position(Vector4(850.0f, -220.0f, 200.0f, 1.0f));
	m_WarehouseNPC->GetTransform()->Local_Scale(Vector4(162.0f, 159.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> WarehouseRenderer = m_WarehouseNPC->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	WarehouseRenderer->SetSprite(L"WarehouseKeeper.png", 0);
	My_PTR<Animation2D> WarehouseAni = m_WarehouseNPC->Component_Add<Animation2D>();
	My_PTR<NPC_WarehouseKeeper> WarehouseScript = m_WarehouseNPC->Component_Add<NPC_WarehouseKeeper>();
	WarehouseScript->SetDir(L"Left");

	My_PTR<RectCol2D> WarehouseNPCCol = m_WarehouseNPC->Component_Add<RectCol2D>(L"Col_WarehouseNPC");
	WarehouseNPCCol->Local_SubPosition(Vector4(0.0f, 0.0f, -1.0f, 1.0f));
	WarehouseNPCCol->Local_SubScale(Vector4(80.0f, 100.0f, 1.0f, 1.0f));
	m_ColNPCVec.push_back(WarehouseNPCCol);

	// 이름 태그
	m_PotionNPCNameTag = GetScene()->GameObject_Create(L"NPCTag_Potion");
	m_PotionNPCNameTag->GetTransform()->Local_Position(Vector4(700.0f, -270.0f, 10.0f, 1.0f));
	m_PotionNPCNameTag->GetTransform()->Local_Scale(Vector4(57.0f, 17.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PoitionNPCNameTagRenderer = m_PotionNPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	PoitionNPCNameTagRenderer->SetSprite(L"Tag_PotionNPC.png", 0);

	m_WarehouseNPCNameTag = GetScene()->GameObject_Create(L"NPCTag_Warehouse");
	m_WarehouseNPCNameTag->GetTransform()->Local_Position(Vector4(850.0f, -270.0f, 10.0f, 1.0f));
	m_WarehouseNPCNameTag->GetTransform()->Local_Scale(Vector4(56.0f, 17.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> WarehouseNPCNameTagRenderer = m_WarehouseNPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	WarehouseNPCNameTagRenderer->SetSprite(L"Tag_WarehouseNPC.png", 0);

	m_WindSpiritNPCNameTag = GetScene()->GameObject_Create(L"NPCTag_WindSpirit");
	m_WindSpiritNPCNameTag->GetTransform()->Local_Position(Vector4(-270.0f, -270.0f, 10.0f, 1.0f));
	m_WindSpiritNPCNameTag->GetTransform()->Local_Scale(Vector4(68.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> WindSpiritNPCNameTagRenderer = m_WindSpiritNPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	WindSpiritNPCNameTagRenderer->SetSprite(L"Tag_WindSpirit.png", 0);

	m_TreeSpiritNPCNameTag = GetScene()->GameObject_Create(L"NPCTag_TreeSpirit");
	m_TreeSpiritNPCNameTag->GetTransform()->Local_Position(Vector4(-650.0f, -270.0f, 10.0f, 1.0f));
	m_TreeSpiritNPCNameTag->GetTransform()->Local_Scale(Vector4(80.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> TreeSpiritNPCNameTagRenderer = m_TreeSpiritNPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	TreeSpiritNPCNameTagRenderer->SetSprite(L"Tag_TreeSpirits.png", 0);

	m_SmallSpiritNPCNameTag = GetScene()->GameObject_Create(L"NPCTag_SmallSpirit");
	m_SmallSpiritNPCNameTag->GetTransform()->Local_Position(Vector4(100.0f, -270.0f, 10.0f, 1.0f));
	m_SmallSpiritNPCNameTag->GetTransform()->Local_Scale(Vector4(68.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SmallSpiritNPCNameTagRenderer = m_SmallSpiritNPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	SmallSpiritNPCNameTagRenderer->SetSprite(L"Tag_SmallSpirit.png", 0);

	m_RockSpiritNPCNameTag = GetScene()->GameObject_Create(L"NPCTag_RockSpirit");
	m_RockSpiritNPCNameTag->GetTransform()->Local_Position(Vector4(300.0f, -270.0f, 10.0f, 1.0f));
	m_RockSpiritNPCNameTag->GetTransform()->Local_Scale(Vector4(55.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> RockSpiritNPCNameTagRenderer = m_RockSpiritNPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::NPC);
	RockSpiritNPCNameTagRenderer->SetSprite(L"Tag_RockSpirit.png", 0);
}


void TownScene::PortalLoading()
{
	m_PrePortal = GetScene()->GameObject_Create(L"PrePortal_Town");
	m_PrePortal->GetTransform()->Local_Position(Vector4(-1200.0f, -145.0f, 10.0f, 1.0f));
	m_PrePortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PrePortalRenderer = m_PrePortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	PrePortalRenderer->SetSprite(L"Portal.png", 0);
	My_PTR<Animation2D> PrePortalAnim = m_PrePortal->Component_Add<Animation2D>();
	My_PTR<TownPortalAniScript> PrePortalAniScript = m_PrePortal->Component_Add<TownPortalAniScript>();
	m_PrePortalCol = m_PrePortal->Component_Add<RectCol2D>(L"Town_PrePortalCol");


	m_NextPortal = GetScene()->GameObject_Create(L"NextPortal_Town");
	m_NextPortal->GetTransform()->Local_Position(Vector4(1300.0f, -145.0f, 10.0f, 1.0f));
	m_NextPortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NextPortalRenderer = m_NextPortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	NextPortalRenderer->SetSprite(L"Portal.png", 0);
	My_PTR<Animation2D> NextPortalAnim = m_NextPortal->Component_Add<Animation2D>();
	My_PTR<TownPortalAniScript> NextPortalAniScript = m_NextPortal->Component_Add<TownPortalAniScript>();
	m_NextPortalCol = m_NextPortal->Component_Add<RectCol2D>(L"Town_NextPortalCol");
}


void TownScene::WeatherLoading()
{
	m_Snow01 = GetScene()->GameObject_Create(L"Town_Snow01");
	m_Snow01->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_Snow01->GetTransform()->Local_Scale(Vector4(1368.0f, 768.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> Snow01Renderer = m_Snow01->Component_Add<SpriteRenderer2D>(RENDER_ORDER::WEATHER_SNOW);
	Snow01Renderer->SetSprite(L"Weather_Ani_1.png", 0);
	My_PTR<Animation2D> Snow01Anim = m_Snow01->Component_Add<Animation2D>();
	My_PTR<Weather_Snow> Snow01Script = m_Snow01->Component_Add<Weather_Snow>();
}

void TownScene::InventoryLoading()
{
	// 기본 오브젝트들
	m_InventoryEmptyObj = GetScene()->GameObject_Create(L"InvenEmtpyObj");
	m_InventoryEmptyObj->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	m_InventoryEmptyObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_InvenMoveObj = GetScene()->GameObject_Create(L"InventoryMoveObj");
	m_InvenMoveObj->SetParent(m_InventoryEmptyObj);
	m_InvenMoveObj->GetTransform()->Local_Position(Vector4(-20.0f, 154.0f, 1.0f, 1.0f));
	m_InvenMoveObj->GetTransform()->Local_Scale(Vector4(130.0f, 21.0f, 1.0f, 1.0f));

	InvenMoveObjectScript = m_InvenMoveObj->Component_Add<Inven_MoveScript>();				// 움직이는 애 스크립트
	InvenMoveObjectScript->SetCamera(m_UICameraCom);
	InvenMoveObjectScript->SetMoveObject(m_InventoryEmptyObj);

	InvenMoveCol = m_InvenMoveObj->Component_Add<RectCol2D>(L"InventoryMoveObjCol");		// 움직이는 애 충돌체
	InvenMoveCol->SetUICamera(m_UICameraCom);

	m_InvenBase = GetScene()->GameObject_Create(L"InvenBaseObj");
	m_InvenBase->SetParent(m_InventoryEmptyObj);
	m_InvenBase->GetTransform()->Local_Position(Vector4(1.0f, 1.0f, -1.0f, 1.0f));
	m_InvenBase->GetTransform()->Local_Scale(Vector4(172.0f, 335.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBaseRenderer = m_InvenBase->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBaseRenderer->SetSprite(L"Inven_Base.png", 0);

	m_InvenBtClose = GetScene()->GameObject_Create(L"InvenBtClose");
	m_InvenBtClose->SetParent(m_InventoryEmptyObj);
	m_InvenBtClose->GetTransform()->Local_Position(Vector4(70.0f, 154.0f, -2.0f, 1.0f));
	m_InvenBtClose->GetTransform()->Local_Scale(Vector4(13.0f, 13.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtCloseRenderer = m_InvenBtClose->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtCloseRenderer->SetSprite(L"Inven_CloseBt.png", 0);
	My_PTR<Animation2D> InvenBtCloseAni = m_InvenBtClose->Component_Add<Animation2D>();
	InvenBtCloseAniScript = m_InvenBtClose->Component_Add<Inven_BtAnimation>();
	InvenBtCloseCol = m_InvenBtClose->Component_Add<RectCol2D>(L"InvenBtCloseCol");
	InvenBtCloseCol->SetUICamera(m_UICameraCom);


	// 하위 버튼들
	m_InvenBtDisassemble = GetScene()->GameObject_Create(L"InvenBtDisassemble");
	m_InvenBtDisassemble->SetParent(m_InventoryEmptyObj);
	m_InvenBtDisassemble->GetTransform()->Local_Position(Vector4(-65.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtDisassemble->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtDisassembleRenderer = m_InvenBtDisassemble->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtDisassembleRenderer->SetSprite(L"Inven_DisassembleBt.png", 0);
	My_PTR<Animation2D> InvenBtDisassembleAni = m_InvenBtDisassemble->Component_Add<Animation2D>();
	InvenBtDisassembleAniScript = m_InvenBtDisassemble->Component_Add<Inven_BtAnimation>();
	InvenBtDisassembleCol = m_InvenBtDisassemble->Component_Add<RectCol2D>(L"InvenBtDisassembleCol");
	InvenBtDisassembleCol->SetUICamera(m_UICameraCom);

	m_InvenBtExtract = GetScene()->GameObject_Create(L"InvenBtExtract");
	m_InvenBtExtract->SetParent(m_InventoryEmptyObj);
	m_InvenBtExtract->GetTransform()->Local_Position(Vector4(-39.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtExtract->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtExtractRenderer = m_InvenBtExtract->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtExtractRenderer->SetSprite(L"Inven_ExtractBt.png", 0);
	My_PTR<Animation2D> InvenBtExtractAni = m_InvenBtExtract->Component_Add<Animation2D>();
	InvenBtExtractAniScript = m_InvenBtExtract->Component_Add<Inven_BtAnimation>();
	InvenBtExtractCol = m_InvenBtExtract->Component_Add<RectCol2D>(L"InvenBtExtractCol");
	InvenBtExtractCol->SetUICamera(m_UICameraCom);

	m_InvenBtAppraise = GetScene()->GameObject_Create(L"InvenBtAppraise");
	m_InvenBtAppraise->SetParent(m_InventoryEmptyObj);
	m_InvenBtAppraise->GetTransform()->Local_Position(Vector4(-13.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtAppraise->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtAppraise = m_InvenBtAppraise->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtAppraise->SetSprite(L"Inven_AppraiseBt.png", 0);
	My_PTR<Animation2D> InvenBtAppraiseAni = m_InvenBtAppraise->Component_Add<Animation2D>();
	InvenBtAppraiseAniScript = m_InvenBtAppraise->Component_Add<Inven_BtAnimation>();
	InvenBtAppraiseCol = m_InvenBtAppraise->Component_Add<RectCol2D>(L"InvenBtAppraiseCol");
	InvenBtAppraiseCol->SetUICamera(m_UICameraCom);

	m_InvenBtPot = GetScene()->GameObject_Create(L"InvenBtPot");
	m_InvenBtPot->SetParent(m_InventoryEmptyObj);
	m_InvenBtPot->GetTransform()->Local_Position(Vector4(13.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtPot->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtPote = m_InvenBtPot->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtPote->SetSprite(L"Inven_PotBt.png", 0);

	m_InvenBtToad = GetScene()->GameObject_Create(L"InvenBtToad");
	m_InvenBtToad->SetParent(m_InventoryEmptyObj);
	m_InvenBtToad->GetTransform()->Local_Position(Vector4(39.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtToad->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtToad = m_InvenBtToad->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtToad->SetSprite(L"Inven_ToadBt.png", 0);
	My_PTR<Animation2D> InvenBtToadAni = m_InvenBtToad->Component_Add<Animation2D>();
	InvenBtToadAniScript = m_InvenBtToad->Component_Add<Inven_BtAnimation>();
	InvenBtToadCol = m_InvenBtToad->Component_Add<RectCol2D>(L"InvenBtToadCol");
	InvenBtToadCol->SetUICamera(m_UICameraCom);

	m_InvenBtUpgrade = GetScene()->GameObject_Create(L"InvenBtUpgrade");
	m_InvenBtUpgrade->SetParent(m_InventoryEmptyObj);
	m_InvenBtUpgrade->GetTransform()->Local_Position(Vector4(65.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtUpgrade->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtUpgrade = m_InvenBtUpgrade->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtUpgrade->SetSprite(L"Inven_UpgradeBt.png", 0);
	My_PTR<Animation2D> InvenBtUpgradeAni = m_InvenBtUpgrade->Component_Add<Animation2D>();
	InvenBtUpgradeAniScript = m_InvenBtUpgrade->Component_Add<Inven_BtAnimation>();
	InvenBtUpgradeCol = m_InvenBtUpgrade->Component_Add<RectCol2D>(L"InvenBtUpgradeCol");
	InvenBtUpgradeCol->SetUICamera(m_UICameraCom);

	// 장비 소비 기타 설치 캐시
	m_InvenBtEquip = GetScene()->GameObject_Create(L"InvenBtEquip");
	m_InvenBtEquip->SetParent(m_InventoryEmptyObj);
	m_InvenBtEquip->GetTransform()->Local_Position(Vector4(-60.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtEquip->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtEquipRenderer = m_InvenBtEquip->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtEquipRenderer->SetSprite(L"Inven_Kind_Equip.png", 0);
	My_PTR<Animation2D> InvenBtEquipAni = m_InvenBtEquip->Component_Add<Animation2D>();
	InvenBtEquipAniScript = m_InvenBtEquip->Component_Add<Inven_BtKind>();
	InvenBtEquipAniScript->On();
	InvenBtEquipCol = m_InvenBtEquip->Component_Add<RectCol2D>(L"InvenBtEquipCol");
	InvenBtEquipCol->SetName(L"InvenBtEquipCol");
	InvenBtEquipCol->SetUICamera(m_UICameraCom);

	m_InvenBtConsum = GetScene()->GameObject_Create(L"InvenBtComsum");
	m_InvenBtConsum->SetParent(m_InventoryEmptyObj);
	m_InvenBtConsum->GetTransform()->Local_Position(Vector4(-29.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtConsum->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtComsumRenderer = m_InvenBtConsum->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtComsumRenderer->SetSprite(L"Inven_Kind_Comsum.png", 0);
	My_PTR<Animation2D> InvenBtComsumAni = m_InvenBtConsum->Component_Add<Animation2D>();
	InvenBtComsumAniScript = m_InvenBtConsum->Component_Add<Inven_BtKind>();
	InvenBtComsumAniScript->Off();
	InvenBtComsumCol = m_InvenBtConsum->Component_Add<RectCol2D>(L"InvenBtComsumCol");
	InvenBtComsumCol->SetUICamera(m_UICameraCom);

	m_InvenBtEtc = GetScene()->GameObject_Create(L"InvenBtEtc");
	m_InvenBtEtc->SetParent(m_InventoryEmptyObj);
	m_InvenBtEtc->GetTransform()->Local_Position(Vector4(2.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtEtc->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtEtcRenderer = m_InvenBtEtc->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtEtcRenderer->SetSprite(L"Inven_Kind_Etc.png", 0);
	My_PTR<Animation2D> InvenBtEtcAni = m_InvenBtEtc->Component_Add<Animation2D>();
	InvenBtEtcAniScript = m_InvenBtEtc->Component_Add<Inven_BtKind>();
	InvenBtEtcAniScript->Off();
	InvenBtEtcCol = m_InvenBtEtc->Component_Add<RectCol2D>(L"InvenBtEtcCol");
	InvenBtEtcCol->SetUICamera(m_UICameraCom);

	m_InvenBtInstall = GetScene()->GameObject_Create(L"InvenBtInstall");
	m_InvenBtInstall->SetParent(m_InventoryEmptyObj);
	m_InvenBtInstall->GetTransform()->Local_Position(Vector4(33.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtInstall->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtInstallRenderer = m_InvenBtInstall->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtInstallRenderer->SetSprite(L"Inven_Kind_Install.png", 0);
	My_PTR<Animation2D> InvenBtInstallAni = m_InvenBtInstall->Component_Add<Animation2D>();
	InvenBtInstallAniScript = m_InvenBtInstall->Component_Add<Inven_BtKind>();
	InvenBtInstallAniScript->Off();
	InvenBtInstallCol = m_InvenBtInstall->Component_Add<RectCol2D>(L"InvenBtInstallCol");
	InvenBtInstallCol->SetUICamera(m_UICameraCom);

	m_InvenBtCash = GetScene()->GameObject_Create(L"InvenBtCash");
	m_InvenBtCash->SetParent(m_InventoryEmptyObj);
	m_InvenBtCash->GetTransform()->Local_Position(Vector4(64.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtCash->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtCashRenderer = m_InvenBtCash->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtCashRenderer->SetSprite(L"Inven_Kind_Cash.png", 0);
	My_PTR<Animation2D> InvenBtCashAni = m_InvenBtCash->Component_Add<Animation2D>();
	InvenBtCashAniScript = m_InvenBtCash->Component_Add<Inven_BtKind>();
	InvenBtCashAniScript->Off();
	InvenBtCashCol = m_InvenBtCash->Component_Add<RectCol2D>(L"InvenBtCashCol");
	InvenBtCashCol->SetUICamera(m_UICameraCom);

	// 충돌체 묶기
	// 기본
	m_InventoryButtonColVector.push_back(InvenMoveCol);
	m_InventoryButtonColVector.push_back(InvenBtCloseCol);
	m_InventoryButtonColVector.push_back(InvenBtDisassembleCol);
	m_InventoryButtonColVector.push_back(InvenBtExtractCol);
	m_InventoryButtonColVector.push_back(InvenBtAppraiseCol);
	m_InventoryButtonColVector.push_back(InvenBtToadCol);
	m_InventoryButtonColVector.push_back(InvenBtUpgradeCol);
	m_InventoryButtonColVector.push_back(InvenBtEquipCol);
	m_InventoryButtonColVector.push_back(InvenBtComsumCol);
	m_InventoryButtonColVector.push_back(InvenBtEtcCol);
	m_InventoryButtonColVector.push_back(InvenBtInstallCol);
	m_InventoryButtonColVector.push_back(InvenBtCashCol);


	InventoryCreate();

}

void TownScene::InventoryCreate()
{
	InventorySlotCreate();

	// 총 모음
	m_AllItems.push_back(m_InvenItemEquip);
	m_AllItems.push_back(m_InvenItemConsum);
	m_AllItems.push_back(m_InvenItemEtc);
	m_AllItems.push_back(m_InvenItemInstall);
	m_AllItems.push_back(m_InvenItemCash);

	// 사용하는 인벤토리 슬롯 온오프
	InventorySlotOn(m_InvenItemEquip);
	InventorySlotOff(m_InvenItemConsum);
	InventorySlotOff(m_InvenItemEtc);
	InventorySlotOff(m_InvenItemInstall);
	InventorySlotOff(m_InvenItemCash);
}

void TownScene::InventorySlotCreate()
{
	//// 장비
	//for (size_t y = 0; y < 6; y++)
	//{
	//	for (size_t x = 0; x < 4; x++)
	//	{
	//		Vector4 EmptyItemPos = Vector4(-59.0f, 100.5f, -7.0f, 1.0f);
	//		EmptyItemPos.x = -59.0f + (36.0f * x);
	//		EmptyItemPos.y = 100.5f - (35.0f * y);


	//		My_PTR<GameObject> EmptyItem = GetScene()->GameObject_Create(L"Item");
	//		EmptyItem->SetParent(m_InventoryEmptyObj);
	//		EmptyItem->GetTransform()->Local_Position(EmptyItemPos);
	//		EmptyItem->GetTransform()->Local_Scale(Vector4(30.0f, 31.0f, 1.0f, 1.0f));
	//		My_PTR<SpriteRenderer2D> EmptyItemRenderer = EmptyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
	//		EmptyItemRenderer->SetSprite(L"Inven_ExampleTex01.png", 0);
	//		My_PTR<Animation2D> EmptyItemAni = EmptyItem->Component_Add<Animation2D>();
	//		My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"InventorySlot_Equip");
	//		EmptyItemCol->SetUICamera(m_UICameraCom);
	//		My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
	//		EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::EQUIP);

	//		m_InvenItemEquip.push_back(EmptyItem);
	//		InvenItemEquipCol.push_back(EmptyItemCol);
	//	}
	//}

	// 소비
	for (size_t y = 0; y < 6; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			Vector4 EmptyItemPos = Vector4(-59.0f, 100.5f, -7.0f, 1.0f);
			EmptyItemPos.x = -59.0f + (36.0f * x);
			EmptyItemPos.y = 100.5f - (35.0f * y);


			My_PTR<GameObject> EmptyItem = GetScene()->GameObject_Create(L"Item");
			EmptyItem->SetParent(m_InventoryEmptyObj);
			EmptyItem->GetTransform()->Local_Position(EmptyItemPos);
			EmptyItem->GetTransform()->Local_Scale(Vector4(30.0f, 31.0f, 1.0f, 1.0f));
			My_PTR<SpriteRenderer2D> EmptyItemRenderer = EmptyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
			EmptyItemRenderer->SetSprite(L"EmptyItem.png", 0);
			My_PTR<Animation2D> EmptyItemAni = EmptyItem->Component_Add<Animation2D>();
			My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"InventorySlot_Consume");
			EmptyItemCol->SetUICamera(m_UICameraCom);
			My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
			EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::CONSUME);

			//if (x < 2)
			//{
			//	EmptyItemScript->ItemAdd(ItemCode::ItemCodeName::POTION_RED);
			//	EmptyItemScript->SetItemCount(150);
			//}

			m_InvenItemConsum.push_back(EmptyItem);
			InvenItemConSumCol.push_back(EmptyItemCol);
		}
	}

	//// 기타
	//for (size_t y = 0; y < 6; y++)
	//{
	//	for (size_t x = 0; x < 4; x++)
	//	{
	//		Vector4 EmptyItemPos = Vector4(-59.0f, 100.5f, -7.0f, 1.0f);
	//		EmptyItemPos.x = -59.0f + (36.0f * x);
	//		EmptyItemPos.y = 100.5f - (35.0f * y);

	//		My_PTR<GameObject> EmptyItem = GetScene()->GameObject_Create(L"Item");
	//		EmptyItem->SetParent(m_InventoryEmptyObj);
	//		EmptyItem->GetTransform()->Local_Position(EmptyItemPos);
	//		EmptyItem->GetTransform()->Local_Scale(Vector4(30.0f, 31.0f, 1.0f, 1.0f));
	//		My_PTR<SpriteRenderer2D> EmptyItemRenderer = EmptyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
	//		EmptyItemRenderer->SetSprite(L"Inven_ExampleTex01.png", 0);
	//		My_PTR<Animation2D> EmptyItemAni = EmptyItem->Component_Add<Animation2D>();
	//		My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"InventorySlot_Etc");
	//		EmptyItemCol->SetUICamera(m_UICameraCom);
	//		My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
	//		EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::ETC);

	//		m_InvenItemEtc.push_back(EmptyItem);
	//		InvenItemEtcCol.push_back(EmptyItemCol);
	//	}
	//}

	//// 설치
	//for (size_t y = 0; y < 6; y++)
	//{
	//	for (size_t x = 0; x < 4; x++)
	//	{
	//		Vector4 EmptyItemPos = Vector4(-59.0f, 100.5f, -7.0f, 1.0f);
	//		EmptyItemPos.x = -59.0f + (36.0f * x);
	//		EmptyItemPos.y = 100.5f - (35.0f * y);


	//		My_PTR<GameObject> EmptyItem = GetScene()->GameObject_Create(L"Item");
	//		EmptyItem->SetParent(m_InventoryEmptyObj);
	//		EmptyItem->GetTransform()->Local_Position(EmptyItemPos);
	//		EmptyItem->GetTransform()->Local_Scale(Vector4(30.0f, 31.0f, 1.0f, 1.0f));
	//		My_PTR<SpriteRenderer2D> EmptyItemRenderer = EmptyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
	//		EmptyItemRenderer->SetSprite(L"Inven_ExampleTex02.png", 0);
	//		My_PTR<Animation2D> EmptyItemAni = EmptyItem->Component_Add<Animation2D>();
	//		My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"InventorySlot_Install");
	//		EmptyItemCol->SetUICamera(m_UICameraCom);
	//		My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
	//		EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::INSTALL);
	//		m_InvenItemInstall.push_back(EmptyItem);
	//		InvenItemInstallCol.push_back(EmptyItemCol);
	//	}
	//}

	//// 캐시
	//for (size_t y = 0; y < 6; y++)
	//{
	//	for (size_t x = 0; x < 4; x++)
	//	{
	//		Vector4 EmptyItemPos = Vector4(-59.0f, 100.5f, -7.0f, 1.0f);
	//		EmptyItemPos.x = -59.0f + (36.0f * x);
	//		EmptyItemPos.y = 100.5f - (35.0f * y);


	//		My_PTR<GameObject> EmptyItem = GetScene()->GameObject_Create(L"Item");
	//		EmptyItem->SetParent(m_InventoryEmptyObj);
	//		EmptyItem->GetTransform()->Local_Position(EmptyItemPos);
	//		EmptyItem->GetTransform()->Local_Scale(Vector4(30.0f, 31.0f, 1.0f, 1.0f));
	//		My_PTR<SpriteRenderer2D> EmptyItemRenderer = EmptyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
	//		EmptyItemRenderer->SetSprite(L"Inven_ExampleTex01.png", 0);
	//		My_PTR<Animation2D> EmptyItemAni = EmptyItem->Component_Add<Animation2D>();
	//		My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"InventorySlot_Cash");
	//		EmptyItemCol->SetUICamera(m_UICameraCom);
	//		My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
	//		EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::CASH);
	//		m_InvenItemCash.push_back(EmptyItem);
	//		InvenItemCashCol.push_back(EmptyItemCol);
	//	}
	//}
}

void TownScene::MouseLoading() 
{
	Input::Create_Key(L"MouseLeftButton", VK_LBUTTON);


	m_Mouse = GetScene()->GameObject_Create(L"MouseCursor");
	m_Mouse->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -15.0f, 1.0f));
	m_Mouse->GetTransform()->Local_Scale(Vector4(30.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> MouseCursorRenderer = m_Mouse->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_MOUSECURSOR);
	MouseCursorRenderer->SetSprite(L"MouseCursor.png", 0);
	My_PTR<Animation2D> MouseCursorAni = m_Mouse->Component_Add<Animation2D>();
	MouseScript = m_Mouse->Component_Add<MouseCursorScript>();
	MouseScript->SetCamera(m_UICameraCom);

	CursorCol = m_Mouse->Component_Add<RectCol2D>(L"CursorCol");
	CursorCol->Local_SubPosition(Vector4(-10.0f, 10.0f, 1.0f, 1.0f));
	CursorCol->Local_SubScale(Vector4(3.0f, 3.0f, 1.0f, 1.0f));
	CursorCol->SetName(L"CursorCol");
	CursorCol->SetUICamera(m_UICameraCom);

	CursorCol->PushCollisionEnter(&TownScene::Col_InventoryEnter, this);
	CursorCol->PushCollisionStay(&TownScene::Col_InventoryStay, this);
	CursorCol->PushCollisionExit(&TownScene::Col_InventoryExit, this);


	// 인게임 마우스
	m_InGameMouse = GetScene()->GameObject_Create(L"InGame_MouseCursor");
	m_InGameMouse->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -9.0f, 1.0f));
	m_InGameMouse->GetTransform()->Local_Scale(Vector4(30.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<WorldMouseCursorScript> InGameMouseScript = m_InGameMouse->Component_Add<WorldMouseCursorScript>();
	InGameMouseScript->SetCamera(m_CameraCom);

	InGameCursorCol = m_InGameMouse->Component_Add<RectCol2D>(L"InGameCursorCol");
	InGameCursorCol->Local_SubPosition(Vector4(-10.0f, 10.0f, 1.0f, 1.0f));
	InGameCursorCol->Local_SubScale(Vector4(3.0f, 3.0f, 1.0f, 1.0f));
	InGameCursorCol->SetName(L"InCursorCol");
	InGameCursorCol->SetUICamera(m_CameraCom);

	InGameCursorCol->PushCollisionEnter(&TownScene::Col_QuestEnter, this);
	InGameCursorCol->PushCollisionStay(&TownScene::Col_QuestStay, this);
	InGameCursorCol->PushCollisionExit(&TownScene::Col_QuestExit, this);


	// 인벤토리에서 마우스 클릭시 아이템 이동하기 위한.. (아이템 이미지를 보여주기 위한 오브젝트 초기화)
	m_InvenClickMouseImage = GetScene()->GameObject_Create(L"Inven_ClickItemImage");
	m_InvenClickMouseImage->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -8.0f, 1.0f));
	m_InvenClickMouseImage->GetTransform()->Local_Scale(Vector4(29.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenClickImageRenderer = m_InvenClickMouseImage->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	InvenClickImageRenderer->SetSprite(L"EmptyItem.png", 0);
	//InvenClickImageRenderer->SetSprite(L"RedPoiton.png", 0);
	InvenClickImageScript = m_InvenClickMouseImage->Component_Add<Inven_MouseItem>();
	InvenClickImageScript->SetMouseCamera(m_UICameraCom);
}

void TownScene::QuickSlotLoading()
{
	// 부모
	m_QuickSlotEmptyObj = GetScene()->GameObject_Create(L"QuickSlotEmptyObject");
	m_QuickSlotEmptyObj->GetTransform()->Local_Position(Vector4(400.0f, -280.0f, 1.0f, 1.0f));
	m_QuickSlotEmptyObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	// 퀵슬롯 - 뒷배경
	m_QuickSlotBGBack = GetScene()->GameObject_Create(L"QuickSlot_BG_Back");
	m_QuickSlotBGBack->SetParent(m_QuickSlotEmptyObj);
	m_QuickSlotBGBack->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -3.0f, 1.0f));
	m_QuickSlotBGBack->GetTransform()->Local_Scale(Vector4(172.0f, 67.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuickSlotBGBackRenderer = m_QuickSlotBGBack->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUICKSLOT);
	QuickSlotBGBackRenderer->SetSprite(L"quickSlot.backgrnd.png", 0);

	// 퀵슬롯 - 앞배경
	m_QuickSlotBGFront = GetScene()->GameObject_Create(L"QuickSlot_BG_Front");
	m_QuickSlotBGFront->SetParent(m_QuickSlotEmptyObj);
	m_QuickSlotBGFront->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -6.0f, 1.0f));
	m_QuickSlotBGFront->GetTransform()->Local_Scale(Vector4(176.0f, 73.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuickSlotBGFrontRenderer = m_QuickSlotBGFront->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUICKSLOT);
	QuickSlotBGFrontRenderer->SetSprite(L"quickSlot.layer_cover.png", 0);

	// 퀵슬롯들 (타일맵처럼 만듦 -> Vector에 모아주기)
	Vector4 BasePos = m_QuickSlotEmptyObj->GetTransform()->Local_Position();
	BasePos.x -= 70.0f;
	BasePos.y += 17.0f;
	BasePos.z = -5.0f;

	for (size_t y = 0; y < 2; ++y)
	{
		for (size_t x = 0; x < 5; ++x)
		{
			Vector4 NewCreatePos = BasePos;

			//NewCreatePos.x += (x * (16.0f + 3.0f));
			//NewCreatePos.y -= (y * (16.0f + 3.0f));

			NewCreatePos.x += (x * 35.0f);
			NewCreatePos.y -= (y * 35.0f);

			My_PTR<GameObject> NewQuickSlot = GetScene()->GameObject_Create(L"QuickSlot");
			NewQuickSlot->GetTransform()->Local_Position(Vector4(NewCreatePos));
			NewQuickSlot->GetTransform()->Local_Scale(Vector4(32.0f, 32.0f, 1.0f, 1.0f));
			My_PTR<SpriteRenderer2D> NewQuickSlotRenderer = NewQuickSlot->Component_Add<SpriteRenderer2D>(UI_QUICKSLOT);
			NewQuickSlotRenderer->SetSprite(L"EmptyItem.png", 0);
			My_PTR<QuickSlotScript> NewQuickSlotScript = NewQuickSlot->Component_Add<QuickSlotScript>();
			NewQuickSlotScript->SetSoundPlayer(m_EffectSoundPlayer);
			My_PTR<RectCol2D> NewQuickSlotCollision = NewQuickSlot->Component_Add<RectCol2D>(L"QuickSlotCol");
			NewQuickSlotCollision->SetUICamera(m_UICameraCom);
			NewQuickSlotCollision->Local_SubScale(NewQuickSlot->GetTransform()->Local_Scale());


			// 스킬 5개 넣어주기 ㅇㅅㅇ 
			if (0 == y)
			{
				if (0 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"BlazingExtinction_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_Insert");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_BE);
					NewQuickSlotScript->SetCoolTime(15.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"BlazingExtinction");
				}
				else if (1 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"Blizzard_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_Home");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_BZ);
					NewQuickSlotScript->SetCoolTime(12.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"Blizzard");
				}
				else if (2 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"Cataclysm_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_PageUp");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_CC);
					NewQuickSlotScript->SetCoolTime(180.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"Cataclysm");
				}
				else if (3 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_1");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
				else if (4 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_2");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
			}
			else if (1 == y)
			{
				if (0 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"PoisonMist_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_Delete");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_PM);
					NewQuickSlotScript->SetCoolTime(20.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"PoisonMist");

				}
				else if (1 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"SoulPenetration_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_End");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_SP);
					NewQuickSlotScript->SetCoolTime(2.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"SoulPenetration");
				}
				else if (2 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_PageDown");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
				else if (3 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_3");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
				else if (4 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_4");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
			}

			// std::vector로 관리하기
			m_QuickSlotVector.push_back(NewQuickSlot);
			m_QuickSlotCol.push_back(NewQuickSlotCollision);

		}
	}

}

void TownScene::CollisionNameLink()
{
	GetScene()->m_CollisionManager.Link(L"Player", L"Monster");

	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InventoryMoveObjCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtCloseCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtDisassembleCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtExtractCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtAppraiseCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtToadCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtUpgradeCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtEquipCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtComsumCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtEtcCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtInstallCol");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InvenBtCashCol");

	GetScene()->m_CollisionManager.Link(L"CursorCol", L"InventorySlot_Consume");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"QuickSlotCol");

	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Quest_DialogBtExitCol");		// 퀘스트관련 버튼  나가기
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Quest_DialogBtNoCol");			// 퀘스트 버튼 - 아니오
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Quest_DialogBtCheckCol");		// 퀘스트 버튼 - 확인
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Quest_DialogBtYesCol");			// 퀘스트 버튼 - 예

	// 상점
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtEuip");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtConsume");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtEtc");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtInstall");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtCash");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtAll");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtReBuy");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtExit");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtBuy");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"Shop_BtSell");

	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopBuy_RedPoiton");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopBuy_BluePoiton");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopBuy_OrangePoiton");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopBuy_WhitePoiton");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopBuy_ManaElixir");

	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopSell_FirstMyItem");		// 내가 팔 수 있는 아이템 충돌체
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopSell_SecondMyItem");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopSell_ThirdMyItem");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopSell_FourthMyItem");
	GetScene()->m_CollisionManager.Link(L"CursorCol", L"ShopSell_FivthMyItem");

	// NPC
	GetScene()->m_CollisionManager.Link(L"InGameCursorCol", L"Col_WindSpiritNPC");
	GetScene()->m_CollisionManager.Link(L"InGameCursorCol", L"Col_RockSpiritNPC");
	GetScene()->m_CollisionManager.Link(L"InGameCursorCol", L"Col_TreeSpiritNPC");
	GetScene()->m_CollisionManager.Link(L"InGameCursorCol", L"Col_SmallSpiritNPC");

	GetScene()->m_CollisionManager.Link(L"InGameCursorCol", L"Col_PotionNPC");
	GetScene()->m_CollisionManager.Link(L"InGameCursorCol", L"Col_WarehouseNPC");

	// 포탈  - Player
	GetScene()->m_CollisionManager.Link(L"Player", L"Town_PrePortalCol");
	GetScene()->m_CollisionManager.Link(L"Player", L"Town_NextPortalCol");

}

void TownScene::QuestAlarmLoading()
{
	// 퀘스트 - NPC 머리 위에 뜨는 알람 (전구, 펼쳐진 책, 덮은 책)
	Vector4 RockQuestAlarmPos = m_RockSpiritNPC->GetTransform()->Local_Position();
	RockQuestAlarmPos.z = 10.0f;
	RockQuestAlarmPos.y += 70.0f;
	RockQuestAlarmPos.x += 50.0f;

	m_RockQuestAlarm = GetScene()->GameObject_Create(L"RockQuestAlarm");
	m_RockQuestAlarm->GetTransform()->Local_Position(RockQuestAlarmPos);
	m_RockQuestAlarm->GetTransform()->Local_Scale(Vector4(44.0f, 46.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> RockQuestAlarmRenderer = m_RockQuestAlarm->Component_Add<SpriteRenderer2D>(RenderOrder::NPC);
	RockQuestAlarmRenderer->SetSprite(L"Quest_Alarm.png", 0);
	My_PTR<Animation2D> RockQuestAlarmAni = m_RockQuestAlarm->Component_Add<Animation2D>();
	RockQuestAlarmScript = m_RockQuestAlarm->Component_Add<QuestAlarm>();
	My_PTR<RectCol2D> RockQuestRectCol = m_RockQuestAlarm->Component_Add<RectCol2D>(L"RockQuestAlarmCol");
}

void TownScene::ShopLoading()
{
	m_ShopEmptyObj = GetScene()->GameObject_Create(L"Shop_PotionShopEmtpy");
	m_ShopEmptyObj->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ShopEmptyObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_ShopBackground = GetScene()->GameObject_Create(L"Shop_PotionShopBackground");
	m_ShopBackground->SetParent(m_ShopEmptyObj);
	m_ShopBackground->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -1.0f, 1.0f));
	m_ShopBackground->GetTransform()->Local_Scale(Vector4(465.0f, 328.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PotionShopRenderer = m_ShopBackground->Component_Add<SpriteRenderer2D>(RenderOrder::UI_INVENTORY);
	PotionShopRenderer->SetSprite(L"PotionShop_Background.png", 0);


	// 버튼
	m_ShopBtEquip = GetScene()->GameObject_Create(L"Shop_BtEuip");
	m_ShopBtEquip->SetParent(m_ShopEmptyObj);
	m_ShopBtEquip->GetTransform()->Local_Position(Vector4(29.5f, 64.5f, -2.0f, 1.0f));
	m_ShopBtEquip->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopEquipRenderer = m_ShopBtEquip->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopEquipRenderer->SetSprite(L"ShpoBt_Equip.png", 0);
	My_PTR<Animation2D> ShopEquipAni = m_ShopBtEquip->Component_Add<Animation2D>();
	m_ShopEquipAniScript = m_ShopBtEquip->Component_Add<Inven_BtKind>();
	m_ShopEquipAniScript->SetAnimation(L"Enabled");
	m_ShopEquipCol = m_ShopBtEquip->Component_Add<RectCol2D>(L"Shop_BtEuip");
	m_ShopEquipCol->SetUICamera(m_UICameraCom);

	m_ShopBtComsume = GetScene()->GameObject_Create(L"Shop_BtConsume");
	m_ShopBtComsume->SetParent(m_ShopEmptyObj);
	m_ShopBtComsume->GetTransform()->Local_Position(Vector4(72.5f, 64.5f, -2.0f, 1.0f));
	m_ShopBtComsume->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopConsumeRenderer = m_ShopBtComsume->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopConsumeRenderer->SetSprite(L"ShopBt_Consume.png", 0);
	My_PTR<Animation2D> ShopComsumAni = m_ShopBtComsume->Component_Add<Animation2D>();
	m_ShopConsumeAniScript = m_ShopBtComsume->Component_Add<Inven_BtKind>();
	m_ShopConsumeCol = m_ShopBtComsume->Component_Add<RectCol2D>(L"Shop_BtConsume");
	m_ShopConsumeCol->SetUICamera(m_UICameraCom);

	m_ShopBtEtc = GetScene()->GameObject_Create(L"Shop_BtEtc");
	m_ShopBtEtc->SetParent(m_ShopEmptyObj);
	m_ShopBtEtc->GetTransform()->Local_Position(Vector4(115.5f, 64.5f, -2.0f, 1.0f));
	m_ShopBtEtc->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopEtcRenderer = m_ShopBtEtc->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopEtcRenderer->SetSprite(L"ShopBt_Etc.png", 0);
	My_PTR<Animation2D> ShopEtcAni = m_ShopBtEtc->Component_Add<Animation2D>();
	m_ShopEtcAniScript = m_ShopBtEtc->Component_Add<Inven_BtKind>();
	m_ShopEtcCol = m_ShopBtEtc->Component_Add<RectCol2D>(L"Shop_BtEtc");
	m_ShopEtcCol->SetUICamera(m_UICameraCom);

	m_ShopBtInstall = GetScene()->GameObject_Create(L"Shop_BtInstall");
	m_ShopBtInstall->SetParent(m_ShopEmptyObj);
	m_ShopBtInstall->GetTransform()->Local_Position(Vector4(158.5f, 64.5f, -2.0f, 1.0f));
	m_ShopBtInstall->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopInstallRenderer = m_ShopBtInstall->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopInstallRenderer->SetSprite(L"ShopBt_Install.png", 0);
	My_PTR<Animation2D> ShopInstall = m_ShopBtInstall->Component_Add<Animation2D>();
	m_ShopInstallAniScript = m_ShopBtInstall->Component_Add<Inven_BtKind>();
	m_ShopInstallCol = m_ShopBtInstall->Component_Add<RectCol2D>(L"Shop_BtInstall");
	m_ShopInstallCol->SetUICamera(m_UICameraCom);

	m_ShopCash = GetScene()->GameObject_Create(L"Shop_BtCash");
	m_ShopCash->SetParent(m_ShopEmptyObj);
	m_ShopCash->GetTransform()->Local_Position(Vector4(201.5f, 64.5f, -2.0f, 1.0f));
	m_ShopCash->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopCashRenderer = m_ShopCash->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopCashRenderer->SetSprite(L"ShopBt_Cash.png", 0);
	My_PTR<Animation2D> ShopCashAni = m_ShopCash->Component_Add<Animation2D>();
	m_ShopCashAniScript = m_ShopCash->Component_Add<Inven_BtKind>();
	m_ShopCashCol = m_ShopCash->Component_Add<RectCol2D>(L"Shop_BtCash");
	m_ShopCashCol->SetUICamera(m_UICameraCom);

	m_ShopBtAll = GetScene()->GameObject_Create(L"Shop_BtAll");
	m_ShopBtAll->SetParent(m_ShopEmptyObj);
	m_ShopBtAll->GetTransform()->Local_Position(Vector4(-200.0f, 64.5f, -2.0f, 1.0f));
	m_ShopBtAll->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopAllRenderer = m_ShopBtAll->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopAllRenderer->SetSprite(L"ShopBt_All.png", 0);
	My_PTR<Animation2D> ShopAllAni = m_ShopBtAll->Component_Add<Animation2D>();
	m_ShopAllAniScript = m_ShopBtAll->Component_Add<Inven_BtKind>();
	m_ShopAllAniScript->SetAnimation(L"Enabled");
	m_ShopAllCol = m_ShopBtAll->Component_Add<RectCol2D>(L"Shop_BtAll");
	m_ShopAllCol->SetUICamera(m_UICameraCom);

	m_ShopBtReBuy = GetScene()->GameObject_Create(L"Shop_BtReBuy");
	m_ShopBtReBuy->SetParent(m_ShopEmptyObj);
	m_ShopBtReBuy->GetTransform()->Local_Position(Vector4(-157.0f, 64.5f, -2.0f, 1.0f));
	m_ShopBtReBuy->GetTransform()->Local_Scale(Vector4(42.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopReBuyRenderer = m_ShopBtReBuy->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopReBuyRenderer->SetSprite(L"ShopBt_ReBuy.png", 0);
	My_PTR<Animation2D> ShopReBuyAni = m_ShopBtReBuy->Component_Add<Animation2D>();
	m_ShopRebuyAniScript = m_ShopBtReBuy->Component_Add<Inven_BtKind>();
	m_ShopRebuyCol = m_ShopBtReBuy->Component_Add<RectCol2D>(L"Shop_BtReBuy");
	m_ShopRebuyCol->SetUICamera(m_UICameraCom);

	m_ShopBtExit = GetScene()->GameObject_Create(L"Shop_BtExit");
	m_ShopBtExit->SetParent(m_ShopEmptyObj);
	m_ShopBtExit->GetTransform()->Local_Position(Vector4(-44.5f, 110.5f, -2.0f, 1.0f));
	m_ShopBtExit->GetTransform()->Local_Scale(Vector4(64.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopExitRenderer = m_ShopBtExit->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopExitRenderer->SetSprite(L"Bt_ShopExit.png", 0);
	My_PTR<Animation2D> ShopExitAni = m_ShopBtExit->Component_Add<Animation2D>();
	m_ShopExitAniScript = m_ShopBtExit->Component_Add<Inven_BtAnimation>();
	m_ShopExitCol = m_ShopBtExit->Component_Add<RectCol2D>(L"Shop_BtExit");
	m_ShopExitCol->SetUICamera(m_UICameraCom);
	

	m_ShopBtItemBuy = GetScene()->GameObject_Create(L"Shop_BtBuy");
	m_ShopBtItemBuy->SetParent(m_ShopEmptyObj);
	m_ShopBtItemBuy->GetTransform()->Local_Position(Vector4(-44.5f, 92.5f, -2.0f, 1.0f));
	m_ShopBtItemBuy->GetTransform()->Local_Scale(Vector4(64.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopItemBuyRenderer = m_ShopBtItemBuy->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopItemBuyRenderer->SetSprite(L"Bt_ShopBuy.png", 0);
	My_PTR<Animation2D> ShopItemBuyAni = m_ShopBtItemBuy->Component_Add<Animation2D>();
	m_ShopItemBuyAniScript = m_ShopBtItemBuy->Component_Add<Inven_BtAnimation>();
	m_ShopItemBuyCol = m_ShopBtItemBuy->Component_Add<RectCol2D>(L"Shop_BtBuy");
	m_ShopItemBuyCol->SetUICamera(m_UICameraCom);

	m_ShopBtItemSell = GetScene()->GameObject_Create(L"Shop_BtSell");
	m_ShopBtItemSell->SetParent(m_ShopEmptyObj);
	m_ShopBtItemSell->GetTransform()->Local_Position(Vector4(190.5f, 110.5f, -2.0f, 1.0f));
	m_ShopBtItemSell->GetTransform()->Local_Scale(Vector4(64.0f, 16.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopItemSellRenderer = m_ShopBtItemSell->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	ShopItemSellRenderer->SetSprite(L"Bt_ShopSell.png", 0);
	My_PTR<Animation2D> ShopItemSellAni = m_ShopBtItemSell->Component_Add<Animation2D>();
	m_ShopItemSellAniScript = m_ShopBtItemSell->Component_Add<Inven_BtAnimation>();
	m_ShopItemSellCol = m_ShopBtItemSell->Component_Add<RectCol2D>(L"Shop_BtSell");
	m_ShopItemSellCol->SetUICamera(m_UICameraCom);


	// 상점에서 판매하는 아이템
	m_ShopBuyRedPotion = GetScene()->GameObject_Create(L"ShopBuy_RedPoiton");
	m_ShopBuyRedPotion->SetParent(m_ShopEmptyObj);
	m_ShopBuyRedPotion->GetTransform()->Local_Position(Vector4(-121.5f, 32.5f, -3.0f, 1.0f));
	m_ShopBuyRedPotion->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopBuyRedPiotonRenderer = m_ShopBuyRedPotion->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	ShopBuyRedPiotonRenderer->SetSprite(L"Shop_RedPoiton.png", 0);
	My_PTR<Animation2D> ShopBuyRedPoitionAni = m_ShopBuyRedPotion->Component_Add<Animation2D>();
	m_ShopBuyRedPoitonAniScript = m_ShopBuyRedPotion->Component_Add<ShopBuyItemBt>();
	m_ShopBuyRedPotionCol = m_ShopBuyRedPotion->Component_Add<RectCol2D>(L"ShopBuy_RedPoiton");
	m_ShopBuyRedPotionCol->SetUICamera(m_UICameraCom);

	m_ShopBuyBluePotion = GetScene()->GameObject_Create(L"ShopBuy_BluePoiton");
	m_ShopBuyBluePotion->SetParent(m_ShopEmptyObj);
	m_ShopBuyBluePotion->GetTransform()->Local_Position(Vector4(-121.5f, -9.5f, -3.0f, 1.0f));
	m_ShopBuyBluePotion->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopBuyBluePiotonRenderer = m_ShopBuyBluePotion->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	ShopBuyBluePiotonRenderer->SetSprite(L"Shop_BluePotion.png", 0);
	My_PTR<Animation2D> ShopBuyBluePoitionAni = m_ShopBuyBluePotion->Component_Add<Animation2D>();
	m_ShopBuyBluePoitonAniScript = m_ShopBuyBluePotion->Component_Add<ShopBuyItemBt>();
	m_ShopBuyBluePotionCol = m_ShopBuyBluePotion->Component_Add<RectCol2D>(L"ShopBuy_BluePoiton");
	m_ShopBuyBluePotionCol->SetUICamera(m_UICameraCom);

	m_ShopBuyOrangePoiton = GetScene()->GameObject_Create(L"ShopBuy_OrangePoiton");
	m_ShopBuyOrangePoiton->SetParent(m_ShopEmptyObj);
	m_ShopBuyOrangePoiton->GetTransform()->Local_Position(Vector4(-121.5f, -51.5f, -3.0f, 1.0f));
	m_ShopBuyOrangePoiton->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopBuyOrangePiotonRenderer = m_ShopBuyOrangePoiton->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	ShopBuyOrangePiotonRenderer->SetSprite(L"Shop_OrangePotion.png", 0);
	My_PTR<Animation2D> ShopBuyOrangePoitionAni = m_ShopBuyOrangePoiton->Component_Add<Animation2D>();
	m_ShopBuyOrangePoitonAniScript = m_ShopBuyOrangePoiton->Component_Add<ShopBuyItemBt>();
	m_ShopBuyOrangePoitonCol = m_ShopBuyOrangePoiton->Component_Add<RectCol2D>(L"ShopBuy_OrangePoiton");
	m_ShopBuyOrangePoitonCol->SetUICamera(m_UICameraCom);

	m_ShopBuyWhitePoiton = GetScene()->GameObject_Create(L"ShopBuy_WhitePoiton");
	m_ShopBuyWhitePoiton->SetParent(m_ShopEmptyObj);
	m_ShopBuyWhitePoiton->GetTransform()->Local_Position(Vector4(-121.5f, -93.5f, -3.0f, 1.0f));
	m_ShopBuyWhitePoiton->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopBuyWhitePiotonRenderer = m_ShopBuyWhitePoiton->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	ShopBuyWhitePiotonRenderer->SetSprite(L"Shop_WhitePotion.png", 0);
	My_PTR<Animation2D> ShopBuyWhitePoitionAni = m_ShopBuyWhitePoiton->Component_Add<Animation2D>();
	m_ShopBuyWhitePoitonAniScript = m_ShopBuyWhitePoiton->Component_Add<ShopBuyItemBt>();
	m_ShopBuyWhitePotionCol = m_ShopBuyWhitePoiton->Component_Add<RectCol2D>(L"ShopBuy_WhitePoiton");
	m_ShopBuyWhitePotionCol->SetUICamera(m_UICameraCom);

	m_ShopBuyManaElixir = GetScene()->GameObject_Create(L"ShopBuy_ManaElixir");
	m_ShopBuyManaElixir->SetParent(m_ShopEmptyObj);
	m_ShopBuyManaElixir->GetTransform()->Local_Position(Vector4(-121.5f, -135.5f, -3.0f, 1.0f));
	m_ShopBuyManaElixir->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> ShopBuyManaElixirPiotonRenderer = m_ShopBuyManaElixir->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	ShopBuyManaElixirPiotonRenderer->SetSprite(L"Shop_ManaElixir.png", 0);
	My_PTR<Animation2D> ShopBuyElixirAni = m_ShopBuyManaElixir->Component_Add<Animation2D>();
	m_ShopBuyManaElxirAniScript = m_ShopBuyManaElixir->Component_Add<ShopBuyItemBt>();
	m_ShopBuyElixirCol = m_ShopBuyManaElixir->Component_Add<RectCol2D>(L"ShopBuy_ManaElixir");
	m_ShopBuyElixirCol->SetUICamera(m_UICameraCom);

	// 내가 판매할 수 있는 아이템 - EmptyItem
	m_ShopSellMyItem[EnumOrder::FIRST] = GetScene()->GameObject_Create(L"ShopSell_FirstMyItem");
	m_ShopSellMyItem[EnumOrder::FIRST]->SetParent(m_ShopEmptyObj);
	m_ShopSellMyItem[EnumOrder::FIRST]->GetTransform()->Local_Position(Vector4(110.0f, 32.5f, -3.0f, 1.0f));
	m_ShopSellMyItem[EnumOrder::FIRST]->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SellFirstMyItem = m_ShopSellMyItem[EnumOrder::FIRST]->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	SellFirstMyItem->SetSprite(L"Shop_WhitePotion.png", 0);
	My_PTR<Animation2D> SellFirtMyItemAni = m_ShopSellMyItem[EnumOrder::FIRST]->Component_Add<Animation2D>();
	m_ShopSellMyItemAniScript[EnumOrder::FIRST] = m_ShopSellMyItem[EnumOrder::FIRST]->Component_Add<ShopBuyItemBt>();
	m_ShopSellMyItemNumberScript[EnumOrder::FIRST] = m_ShopSellMyItem[EnumOrder::FIRST]->Component_Add<ShopBuyItemNumber>();
	m_ShopSellMyItemCol[EnumOrder::FIRST] = m_ShopSellMyItem[EnumOrder::FIRST]->Component_Add<RectCol2D>(L"ShopSell_FirstMyItem");
	m_ShopSellMyItemCol[EnumOrder::FIRST]->SetUICamera(m_UICameraCom);

	m_ShopSellMyItem[EnumOrder::SECOND] = GetScene()->GameObject_Create(L"ShopSell_SecondMyItem");
	m_ShopSellMyItem[EnumOrder::SECOND]->SetParent(m_ShopEmptyObj);
	m_ShopSellMyItem[EnumOrder::SECOND]->GetTransform()->Local_Position(Vector4(110.0f, -9.5f, -3.0f, 1.0f));
	m_ShopSellMyItem[EnumOrder::SECOND]->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SellSecondMyItem = m_ShopSellMyItem[EnumOrder::SECOND]->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	SellSecondMyItem->SetSprite(L"Shop_WhitePotion.png", 0);
	My_PTR<Animation2D> SellSecondMyItemAni = m_ShopSellMyItem[EnumOrder::SECOND]->Component_Add<Animation2D>();
	m_ShopSellMyItemAniScript[EnumOrder::SECOND] = m_ShopSellMyItem[EnumOrder::SECOND]->Component_Add<ShopBuyItemBt>();
	m_ShopSellMyItemNumberScript[EnumOrder::SECOND] = m_ShopSellMyItem[EnumOrder::SECOND]->Component_Add<ShopBuyItemNumber>();
	m_ShopSellMyItemCol[EnumOrder::SECOND] = m_ShopSellMyItem[EnumOrder::SECOND]->Component_Add<RectCol2D>(L"ShopSell_SecondMyItem");
	m_ShopSellMyItemCol[EnumOrder::SECOND]->SetUICamera(m_UICameraCom);

	m_ShopSellMyItem[EnumOrder::THIRD] = GetScene()->GameObject_Create(L"ShopSell_ThirdMyItem");
	m_ShopSellMyItem[EnumOrder::THIRD]->SetParent(m_ShopEmptyObj);
	m_ShopSellMyItem[EnumOrder::THIRD]->GetTransform()->Local_Position(Vector4(110.0f, -51.5f, -3.0f, 1.0f));
	m_ShopSellMyItem[EnumOrder::THIRD]->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SellThirdMyItem = m_ShopSellMyItem[EnumOrder::THIRD]->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	SellThirdMyItem->SetSprite(L"Shop_WhitePotion.png", 0);
	My_PTR<Animation2D> SellThirdMyItemAni = m_ShopSellMyItem[EnumOrder::THIRD]->Component_Add<Animation2D>();
	m_ShopSellMyItemAniScript[EnumOrder::THIRD] = m_ShopSellMyItem[EnumOrder::THIRD]->Component_Add<ShopBuyItemBt>();
	m_ShopSellMyItemNumberScript[EnumOrder::THIRD] = m_ShopSellMyItem[EnumOrder::THIRD]->Component_Add<ShopBuyItemNumber>();
	m_ShopSellMyItemCol[EnumOrder::THIRD] = m_ShopSellMyItem[EnumOrder::THIRD]->Component_Add<RectCol2D>(L"ShopSell_ThirdMyItem");
	m_ShopSellMyItemCol[EnumOrder::THIRD]->SetUICamera(m_UICameraCom);

	m_ShopSellMyItem[EnumOrder::FOURTH] = GetScene()->GameObject_Create(L"ShopSell_FourthMyItem");
	m_ShopSellMyItem[EnumOrder::FOURTH]->SetParent(m_ShopEmptyObj);
	m_ShopSellMyItem[EnumOrder::FOURTH]->GetTransform()->Local_Position(Vector4(110.0f, -93.5f, -3.0f, 1.0f));
	m_ShopSellMyItem[EnumOrder::FOURTH]->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SellFourthMyItem = m_ShopSellMyItem[EnumOrder::FOURTH]->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	SellFourthMyItem->SetSprite(L"Shop_WhitePotion.png", 0);
	My_PTR<Animation2D> SellFourthMyItemAni = m_ShopSellMyItem[EnumOrder::FOURTH]->Component_Add<Animation2D>();
	m_ShopSellMyItemAniScript[EnumOrder::FOURTH] = m_ShopSellMyItem[EnumOrder::FOURTH]->Component_Add<ShopBuyItemBt>();
	m_ShopSellMyItemNumberScript[EnumOrder::FOURTH] = m_ShopSellMyItem[EnumOrder::FOURTH]->Component_Add<ShopBuyItemNumber>();
	m_ShopSellMyItemCol[EnumOrder::FOURTH] = m_ShopSellMyItem[EnumOrder::FOURTH]->Component_Add<RectCol2D>(L"ShopSell_FourthMyItem");
	m_ShopSellMyItemCol[EnumOrder::FOURTH]->SetUICamera(m_UICameraCom);

	m_ShopSellMyItem[EnumOrder::FIVTH] = GetScene()->GameObject_Create(L"ShopSell_FivthMyItem");
	m_ShopSellMyItem[EnumOrder::FIVTH]->SetParent(m_ShopEmptyObj);
	m_ShopSellMyItem[EnumOrder::FIVTH]->GetTransform()->Local_Position(Vector4(110.0f, -135.5f, -3.0f, 1.0f));
	m_ShopSellMyItem[EnumOrder::FIVTH]->GetTransform()->Local_Scale(Vector4(202.0f, 41.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SellFivthMyItem = m_ShopSellMyItem[EnumOrder::FIVTH]->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	SellFivthMyItem->SetSprite(L"Shop_WhitePotion.png", 0);
	My_PTR<Animation2D> SellFivthMyItemAni = m_ShopSellMyItem[EnumOrder::FIVTH]->Component_Add<Animation2D>();
	m_ShopSellMyItemAniScript[EnumOrder::FIVTH] = m_ShopSellMyItem[EnumOrder::FIVTH]->Component_Add<ShopBuyItemBt>();
	m_ShopSellMyItemNumberScript[EnumOrder::FIVTH] = m_ShopSellMyItem[EnumOrder::FIVTH]->Component_Add<ShopBuyItemNumber>();
	m_ShopSellMyItemCol[EnumOrder::FIVTH] = m_ShopSellMyItem[EnumOrder::FIVTH]->Component_Add<RectCol2D>(L"ShopSell_FivthMyItem");
	m_ShopSellMyItemCol[EnumOrder::FIVTH]->SetUICamera(m_UICameraCom);

	// 매니저
	std::vector<GameObject*> NewMyItemInitVector;
	NewMyItemInitVector.push_back(m_ShopSellMyItem[EnumOrder::FIRST]);
	NewMyItemInitVector.push_back(m_ShopSellMyItem[EnumOrder::SECOND]);
	NewMyItemInitVector.push_back(m_ShopSellMyItem[EnumOrder::THIRD]);
	NewMyItemInitVector.push_back(m_ShopSellMyItem[EnumOrder::FOURTH]);
	NewMyItemInitVector.push_back(m_ShopSellMyItem[EnumOrder::FIVTH]);

	m_ShopSellMyItemManager = GetScene()->GameObject_Create(L"ShopSellMyItemManager");
	m_ShopSellMyItemManager->GetTransform()->Local_Position(Vector4::Zero);
	m_ShopSellMyItemManager->GetTransform()->Local_Scale(Vector4::Zero);
	m_ShopSellMyItemManagerScript = m_ShopSellMyItemManager->Component_Add<ShopSellItemManagerScript>();
	m_ShopSellMyItemManagerScript->SetInvenItemCounsumVector(m_InvenItemConsum);
	m_ShopSellMyItemManagerScript->SetMyItem(NewMyItemInitVector);
}

void TownScene::MoneyLoading()
{
	m_InvenMoneyManagerObject = GetScene()->GameObject_Create(L"Inven_MoenyManager");
	m_InvenMoneyManagerObject->SetParent(m_InventoryEmptyObj);
	m_InvenMoneyManagerObject->GetTransform()->Local_Position(Vector4(35.5f, -108.0f, 0.0f, 0.0f));
	m_InvenMoneyManagerObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_InvenMoenyManagerScript = m_InvenMoneyManagerObject->Component_Add<MesoManager>();

	m_ShopMeoneyManagerObject = GetScene()->GameObject_Create(L"Shop_MoenyManager");
	m_ShopMeoneyManagerObject->SetParent(m_ShopEmptyObj);
	m_ShopMeoneyManagerObject->GetTransform()->Local_Position(Vector4(217.0f, 93.0f, 0.0f, 0.0));
	m_ShopMeoneyManagerObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_ShopManagerScript = m_ShopMeoneyManagerObject->Component_Add<MesoManager>();
}


void TownScene::QuestLoading()
{
	// 대화창 - 대화관련
	m_QuestDialogEmptyObj = GetScene()->GameObject_Create(L"QuestDialogEmptyObject");
	m_QuestDialogEmptyObj->GetTransform()->Local_Position(Vector4(0.0f, -303.0f, 0.0f, 1.0f));
	m_QuestDialogEmptyObj->GetTransform()->Local_Scale(Vector4::One);

	Vector4 DialogPosVector = Vector4::Zero;
	DialogPosVector.z = -8.0f;

	m_NPCDialogBaseBackground = GetScene()->GameObject_Create(L"QuestDialog_BG");
	m_NPCDialogBaseBackground->SetParent(m_QuestDialogEmptyObj);
	m_NPCDialogBaseBackground->GetTransform()->Local_Position(DialogPosVector);
	m_NPCDialogBaseBackground->GetTransform()->Local_Scale(Vector4(824.0f, 162.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NPCDialogBaseBGRenderer = m_NPCDialogBaseBackground->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	NPCDialogBaseBGRenderer->SetSprite(L"DialogBaseBackgorund.png", 0);


	DialogPosVector = Vector4::Zero;
	DialogPosVector.x += 301.0f;
	DialogPosVector.y += 60.0f;
	DialogPosVector.z -= 9.0f;

	m_NPCTexture2D = GetScene()->GameObject_Create(L"QuestDialog_NPCTexture");
	m_NPCTexture2D->SetParent(m_QuestDialogEmptyObj);
	m_NPCTexture2D->GetTransform()->Local_Position(DialogPosVector);
	m_NPCTexture2D->GetTransform()->Local_Scale(Vector4(268.5f, 298.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NPCTexture2DRenderer = m_NPCTexture2D->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTNPCTEXTURE2D);
	NPCTexture2DRenderer->SetSprite(L"RockSpiritFace.png", 1);

	DialogPosVector = Vector4::Zero;
	DialogPosVector.z = -9.0f;

	m_NPCDialogChat = GetScene()->GameObject_Create(L"QuestDialog_NPCDialogChat");
	m_NPCDialogChat->SetParent(m_QuestDialogEmptyObj);
	m_NPCDialogChat->GetTransform()->Local_Position(DialogPosVector);
	m_NPCDialogChat->GetTransform()->Local_Scale(Vector4(824.0f, 162.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NPCDialogChatRenderer = m_NPCDialogChat->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	NPCDialogChatRenderer->SetSprite(L"Dialog_NPCchat.png", 0);

	DialogPosVector = Vector4::Zero;
	DialogPosVector.x += 301.0f;
	DialogPosVector.y -= 51.0f;
	DialogPosVector.z -= 10.0f;

	m_NPCNameTag = GetScene()->GameObject_Create(L"QuestDialog_NPCNameTag");
	m_NPCNameTag->SetParent(m_QuestDialogEmptyObj);
	m_NPCNameTag->GetTransform()->Local_Position(DialogPosVector);
	m_NPCNameTag->GetTransform()->Local_Scale(Vector4(196.0f, 50.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NPCNameTagRenderer = m_NPCNameTag->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	NPCNameTagRenderer->SetSprite(L"DialogNPC_NameTags.png", 0);

	// 대화창 - 버튼
	DialogPosVector = Vector4::Zero;
	DialogPosVector.x -= 357.0f;
	DialogPosVector.y -= 66.5f;
	DialogPosVector.z -= 10.0f;

	m_QuestBtDialogExit = GetScene()->GameObject_Create(L"QuestDialog_BtExit");
	m_QuestBtDialogExit->SetParent(m_QuestDialogEmptyObj);
	m_QuestBtDialogExit->GetTransform()->Local_Position(DialogPosVector);
	m_QuestBtDialogExit->GetTransform()->Local_Scale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuestDialogExitBTRenderer = m_QuestBtDialogExit->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	QuestDialogExitBTRenderer->SetSprite(L"DialogBt_Exit.png", 3);
	My_PTR<Animation2D> QuestDialogExitBTAniamtion = m_QuestBtDialogExit->Component_Add<Animation2D>();
	m_QuestBtDialogExitCol = m_QuestBtDialogExit->Component_Add<RectCol2D>(L"Quest_DialogBtExitCol");
	m_QuestBtDialogExitCol->Local_SubPosition(Vector4(0.0f, -303.0f, 0.0f, 0.0f));
	m_QuestBtDialogExitCol->Local_SubScale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	m_QDialogExitBtAniScript = m_QuestBtDialogExit->Component_Add<Inven_BtAnimation>();

	DialogPosVector = Vector4::Zero;
	DialogPosVector.x += 136.0f;
	DialogPosVector.y -= 66.5f;
	DialogPosVector.z -= 10.0f;

	m_QuestBtDialogNo = GetScene()->GameObject_Create(L"QuestDialog_BtNo");
	m_QuestBtDialogNo->SetParent(m_QuestDialogEmptyObj);
	m_QuestBtDialogNo->GetTransform()->Local_Position(DialogPosVector);
	m_QuestBtDialogNo->GetTransform()->Local_Scale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuestDialogNoBTRenderer = m_QuestBtDialogNo->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	QuestDialogNoBTRenderer->SetSprite(L"DialogBt_No.png", 3);
	My_PTR<Animation2D> QuestDialogNoBTAnimation = m_QuestBtDialogNo->Component_Add<Animation2D>();
	m_QuestBtDialogNoCol = m_QuestBtDialogNo->Component_Add<RectCol2D>(L"Quest_DialogBtNoCol");
	m_QuestBtDialogNoCol->Local_SubPosition(Vector4(0.0f, -303.0f, 0.0f, 0.0f));
	m_QuestBtDialogNoCol->Local_SubScale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	m_QDialogNoBtAniScript = m_QuestBtDialogNo->Component_Add<Inven_BtAnimation>();

	m_QuestBtDialogCheck = GetScene()->GameObject_Create(L"QuestDialog_BtCheck");
	m_QuestBtDialogCheck->SetParent(m_QuestDialogEmptyObj);
	m_QuestBtDialogCheck->GetTransform()->Local_Position(DialogPosVector);
	m_QuestBtDialogCheck->GetTransform()->Local_Scale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuestDialogCheckBTRenderer = m_QuestBtDialogCheck->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	QuestDialogCheckBTRenderer->SetSprite(L"DialogBt_Check.png", 3);
	My_PTR<Animation2D> QuestDialogCheckBtAnimation = m_QuestBtDialogCheck->Component_Add<Animation2D>();
	m_QuestBtDialogCheckCol = m_QuestBtDialogCheck->Component_Add<RectCol2D>(L"Quest_DialogBtCheckCol");
	m_QuestBtDialogCheckCol->Local_SubPosition(Vector4(0.0f, -303.0f, 0.0f, 0.0f));
	m_QuestBtDialogCheckCol->Local_SubScale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	m_QDialogCheckAniScript = m_QuestBtDialogCheck->Component_Add<Inven_BtAnimation>();

	DialogPosVector = Vector4::Zero;
	DialogPosVector.x += 50.0f;
	DialogPosVector.y -= 66.5f;
	DialogPosVector.z -= 10.0f;

	m_QuestBtDialogYes = GetScene()->GameObject_Create(L"QuestDialog_BtYes");
	m_QuestBtDialogYes->SetParent(m_QuestDialogEmptyObj);
	m_QuestBtDialogYes->GetTransform()->Local_Position(DialogPosVector);
	m_QuestBtDialogYes->GetTransform()->Local_Scale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuestDialogYesBTRenderer = m_QuestBtDialogYes->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	QuestDialogYesBTRenderer->SetSprite(L"DialogBt_Yes.png", 3);
	My_PTR<Animation2D> QuestDialogYesBTAnimation = m_QuestBtDialogYes->Component_Add<Animation2D>();
	m_QuestBtDialogYesCol = m_QuestBtDialogYes->Component_Add<RectCol2D>(L"Quest_DialogBtYesCol");
	m_QuestBtDialogYesCol->Local_SubPosition(Vector4(0.0f, -303.0f, 0.0f, 0.0f));
	m_QuestBtDialogYesCol->Local_SubScale(Vector4(84.0f, 19.0f, 1.0f, 1.0f));
	m_QDialogYesBtAniScript = m_QuestBtDialogYes->Component_Add<Inven_BtAnimation>();

	// 퀘스트 알리미
	m_QuestAlarmEmptyObj = GetScene()->GameObject_Create(L"QuestAlarm_EmtpyObject");
	m_QuestAlarmEmptyObj->GetTransform()->Local_Position(Vector4(404.5f, 100.0f, 1.0f, 1.0f));
	m_QuestAlarmEmptyObj->GetTransform()->Local_Scale(Vector4::One);

	Vector4 QuestAlarmPos = Vector4::Zero;
	QuestAlarmPos.z -= 1.0f;

	m_QuestAlarmBG = GetScene()->GameObject_Create(L"QuestAlarm_Background");
	m_QuestAlarmBG->SetParent(m_QuestAlarmEmptyObj);
	m_QuestAlarmBG->GetTransform()->Local_Position(QuestAlarmPos);
	m_QuestAlarmBG->GetTransform()->Local_Scale(Vector4(215.0f, 130.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuestAlarmBGRenderer = m_QuestAlarmBG->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUESTDIALOG);
	QuestAlarmBGRenderer->SetSprite(L"QuestAlarmBackground_03.png", 0);

	QuestAlarmPos.z -= 1.0f;
	QuestAlarmPos.x += 27.0f;
	QuestAlarmPos.y += 11.0f;

	m_QuesetAlarmManaElixirNumbers = GetScene()->GameObject_Create(L"QuestAlarm_ElixirNumber");
	m_QuesetAlarmManaElixirNumbers->SetParent(m_QuestAlarmEmptyObj);
	m_QuesetAlarmManaElixirNumbers->GetTransform()->Local_Position(QuestAlarmPos);
	m_QuesetAlarmManaElixirNumbers->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	m_QuestAlarmManaElixirNumbersScript = m_QuesetAlarmManaElixirNumbers->Component_Add<QuestAlarmElixirNumberScript>();
	m_QuestAlarmManaElixirNumbersScript->SetInvenConsumeVector(m_InvenItemConsum);
}

////////////////////////////////////////////////////////////////////////


void TownScene::ShopBuyItemAniChange_None()
{
	m_ShopBuyRedPoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyBluePoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyWhitePoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyOrangePoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyManaElxirAniScript->SetAnimation(L"ClickNone");
}

void TownScene::ShopBuyItemAniChange_Pressed()
{
	m_ShopBuyRedPoitonAniScript->SetAnimation(L"ClickPressed");
	m_ShopBuyBluePoitonAniScript->SetAnimation(L"ClickPressed");
	m_ShopBuyWhitePoitonAniScript->SetAnimation(L"ClickPressed");
	m_ShopBuyOrangePoitonAniScript->SetAnimation(L"ClickPressed");
	m_ShopBuyManaElxirAniScript->SetAnimation(L"ClickPressed");
}

void TownScene::ShopSellItemAniChange_None()
{
	for (size_t i = 0; i < 5; i++)
	{
		m_ShopSellMyItemAniScript[i]->SetAnimation(L"ClickNone");
	}
}

void TownScene::ShopSellItemAniChange_Pressed()
{
	for (size_t i = 0; i < 5; i++)
	{
		m_ShopSellMyItemAniScript[i]->SetAnimation(L"ClickPressed");
	}
}

void TownScene::ShopBuyItemManager(std::wstring _SelectItem)
{
	int ShopBuyMoeny = 0;

	if (L"" == _SelectItem)
		return;

	ItemCode::ItemCodeName BuyItemCode = ItemCode::ItemCodeName::EMPTY;

	// 아이템사기  -> +인벤토리에 추가
	// 있다가 돈도 추가해서 돈이 차감되게..
	// 아이템 종류 설정
	if (L"ShopBuyItem_RedPotion" == _SelectItem)
	{
		BuyItemCode = ItemCode::ItemCodeName::POTION_RED;
		ShopBuyMoeny = 250;
	}
	else if (L"ShopBuyItem_BluePotion" == _SelectItem)
	{
		BuyItemCode = ItemCode::ItemCodeName::POTION_BLUE;
		ShopBuyMoeny = 1000;
	}
	else if (L"ShopBuyItem_WhitePotion" == _SelectItem)
	{
		BuyItemCode = ItemCode::ItemCodeName::POTION_WHITE;
		ShopBuyMoeny = 4800;
	}
	else if (L"ShopBuyItem_OrangePotion" == _SelectItem)
	{
		BuyItemCode = ItemCode::ItemCodeName::POTION_ORANGE;
		ShopBuyMoeny = 2400;
	}
	else if (L"ShopBuyItem_ManaElixir" == _SelectItem)
	{
		BuyItemCode = ItemCode::ItemCodeName::POTION_ELIXIR;
		ShopBuyMoeny = 9300;
	}

	if (ItemCode::ItemCodeName::EMPTY == BuyItemCode)
		return;

	// 인벤토리칸에 비워진 칸이 있는지 검사
	bool EmptyInven = false;
	GameObject* EmptyItemConsumeObj = nullptr;

	for (size_t i = 0; i < m_InvenItemConsum.size(); ++i)
	{
		if (ItemCode::ItemCodeName::EMPTY == m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCode())
		{
			EmptyItemConsumeObj = m_InvenItemConsum[i];
			break;
		}
	}

	// 비워진 칸이 없다면 return
	if (nullptr == EmptyItemConsumeObj)
		return;

	// 비워진 칸이 있다면 (아이템추가)
	EmptyItemConsumeObj->GetComponent<Inven_Info>()->SetItemCount(50);
	EmptyItemConsumeObj->GetComponent<Inven_Info>()->ItemAdd(BuyItemCode);

	m_ShopManagerScript->DecMeso(ShopBuyMoeny);

}

void TownScene::ShopSellItemManager(GameObject* _SellMyItem)
{
	int ShopBuyMoeny = 0;

	if (nullptr == _SellMyItem)
		return;

	ItemCode::ItemCodeName BuyItemCode = _SellMyItem->GetComponent<Inven_Info>()->GetItemCode();

	switch (BuyItemCode)
	{
	case ItemCode::EMPTY:
		return;
		break;
	case ItemCode::POTION_RED:
		ShopBuyMoeny = 250;
		break;
	case ItemCode::POTION_BLUE:
		ShopBuyMoeny = 1000;
		break;
	case ItemCode::POTION_WHITE:
		ShopBuyMoeny = 4800;
		break;
	case ItemCode::POTION_ORANGE:
		ShopBuyMoeny = 2400;
		break;
	case ItemCode::POTION_ELIXIR:
		ShopBuyMoeny = 9300;
		break;
	default:
		break;
	}

	_SellMyItem->GetComponent<Inven_Info>()->ItemAdd(ItemCode::EMPTY);
	_SellMyItem->GetComponent<Inven_Info>()->SetItemCount(0);

	m_ShopManagerScript->AddMeso(ShopBuyMoeny);
	m_ShopSellMyItemNumberScript[CurSelectShopMyItemOrder]->SetCount(0);
	m_ShopSellMyItemAniScript[CurSelectShopMyItemOrder]->SetAnimation(L"ClickNone");
	CurSelectShopMyItemOrder = 0;
}



void TownScene::InventoryOff()
{
	m_InvenMoveObj->Update_Off();
	m_InvenBase->Update_Off();

	m_InvenBtClose->Update_Off();
	m_InvenBtDisassemble->Update_Off();
	m_InvenBtExtract->Update_Off();
	m_InvenBtAppraise->Update_Off();
	m_InvenBtPot->Update_Off();
	m_InvenBtToad->Update_Off();
	m_InvenBtUpgrade->Update_Off();

	m_InvenBtEquip->Update_Off();
	m_InvenBtConsum->Update_Off();
	m_InvenBtEtc->Update_Off();
	m_InvenBtInstall->Update_Off();
	m_InvenBtCash->Update_Off();

	m_InventoryEmptyObj->Update_Off();

	// 슬롯
	InventorySlotOff(m_InvenItemEquip);
	InventorySlotOff(m_InvenItemConsum);
	InventorySlotOff(m_InvenItemEtc);
	InventorySlotOff(m_InvenItemInstall);
	InventorySlotOff(m_InvenItemCash);

	// 돈
	m_InvenMoenyManagerScript->Money_Off();

}

void TownScene::InventoryOn()
{
	m_InvenMoveObj->Update_On();
	m_InvenBase->Update_On();

	m_InvenBtClose->Update_On();
	m_InvenBtDisassemble->Update_On();
	m_InvenBtExtract->Update_On();
	m_InvenBtAppraise->Update_On();
	m_InvenBtPot->Update_On();
	m_InvenBtToad->Update_On();
	m_InvenBtUpgrade->Update_On();

	m_InvenBtEquip->Update_On();
	m_InvenBtConsum->Update_On();
	m_InvenBtEtc->Update_On();
	m_InvenBtInstall->Update_On();
	m_InvenBtCash->Update_On();

	m_InventoryEmptyObj->Update_On();

	// 슬롯
	InventorySlotOff(m_InvenItemEquip);
	InventorySlotOff(m_InvenItemConsum);
	InventorySlotOff(m_InvenItemEtc);
	InventorySlotOff(m_InvenItemInstall);
	InventorySlotOff(m_InvenItemCash);

	if (true == InvenBtEquipAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemEquip);
	}
	else if (true == InvenBtComsumAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemConsum);
	}
	else if (true == InvenBtEtcAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemEtc);
	}
	else if (true == InvenBtInstallAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemInstall);
	}
	else if (true == InvenBtCashAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemCash);
	}


	m_InvenMoenyManagerScript->Money_On();
}

void TownScene::ShopOff()
{
	m_ShopBuySelectItem = L"";
	m_ShopBuyRedPoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyBluePoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyWhitePoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyOrangePoitonAniScript->SetAnimation(L"ClickNone");
	m_ShopBuyManaElxirAniScript->SetAnimation(L"ClickNone");

	m_ShopSellMyItem[EnumOrder::FIRST]->Update_Off();
	m_ShopSellMyItem[EnumOrder::SECOND]->Update_Off();
	m_ShopSellMyItem[EnumOrder::THIRD]->Update_Off();
	m_ShopSellMyItem[EnumOrder::FOURTH]->Update_Off();
	m_ShopSellMyItem[EnumOrder::FIVTH]->Update_Off();

	m_ShopSellMyItemNumberScript[EnumOrder::FIRST]->NumberUpdate_Off();
	m_ShopSellMyItemNumberScript[EnumOrder::SECOND]->NumberUpdate_Off();
	m_ShopSellMyItemNumberScript[EnumOrder::THIRD]->NumberUpdate_Off();
	m_ShopSellMyItemNumberScript[EnumOrder::FOURTH]->NumberUpdate_Off();
	m_ShopSellMyItemNumberScript[EnumOrder::FIVTH]->NumberUpdate_Off();

	m_ShopManagerScript->Money_Off();

	m_ShopBackground->Update_Off();
	m_ShopBtEquip->Update_Off();
	m_ShopBtComsume->Update_Off();
	m_ShopBtEtc->Update_Off();
	m_ShopBtInstall->Update_Off();
	m_ShopCash->Update_Off();
	m_ShopBtAll->Update_Off();
	m_ShopBtReBuy->Update_Off();
	m_ShopBtItemBuy->Update_Off();
	m_ShopBtItemSell->Update_Off();
	m_ShopBtExit->Update_Off();

	m_ShopBuyRedPotion->Update_Off();
	m_ShopBuyBluePotion->Update_Off();
	m_ShopBuyWhitePoiton->Update_Off();
	m_ShopBuyOrangePoiton->Update_Off();
	m_ShopBuyManaElixir->Update_Off();


	m_ShopEmptyObj->Update_Off();
}

void TownScene::ShopOn()
{
	m_ShopEmptyObj->Update_On();

	m_ShopBackground->Update_On();
	m_ShopBtEquip->Update_On();
	m_ShopBtComsume->Update_On();
	m_ShopBtEtc->Update_On();
	m_ShopBtInstall->Update_On();
	m_ShopCash->Update_On();
	m_ShopBtAll->Update_On();
	m_ShopBtReBuy->Update_On();
	m_ShopBtItemBuy->Update_On();
	m_ShopBtItemSell->Update_On();
	m_ShopBtExit->Update_On();

	m_ShopBuyRedPotion->Update_On();
	m_ShopBuyBluePotion->Update_On();
	m_ShopBuyWhitePoiton->Update_On();
	m_ShopBuyOrangePoiton->Update_On();
	m_ShopBuyManaElixir->Update_On();

	m_ShopSellMyItem[EnumOrder::FIRST]->Update_On();
	m_ShopSellMyItem[EnumOrder::SECOND]->Update_On();
	m_ShopSellMyItem[EnumOrder::THIRD]->Update_On();
	m_ShopSellMyItem[EnumOrder::FOURTH]->Update_On();
	m_ShopSellMyItem[EnumOrder::FIVTH]->Update_On();

	m_ShopSellMyItemNumberScript[EnumOrder::FIRST]->NumberUpdate_On();
	m_ShopSellMyItemNumberScript[EnumOrder::SECOND]->NumberUpdate_On();
	m_ShopSellMyItemNumberScript[EnumOrder::THIRD]->NumberUpdate_On();
	m_ShopSellMyItemNumberScript[EnumOrder::FOURTH]->NumberUpdate_On();
	m_ShopSellMyItemNumberScript[EnumOrder::FIVTH]->NumberUpdate_On();

	m_ShopManagerScript->Money_On();
}

void TownScene::ShopBuyAllItemOn()
{
	m_ShopBuyRedPotion->Update_On();
	m_ShopBuyBluePotion->Update_On();
	m_ShopBuyWhitePoiton->Update_On();
	m_ShopBuyOrangePoiton->Update_On();
	m_ShopBuyManaElixir->Update_On();
}

void TownScene::ShopBuyAllItemOff()
{
	m_ShopBuyRedPotion->Update_Off();
	m_ShopBuyBluePotion->Update_Off();
	m_ShopBuyWhitePoiton->Update_Off();
	m_ShopBuyOrangePoiton->Update_Off();
	m_ShopBuyManaElixir->Update_Off();
}

void TownScene::ShopMyItem_On()
{
	for (int i = 0; i < 5; ++i)
	{
		m_ShopSellMyItem[i]->Update_On();
		m_ShopSellMyItemAniScript[i]->Update_On();
		m_ShopSellMyItemNumberScript[i]->NumberUpdate_On();
	}
}

void TownScene::ShopMyItem_Off()
{
	for (int i = 0; i < 5; ++i)
	{
		m_ShopSellMyItem[i]->Update_Off();
		m_ShopSellMyItemAniScript[i]->Update_Off();
		m_ShopSellMyItemNumberScript[i]->NumberUpdate_Off();
	}
}

void TownScene::QuestDialog_AllOff()
{
	m_NPCDialogBaseBackground->Update_Off();
	m_NPCTexture2D->Update_Off();
	m_NPCDialogChat->Update_Off();
	m_NPCNameTag->Update_Off();

	m_QuestBtDialogExit->Update_Off();
	m_QuestBtDialogYes->Update_Off();
	m_QuestBtDialogNo->Update_Off();
	m_QuestBtDialogCheck->Update_Off();

	m_QuestBtDialogExitCol->Update_Off();
	m_QuestBtDialogYesCol->Update_Off();
	m_QuestBtDialogNoCol->Update_Off();
	m_QuestBtDialogCheckCol->Update_Off();
}

// 상황에 따른 버튼 활성화 + 대화챗
void TownScene::QuestRockDialog_On(QuestDialogSituation::Dialog_NPC _NPC)
{
	m_NPCDialogChat->Update_On();
	m_NPCTexture2D->Update_On();

	switch (_NPC)
	{
	case QuestDialogSituation::NPC_ROCK:
		// 대화 활성화
		m_NPCNameTag->GetComponent<SpriteRenderer2D>()->SetSprite(L"DialogNPC_NameTags.png", 0);
		m_NPCTexture2D->GetComponent<SpriteRenderer2D>()->SetSprite(L"RockSpiritFace.png", 1);
		switch (m_CurQuestSituation)
		{
		case QuestDialogSituation::DIALOG_NONE:
			m_CurQuestSituation = QuestDialogSituation::DIALOG_ROCK_BEFORE;
		case QuestDialogSituation::DIALOG_ROCK_BEFORE:
		case QuestDialogSituation::DIALOG_ROCK_AFTER:
		case QuestDialogSituation::DIALOG_ROCK_QING:
		case QuestDialogSituation::DIALOG_ROCK_QDEFUSAL:
			switch (m_RockQuestPresent)
			{
			case 0:
				m_CurQuestSituation = QuestDialogSituation::DIALOG_ROCK_BEFORE;
				m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 0);
				break;
			case 1:
				m_CurQuestSituation = QuestDialogSituation::DIALOG_ROCK_QING;
				m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 3);
				break;
			case 2:
				m_CurQuestSituation = QuestDialogSituation::DIALOG_ROCK_QDEFUSAL;
				m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 2);
				break;
			case 3:
				m_CurQuestSituation = QuestDialogSituation::DIALOG_ROCK_AFTER;
				m_NPCTexture2D->GetComponent<SpriteRenderer2D>()->SetSprite(L"RockSpiritFace.png", 0);
				m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 1);
				break;
			default:
				break;
			}
			break;
		case QuestDialogSituation::DILAOG_TREE_BEFORE:
		case QuestDialogSituation::DIALOG_WIND_BEFORE:
			m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 0);
			break;
		default:
			break;
		}
		break;
	case QuestDialogSituation::NPC_SMALL:
		// 대화 활성화
		m_CurQuestSituation = QuestDialogSituation::DIALOG_WIND_BEFORE;
		m_NPCTexture2D->GetComponent<SpriteRenderer2D>()->SetSprite(L"SmallSpiritFace.png", 6);
		m_NPCNameTag->GetComponent<SpriteRenderer2D>()->SetSprite(L"DialogNPC_NameTags.png", 1);
		m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 4);
		break;
	case QuestDialogSituation::NPC_WIND:
		m_CurQuestSituation = QuestDialogSituation::DIALOG_WIND_BEFORE;
		m_NPCTexture2D->GetComponent<SpriteRenderer2D>()->SetSprite(L"WindSpiritFace.png", 0);
		m_NPCNameTag->GetComponent<SpriteRenderer2D>()->SetSprite(L"DialogNPC_NameTags.png", 2);
		m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 5);
		break;
	case QuestDialogSituation::NPC_TREE:
		m_CurQuestSituation = QuestDialogSituation::DILAOG_TREE_BEFORE;
		m_NPCTexture2D->GetComponent<SpriteRenderer2D>()->SetSprite(L"TreeSpiritFace.png", 0);
		m_NPCNameTag->GetComponent<SpriteRenderer2D>()->SetSprite(L"DialogNPC_NameTags.png", 3);
		m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 4);
		break;
	case QuestDialogSituation::NPC_NONE:
		m_NPCDialogChat->Update_Off();
		m_NPCTexture2D->Update_Off();
		return;
	default:
		break;

	}


	if (QuestDialogSituation::Dialog_Situation::DIALOG_NONE == m_CurQuestSituation)
		return;

	m_NPCDialogBaseBackground->Update_On();
	m_NPCNameTag->Update_On();

	m_QuestBtDialogExit->Update_On();
	m_QuestBtDialogExitCol->Update_On();

	// 버튼, 충돌체 활성화
	switch (m_CurQuestSituation)
	{
	case QuestDialogSituation::DIALOG_ROCK_BEFORE:
		m_QuestBtDialogYes->Update_On();
		m_QuestBtDialogNo->Update_On();
		m_QuestBtDialogCheck->Update_Off();

		m_QuestBtDialogYesCol->Update_On();
		m_QuestBtDialogNoCol->Update_On();
		m_QuestBtDialogCheckCol->Update_Off();
		break;
	case QuestDialogSituation::DIALOG_ROCK_AFTER:
	case QuestDialogSituation::DIALOG_ROCK_QING:
	case QuestDialogSituation::DIALOG_ROCK_QDEFUSAL:
	case QuestDialogSituation::DILAOG_TREE_BEFORE:
	case QuestDialogSituation::DIALOG_WIND_BEFORE:
		m_QuestBtDialogYes->Update_Off();
		m_QuestBtDialogNo->Update_Off();
		m_QuestBtDialogCheck->Update_On();

		m_QuestBtDialogYesCol->Update_Off();
		m_QuestBtDialogNoCol->Update_Off();
		m_QuestBtDialogCheckCol->Update_On();
		break;
	default:
		break;
	}

	//// 대화 활성화
	//switch (m_CurQuestSituation)
	//{
	//case QuestDialogSituation::DIALOG_ROCK_BEFORE:
	//	m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 0);
	//	break;
	//case QuestDialogSituation::DIALOG_ROCK_AFTER:
	//	m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 1);
	//	break;
	//case QuestDialogSituation::DIALOG_ROCK_QING:
	//	m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 3);
	//	break;
	//case QuestDialogSituation::DIALOG_ROCK_QDEFUSAL:
	//	m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 2);
	//	break;
	//case QuestDialogSituation::DILAOG_TREE_BEFORE:
	//	m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 4);
	//	break;
	//case QuestDialogSituation::DIALOG_WIND_BEFORE:
	//	m_NPCDialogChat->GetComponent<SpriteRenderer2D>()->SetSprite(L"Dialog_NPCchat.png", 5);
	//	break;
	//default:
	//	break;
	//}
}

void TownScene::QuestAlarm_Off()
{
	m_QuestAlarmBG->Update_Off();
	m_QuestAlarmManaElixirNumbersScript->AllNumber_Off();
}

void TownScene::QuestAlarm_On()
{
	m_QuestAlarmBG->Update_On();
	m_QuestAlarmManaElixirNumbersScript->AllNumber_On();
}


void TownScene::ClickItemInfo(GameObject* _ItemObj)
{
	m_ClickItemInfo = _ItemObj;
}

void TownScene::InventorySlotOff(std::vector<GameObject*> _Slot)
{
	for (size_t i = 0; i < _Slot.size(); i++)
	{
		//_Slot[i]->Update_Off();
		_Slot[i]->GetComponent<RectCol2D>()->Update_Off();
		_Slot[i]->GetComponent<Inven_Info>()->Number_Off();
		_Slot[i]->GetComponent<Inven_Info>()->RenderOff();
	}
}

void TownScene::InventorySlotOn(std::vector<GameObject*> _Slot)
{
	for (size_t i = 0; i < _Slot.size(); i++)
	{
		//_Slot[i]->Update_On();
		_Slot[i]->GetComponent<Inven_Info>()->Number_On();
		_Slot[i]->GetComponent<Inven_Info>()->RenderOn();
		_Slot[i]->GetComponent<RectCol2D>()->Update_On();
	}
}


////////////////////////////////////////////////////////////

// 충돌
void TownScene::ColTestEnter(Collision* _This, Collision* _Other)
{

	//_This->GetGameObject()->Death();		// 부딪히면 사망하게하기

											//_Other->GetTransform()->Local_Scale(_Other->GetTransform()->Local_Scale() * 0.8f);		// 점점 줄어들게하기

}

void TownScene::ColTestStay(Collision* _This, Collision* _Other)
{
	int a = 0;
	//_Other->GetTransform()->Local_Scale(_Other->GetTransform()->Local_Scale() * 0.8f);		// 점점 줄어들게하기

}

void TownScene::ColTestExit(Collision* _This, Collision* _Other)
{
	int a = 0;
}

// 포탈 충돌
void TownScene::ColPortalEnter(Collision* _This, Collision* _Other)
{
}

void TownScene::ColPortalStay(Collision* _This, Collision* _Other)
{
	if (_This == m_PlayerCol)
	{
		if (_Other == m_PrePortalCol)
		{
			if (true == Input::Down(L"Key_Space"))
			{
				// 씬 옮기기 - 에스페라
				// 정보들 옮겨주기(내일추가)
				// 정보들을 옮겨준 후에 씬을 이동시킨다
				SoundPlay_Effect(L"Portal.mp3");
				// 인벤토리 정보
				MapleGlobalValue::SetInvenItemConsume(m_InvenItemConsum);
				MapleGlobalValue::SetQuickSlotVector(m_QuickSlotVector);

				Vector4 ChangePos = m_GamePlayer->GetTransform()->Local_Position();
				ChangePos.x = 1048.0f;
				ChangePos.y = -300.0f;

				MapleGlobalValue::PlayerInfo(ChangePos, 0, 0, 0);
				MapleGlobalValue::SetCameraMovePos(Vector4(650.0f, -140.0f, 0.0f, 0.0f));
				m_BGMPlayer->Stop_BGM();
				Scene::Scene_Change(L"ChewChewIslandScene");
				//Scene::Scene_Change(L"EsferaScene");
			}
		}
		else if (_Other == m_NextPortalCol)
		{
			if (true == Input::Down(L"Key_Space"))
			{
				// 씬 옮기기 -  츄츄 아일랜드
				//MonsterStageScene2* EsferaSceneScript = Scene::Script_Find<MonsterStageScene2>(L"ChewChewIslandScene");

				Scene::Scene_Change(L"EsferaScene");
			}
		}
	}
}

void TownScene::ColPortalExit(Collision* _This, Collision* _Other)
{

}


// UI 충돌
void TownScene::Col_InventoryEnter(Collision* _This, Collision* _Other)
{
	if (_This == CursorCol)
	{
		if (_Other == InvenMoveCol)
		{
		}

		if (_Other == InvenBtCloseCol)
		{
			// 닫기
			InvenBtCloseAniScript->SetAnimation(L"MouseOver");

		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			InvenBtDisassembleAniScript->SetAnimation(L"MouseOver");

		}
		else if (_Other == InvenBtExtractCol)
		{
			// 물약버튼
			InvenBtExtractAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == InvenBtAppraiseCol)
		{
			// 돋보기
			InvenBtAppraiseAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			InvenBtToadAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			InvenBtUpgradeAniScript->SetAnimation(L"MouseOver");

		}
		else if (_Other == InvenBtEquipCol)
		{
			// 장비

		}
		else if (_Other == InvenBtComsumCol)
		{
			// 소비

		}
		else if (_Other == InvenBtEtcCol)
		{
			// 기타

		}
		else if (_Other == InvenBtInstallCol)
		{
			// 설치

		}
		else if (_Other == InvenBtCashCol)
		{
			// 캐시
		}


		/////////////////////////////////////////////////////////////// 상점
		if (_Other == m_ShopExitCol)
		{
			// 상점나가기
			m_ShopExitAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == m_ShopItemBuyCol)
		{
			// 아이템 사기
			m_ShopItemBuyAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == m_ShopItemSellCol)
		{
			// 아이템 팔기
			m_ShopItemSellAniScript->SetAnimation(L"MouseOver");
		}


		/////////////////////////////////////////////////////////////// 퀘스트
		if (_Other == m_QuestBtDialogExitCol)
		{
			m_QDialogExitBtAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == m_QuestBtDialogYesCol)
		{
			m_QDialogYesBtAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == m_QuestBtDialogNoCol)
		{
			m_QDialogNoBtAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == m_QuestBtDialogCheckCol)
		{
			m_QDialogCheckAniScript->SetAnimation(L"MouseOver");
		}


	}
}

void TownScene::Col_InventoryStay(Collision* _This, Collision* _Other)
{
	if (_This == CursorCol)
	{
		if (_Other == InvenMoveCol)
		{
			if (true == Input::DownStay(L"MouseLeftButton"))
			{
				InvenMoveObjectScript->SetbMove(true);
			}
		}

		if (_Other == InvenBtCloseCol)
		{
			// 닫기
			if (true == Input::Down(L"MouseLeftButton"))
			{
				Vector4 CloseMoveInven = Vector4(0.0f, 1000.0f, 0.0f, 0.0f);
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtCloseAniScript->SetAnimation(L"Pressed");
				InventoryOff();
			}
		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			if (true == Input::Down(L"MouseLeftButton"))
			{
				InvenBtDisassembleAniScript->SetAnimation(L"Pressed");
				SoundPlay_Effect(L"btClick.mp3");
			}
		}
		else if (_Other == InvenBtExtractCol)
		{
			// 물약버튼
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtExtractAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtAppraiseCol)
		{
			// 돋보기
			if (true == Input::Down(L"MouseLeftButton"))
			{
				InvenBtAppraiseAniScript->SetAnimation(L"Pressed");
				SoundPlay_Effect(L"btClick.mp3");
			}
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			if (true == Input::Down(L"MouseLeftButton"))
			{
				InvenBtToadAniScript->SetAnimation(L"Pressed");
				SoundPlay_Effect(L"btClick.mp3");
			}
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			if (true == Input::Down(L"MouseLeftButton"))
			{
				InvenBtUpgradeAniScript->SetAnimation(L"Pressed");
				SoundPlay_Effect(L"btClick.mp3");
			}
		}
		else if (_Other == InvenBtEquipCol)
		{
			// 장비
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");

				// 버튼 누르기
				InvenBtEquipAniScript->On();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOn(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtComsumCol)
		{
			// 소비
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");

				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->On();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOn(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtEtcCol)
		{
			// 기타
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");

				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->On();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOn(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtInstallCol)
		{
			// 설치
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");

				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->On();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOn(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtCashCol)
		{
			// 캐시
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");

				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->On();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOn(m_InvenItemCash);
			}
		}


		// 클릭한 아이템 정보 - 인벤
		for (size_t i = 0; i < InvenItemConSumCol.size(); ++i)
		{
			// 소비
			if (_Other == InvenItemConSumCol[i])
			{
				if (true == Input::Down(L"MouseLeftButton"))
				{
					SoundPlay_Effect(L"btClick.mp3");

					if (nullptr == m_ClickItemInfo)
					{
						m_ClickItemInfo = m_InvenItemConsum[i];
						m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(m_ClickItemInfo->GetComponent<Inven_Info>()->GetItemCode());

						if (L"EmptyItem.png" == m_ClickItemInfo->GetComponent<Inven_Info>()->GetInfo()->ItemName)
						{
							m_ClickItemInfo = nullptr;
							return;
						}

					}
					else
					{
						// 이미지 체인지하기 (아이템 스왑)
						ItemCode::ItemCodeName TempItem = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCode();
						ItemCode::ItemCodeName SelectItem = m_ClickItemInfo->GetComponent<Inven_Info>()->GetItemCode();

						// 개수 알기
						int TempItemCount = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCount();
						int SelectCount = m_ClickItemInfo->GetComponent<Inven_Info>()->GetItemCount();

						// 아이템 개수 변경 ( 추후 같은 이미지라면 합치기 )
						// 같은이미지의 경우와 다른 이미지의 경우로 나누어서 생각한다.
						std::wstring TempItemName = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetInfo()->ItemName;
						std::wstring SelectItemName = m_ClickItemInfo->GetComponent<Inven_Info>()->GetInfo()->ItemName;

						if (TempItemName == SelectItemName)
						{
							// 동일한 이미지라면 -> 개수가 999개를 초과하는지 검사해야 한다.
							// 초과시 남은 여분의 아이템들은 기존의 아이템칸에 있는다.

							int TotalCount = TempItemCount + SelectCount;

							if (TotalCount > 999)
							{
								// 999개 초과
								m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(999);
								TotalCount -= 999;
								m_ClickItemInfo->GetComponent<Inven_Info>()->SetItemCount(TotalCount);
							}
							else
							{
								// 999개 이하
								m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(TotalCount);
								m_ClickItemInfo->GetComponent<Inven_Info>()->SetItemCount(0);

								// 아이템 이미지 변경
								m_ClickItemInfo->GetComponent<Inven_Info>()->ItemAdd(ItemCode::ItemCodeName::EMPTY);

							}

						}
						else
						{
							m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(SelectCount);
							m_ClickItemInfo->GetComponent<Inven_Info>()->SetItemCount(TempItemCount);

							// 아이템 이미지 변경
							m_InvenItemConsum[i]->GetComponent<Inven_Info>()->ItemAdd(SelectItem);
							m_ClickItemInfo->GetComponent<Inven_Info>()->ItemAdd(TempItem);
						}

						// 빈 이미지인지 확인 -> 숫자 제거
						std::wstring InvenSlotItemName = m_ClickItemInfo->GetComponent<Inven_Info>()->GetInfo()->ItemName;
						if (L"EmptyItem.png" == InvenSlotItemName)
						{
							m_ClickItemInfo->GetComponent<Inven_Info>()->SetEmptyTexture();
						}

						// 마우스 따라다니는 이미지 지우기
						m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
						m_ClickItemInfo = nullptr;

					}

				}
			}
		}

		// 클릭한거.. 퀵슬롯 관련
		for (size_t i = 0; i < m_QuickSlotCol.size(); ++i)
		{
			if (_Other == m_QuickSlotCol[i])
			{
				// 클릭한게 인벤에서 클릭한건지 퀵슬롯에서 클릭한건지 검사
				// 선 인벤 후 퀵슬롯 -> 인벤토리의 소비템정보를 퀵슬롯에 넣어주기..
				// 선 퀵슬롯 후 퀵슬롯 -> 퀵슬롯의 정보를 서로 스왑
				if (true == Input::Down(L"MouseLeftButton"))
				{
					SoundPlay_Effect(L"btClick.mp3");

					if (nullptr == m_ClickItemInfo)
					{
						// 선 퀵슬롯 -> 퀵슬롯의 정보를 알려준다. 인벤에서 만약 이 정보가 퀵슬롯일 경우 인벤에 넣을떄 불가능하게한다.
						m_ClickItemInfo = m_QuickSlotVector[i];			// 퀵슬롯의 정보
						m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName());

						if (ItemCode::EMPTY == m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName())
						{
							m_ClickItemInfo = nullptr;
							return;
						}

					}
					else
					{
						// 후 퀵슬롯 or 인벤 -> 선클릭한게 퀵슬롯인지 인벤인지 확인

						// 인벤
						for (size_t k = 0; k < m_InvenItemConsum.size(); ++k)
						{
							GameObject* FirstClickObj = m_InvenItemConsum[k];
							GameObject* MouseClickInfo = m_ClickItemInfo;

							// 선 인벤 후 퀵슬롯
							if (MouseClickInfo == FirstClickObj)
							{
								// 만약 클릭한 퀵슬롯자리가 스킬이라면 그냥 패스
								ItemCode::ItemCodeName CurQuickSlotClick = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

								// 스킬이라면
								if (CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BE
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BZ
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_CC
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_PM
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_SP)
								{
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;
									return;
								}
								else
								{
									// 스킬이 아니라면 물약자리인지 빈칸인지.. 2종류임 (퀵슬롯 자리) 일단 인벤->퀵슬롯이니까 100% 소비템임
									// but 필요없음! 그냥 바꿔주기
									ItemCode::ItemCodeName SelectItemCode = FirstClickObj->GetComponent<Inven_Info>()->GetItemCode();		// 코드
									std::wstring SelectName = FirstClickObj->GetComponent<Inven_Info>()->GetInfo()->ItemName;


									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(SelectItemCode);
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(m_InvenItemConsum[k]);		// 안에서 카운트까지해줌
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetSkillName(L"");

									// 바꾼후 마우스 이미지 원래대로..
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;

									return;
								}
							}
						}

						// 퀵슬롯
						for (size_t q = 0; q < m_QuickSlotVector.size(); ++q)
						{
							GameObject* FirstClickObj = m_QuickSlotVector[q];
							GameObject* MouseClickInfo = m_ClickItemInfo;

							// 선 퀵 후 퀵
							if (MouseClickInfo == FirstClickObj)
							{
								// 퀵슬롯에서 자리바꾸는건 스킬, 물약 제약 받지 않고 이동한다.
								ItemCode::ItemCodeName CurQuickSlotClick = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

								// 스킬이라면
								if (CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BE
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BZ
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_CC
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_PM
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_SP)
								{
									// 선 스킬 후 스킬
									// 선 스킬 후 물약 일경우 따진다
									ItemCode::ItemCodeName LastItemCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();

									if (CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BE
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BZ
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_CC
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_PM
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_SP)
									{
										// 스킬은 쿨타임, 스킬이름 변경, 코드 변경
										// 후클릭 이미지 임시로 저장
										float TempCoolTime = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTime();
										std::wstring TempSkillName = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillName();
										ItemCode::ItemCodeName TempCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

										// m_QuickSlotVector[i]는 후 클릭
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetSkillName());
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCoolTime());
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName());

										// 선클릭한것도 바꿔주기
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(TempSkillName);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(TempCoolTime);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempCode);
									}
									else
									{
										// 후 물약
										float TempCoolTime = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTime();
										std::wstring TempSkillName = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillName();
										ItemCode::ItemCodeName TempCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

										ItemCode::ItemCodeName TempFirstCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempFirstInventoryItemObj = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetInventoryItem();

										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempFirstCode);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(TempFirstInventoryItemObj);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(L"");

										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(TempCoolTime);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(TempSkillName);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillManager(m_SkillManager);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(TempSkillName);

									}

									// 바꾼후 마우스 이미지 원래대로..
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;
								}
								else
								{
									// 선클릭이 물약일 경우
									// 후 클릭이 물약인지, 스킬인지 체크해야한다
									ItemCode::ItemCodeName LastClickItemCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();

									if (ItemCode::ItemCodeName::SKILL_BE == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_BZ == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_CC == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_PM == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_SP == LastClickItemCode)
									{
										// 후클릭이 스킬일 경우  선물 후스
										ItemCode::ItemCodeName TempFirstCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempFirstInventoryItemObj = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetInventoryItem();

										float TempLastCoolTime = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCoolTime();
										std::wstring TempLastSkillName = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetSkillName();
										ItemCode::ItemCodeName TempLastCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();

										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(TempLastCoolTime);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempLastCode);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(m_SkillManager);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(TempLastSkillName);

										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempFirstCode);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(TempFirstInventoryItemObj);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(L"");


									}
									else
									{
										// 후클릭이 물약일 경우 or 빈칸
										ItemCode::ItemCodeName TempFirstCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempFirstInventoryItemObj = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetInventoryItem();

										ItemCode::ItemCodeName TempLastCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempLastInventoryItemObj = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetInventoryItem();

										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempLastCode);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(TempLastInventoryItemObj);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(L"");

										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempFirstCode);

										if (ItemCode::ItemCodeName::EMPTY != TempFirstCode)
										{
											m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(TempFirstInventoryItemObj);
										}
										else
										{
											m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
										}
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(L"");
									}

									// 바꾼후 마우스 이미지 원래대로..
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;

								}
							}
						}
					}
				}
			}
		}

		/////////////////////////////////////////////////////////////// 상점
		if (_Other == m_ShopEquipCol)
		{
			// 장비
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopMyItem_Off();

				m_ShopEquipAniScript->On();
				m_ShopConsumeAniScript->Off();
				m_ShopEtcAniScript->Off();
				m_ShopInstallAniScript->Off();
				m_ShopCashAniScript->Off();
			}
		}
		else if (_Other == m_ShopConsumeCol)
		{
			// 소비
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopMyItem_On();

				m_ShopEquipAniScript->Off();
				m_ShopConsumeAniScript->On();
				m_ShopEtcAniScript->Off();
				m_ShopInstallAniScript->Off();
				m_ShopCashAniScript->Off();
			}
		}
		else if (_Other == m_ShopEtcCol)
		{
			// 기타
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopMyItem_Off();

				m_ShopEquipAniScript->Off();
				m_ShopConsumeAniScript->Off();
				m_ShopEtcAniScript->On();
				m_ShopInstallAniScript->Off();
				m_ShopCashAniScript->Off();
			}
		}
		else if (_Other == m_ShopInstallCol)
		{
			// 설치
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopMyItem_Off();

				m_ShopEquipAniScript->Off();
				m_ShopConsumeAniScript->Off();
				m_ShopEtcAniScript->Off();
				m_ShopInstallAniScript->On();
				m_ShopCashAniScript->Off();
			}
		}
		else if (_Other == m_ShopCashCol)
		{
			// 캐시
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopMyItem_Off();

				m_ShopEquipAniScript->Off();
				m_ShopConsumeAniScript->Off();
				m_ShopEtcAniScript->Off();
				m_ShopInstallAniScript->Off();
				m_ShopCashAniScript->On();
			}
		}
		else if (_Other == m_ShopAllCol)
		{
			// 전체
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopBuyAllItemOn();
				m_ShopAllAniScript->On();
				m_ShopRebuyAniScript->Off();
			}
		}
		else if (_Other == m_ShopRebuyCol)
		{
			// 재구매
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				m_ShopAllAniScript->Off();
				m_ShopRebuyAniScript->On();
				ShopBuyAllItemOff();
			}
		}
		else  if (_Other == m_ShopExitCol)
		{
			// 상점나가기ShopOff
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopOff();
				m_ShopBuySelectItem = L"";
			}
		}
		else if (_Other == m_ShopItemBuyCol)
		{
			// 아이템 사기
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"casher.mp3");

				if (L"ShopBuyItem_RedPotion" == m_ShopBuySelectItem
					|| L"ShopBuyItem_BluePotion" == m_ShopBuySelectItem
					|| L"ShopBuyItem_OrangePotion" == m_ShopBuySelectItem
					|| L"ShopBuyItem_WhitePotion" == m_ShopBuySelectItem
					|| L"ShopBuyItem_ManaElixir" == m_ShopBuySelectItem)
				{
					ShopBuyItemManager(m_ShopBuySelectItem);
				}
			}
		}
		else if (_Other == m_ShopItemSellCol)
		{
			// 아이템 팔기
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"casher.mp3");
				ShopSellItemManager(m_shopSellSelectItem);
			}
		}

		// 상점 아이템
		// 좌클릭시 아이템 선택
		// 우클릭시 아이템 바로 사기
		if (_Other == m_ShopBuyRedPotionCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				ShopBuyItemAniChange_None();
				m_ShopBuyRedPoitonAniScript->SetAnimation(L"ClickPressed");
				m_ShopBuySelectItem = L"ShopBuyItem_RedPotion";
			}
		}
		else if (_Other == m_ShopBuyBluePotionCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopBuyItemAniChange_None();
				m_ShopBuyBluePoitonAniScript->SetAnimation(L"ClickPressed");
				m_ShopBuySelectItem = L"ShopBuyItem_BluePotion";
			}
		}
		else if (_Other == m_ShopBuyOrangePoitonCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopBuyItemAniChange_None();
				m_ShopBuyOrangePoitonAniScript->SetAnimation(L"ClickPressed");
				m_ShopBuySelectItem = L"ShopBuyItem_OrangePotion";
			}
		}
		else if (_Other == m_ShopBuyWhitePotionCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopBuyItemAniChange_None();
				m_ShopBuyWhitePoitonAniScript->SetAnimation(L"ClickPressed");
				m_ShopBuySelectItem = L"ShopBuyItem_WhitePotion";
			}
		}
		else if (_Other == m_ShopBuyElixirCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				ShopBuyItemAniChange_None();
				m_ShopBuyManaElxirAniScript->SetAnimation(L"ClickPressed");
				m_ShopBuySelectItem = L"ShopBuyItem_ManaElixir";
			}
		}


		// 내가 팔 수 있는 아이템 클릭
		for (size_t i = 0; i < 5; i++)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				if (_Other == m_ShopSellMyItemCol[i])
				{
					SoundPlay_Effect(L"btClick.mp3");
					std::vector<GameObject*> ClickMySellItem = m_ShopSellMyItemManagerScript->GetShopInvenMyItem();

					ShopSellItemAniChange_None();
					m_ShopSellMyItemAniScript[i]->SetAnimation(L"ClickPressed");
					m_shopSellSelectItem = ClickMySellItem[i];
					CurSelectShopMyItemOrder = (int)(i);
				}
			}
		}



		/////////////////////////////////////////////////////////////// 퀘스트
		if (_Other == m_QuestBtDialogExitCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				m_QDialogExitBtAniScript->SetAnimation(L"Pressed");

				switch (m_DialogNPC)
				{
				case QuestDialogSituation::NPC_ROCK:
					if (2 == m_RockQuestPresent)
					{
						m_RockQuestPresent = 0;
					}
					break;
				default:
					m_CurQuestSituation = QuestDialogSituation::DIALOG_NONE;
					break;
				}

				QuestDialog_AllOff();
			}
		}
		else if (_Other == m_QuestBtDialogYesCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				SoundPlaye_Voice(L"RPSgame.mp3");
				m_QDialogYesBtAniScript->SetAnimation(L"Pressed");

				switch (m_DialogNPC)
				{
				case QuestDialogSituation::NPC_ROCK:
					QuestAlarm_On();
					RockQuestAlarmScript->ChangeAnimation(L"Quest_Doing");
					switch (m_CurQuestSituation)
					{
					case QuestDialogSituation::DIALOG_ROCK_BEFORE:
						m_RockQuestPresent = 1;
						break;
					}
					break;
				}
				QuestDialog_AllOff();

			}
		}
		else if (_Other == m_QuestBtDialogNoCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				SoundPlaye_Voice(L"7.mp3");
				m_QDialogNoBtAniScript->SetAnimation(L"Pressed");

				switch (m_DialogNPC)
				{
				case QuestDialogSituation::NPC_ROCK:
					switch (m_CurQuestSituation)
					{
					case QuestDialogSituation::DIALOG_ROCK_BEFORE:
						m_RockQuestPresent = 2;
						QuestRockDialog_On(m_DialogNPC);
						m_RockQuestPresent = 0;
						break;
					}
					break;
				}
			}
		}
		else if (_Other == m_QuestBtDialogCheckCol)
		{
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				m_QDialogCheckAniScript->SetAnimation(L"Pressed");
				QuestDialog_AllOff();


				switch (m_DialogNPC)
				{
				case QuestDialogSituation::NPC_ROCK:
					break;
				case QuestDialogSituation::NPC_SMALL:
				case QuestDialogSituation::NPC_WIND:
				case QuestDialogSituation::NPC_TREE:
				case QuestDialogSituation::NPC_NONE:
					m_CurQuestSituation = QuestDialogSituation::DIALOG_NONE;
					break;
				default:
					break;
				}
			}
		}

	}
}


void TownScene::Col_InventoryExit(Collision* _This, Collision* _Other)
{
	if (_This == CursorCol)
	{
		// 인벤토리
		if (_Other == InvenMoveCol)
		{
		}

		if (_Other == InvenBtCloseCol)
		{
			// 닫기
			InvenBtCloseAniScript->SetAnimation(L"Normal");

		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			InvenBtDisassembleAniScript->SetAnimation(L"Normal");

		}
		else if (_Other == InvenBtExtractCol)
		{
			// 물약버튼
			InvenBtExtractAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == InvenBtAppraiseCol)
		{
			// 돋보기
			InvenBtAppraiseAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			InvenBtToadAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			InvenBtUpgradeAniScript->SetAnimation(L"Normal");

		}
		else if (_Other == InvenBtEquipCol)
		{
			// 장비

		}
		else if (_Other == InvenBtComsumCol)
		{
			// 소비

		}
		else if (_Other == InvenBtEtcCol)
		{
			// 기타

		}
		else if (_Other == InvenBtInstallCol)
		{
			// 설치

		}
		else if (_Other == InvenBtCashCol)
		{
			// 캐시
		}


		/////////////////////////////////////////////////////////////// 상점
		if (_Other == m_ShopExitCol)
		{
			// 상점나가기
			m_ShopExitAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == m_ShopItemBuyCol)
		{
			// 아이템 사기
			m_ShopItemBuyAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == m_ShopItemSellCol)
		{
			// 아이템 팔기
			m_ShopItemSellAniScript->SetAnimation(L"Normal");
		}


		/////////////////////////////////////////////////////////////// 퀘스트
		if (_Other == m_QuestBtDialogExitCol)
		{
			m_QDialogExitBtAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == m_QuestBtDialogYesCol)
		{
			m_QDialogYesBtAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == m_QuestBtDialogNoCol)
		{
			m_QDialogNoBtAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == m_QuestBtDialogCheckCol)
		{
			m_QDialogCheckAniScript->SetAnimation(L"Normal");
		}

	}
}

// 퀘스트
void TownScene::Col_QuestEnter(Collision* _This, Collision* _Other)
{

}

void TownScene::Col_QuestStay(Collision* _This, Collision* _Other)
{
	if (_This == InGameCursorCol)
	{
		//  NPC 관련 - 상점, 퀘스트
		for (size_t i = 0; i < m_ColNPCVec.size(); i++)
		{
			if (_Other == m_ColNPCVec[i])
			{

				std::wstring OtherColGroupName = _Other->GetGroupName();

				if (L"Col_WindSpiritNPC" == OtherColGroupName)
				{
					m_DialogNPC = QuestDialogSituation::Dialog_NPC::NPC_WIND;
				}
				else if (L"Col_RockSpiritNPC" == OtherColGroupName)
				{
					m_DialogNPC = QuestDialogSituation::Dialog_NPC::NPC_ROCK;
				}
				else if (L"Col_TreeSpiritNPC" == OtherColGroupName)
				{
					m_DialogNPC = QuestDialogSituation::Dialog_NPC::NPC_TREE;
				}
				else if (L"Col_SmallSpiritNPC" == OtherColGroupName)
				{
					m_DialogNPC = QuestDialogSituation::Dialog_NPC::NPC_SMALL;
				}



				if (true == Input::Down(L"MouseLeftButton"))
				{
					InventoryOff();
					SoundPlay_Effect(L"btClick.mp3");

					std::wstring OtherColGroupName = _Other->GetGroupName();

					if (L"Col_WindSpiritNPC" == OtherColGroupName)
					{
						SoundPlaye_Voice(L"rise.mp3");
						QuestRockDialog_On(m_DialogNPC);
						return;
					}
					else if (L"Col_RockSpiritNPC" == OtherColGroupName)
					{
						SoundPlaye_Voice(L"lonely.mp3");
						if (1 == m_RockQuestPresent && true == m_QuestAlarmManaElixirNumbersScript->GetQuestComp())
						{
							// 퀘스트 완료 - NPC_RockSpirit
							m_RockSpiritNPC->GetComponent<NPC_RockSpirit>()->ChangeAnimation(L"Free");
							QuestAlarm_Off();
							m_RockQuestAlarm->Update_Off();
							m_RockQuestPresent = 3;
						}
						else
						{
							QuestRockDialog_On(m_DialogNPC);
						}
						return;
					}
					else if (L"Col_TreeSpiritNPC" == OtherColGroupName)
					{
						SoundPlaye_Voice(L"4.mp3");
						QuestRockDialog_On(m_DialogNPC);
						return;
					}
					else if (L"Col_SmallSpiritNPC" == OtherColGroupName)
					{
						SoundPlaye_Voice(L"8.mp3");
						QuestRockDialog_On(m_DialogNPC);
						return;
					}
					else if (L"Col_PotionNPC" == OtherColGroupName)
					{
						// 포션 상점
						ShopOn();
						return;
					}
					else if (L"Col_WarehouseNPC" == OtherColGroupName)
					{
						return;
					}
				}
			}
		}
	}


}

void TownScene::Col_QuestExit(Collision* _This, Collision* _Other)
{
	//  NPC 관련 - 상점, 퀘스트
	for (size_t i = 0; i < m_ColNPCVec.size(); i++)
	{
		if (_Other == m_ColNPCVec[i])
		{
			MouseScript->MouseStateChange(L"Base");
			return;
		}
	}
}


void TownScene::DebugRender()
{
	//GameDebug::DrawStr(L"하하하ㅏ 크햐햐햐햐햫ㅅ 폰트 나쁜넘ㅁ 으아얌ㄹ너아러ㅑㄴ거ㅑㅇㄴㅁㄹ 너시러", 20.0f, { 100.f, 40.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}

void TownScene::SoundPlay_BGM(std::wstring _SoundFileName)
{
	m_BGMPlayer->Play_BGM(_SoundFileName.c_str());
}

void TownScene::SoundPlay_Effect(std::wstring _SoundFileName)
{
	m_EffectSoundPlayer->Play(_SoundFileName.c_str());
}

void TownScene::SoundPlaye_Voice(std::wstring _SoundFileName)
{
	m_VoiceSoundPlayer->Play(_SoundFileName.c_str());
}

//
void TownScene::BGMSoundLoading()
{
	// 사운드플레이
	m_BGMEmptyObject = GetScene()->GameObject_Create(L"BGMPlayer");
	m_BGMEmptyObject->GetTransform()->Local_Position(Vector4::Zero);
	m_BGMEmptyObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_BGMPlayer = m_BGMEmptyObject->Component_Add<SoundPlayer>();
}

void TownScene::EffectSoundLoading()
{
	m_EffectSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_EffectSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_EffectSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_EffectSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}

void TownScene::VoiceSoundLoading()
{
	m_VoiceSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_VoiceSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_VoiceSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_VoiceSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}