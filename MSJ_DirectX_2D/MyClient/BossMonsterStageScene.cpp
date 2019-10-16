#include "BossMonsterStageScene.h"
#include <Scene.h>
#include <TownPortalAniScript.h>

#include <PinkbinRegen.h>
#include <Pinkbin.h>

#include <InGameCamera.h>

#include <PlayerMove.h>
#include <PlayerAnimation.h>
#include <SkillManager.h>
#include <UIMove.h>
#include <BossMonster_HP.h>

#include <Texture.h>
#include <Sprite.h>
#include <RectCol2D.h>
#include <GameDebug.h>
#include <InGameCamera.h>

#include <MapleGlobalValue.h>

// 인벤토리
#include <Inven_BtAnimation.h>
#include <Inven_BtKind.h>
#include <Inven_Info.h>
#include <ItemCode.h>
#include <Inven_MouseItem.h>
#include <Inven_MoveScript.h>

// 돈
#include <MesoManager.h>

// 커서
#include <MouseCursorScript.h>
#include <WorldMouseCursorScript.h>

// 퀵슬롯
#include <QuickSlotScript.h>
#include <CoolTimeScript.h>

// 몬스터힛
#include <MonsterHitScript.h>

// 데미지스킨
#include <MonsterDamageSkin.h>

// 아이템
#include <ItemMoveScript.h>

// 배경이동
#include <BG_PinkbinCloudMove.h>

#include <PinkbinHPBar.h>
#include <SoundPlayer.h>

BossMonsterStageScene::BossMonsterStageScene()
	: m_Background(nullptr), m_Cloud(nullptr), m_GrayCloud(nullptr), m_OrangeCloud(nullptr), m_WhiteCloud(nullptr), m_TempleCloud(nullptr), m_TempleObject(nullptr)
	, m_MoveTile(nullptr), m_BossPinkbin(nullptr), m_PrePortal(nullptr), m_GamePlayer(nullptr)
	, m_StatusBarBackground(nullptr), m_StatusBarBase(nullptr), m_StatusBarGauge(nullptr), m_HPGray(nullptr), m_MPGray(nullptr), m_ExpGray(nullptr), m_GaugeGraduation(nullptr)
	, m_PlayerCoatArm(nullptr), m_PlayerArm(nullptr), m_PlayerEye(nullptr), m_PlayerHair(nullptr), m_PlayerHead(nullptr), m_PlayerCoat(nullptr), m_PlayerBody(nullptr)
	, m_PlayerMoveScript(nullptr), m_SkillManager(nullptr)
	, m_BossHPBarBase(nullptr), m_BossHPBarFirstLine(nullptr), m_BossHPBarSecondLine(nullptr), m_BossHPBarGray(nullptr), m_ConnectPinkbin(false)
{
}


BossMonsterStageScene::~BossMonsterStageScene()
{
}


bool BossMonsterStageScene::Loading()
{
	// 사운드
	BGMSoundLoading();
	EffectSoundLoading();

	CollisionNameLink();

	BGLoading();
	UILoading();
	PortalLoading();
	CreateCamera();
	PlayerLoading();
	CameraScriptLoading();
	MonsterLoading();
	BossHPBarLoading();

	// 인벤토리
	InventoryLoading();

	// 퀵슬롯
	QuickSlotLoading();


	// 마우스
	MouseLoading();

	// 돈 관련
	MoneyLoading();


	// 인벤토리 끄기
	InventoryOff();

	NearCloudScript->SetPlayer(m_GamePlayer);


	return true;
}

void BossMonsterStageScene::Start()
{
	SoundPlay_BGM(L"ArcanaBoss.mp3");
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_MainCamera->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));


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
				// 나도 쿨타임 넣어준다
				float NewCurCoolTime = NewCoolTimeScirpt->GetCurCoolTime();
				int NewCurAniFrame = NewCoolTimeScirpt->GetCurCoolTimeFrame();

				CoolTimeScript* NewCoolTimeScript = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTimeScript();
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

	// 캐릭터 포스
	m_GamePlayer->GetTransform()->Local_Position(MapleGlobalValue::PlayerPostion);
	m_CamScript->SetMovePos(MapleGlobalValue::CameraMovePos);
}

void BossMonsterStageScene::Update()
{
	if (false == m_ConnectPinkbin)
	{
		PinkbinSet();
	}
}

void BossMonsterStageScene::LateUpdate()
{

}

void BossMonsterStageScene::End()
{

}

void BossMonsterStageScene::DebugRender()
{
	// FPS
	//wchar_t CurFPS[256];
	//swprintf_s(CurFPS, L"FPS = %d", Time::FPS());
	//GameDebug::DrawStr(CurFPS, 20, { 10.0f, 30.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	//// 캐릭터 포스
	//wchar_t PosXString[256];
	//float PlayerX = m_GamePlayer->GetTransform()->Local_Position().x;
	//swprintf_s(PosXString, L"X = %d", (int)PlayerX);
	//GameDebug::DrawStr(PosXString, 20, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	//wchar_t PosYString[256];
	//float PlayerY = m_GamePlayer->GetTransform()->Local_Position().y;
	//swprintf_s(PosYString, L"Y = %d", (int)PlayerY);
	//GameDebug::DrawStr(PosYString, 20, { 150.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}


//

void BossMonsterStageScene::BGLoading()
{
	// 뒷배경
	m_Background = GetScene()->GameObject_Create(L"TimeTempleBoss_Bacground");
	m_Background->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 300.0f));
	m_Background->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_Background->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BACKGROUND)->SetSprite(L"TOG_BG.png", 0);
	My_PTR<Animation2D> BGANi = m_Background->Component_Add<Animation2D>();

	// 주황색 구름
	m_OrangeCloud = GetScene()->GameObject_Create(L"TimeTempleBoos_OrangeCloud");
	m_OrangeCloud->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 298.0f));
	m_OrangeCloud->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_OrangeCloud->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer02)->SetSprite(L"TOG_OrangeCloud.png", 0);
	My_PTR<Animation2D> OrangeCloudAni = m_OrangeCloud->Component_Add<Animation2D>();

	// 잿빛 구름
	m_GrayCloud = GetScene()->GameObject_Create(L"TimeTempleBoss_GrayCloud");
	m_GrayCloud->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 296.0f));
	m_GrayCloud->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_GrayCloud->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer03)->SetSprite(L"TOG_GrayCloud.png", 0);
	My_PTR<Animation2D> GrayCloudAni = m_GrayCloud->Component_Add<Animation2D>();

	// 작은 신전들 + 구름
	m_TempleCloud = GetScene()->GameObject_Create(L"TimeTemple_TempleCloud");
	m_TempleCloud->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 298.0f));
	m_TempleCloud->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_TempleCloud->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer04)->SetSprite(L"TOG_TempleCloud.png", 0);
	My_PTR<Animation2D> TempleCloudAni = m_TempleCloud->Component_Add<Animation2D>();

	// 하얀 구름
	m_WhiteCloud = GetScene()->GameObject_Create(L"TimeTemple_WhiteCloud");
	m_WhiteCloud->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 299.0f));
	m_WhiteCloud->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_WhiteCloud->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer05)->SetSprite(L"TOG_WhiteCloud.png", 0);
	My_PTR<Animation2D> WhiteCloudAni = m_WhiteCloud->Component_Add<Animation2D>();

	// 가까운 구름
	m_Cloud = GetScene()->GameObject_Create(L"TimeTemple_NearCloud");
	m_Cloud->GetTransform()->Local_Position(Vector4(-100.0f, 150.0f, 297.0f));
	m_Cloud->GetTransform()->Local_Scale(Vector4(1536.0f, 642.0f, 1.0f));
	m_Cloud->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer06)->SetSprite(L"TOG_Cloud.png", 0);
	My_PTR<Animation2D> NearCloudAni = m_Cloud->Component_Add<Animation2D>();
	NearCloudScript = m_Cloud->Component_Add<BG_PinkbinCloudMove>();

	// 충돌할 타일
	m_MoveTile = GetScene()->GameObject_Create(L"TimeTempleBoss_MoveTile");
	m_MoveTile->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 400.0f));
	m_MoveTile->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_MoveTile->Component_Add<SpriteRenderer2D>(RENDER_ORDER::COLTILE)->SetSprite(L"TOG_MoveTile.png", 0);

	// 신전 오브젝트
	m_TempleObject = GetScene()->GameObject_Create(L"TimeTemple_TempleObject");
	m_TempleObject->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 288.0f));
	m_TempleObject->GetTransform()->Local_Scale(Vector4(1788.0f, 850.0f, 1.0f));
	m_TempleObject->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07)->SetSprite(L"TOG_Object.png", 0);
	My_PTR<Animation2D> TempleObjectAni = m_TempleObject->Component_Add<Animation2D>();

}

void BossMonsterStageScene::UILoading()
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

void BossMonsterStageScene::CreateCamera()
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

void BossMonsterStageScene::PortalLoading()
{
	m_PrePortal = GetScene()->GameObject_Create(L"PrePortal_Binkbin");
	m_PrePortal->GetTransform()->Local_Position(Vector4(-700.0f, -148.0f, 10.0f, 1.0f));
	m_PrePortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PrePortalRenderer = m_PrePortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	PrePortalRenderer->SetSprite(L"Portal.png", 0);
	My_PTR<Animation2D> PrePortalAnim = m_PrePortal->Component_Add<Animation2D>();
	My_PTR<TownPortalAniScript> PrePortalAniScript = m_PrePortal->Component_Add<TownPortalAniScript>();
	m_PrePortalCol = m_PrePortal->Component_Add<RectCol2D>(L"BossPinkbin_PrePortalCol");
}

void BossMonsterStageScene::MonsterLoading()
{
	// 핑크빈 (리젠)
	m_BossPinkbin = GetScene()->GameObject_Create(L"PinkbinRegen");
	m_BossPinkbin->GetTransform()->Local_Position(Vector4(20.0f, -77.0f, 100.0f));
	m_BossPinkbin->GetTransform()->Local_Scale(Vector4(451.0f, 347.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PinkRenderer = m_BossPinkbin->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
	PinkRenderer->SetSprite(L"Pinkbin_Regen.png", 0);
	My_PTR<Animation2D> PinkAni = m_BossPinkbin->Component_Add<Animation2D>();


	My_PTR<PinkbinRegen> PinkScript = m_BossPinkbin->Component_Add<PinkbinRegen>();
	PinkScript->SetUpTilePos(false);
	PinkScript->SetFloor(m_MoveTile);
	PinkScript->SetPlayer(m_GamePlayer);
	PinkScript->SetBackground(m_Background->GetTransform()->Local_Scale());
}

void BossMonsterStageScene::PinkbinSet()
{
	if (nullptr != m_BossPinkbin->GetComponent<PinkbinRegen>()->GetPinkbin())
	{
		m_ConnectPinkbin = true;
		m_RealPinkbin = m_BossPinkbin->GetComponent<PinkbinRegen>()->GetPinkbin();
		// 스킬 매니저에 핑크빈 넣어주기
		m_SkillManager->SetMonster(m_BossPinkbin);

		My_PTR<GameObject> NewBossPinkbinColObj = GetScene()->GameObject_Create(L"PinkbinColObj");
		NewBossPinkbinColObj->SetParent(m_BossPinkbin->GetComponent<PinkbinRegen>()->GetPinkbin());
		NewBossPinkbinColObj->GetTransform()->Local_Position(Vector4(0.0f, -120.0f, 0.0f, 0.0f));
		NewBossPinkbinColObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_DamageSkinScript = NewBossPinkbinColObj->Component_Add<MonsterDamageSkin>();
		m_BossCol = NewBossPinkbinColObj->Component_Add<RectCol2D>(L"BossMonsterCol");
		m_BossCol->Local_SubScale(Vector4(100.0f, 100.0f, 0.0f, 0.0f));
		m_BossCol->PushCollisionEnter(&BossMonsterStageScene::Col_BossMonsterEnter, this);
		m_BossCol->PushCollisionStay(&BossMonsterStageScene::Col_BossMonsterStay, this);
		m_BossCol->PushCollisionExit(&BossMonsterStageScene::Col_BossMonsterExit, this);
	}
}

void BossMonsterStageScene::BossHPBarLoading()
{
	//WEATHER_SNOW
	m_BossHPBarBase = GetScene()->GameObject_Create(L"BossHPBarBase");
	m_BossHPBarBase->GetTransform()->Local_Position(Vector4(0.0f, 250.0f, -5.0f, 1.0f));
	m_BossHPBarBase->GetTransform()->Local_Scale(Vector4(801.0f, 40.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> BaseHPBarRenderer = m_BossHPBarBase->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
	BaseHPBarRenderer->SetSprite(L"Boss_PinkbinHPBar.png", 0);

	m_BossHPBarFirstLine = GetScene()->GameObject_Create(L"BossHPBarFirstLine");
	m_BossHPBarFirstLine->GetTransform()->Local_Position(Vector4(-400.0f, 260.0f, -4.0f, 1.0f));
	m_BossHPBarFirstLine->GetTransform()->Local_Scale(Vector4(800.0f, 10.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> FirstLineRenderer = m_BossHPBarFirstLine->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PINKBIN_FIRSTLINE);
	FirstLineRenderer->SetSprite(L"boss_will_hp.gauge.1.png", 0);
	My_PTR<BossMonster_HP> FirstLineScript = m_BossHPBarFirstLine->Component_Add<BossMonster_HP>();
	FirstLineScript->SetHP(20000);			// 추후 수정 핑크빈이랑 연결해줘야함

	m_BossHPBarSecondLine = GetScene()->GameObject_Create(L"BossHPBarSecondLine");
	m_BossHPBarSecondLine->GetTransform()->Local_Position(Vector4(-400.0f, 260.0f, -3.0f, 1.0f));
	m_BossHPBarSecondLine->GetTransform()->Local_Scale(Vector4(800.0f, 10.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> SecondLineRenderer = m_BossHPBarSecondLine->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PINKBIN_SECONDLINE);
	SecondLineRenderer->SetSprite(L"boss_will_hp.gauge.2.png", 0);
	My_PTR<BossMonster_HP> SecondLineScript = m_BossHPBarSecondLine->Component_Add<BossMonster_HP>();
	SecondLineScript->SetHP(20000);			// 추후 수정 핑크빈이랑 연결해줘야함


	m_BossHPBarGray = GetScene()->GameObject_Create(L"BossHPBarGrayLine");
	m_BossHPBarGray->GetTransform()->Local_Position(Vector4(0.0f, 260.0f, -3.0f, 1.0f));
	m_BossHPBarGray->GetTransform()->Local_Scale(Vector4(800.0f, 10.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> GrayLineRenderer = m_BossHPBarGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PINKBIN_GRAYLINE);
	GrayLineRenderer->SetSprite(L"boss_will_hp.gauge.0.png", 0);


	// HP바 연결
	My_PTR<GameObject> BossPinkbinHPBarScriptObj = GetScene()->GameObject_Create(L"HPBarScriptObj");
	BossPinkbinHPBarScriptObj->GetTransform()->Local_Position(Vector4::Zero);
	BossPinkbinHPBarScriptObj->GetTransform()->Local_Scale(Vector4::Zero);
	m_BossHPScript = BossPinkbinHPBarScriptObj->Component_Add<PinkbinHPBar>();
	m_BossHPScript->SetFirstLineScript(FirstLineScript);
	m_BossHPScript->SetSecondLineScript(SecondLineScript);
}

void BossMonsterStageScene::CameraScriptLoading()
{
	m_CamScript = m_MainCamera->Component_Add<InGameCamera>();
	m_CamScript->SetCurMap(m_Background);
	m_CamScript->SetPlayer(m_GamePlayer);
}

void BossMonsterStageScene::PlayerLoading()
{
	// 캐릭터 부모
	m_GamePlayer = GetScene()->GameObject_Create(L"Player");
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_GamePlayer->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 1.0f));

	m_PlayerCol = m_GamePlayer->Component_Add<RectCol2D>(L"Player");
	m_PlayerCol->Local_SubPosition(Vector4(0.0f, -24.0f, 0.0f, 0.0f));
	m_PlayerCol->Local_SubScale(Vector4(60.0f, 80.0f, 1.0f, 1.0f));
	m_PlayerCol->PushCollisionEnter(&BossMonsterStageScene::Col_PlayerEnter, this);
	m_PlayerCol->PushCollisionStay(&BossMonsterStageScene::Col_PlayerStay, this);
	m_PlayerCol->PushCollisionExit(&BossMonsterStageScene::Col_PlayerExit, this);

	m_PlayerMoveScript = m_GamePlayer->Component_Add<PlayerMove>();
	m_PlayerMoveScript->AddFloor(m_MoveTile);
	m_PlayerMoveScript->SetSoundPlayer(m_EffectSoundPlayer);
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
}


void BossMonsterStageScene::CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z)
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

void BossMonsterStageScene::InventoryLoading()
{
	// 기본 오브젝트들
	m_InventoryEmptyObj = GetScene()->GameObject_Create(L"ChewChewIsland_InvenEmtpyObj");
	m_InventoryEmptyObj->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	m_InventoryEmptyObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_InvenMoveObj = GetScene()->GameObject_Create(L"ChewChewIsland_InventoryMoveObj");
	m_InvenMoveObj->SetParent(m_InventoryEmptyObj);
	m_InvenMoveObj->GetTransform()->Local_Position(Vector4(-20.0f, 154.0f, 1.0f, 1.0f));
	m_InvenMoveObj->GetTransform()->Local_Scale(Vector4(130.0f, 21.0f, 1.0f, 1.0f));

	InvenMoveObjectScript = m_InvenMoveObj->Component_Add<Inven_MoveScript>();				// 움직이는 애 스크립트
	InvenMoveObjectScript->SetCamera(m_UICameraCom);
	InvenMoveObjectScript->SetMoveObject(m_InventoryEmptyObj);

	InvenMoveCol = m_InvenMoveObj->Component_Add<RectCol2D>(L"Chu_InventoryMoveObjCol");		// 움직이는 애 충돌체
	InvenMoveCol->SetUICamera(m_UICameraCom);

	m_InvenBase = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBaseObj");
	m_InvenBase->SetParent(m_InventoryEmptyObj);
	m_InvenBase->GetTransform()->Local_Position(Vector4(1.0f, 1.0f, -1.0f, 1.0f));
	m_InvenBase->GetTransform()->Local_Scale(Vector4(172.0f, 335.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBaseRenderer = m_InvenBase->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBaseRenderer->SetSprite(L"Inven_Base.png", 0);

	m_InvenBtClose = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtClose");
	m_InvenBtClose->SetParent(m_InventoryEmptyObj);
	m_InvenBtClose->GetTransform()->Local_Position(Vector4(70.0f, 154.0f, -2.0f, 1.0f));
	m_InvenBtClose->GetTransform()->Local_Scale(Vector4(13.0f, 13.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtCloseRenderer = m_InvenBtClose->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtCloseRenderer->SetSprite(L"Inven_CloseBt.png", 0);
	My_PTR<Animation2D> InvenBtCloseAni = m_InvenBtClose->Component_Add<Animation2D>();
	InvenBtCloseAniScript = m_InvenBtClose->Component_Add<Inven_BtAnimation>();
	InvenBtCloseCol = m_InvenBtClose->Component_Add<RectCol2D>(L"Chu_InvenBtCloseCol");
	InvenBtCloseCol->SetUICamera(m_UICameraCom);


	// 하위 버튼들
	m_InvenBtDisassemble = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtDisassemble");
	m_InvenBtDisassemble->SetParent(m_InventoryEmptyObj);
	m_InvenBtDisassemble->GetTransform()->Local_Position(Vector4(-65.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtDisassemble->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtDisassembleRenderer = m_InvenBtDisassemble->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtDisassembleRenderer->SetSprite(L"Inven_DisassembleBt.png", 0);
	My_PTR<Animation2D> InvenBtDisassembleAni = m_InvenBtDisassemble->Component_Add<Animation2D>();
	InvenBtDisassembleAniScript = m_InvenBtDisassemble->Component_Add<Inven_BtAnimation>();
	InvenBtDisassembleCol = m_InvenBtDisassemble->Component_Add<RectCol2D>(L"Chu_InvenBtDisassembleCol");
	InvenBtDisassembleCol->SetUICamera(m_UICameraCom);

	m_InvenBtExtract = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtExtract");
	m_InvenBtExtract->SetParent(m_InventoryEmptyObj);
	m_InvenBtExtract->GetTransform()->Local_Position(Vector4(-39.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtExtract->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtExtractRenderer = m_InvenBtExtract->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtExtractRenderer->SetSprite(L"Inven_ExtractBt.png", 0);
	My_PTR<Animation2D> InvenBtExtractAni = m_InvenBtExtract->Component_Add<Animation2D>();
	InvenBtExtractAniScript = m_InvenBtExtract->Component_Add<Inven_BtAnimation>();
	InvenBtExtractCol = m_InvenBtExtract->Component_Add<RectCol2D>(L"Chu_InvenBtExtractCol");
	InvenBtExtractCol->SetUICamera(m_UICameraCom);

	m_InvenBtAppraise = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtAppraise");
	m_InvenBtAppraise->SetParent(m_InventoryEmptyObj);
	m_InvenBtAppraise->GetTransform()->Local_Position(Vector4(-13.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtAppraise->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtAppraise = m_InvenBtAppraise->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtAppraise->SetSprite(L"Inven_AppraiseBt.png", 0);
	My_PTR<Animation2D> InvenBtAppraiseAni = m_InvenBtAppraise->Component_Add<Animation2D>();
	InvenBtAppraiseAniScript = m_InvenBtAppraise->Component_Add<Inven_BtAnimation>();
	InvenBtAppraiseCol = m_InvenBtAppraise->Component_Add<RectCol2D>(L"Chu_InvenBtAppraiseCol");
	InvenBtAppraiseCol->SetUICamera(m_UICameraCom);

	m_InvenBtPot = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtPot");
	m_InvenBtPot->SetParent(m_InventoryEmptyObj);
	m_InvenBtPot->GetTransform()->Local_Position(Vector4(13.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtPot->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtPote = m_InvenBtPot->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtPote->SetSprite(L"Inven_PotBt.png", 0);

	m_InvenBtToad = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtToad");
	m_InvenBtToad->SetParent(m_InventoryEmptyObj);
	m_InvenBtToad->GetTransform()->Local_Position(Vector4(39.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtToad->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtToad = m_InvenBtToad->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtToad->SetSprite(L"Inven_ToadBt.png", 0);
	My_PTR<Animation2D> InvenBtToadAni = m_InvenBtToad->Component_Add<Animation2D>();
	InvenBtToadAniScript = m_InvenBtToad->Component_Add<Inven_BtAnimation>();
	InvenBtToadCol = m_InvenBtToad->Component_Add<RectCol2D>(L"Chu_InvenBtToadCol");
	InvenBtToadCol->SetUICamera(m_UICameraCom);

	m_InvenBtUpgrade = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtUpgrade");
	m_InvenBtUpgrade->SetParent(m_InventoryEmptyObj);
	m_InvenBtUpgrade->GetTransform()->Local_Position(Vector4(65.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtUpgrade->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtUpgrade = m_InvenBtUpgrade->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtUpgrade->SetSprite(L"Inven_UpgradeBt.png", 0);
	My_PTR<Animation2D> InvenBtUpgradeAni = m_InvenBtUpgrade->Component_Add<Animation2D>();
	InvenBtUpgradeAniScript = m_InvenBtUpgrade->Component_Add<Inven_BtAnimation>();
	InvenBtUpgradeCol = m_InvenBtUpgrade->Component_Add<RectCol2D>(L"Chu__InvenBtUpgradeCol");
	InvenBtUpgradeCol->SetUICamera(m_UICameraCom);

	// 장비 소비 기타 설치 캐시
	m_InvenBtEquip = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtEquip");
	m_InvenBtEquip->SetParent(m_InventoryEmptyObj);
	m_InvenBtEquip->GetTransform()->Local_Position(Vector4(-60.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtEquip->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtEquipRenderer = m_InvenBtEquip->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtEquipRenderer->SetSprite(L"Inven_Kind_Equip.png", 0);
	My_PTR<Animation2D> InvenBtEquipAni = m_InvenBtEquip->Component_Add<Animation2D>();
	InvenBtEquipAniScript = m_InvenBtEquip->Component_Add<Inven_BtKind>();
	InvenBtEquipAniScript->On();
	InvenBtEquipCol = m_InvenBtEquip->Component_Add<RectCol2D>(L"Chu_InvenBtEquipCol");
	InvenBtEquipCol->SetName(L"InvenBtEquipCol");
	InvenBtEquipCol->SetUICamera(m_UICameraCom);

	m_InvenBtConsum = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtComsum");
	m_InvenBtConsum->SetParent(m_InventoryEmptyObj);
	m_InvenBtConsum->GetTransform()->Local_Position(Vector4(-29.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtConsum->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtComsumRenderer = m_InvenBtConsum->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtComsumRenderer->SetSprite(L"Inven_Kind_Comsum.png", 0);
	My_PTR<Animation2D> InvenBtComsumAni = m_InvenBtConsum->Component_Add<Animation2D>();
	InvenBtComsumAniScript = m_InvenBtConsum->Component_Add<Inven_BtKind>();
	InvenBtComsumAniScript->Off();
	InvenBtComsumCol = m_InvenBtConsum->Component_Add<RectCol2D>(L"Chu_InvenBtComsumCol");
	InvenBtComsumCol->SetUICamera(m_UICameraCom);

	m_InvenBtEtc = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtEtc");
	m_InvenBtEtc->SetParent(m_InventoryEmptyObj);
	m_InvenBtEtc->GetTransform()->Local_Position(Vector4(2.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtEtc->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtEtcRenderer = m_InvenBtEtc->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtEtcRenderer->SetSprite(L"Inven_Kind_Etc.png", 0);
	My_PTR<Animation2D> InvenBtEtcAni = m_InvenBtEtc->Component_Add<Animation2D>();
	InvenBtEtcAniScript = m_InvenBtEtc->Component_Add<Inven_BtKind>();
	InvenBtEtcAniScript->Off();
	InvenBtEtcCol = m_InvenBtEtc->Component_Add<RectCol2D>(L"Chu_InvenBtEtcCol");
	InvenBtEtcCol->SetUICamera(m_UICameraCom);

	m_InvenBtInstall = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtInstall");
	m_InvenBtInstall->SetParent(m_InventoryEmptyObj);
	m_InvenBtInstall->GetTransform()->Local_Position(Vector4(33.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtInstall->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtInstallRenderer = m_InvenBtInstall->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtInstallRenderer->SetSprite(L"Inven_Kind_Install.png", 0);
	My_PTR<Animation2D> InvenBtInstallAni = m_InvenBtInstall->Component_Add<Animation2D>();
	InvenBtInstallAniScript = m_InvenBtInstall->Component_Add<Inven_BtKind>();
	InvenBtInstallAniScript->Off();
	InvenBtInstallCol = m_InvenBtInstall->Component_Add<RectCol2D>(L"Chu_InvenBtInstallCol");
	InvenBtInstallCol->SetUICamera(m_UICameraCom);

	m_InvenBtCash = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtCash");
	m_InvenBtCash->SetParent(m_InventoryEmptyObj);
	m_InvenBtCash->GetTransform()->Local_Position(Vector4(64.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtCash->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtCashRenderer = m_InvenBtCash->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtCashRenderer->SetSprite(L"Inven_Kind_Cash.png", 0);
	My_PTR<Animation2D> InvenBtCashAni = m_InvenBtCash->Component_Add<Animation2D>();
	InvenBtCashAniScript = m_InvenBtCash->Component_Add<Inven_BtKind>();
	InvenBtCashAniScript->Off();
	InvenBtCashCol = m_InvenBtCash->Component_Add<RectCol2D>(L"Chu_InvenBtCashCol");
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

void BossMonsterStageScene::InventoryCreate()
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

void BossMonsterStageScene::InventorySlotCreate()
{
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
			My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"Chu_InventorySlot_Consume");
			EmptyItemCol->SetUICamera(m_UICameraCom);
			My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
			EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::CONSUME);

			m_InvenItemConsum.push_back(EmptyItem);
			InvenItemConSumCol.push_back(EmptyItemCol);
		}
	}
}

void BossMonsterStageScene::InventoryOff()
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

void BossMonsterStageScene::InventoryOn()
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

void BossMonsterStageScene::InventorySlotOff(std::vector<GameObject*> _Slot)
{
	for (size_t i = 0; i < _Slot.size(); i++)
	{
		//_Slot[i]->Update_Off();
		_Slot[i]->GetComponent<RectCol2D>()->Update_Off();
		_Slot[i]->GetComponent<Inven_Info>()->Number_Off();
		_Slot[i]->GetComponent<Inven_Info>()->RenderOff();
	}
}

void BossMonsterStageScene::InventorySlotOn(std::vector<GameObject*> _Slot)
{
	for (size_t i = 0; i < _Slot.size(); i++)
	{
		//_Slot[i]->Update_On();
		_Slot[i]->GetComponent<Inven_Info>()->Number_On();
		_Slot[i]->GetComponent<Inven_Info>()->RenderOn();
		_Slot[i]->GetComponent<RectCol2D>()->Update_On();
	}

}

void BossMonsterStageScene::ClickItemInfo(GameObject* _ItemObj)
{
	m_ClickItemInfo = _ItemObj;
}

void BossMonsterStageScene::QuickSlotLoading()
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


// 돈
void BossMonsterStageScene::MoneyLoading()
{
	m_InvenMoneyManagerObject = GetScene()->GameObject_Create(L"ChewChewIsland_Inven_MoenyManager");
	m_InvenMoneyManagerObject->SetParent(m_InventoryEmptyObj);
	m_InvenMoneyManagerObject->GetTransform()->Local_Position(Vector4(35.5f, -108.0f, 0.0f, 0.0f));
	m_InvenMoneyManagerObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_InvenMoenyManagerScript = m_InvenMoneyManagerObject->Component_Add<MesoManager>();
}

// 마우스
void BossMonsterStageScene::MouseLoading()
{
	//Input::Create_Key(L"MouseLeftButton", VK_LBUTTON);


	m_Mouse = GetScene()->GameObject_Create(L"ChewChewIsland_MouseCursor");
	m_Mouse->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -15.0f, 1.0f));
	m_Mouse->GetTransform()->Local_Scale(Vector4(30.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> MouseCursorRenderer = m_Mouse->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_MOUSECURSOR);
	MouseCursorRenderer->SetSprite(L"MouseCursor.png", 0);
	My_PTR<Animation2D> MouseCursorAni = m_Mouse->Component_Add<Animation2D>();
	MouseScript = m_Mouse->Component_Add<MouseCursorScript>();
	MouseScript->SetCamera(m_UICameraCom);

	CursorCol = m_Mouse->Component_Add<RectCol2D>(L"Chu_CursorCol");
	CursorCol->Local_SubPosition(Vector4(-10.0f, 10.0f, 1.0f, 1.0f));
	CursorCol->Local_SubScale(Vector4(3.0f, 3.0f, 1.0f, 1.0f));
	CursorCol->SetName(L"CursorCol");
	CursorCol->SetUICamera(m_UICameraCom);

	CursorCol->PushCollisionEnter(&BossMonsterStageScene::Col_InventoryEnter, this);
	CursorCol->PushCollisionStay(&BossMonsterStageScene::Col_InventoryStay, this);
	CursorCol->PushCollisionExit(&BossMonsterStageScene::Col_InventoryExit, this);


	// 인벤토리에서 마우스 클릭시 아이템 이동하기 위한.. (아이템 이미지를 보여주기 위한 오브젝트 초기화)
	m_InvenClickMouseImage = GetScene()->GameObject_Create(L"ChewChewIsland_Inven_ClickItemImage");
	m_InvenClickMouseImage->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -8.0f, 1.0f));
	m_InvenClickMouseImage->GetTransform()->Local_Scale(Vector4(29.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenClickImageRenderer = m_InvenClickMouseImage->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	InvenClickImageRenderer->SetSprite(L"EmptyItem.png", 0);
	//InvenClickImageRenderer->SetSprite(L"RedPoiton.png", 0);
	InvenClickImageScript = m_InvenClickMouseImage->Component_Add<Inven_MouseItem>();
	InvenClickImageScript->SetMouseCamera(m_UICameraCom);
}

// UI 충돌
void BossMonsterStageScene::Col_InventoryEnter(Collision* _This, Collision* _Other)
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

	}
}

void BossMonsterStageScene::Col_InventoryStay(Collision* _This, Collision* _Other)
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
				SoundPlay_Effect(L"btClick.mp3");
				Vector4 CloseMoveInven = Vector4(0.0f, 1000.0f, 0.0f, 0.0f);

				InvenBtCloseAniScript->SetAnimation(L"Pressed");
				InventoryOff();
			}
		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtDisassembleAniScript->SetAnimation(L"Pressed");
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
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtAppraiseAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtToadAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtUpgradeAniScript->SetAnimation(L"Pressed");
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

	}
}


void BossMonsterStageScene::Col_InventoryExit(Collision* _This, Collision* _Other)
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


	}
}

// 몬스터 충돌함수
void BossMonsterStageScene::Col_BossMonsterEnter(Collision* _This, Collision* _Other)
{

	if (0 == m_BossHPScript->GetHP())
	{
		return;
	}

	float BEzValue = 10.0f;
	std::wstring SkillName = _Other->GetGroupName();

	if (L"Skill_BlizzardCol" == SkillName)
	{
		SoundPlay_Effect(L"Use_Blizzard.mp3");
		// 랜덤한 데미지 ( 150 ~ 300 );
		int NewDamage = (rand() % 150) + 5267;

		// HP감소
		m_BossHPScript->Damage(NewDamage);
		m_DamageSkinScript->Create_DamageSkin(NewDamage);

		// 이펙트
		My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
		NewHitEffect->GetTransform()->Local_Position(m_BossCol->GetTransform()->Local_Position());
		NewHitEffect->GetTransform()->Local_Scale(Vector4(103.0f, 126.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
		NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
		My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
		My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
		NewHitEffectScript->SetMovePos(Vector4::Zero);
		NewHitEffectScript->SetSkillName(L"HE_BZ");
		--BEzValue;
	}
	else if (L"Skill_BlazingExtinction" == SkillName)
	{
		SoundPlay_Effect(L"Use_BlazingExtinction.mp3");
		// 데미지 -> 스크립트랑 연결해준다.
		// 랜덤한 데미지
		int NewDamage = (rand() % 257) + 6324;
		// HP감소
		m_BossHPScript->Damage(NewDamage);
		m_DamageSkinScript->Create_DamageSkin(NewDamage);

		// 이펙트
		My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
		NewHitEffect->GetTransform()->Local_Position(m_BossCol->GetTransform()->Local_Position());
		NewHitEffect->GetTransform()->Local_Scale(Vector4(405.0f, 308.0f, BEzValue, 1.0f));
		My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
		NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
		My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
		My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
		NewHitEffectScript->SetMovePos(Vector4(0.0f, 50.0f, 0.0f, 0.0f));
		NewHitEffectScript->SetSkillName(L"HE_BE");
		--BEzValue;
	}
	else if (L"Skill_Cataclysm" == SkillName)
	{
		SoundPlay_Effect(L"Use_CC2.mp3");
		// 데미지 -> 스크립트랑 연결해준다.
		// 랜덤한 데미지
		int NewDamage = (rand() % 999) + 82179;
		// HP감소
		m_BossHPScript->Damage(NewDamage);
		m_DamageSkinScript->Create_DamageSkin(NewDamage);

		// 이펙트
		My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
		NewHitEffect->GetTransform()->Local_Position(m_BossCol->GetTransform()->Local_Position());
		NewHitEffect->GetTransform()->Local_Scale(Vector4(260.0f, 242.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF12);
		NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
		My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
		My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
		NewHitEffectScript->SetMovePos(Vector4(0.0f, 20.0f, BEzValue, 0.0f));
		NewHitEffectScript->SetSkillName(L"HE_CC");
		--BEzValue;
	}
	else if (L"Skill_PoisonMist" == SkillName)
	{
		// 데미지 -> 스크립트랑 연결해준다.
		// 랜덤한 데미지
		int NewDamage = (rand() % 200) + 2957;
		// HP감소
		m_BossHPScript->Damage(NewDamage);
		m_DamageSkinScript->Create_DamageSkin(NewDamage);

		// 이펙트
		My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
		NewHitEffect->GetTransform()->Local_Position(m_BossCol->GetTransform()->Local_Position());
		NewHitEffect->GetTransform()->Local_Scale(Vector4(260.0f, 242.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
		NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
		My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
		My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
		NewHitEffectScript->SetMovePos(Vector4::Zero);
		NewHitEffectScript->SetSkillName(L"HE_PM");
		--BEzValue;
	}
	else if (L"Skill_SoulPenetration" == SkillName)
	{
		SoundPlay_Effect(L"Use_03.mp3");
		// 데미지 -> 스크립트랑 연결해준다.
		// 랜덤한 데미지
		int NewDamage = (rand() % 200) + 3957;
		// HP감소
		m_BossHPScript->Damage(NewDamage);
		m_DamageSkinScript->Create_DamageSkin(NewDamage);

		// 이펙트
		My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
		NewHitEffect->GetTransform()->Local_Position(m_BossCol->GetTransform()->Local_Position());
		NewHitEffect->GetTransform()->Local_Scale(Vector4(242.0f, 242.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF12);
		NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
		My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
		My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
		NewHitEffectScript->SetMovePos(Vector4(0.0f, 10.0f, BEzValue, 0.0f));
		NewHitEffectScript->SetSkillName(L"HE_SP");
		--BEzValue;
	}

	if (0 == m_BossHPScript->GetHP())
	{
		m_BossPinkbin->GetComponent<PinkbinRegen>()->PinkbinDeath();
		return;
	}
}

void BossMonsterStageScene::Col_BossMonsterStay(Collision* _This, Collision* _Other)
{

}

void BossMonsterStageScene::Col_BossMonsterExit(Collision* _This, Collision* _Other)
{

}

// 플레이어 충돌
void BossMonsterStageScene::Col_PlayerEnter(Collision* _This, Collision* _Other)
{
	if (m_PlayerCol == _This)
	{
		std::wstring OtherName = _Other->GetGroupName();

		if (L"Pinkbin_Attack01" == OtherName)
		{
			// 캐릭터한테 이펙트 쏴주기
			// 이펙트
			My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
			Vector4 CharacterPos = m_GamePlayer->GetTransform()->Local_Position();
			CharacterPos.z = -1.0f;
			NewHitEffect->GetTransform()->Local_Position(CharacterPos);
			NewHitEffect->GetTransform()->Local_Scale(Vector4(97.0f, 107.0f, 1.0f, 1.0f));
			My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::PLAYER_CHILD);
			NewHitEffectRenderer->SetSprite(L"Attack1_Hit.png", 0);
			My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
			My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
			NewHitEffectScript->SetMovePos(Vector4(0.0f, 10.0f, CharacterPos.z, 0.0f));
			NewHitEffectScript->SetSkillName(L"BOSS_01");
		}
	}
}
void BossMonsterStageScene::Col_PlayerStay(Collision* _This, Collision* _Other)
{
	// 포탈 관련 + 핑크빈 이펙트
	if (m_PlayerCol == _This)
	{
		if (m_PrePortalCol == _Other)
		{
			if (true == Input::Down(L"Key_Space"))
			{
				// 씬 옮기기 - 에스페라
				// 정보들 옮겨주기(내일추가)
				// 정보들을 옮겨준 후에 씬을 이동시킨다

				// 인벤토리 정보
				MapleGlobalValue::SetInvenItemConsume(m_InvenItemConsum);
				MapleGlobalValue::SetQuickSlotVector(m_QuickSlotVector);
				Scene::Scene_Change(L"PinkbinScene");

			}
		}

	}

	// 돈관련
	//for (size_t i = 0; i < m_ItemColVector.size(); ++i)
	//{
	//	if (_Other == m_ItemColVector[i])
	//	{
	//		if (true == Input::Down(L"Key_Space"))
	//		{
	//			// 돈 먹기
	//			int randMoney = (rand() % 300) + 10000;
	//			m_InvenMoenyManagerScript->AddMeso(randMoney);
	//			m_ItemObjVector[i]->GetComponent<ItemMoveScript>()->SetDrop(true);
	//			m_ItemObjVector.erase(m_ItemObjVector.begin() + i);
	//			m_ItemColVector.erase(m_ItemColVector.begin() + i);
	//		}
	//	}
	//}
}

void BossMonsterStageScene::Col_PlayerExit(Collision* _This, Collision* _Other)
{

}

//  충돌체 링크
void BossMonsterStageScene::CollisionNameLink()
{
	GetScene()->m_CollisionManager.Link(L"Player", L"Monster");
	GetScene()->m_CollisionManager.Link(L"Player", L"Money");

	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InventoryMoveObjCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtCloseCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtDisassembleCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtExtractCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtAppraiseCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtToadCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtUpgradeCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtEquipCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtComsumCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtEtcCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtInstallCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtCashCol");

	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InventorySlot_Consume");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_QuickSlotCol");

	// 포탈  - Player
	GetScene()->m_CollisionManager.Link(L"Player", L"Town_PrePortalCol");
	GetScene()->m_CollisionManager.Link(L"Player", L"Town_NextPortalCol");

	GetScene()->m_CollisionManager.Link(L"Player", L"Pinkbin_Attack01");
	GetScene()->m_CollisionManager.Link(L"Player", L"Pinkbin_Attack02");
	GetScene()->m_CollisionManager.Link(L"Player", L"Pinkbin_Attack03");
	GetScene()->m_CollisionManager.Link(L"Player", L"Pinkbin_Attack04");

	// 몬스터랑 스킬
	GetScene()->m_CollisionManager.Link(L"BossMonsterCol", L"Skill_BlizzardCol");
	GetScene()->m_CollisionManager.Link(L"BossMonsterCol", L"Skill_BlazingExtinction");
	GetScene()->m_CollisionManager.Link(L"BossMonsterCol", L"Skill_Cataclysm");
	GetScene()->m_CollisionManager.Link(L"BossMonsterCol", L"Skill_PoisonMist");
	GetScene()->m_CollisionManager.Link(L"BossMonsterCol", L"Skill_SoulPenetration");

	// 포탈  - Player
	GetScene()->m_CollisionManager.Link(L"Player", L"ChewChewIsland_PrePortalCol");
	GetScene()->m_CollisionManager.Link(L"Player", L"ChewChewIsland_NextPortalCol");
}

void BossMonsterStageScene::SoundPlay_BGM(std::wstring _SoundFileName)
{
	m_BGMPlayer->Play_BGM(_SoundFileName.c_str());
}

void BossMonsterStageScene::SoundPlay_Effect(std::wstring _SoundFileName)
{
	m_EffectSoundPlayer->Play(_SoundFileName.c_str());
}

//
void BossMonsterStageScene::BGMSoundLoading()
{
	// 사운드플레이
	m_BGMEmptyObject = GetScene()->GameObject_Create(L"BGMPlayer");
	m_BGMEmptyObject->GetTransform()->Local_Position(Vector4::Zero);
	m_BGMEmptyObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_BGMPlayer = m_BGMEmptyObject->Component_Add<SoundPlayer>();
}

void BossMonsterStageScene::EffectSoundLoading()
{
	m_EffectSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_EffectSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_EffectSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_EffectSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}